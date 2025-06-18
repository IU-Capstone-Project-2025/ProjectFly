const int reeds[4] = {2, 4, 5, 6};


void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("---- New Reading ----");
  for (int i = 0; i<(sizeof(reeds)/sizeof(reeds[0])); i++){
    int raw = digitalRead(reeds[i]);
    

    if (raw == HIGH)
        Serial.println("Магнитное поле обнаружено!");
    else 
        Serial.println("No.");
    delay(500);
  }
}
