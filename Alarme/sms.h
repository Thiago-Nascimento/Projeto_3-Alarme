#include "RestClient.h"
#include <UIPEthernet.h>

const char* sid = "ACa9e8b4488a86ad0105ba30f525046f15";
const char* token = "c245f983935f9cd6a8b9348b8e171217";
const char* to = "5511999191645";//
const char* from = "14352362364";

String response = "";
String parametros = "sid=";

EthernetClient ethclient;

RestClient client = RestClient("192.168.3.186", 3000, ethclient);

int smsEnviado;

int smsSend(int smsEnviado)
{
  if (smsEnviado == 0)
  {
    int statusCode = client.post("/sms", parametros.c_str(), &response);
    Serial.print(F("Status da resposta: "));
    Serial.println(statusCode);
    Serial.print(F("Resposta do servidor: "));
    Serial.println(response);
    smsEnviado = 1;
  }
  return smsEnviado;
}

