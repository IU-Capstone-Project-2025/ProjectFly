#define TRIG 11
#define ECHO 12

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  digitalWrite(TRIG, 0);
  Serial.begin(9600);
}

uint32_t sonar() {
  digitalWrite(TRIG, 1);
  delayMicroseconds(10);
  digitalWrite(TRIG, 0);

  uint32_t dur = pulseIn(ECHO, HIGH);
  return dur / 58;
}

void loop() {
  Serial.println(sonar());
  delay(20);
}
