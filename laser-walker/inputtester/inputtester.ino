  
/* note: RED wire goes to 5V, BROWN wire goes to GND, ORANGE wire goes tot he digital output pin */

#include <Servo.h>
#include <math.h>

#define APIN 9  //digital output pin for motor 1
#define AREADPIN 14  // Analog read pin 
#define BPIN 12 //digital output pin for motor 2
#define POWERPIN 11
#define WAIT 1000

Servo serv1;
Servo serv2;

int intensity = 0; // adc reading of light signal
int MotorCount = 0; // controls the amount of time spent on a single motor
int newintensity = 0;
float fintensity = 0; // light intensity converted to voltage as floating point
float fnewintensity = 0;
int initial = 90;   // initial servo position
int Asign = 1;  
int Bsign = 1;
int stepsize = 20; //resolution of servo rotation (it is in degrees)
int Aservostate = initial;
int Bservostate = initial;
float Adifference; // variable that represents the new voltage reading minus the old
float Bdifference;
int dgain = 100; // sets the gain on the Adifference feedback (I want to make this a tunable parameter)
int MotorIndicator = 1; //determines which motor to operate, starts with motor 1
float threshold = .1; //determines at which point to bail on working... in Volts


void setup() {
  serv1.attach(APIN);
  serv2.attach(BPIN);
  serv1.write(initial);
  serv2.write(initial);
  Serial.begin(9600); // set up Serial library at 9600 bps
  delay(5000); // buy some time to get the motor locked in;
}
int loopnum = 1;
void loop() {
  Serial.print("Start Loop: ");
  Serial.print(loopnum);
  Serial.print("\n");
  Serial.print("Motor: ");
  Serial.print(MotorIndicator);
  Serial.print("\n");
  intensity = analogRead(AREADPIN);
  fintensity = 5.0 * intensity / 1024; //turn it into a voltage reading, a float
  Serial.println("Voltage Reading: ");
  Serial.println(fintensity, 5);
  if ((MotorIndicator == 1) and (Aservostate < 180) and (Aservostate > 0)) {
    if (MotorCount > 20) {
      MotorIndicator = 2;
      MotorCount = 0;
    }
    MotorCount += 1;
    Serial.println("Motor is now: ");
    Serial.println(MotorIndicator);
    Aservostate = Aservostate + Asign*stepsize;
    Serial.println("Aservostate: ");
    Serial.println(Aservostate);
    delay(100);
    serv1.write(Aservostate);   //rotate the servo, attached to mirror mount
    delay(2000);
    newintensity = analogRead(AREADPIN); // get new reading
    fnewintensity = 5.0 * newintensity / 1024; //turn it into a voltage reading, a float
    Serial.println("New Voltage: ");
    Serial.println(fnewintensity);
    Adifference = fnewintensity - fintensity;
    Serial.println("Adifference: ");
    Serial.println(Adifference);
    if (abs(Adifference) < threshold) {
      MotorIndicator = 2;
      MotorCount = 0;
      Asign = Asign * -1;
      Aservostate = Aservostate + Asign*stepsize; //go back to where it was before 
    }
    else if (abs(Adifference) > threshold) { //flip direction of rotation if Adifference is negative...
      if (Adifference < 0) {
        Asign = Asign * -1;
      }
    }
  }
  else if ((MotorIndicator == 2) and (Bservostate < 180) and (Bservostate > 0)) {
    if (MotorCount > 20) {
      MotorIndicator = 1;
      MotorCount = 0;
    }
    Serial.print("Motor is now: ");
    Serial.print(MotorIndicator);
    Serial.print("\n");
    Bservostate = Bservostate + Bsign*stepsize;
    serv2.write(Bservostate);   //rotate the servo, attached to mirror mount
    delay(2000);
    newintensity = analogRead(AREADPIN); // get new reading
    fnewintensity = 5.0 * newintensity / 1024; //turn it into a voltage reading, a float
    Serial.println("New Voltage: ");
    Serial.println(fnewintensity);
    Bdifference = fnewintensity - fintensity;
    Serial.println("Bdifference: ");
    Serial.println(Bdifference);
    if (abs(Bdifference) < threshold) {
      MotorIndicator = 1;
      MotorCount = 0;
      Bsign = Bsign * -1;
      Bservostate = Bservostate + Bsign*stepsize; //go back to where it was before 
    }
    else if (abs(Bdifference) > threshold) {
      if (Bdifference < 1) {
        Bsign = Bsign * -1;
      }
    }
  }
  else if ((Aservostate < 0) || (Bservostate < 0) || (Aservostate > 180) || (Bservostate > 0)) {
    Serial.println("No more can be done...reset me please");
    //exit(0);
  }
  loopnum += 1;
  delay(4000);     
}

           
