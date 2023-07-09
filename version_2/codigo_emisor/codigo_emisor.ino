/*
 * este el codigo para la shiel de sensores flexibles 
 * diseñado por Jose Antonio Martin Pretel
 * fecha: 15/09/2019
 */
#include <RF24.h>

#define CODO A0
#define MUNECA_IZ A1
#define MUNECA_DE A2
#define DEDO A3

#define led 2

#define echo 4
#define trig 5

#define CE 9
#define CSN 10

RF24 radio(CE, CSN);
byte direcion[][6] = {"0"};

int transmitir;
long duracion;
int distancia;

bool bandera = false;
unsigned long tiempo_inicial;

struct datos
{
  int codo;
  int muneca_derez = 255;
  int muneca_iz = 255;
  int dedo;
} inicial, permanente, salida;

void comunicacion();
void transformacion();
void modo_1();


void sensor_flexible();
void modo_2();

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200); // monitor serial
  delay(100);
  // configuracion del nrf24l01
  radio.begin();
  radio.setChannel(115);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(direcion[0]);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(led, OUTPUT);

  // lectura de datos iniciales
  inicial.codo = analogRead(CODO);
  inicial.muneca_iz = analogRead(MUNECA_IZ);
  inicial.muneca_derez = analogRead(MUNECA_DE);
  inicial.dedo = analogRead(DEDO);
}

void loop()
{
  // put your main code here, to run repeatedly:
  switch (1)
  {
  case 1:
    modo_1();
    break;
  case 2:
    modo_2();
    break;
  }
}

void modo_1()
{ /* Este modo es para contral el coche 
  * con los 4 sensores flexibles
  */
  transformacion();
  comunicacion();
}

void comunicacion()
{
  radio.write(&salida, sizeof(salida));

  Serial.print("codo: ");
  Serial.println(salida.codo);
  Serial.print("MUÑENA izquierda: ");
  Serial.println(salida.muneca_iz);
  Serial.print("muñeca derecha: ");
  Serial.println(salida.muneca_derez);
  Serial.print("dedo: ");
  Serial.println(salida.dedo);
  delay(100);
}

void transformacion()
{
  /*esta funcion se encarga de leer los datos, ponerle limites superior
  *e inferior y convertir las variables de entrada 10bit en salida de 8 bit,
  * haciendo una recla de 3
  */
  permanente.codo = analogRead(CODO) - inicial.codo; //ponemos a 0 los valores de los sensores
  permanente.muneca_iz = analogRead(MUNECA_IZ) - inicial.muneca_iz;
  permanente.muneca_derez = analogRead(MUNECA_DE) - inicial.muneca_derez;
  permanente.dedo = analogRead(DEDO) - inicial.dedo;

  salida.codo = map(permanente.codo, 0, 130, 0, 255); //hacemos unas reglas de 3
  salida.muneca_iz = map(permanente.muneca_iz, 0, 130, 255, 0);
  salida.muneca_derez = map(permanente.muneca_derez, 0, 130, 255, 0);
  salida.dedo = map(permanente.dedo, 0, 130, 0, 255);

  if (salida.codo > 255)
    salida.codo = 255;
  if (salida.muneca_iz > 255)
    salida.muneca_iz = 255;
  if (salida.muneca_derez > 255)
    salida.muneca_derez = 255;
  if (salida.dedo > 255)
    salida.dedo = 255;
  if (salida.codo < 0)
    salida.codo = 0;
  if (salida.muneca_iz < 0)
    salida.muneca_iz = 0;
  if (salida.muneca_derez < 0)
    salida.muneca_derez = 0;
  if (salida.dedo < 0)
    salida.dedo = 0;
}

void modo_2()
{ /* este modo es para controlar el coche
  * con un sensor flexible la direcion y la velocidad
  * con el sensor de ultrasonidos. Cuando entra 
  * este modo el led parpadea
  */
  unsigned long tiempo = millis();
  if ((tiempo - tiempo_inicial) > 100)
  {
    tiempo_inicial = tiempo;
    if (bandera == false) //parpadeo del led multitarea
      bandera = true;
    else
      bandera = false;
  }
  digitalWrite(led, bandera);

  sensor_distancia();
  sensor_flexible();

  Serial.print("ultrasonidos: ");
  Serial.println(distancia);
  Serial.print("flex: ");
  Serial.println(salida.dedo);
  radio.write(&salida, sizeof(salida)); //envia los datos
}

void sensor_distancia()
{
  //codigo para leer el sensor de ultrasonidos
  digitalWrite(trig, LOW);
  delayMicroseconds(5);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  duracion = pulseIn(echo, HIGH);
  distancia = duracion * 0.034 / 2;

  if (distancia > 40) //limite superior de 40
    distancia = 40;

  if (distancia < 5)
    distancia = 0;
  salida.codo = map(distancia, 40, 0, 0, 255); // es una regla de 3 directamente inversa
}

void sensor_flexible()
{
  permanente.dedo = analogRead(DEDO) - inicial.dedo; //ponemos a 0 el valor del sensor_flexible
  salida.dedo = map(permanente.dedo, 0, 130, 0, 255);

  if (salida.dedo > 255)
    salida.dedo = 255;

  if (salida.dedo < 0)
    salida.dedo = 0;
}
