/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "Application.h"

#include "effect/examples/Clock16.h"
//#include "effect/examples/Clock22.h"
#include "effect/examples/DrawTest.h"
#include "fonts/Fonts.h"

#include <WiFi.h>
#include <SPIFFS.h>

F_USE_NAMESPACE

const int Application::CLOCK_PIN = 4;
const int Application::LOAD_PIN = 16;
const int Application::DATA_PINS[] = { 17, 5, 18, 19, 0, 0, 0, 0 };
const int Application::DELAY = 1;
const int Application::ROWS = 4;

Application::Application() :
    _server(80),
    _universe(CLOCK_PIN, LOAD_PIN, DELAY),
    _canvas(64, 64),
    _comp(64, 64)
{
    for (size_t row = 0; row < ROWS; ++row) {
        _matrices[row] = _universe.createChain(DATA_PINS[row])->createMatrices(8);
        _canvas.addMatrices(_matrices[row], 0, 8 * row, 8, 0);
    }

    //_comp.addEffectLayer(new ff::Clock16());
    //_comp.addEffectLayer(new ff::DrawTest(), Bitmap::Xor);
}

void Application::setup()
{
    Serial.begin(115200);
    SPIFFS.begin(true);

    _env.read();

    _universe.setClockPinInverted(true);
    _universe.setLoadPinInverted(true);
    _universe.setBrightness(8);
    _universe.initialize();


    _canvas.drawText("WELCOME!", &ff::Fonts::fontC64, 0, 0);
    _canvas.drawText("THIS IS:", &ff::Fonts::fontC64, 0, 8);
    _canvas.drawText("BIT-----", &ff::Fonts::fontC64, 0, 16);
    _canvas.drawText("MATRIXXX", &ff::Fonts::fontC64, 0, 24);
    _canvas.update();
    _universe.writeDisplay();

    _connectWifi();
    _midi.begin();
    
    //float offset = _env.getFloat("TIMEZONE_OFFSET");
    //float dst = _env.getFloat("TIMEZONE_DST");
    //_comp.fetchRealTime(offset, dst);
    //_server.initialize(&_universe);

    delay(2000);
    _canvas.clear();
}

void Application::loop()
{
    static bool isOn = false;
    //_server.handleClient();

    _canvas.clear();
    _canvas.drawText("0001020304050607", &ff::Fonts::font04B24, 0, 0, 4);
    _canvas.drawText("0809101112131415", &ff::Fonts::font04B24, 0, 8, 4);
    _canvas.drawText("1617181920212223", &ff::Fonts::font04B24, 0, 16, 4);
    _canvas.drawText("2425262728293031", &ff::Fonts::font04B24, 0, 24, 4);

    if (_comp.render()) {
        _canvas.blit(_comp);

        //isOn = !isOn;
        //_canvas.set(0, 0, isOn);

        //_canvas.update();
        //_universe.writeDisplay();
    }

    isOn = !isOn;
    _canvas.set(0, 0, isOn);

    _canvas.update();
    _universe.writeDisplay();

    delay(10);
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

    //int16_t count = WiFi.scanNetworks();
    //for (int16_t i = 0; i < count; ++i) {
    //    Serial.printf("%d: %s, Ch:%d (%ddBm) %d\n", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.encryptionType(i));
    //}

    while (wifiStatus != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        wifiStatus = WiFi.begin(ssid, password);
    }

    IPAddress ip = WiFi.localIP();
    Serial.printf("\nConnected! IP Address: %s\n", ip.toString().c_str());
}