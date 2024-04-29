#include <Servo.h>


 // Motor A connections
int enA = 3; // White
int in1 = 4; // Orange
int in2 = 5; // Purple
// Motor B connections
int enB = 11; // White
int in3 = 13; // Blue
int in4 = 12; // Grey
int servo_pin = 10;
int laserPin = 2; // Red
double rightWheelCurrentSpeed = 0;
double leftWheelCurrentSpeed = 0;
  

Servo laserServo;
int angle = 40;
int motorServoPin = 4;
int constantSpeed = 75;

String command = "";




void pinSetup(){

	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
  pinMode(laserPin, OUTPUT);
  pinMode(motorServoPin, OUTPUT);
	
	// Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW); 
  digitalWrite(motorServoPin, HIGH);

  laserServo.attach(servo_pin);
}

void setup() {
  pinSetup();
  laserServo.write(angle);
  Serial.begin(9600);
}


// Commands
// 1 - Forward
// 2 - Backwards
// 3 - Right
// 4 - Left
// 5 - Laser on
// 6 - Laser off
// 7 - Laser Angle: usage: 7009 (9 degree)
void loop() {
  if (Serial.available()) {
    delay(10);
    while (Serial.available() > 0) {
     char character = Serial.read();
     command += character;
    }
    parseCommand();
    command = "";
    Serial.flush();
  }
  
}

void parseCommand() {
   if (command[0] == '1') {
      String timesToRunString = command.substring(1, 4);
      int timesToRun = timesToRunString.toInt();
      forward(timesToRun);
    } else if (command[0] == '2') {
      String timesToRunString = command.substring(1, 4);
      int timesToRun = timesToRunString.toInt();
      backward(timesToRun);
    } else if (command[0] == '3') {
      String timesToRunString = command.substring(1, 4);
      int timesToRun = timesToRunString.toInt();
      right(timesToRun);
    } else if (command[0] == '4') {
      String timesToRunString = command.substring(1, 4);
      int timesToRun = timesToRunString.toInt();
      left(timesToRun);
    } else if (command == "5") {
      startLaser();
    } else if (command == "6") {
      stopLaser();
    } else if (command[0] == '7') {
      String angle = command.substring(1, 4);
      int angleInt = angle.toInt();
      changeLaserAngle(angleInt);
    }
}

// void parseCommand() {
//   // Parsing forward command as an example
//   // The command will be in the following format
//   // <func_name>:<parameter_single>!
//   // The exclaimation mark is a stopper command
//   if (commandString.indexOf("!") != -1) { // This means it has a stopper command
//     // We remove the stopper command
//     commandString.replace("!", "");
//     int sepIndex = commandString.indexOf(":");
    
//     String command = commandString.substring(0,sepIndex);
//     String parameter = commandString.substring(sepIndex + 1, commandString.length());
//     int parameterInt = parameter.toInt();
    
//     // Here we have the command and parameters available
//     // So now we just check what the command is and run the appropriate
//     // function with the parameter
//    if (command == "forward") {
//       forward();
//     }
//     // end of backward command
//   if (command == "backward") {
//       backward();
//     }

//   if (command == "right") {
//       // This is the backward command
//       // We check if there are any parameters - For backward, it will just be speed
//       right();
//     }

//   if (command == "left") {
//       // This is the backward command
//       left();
//    }  

// if (command == "laser") {
//       // This is the backward command
//        int switcher = parameterInt;
//        if (parameterInt == 0){
//          stopLaser();
//        }
    
//       else {
//         startLaser();
//       }
//     }
// if (command == "stop"){
//   stop2();
// }
    
//   }
// }

void backward(int timesToRun) {
    int backwardSpeed = 85;
    double percentageReduction = 14.3;
    double percentageReduced = (percentageReduction * backwardSpeed) / 100;
    double leftWheelSpeed = backwardSpeed - percentageReduction;
    double rightWheelSpeed = backwardSpeed;
    for (int i = 0; i < timesToRun; i++) {
      runMotorBackward(leftWheelSpeed, rightWheelSpeed);
      delay(25);
      stop2();
      delay(15);
    }    
}

void forward(int timesToRun) {
    double percentageReduction = 2;
    double percentageReduced = (percentageReduction * constantSpeed) / 100;
    double leftWheelSpeed = constantSpeed;
    double rightWheelSpeed = constantSpeed - percentageReduced;
    for (int i = 0; i < timesToRun; i++) {
      runMotorForward(leftWheelSpeed, rightWheelSpeed);
      delay(25);
      stop2();
      delay(10);
    }    
}

void spinIdleRight() {
    analogWrite(enA, 80);  
    analogWrite(enB, 80);
    int timesToRun = 7;
    for (int i = 0; i < timesToRun; i++) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
      delay(25);
      stop2();
      delay(25);
    }  
}


void runMotorForward(double leftSpeed, double rightSpeed) {
    rightWheelCurrentSpeed = rightSpeed;
    leftWheelCurrentSpeed = leftSpeed;
    analogWrite(enA, leftSpeed);  
    analogWrite(enB, rightSpeed); 
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
}

void runMotorBackward(double leftSpeed, double rightSpeed) {
    rightWheelCurrentSpeed = rightSpeed;
    leftWheelCurrentSpeed = leftSpeed;
    analogWrite(enA, leftSpeed);  
    analogWrite(enB, rightSpeed); 
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
}

void stop2() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void left(int timesToRun) {
    analogWrite(enA, 80);  
    analogWrite(enB, 80); 
    int timeStarted = millis();
    for (int i = 0; i < timesToRun; i++) {
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      delay(25);
      stop2();
      delay(25);
    }    

}

void right(int timesToRun) {
    analogWrite(enA, 80);  
    analogWrite(enB, 80); 
    int timeStarted = millis();
    for (int i = 0; i < timesToRun; i++) {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
        delay(25);
        stop2();
        delay(25);
      }    
}

void changeLaserAngle(int angle) {
  laserServo.write(angle);
}


void startLaser() {
  digitalWrite(laserPin, HIGH);
}

void stopLaser() {
  digitalWrite(laserPin, LOW);
}


void stop1(){
  while (leftWheelCurrentSpeed > 0 && rightWheelCurrentSpeed > 0) {
    leftWheelCurrentSpeed -= 0.75;
    rightWheelCurrentSpeed -= 0.75;
    analogWrite(enA, leftWheelCurrentSpeed);
    analogWrite(enB, rightWheelCurrentSpeed);
    
  }
    
}