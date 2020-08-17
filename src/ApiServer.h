/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_BITMATRIX_APISERVER_H
#define _ESP_BITMATRIX_APISERVER_H

#include "app.h"
#include "matrix/MAX7219Universe.h"

#include <WebServer.h>


F_BEGIN_NAMESPACE

class ApiServer
{
public:
    ApiServer(int port = 80);

    void initialize(MAX7219Universe* pUniverse);
    void handleClient();

private:
    void _handleRoot();
    void _handleBrightness();
    void _handleSet();

    ::WebServer _server;
    MAX7219Universe* _pUniverse;
    int _brightness;
};

F_END_NAMESPACE

#endif // _ESP_BITMATRIX_APISERVER_H