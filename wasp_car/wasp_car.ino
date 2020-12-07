#include <SoftwareSerial.h> 

// HC-05 bluetooth module communication via Serial
SoftwareSerial hc05(13, 12); // RX | TX 

String hc05Value = "";
int angle = 0;
int strength = 0;
int button = 0;
int LED = 10; 


// L298N control via 6 GPIO pins (digital and analog)
// Motor 1 is the one which is on the LEFT hand side
int motor1pin1 = 7;
int motor1pin2 = 4;
int motor1Enable = 5;

// Motor 2 is the one which is on the RIGHT hand side
int motor2pin1 = 2;
int motor2pin2 = 3;
int motor2Enable = 6;

// Main setup method which calls both Bluetooth HC-05 and L298N motors setup
void setup() {
  setupBluetooth();
  setupMotors();
}

// Main loop which reads joystick input via bluetooth HC-05 from Android application
// and then makes moves using DC motors controlled via L298N DC motor drive 
void loop() {
  loopBluetooth();
  loopMotors();
}


// Setup Bluetooth HC-05 module
void setupBluetooth() 
{   
 Serial.begin(9600); 
 hc05.begin(9600); 
 pinMode(LED, OUTPUT); 

 // TODO set custom password
 Serial.println("Ready to connect\nDefualt password is 1234 or 000"); 
}  

// Reads joystick input via bluetooth HC-05 from Android application
void loopBluetooth() {
  
  if (hc05.available()>0) 
  {
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


// setup L298N motors drive
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
    
    Serial.print("A:");
    Serial.print(angle);
    Serial.print("S:");
    Serial.print(strength);
    
    // forward left
    if (0 <= angle and angle <= 45) {
      motor1Forward(map(angle, 45, 0, 0, 255)*strength/100);
      motor2Forward(map(strength, 0, 100, 0, 255));
    }
    
    // left forward
    else if (45 < angle and angle <= 90) {
      motor1Backwards(map(angle, 45, 90, 0, 255)*strength/100);
      motor2Forward(map(strength, 0, 100, 0, 255));
    }
    
    // left backwards
    else if (90 < angle and angle <= 135) {
      motor1Backwards(map(strength, 0, 100, 0, 255));
      motor2Forward(map(angle, 135, 90, 0, 255)*strength/100);
    }
    
    // backwards left
    else if (135 < angle and angle <= 180) {
      motor1Backwards(map(strength, 0, 100, 0, 255));
      motor2Backwards(map(angle, 135, 180, 0, 255)*strength/100);
    }
    
    // backwards right
    else if (180 < angle and angle <= 225) {
      motor1Backwards(map(strength, 0, 100, 0, 255));
      motor2Backwards(map(angle, 225, 180, 0, 255)*strength/100);
    }
    
    // right backwards
    else if (225 < angle and angle <= 270) {
      motor1Backwards(map(strength, 0, 100, 0, 255));
      motor2Forward(map(angle, 225, 270, 0, 255)*strength/100);
    }
    
    // right forward
    else if (270 < angle and angle <= 315) {
      motor1Forward(map(strength, 0, 100, 0, 255));
      motor2Backwards(map(angle, 315, 270, 0, 255)*strength/100);
    }

    // forward right
    else if (315 < angle and angle <= 360) {
      motor1Forward(map(strength, 0, 100, 0, 255));
      motor2Forward(map(angle, 315, 360, 0, 255)*strength/100);
    }
    Serial.println();
  }
  else {
    // Turn off motors
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);
    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, LOW);
  }
}

/* 
 *  Motor 1 runs FORWARD.
 *  
 *  Motor 1 is the one which is on the LEFT hand side
 *  
 *  power - how fast and powerful wheels should be spinning. Value between 0 and 255
 */
void motor1Forward(float power) {
      digitalWrite(motor1pin1, LOW);
      digitalWrite(motor1pin2, HIGH);
      analogWrite(motor1Enable, power);
      
      Serial.print(";M1F:");
      Serial.print(power);
}

/* 
 *  Motor 2 runs FORWARD.
 *  
 *  Motor 2 is the one which is on the RIGHT hand side
 *  
 *  power - how fast and powerful wheels should be spinning. Value between 0 and 255
 */
void motor2Forward(float power) {
      digitalWrite(motor2pin1, LOW);
      digitalWrite(motor2pin2, HIGH);
      analogWrite(motor2Enable, power);
      
      Serial.print(";M2F:");
      Serial.print(power);
}

/* 
 *  Motor 1 runs BACKWARDS.
 *  
 *  Motor 1 is the one which is on the LEFT hand side
 *  
 *  power - how fast and powerful wheels should be spinning. Value between 0 and 255
 */
void motor1Backwards(float power) {
      digitalWrite(motor1pin1, HIGH);
      digitalWrite(motor1pin2, LOW);
      analogWrite(motor1Enable, power);
      
      Serial.print(";M1B:");
      Serial.print(power);
}

/* 
 *  Motor 2 runs BACKWARDS.
 *  
 *  Motor 2 is the one which is on the RIGHT hand side
 *  
 *  power - how fast and powerful wheels should be spinning. Value between 0 and 255
 */
void motor2Backwards(float power) {
      digitalWrite(motor2pin1, HIGH);
      digitalWrite(motor2pin2, LOW);
      analogWrite(motor2Enable, power);
      
      Serial.print(";M1B:");
      Serial.print(power);
}
