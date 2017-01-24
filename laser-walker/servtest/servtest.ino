#include <Servo.h>
#include <math.h>

/* note: RED wire goes to 5V, BROWN wire goes to GND, ORANGE wire goes tot he digital output pin */


#define APIN 9
#define BPIN 10
#define CPIN 11 
#define DPIN 12


Servo serv1;
Servo serv2;
Servo serv3;
Servo serv4;

void setup() {
  serv1.attach(APIN);
  serv1.write(0);
  delay(400);
  serv1.write(90);
  delay(400);
  serv2.attach(BPIN);
  serv2.write(90);
  delay(400);
  serv3.attach(CPIN);
  serv3.write(90);
  delay(400);
  serv3.write(20);
  delay(400);
  serv4.attach(DPIN);
  serv4.write(90);
  delay(200);
  
}
int curr = 90;
void loop() {  
  exit(0);
  /* servo.Write() version  
   
  int max = 180;
  int step = 10;
  int min = 0;
  while (curr < max) {
    curr += step;
    serv1.write(curr);
    delay(1000);
    //serv2.write(curr);
    //delay(100);
  }
  while (curr > min) {
    curr -= step;
    serv1.write(curr);
    delay(1000);
    //serv2.write(curr);
    //delay(100);
    }
  /* writeMicroseconds version */ 
  /*int max = 3000;
  int step = 100;
  int min = 100;
  int curr = 1800;
  while (curr < max) {
    curr += step;
    serv1.writeMicroseconds(curr);
    delay(1000);
  }
  while (curr > min) {
    curr -= step;
    serv1.writeMicroseconds(curr);
    delay(1000);
    }
    */
}
