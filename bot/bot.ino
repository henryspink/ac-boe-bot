#include <HCSR04.h>
#include <Servo.h>

const Instruction FORWARDS_MAX = Instruction {
    .speed = 1,
    .heading = 0
};

const Instruction STOP = Instruction {
    .speed = 0,
    .heading = 0
};

const uint16_t distGround = /* !find value! temp: */ 400;
const int BAUD_RATE = 115200;

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

// void print_dist(float dist) {
//     Serial.print("Distance: ");
//     Serial.print(dist);
//     Serial.print("\n");
// }

void movement(State state, Instruction instruction) {
    return;
}

void infrared(State state) {
    return;
}

State findHeading(State state) {
    float currentDist = state.dist;
    int leftSideRotations;
    int rightSideRotations;

    while (((currentDist <= currentDist * 1.1) && (currentDist >= currentDist * 0.9)) || (currentDist == -1)) { // 10% tolerance
        if (currentDist != -1) {
            left_motor.write(180);
            leftSideRotations++;
            currentDist = distSensor.measureDistanceCm();
        }
    }
    for (int i = 0; i < leftSideRotations; i++) {
        right_motor.write(-180);
    }
    while (((currentDist <= currentDist * 1.1) && (currentDist >= currentDist * 0.9)) || (currentDist == -1)) { // 10% tolerance
        if (currentDist != -1) {
            right_motor.write(-180);
            currentDist = distSensor.measureDistanceCm();
            rightSideRotations++;
        }
    }

    if (leftSideRotations > rightSideRotations) {
        for (int i = 0; i < rightSideRotations; i++) {
            left_motor.write(180);
        }
        while (((currentDist <= currentDist * 1.1) && (currentDist >= currentDist * 0.9)) || (currentDist == -1)) { // 10% tolerance
            if (currentDist != -1) {
                left_motor.write(-180);
                currentDist = distSensor.measureDistanceCm();
                leftSideRotations++;
            }
        }
        return State {
            .dist = currentDist,
            .heading = state.heading - leftSideRotations,
            .speed = state.speed
        };
    } else {
        return State {
            .dist = currentDist,
            .heading = state.heading + rightSideRotations,
            .speed = state.speed
        };
    }

    
}

void setup() {
    Serial.begin(BAUD_RATE);
    // Serial.println("Setup start");
    left_motor.attach(left_motor_pin);
    right_motor.attach(right_motor_pin);
    // Serial.println("Setup complete");
}

void loop() {
    // left_motor.write(180);
    // right_motor.write(-180);
    float dist = distSensor.measureDistanceCm();
    // Serial.print(dist);
    State state {
        .dist = dist,
        .heading = heading,
        .speed = 0
    };
    if (dist < 0) {
        // Serial.println("Distance error");
    } else if (dist < distGround+10) {
        // print_dist(dist);
        state = findHeading(state);
        left_motor.write(-180);
        right_motor.write(180);
        // movement(state, )
    } else {
        Instruction instruction {
            .speed = 1,
            .heading = 0
        };
        // Serial.println(instruction.speed);
        movement(state, instruction);
    }
}
