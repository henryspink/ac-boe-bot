#include <HCSR04.h>
#include <Servo.h>

const uint16_t distGround = /* !find value! temp: */ 400;

// motors
Servo left_motor;
const uint8_t left_motor_pin = 12;
Servo right_motor;
const uint8_t right_motor_pin = 13;

// distance sensor
const uint8_t trig = 11;
const uint8_t echo = 10;
UltraSonicDistanceSensor distSensor = UltraSonicDistanceSensor(trig, echo);

float heading = 0;

struct State {
    public:
        float dist;
        float heading;
        float speed;
};

struct Instruction {
    public:
        float speed;
        float heading;
};

void print_dist(float dist) {
    Serial.print("Distance: ");
    Serial.print(dist);
    Serial.print("\n");
}

void movement(State state, Instruction instruction) {
    return;
}

void infrared(State state) {
    return;
}

void rotate(State state) {
    int leftSideRotations;
    int rightSideRotations;

    while ((state.dist >= state.dist * 1.1) && (state.dist <= state.dist * 0.9)) { // 10% tolerance
        left_motor.write(180);
        leftSideRotations++;
        state.dist = distSensor.measureDistanceCm();
    }
    while ((state.dist >= state.dist * 1.1) && (state.dist <= state.dist * 0.9)) { // 10% tolerance
        right_motor.write(180);
        state.dist = distSensor.measureDistanceCm();
        rightSideRotations++;
    }

    if (leftSideRotations > rightSideRotations) {
        while ((state.dist >= state.dist * 1.1) && (state.dist <= state.dist * 0.9)) { // 10% tolerance
            left_motor.write(180);
            state.dist = distSensor.measureDistanceCm();
            leftSideRotations++;
        }
    } else {
        while ((state.dist >= state.dist * 1.1) && (state.dist <= state.dist * 0.9)) { // 10% tolerance
            right_motor.write(180);
            state.dist = distSensor.measureDistanceCm();
            rightSideRotations++;
        }
    }
}

void setup() {
    Serial.begin(9600);
    Serial.println("Setup start");
    left_motor.attach(left_motor_pin);
    right_motor.attach(right_motor_pin);
    Serial.println("Setup complete");
}

void loop() {
    float dist = distSensor.measureDistanceCm();
    State state {
        .dist = dist,
        .heading = heading,
        .speed = 0
    };
    if (dist < 0) {
        Serial.println("Distance error");
    } else if (dist < distGround+10) {
        print_dist(dist);
    } else {
        Instruction instruction {

        };
        movement(state, instruction);
    }
}
