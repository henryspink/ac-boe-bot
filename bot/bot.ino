#include <HCSR04.h>

#include <Arduino.h>
#include <ArduinoSTL.h>

// #include <system_configuration.h>
// #include <unwind-cxx.h>

#include <Servo.h>

// using namespace std;

std::vector<int> j;


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

const uint8_t BAUD_RATE = 115200;
const uint8_t MAX_SPEED = 18;

// motors
Servo left_motor;
const uint8_t left_motor_pin = 12;
Servo right_motor;
const uint8_t right_motor_pin = 13;

// distance sensor
const uint8_t trig = 11;
const uint8_t echo = 10;
UltraSonicDistanceSensor distSensor = UltraSonicDistanceSensor(trig, echo);


void setup() {
    left_motor.attach(left_motor_pin);
    right_motor.attach(right_motor_pin);
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    Serial.begin(BAUD_RATE);
    Serial.println("Setup complete");

    // IR Sensor
    pinMode(6, OUTPUT);
}

void loop() {
    // // speeds up
    // for (int speed; speed < MAX_SPEED; speed++) {
    //     left_motor.write(speed);
    //     right_motor.write(speed);
    //     Serial.println(speed);
    //     delay(100);
    // }
    // // digitalWrite(2, HIGH);
    // Serial.println("Motors on");
    // left_motor.write(MAX_SPEED);
    // right_motor.write(MAX_SPEED);
    // delay(2000);
    // Serial.println("Motors off");
    // left_motor.write(0);
    // right_motor.write(0);

    Serial.print("Distance: ");
    Serial.print(distSensor.measureDistanceCm());
}
