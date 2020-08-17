/**
 * BitMatrix - Raspberry Pi Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "app.h"
#include "MAX7219Universe.h"
#include "MAX7219Chain.h"
#include "MAX7219Matrix.h"
#include "Canvas.h"

#include <wiringPi.h>
#include <random>
#include <cmath>
#include <iostream>
#include <string>
#include <signal.h>

using namespace std;

volatile bool isRunning = true;

void handler(int signum)
{
	isRunning = false;
}

void test1(MAX7219Universe* pUniverse)
{
	auto pChain0 = pUniverse->chain(0);
	auto pChain1 = pUniverse->chain(1);

	int i = 0;
	while(isRunning) {
		for (int d = 0; d < 8; ++d) {
			auto pMatrix0 = pChain0->matrix(d);
			pMatrix0->setBrightness(uint8_t(sin(float(i + d * 5) * 0.3f) * 8.0f + 8.0f));			
			pMatrix0->clear();
			int dd = (d * 3 + (i / 8) * 5) % 8;
			if (d % 2) {
				pMatrix0->setDot((d + i) % 8, dd, true);
			}
			else {
				pMatrix0->setDot((dd + 4) % 8, (d + i + 4) % 8, true);			
			}

			auto pMatrix1 = pChain1->matrix(d);
			pMatrix1->setBrightness(uint8_t(sin(float(i + d * 7) * 0.3f) * 8.0f + 8.0f));			
			pMatrix1->clear();
			if (d % 2) {
				pMatrix1->setDot((dd + 3) % 8, (d + i + 3) % 8, true);			
			}
			else {
				pMatrix1->setDot((d + i + 5) % 8, (dd + 5) % 8, true);
			}
		}
		pUniverse->update();
		++i;
		delay(50);
	}
}

int main(int argc, char** ppArgv)
{
	signal(SIGINT, handler);
	wiringPiSetup();

	const int DATA_PIN0 = 15;
	const int DATA_PIN1 = 4;
	const int LOAD_PIN = 16;
	const int CLOCK_PIN = 1;

	auto pUniverse = new MAX7219Universe(CLOCK_PIN, LOAD_PIN, 1);
	pUniverse->setClockPinInverted(true);
	pUniverse->setLoadPinInverted(true);

	auto pCanvas = new Canvas(256, 256);

	auto pChain0 = pUniverse->createChain(DATA_PIN0);
	pCanvas->addMatrices(pChain0->createMatrices(8), 8, 8, 32, 0);

	auto pChain1 = pUniverse->createChain(DATA_PIN1);
	pCanvas->addMatrices(pChain1->createMatrices(8), 8, 40, 32, 0);

	pUniverse->initialize();
	//pUniverse->setBrightness(15);

	default_random_engine gen;
	uniform_int_distribution<uint32_t> dist(16, 32);
	Canvas::bitmap_t font_c64("../assets/fonts/dm8_c64.bmp");
	Canvas::bitmap_t font_04b("../assets/fonts/dm8_04b24.bmp");

	int ic = 0;
	while(isRunning) {
		uint8_t ONE[] = { 1 };
		uint32_t pattern = 0xffffffff;
		pCanvas->bitmap()->fill(0);
		//pCanvas->drawText("Hello", font_04b, 0, 0, 4);
		//pCanvas->drawText("World!", font_04b, 0, 8, 4);
		//pCanvas->bitmap()->draw_circle(dist(gen), dist(gen), dist(gen), ONE, 1.0f, pattern);
		for (size_t im = 0; im < pCanvas->numMappings(); ++im) {
			const auto& mapping = pCanvas->mapping(im);
			string text = to_string(im);
			if (text.size() == 1) {
				text.insert(0, 1, '0');
			}

			int offset = ((ic + im) % 16);
			pCanvas->drawText(text, font_04b, mapping.x - offset + 8, mapping.y, 4);
			mapping.pMatrix->setBrightness(offset < 8 ? offset : 15 - offset);
		}
		
		pCanvas->update();
		pUniverse->update();
		ic++;
		delay(40);
	}

	//test1(pUniverse);

	pUniverse->shutdown();
	F_SAFE_DELETE(pUniverse);

	return 0;
}
