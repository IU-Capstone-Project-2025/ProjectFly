#define PIN_POT A0
void setup() {
  Serial.begin(9600);
  pinMode(PIN_POT, INPUT);
};

float potValueToAngle(int potValue){
  return (potValue / 1023.0) * 200.0;
};

void loop() {
  int rotat;
  float angle;
  rotat = analogRead(PIN_POT);
  angle = potValueToAngle(rotat);

  Serial.print("Potentiometr value:3");
  Serial.print(rotat);
  Serial.print("  |  Angle: ");
  Serial.print(angle, 1);
  Serial.println(" degrees");
  delay(1);
}
