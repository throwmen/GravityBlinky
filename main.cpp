/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include <iostream>

// Blinking rate in milliseconds
#define BLINKING_RATE     100ms


UnbufferedSerial serial(USBTX, USBRX, 9600);
AnalogIn lightSensor1(A1);
AnalogIn lightSensor2(A0);
Timer timer;
bool manin = false;
int main()
{
    std::cout << "Inicia la gravity: " << std::endl;
    float treshold = 0.1;

    while (true) {
        float lightValue1 = lightSensor1.read();
        float lightValue2 = lightSensor2.read();

        if(lightValue1 < treshold){
            timer.start();
            manin = true;
        }
        else if (lightValue2 < treshold){
            if (manin){
                timer.stop(); 
                float timeInSeconds = static_cast<float>(timer.read_us()); //BUSCAR COMO PASARLO A SEGUNDOS. SI ES NECESARIO USAR NUEVO TIMER
                if (timeInSeconds != 0){
                    float gravity = 2*0.28/pow(timeInSeconds,2);
                    std::cout << "La gravedad es de: " << gravity << " m/s^2" << std::endl;
                    ThisThread::sleep_for(BLINKING_RATE);
                    timer.reset();
                    manin = false;
                }
            }  
        }
    }
}
