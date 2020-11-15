/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "MatrixPlayer.h"

#include "effects/IndexEffect.h"
#include "effects/BigEffect.h"
#include "effects/StaticKeyEffect.h"
#include "effects/ParticleEffect.h"
#include "effects/ShapeEffect.h"

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
    if (F_DEBUG) {
        Serial.printf("[MatrixPlayer] - MIDI message: %s\n", message.toString().c_str());
    }

    if (message.isChannelMessage() && message.channel() == 0) {
        execSystemCommand(message);
        return;
    }

    switch(message.status()) {
        case MidiStatus::NoteOn:
        case MidiStatus::NoteOff:
            dispatchNote(message);
            break;
        case MidiStatus::ControlChange:
            dispatchController(message);
            break;
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

    if (message.status() == MidiStatus::NoteOn && message.velocity() > 0) {
        if (!_pressedKeys[ch][note]) {
            _pressedKeys[ch][note] = true;
        
            Effect* pEffect = createEffect(message);
            if (pEffect) {
                _effectMap[ch].emplace(note, pEffect);
                _layer.add(pEffect);
                pEffect->start(_timing);
            }
        }
    }
    else if (_pressedKeys[ch][note]) {
        _pressedKeys[ch][note] = false;
        
        if (!_sustainEnabled[ch]) {
            auto range = _effectMap[ch].equal_range(note);
            for (auto it = range.first; it != range.second; ++it) {
                it->second->stop(_timing);
            }
            _effectMap[ch].erase(range.first, range.second);
        }
    }
}

void MatrixPlayer::dispatchController(const MidiMessage& message)
{
    uint8_t ch = message.channel();
    uint8_t value = message.value();
    double intPart, fracPart;

    switch(message.controller()) {
    case MidiController::Volume:
        _pUniverse->setMaxBrightness(value / 12);
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
                    it->second->stop(_timing);
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
}

Effect* MatrixPlayer::createEffect(const MidiMessage& message)
{
    //uint8_t note = message.note();
    //uint8_t velocity = message.velocity();

    switch(message.channel()) {
        case 1:
            return new BigEffect(message);
        case 2:
            return new StaticKeyEffect(message);
        case 3:
            return new ParticleEffect(message);
        case 4:
            return new ShapeEffect(message);
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
                entry.second->stop(_timing);
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
        // play test pattern
        Effect* pEffect = new IndexEffect();
        _layer.add(pEffect);
        pEffect->start(_timing);
        break;
    }
}