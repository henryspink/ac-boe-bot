#include <HCSR04.h>
#include <Servo.h>

// motors
Servo left_motor;
const uint8_t left_motor_pin = 12;
Servo right_motor;
const uint8_t right_motor_pin = 13;

// distance sensor
const uint8_t trig = 11;
const uint8_t echo = 10;
UltraSonicDistanceSensor distSensor = UltraSonicDistanceSensor(trig, echo);


void print_dist(float dist) {
    Serial.print("Distance: ");
    Serial.print(dist);
    Serial.print("\n");
}

void setup() {
  Serial.begin(9600);
  Serial.println("Setup start");
  Serial.println("Setup complete\n");
}

void loop() {
    float dist = distSensor.measureDistanceCm();
    if (dist != -1) {
        // Update state
    }

    if (dist >= 5) {
        // Rotate
    }
  print_dist(distSensor.measureDistanceCm());
}
