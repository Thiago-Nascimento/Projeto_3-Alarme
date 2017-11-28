/*#include "RestClient.h"
#include <UIPEthernet.h>

RestClient client = RestClient("192.168.3.186", 3000, ethclient);
const char* sid = "ACa9e8b4488a86ad0105ba30f525046f15";
const char* token = "c245f983935f9cd6a8b9348b8e171217";
const char* to = "5511999191645";//
const char* from = "14352362364";

String response = "";
String parametros = "sid=";

int smsEnviado;

void smsSetup(){
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

int smsSend(int smsEnviado){
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
*/
