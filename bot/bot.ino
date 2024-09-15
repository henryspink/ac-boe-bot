#include <HCSR04.h>
#include <Servo.h>

#define BAUD_RATE 115200

// motors
Servo ir_motor;
Servo left_motor;
Servo right_motor;
#define IR_MOTOR_PIN 10
#define LEFT_MOTOR_PIN 12
#define RIGHT_MOTOR_PIN 13

// distance sensor
#define DIST_THRESH 20
#define TRIGGER_PIN 4
#define ECHO_PIN 3
UltraSonicDistanceSensor distSensor = UltraSonicDistanceSensor(TRIGGER_PIN, ECHO_PIN);

#define IR_PIN 2
#define LED_PIN 6

int ir_rotations = 0;
int step = 1;

// namespace math {
//     #include <cmath>
// }

float heading = 0;

struct State {
    public:
        float dist;         /* cm               */
        float heading;      /* deg 0 -> 360     */
        float speed;        /* 0 -> 180      */
        // int ir_rotations;   /* deg -90 -> 90    */
};

// struct Instruction {
//     public:
//         float speed;
//         float heading;
// };


// namespace DIRECTION {
//     const Instruction FORWARDS = Instruction {
//         .speed = 180,
//         .heading = 0
//     };

//     const Instruction BACKWARDS = Instruction {
//         .speed = 0,
//         .heading = 0
//     };

//     const Instruction STOP = Instruction {
//         .speed = 0,
//         .heading = 0
//     };
// }

void print_dist(float dist) {
    dist > 0 ? Serial.print("Distance: ") : Serial.print("Distance error: ");
    Serial.print(dist);
    Serial.print("\n");
}

// void movement(State state, Instruction instruction) {
//     left_motor.attach(LEFT_MOTOR_PIN);
//     right_motor.attach(RIGHT_MOTOR_PIN);
//     int val = map(instruction.speed, -180, 180, 0, 180);
//     if (instruction.speed == 0) {
//         left_motor.detach();
//         right_motor.detach();
//         return;
//     }
//     if (instruction.heading != 0) {
           // heading = state.heading + instruction.heading;
//     }
//     left_motor.write(val);
//     right_motor.write(val);
// }

void stop() {
    left_motor.detach();
    right_motor.detach();
}

void forwards(int speed) {
    left_motor.attach(LEFT_MOTOR_PIN);
    right_motor.attach(RIGHT_MOTOR_PIN);
    left_motor.write(180-speed);
    right_motor.write(speed);
}

void infrared() {
    //! TODO: find rotations amount
    // Serial.println(ir_rotations);
    // Serial.println(step);

    ir_rotations += step;
    ir_motor.attach(IR_MOTOR_PIN);

    // ir_motor.write(50 * step);
    if (step == -1) {
        ir_motor.write(0);
        delay(75);
        ir_motor.detach();
    } else if (step == 1) {
        ir_motor.write(180);
        delay(75);
        ir_motor.detach();
    } else {
        Serial.println("Invalid step");
    }

    if (ir_rotations >= 4 || ir_rotations <= -4) step = -step;

    int ir_signal = digitalRead(IR_PIN);
    digitalWrite(LED_PIN, !ir_signal);
    // Serial.println(ir_signal);
    // return State {
    //     .dist = state.dist,
    //     .heading = state.heading,
    //     .speed = state.speed,
    //     .ir_rotations = rotations
    // };
}

State dodge_object(State state) {
    float initialDist = state.dist;
    float currentDist = initialDist;

    int leftRotations = 0;
    int rightRotations = 0;

    int rotations = 0;
    int direction = 1;  // 1 for left, -1 for right

    Serial.println("Dodge object");

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
        leftRotations = rotations;
        rotations = 0;
        while (true) {
            currentDist = distSensor.measureDistanceCm();
            if (currentDist < 0 || abs(currentDist - initialDist) / initialDist > 0.1) {
                break;
            }
            
            if (direction == 1) {
                left_motor.write(-180);
                right_motor.write(0);
            } else {
                right_motor.write(-180);
                left_motor.write(0);
            }
            
            delay(50);  // Small delay to allow motor movement
            rotations++;
        }
        rightRotations = rotations;
        if (rightRotations > leftRotations) {
            direction *= -1;
            for (int i = 0; i < (rightRotations + leftRotations); i++) {
                if (direction == 1) {
                    left_motor.write(-180);
                    right_motor.write(0);
                } else {
                    right_motor.write(-180);
                    left_motor.write(0);
                }
            }
        }
    }

    return State {
        .dist = currentDist,
        .heading = state.heading + (direction * rotations),
        .speed = state.speed,
        // .ir_rotations = state.ir_rotations,
    };
}

void setup() {
    Serial.begin(BAUD_RATE);
    Serial.println("Setup start");
    ir_motor.attach(IR_MOTOR_PIN);
    left_motor.attach(LEFT_MOTOR_PIN);
    right_motor.attach(RIGHT_MOTOR_PIN);
    pinMode(IR_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    Serial.println("Setup complete");
}

void loop() {
    float dist = distSensor.measureDistanceCm();
    // print_dist(dist);
    State state {
        .dist = dist,
        .heading = heading,
        .speed = 0
    };
    infrared();
    if ((round(dist) < DIST_THRESH) && (state.dist > 0)) { //Object is infront of it
        Serial.println("Object detected");
        state = dodge_object(state);
        forwards(180);
    } else {
        Serial.println("No object detected");
        forwards(180);
    }
    delay(100);
}
