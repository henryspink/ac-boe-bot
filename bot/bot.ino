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

float heading = 0;

// struct State {
//     public:
//         float dist;         /* cm               */
//         float heading;      /* deg 0 -> 360     */
//         float speed;        /* 0 -> 180         */
//         // int ir_rotations;/* deg -90 -> 90    */
// };

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
    ir_rotations += step;

    ir_motor.attach(IR_MOTOR_PIN);
    // either 180 if step is 1, or -180 (which is converted to 0 in the code for Servo::write) if step is -1
    ir_motor.write(180 * step);
    delay(75);
    // detach is the only reliable way i have found to stop the motor
    ir_motor.detach();

    // 'bounce' the ir sensor back and forth
    if (ir_rotations >= 4 || ir_rotations <= -4) step = -step;

    // for some reason the ir sensor returns HIGH when there is no signal, so we invert it to write it to the LED.
    // (idk if this is a wiring issue or a sensor issue)
    int ir_signal = digitalRead(IR_PIN);
    digitalWrite(LED_PIN, !ir_signal);

    //* DEBUG PRINTS
    // Serial.println(ir_rotations);
    // Serial.println(step);
    // Serial.println(ir_signal);
}

void dodge_object(float initialDist) {
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
            
            left_motor.write(180*direction);
            right_motor.write(-180*direction);
            
            delay(50);  // Small delay to allow motor movement
            rotations++;
            infrared();

        }
        
        // Reset position
        for (int i = 0; i < rotations; i++) {
            left_motor.write(180*direction);
            right_motor.write(-180*direction);
            delay(50);
            infrared();

        }
        
        direction *= -1;  // Switch direction for next iteration
        leftRotations = rotations;
        rotations = 0;
        while (true) {
            currentDist = distSensor.measureDistanceCm();
            if (currentDist < 0 || abs(currentDist - initialDist) / initialDist > 0.1) {
                break;
            }
            
            left_motor.write(180*direction);
            right_motor.write(-180*direction);
            
            delay(50);  // Small delay to allow motor movement
            rotations++;
            infrared();

        }
        rightRotations = rotations;
        if (rightRotations > leftRotations) {
            direction *= -1;
            for (int i = 0; i < (rightRotations + leftRotations); i++) {
                left_motor.write(180*direction);
                right_motor.write(-180*direction);
                infrared();
            }
        }
    }
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
    infrared();
    if ((round(dist) < DIST_THRESH) && (dist > 0)) { // Object is infront of it
        Serial.println("Object detected");
        dodge_object(dist);
        forwards(180);
    } else {
        Serial.println("No object detected");
        forwards(180);
    }

    //* DEBUG PRINT AND DELAY
    // print_dist(dist);
    // delay(100);
}
