/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_BITMATRIX_APPLICATION_H
#define _ESP_BITMATRIX_APPLICATION_H

#include "app.h"
#include "ApiServer.h"

#include "matrix/MAX7219Universe.h"
#include "matrix/MAX7219Chain.h"
#include "matrix/MAX7219Matrix.h"
#include "matrix/Canvas.h"
#include "effect/Composition.h"
#include "core/Environment.h"
#include "net/BLEMidi.h"

F_BEGIN_NAMESPACE

class Application
{
private:
    static const int CLOCK_PIN;
    static const int LOAD_PIN;
    static const int DATA_PINS[8];
    static const int DELAY;
    static const int ROWS;

public:
    Application();
    virtual ~Application() {}

    void setup();
    void loop();

private:
    void _connectWifi();

    Environment _env;
    ApiServer _server;
    BLEMidi _midi;

    MAX7219Universe _universe;
    MAX7219Matrices _matrices[8];
    Canvas _canvas;
    Composition _comp;
};

F_END_NAMESPACE

#endif // _ESP_BITMATRIX_APPLICATION_H