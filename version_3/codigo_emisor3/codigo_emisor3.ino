/*
   este el codigo para la transimision de datos del ordenador al robot
   diseñado por Jose Antonio Martin Pretel
   fecha: 17/05/2023
*/
#include <RF24.h>

#define CE 9
#define CSN 10

#define DEBUG 0

RF24 radio(CE, CSN);
byte direcion[][6] = {"0"};

String inputString = "";  // Almacenar la cadena recibida
bool stringComplete = false;// Indicar que ha recibido una cadena
const char separator = ','; // Separador (,)
#define longitud 4 // Cantidad de datos que vamos recibir
uint8_t data[longitud]; // Vector que va almacenar los datos recibidos


struct datos
{
  uint8_t aceleracion;
  uint8_t derecha;
  uint8_t izquierda;
  uint8_t direcion;
  //uint8_t rotacion;
} salida;


void setup() {
  Serial.begin(115200); // monitor serial
  // configuracion del nrf24l01
  radio.begin();
  radio.setChannel(115);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(direcion[0]);
}

void loop() {
//////////////recepcion de datos//////////////
  if (stringComplete) {
    for (uint8_t i = 0; i < longitud; i++) {
      int index = inputString.indexOf(separator); //cuenta cuantos caracteres hay hasta el separador
      data[i] = inputString.substring(0, index).toInt(); //almacema desde el caracter 0 hasta el separador, y lo convierte en int
      inputString = inputString.substring(index + 1);
    }
    inputString = "";//limpiamos la cadena
    stringComplete = false; //bajamos la vandera
  }

  salida.aceleracion = data[0];
  salida.direcion  = data[1];
  salida.derecha = data[2];
  salida.izquierda = data[3];
  //salida.rotacion= data[4];


//////////////envio de datos//////////////
  radio.write(&salida, sizeof(salida));
  #if DEBUG
  Serial.print("aceleracion: ");
  Serial.println(salida.aceleracion);
  Serial.print("izquierda: ");
  Serial.println(salida.izquierda);
  Serial.print("derecha: ");
  Serial.println(salida.derecha);
  Serial.print("direcion : ");
  Serial.println(salida.direcion );
  #endif
  delay(100);
}

void serialEvent() {
  while (Serial.available()) {
    char inchar = (char)Serial.read(); //leemos caracter a caracter
    inputString += inchar; //almacenamos toda la cadena
    if (inchar == '\n') { 
      stringComplete = true;
    }
  }
}
