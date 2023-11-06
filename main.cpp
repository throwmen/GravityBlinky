/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include <iostream>

// Blinking rate in milliseconds
#define BLINKING_RATE     100ms


UnbufferedSerial serial(USBTX, USBRX, 9600);
AnalogIn lightSensor1(A0);
AnalogIn lightSensor2(A1);
Timer timer;

int main()
{
    bool paso1 = true;
    bool paso2 = false;
    std::cout << "Inicia el cálculo de la gravedad: " << std::endl;
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
            float timeInSeconds = duration_cast<milliseconds>(timer.elapsed_time()).count()/1000;
            float gravity = 2*height/pow(timeInSeconds,2);
            std::cout << "La gravedad es de: " << gravity << " m/s^2" << std::endl;
            timer.reset();
            paso1 = true; 
            paso2 = false;       
        }
    }
}