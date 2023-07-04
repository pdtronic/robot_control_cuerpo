#include <VirtualWire.h>

/*
 * este el codigo para la shiel de sensores flexibles 
 * diseñado por Jose Antonio Martin Pretel
 * fecha: 6/04/2019
 */

#define CODO A0
#define MUNECA_IZ A1
#define MUNECA_DE A2
#define DEDO A3

int valor_codo_inicial;
int valor_codo;
int valor_codo_8bit;

int valor_muneca_iz_inicial;
int valor_muneca_iz;
int valor_muneca_iz_8bit;

int valor_muneca_derez_inicial;
int valor_muneca_derez;
int valor_muneca_derez_8bit;

int valor_dedo_inicial;
int valor_dedo;
int valor_dedo_8bit;


void comunicacion();
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);

vw_setup(2000);
vw_set_tx_pin(2);

valor_codo_inicial=analogRead(CODO);
valor_muneca_iz_inicial=analogRead(MUNECA_IZ);
valor_muneca_derez_inicial=analogRead(MUNECA_DE);
valor_dedo_inicial=analogRead(DEDO);
}

void loop() {
  // put your main code here, to run repeatedly:

valor_codo=analogRead(CODO)- valor_codo_inicial;
valor_muneca_iz=analogRead(MUNECA_IZ)- valor_muneca_iz_inicial;
valor_muneca_derez=analogRead(MUNECA_DE)- valor_muneca_derez_inicial;
valor_dedo=analogRead(DEDO)- valor_dedo_inicial;

valor_codo_8bit=map(valor_codo,0,130,0,255);
valor_muneca_iz_8bit=map(valor_muneca_iz,0,130,255,0);
valor_muneca_derez_8bit=map(valor_muneca_derez,0,130,255,0);
valor_dedo_8bit=map(valor_dedo,0,130,0,255);

if(valor_codo_8bit>255) valor_codo_8bit=255; 
if(valor_muneca_iz_8bit>255) valor_muneca_iz_8bit=255;
if(valor_muneca_derez_8bit>255) valor_muneca_derez_8bit=255;
if(valor_dedo_8bit>255) valor_dedo_8bit=255;

if(valor_codo_8bit<0) valor_codo_8bit=0; 
if(valor_muneca_iz_8bit<0) valor_muneca_iz_8bit=0;
if(valor_muneca_derez_8bit<0) valor_muneca_derez_8bit=0;
if(valor_dedo_8bit<0) valor_dedo_8bit=0;
comunicacion();
}

void comunicacion(){

Serial.print("codo: ");
Serial.println(valor_codo_8bit);
Serial.print("MUÑENA izquierda: ");
Serial.println(valor_muneca_iz_8bit);
Serial.print("muñeca derecha: ");
Serial.println(valor_muneca_derez_8bit);
Serial.print("dedo: ");
Serial.println(valor_dedo_8bit);

  char aray[VW_MAX_MESSAGE_LEN];   
String str="";

str="a"+String(valor_codo_8bit);
str.toCharArray(aray,sizeof(aray));
vw_send((uint8_t*)aray,strlen(aray));
vw_wait_tx();

str="b"+String(valor_muneca_iz_8bit);
str.toCharArray(aray,sizeof(aray));
vw_send((uint8_t*)aray,strlen(aray));
vw_wait_tx();

str="c"+String(valor_muneca_derez_8bit);
str.toCharArray(aray,sizeof(aray));
vw_send((uint8_t*)aray,strlen(aray));
vw_wait_tx();

str="d"+String(valor_dedo_8bit);
str.toCharArray(aray,sizeof(aray));
vw_send((uint8_t*)aray,strlen(aray));
vw_wait_tx();
}
