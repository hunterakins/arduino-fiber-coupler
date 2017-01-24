   /* The walk algorithm is as follows. Pick a pair of mirror degrees of freedom, A and B.
   *  STEP 0: If Acounter = 2, exit the loop.
   *  STEP 1: Measure the Voltage reading intensity, convert to fintensity.
   *  STEP 2; Turn A by Asign * stepsize
   *  STEP 3; Take a voltage reading, called Intensity. Set Bcounter to 0.
   *  STEP 4: Then turn B by Bsign * stepsize.
   *  STEP 5: Take a voltage reading, call it ProbeIntensity
   *  STEP 6: If output power is increased, i.e. ProbeIntensity > Intensity, turn B by Bsign* stepsize again.  
   *          Repeat previous step until output power is not increased by a stepsize. Then undo that step and return to STEP 2.
   *          If output power is decreased: 
   *             If Bcounter = 1 undo that step and change Asign to -1*Asign. Increment Acounter by 1. 
   *             If Bcounter = 0, undo that step and change Bsign to -1*BSIGN and repeat from STEP 4. Change Bcounter from 0 to 1.
   *  
   *  
   */

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

  int stepsize = 10;
  int intensity; // adc reading of light signal
  int MotorCount = 0; // controls the amount of time spent on a single motor
  int newintensity;
  float fintensity; // light intensity converted to voltage as floating point
  float fnewintensity;
  int statusquo;
  float fstatusquo;
  float currdifference;
  int Ainitial = 90;   // initial servo position
  int Binitial = 40;
  int Cinitial = 110;
  int Dinitial = 60;
  int Alowerbound = 80;
  int Blowerbound = 30;
  int Clowerbound = 100;
  int Dlowerbound = 50;
  int Aupperbound = 100;
  int Bupperbound = 50;
  int Cupperbound = 120;
  int Dupperbound = 70;
  int Asign = 1;
  int Bsign = 1;
  int Csign = 1;
  int Dsign = 1;
  int Aservostate = Ainitial;
  int Bservostate = Binitial;
  int Cservostate = Cinitial;
  int Dservostate = Dinitial;
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
  int Acounter = 0;
  int Bcounter = 0;
  int Ccounter = 0;
  int Dcounter = 0;
  int Aindicator = 0;
  int Bindicator = 0;
  int Cindicator = 0;
  int Dindicator = 0;
  int BcruisinIndicator = 0; //increments when B starts cruisin (step creates a positive difference)
  int DcruisinIndicator = 0;
  int DOG = Dinitial;
  int BOG = Binitial; // o.g. B position at the beginning of the algorithm

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
    delay(5000); // buy some time to get the motor locked in;
  }

  void loop() {
     delay(1000);
     statusquo = analogRead(AREADPIN);
     fstatusquo = 5.0 * statusquo / 1024;
     intensity = statusquo;
     fintensity = fstatusquo;
     Serial.print("fstatusquo: ");
     Acounter = 0; //increments if a positive change in A leads to two consecutive fails (both ways fail for B); that way if both turns in A lead to a dead end, I'm at a local max
     Bcounter = 0; //increments if a move in a  direction fails 
     BcruisinIndicator = 0; //increments if a turn in B leads to a positive difference to indicate that B is cruisin
     Serial.print(fstatusquo);
     Serial.print("\n");

     
    while (Acounter < 2) {    //until both ways result in failure
      Serial.print("Probing with A\n");
      Serial.print("Acounter = ");
      Serial.print(Acounter);
      Serial.print("\n");
      Aservostate += Asign * stepsize;
      BcruisinIndicator = 0;   
      Bcounter = 0;
      serv1.write(Aservostate); //let's take a little probe step 
      delay(100);
      intensity = analogRead(AREADPIN);
      fintensity = 5.0 * intensity / 1024;
      Serial.print("Intensity = ");
      Serial.print(fintensity);
      Serial.print("\n");
      delay(5000);                                 
      
      while (Bcounter < 2) {    // until I fail going both ways
        Serial.println("Starting the B loop");
        Serial.print("Bcounter = ");
        Serial.print(Bcounter);
        Serial.print("\n");
        Bservostate = Bsign * stepsize; 
        delay(1000);
        Serial.println("Taking a probe with B");
        serv2.write(Bservostate); //let's take a little probe step
        delay(5000);
        if (BcruisinIndicator == 1) {   //my last move created a positive difference(in the world)
          Serial.println("Still cruisin on B");
          intensity = newintensity;
          fintensity = fnewintensity;
          newintensity = analogRead(AREADPIN); // read the new power
          fnewintensity = 5.0*fnewintensity/1024;                   
          Serial.print("New power level: ");
          Serial.print(fnewintensity);
          Serial.print("\t");
          currdifference = fnewintensity - fintensity;    
          Serial.print("Current difference: ");
          Serial.print(currdifference);
          Serial.print("\t");               
          delay(1000);
          if (currdifference < 0) {  // I can go no further this way
            Bservostate = -Bsign * stepsize; //shit, go back
            if (fintensity > fstatusquo) {  //the end of a successful run
              BcruisinIndicator = 0; //no longer cruisin :(
              Bcounter = 2; // need to get back to A...get me outta this B loop
            }
            if (fintensity < fstatusquo) { //this whole cruise was a fail
              serv2.write(BOG);  // ...go back to where we started
              Bcounter +=1;
              if (Bcounter == 2) {
                Serial.println("This direction of A failed");
                Acounter += 1; //I must have failed both B ways
                Asign = -1 * Asign; //better try the other A direction
                Aservostate = Asign * stepsize; //gotta go back first
                serv1.write(Aservostate);
              }
              Bsign = -1 * Bsign; //and try the other way 
            }
            else {
              Serial.print("Still B cruisin...\n");
            }
          }
        }
        else {
          intensity = statusquo; //I skipped the Bindicator, so I'm either on my first move or the other direction didn't work out...
          fintensity = fstatusquo;  //so my power level OUGHT to be fstatus quo
          newintensity = analogRead(APIN);
          fnewintensity = 5.0*newintensity / 1024;
          currdifference = fnewintensity - fintensity;
          delay(5000);
          Serial.print("New power level: ");
          Serial.print(fnewintensity);
          Serial.print("\t");
          Serial.print("Current difference: ");
          Serial.print(currdifference);
          Serial.print("\t");               
          if (currdifference > 0) {   
            BcruisinIndicator = 1; //let's cruiseeee baby
            Serial.println("Begin the cruise");
          }
          if (currdifference <= 0) {
            Serial.println("This way is no good");
            Bcounter += 1;         //this way is no good
            if (Bcounter == 2) {
              Acounter += 1;
            }
            serv2.write(BOG);  //back to the OG position
            Bsign = -1 * Bsign;   //try the other way?
          }
        }
      } 
    }
    // END OF THE A-B WALKING, SWITCH TO THE C WALKING, GET THE NEW STATUS QUO FOR THIS ROUND
    statusquo = analogRead(AREADPIN);
    fstatusquo = statusquo * 5 / 1024;
    delay(1000);
    while (Ccounter < 2) {    //until both ways result in failure
      Cservostate += stepsize * Csign;   
      serv3.write(Cservostate); //let's take a little probe step 
      delay(150);                                 
      while (Dcounter < 2) {    // until I fail going both ways
        Dservostate =  stepsize * Dsign ; 
        serv4.write(Dservostate); //let's take a little probe step
        if (DcruisinIndicator == 1) {   //my last move created a positive difference(in the world)
          Serial.println("Still cruisin on D");
          intensity = newintensity;
          fintensity = fnewintensity;
          newintensity = analogRead(AREADPIN); // read the new power
          fnewintensity = 5.0*fnewintensity/1024;                   
          Serial.print("New power level: ");
          Serial.print(fnewintensity);
          Serial.print("\t");
          currdifference = fnewintensity - fintensity;    
          Serial.print("Current difference: ");
          Serial.print(currdifference);
          Serial.print("\t");               
          delay(1000);
          if (currdifference < 0) {  // I can go no further this way
            Dservostate = - stepsize * Dsign; //shit, go back
            if (fintensity > fstatusquo) {  //the end of a successful run
              DcruisinIndicator = 0; //no longer cruisin :(
              Dcounter = 2; // need to get back to C...get me outta this D loop
            }
            if (fintensity < fstatusquo) { //this whole cruise was a fail
              serv4.write(DOG);  // ...go back to where we started
              Dcounter +=1;
              if (Dcounter == 2) {
                Ccounter += 1; //I must have failed both D ways
                Csign = -1 * Csign; //better try the other C direction
                Cservostate = stepsize * Csign ; //gotta go back first
                serv3.write(Cservostate);
              }
              Dsign = -1 * Dsign; //and try the other way 
            }
            else {
              Serial.print("Still D cruisin...\n");
            }
          }
        }
        else {
          intensity = statusquo; //I skipped the Dindicator, so I'm either on my first move or the other direction didn't work out...
          fintensity = fstatusquo;  //so my power level OUGHT to be fstatus quo
          newintensity = analogRead(CPIN);
          fnewintensity = 5.0*newintensity / 1024;
          currdifference = fnewintensity - fintensity;
          Serial.print("New power level: ");
          Serial.print(fnewintensity);
          Serial.print("\t");
          Serial.print("Current difference: ");
          Serial.print(currdifference);
          Serial.print("\t");               
          delay(1000);
          if (currdifference > 0) {   
            DcruisinIndicator = 1; //let's cruiseeee baby
            Serial.print("Cruising on D/n");
          }
          if (currdifference <= 0) {
            Dcounter += 1;         //this way is no good
            serv4.write(DOG);  //back to the OG position
            Dsign = -1 * Dsign;   //try the other way?
          }
        }
      } 
    }
    delay(1000);
    //END OF BOTH AB AND CD WALKING TRY AGAIN NOW
  }
