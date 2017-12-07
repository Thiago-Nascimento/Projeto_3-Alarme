#include <Keypad.h>
#include <Wire.h>

bool digitandoSenha = false;
String senhaDigitada;
//char* senha = "1577";

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns

char keys[ROWS][COLS] = {  
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {5, 4, 3, 2};   //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6};       //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup()
{
  Wire.begin();  
  Serial.begin(9600);
  pinMode(2, INPUT);
}
  
void loop()
{
  char key = keypad.getKey();
  
  if (key)
  {
    
    if (digitandoSenha && key != '#')
    {
      senhaDigitada += key;
      Serial.println(senhaDigitada);
    }
    
    if(key == '*')
    {
      senhaDigitada = "";
      digitandoSenha = true;
      Serial.println("Digitando Senha...");        
    }
    
    if (key == '#')
    {
      char teste[10];
      
      digitandoSenha = false;
      Serial.println("Senha Digitada...");
      Serial.print("Senha: ");
      Serial.println(senhaDigitada);

      senhaDigitada.toCharArray(teste, 10);

      Serial.println(teste);
      
//      if (senhaDigitada == senha)
//      {
//        Serial.println("");
//        Serial.println("Senha Correta");
//        Serial.println("Alarme Desativado");
//        Serial.println("");
//      }
//      else
//      {
//        Serial.println("ERROU");
//        Serial.println("");
      }
    }
  }

