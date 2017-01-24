#define AREADPIN 14

void setup() {
  Serial.begin(9600);
}


int val;
float fval;
void loop() {
  val = analogRead(AREADPIN);
  Serial.print("Value: ");
  Serial.print(val);
  Serial.print("\n");
  Serial.print("Converted to a voltage: ");
  fval = 5.0 * val / 1024;
  Serial.print(fval);
  delay(1000);
}
