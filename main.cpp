
#include "rgb_lcd.h"
#include "mbed.h"


#define WAIT_TIME_MS 500

DigitalOut led1(LED1);
I2C i2c(PB_9, PB_8);
rgb_lcd rgbLCD(&i2c );

float masa;
AnalogIn datoV(A3);
DigitalIn boton(D4);
char masa_pantalla[16];
float Vout;
float minimo;
float maximo;
float tension;
int i;


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
  //rgbLCD.locate(0, 0);
  //rgbLCD.print("Quiten la carga");
  //rgbLCD.locate(0, 1);
  //rgbLCD.print("de la bascula");
  if (boton == 1) {

    minimo = tomaDeDatos();

    estado = ajusteMaximo;
  }
}

void funcionAjusteMaximo() {
  rgbLCD.setCursor(0, 0);
  char cadena[5]="hola";
  rgbLCD.read(cadena, 5);
 // rgbLCD.locate(0, 1);
  //rgbLCD.print("carga de 100 g");
  if (boton == 1) {

    maximo = tomaDeDatos();
    estado = Medicion;
  }
}

void funcionMedicion() {
  //rgbLCD.locate(0, 0);
  //rgbLCD.print("Coloquen la pesa");

  tension = tomaDeDatos();
  masa = calcularMasa(tension);
  printf("%f\n", masa);
  // int intpeso=masa;
  
  sprintf(masa_pantalla, "%f\n", masa);
  printf("%s\n", masa_pantalla);
  //rgbLCD.locate(0, 0);
  wait_us(1000);
  //rgbLCD.print(masa_pantalla,16);
  // rgbLCD.locate(15, 1);
  // rgbLCD.print("g");
}

void funcionInicial() {
  //rgbLCD.locate(0, 1); // set the color
  //rgbLCD.print("Buenas tardes");
  //rgbLCD.locate(0, 1);
 // rgbLCD.print("abonados");
  wait_us(1000000);
  estado = ajusteMinimo;
}

int main() {
rgbLCD.begin();
  rgbLCD.setCursor(0, 0);
  char cadena[5]="hola";
  
  rgbLCD.display();
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
