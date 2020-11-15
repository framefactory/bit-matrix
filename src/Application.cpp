/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "Application.h"
#include "Fonts.h"

#include <WiFi.h>
#include <SPIFFS.h>

F_USE_NAMESPACE

const int Application::CLOCK_PINS[] = { 4, 14 };
const int Application::LOAD_PINS[] = { 16, 27 };
const int Application::DATA_PINS[] = { 26, 25, 33, 32, 17, 5, 18, 19 };
const int Application::DELAY = 1;
const int Application::ROWS = 8;

Application::Application() :
    _server(80),
    _universe(CLOCK_PINS[0], LOAD_PINS[0], DELAY),
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

    _universe.setSecondClockPin(CLOCK_PINS[1]);
    _universe.setSecondLoadPin(LOAD_PINS[1]);
    _universe.setClockPinInverted(true);
    _universe.setLoadPinInverted(true);
    _universe.setMaxBrightness(2);
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

    // ------ BEGIN DEBUG
    static int dotX = 0;
    static double debugSeconds = 0.0;

    if (F_DEBUG) {
        dotX = (dotX + 1) % 8;
        _canvas.set(dotX, 0, true);

        if (_pPlayer->timing().seconds > debugSeconds) {
            debugSeconds += 10.0;
            Serial.printf("[Application] free heap size: %d, largest block: %d\n",
                heap_caps_get_free_size(MALLOC_CAP_8BIT),
                heap_caps_get_largest_free_block(MALLOC_CAP_8BIT)
            );
        }
    }
    // ----- END DEBUG

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