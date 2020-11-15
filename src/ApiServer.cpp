/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "ApiServer.h"

F_USE_NAMESPACE

ApiServer::ApiServer(int port) :
    _server(port),
    _brightness(4)
{
    _server.on("/", [&]() { this->_handleRoot(); });
    _server.on("/brightness/{}", [&]() { this->_handleBrightness(); });
    _server.on("/set/{}/{}", [&]() { this->_handleSet(); });

    _server.onNotFound([&]() {
            _server.send(404, "text/plain", "ESP32 not found: " + _server.uri());
    });
}

void ApiServer::initialize(MAX7219Universe* pUniverse)
{
    _pUniverse = pUniverse;
    _server.begin();
}

void ApiServer::handleClient()
{
    _server.handleClient();
}

void ApiServer::_handleRoot()
{
    _server.send(200, "text/plain", "ESP32: " + _server.uri());
}

void ApiServer::_handleBrightness()
{
    const String& method = _server.pathArg(0);

    if (method == "up") {
        _brightness = std::min(15, _brightness + 1);
        _pUniverse->setMaxBrightness(_brightness);
        _server.send(200, "text/plain", "ESP32: Brightness up: " + String(_brightness));
    }
    else if (method == "down") {
        _brightness = std::max(0, _brightness - 1);
        _pUniverse->setMaxBrightness(_brightness);
        _server.send(200, "text/plain", "ESP32: Brightness down: " + String(_brightness));
    }

    _pUniverse->writeBrightness();
}

void ApiServer::_handleSet()
{
    Serial.println("handleSet");

    Serial.printf("URI: %s\n", _server.uri().c_str());
    Serial.printf("Method: %s\n", _server.method() == HTTP_GET ? "GET" : "POST");
    Serial.printf("Arguments: %d\n", _server.args());

      for (int i = 0; i < _server.args(); ++i) {
        Serial.printf("  %s: %s\n", _server.argName(i).c_str(), _server.arg(i).c_str());
    }

      for (int i = 0; i < 2; ++i) {
        Serial.printf("  PathArg %d: %s\n", i, _server.pathArg(i).c_str());
    }

    _server.send(200, "text/plain", "ESP32: " + _server.uri());
}
