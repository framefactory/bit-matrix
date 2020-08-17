/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include <Arduino.h>
#include "Application.h"

ff::Application app;

void setup()
{
    app.setup();
}

void loop()
{
    app.loop();
}