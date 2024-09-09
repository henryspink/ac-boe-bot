#include <Servo.h>

Servo left_motor;
const uint8_t left_motor_pin = 12;
Servo right_motor;
const uint8_t right_motor_pin = 13;
uint16_t time = 0;

void setup() {
    Serial.begin(9600);
    Serial.println("Setup start");
    left_motor.attach(left_motor_pin);
    right_motor.attach(right_motor_pin);
    Serial.println("Setup complete");

    left_motor.write(180);
}