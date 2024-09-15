// // #include <HCSR04.h>
// #include <Servo.h>

// // const uint8_t led = 6;

// // const uint8_t trig = 4;
// // const uint8_t echo = 3;
// // UltraSonicDistanceSensor distSensor = UltraSonicDistanceSensor(trig, echo);

// // const uint8_t ir = 2;

// Servo ir_motor;
// uint8_t ir_motor_pin = 11;

// // int ir_rotations = 0;
// // int sign = -1;

// void setup() {
//     Serial.begin(9600);
//     Serial.println("Setup start");
//     // pinMode(led, OUTPUT);
//     // pinMode(ir, INPUT);
//     ir_motor.attach(ir_motor_pin);
//     Serial.println("Setup complete");
// }

// void loop() {
//     // float dist = distSensor.measureDistanceCm();
//     // bool ir_signal = digitalRead(ir);
//     // // Serial.println(dist);
//     // // if (dist < 0) {
//     // //     digitalWrite(led, LOW);
//     // // } else {
//     // //     digitalWrite(led, HIGH);
//     // // }
//     // // Serial.println(ir_signal);
//     // digitalWrite(led, !ir_signal);

//     // //! TODO: find rotations amount


//     // Serial.println(sign);
//     // Serial.println(50*sign);
//     // ir_motor.write(50*sign);
//     // delay(10);
//     // ir_motor.write(0);
//     // ir_rotations += sign;
//     // Serial.println(ir_rotations);
//     // // if (ir_rotations >= 4 || ir_rotations <= -4) sign = -sign;
//     // if (ir_rotations >= 4) {
//     //   sign = -1;
//     // } else if (ir_rotations <= -4) {
//     //   sign = 1;
//     // }
//     // delay(1000);
//     // // Serial.println(sign);

//     // // if (ir_rotations >= 0 && ir_rotations < 4) {
//     // //     ir_motor.write(180);
//     // //     delay(50);
//     // //     ir_motor.write(0);
//     // //     ir_rotations--;
//     // // } else if (ir_rotations <= 0 && ir_rotations > -4){
//     // //     ir_motor.write(-180);
//     // //     delay(50);
//     // //     ir_motor.write(0);
//     // //     ir_rotations++;
//     // // } else {
//     // //   Serial.println("die");
//     // // }
//     ir_motor.write(50); // Set speed
//     delay(1000); // Run for 1 second
//     ir_motor.write(0); // Stop motor
//     delay(500); // Wait for 0.5 seconds
//     Serial.println("hi");
//     // Move backward
//     ir_motor.write(-50); // Set speed
//     delay(1000); // Run for 1 second
//     ir_motor.write(0); // Stop motor
//     delay(500); // Wait for 0.5 second
// }