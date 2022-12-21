
#include "Grove_LCD_RGB_Backlight.h"
#include "mbed.h"

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
float cincuenta;
float diez;
float tension;
int i;

I2C i2c(I2C_SDA, I2C_SCL);
enum estados {
  inicial,
  ajusteMinimo,
  ajusteMaximo,
  Medicion,
  ajusteCincuenta,
  ajusteDiez
} estado;

float tomaDeDatos() {
  float vector[1000];
  int contador = 0;
  float promedio = 0.0;
  for (i = 1; i <= 1000; i++) {
    if(datoV.read_voltage()!=0.0&&datoV.read_voltage()!=3.3){
    vector[i] = datoV.read_voltage();
    contador++;
    wait_us(10000);
    promedio = promedio + vector[i];
    printf("%f\n",vector[i]);
    }
  }

  promedio = promedio / contador;
  printf("%d\n%f\n", contador, promedio);
  return promedio;
}

// Esta función calcula el valor en masa en base a la tensión de salida del
// acondicionamiento de señal
float calcularMasa(float Vout) {
  //masa = (Vout - minimo) * 100 / (maximo - minimo);
  //if(masa<50){
   //   masa=(Vout - minimo) * 50 / (cincuenta - minimo);
 // }else if (masa<10) {
 /// masa=(Vout - minimo) * 10 / (diez - minimo);
 // }
 float n100, n50, n10, n, cincuenta100, cincuenta10, a;
    n100=100/(maximo-minimo);
    n50=50/(cincuenta-minimo);
    n10=10/(diez-minimo);
    n=(n100+n50+n10)/3;
    cincuenta100=(cincuenta - minimo) * 100 / (maximo - minimo);
    cincuenta10=(cincuenta - minimo) * 10 / (diez - minimo);
    cincuenta=(cincuenta+cincuenta100+cincuenta10)/3;
    a=cincuenta-n*50;
    masa=(Vout-a)/n;
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
  rgbLCD.print("Quiten la carga");
  rgbLCD.locate(0, 1);
  rgbLCD.print("de la bascula");
  if (boton == 1) {
    minimo = tomaDeDatos();
    estado = ajusteMaximo;
  }
}

void funcionAjusteMaximo() {

  // funcionBorrarPantalla();
  rgbLCD.locate(0, 0);
  rgbLCD.print("Coloquen la      ");
  rgbLCD.locate(0, 1);
  rgbLCD.print("carga de 100 g");
  if (boton == 1) {

    maximo = tomaDeDatos();
    estado = ajusteCincuenta;
  }
}

void funcionAjusteCincuenta() {
  rgbLCD.locate(0, 0);
  rgbLCD.print("Coloquen la      ");
  rgbLCD.locate(0, 1);
  rgbLCD.print("carga de 50 g ");
  if (boton == 1) {
    cincuenta = tomaDeDatos();
    estado = ajusteDiez;
  }
}

void funcionAjusteDiez(){
    rgbLCD.locate(0, 0);
  rgbLCD.print("Coloquen la      ");
  rgbLCD.locate(0, 1);
  rgbLCD.print("carga de 10 g ");
  if (boton == 1) {
    diez = tomaDeDatos();
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
    case ajusteCincuenta:
      funcionAjusteCincuenta();
      break;
    case ajusteDiez:
      funcionAjusteDiez();
      break;
    }
    wait_us(1000);
  }
}
