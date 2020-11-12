/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "Application.h"
#include "fonts/Fonts.h"

#include <WiFi.h>
#include <SPIFFS.h>

F_USE_NAMESPACE

const int Application::CLOCK_PIN = 4;
const int Application::LOAD_PIN = 16;
const int Application::DATA_PINS[] = { 26, 25, 33, 32, 17, 5, 18, 19 };
const int Application::DELAY = 1;
const int Application::ROWS = 8;

Application::Application() :
    _server(80),
    _universe(CLOCK_PIN, LOAD_PIN, DELAY),
    _canvas(64, 64)
{
}

void Application::setup()
{
    Serial.begin(115200);
    SPIFFS.begin(true);

    _env.read();

    for (size_t row = 0; row < ROWS; ++row) {
        _matrices[row] = _universe.createChain(DATA_PINS[row])->createMatrices(8);
        _canvas.addMatrices(_matrices[row], 0, 8 * row, 8, 0);
    }

    _universe.setClockPinInverted(true);
    _universe.setLoadPinInverted(true);
    _universe.setBrightness(2);
    _universe.initialize();

    _pPlayer = new MatrixPlayer(&_midiPort, &_universe, &_canvas);

    _canvas.rect(0, 0, 64, 64);
    _canvas.drawText("BIT", &ff::Fonts::fontC64, 8, 8);
    _canvas.drawText("MATRIX", &ff::Fonts::fontC64, 8, 16);
    _canvas.drawText("V 1.0", &ff::Fonts::fontC64, 8, 24);

    _canvas.drawText("FRAME", &ff::Fonts::fontC64, 8, 40);
    _canvas.drawText("FACTORY", &ff::Fonts::fontC64, 8, 48);
    _canvas.update();
    _universe.writeDisplay();

    _connectWifi();
    _midiPort.begin();
    
    //float offset = _env.getFloat("TIMEZONE_OFFSET");
    //float dst = _env.getFloat("TIMEZONE_DST");
    //_comp.fetchRealTime(offset, dst);
    //_server.initialize(&_universe);

    delay(2000);
    _canvas.clear();
    _universe.initialize();
}

void Application::loop()
{
    //_server.handleClient();

    _pPlayer->update();

    // _canvas.clear();
    // _indexEffect.render(&_canvas, _pPlayer->timing());
    // _linesEffect.render(&_canvas, _pPlayer->timing());

    static bool isOn = false;
    isOn = !isOn;
    _canvas.set(0, 0, isOn);

    _canvas.update();
    _universe.writeDisplay();

    delay(1);
}

void Application::_connectWifi()
{
    const char* ssid = _env.getCStr("WIFI_SSID");
    if (!ssid) {
        Serial.println("[WARNING] Can't connect to WiFi, SSID not set.");
        return;
    }

    const char* password = _env.getCStr("WIFI_PASSWORD");
    if (!password) {
        Serial.println("[WARNING] Can't connect to WiFi, password not set.");
        return;
    }

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