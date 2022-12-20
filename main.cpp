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
    float minimo;
    float maximo;
    float tension;

I2C i2c(I2C_SDA, I2C_SCL);
void ajusteLimites(){
   rgbLCD.print("Buenas tardes abonados");
   wait_us(1000000);
   rgbLCD.print("Quiten la carga de la bascula");
   minimo=tomaDeDatos();
   rgbLCD.print("Coloquen la carga de 100 g");
   maximo=tomaDeDatos();
}

float tomaDeDatos(){
    float vector[1000];
    int contador;
    float promedio=0.0;
    wait_us(3000000);
    for(int i=1;i<=1001;i++){
        vector[i]=datoV.read_voltage();
        contador++;
        wait_us(1000);
        promedio=promedio+vector[i];
    }
    promedio=promedio/contador;
    return promedio;
}

// Esta función calcula el valor en masa en base a la tensión de salida del acondicionamiento de señal
float calcularMasa(float datoV){
    masa=(Vout-minimo)*100/(maximo-minimo);
    return masa;
}
 
int main()
{
    datoV.set_reference_voltage(3.3);
    while (true)
    {   
        rgbLCD.print("Buenas tardes abonados");
        wait_us(1000000);
        rgbLCD.print("Quiten la carga de la bascula");
        minimo=tomaDeDatos();
        rgbLCD.print("Coloquen la carga de 100 g");
        maximo=tomaDeDatos();
        ajusteLimites();
        rgbLCD.print("Coloquen la carga que pretenden pesar");
        tension=tomaDeDatos();
        calcularMasa(tension);
        rgbLCD.setRGB(0xff, 0xff, 0xff);    //set the color 
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



