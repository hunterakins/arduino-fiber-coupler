/* Brute force version

    Program takes in as parameters a stepsize, an initial angle and final angle. The servo will sweep through all possibilites of positions separated  by the given step size. 
    It stores the maximum attained so far, replacing it whenever it encounters a higher power value. A two value array stores the indices of the servo positions of the highest 
    attained maximum.
    Suppose the stepsize is 90. Then there are 180 / 90 + 1 = 3 positions: (0, 90, 180). There are 9 possible values for the combined servo system...
    The algorithm takes forever. If we are already nearby the right value, the initial and final. values should straddle 90, the initial angle for all servos. For example, I could pick 
    as initial and final values 60 and 120, with a step size of 10. Then I'd loosen the motors so that they can assume a 90 degree angle without turning the mirrors. Then it explores all the 
    nearby configurations. 
*/
/* note: RED wire goes to 5V, BROWN wire goes to GND, ORANGE wire goes tot he digital output pin */

#include <Servo.h>
#include <math.h>

#define APIN 9  //digital output pin for motor 1
#define AREADPIN 14  // Analog read pin 
#define BPIN 10 //digital output pin for motor 2
#define CPIN 11
#define DPIN 12
#define POWERPIN 11
#define WAIT 1000

Servo serv1;
Servo serv2;
Servo serv3;
Servo serv4;

int intensity = 0; // adc reading of light signal
int MotorCount = 0; // controls the amount of time spent on a single motor
int newintensity = 0;
float fintensity = 0; // light intensity converted to voltage as floating point
float fnewintensity = 0;
int stepsize = 20; //resolution of servo rotation (it is in degrees)
int Ainitial = 90;   // initial servo position
int Alowerbound = 0;
int Aupperbound = 180;
int Binitial = 90;
int Blowerbound = 0;
int Bupperbound = 180;
int Cinitial = 90;
int Clowerbound = 0;
int Cupperbound = 180;
int Dinitial = 0;
int Dlowerbound = 0;
int Dupperbound = 180;
int Asign = 1;
int Bsign = 1;
int Csign = 1;
int Dsign = 1;
int Aservostate = Ainitial;
int Bservostate = Binitial;
int Cservostate = Cinitial;
int Dservostate = Dinitial;
int dgain = 100; // sets the gain on the Adifference feedback (I want to make this a tunable parameter)
int i, j, k, l = 0;
int powermax = 0;
float fpowermax;
int maxpowpos[4]; //array with the indices of the maxpositions i.e. if maxpowpos = [3, 5], then the maxpower servo configuration corresponds to serv1 at 3*stepsize and serv2 at 5*stepsize
int currpower;
float fcurrpower;
int Apos;
int Bpos;
int Cpos;
int Dpos;
int Afinalpos;
int Bfinalpos;
int Cfinalpos;
int Dfinalpos;
float fstandardpower = 0;


void setup() {
  serv1.attach(APIN);
  serv2.attach(BPIN);
  serv3.attach(CPIN);
  serv4.attach(DPIN);
  serv1.write(Ainitial);
  delay(200);
  serv2.write(Binitial);
  delay(200);
  serv3.write(Cinitial);
  delay(200);
  serv4.write(Dinitial);
  delay(200);
  Serial.begin(9600); // set up Serial library at 9600 bps
  delay(10000); // buy some time to get the motor locked in;
}

int loopnum = 0; //which loop am I on?
void loop() {
  Serial.print("Start Loop: ");
  Serial.print(loopnum);
  Serial.print("\n");
  for (Apos = Alowerbound; Apos < Aupperbound; Apos += stepsize) {
    j = 0;
    k = 0;
    l = 0;
    i += 1;
    serv1.write(Apos);
    delay(100);
    for (Bpos = Blowerbound; Bpos < Bupperbound; Bpos += stepsize) {
      k = 0;
      l = 0;
      j += 1;
      serv2.write(Bpos);
      delay(100);
      for (Cpos = Clowerbound; Cpos < Cupperbound; Cpos += stepsize) {
        l = 0;
        k += 1;
        serv3.write(Cpos);
        delay(100);
        for (Dpos = Dlowerbound; Dpos < Dupperbound; Dpos += stepsize) {
          l += 1;
          serv4.write(Dpos);
          delay(100);
          currpower = analogRead(AREADPIN);
          if (currpower > powermax) {
            powermax = currpower;
            maxpowpos[0] = i;
            maxpowpos[1] = j;
            maxpowpos[2] = k;
            maxpowpos[3] = l;
          }
          fcurrpower = 5.0 * currpower / 1024;
          fpowermax = 5.0 * powermax / 1024;
          Serial.print(fcurrpower);
          Serial.print(",");
          Serial.print(fpowermax);
          Serial.print(",");
          Serial.print(i);
          Serial.print(",");
          Serial.print(j);
          Serial.print(",");
          Serial.print(k);
          Serial.print(",");
          Serial.print(l);
          Serial.print("\n");
        }
      }
    }
  }
  delay(3000);
  serv1.write(Alowerbound);
  serv1.write(Blowerbound);
  serv1.write(Clowerbound);
  serv1.write(Dlowerbound);
  Afinalpos = Alowerbound + stepsize * (maxpowpos[0]-1);
  Bfinalpos = Blowerbound + stepsize * (maxpowpos[1]-1);
  Cfinalpos = Clowerbound + stepsize * (maxpowpos[2]-1);
  Dfinalpos = Dlowerbound + stepsize * (maxpowpos[3]-1);
  Serial.println("Final indices are");
  Serial.print(maxpowpos[0]);
  Serial.print("\t");
  Serial.print(maxpowpos[1]);
  Serial.print("\t");
  Serial.print(maxpowpos[2]);
  Serial.print("\t");
  Serial.print(maxpowpos[3]);
  Serial.print("\n");
  Serial.println("A final position: ");
  Serial.println(Afinalpos);
  Serial.println("B final position: ");
  Serial.println(Bfinalpos);
  Serial.println("C final position: ");
  Serial.println(Cfinalpos);
  Serial.println("D final position: ");
  Serial.println(Dfinalpos);
  delay(3000);
  serv1.write(Afinalpos);
  delay(500);
  serv2.write(Bfinalpos);
  delay(500);
  serv3.write(Cfinalpos);
  delay(500);
  serv4.write(Dfinalpos);
  delay(1000);
  currpower = analogRead(AREADPIN);
  fcurrpower = 5.0 * currpower / 1024;
  Serial.print("Max power attained was: ");
  Serial.print(fpowermax);
  Serial.print("\n");
  Serial.print("Final power is: ");
  Serial.print(fcurrpower);
  Serial.print("\n");
  delay(5000);
  if ((loopnum == 1) and (fcurrpower > fstandardpower)) {
    loopnum += 1;
    stepsize = stepsize / 2;
    Ainitial = Afinalpos;
    Alowerbound = (Ainitial == 0) ? 0 : (Ainitial - 20);
    Aupperbound = (Ainitial == 180) ? 180 : (Ainitial + 20);
    Binitial = Bfinalpos;
    Blowerbound = (Binitial == 0) ? 0 : (Binitial - 20);
    Bupperbound = (Binitial == 180) ? 180 : (Binitial + 20);
    Cinitial = Cfinalpos;
    Clowerbound = (Cinitial == 0) ? 0 : (Cinitial - 20);
    Bupperbound = (Cinitial == 180) ? 180 : (Cinitial + 20);
    Dinitial = Dfinalpos;
    Dlowerbound = (Dinitial == 0) ? 0 : (Dinitial - 20);
    Dupperbound = (Dinitial == 180) ? 180 : (Dinitial + 20);
  }
  else {
    delay(5000);
    exit(0);
  }
}


