#include <HCSR04.h>

#include <ArduinoSTL.h>
#include <Arduino.h>
#include <system_configuration.h>
#include <unwind-cxx.h>

#include <Servo.h>
using namespace std;

vector<int> j;

int echo5 () {

}

int infared() {
    
}

int floodfill() {
    //we see a thing
    //turn to the thing
    //move forward until one of 2 things
    //thing disappears or obstacle see in path
    //if obstacle see in path
    //  rotate left until obstacle no more
    //  rotate right until thing no more
    //  consider furthest poitn that is closest to target
    //  need SA
    //  rotate and move until inline obstacle 

    return 0;
}
const uint8_t MAX_SPEED = 18;

// motors
Servo left_motor;
Servo right_motor;
Servo servo;

// distance sensor
const uint8_t trig = 3;
const uint8_t echo = 3;


void setup() {
    Serial.begin(115200);
    left_motor.attach(12);
    right_motor.attach(13);
    pinMode(trig, 0x0);
    pinMode(echo, 0x0);
    Serial.println("Setup complete");
}

void loop() {
    // speeds up
    for (int speed; speed < MAX_SPEED; speed++) {
        left_motor.write(speed);
        right_motor.write(speed);
        Serial.println(speed);
        delay(100);
    }
    // digitalWrite(2, HIGH);
    Serial.println("Motors on");
    left_motor.write(MAX_SPEED);
    right_motor.write(MAX_SPEED);
    delay(2000);
    Serial.println("Motors off");
    left_motor.write(0);
    right_motor.write(0);
}
int servoPin = 9;   // Define the servo control pin
int trigPin = 10;   // Define the ultrasonic sensor trigger pin
int echoPin = 11;   // Define the ultrasonic sensor echo pin
int motorPin1 = 6;  // Define motor control pins for forward movement
int motorPin2 = 7;
int motorPin3 = 8;  // Define motor control pins for turning
int motorPin4 = 5;

void setup() {
  pinMode(trigPin, OUTPUT);  // Set trigPin as output
  pinMode(echoPin, INPUT);   // Set echoPin as input
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  servo.attach(servoPin);  // Attach the servo to the control pin
  Serial.begin(9600);        // Start the serial communication
}

long readDistance() {
  digitalWrite(trigPin, LOW);  
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW); 
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

void turnToThing() {
  servo.write(90); // Turn to the thing (assuming it's directly ahead)
}

void moveForward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
}

void stopMovement() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
}

void rotateLeft() {
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

void rotateRight() {
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
}

void alignAndMoveToThing() {
  moveForward();  // Start moving forward

  while (true) {
    long distance = readDistance();

    if (distance > 100) {
      stopMovement();
      Serial.println("Thing disappeared!");
      break;  // Stop if the thing disappears
    }

    if (distance < 20) { // Adjust this threshold based on your sensor
      stopMovement();
      Serial.println("Obstacle detected!");

      // Rotate left until no more obstacle
      rotateLeft();
      delay(500);  // Adjust timing based on obstacle size/distance
      stopMovement();

      // Rotate right until the thing is no more
      rotateRight();
      delay(500);  // Adjust timing based on object size/distance
      stopMovement();

      // Consider the furthest point that is closest to the target
      // Implement logic here to determine the point and rotate/move accordingly

      // If obstacle is inline, rotate and move
      rotateLeft();  // Adjust based on target alignment
      delay(100);    // Adjust timing based on alignment
      moveForward(); // Resume moving forward
    }
  }
}

void loop() {
  turnToThing();     // Initial turn towards the thing
  alignAndMoveToThing();  // Move towards the thing with obstacle avoidance
  delay(1000);       // Delay for stability
}
