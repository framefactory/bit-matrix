/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_BITMATRIX_MATRIXPLAYER_H
#define _ESP_BITMATRIX_MATRIXPLAYER_H

#include "app.h"
#include "MidiEffect.h"
#include "MidiTiming.h"

#include "matrix/MAX7219Universe.h"

#include "effect/Bitmap.h"
#include "effect/Layer.h"

#include "net/MidiPort.h"
#include "net/MidiListener.h"
#include "net/MidiMessage.h"

#include <map>

F_BEGIN_NAMESPACE

class MatrixPlayer : public MidiListener
{
public:
    MatrixPlayer(MidiPort* pPort, MAX7219Universe* pUniverse, Bitmap* pTarget);
    virtual ~MatrixPlayer();

    void update();

    const Timing& timing() const { return _timing; }

    virtual void onMidiMessage(const MidiMessage& message) override;
    virtual void onSysEx(const std::string& sysEx) override;
    virtual void onRPN(uint16_t param, uint16_t value) override;
    virtual void onNRPN(uint16_t param, uint16_t value) override;

protected:
    void dispatchController(const MidiMessage& message);
    void dispatchNote(const MidiMessage& message);
    MidiEffect* createEffect(const MidiMessage& message);
    void execSystemCommand(const MidiMessage& message);

private:
    MidiPort* _pMidiPort = nullptr;
    MAX7219Universe* _pUniverse = nullptr;
    Bitmap* _pTarget = nullptr;

    Layer _layer;
    MidiTiming _timing;

    bool _pressedKeys[16][128];
    bool _sustainEnabled[16];

    typedef std::multimap<uint8_t, MidiEffect*> effectMap_t;
    effectMap_t _effectMap[16];

    
};

F_END_NAMESPACE

#endif // _ESP_BITMATRIX_MATRIXPLAYER_H