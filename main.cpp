
#include <stdio.h>
#include "mbed.h"
#include "Grove_LCD_RGB_Backlight.h"
#include <math.h>

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


float tomaDeDatos(){
    float vector[1000];
    int contador;
    float promedio=0.0;
    float prueba1=datoV.read_voltage();
    wait_us(7000000);
    float prueba2=datoV.read_voltage();
    if(fabsprueba1-prueba2){
        rgbLCD.locate(0,0);
        rgbLCD.print("¿Estais ahi?");
        rgbLCD.locate(0,1);
        rgbLCD.print("");
        wait_us(5000000);
    }
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
        rgbLCD.setRGB(0xff, 0xff, 0xff);//set the color
        rgbLCD.print("Buenas tardes ");
        rgbLCD.locate(0,1);
        rgbLCD.print("abonados");
        wait_us(1000000);
        rgbLCD.locate(0,0);
        rgbLCD.print("Quiten la carga");
        rgbLCD.locate(0,1);
        rgbLCD.print("de la bascula");
        minimo=tomaDeDatos();
        rgbLCD.locate(0,0);
        rgbLCD.print("Coloquen la");
        rgbLCD.locate(0,1);
        rgbLCD.print("carga de 100 g");
        maximo=tomaDeDatos();
        rgbLCD.locate(0,0);
        rgbLCD.print("Coloquen la carga");
        rgbLCD.locate(0,1);
        rgbLCD.print("que pretenden pesar");
        tension=tomaDeDatos();
        masa=calcularMasa(tension);
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



