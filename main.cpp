
#include "Grove_LCD_RGB_Backlight.h"
#include "mbed.h"

#define WAIT_TIME_MS 500

DigitalOut led1(LED1);
Grove_LCD_RGB_Backlight rgbLCD(PB_9, PB_8);
float dato = 0.0;
float datoMin = 0.0;
float datoMax = 0.0;
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
enum estados {
  inicial,
  ajusteMinimo,
  ajusteMaximo,
  Medicion,
} estado;

float tomaDeDatos() {
  float vector[1000];
  int contador = 0;
  float promedio = 0.0;
  wait_us(100000);

  for (i = 1; i <= 1000; i++) {

    promedio = 0.9 * promedio + 0.1 * datoV.read_voltage();
    contador++;
    wait_us(10000);
  }

  promedio = promedio / contador;
  return promedio;
}

// Esta función calcula el valor en masa en base a la tensión de salida del
// acondicionamiento de señal
float calcularMasa(float Vout) {

  masa = (Vout - minimo) * 50 / (maximo - minimo);
  dato = dato * 0.9 + 0.1 * masa;
    
  return masa;
}

void funcionInicial() {
  rgbLCD.locate(0, 0); // set the color
  rgbLCD.print("Buenas tardes");
  rgbLCD.locate(0, 1);
  rgbLCD.print("abonados");
  wait_us(1000000);
  estado = ajusteMinimo;
}

void funcionAjusteMinimo() {
  // funcionBorrarPantalla();
  rgbLCD.locate(0, 0);
  rgbLCD.print("No coloquen     ");
  rgbLCD.locate(0, 1);
  rgbLCD.print("ninguna carga   ");
  if (boton == 1) {
    rgbLCD.locate(0, 0);
    rgbLCD.print("Calculando...    ");
    rgbLCD.locate(0, 1);
    rgbLCD.print("                ");
    minimo = tomaDeDatos();
    estado = ajusteMaximo;
  }
}

void funcionAjusteMaximo() {

  // funcionBorrarPantalla();
  rgbLCD.locate(0, 0);
  rgbLCD.print("Coloquen la      ");
  rgbLCD.locate(0, 1);
  rgbLCD.print("carga de 50 g");
  if (boton == 1) {
    rgbLCD.locate(0, 0);
    rgbLCD.print("Calculando...    ");
    rgbLCD.locate(0, 1);
    rgbLCD.print("                ");
    maximo = tomaDeDatos();
    estado = Medicion;
  }
}
void funcionMedicion() {
  // funcionBorrarPantalla();
  rgbLCD.locate(0, 0);
  rgbLCD.print("Coloquen la pesa");
  rgbLCD.locate(0, 1);
  rgbLCD.print("                ");
  if (boton == 1) {
    rgbLCD.locate(0, 0);
    rgbLCD.print("Calculando...    ");
    rgbLCD.locate(0, 1);
    rgbLCD.print("                ");
    tension = tomaDeDatos();
    masa = calcularMasa(tension);
    printf("%f", masa);
    sprintf(masa_pantalla, "%f", masa);
    printf(masa_pantalla);
    rgbLCD.locate(0, 0);
    rgbLCD.print("masa=           ");
    rgbLCD.locate(0, 1);
    rgbLCD.print("                ");
    rgbLCD.locate(1, 1);
    rgbLCD.print(masa_pantalla);
    rgbLCD.locate(15, 1);
    rgbLCD.print("g");
    wait_us(10000000);
  }
}

int main() {
  datoV.set_reference_voltage(3.3);
  rgbLCD.setRGB(0xff, 0xff, 0xff);
  // funcionBorrarPantalla();

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
