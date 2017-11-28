#define BLYNK_PRINT Serial
#include <BlynkSimpleUIPEthernet.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "e0f56f5d74f04822aa10996688015ec5";

byte comandoBlynk;
byte estadoBlynk = 0;

BLYNK_WRITE(V0)
{
  comandoBlynk = param.asInt();
}


