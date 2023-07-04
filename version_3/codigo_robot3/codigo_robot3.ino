#include <RF24.h>

/*
   Este el codigo para una shield de motores puente en H
   diseñadp por: José Antonio Martín pretel
   fecha de cracion: 11/3/2019
*/
// definimos los pines a los que van a ir conectados los pines del L293D
#define IN1_C1 8
#define IN2_C1 7
#define IN3_C1 3
#define IN4_C1 4
#define IN1_C2 A1
#define IN2_C2 A0
#define IN3_C2 A2
#define IN4_C2 A3
#define EN_C1 5
#define EN_C2 6

#define CE 9
#define CSN 10

RF24 radio(CE, CSN);

byte direcion[][6] = {"0"};

uint8_t lado_derecho = 0;
uint8_t lado_izquierdo = 0;

// variables de entrada de control
struct datos
{
  uint8_t valor_aceleracion;
  uint8_t valor_derecha;
  uint8_t valor_izquierda;
  uint8_t valor_direccion;
  //uint8_t rotacion;
} entrada;

//diferentes funciones
void adelante(byte derecha, byte izquierda);
void atras(byte derecha, byte izquierda);
void derecha(byte v_comun);
void izquierda(byte v_comun);
void oblicua_derecha();
void oblicua_izquierda();
void rotacion(byte v_comun);
void presentacion();

void comunicacion();
void control();

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(100);
  radio.begin();
  radio.setChannel(115);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, direcion[0]);
  radio.startListening();

  // for(int x=6;x<14;x++)   pinMode(x,OUTPUT); // declaracion de los pines de los motores
  pinMode(IN1_C1, OUTPUT);
  pinMode(IN2_C1, OUTPUT);
  pinMode(IN3_C1, OUTPUT);
  pinMode(IN4_C1, OUTPUT);
  pinMode(EN_C1, OUTPUT);
  pinMode(IN1_C2, OUTPUT);
  pinMode(IN2_C2, OUTPUT);
  pinMode(IN3_C2, OUTPUT);
  pinMode(IN4_C2, OUTPUT);
  pinMode(EN_C2, OUTPUT);
}

void loop()
{
  control();
  //presentacion();
}

void control()
{
  comunicacion();
  lado_derecho = entrada.valor_aceleracion;
  lado_izquierdo = entrada.valor_aceleracion;

  // dirreción
   if ((entrada.valor_direccion < 40) && (entrada.valor_derecha < 40) && (entrada.valor_izquierda < 40))
    adelante(lado_derecho, lado_izquierdo);
  else if ((entrada.valor_direccion > 40) && (entrada.valor_derecha < 40) && (entrada.valor_izquierda < 40))
    atras(lado_derecho, lado_izquierdo);
    if (entrada.valor_derecha > 40)
    derecha(entrada.valor_derecha);
  else if (entrada.valor_izquierda > 40)
    izquierda(entrada.valor_izquierda);
 
} //control

//si la muñena izquirda la muevo mas de x, q las ruedas del lado derecho disminuña x
//si la muñena derecha la muevo mas de x, q las ruedas del lado izquierdo disminuña x
// si no, que las 4 tengan la misma velocidad

void comunicacion()
{
  if (radio.available())
  {
    while (radio.available())
    {
      radio.read(&entrada, sizeof(entrada));
    }
//    Serial.println(entrada.rotacion);
    Serial.print("valor de dirrecion: ");
    Serial.println(entrada.valor_direccion);
    /*Serial.print("valor de aceleracion: ");
    Serial.println(entrada.valor_aceleracion);
    Serial.print("valor izquierda: ");
    Serial.println(entrada.valor_izquierda);
    Serial.print("valor derecha: ");
    Serial.println(entrada.valor_derecha);
    Serial.print("valor de dirrecion: ");
    Serial.println(entrada.valor_direccion);*/
  }
}

void adelante(byte derecha, byte izquierda)
{
  //este el lado derecho
  digitalWrite(IN1_C1, HIGH);
  digitalWrite(IN2_C1, LOW);
  digitalWrite(IN3_C1, LOW);
  digitalWrite(IN4_C1, HIGH);
  analogWrite(EN_C1, derecha); // minimo para que empieze a andar 80

  // este es el lado izquierdo
  digitalWrite(IN1_C2, LOW);
  digitalWrite(IN2_C2, HIGH);
  digitalWrite(IN3_C2, HIGH);
  digitalWrite(IN4_C2, LOW);
  analogWrite(EN_C2, izquierda);
}

void atras(byte derecha, byte izquierda)
{
  digitalWrite(IN1_C1, LOW);
  digitalWrite(IN2_C1, HIGH);
  digitalWrite(IN3_C1, HIGH);
  digitalWrite(IN4_C1, LOW);
  analogWrite(EN_C1, derecha);
  digitalWrite(IN1_C2, HIGH);
  digitalWrite(IN2_C2, LOW);
  digitalWrite(IN3_C2, LOW);
  digitalWrite(IN4_C2, HIGH);
  analogWrite(EN_C2, izquierda);
}

void derecha(byte v_comun)
{
  digitalWrite(IN1_C1, HIGH);
  digitalWrite(IN2_C1, LOW);
  digitalWrite(IN3_C1, HIGH);
  digitalWrite(IN4_C1, LOW);
  analogWrite(EN_C1, v_comun);

  digitalWrite(IN1_C2, HIGH);
  digitalWrite(IN2_C2, LOW);
  digitalWrite(IN3_C2, HIGH);
  digitalWrite(IN4_C2, LOW);
  analogWrite(EN_C2, v_comun);
}

void izquierda(byte v_comun)
{
  digitalWrite(IN1_C1, LOW);
  digitalWrite(IN2_C1, HIGH);
  digitalWrite(IN3_C1, LOW);
  digitalWrite(IN4_C1, HIGH);
  analogWrite(EN_C1, v_comun);

  digitalWrite(IN1_C2, LOW);
  digitalWrite(IN2_C2, HIGH);
  digitalWrite(IN3_C2, LOW);
  digitalWrite(IN4_C2, HIGH);
  analogWrite(EN_C2, v_comun);
}

void oblicua_derecha()
{
  digitalWrite(IN1_C1, HIGH);
  digitalWrite(IN2_C1, LOW);
  digitalWrite(IN3_C1, LOW);
  digitalWrite(IN4_C1, LOW);
  analogWrite(EN_C1, 180);

  digitalWrite(IN1_C2, LOW);
  digitalWrite(IN2_C2, LOW);
  digitalWrite(IN3_C2, HIGH);
  digitalWrite(IN4_C2, LOW);
  analogWrite(EN_C2, 180);
}

void oblicua_izquierda()
{
  digitalWrite(IN1_C1, LOW);
  digitalWrite(IN2_C1, LOW);
  digitalWrite(IN3_C1, LOW);
  digitalWrite(IN4_C1, HIGH);
  analogWrite(EN_C1, 180);

  digitalWrite(IN1_C2, LOW);
  digitalWrite(IN2_C2, HIGH);
  digitalWrite(IN3_C2, LOW);
  digitalWrite(IN4_C2, LOW);
  analogWrite(EN_C2, 180);
}

void rotacion(byte v_comun)
{
  digitalWrite(IN1_C1, HIGH);
  digitalWrite(IN2_C1, LOW);
  digitalWrite(IN3_C1, HIGH);
  digitalWrite(IN4_C1, LOW);
  analogWrite(EN_C1, v_comun);

  digitalWrite(IN1_C2, LOW);
  digitalWrite(IN2_C2, HIGH);
  digitalWrite(IN3_C2, LOW);
  digitalWrite(IN4_C2, HIGH);
  analogWrite(EN_C2, v_comun);
}
void presentacion()
{
  adelante(180, 180);
  delay(2000);
  atras(180, 180);
  delay(2000);
  derecha(180);
  delay(2000);
  izquierda(180);
  delay(2000);
  oblicua_derecha();
  delay(2000);
  oblicua_izquierda();
  delay(2000);
  rotacion(180);
  delay(2000);
}
