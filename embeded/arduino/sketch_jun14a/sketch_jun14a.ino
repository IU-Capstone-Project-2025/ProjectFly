const int hallpins[5] = {A0, A1, A2, A3, A4};
const float ADCmv = 5.0/1024.0;


void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("---- New Reading ----");
  for (int i = 0; i<(sizeof(hallpins)/sizeof(hallpins[0])); i++){
    int raw = analogRead(hallpins[i]);
    float voltage = ADCmv * raw;
    Serial.print("Sensor ");
    Serial.print(i+1);
    Serial.print(": ");
    Serial.print("Raw: ");
    Serial.print(raw);
    Serial.print(", Voltage: ");
    Serial.print(voltage, 3);
    Serial.print(" V");

    if (voltage < 2.0){
      Serial.println("South poul is close         yes");
    }
    else if (voltage > 3.0) {
      Serial.println(" => North Pole is clsoe          yes");
    } else {
      Serial.println(" => No ");
    }
    delay(500);
  }
}
