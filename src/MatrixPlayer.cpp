/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "MatrixPlayer.h"

#include "effects/TestPatternEffect.h"
#include "effects/BigEffect.h"
#include "effects/StaticKeyEffect.h"
#include "effects/ParticleEffect.h"
#include "effects/ShapeEffect.h"
#include "effects/AnimationEffect.h"

#include "net/MidiController.h"
#include "matrix/MAX7219Matrix.h"

#include <cmath>

F_USE_NAMESPACE

MatrixPlayer::MatrixPlayer(MidiPort* pPort, MAX7219Universe* pUniverse, Bitmap* pTarget) :
    _pMidiPort(pPort),
    _pUniverse(pUniverse),
    _pTarget(pTarget),
    _layer(pTarget, Bitmap::BlendOp::Replace)
{
    for (size_t c = 0; c < 16; ++c) {
        _sustainEnabled[c] = false;
        for (size_t i = 0; i < 128; ++i) {
            _pressedKeys[c][i] = false;
        }
    }

    _pMidiPort->setListener(this);
    _layer.setAutoClear(true);
}

MatrixPlayer::~MatrixPlayer()
{
    _pMidiPort->setListener(nullptr);
}

void MatrixPlayer::update()
{
    _timing.update();
    _pMidiPort->dispatch();

    _layer.render(_pTarget, _timing);
    _layer.removeDisabled();
}

void MatrixPlayer::onMidiMessage(const MidiMessage& message)
{
    // if (F_DEBUG) {
    //     Serial.printf("[MatrixPlayer] - MIDI message: %s\n", message.toString().c_str());
    // }

    switch(message.status()) {
        case MidiStatus::NoteOn:
        case MidiStatus::NoteOff:
            dispatchNote(message);
            break;
        case MidiStatus::ControlChange:
            dispatchController(message);
            break;
        case MidiStatus::PitchBend:
            _timing.pitchBend = (message.data0() + (message.data1() << 7) - 0x2000) / 8192.0f;
            break;
    }
}

void MatrixPlayer::onSysEx(const std::string& sysEx)
{
    Serial.printf("[MatrixPlayer] - onSysEx, length: %d\n", sysEx.length());
    for (auto c : sysEx) {
        Serial.printf("%d, ", (int)c);
    }    
    Serial.println();

    uint8_t command = sysEx[3];
    switch(command) {
        case 0: { // set text
            uint8_t id = sysEx[4] % 16;
            auto& text = _timing.text[id];
            text.x = sysEx[5];
            text.y = sysEx[6];
            text.text = sysEx.substr(7);
        } break;

        case 1: { // set pixels
            
        } break;
    }
}

void MatrixPlayer::onRPN(uint16_t param, uint16_t value)
{
    if (F_DEBUG) {
        Serial.printf("RPN %d, %d\n", param, value);
    }
}

void MatrixPlayer::onNRPN(uint16_t param, uint16_t value)
{
    if (F_DEBUG) {
        Serial.printf("NRPN %d, %d\n", param, value);
    }
}

void MatrixPlayer::dispatchNote(const MidiMessage& message)
{
    uint8_t ch = message.channel();
    uint8_t note = message.note();

    if (ch == 0) {
        execSystemCommand(message);
        return;
    }

    if (message.status() == MidiStatus::NoteOn && message.velocity() > 0) {
        if (!_pressedKeys[ch][note]) {
            _pressedKeys[ch][note] = true;
        
            MidiEffect* pEffect = createEffect(message);
            if (pEffect) {
                _effectMap[ch].emplace(note, pEffect);
                _layer.add(pEffect);
                pEffect->start(_timing, message);
            }
        }
    }
    else if (_pressedKeys[ch][note]) {
        _pressedKeys[ch][note] = false;
        
        if (!_sustainEnabled[ch]) {
            auto range = _effectMap[ch].equal_range(note);
            for (auto it = range.first; it != range.second; ++it) {
                it->second->stop(_timing, message);
            }
            _effectMap[ch].erase(range.first, range.second);
        }
    }
}

void MatrixPlayer::dispatchController(const MidiMessage& message)
{
    uint8_t ch = message.channel();
    uint8_t controller = message.controller();
    uint8_t value = message.value();
    double intPart, fracPart;

    switch(message.controller()) {
    case MidiController::Volume:
        _pUniverse->setMaxBrightness(value / 8);
        _pUniverse->writeBrightness();
        break;

    case MidiController::GeneralPurposeController1:
        fracPart = modf(_timing.tempo, &intPart);
        _timing.tempo = 60.0 + value + fracPart;
        if (F_DEBUG) {
            Serial.printf("[MatrixPlayer] - tempo: %f BPM\n", _timing.tempo);
        }
        break;

    case MidiController::GeneralPurposeController1_LSB:
        fracPart = modf(_timing.tempo, &intPart);
        _timing.tempo = intPart + value * 0.01;
        if (F_DEBUG) {
            Serial.printf("[MatrixPlayer] - tempo: %f BPM\n", _timing.tempo);
        }
        break;

    case MidiController::SustainPedalSwitch:
        bool sustainEnabled = value >= 64;

        if (!sustainEnabled && _sustainEnabled[ch]) {
            auto it = _effectMap[ch].begin();
            while (it != _effectMap[ch].end()) {
                if (!_pressedKeys[ch][it->first]) {
                    MidiMessage noteOff = it->second->startMessage();
                    noteOff.setBytes(MidiStatus::NoteOff + noteOff.channel(), noteOff.note(), 0);
                    it->second->stop(_timing, noteOff);
                    it = _effectMap[ch].erase(it);
                }
                else {
                    ++it;
                }
            }
        }

        _sustainEnabled[ch] = sustainEnabled;
        break;
    }

    if (controller >= MidiController::SoundController1 && controller <= MidiController::SoundController10) {
        _timing.control[controller - MidiController::SoundController1] = value / 127.0f;
    }
}

MidiEffect* MatrixPlayer::createEffect(const MidiMessage& message)
{
    //uint8_t note = message.note();
    //uint8_t velocity = message.velocity();

    switch(message.channel()) {
        case 1:
            return new BigEffect();
        case 2:
            return new StaticKeyEffect();
        case 3:
            return new ParticleEffect();
        case 4:
            return new ShapeEffect();
        case 5:
            return new AnimationEffect();
    }

    return nullptr;
}

void MatrixPlayer::execSystemCommand(const MidiMessage& message)
{
    // only handle Note On events
    if (message.status() != MidiStatus::NoteOn || message.velocity() == 0) {
        return;
    }

    uint8_t note = message.note();
    switch(note) {
    case 0:
        // terminate all running effects
        for (size_t ch = 0; ch < 16; ++ch) {
            for (auto entry : _effectMap[ch]) {
                //MidiMessage noteOff = entry.second->startMessage();
                //noteOff.setBytes(MidiStatus::NoteOff + noteOff.channel(), noteOff.note(), 0);
                //entry.second->stop(_timing, noteOff);
                entry.second->setEnabled(false);
            }
            _effectMap[ch].clear();
        }
        _layer.removeAll();

        // initialize all matrices
        _pUniverse->initialize();
        break;

    case 1:
        // shut down all matrices
        _pUniverse->writeRegister(MAX7219Matrix::Register::SHUTDOWN, 0x00);
        break;

    case 2:
        runTestPattern(0);
        break;
    case 3:
        runTestPattern(1);
        break;
    case 4:
        runTestPattern(2);
        break;
    }
}

void MatrixPlayer::runTestPattern(int pattern)
{
    Effect* pTestPattern = new TestPatternEffect(pattern);
    _layer.add(pTestPattern);
    pTestPattern->start(_timing);
}