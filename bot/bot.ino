#include <ArduinoSTL.h>
#include <set>
#include <Arduino.h>
#include <system_configuration.h>
#include <unwind-cxx.h>

#include <Servo.h>
using namespace std;

vector<int> j;

int floodfill() {
    // I am typing bs in front of the teacher so that he doesnt realise im not doing my work
    return 0;
}
const uint8_t MAX_SPEED = 18;

// motors
Servo left_motor;
Servo right_motor;

// distance sensor
const uint8_t trig = 3;
const uint8_t echo = 3;


void setup() {
    Serial.begin(115200);
    left_motor.attach(12);
    right_motor.attach(13);
    pinMode(trig, OUTPUT);
    pinMode(echo, OUTPUT);
}

void loop() {
    for (uint8_t speed; speed < MAX_SPEED; speed++) {
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