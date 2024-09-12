#include <HCSR04.h>
#include <Servo.h>


// 360 time

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


const Instruction FORWARDS_MAX = Instruction {
    .speed = 180,
    .heading = 0
};

const Instruction BACKWARDS_MAX = Instruction {
    .speed = -180,
    .heading = 0
};

const Instruction STOP = Instruction {
    .speed = 0,
    .heading = 0
};

// void print_dist(float dist) {
//     Serial.print("Distance: ");
//     Serial.print(dist);
//     Serial.print("\n");
// }

void movement(State state, Instruction instruction) {
    if (instruction.heading != 0) {
        // heading = state.heading + instruction.heading;
    }
    left_motor.write(-instruction.speed);
    left_motor.write(instruction.speed);
}

void infrared(State state) {
    return;
}

State findHeading(State state) {
    float initialDist = state.dist;
    float currentDist = initialDist;
    int rotations = 0;
    int direction = 1;  // 1 for left, -1 for right

    for (int side = 0; side < 2; side++) {
        while (true) {
            currentDist = distSensor.measureDistanceCm();
            if (currentDist < 0 || abs(currentDist - initialDist) / initialDist > 0.1) {
                break;
            }
            
            if (direction == 1) {
                left_motor.write(180);
                right_motor.write(0);
            } else {
                right_motor.write(-180);
                left_motor.write(0);
            }
            
            delay(50);  // Small delay to allow motor movement
            rotations++;
        }
        
        // Reset position
        for (int i = 0; i < rotations; i++) {
            if (direction == 1) {
                right_motor.write(-180);
                left_motor.write(0);
            } else {
                left_motor.write(180);
                right_motor.write(0);
            }
            delay(50);
        }
        
        direction *= -1;  // Switch direction for next iteration
        rotations = 0;
    }

    return State {
        .dist = currentDist,
        .heading = state.heading + (direction * rotations),
        .speed = state.speed
    };
}

void setup() {
    Serial.begin(9600);
    // Serial.println("Setup start");
    left_motor.attach(left_motor_pin);
    right_motor.attach(right_motor_pin);
    // Serial.println("Setup complete");
}

void loop() {
    // left_motor.write(180);
    // right_motor.write(-180);
    float dist = distSensor.measureDistanceCm();
    Serial.println(dist);
    State state {
        .dist = dist,
        .heading = heading,
        .speed = 0
    };
    if (dist < 0) {
        // Serial.println("Distance error");
    } else if (dist < distGround+10) {
        // Serial.println(dist);
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
