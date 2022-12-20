/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */


#include <stdio.h>
#include "mbed.h"
#include "Grove_LCD_RGB_Backlight.h"
 
/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */



 
#define WAIT_TIME_MS 500 
DigitalOut led1(LED1);
Grove_LCD_RGB_Backlight rgbLCD(PB_9,PB_8);

 float masa;
 AnalogIn datoV(A3);
 char masa_pantalla[10];
 float Vout;

I2C i2c(I2C_SDA, I2C_SCL);

float calcularMasa(int16_t datoV );
//
 
int main()
{
    datoV.set_reference_voltage(3.3);
    while (true)
    { 
        rgbLCD.setRGB(0xff, 0xff, 0xff);                 //set the color 
        rgbLCD.locate(0,0);
        rgbLCD.print("masa = ");
        rgbLCD.locate(0,1);
        masa=(datoV.read_voltage()-0.36)/3.3*100;
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
float calcularMasa(float datoV ){
    float masa;
    float Vout ; 
    Vout = (datoV / 4096.0) * (1000.0) ; // así se obtiene el dato en mV  // así se pasa el dato a kg
    return Vout;  
}
   ////