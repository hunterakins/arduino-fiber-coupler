#include <Servo.h>
#include <math.h>

Servo myservo;

int pos = 0;



void setup() {
  myservo.attach(13);
  myservo.write(0);
}

int tot = 0;    /*time in milliseconds */
void loop() {
    while (pos < 180) {
      myservo.write(pos);
      pos += 20;
      delay(100);
    }
    while (pos > 0) {
      myservo.write(pos);
      pos -= 20;
      delay(100);
    }
}

