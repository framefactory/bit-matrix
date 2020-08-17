/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "Application.h"

#include "effect/examples/Clock.h"
#include "effect/examples/DrawTest.h"
#include "fonts/Fonts.h"

#include <WiFi.h>
#include <SPIFFS.h>

F_USE_NAMESPACE

Application::Application() :
    _server(80),
    _universe(CLOCK_PIN, LOAD_PIN, DELAY),
    _matrices(_universe.createChain(DATA_PIN)->createMatrices(6)),
    _canvas(48, 8)
{
    _canvas.addMatrices(_matrices, 0, 0, 8, 0);
    _player.addEffect(new ff::Clock(&_canvas));
    _player.addEffect(new ff::DrawTest(&_canvas));
}

void Application::setup()
{
    Serial.begin(115200);
    SPIFFS.begin(true);
    _env.read();

    _universe.initialize();

    _canvas.drawText("BITMATRIX", &ff::Fonts::font04B24, 6, 0, 4);
    _canvas.update();
    _universe.writeDisplay();

    _connectWifi();
    
    float offset = _env.getFloat("TIMEZONE_OFFSET");
    float dst = _env.getFloat("TIMEZONE_DST");
    _player.fetchRealTime(offset, dst);
    _server.initialize(&_universe);

    //_player.startEffect(ff::Clock::name);
    _player.startEffect(ff::DrawTest::name);
    _canvas.clear();
}

void Application::loop()
{
    _server.handleClient();

    if (_player.render()) {
        _canvas.update();
        _universe.writeDisplay();
        _canvas.clear();
    }
}

void Application::_connectWifi()
{
    const char* ssid = _env.getCStr("WIFI_SSID");
    const char* password = _env.getCStr("WIFI_PASSWORD");

    Serial.printf("Connecting to WiFi, SSID: %s ", ssid);
    int wifiStatus = WiFi.begin(ssid, password);

    while (wifiStatus != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        wifiStatus = WiFi.begin(ssid, password);
    }

    IPAddress ip = WiFi.localIP();
    Serial.printf("\nConnected! IP Address: %s\n", ip.toString().c_str());
}