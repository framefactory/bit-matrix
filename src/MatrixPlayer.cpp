/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "MatrixPlayer.h"

#include "effects/MidiEffect.h"
#include "effects/StaticKeyEffect.h"
#include "effects/IndexEffect.h"

#include "net/MidiController.h"
#include "matrix/MAX7219Matrix.h"

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
    //Serial.println(message.toString().c_str());

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
    Serial.printf("RPN %d, %d\n", param, value);
}

void MatrixPlayer::onNRPN(uint16_t param, uint16_t value)
{
    Serial.printf("NRPN %d, %d\n", param, value);
}

void MatrixPlayer::dispatchNote(const MidiMessage& message)
{
    uint8_t ch = message.channel();

    if (ch == 0) {
        execSystemCommand(message);
        return;
    }

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

    //Serial.printf("[MatrixPlayer] effects in map: %d\n", _effectMap.size());
    //Serial.printf("[MatrixPlayer] effects in layer: %d\n", _layer.size());
    // Serial.printf("[MatrixPlayer] free heap size: %d, largest block: %d\n",
    //     heap_caps_get_free_size(MALLOC_CAP_8BIT),
    //     heap_caps_get_largest_free_block(MALLOC_CAP_8BIT)    
    // );
}

void MatrixPlayer::dispatchController(const MidiMessage& message)
{
    uint8_t ch = message.channel();
    uint8_t value = message.value();

    switch(message.controller()) {
    case MidiController::Volume:
        _pUniverse->setBrightness(value / 12);
        _pUniverse->writeBrightness();
        break;

    case MidiController::SustainPedalSwitch:
        bool sustainEnabled = value >= 64;

        if (!sustainEnabled && _sustainEnabled[ch]) {
            for (auto entry : _effectMap[ch]) {
                if (!_pressedKeys[entry.first]) {
                    entry.second->stop(_timing);
                }
            }
        }

        _sustainEnabled[ch] = sustainEnabled;
        break;
    }
}

Effect* MatrixPlayer::createEffect(const MidiMessage& message)
{
    uint8_t note = message.note();
    uint8_t velocity = message.velocity();

    switch(message.channel()) {
        case 1:
            return new MidiEffect(message);
        case 2:
            return new StaticKeyEffect(message);
    }

    return nullptr;
}

void MatrixPlayer::execSystemCommand(const MidiMessage& message)
{
    // only handle Note On events
    if (message.status() == MidiStatus::NoteOff || message.velocity() == 0) {
        return;
    }

    uint8_t note = message.note();
    if (note == 0) {
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
    }
    else if (note == 1) {
        // shut down all matrices
        _pUniverse->writeRegister(MAX7219Matrix::Register::SHUTDOWN, 0x00);
    }
    else if (note >= 2 && note <= 11) {
        // adjust brightness
        _pUniverse->setBrightness(note - 2);
        _pUniverse->writeBrightness();
    }
    else if (note == 12) {
        Effect* pEffect = new IndexEffect();
        _layer.add(pEffect);
        pEffect->start(_timing);
    }
}