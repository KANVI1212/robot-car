#include <SPI.h>
#include <RFID.h>
#define SS_PIN 10
#define RST_PIN A6
RFID rfid(SS_PIN, RST_PIN);
String rfidCard;
const int RM1 = A0;
const int RM2 = A2;
const int LM1 =A4 ;
const int LM2 =A5;
const int trigPin =4;
const int echoPin =2;
#define LS 2 // left sensor
#define RS 3 // right sensor
int a;
int b;
int command; //Int to store app command state.
int Speed = 250; // 0 - 255.
int Speedsec;
int buttonState = 0;
int lastButtonState = 0;
int Turnradius = 0; //Set the radius of a turn, 0 - 255 Note:the robot will malfunction if this is higher than int Speed.
int brakeTime = 45;
int brkonoff = 1; 
void setup() {
  Serial.begin(9600);
  Serial.println("Starting the RFID Reader...");
  SPI.begin();
  rfid.init();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode (RM1, OUTPUT);
  pinMode (RM2, OUTPUT);
  pinMode (LM1, OUTPUT);
  pinMode (LM2, OUTPUT);
  pinMode(LS, INPUT);
  pinMode(RS, INPUT);
}
long duration, distance;
void loop() {

  if (rfid.isCard()) {
    if (rfid.readCardSerial()) {
      rfidCard = String(rfid.serNum[0]) + " " + String(rfid.serNum[1]) + " " + String(rfid.serNum[2]) + " " + String(rfid.serNum[3]);
      Serial.println(rfidCard);}}
      if (rfidCard == "102 199 96 36"){
        obst();
      }
      else if (rfidCard == "118 214 239 36"){
        LF();
           }
      else if (rfidCard == "229 42 187 48"){
        blu();}}
           //obstacle avoiding 
void obst()
{
  Speed=10;
   digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);  
  duration = pulseIn(echoPin, HIGH);
  distance = duration/58.2;
    Serial.println(distance);
    Speed=10;
 if (distance > 19)            
  {
    digitalWrite(RM1, HIGH);                    // move forward
    digitalWrite(RM2, LOW);
    digitalWrite(LM1, HIGH);                                
    digitalWrite(LM2, LOW);                                                       
  }

  if (distance < 18)
  {
    digitalWrite(RM1, LOW);  //Stop                
    digitalWrite(RM2, LOW);
    digitalWrite(LM1, LOW);                                
    digitalWrite(LM2, LOW);
    delay(500);
    digitalWrite(RM1, LOW);      //movebackword         
    digitalWrite(RM2, HIGH);
    digitalWrite(LM1, LOW);                                
    digitalWrite(LM2, HIGH);
    delay(500);
    digitalWrite(RM1, LOW);  //Stop                
    digitalWrite(RM2, LOW);
    digitalWrite(LM1, LOW);                                
    digitalWrite(LM2, LOW);  
    delay(100);  
    digitalWrite(RM1, HIGH);       
    digitalWrite(RM2, LOW);   
    digitalWrite(LM1, LOW);                                 
    digitalWrite(LM2, LOW);  
    delay(500);
  }

}
//line following 
void LF(){
  Serial.print("a data");
 Serial.println(a);
  Serial.print("b data");
 Serial.println(b);
  
  a=digitalRead(LS);
  b=digitalRead(RS);
  delay(500);
if((a) && (b)) // Move Forward on line
{
digitalWrite(LM1, HIGH);
digitalWrite(LM2, LOW);
digitalWrite(RM1, HIGH);
digitalWrite(RM2, LOW);
Serial.println("fORWARD");
delay(500);
}
if((a) && (!b)) // turn left by rotationg left motors in forward and right ones in backward direction
{
digitalWrite(LM1, HIGH);
digitalWrite(LM2, LOW);
Serial.println("LEFT");
digitalWrite(RM1, LOW);
digitalWrite(RM2, HIGH);
}
if((!a) && (b)) // Turn right by rotating right motors in forward and left ones in backward direction
{
digitalWrite(LM1, LOW);
digitalWrite(LM2, HIGH);
digitalWrite(RM1, HIGH);
digitalWrite(RM2, LOW);
Serial.println("RIGHT");
}

if((!a) && (!b)) // Finish line, stop both the motors
{
digitalWrite(LM1, LOW);
digitalWrite(LM2, LOW);
digitalWrite(RM1, LOW);
digitalWrite(RM2, LOW);
Serial.println("STOP");
}
}
//bluetooth car
void blu()
  {
  if (Serial.available() > 0) {
    command = Serial.read();
    Stop(); //Initialize with motors stoped.
   switch (command) {
      case 'F':
        forward();
        break;
      case 'B':
        back();
        break;
      case 'L':
        left();
        break;
      case 'R':
        right();
        break;
      case 'G':
        forwardleft();
        break;
      case 'I':
        forwardright();
        break;
      case 'H':
        backleft();
        break;
      case 'J':
        backright();
        break;
      case '0':
        Speed = 100;
        break;
      case '1':
        Speed = 140;
        break;
      case '2':
        Speed = 153;
        break;
      case '3':
        Speed = 165;
        break;
      case '4':
        Speed = 178;
        break;
      case '5':
        Speed = 191;
        break;
      case '6':
        Speed = 204;
        break;
      case '7':
        Speed = 216;
        break;
      case '8':
        Speed = 229;
        break;
      case '9':
        Speed = 242;
        break;
      case 'q':
        Speed = 255;
        break;}
    Speedsec = Turnradius;
    if (brkonoff == 1) {
      brakeOn();
    } else {
      brakeOff();
    }
  }
}

void forward() {
  analogWrite(RM1, Speed);
  analogWrite(LM1, Speed);
}

void back() {
  analogWrite(RM2, Speed);
  analogWrite(LM2, Speed);
}

void left() {
  analogWrite(LM1, Speed);
  analogWrite(RM2, Speed);
}

void right() {
  analogWrite(LM2, Speed);
  analogWrite(RM1, Speed);
}
void forwardleft() {
  analogWrite(RM1, Speedsec);
  analogWrite(LM1, Speed);
}
void forwardright() {
  analogWrite(RM1, Speed);
  analogWrite(LM1, Speedsec);
}
void backright() {
  analogWrite(RM2, Speed);
  analogWrite(LM2, Speedsec);
}
void backleft() {
  analogWrite(RM2, Speedsec);
  analogWrite(LM2, Speed);
}

void Stop() {
  analogWrite(RM1, 0);
  analogWrite(RM2, 0);
  analogWrite(LM1, 0);
  analogWrite(LM2, 0);
}

void brakeOn() {
  //Here's the future use: an electronic braking system!
  // read the pushbutton input pin:
  buttonState = command;
  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == 'S') {
      if (lastButtonState != buttonState) {
        digitalWrite(RM1, HIGH);
        digitalWrite(RM2, HIGH);
        digitalWrite(LM1, HIGH);
        digitalWrite(LM2, HIGH);
        delay(brakeTime);
        Stop();
      }
    }
    // save the current state as the last state,
    //for next time through the loop
    lastButtonState = buttonState;
  }
}
void brakeOff() {

}
