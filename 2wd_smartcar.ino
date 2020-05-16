#include <Servo.h>

/*************************************************
 * Public Constants
 *************************************************/

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

//Mario main theme melody
int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,
};
//Mario main them tempo
int tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};

//int pinLB=6;
//int pinLF=9;
//int pinRB=10;
//int pinRF=11;

// MOTOR PINS
int pinLB=2;
int pinLF=4;
int Lpwm_pin=5;
int pinRB=7;
int pinRF=8;
int Rpwm_pin=10;

// SENSOR PINS
int frontInputPin=A0;
int frontOutputPin=A1;
int backInputPin=A2;
int backOutputPin=A3;

// buzzer
int pinBuzzer = 3;

int Fspeedd=0;
int Rspeedd=0;
int Lspeedd=0;
int directionn=0;

Servo myservo;
int pinServo=9;

int delay_time=250;
int Fgo=8;
int Rgo=6;
int Lgo=4;
int Bgo=2;


int maxDistance = 400; // inches

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  myservo.attach(pinServo);
  pinMode(pinLB, OUTPUT);
  pinMode(pinLF, OUTPUT);
  pinMode(pinRB, OUTPUT);
  pinMode(pinRF, OUTPUT);
  pinMode(Lpwm_pin, OUTPUT);
  pinMode(Rpwm_pin, OUTPUT);

  pinMode(pinBuzzer, OUTPUT);

  int size = sizeof(melody) / sizeof(int);
  for (int thisNote = 0; thisNote < size; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / tempo[thisNote];

    buzz(pinBuzzer, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    // stop the tone playing:
    buzz(pinBuzzer, 0, noteDuration);

  }
}


void buzz(int targetPin, long frequency, long length) {
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
}

void BREAK(int direction = 0) {
  if (direction == 0) {
    digitalWrite(pinLB, HIGH); // Turn HIGH motor A
    digitalWrite(pinLF, LOW);
  }
  if (direction == 1) {
    digitalWrite(pinLB, LOW); // Turn HIGH motor A
    digitalWrite(pinLF, HIGH);
  }
  analogWrite(Lpwm_pin, 255); // TO set the turning speed to 200 out of possible range 0 to 255
  delay(200);

  digitalWrite(pinLB, LOW); // Turn HIGH motor A
  digitalWrite(pinLF, LOW);
}

void buzzerDistance(double distance, int buzzerSound = 1000) {
  if (distance > maxDistance) {
    distance = maxDistance;
  }

//  noTone(pinBuzzer);
  tone(pinBuzzer, buzzerSound, 100 * (double) (200 / distance));
  delay(1000 * (double) (distance / maxDistance));
  tone(pinBuzzer, 0, 100 * (double) (200 / distance));
}

int decideDriveCounter = 0;
void loop() {
  decideDriveCounter++;
  Serial.print("\r\n");
  Serial.print(decideDriveCounter);
  Serial.print("\r\n");
  if (decideDriveCounter >= 0 && decideDriveCounter <= 10) {
    if (DECIDEDRIVE()) {
      return;
    } else {
      while (DRIVEBACKWARD()) {
        decideDriveCounter = 0;
        Serial.print("driving backwards!\n");
      }
    }
  }
  
  while (DRIVEFORWARD()) {
    decideDriveCounter = 0;
    Serial.print("driving forward!\n");
  }

  while (DRIVEBACKWARD()) {
    decideDriveCounter = 0;
    Serial.print("driving backwards!\n");
  }
}

int * scanLeftToRight() {
  myservo.write(0);
  int deg0 = getFrontDistance(0);
  delay(100);

  myservo.write(45);
  int deg45 = getFrontDistance(0);
  delay(100);

  myservo.write(90);
  int deg90 = getFrontDistance(0);
  delay(100);

  myservo.write(135);
  int deg135 = getFrontDistance(0);
  delay(100);

  myservo.write(180);
  int deg180 = getFrontDistance(0);
  delay(100);

  static int r[4];
  r[0] = deg0;
  r[1] = deg45;
  r[2] = deg90;
  r[3] = deg135;
  r[4] = deg180;

  return r;
}

void LOOKLEFT() {
  myservo.write(0);
  delay(1000);
}

void LOOKCENTER() {
  myservo.write(90);
  delay(1000);
}

void LOOKRIGHT() {
  myservo.write(180);
  delay(1000);
}

void STOP() {
  digitalWrite(pinLB, LOW); // Turn the motor off
  digitalWrite(pinLF, LOW); // Turn the motor off  
  digitalWrite(pinRB, LOW); // Turn the motor off
  digitalWrite(pinRF, LOW); // Turn the motor off
  analogWrite(Lpwm_pin, 0); // TO set the turning speed to 200 out of possible range 0 to 255
  analogWrite(Lpwm_pin, 0);
  delay(1000);
}

bool DECIDEDRIVE() {
  int * distances = scanLeftToRight();
  int distance = distances[2]; // front distance

  digitalWrite(pinRB, LOW); // turn HIGH motor B
  digitalWrite(pinRF, LOW);  // To set the turning speed to 200 out of possible range 0 to 255 
  analogWrite(Rpwm_pin, 0);

  int maxDistanceDecide = 60;

  Serial.print(distances[2]);
  if (distances[2] > maxDistanceDecide || distances[2] <= 5) {
    return false;
  }
  
  // Run the motors on both direction at fixed speed
  digitalWrite(pinLB, LOW); // Turn HIGH motor A
  digitalWrite(pinLF, HIGH);
  analogWrite(Lpwm_pin, 0); // TO set the turning speed to 200 out of possible range 0 to 255

  if (distances[1] > distances[3]) {
    digitalWrite(pinRB, HIGH); // turn HIGH motor B
    digitalWrite(pinRF, LOW);  // To set the turning speed to 200 out of possible range 0 to 255 
    analogWrite(Rpwm_pin, 255);
    delay(100);  // Delay
  }

  if (distances[3] > distances[1]) {
    digitalWrite(pinRB, LOW); // turn HIGH motor B
    digitalWrite(pinRF, HIGH);  // To set the turning speed to 200 out of possible range 0 to 255 
    analogWrite(Rpwm_pin, 255);
    delay(100);  // Delay
  }

  digitalWrite(pinLB, LOW);
  digitalWrite(pinLF, HIGH);
  analogWrite(Lpwm_pin, 255); // TO set the turning speed to 200 out of possible range 0 to 255
  delay(100);

  double accelerate = 255 * ((double) distances[2] / 60);
  
  analogWrite(Lpwm_pin, accelerate); // TO set the turning speed to 200 out of possible range 0 to 255

  return true;
}

bool DRIVEFORWARD() {
  myservo.write(90); // make sure to look at the front!
  
  double accelerate = 0;
  int distance = getFrontDistance(0);

  Serial.print(distance);
  Serial.print("\r\n");
  if (distance <= 20) {
    distance = 0;
    analogWrite(Lpwm_pin, accelerate); // TO set the turning speed to 200 out of possible range 0 to 255
    return false;
  }

  digitalWrite(pinLB, LOW);
  digitalWrite(pinLF, HIGH);
  analogWrite(Lpwm_pin, 255); // TO set the turning speed to 200 out of possible range 0 to 255
  delay(100);

  accelerate = 255 * ((double) distance / 50);
  
  analogWrite(Lpwm_pin, accelerate); // TO set the turning speed to 200 out of possible range 0 to 255

  if (DECIDEDRIVE()) {
    return true;
  }

  return accelerate > 0;
}

bool DRIVEBACKWARD() {
  double accelerate = 0;
  int distance = getBackDistance(0);

  if (distance <= 20) {
    distance = 0;
    return false;
  }

  digitalWrite(pinLB, HIGH);
  digitalWrite(pinLF, LOW);
  analogWrite(Lpwm_pin, 255); // TO set the turning speed to 200 out of possible range 0 to 255
  delay(100);

  accelerate = 255 * ((double) distance / 50);
  
  analogWrite(Lpwm_pin, accelerate); // TO set the turning speed to 200 out of possible range 0 to 255

  return accelerate > 0;
}

void BACKWARDLEFT() {
  // Run the motors on both direction at fixed speed
  digitalWrite(pinLB, LOW); // Turn HIGH motor A
  digitalWrite(pinLF, HIGH);
  analogWrite(Lpwm_pin, 0); // TO set the turning speed to 200 out of possible range 0 to 255
 
  digitalWrite(pinRB, LOW); // turn HIGH motor B
  digitalWrite(pinRF, HIGH);  // To set the turning speed to 200 out of possible range 0 to 255 
  analogWrite(Rpwm_pin, 255);
  delay(500);  // Delay

  // Run the motors on both direction at fixed speed
  digitalWrite(pinLB, HIGH); // Turn HIGH motor A
  digitalWrite(pinLF, LOW);
  analogWrite(Lpwm_pin, 200); // TO set the turning speed to 200 out of possible range 0 to 255
  delay(2000);
}

void BACKWARDRIGHT() {
    // Run the motors on both direction at fixed speed
  digitalWrite(pinLB, LOW); // Turn HIGH motor A
  digitalWrite(pinLF, HIGH);
  analogWrite(Lpwm_pin, 0); // TO set the turning speed to 200 out of possible range 0 to 255
 
  digitalWrite(pinRB, HIGH); // turn HIGH motor B
  digitalWrite(pinRF, LOW);  // To set the turning speed to 200 out of possible range 0 to 255 
  analogWrite(Rpwm_pin, 255);
  delay(500);  // Delay

  // Run the motors on both direction at fixed speed
  digitalWrite(pinLB, HIGH); // Turn HIGH motor A
  digitalWrite(pinLF, LOW);
  analogWrite(Lpwm_pin, 200); // TO set the turning speed to 200 out of possible range 0 to 255
  delay(2000);
}

void TURNLEFT() {
  // Run the motors on both direction at fixed speed
  digitalWrite(pinLB, LOW); // Turn HIGH motor A
  digitalWrite(pinLF, HIGH);
  analogWrite(Lpwm_pin, 0); // TO set the turning speed to 200 out of possible range 0 to 255
 
  digitalWrite(pinRB, HIGH); // turn HIGH motor B
  digitalWrite(pinRF, LOW);  // To set the turning speed to 200 out of possible range 0 to 255 
  analogWrite(Rpwm_pin, 255);
  delay(500);  // Delay

  // Run the motors on both direction at fixed speed
  digitalWrite(pinLB, LOW); // Turn HIGH motor A
  digitalWrite(pinLF, HIGH);
  analogWrite(Lpwm_pin, 255); // TO set the turning speed to 200 out of possible range 0 to 255
  delay(2000);
}

void TURNRIGHT() {
  // Run the motors on both direction at fixed speed
  digitalWrite(pinLB, LOW); // Turn HIGH motor A
  digitalWrite(pinLF, HIGH);
  analogWrite(Lpwm_pin, 0); // TO set the turning speed to 200 out of possible range 0 to 255
 
  digitalWrite(pinRB, LOW); // turn HIGH motor B
  digitalWrite(pinRF, HIGH);  // To set the turning speed to 200 out of possible range 0 to 255 
  analogWrite(Rpwm_pin, 255);
  delay(500);  // Delay

  // Run the motors on both direction at fixed speed
  digitalWrite(pinLB, LOW); // Turn HIGH motor A
  digitalWrite(pinLF, HIGH);
  analogWrite(Lpwm_pin, 255); // TO set the turning speed to 200 out of possible range 0 to 255
  delay(2000);
}

void DRIVECIRCLE()
{
  // Run the motors on both direction at fixed speed
  digitalWrite(pinLB, HIGH); // Turn HIGH motor A
  digitalWrite(pinLF, LOW);
  analogWrite(Lpwm_pin, 10); // TO set the turning speed to 200 out of possible range 0 to 255
 
  digitalWrite(pinRB, HIGH); // turn HIGH motor B
  digitalWrite(pinRF, LOW);  // To set the turning speed to 200 out of possible range 0 to 255 
  analogWrite(Rpwm_pin, 500);
  delay(2000);  // Delay to 2 seconds
  
  // Changing the direction of the motor
  
  digitalWrite(pinLB, LOW); 
  digitalWrite(pinLF, HIGH);  
  digitalWrite(pinRB, LOW);
  digitalWrite(pinRF, HIGH); 
  delay(2000); // Delay to 2 seconds
 
  digitalWrite(pinLB, LOW); // Turn the motor off
  digitalWrite(pinLF, LOW); // Turn the motor off  
  digitalWrite(pinRB, LOW); // Turn the motor off
  digitalWrite(pinRF, LOW); // Turn the motor off
}

void DRIVEONE()
{
  // Run the motors on both direction at fixed speed
  digitalWrite(pinLB, HIGH); // Turn HIGH motor A
  digitalWrite(pinLF, LOW);
  analogWrite(Lpwm_pin, 200); // TO set the turning speed to 200 out of possible range 0 to 255
 
  digitalWrite(pinRB, HIGH); // turn HIGH motor B
  digitalWrite(pinRF, LOW);  // To set the turning speed to 200 out of possible range 0 to 255 
  analogWrite(Rpwm_pin, 200);
  delay(2000);  // Delay to 2 seconds
  
  // Changing the direction of the motor
  
  digitalWrite(pinLB, LOW); 
  digitalWrite(pinLF, HIGH);  
  digitalWrite(pinRB, LOW);
  digitalWrite(pinRF, HIGH); 
  delay(2000); // Delay to 2 seconds
 
  digitalWrite(pinLB, LOW); // Turn the motor off
  digitalWrite(pinLF, LOW); // Turn the motor off  
  digitalWrite(pinRB, LOW); // Turn the motor off
  digitalWrite(pinRF, LOW); // Turn the motor off
}
void DRIVETWO()
{
/*
  These function will turn the motors on the possible speeds, the maximum speed turns is determined
  by the motor specs and the operating voltage. The PWM(Pulse with modulation values will sent
  by analogWrite() function to drive the maxim speed. 
*/
  
  digitalWrite(pinLB, LOW);
  digitalWrite(pinLF, HIGH);  
  digitalWrite(pinRB, LOW);
  digitalWrite(pinRF, HIGH); 

  for (int x = 0; x < 256; x++)   // Motor will accelerate from 0 to maximum speed
  {
    analogWrite(Lpwm_pin, x);
    analogWrite(Rpwm_pin, x);
    delay(20);
  } 
 
  for (int y = 255; y >= 0; --y)  // Motor will decelerate from maximum speed to 0
  {
    analogWrite(Lpwm_pin, y);
    analogWrite(Rpwm_pin, y);
    delay(20);
  } 
 
  digitalWrite(pinLB, LOW); //  Tun Off All the Motors 
  digitalWrite(pinLF, LOW);  
  digitalWrite(pinRB, LOW);
  digitalWrite(pinRF, LOW);  
}

long getFrontDistance(int type) {
  //  type can be 0 for inches and 1 for centimeter
  long duration, inches, cm;
  pinMode(frontInputPin, OUTPUT);
  digitalWrite(frontInputPin, LOW);
  delayMicroseconds(2);
  digitalWrite(frontInputPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(frontInputPin, LOW);
  pinMode(frontOutputPin, INPUT);
  duration = pulseIn(frontOutputPin, HIGH);
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  buzzerDistance(inches);

  if (type == 0) {
    return inches;
  }

  return cm;
}

long getBackDistance(int type) {
  //  type can be 0 for inches and 1 for centimeter
  long duration, inches, cm;
  pinMode(backInputPin, OUTPUT);
  digitalWrite(backInputPin, LOW);
  delayMicroseconds(2);
  digitalWrite(backInputPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(backInputPin, LOW);
  pinMode(backOutputPin, INPUT);
  duration = pulseIn(backOutputPin, HIGH);
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
//  Serial.print(inches);
//  Serial.print("in, ");
//  Serial.print(cm);
//  Serial.print("cm");
//  Serial.println();
  buzzerDistance(inches, 1500);

  if (type == 0) {
    return inches;
  }

  return cm;
}

long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
