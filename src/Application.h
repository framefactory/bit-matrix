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
#include "effect/Player.h"
#include "core/Environment.h"

F_BEGIN_NAMESPACE

class Application
{
private:
    static const int CLOCK_PIN = 23;
    static const int LOAD_PIN = 22;
    static const int DATA_PIN = 21;
    static const int DELAY = 1;

public:
    Application();
    virtual ~Application() {}

    void setup();
    void loop();

private:
    void _connectWifi();

    Environment _env;
    ApiServer _server;

    MAX7219Universe _universe;
    MAX7219Matrices _matrices;
    Canvas _canvas;
    Player _player;
};

F_END_NAMESPACE

#endif // _ESP_BITMATRIX_APPLICATION_H