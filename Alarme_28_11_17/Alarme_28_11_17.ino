#include <Keypad.h>
#include <AsyncDelay.h>
#include <LiquidCrystal.h>
#include <UIPEthernet.h>
#include <utility/logging.h>
#include "RestClient.h"

AsyncDelay delay_10s;
AsyncDelay delayLedBlink;
AsyncDelay delayLCD;
LiquidCrystal lcd(31, 33, 35, 37, 39, 41);
EthernetClient ethclient;

bool digitandoSenha = false;
bool estadoAlarme = false;

String senhaDigitada;
String senha = "1577";

const int ledVerde = A4;
const int ledAmarelo = 45;
const int receptor = A5;
const int buzzer = 9;

int valorSensor = 0;

#define ATIVADO 1
#define ATIVANDO 2
#define DESATIVADO 3
#define PREALARME 4
#define DISPARADO 5
#define Luz_Fundo  43

byte estado = DESATIVADO;

boolean yellowLed = LOW;

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
const byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x21 };

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {5, 4, 3, A0}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

RestClient client = RestClient("192.168.3.186", 3000, ethclient);
const char* sid = "ACa9e8b4488a86ad0105ba30f525046f15";
const char* token = "c245f983935f9cd6a8b9348b8e171217";
const char* to = "5511999191645";//
const char* from = "14352362364";

String response = "";
String parametros = "sid=";

int smsEnviado;
  
char aux[1] = {'.'};

void setup() {
  Serial.begin(9600);
  pinMode(receptor, INPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);

  lcd.begin(16, 2); //Inicializa LCD
  lcd.clear();     //Limpa o LCD
  pinMode(Luz_Fundo,OUTPUT); //define o pino como saída
  digitalWrite(Luz_Fundo,HIGH); // Liga a luz do display.
  escreveLCD("Conectando","");
  
  delay_10s.start(10000, AsyncDelay::MILLIS);
  delayLedBlink.start(500, AsyncDelay::MILLIS);
 
  if(Ethernet.begin(mac)) 
  {
    Serial.println("Conectado via DHCP");
    Serial.print("IP recebido:"); Serial.println(Ethernet.localIP());
  }
  else
  {
    escreveLCD("Erro ao ","Conectar");
    delay(1000);
  }

  escreveLCD("Sistema pronto","");
  delayLCD.start(5000, AsyncDelay::MILLIS);
  
  parametros.concat(sid);

  parametros.concat("&token=");
  parametros.concat(token);

  parametros.concat("&to=");
  parametros.concat(to);

  parametros.concat("&from=");
  parametros.concat(from);

  parametros.concat("&body=SUA CASA FOI INVADIDA");

  Serial.println(parametros);

  
}

void loop() {
  char key = keypad.getKey();
  valorSensor = digitalRead(receptor);
  int flag1;
  boolean flag2 = false;

  switch (estado) {
    case ATIVADO:
      if(verificaSenha(key, &digitandoSenha, &senhaDigitada, &senha)==3){
        delayLCD.expire();
        delayLCD.repeat();
        digitalWrite(Luz_Fundo,HIGH); 
        escreveLCD("Alarme","desativado");
        Serial.println("Alarme desativado.");
        estado = DESATIVADO;
        digitalWrite(ledVerde, LOW);
        smsEnviado = 0;
      }
      if (valorSensor == 1) {
        estado = PREALARME;
        delay_10s.expire();
        delay_10s.repeat();
      }
      break;
    case ATIVANDO:
      flag1 = verificaSenha(key, &digitandoSenha, &senhaDigitada, &senha);
      if(flag1 == 2){
        flag2 = true;
      }
     
      if(flag1==3){
        delayLCD.expire();
        delayLCD.repeat();
        digitalWrite(Luz_Fundo,HIGH); 
        escreveLCD("Alarme","desativado");
        Serial.println("Alarme desativado.");
        estado = DESATIVADO;
        digitalWrite(ledVerde, LOW);
        digitalWrite(ledAmarelo, LOW);   
      }
      if(!flag2 && delayLedBlink.isExpired()){
        delayLCD.expire();
        delayLCD.repeat();
        digitalWrite(Luz_Fundo,HIGH); 
        lcd.setCursor(11,1);
        lcd.print(aux);
        if(aux[0]=='.')
          aux[0] = ' ';
        else
          aux[0] = '.';
      }
      if(delayLedBlink.isExpired()){
        digitalWrite(ledAmarelo, !digitalRead(ledAmarelo));
        delayLedBlink.repeat();
      }
      if (delay_10s.isExpired()) {
        estado = ATIVADO;
        delayLCD.expire();
        delayLCD.repeat();
        digitalWrite(Luz_Fundo,HIGH); 
        escreveLCD("Alarme","ativado");
        Serial.println("Alarme Ativado");
        digitalWrite(ledAmarelo, LOW);        
        digitalWrite(ledVerde, HIGH);
      }
      break;

    case DESATIVADO:
      if(verificaSenha(key, &digitandoSenha, &senhaDigitada, &senha)==3){
        delayLCD.expire();
        delayLCD.repeat();
        digitalWrite(Luz_Fundo,HIGH); 
        escreveLCD("Alarme","ativando...");
        Serial.println("Alarme ativando.");
        estado = ATIVANDO;
        delay_10s.expire();
        delay_10s.repeat();
        delayLedBlink.expire();
        delayLedBlink.repeat();
        digitalWrite(ledAmarelo, HIGH);
        flag2 = false;
      }
      break;
    case PREALARME:
      if(verificaSenha(key, &digitandoSenha, &senhaDigitada, &senha)==3){
        delayLCD.expire();
        delayLCD.repeat();
        digitalWrite(Luz_Fundo,HIGH); 
        escreveLCD("Alarme","desativado");
        Serial.println("Alarme desativado.");
        estado = DESATIVADO;
        digitalWrite(ledVerde, LOW);
      }
      if (delay_10s.isExpired()) {
        estado = DISPARADO;
        delayLCD.expire();
        delayLCD.repeat();
        digitalWrite(Luz_Fundo,HIGH); 
        escreveLCD("Pega","Ladrao!");
        Serial.println("Alarme Disparado");
        digitalWrite(ledVerde, LOW);
        digitalWrite(ledAmarelo, HIGH);
        delayLedBlink.expire();
        delayLedBlink.repeat();
      }
      break;
    case DISPARADO:
      tone(buzzer,7000);

      if(smsEnviado == 0)
      {
        int statusCode = client.post("/sms", parametros.c_str(), &response);
        Serial.print("Status da resposta: ");
        Serial.println(statusCode);
        Serial.print("Resposta do servidor: ");
        Serial.println(response);
        smsEnviado = 1;
      }
      
      if(delayLedBlink.isExpired()){
        digitalWrite(ledVerde, !digitalRead(ledVerde));
        digitalWrite(ledAmarelo, !digitalRead(ledVerde));
        delayLedBlink.repeat();
      }
      if(verificaSenha(key, &digitandoSenha, &senhaDigitada, &senha)==3){
        delayLCD.expire();
        delayLCD.repeat();
        digitalWrite(Luz_Fundo,HIGH); 
        escreveLCD("Alarme","desativado");
        Serial.println("Alarme desativado.");
        estado = DESATIVADO;
        noTone(buzzer);
        digitalWrite(ledVerde, LOW);
        digitalWrite(ledAmarelo, LOW);
      }
      break;
  }
  if(delayLCD.isExpired()){
    digitalWrite(Luz_Fundo,LOW);
    lcd.clear();
  }
}

////////////////////////////
//FUNÇÕES
///////////////////////////
boolean estadoPorta() {
  if (valorSensor == 1 ) {
    Serial.println("Porta Aberta");
    delay(1000);
    return true;
  }
  else {
    Serial.println("Porta Fechada");
    delay(1000);
    return false;
  }
}
/*Alarme desativado*/
void identificaInvasao(boolean estadoPorta, boolean estadoAlarme) {

  /*Porta aberta e alarme ativado*/
  if (estadoPorta == true && estadoAlarme == true) {
    Serial.println("Fomos invadidos");
  }
  /*Porta aberta e alarme desativado*/
  if (estadoPorta == true && estadoAlarme == false) {
    Serial.println("Entrada autorizada");
  }
}
/*Alarme desativado*/
void alarmeDesativado() {

}
/*Alarme Ativado*/
void alarmeAtivado() {

}
/*Alarme Ativando*/
void alarmeAtivando() {

}

int verificaSenha(char key, boolean *digitandoSenha, String *senhaDigitada, String *senha){

 if(key){
   if(key=='*'){
     (*senhaDigitada) = "";
     (*digitandoSenha) = true;
     delayLCD.expire();
     delayLCD.repeat();
     digitalWrite(Luz_Fundo,HIGH); 
     escreveLCD("Digitar senha:","");
     Serial.println("digitando senha....");
     return 1;
   }
   if(key=='#' && (*digitandoSenha)){
     int aux = 0;
     Serial.println("Finalizou senha");
     if((*senhaDigitada) == (*senha)){
       aux = 3;
     }
     else{
       delayLCD.expire();
       delayLCD.repeat();
       digitalWrite(Luz_Fundo,HIGH); 
       escreveLCD("Senha incorreta","");
       Serial.println("Senha incorreta");
       aux = -1;
     }
     (*senhaDigitada) = "";
     (*digitandoSenha) = false;
     return aux;
   }
   if(*digitandoSenha){
      if(*senhaDigitada==""){
        delayLCD.expire();
        delayLCD.repeat();
        digitalWrite(Luz_Fundo,HIGH); 
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("*");
      }
      else{
        delayLCD.expire();
        delayLCD.repeat();
        digitalWrite(Luz_Fundo,HIGH); 
        lcd.print("*");
      }
     (*senhaDigitada) += key;
     Serial.println(*senhaDigitada);
     return 2;
   }
 }
 return 0;
}

void escreveLCD(char *linha1, char *linha2){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(linha1);
  lcd.setCursor(0,1);
  lcd.print(linha2);
}
