
#include "Grove_LCD_RGB_Backlight.h"
#include "mbed.h"
#include <math.h>


#define WAIT_TIME_MS 500

DigitalOut led1(LED1);
Grove_LCD_RGB_Backlight rgbLCD(PB_9, PB_8);

float masa;
AnalogIn datoV(A3);
DigitalIn boton(D4);
char masa_pantalla[16];
float Vout;
float minimo;
float maximo;
float tension;
int i;

I2C i2c(I2C_SDA, I2C_SCL);
enum estados { inicial, ajusteMinimo, ajusteMaximo, Medicion } estado;

float tomaDeDatos() {
  float vector[1000];
  int contador = 0;
  float promedio = 0.0;
  for (i = 1; i <= 1000; i++) {
    vector[i] = datoV.read_voltage();
    contador++;
    wait_us(1000);
    promedio = promedio + vector[i];
  }
  promedio = promedio / contador;
  return promedio;
}

// Esta función calcula el valor en masa en base a la tensión de salida del
// acondicionamiento de señal
float calcularMasa(float Vout) {
  masa = (Vout - minimo) * 100 / (maximo - minimo);
  return masa;
}

void funcionAjusteMinimo() {
    rgbLCD.locate(0, 0);
    rgbLCD.print("Quiten la carga");
    rgbLCD.locate(0, 1);
    rgbLCD.print("de la bascula");
  if (boton == 1) {

    minimo = tomaDeDatos();


    estado = ajusteMaximo;
  }
}

void funcionAjusteMaximo() {
    rgbLCD.locate(0, 0);
    rgbLCD.print("Coloquen la");
    rgbLCD.locate(0, 1);
    rgbLCD.print("carga de 100 g");
  if (boton == 1) {
  
    maximo = tomaDeDatos();
    estado=Medicion;
  }
}

void funcionMedicion() {
  rgbLCD.locate(0, 0);
  rgbLCD.print("Coloquen la pesa");

  tension = tomaDeDatos();
 masa = calcularMasa(tension);
printf("%f\n",masa);
int intpeso=masa;
  rgbLCD.locate(0, 1);
  sprintf(masa_pantalla, "%d", intpeso);
  rgbLCD.print(masa_pantalla);
  //rgbLCD.locate(15, 1);
  //rgbLCD.print("g");
}

void funcionInicial() {
  rgbLCD.locate(0, 1); // set the color
  rgbLCD.print("Buenas tardes");
  rgbLCD.locate(0, 1);
  rgbLCD.print("abonados");
  wait_us(1000000);
  estado = ajusteMinimo;
}

int main() {
  datoV.set_reference_voltage(3.3);

  rgbLCD.setRGB(0xff, 0xff, 0xff);
  estado = inicial;

  while (1) {
    switch (estado) {
    case inicial:
      funcionInicial();
      break;
    case ajusteMinimo:
      funcionAjusteMinimo();
      break;
    case ajusteMaximo:
      funcionAjusteMaximo();
      break;
    case Medicion:
      funcionMedicion();
      break;
    }
     wait_us(1000);
  }
}
