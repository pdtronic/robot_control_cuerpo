#include <VirtualWire.h>


/*
 * Este el codigo para una shield de motores puente en H
 * diseñadp por: José Antonio Martín pretel
 * fecha de cracion: 11/3/2019
 */
// definimos los pines a los que van a ir conectados los pines del L293D 
#define IN1_C1 13
#define IN2_C1 12
#define IN1_C2 8
#define IN2_C2 7
#define EN1_C1 11
#define EN2_C2 6

int lado_derecho=0;
int lado_izquierdo=0;

// variables de entrada de control
 int valor_codo=0;
 int valor_muneca_iz=0;
 int valor_muneca_derez=0;
 int valor_dedo=0;

//diferentes funciones
void adelante();
void atras();
void comunicacion();
void control();

void setup() {
  // put your setup code here, to run once:
// Serial.begin(9600); 
vw_setup(2000);
vw_set_rx_pin(0);
vw_rx_start();
// for(int x=6;x<14;x++)   pinMode(x,OUTPUT); // declaracion de los pines de los motores
pinMode(13,OUTPUT);
pinMode(12,OUTPUT);
pinMode(11,OUTPUT);
pinMode(10,INPUT);
pinMode(9,INPUT);
pinMode(8,OUTPUT);
pinMode(7,OUTPUT);
pinMode(6,OUTPUT);
}

void loop() {
//control();
//Serial.println(lado_derecho);
/*
Serial.print("valor de codo: ");
      Serial.println(valor_codo);
        Serial.print("valor de muñeca izquierda: ");
      Serial.println(valor_muneca_iz);   
      Serial.print("valor de muñeca derecha: ");
      Serial.println(valor_muneca_derez); 
       Serial.print("valor del dedo: ");
      Serial.println(valor_dedo); */
digitalWrite(IN1_C1,LOW);
digitalWrite(IN2_C1,HIGH);
analogWrite(EN1_C1,100); // minimo para que empieze a andar 40

// este es el lado izquierdo
digitalWrite(IN1_C2,LOW);
digitalWrite(IN2_C2,HIGH);
analogWrite(EN2_C2,100);
delay(2000);
digitalWrite(IN1_C1,HIGH);
digitalWrite(IN2_C1,LOW);      
digitalWrite(IN1_C2,HIGH);
digitalWrite(IN2_C2,LOW);
delay(2000);
digitalWrite(IN1_C1,HIGH);
digitalWrite(IN2_C1,LOW);      
digitalWrite(IN1_C2,LOW);
digitalWrite(IN2_C2,HIGH);
delay(2000);
digitalWrite(IN1_C1,LOW);
digitalWrite(IN2_C1,HIGH);      
digitalWrite(IN1_C2,HIGH);
digitalWrite(IN2_C2,LOW);
delay(2000);

}

//si la muñena izquirda la muevo mas de x, q las ruedas del lado derecho disminuña x 
//si la muñena derecha la muevo mas de x, q las ruedas del lado izquierdo disminuña x 
// si no, que las 4 tengan la misma velocidad
void adelante(){
 //este el lado derecho 
digitalWrite(IN1_C1,HIGH);
digitalWrite(IN2_C1,LOW);
analogWrite(EN1_C1,lado_derecho); // minimo para que empieze a andar 40

// este es el lado izquierdo
digitalWrite(IN1_C2,HIGH);
digitalWrite(IN2_C2,LOW);
analogWrite(EN2_C2,lado_izquierdo);
}


void atras(){
digitalWrite(IN1_C1,LOW);
digitalWrite(IN2_C1,HIGH);
analogWrite(EN1_C1,lado_derecho);

digitalWrite(IN1_C2,LOW);
digitalWrite(IN2_C2,HIGH);
analogWrite(EN2_C2,lado_izquierdo);
}


void comunicacion(){
uint8_t Array[VW_MAX_MESSAGE_LEN];
uint8_t largo=VW_MAX_MESSAGE_LEN;

if(vw_get_message((uint8_t*)Array,&largo)){ // si llega un dato
    int i;
    String dato_cadena="";
    if((char) Array[0]=='a'){ // lee la primera letra y si coincide con algunas de las letras programadas
      for(i=1;i<largo;i++){
           dato_cadena.concat((char)Array[i]);
          }
valor_codo=dato_cadena.toInt(); // almacena dicho valor en las diferentes variables     
        }  

     else if((char) Array[0]=='b'){
        for(i=1;i<largo;i++){
            dato_cadena.concat((char)Array[i]);
            }
         valor_muneca_iz=dato_cadena.toInt();
        }  
    else if((char) Array[0]=='c'){
         for(i=1;i<largo;i++){
             dato_cadena.concat((char)Array[i]);
            }
         valor_muneca_derez=dato_cadena.toInt();
        }  
    else if((char) Array[0]=='d'){ 
        for(i=1;i<largo;i++){
          dato_cadena.concat((char)Array[i]);
          }
        valor_dedo=dato_cadena.toInt();
        }
   }    
}

void control(){
  comunicacion();
  // dirreción
   if(valor_dedo>40){  
     adelante(); 
    }
   else{
       atras();
        }
/*      el valor de lado derecho debe ser dirrectamente proporcional
 *      con ek valor del codo, para que este nunca sea mayor que el valor del codo
 *      y el rango que nos movemos es de 0 a 200
 *      regla de 3
 *      valor_muñeca_iz----200
 *      lado_derecho-----valor_codo     lado_derecho=(valor_muñeca_iz*valor_codo)/200
 */
  if(valor_muneca_iz<200) lado_derecho= (valor_muneca_iz*valor_codo)/200;
  else if(valor_muneca_derez<200) lado_izquierdo= (valor_muneca_derez*valor_codo)/200;
 
  else {
    lado_derecho=valor_codo;
    lado_izquierdo=valor_codo; 
  }
}
