/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_EFFECT_MATRIXPLAYER_H
#define _ESP_EFFECT_MATRIXPLAYER_H

#include "app.h"

#include "matrix/MAX7219Universe.h"

#include "effect/Bitmap.h"
#include "effect/Layer.h"
#include "effect/Timing.h"
#include "effect/Effect.h"

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

    Timing& timing() { return _timing; }

    virtual void onMidiMessage(const MidiMessage& message) override;
    virtual void onRPN(uint16_t param, uint16_t value) override;
    virtual void onNRPN(uint16_t param, uint16_t value) override;

protected:
    void dispatchController(const MidiMessage& message);
    void dispatchNote(const MidiMessage& message);
    Effect* createEffect(const MidiMessage& message);
    void execSystemCommand(const MidiMessage& message);

private:
    MidiPort* _pMidiPort = nullptr;
    MAX7219Universe* _pUniverse = nullptr;
    Bitmap* _pTarget = nullptr;

    Layer _layer;
    Timing _timing;

    bool _pressedKeys[16][128];
    bool _sustainEnabled[16];

    typedef std::multimap<uint8_t, Effect*> effectMap_t;
    effectMap_t _effectMap;
};

F_END_NAMESPACE

#endif // _ESP_EFFECT_MATRIXPLAYER_H