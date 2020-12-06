/*   
HC05 - Bluetooth AT-Command mode  
*/ 
#include <SoftwareSerial.h> 
SoftwareSerial hc05(13, 12); // RX | TX 
String hc05Value = "";
int angle = 0;
int strength = 0;
int button = 0;
int LED = 10; 


int motor1pin1 = 2;
int motor1pin2 = 3;
int motor1Enable = 5;

int motor2pin1 = 7;
int motor2pin2 = 4;
int motor2Enable = 6;

void setup() {
  setupBluetooth();
  setupMotors();
}

void loop() {
  loopBluetooth();
  loopMotors();
}


void setupBluetooth() 
{   
 Serial.begin(9600); 
 hc05.begin(9600); 
 pinMode(LED, OUTPUT); 
 Serial.println("Ready to connect\nDefualt password is 1234 or 000"); 
}  

void loopBluetooth() {
  
  if (hc05.available()>0) 
  {
//    flag = hc05.read();
//    Serial.println(flag); 
      String hc05Value = hc05.readStringUntil('#');
      if(hc05Value.length()==7)
      {
        angle = hc05Value.substring(0, 3).toInt();
        strength = hc05Value.substring(3, 6).toInt();
        button = hc05Value.substring(6, 8).toInt();
//        Serial.print("angle: ");Serial.print(angle);Serial.print('\t');
//        Serial.print("strength: ");Serial.print(strength);Serial.print('\t');
//        Serial.print("button: ");Serial.print(button);Serial.println("");
        Serial.flush();
        hc05Value="";
      }
  }
}

void setupMotors() {
  pinMode(motor1Enable, OUTPUT);
  pinMode(motor2Enable, OUTPUT);
  
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);

  // Turn off motors - Initial state
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);
}

// Run car based on these global parameters: angle, strength, button
void loopMotors() {

  if (strength > 0) {
    
    // straight left
    if (angle >= 0 and angle <= 90) {
      digitalWrite(motor1pin1, LOW);
      digitalWrite(motor1pin2, HIGH);
      analogWrite(motor1Enable, map(angle*strength, 9000, 0, 0, 255));
    
      digitalWrite(motor2pin1, LOW);
      digitalWrite(motor2pin2, HIGH);
      analogWrite(motor2Enable, map(strength, 0, 100, 0, 255));
    }
    
    // straight right
    if (angle >= 270 and angle <= 360) {
      digitalWrite(motor1pin1, LOW);
      digitalWrite(motor1pin2, HIGH);
      analogWrite(motor1Enable, map(strength, 0, 100, 0, 255));
    
      digitalWrite(motor2pin1, LOW);
      digitalWrite(motor2pin2, HIGH);
      analogWrite(motor2Enable, map(angle*strength, 27000, 36000, 0, 255));
    }
    // back left
    if (angle > 90 and angle < 180) {
      digitalWrite(motor1pin1, HIGH);
      digitalWrite(motor1pin2, LOW);
      analogWrite(motor1Enable, map(angle*strength, 9000, 18000, 0, 255));
    
      digitalWrite(motor2pin1, HIGH);
      digitalWrite(motor2pin2, LOW);
      analogWrite(motor2Enable, map(strength, 0, 100, 0, 255));
    }
    
    // back right
    if (angle >= 180 and angle < 270) {
      digitalWrite(motor1pin1, HIGH);
      digitalWrite(motor1pin2, LOW);
      analogWrite(motor1Enable, map(strength, 0, 100, 0, 255));
    
      digitalWrite(motor2pin1, HIGH);
      digitalWrite(motor2pin2, LOW);
      analogWrite(motor2Enable, map(angle*strength, 27000, 18000, 0, 255));
    }
  }
  else {
    // Turn off motors
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);
    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, LOW);
  }
}
