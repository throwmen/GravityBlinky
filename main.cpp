/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include <iostream>
#include "TextLCD.h"
using namespace std::chrono;
// Blinking rate in milliseconds
#define BLINKING_RATE     100ms


UnbufferedSerial serial(USBTX, USBRX, 9600);
TextLCD lcd (D2, D3, D4, D5, D6, D7, TextLCD::LCD16x2);
AnalogIn lightSensor1(A0);
AnalogIn lightSensor2(A1);
Timer timer;

int main()
{
    std::cout << "Inicia el cálculo de la gravedad: " << std::endl;
    bool paso1 = true;
    bool paso2 = false;
    float treshold = 0.2;
    const float height = 0.1;

    while (true) {
        if(lightSensor1.read() < treshold && paso1){
            timer.reset();
            timer.start();
            paso1 = false;
            paso2 = true;
        }
        else if (lightSensor2.read() < treshold && paso2){
            timer.stop(); 
            float timeInSeconds = duration_cast<milliseconds>(timer.elapsed_time()).count();
            timeInSeconds /= 1000;
            float gravity = 2*height/pow(timeInSeconds,2);
            std::cout << "La gravedad es de: " << gravity << " m/s^2" << std::endl;
            lcd.cls();
            lcd.locate(0, 0);
            lcd.printf("La aceleración es: ");
            lcd.locate(0, 1);
            lcd.printf("%f .m/s", gravity);
            paso1 = true; 
            paso2 = false;       
        }
    }
}