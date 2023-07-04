#include <RF24.h>

/*
 * Este el codigo para una shield de motores puente en H
 * diseñadp por: José Antonio Martín pretel
 * fecha de cracion: 11/3/2019
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

int lado_derecho = 0;
int lado_izquierdo = 0;

// variables de entrada de control
struct datos
{
  int valor_codo;
  int valor_muneca_derez = 255;
  int valor_muneca_iz = 255;
  int valor_dedo;
} entrada;

//diferentes funciones
void adelante(byte derecha, byte izquierda);
void atras(byte derecha, byte izquierda);
void derecha(byte v_comun);
void izquierda(byte v_comun);
void oblicua_derecha();
void oblicua_izquierda();
void rotacion();
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
  // dirreción
  if ((entrada.valor_dedo < 40) && (entrada.valor_muneca_derez > 200) && (entrada.valor_muneca_iz > 200))
    adelante(lado_derecho, lado_izquierdo);
  else if ((entrada.valor_dedo > 40) && (entrada.valor_muneca_derez > 200) && (entrada.valor_muneca_iz > 200))
    atras(lado_derecho, lado_izquierdo);
  /*      el valor de lado derecho debe ser dirrectamente proporcional
 *      con ek valor del codo, para que este nunca sea mayor que el valor del codo
 *      y el rango que nos movemos es de 0 a 200
 *      regla de 3
 *      valor_muñeca_iz----200
 *      lado_derecho-----entrada. valor_codo     lado_derecho=(valor_muñeca_iz*entrada. valor_codo)/200
 */

  else if ((entrada.valor_muneca_iz < 200) && (entrada.valor_codo > 50))
    lado_derecho = (entrada.valor_muneca_iz * entrada.valor_codo) / 200;
  else if ((entrada.valor_muneca_derez < 200) && (entrada.valor_codo > 50))
    lado_izquierdo = (entrada.valor_muneca_derez * entrada.valor_codo) / 200;

  else if ((entrada.valor_muneca_derez < 200) && (entrada.valor_codo < 50))
    //izquierda(entrada.valor_muneca_derez);
    izquierda(100);
  else if ((entrada.valor_muneca_iz < 200) && (entrada.valor_codo < 50))
    //derecha(entrada.valor_muneca_iz);
    derecha(100);
  else
  {
    analogWrite(EN_C1, entrada.valor_codo);
    analogWrite(EN_C2, entrada.valor_codo);
  } //else
  lado_derecho = entrada.valor_codo;
  lado_izquierdo = entrada.valor_codo;
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
    Serial.print("valor de codo: ");
    Serial.println(entrada.valor_codo);
    Serial.print("valor de muñeca izquierda: ");
    Serial.println(entrada.valor_muneca_iz);
    Serial.print("valor de muñeca derecha: ");
    Serial.println(entrada.valor_muneca_derez);
    Serial.print("valor del dedo: ");
    Serial.println(entrada.valor_dedo);
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

void rotacion()
{
  digitalWrite(IN1_C1, HIGH);
  digitalWrite(IN2_C1, LOW);
  digitalWrite(IN3_C1, HIGH);
  digitalWrite(IN4_C1, LOW);
  analogWrite(EN_C1, 180);

  digitalWrite(IN1_C2, LOW);
  digitalWrite(IN2_C2, HIGH);
  digitalWrite(IN3_C2, LOW);
  digitalWrite(IN4_C2, HIGH);
  analogWrite(EN_C2, 180);
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
  rotacion();
  delay(2000);
}
