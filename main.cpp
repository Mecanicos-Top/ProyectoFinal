
#include "Grove_LCD_RGB_Backlight.h"
#include "mbed.h"
 
//Definición de las variables y las entradas y salidas de datos
Grove_LCD_RGB_Backlight rgbLCD(PB_9, PB_8);
float dato = 0.0;
float datoMin = 0.0;
float datoMax = 0.0;
float masa;
float masa2;
AnalogIn datoV(A3);
DigitalIn boton(D4);
char masa_pantalla[16];
float Vout;
float minimo;
float maximo;
float tension;
float tension2;
int i;
float resolucion = 10.0; // error admisible en porcentaje
float error = resolucion;
Timer timer;

I2C i2c(I2C_SDA, I2C_SCL); //Con esta linea se inicializa la pantalla
enum estados { //Definición de los estados
  inicial,
  ajusteMinimo,
  ajusteMaximo,
  Medicion,
} estado;

float tomaDeDatos() { //Esta función toma 1000 datos durante 1 segundo, es decir, toma datos cada 1ms
  int contador = 0;
  float promedio = 0.0;
  wait_us(100000); //se espera 0.1 segundos, para estabilizar la báscula, igual se está colocando cuando se pulsa el boton,
  //o al pulsar el boton se toca la mesa donde se ssitua la báscula etc

  for (i = 1; i <= 1000; i++) {

    promedio = 0.9 * promedio + 0.1 * datoV.read_voltage(); //Mediante esta ecuación se implementa un filtro de digital, de tal manera que la toma de datos sea más fiable.
    contador++; // se va aumentando el valor del contador cada vez que se dume un dato a promedio
    wait_us(1000);
  }

  promedio = promedio / contador; //ahora si, cuando se termina el for se calcula la media
  return promedio;//y la función devuelve dicha media
}

// Esta función calcula el valor en masa en base a la tensión de salida del
// acondicionamiento de señal
float calcularMasa(float Vout) { //Mediante esta función se calcula la masa a la que corresponde una tensión

  masa = (Vout - minimo) * 50 / (maximo - minimo);//Mediante una interpolacion de los datos máximos y mínimos se calcula a que masa corresponde
  //un valor concreto de tensión, que correpsonde a la asignada a la variable Vout 
  dato = dato * 0.9 + 0.1 * masa; //se vuelve a utilizar la ecuación de la linea 42, será un filtro digital ...

  return masa;
}

void funcionInicial() {//función inicial del programa, inicializa todo el proceso de toma de datos 
  rgbLCD.locate(0, 0); //esta función indica donde se comenzará a escribir en la pantalla, siendo las coordenadas
  //(posición en la linea, fila)
  rgbLCD.print("Buenas tardes"); //corresponde al printf de la pantalla basicamente
  rgbLCD.locate(0, 1);
  rgbLCD.print("abonados");
  wait_us(1000000); /// espera 1 segundo para que se pueda leer el mensaje
  estado = ajusteMinimo;
}

void funcionAjusteMinimo() { //con este estado se asignará la tensión que corresponda a masa=0
  rgbLCD.locate(0, 0);
  rgbLCD.print("No coloquen     ");
  rgbLCD.locate(0, 1);
  rgbLCD.print("ninguna carga   ");
  if (boton == 1) { //cuando se pulse el boton se inicializará la toma de datos para calcular dicho valor
    rgbLCD.locate(0, 0);
    rgbLCD.print("Calculando...    ");
    rgbLCD.locate(0, 1);
    rgbLCD.print("                ");
    minimo = tomaDeDatos();
    estado = ajusteMaximo;
  }
}

void funcionAjusteMaximo() { // procediendo de manera análoga se calcula la masa correspondiente a 50g,
// que será la masa máxima que leerá la báscula
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

void funcionMedicion() { //una vez asignadas las tensiones máximas y minimas, se calcula la masa a la que corresponde la que se pretende medir
  rgbLCD.locate(0, 0);
  rgbLCD.print("Coloquen la pesa");
  rgbLCD.locate(0, 1);
  rgbLCD.print("                ");
  if (boton == 1) {
    rgbLCD.locate(0, 0);
    rgbLCD.print("Calculando...    ");
    rgbLCD.locate(0, 1);
    rgbLCD.print("                ");
    timer.reset();
    timer.start(); //se inicia un temporizador
    while (error >= resolucion||timer.read()<60.0) { //este while hace repetir el proceso hasta que la diferencia entre una medición y
    //otra realizada despues sea menor al error asignado en la definición de variables en procentaje, si el error es mayor a dicho porcentaje
    //se vuelve a realizar la medición hasta que se cumpla, o haya pasado 1 min 
      tension = tomaDeDatos();
      masa = calcularMasa(tension);
      tension2 = tomaDeDatos();
      masa2 = calcularMasa(tension2);
      error = (masa2 - masa) / masa * 100.0;
    }
    printf("%f", masa);
    sprintf(masa_pantalla, "%f", masa); //el valor medido se transforma en char para poder pasarlo por la pantalla
    printf(masa_pantalla);
    rgbLCD.locate(0, 0);
    rgbLCD.print("masa=           ");
    rgbLCD.locate(0, 1);
    rgbLCD.print("                ");
    rgbLCD.locate(1, 1);
    rgbLCD.print(masa_pantalla);
    rgbLCD.locate(15, 1);
    rgbLCD.print("g");
    wait_us(10000000); //se espera 10 segundos para poder volver a realizar una medidición 
  }
}

int main() {
  datoV.set_reference_voltage(3.3); //se declara la tensión máxima que se le asignata al datoV, que será la lectura del pin A3
  rgbLCD.setRGB(0xff, 0xff, 0xff); //Se asigna el color del fondo de la pantalla

  estado = inicial; //definción del estado inicial

  while (1) {
    switch (estado) { //se asigna a los estados su función correspondiente
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
  }
}
