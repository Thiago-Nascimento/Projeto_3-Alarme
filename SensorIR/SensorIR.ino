
int receptor = A5;

void setup() 
{
  Serial.begin(9600);
  pinMode(receptor, INPUT);
}

void loop() 
{
  int estado = digitalRead(receptor);
  Serial.println(estado);
  delay(500);
}
