/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include "mbed.h"
#include "Grove_LCD_RGB_Backlight.h"
 
#define WAIT_TIME_MS 500 
DigitalOut led1(LED1);
Grove_LCD_RGB_Backlight rgbLCD(PB_9,PB_8);

 float masa=4.2; 
// float Vout ; 
 //float G=16.0 ; 
 //int16_t datoV =65536;
 char masa_pantalla[10];


I2C i2c(I2C_SDA, I2C_SCL);



 
int main()
{
    
    while (true)
    { 
        rgbLCD.setRGB(0xff, 0xff, 0xff);                 //set the color 
        rgbLCD.locate(0,0);
        rgbLCD.print("masa = ");
        rgbLCD.locate(0,1);
        sprintf(masa_pantalla, "%f" ,masa);
        rgbLCD.print(masa_pantalla);
        rgbLCD.locate(15,1);
        rgbLCD.print("g");
        led1 = !led1;
        thread_sleep_for(WAIT_TIME_MS);
    }
}
//
//     Esta función calcula el valor en masa en base a la tensión de salida del acondicionamiento de señal
int calcularmasa(int16_t datoV )
{
    Vout = (datoV / 65536.0) * (5000.0) ; // así se obtiene el dato en mV
    masa = (Vout) / (5.0 * G) ;  // así se pasa el dato a kg
    return masa;  
}