#include <Servo.h>
#include <math.h>

/* Purpose of this script: automatically walk a laser beam using four servo motors attached to the four degrees of freedom, using as input the analog output of a power meter
OVERVIEW: EACH MOTOR CORRESPONDS TO A DEGREE OF FREEDOM OF THE SYSTEM: I.E. LENS ORIENTATION IN EACH OF TWO DIRECTIONS AND BEAM ORIENTATION IN EACH OF TWO DIRECTIONS. 
EACH DEGREE OF FREEDOM IS DETERMINED BY THE ANGLE OF THE KNOB THAT CONTROLS IT. THIS KNOB IS IN DIRECT CORRESPONDENCE WITH THE SERVO MOTOR'S ANGLE. 
PROGRAM STRUCTURE: FIX THREE DEGREES OF FREEDOM. SCAN THROUGH VALUES OF THE VARIABLE ONE. STORE VALUES IN AN ARRAY. AFTER SCAN STAGE, SET THE VALUE TO THE ABSOLUTE MAX. DO SMALL 
ADJUSTMENTS AROUND THAT SETTING. 
OPEN UP ANOTHER DEGREE OF FREEDOM, IMPLEMENT THE SAME ALGORITHM, THEN REPEAT IT ONCE MORE FOR THE ORIGINAL FREE ONE. REPEAT.
*/


/* 1pos, 2pos, etc. are the respective pins of the four servo motors */
#define 1pin 5 
#define 2pin 6
#define 3pin 9
#define 4pin 10
#define POWERINPUT /*the analog power meter input */
#define RES 5 /* angular resolution of the servo */
#define MAXANGLE 180
/* the four servos */
Servo serv1;  
Servo serv2;  
Servo serv3;  
Servo serv4;  


void setup() {
  serv1.attach(1pin);
  serv2.attach(2pin);
  serv3.attach(3pin);
  serv4.attach(4pin);  
  serv1.write(90);
  serv2.write(90);
  serv3.write(90);
  serv4.write(90);
}

void loop() {
  float vals1[MAXANGLE / RES + 1];
  float vals2[MAXANGLE / RES + 1];
  float vals3[MAXANGLE / RES + 1];
  float vals4[MAXANGLE / RES + 1];
  servmax(vals1, serv1);
  servmax(vals2, serv2);
  servmax(vals3, serv3);
  servmax(vals4, serv4);
  }
}
/* Overall structure:
 Increment a servo by an angle RES until it reaches the MAXANGLE
 At each incrementation, measure the power meter value and store it in an array called servvals.
 At the end of this step, find the index correpsonding to the max power value. Then go back to that position of the servo.  
 */



/* servmax takes in as  arguments the motor it will adjust and the array of values corresponding to the previous positions (it will overwrite these) */
float servmax(float vals[], Servo currservo) {
  int i = 0;
  int j = 0;
  float pos;
  float locmax = 0;
  int locmaxindex = 0;
  float maxpos = 0;
  for (pos = 0; pos < MAXANGLE; ++pos) {
    pos += RES; /*increment the angle of the motor by the resolution set above */
    vals[i++] = analogRead(POWERINPUT); /*measure the power corresponding to that position and store it in vals */
    currservo.write(pos);
    delay(50)
  } 
  while (j < i) {     /* less than because I increase i once more at the end of the for loop */
    if vals[j] > locmax { 
      locmax = vals[j]
      locmaxindex = j++;
    }
    else {
      ++j;
    }
  }
  maxpos = locmaxindex*RES;
  currservo.write(maxpos); 
  return locmax;
}

