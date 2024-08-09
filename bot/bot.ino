
const int left_motor = 12;
const int right_motor = 13;

/*
@params void
@returns void
*/
void setup() {
    Serial.begin(115200);
    pinMode(left_motor, OUTPUT);
    pinMode(right_motor, OUTPUT);

}

void loop() {
    // Yaseen was here
    // hii
    digitalWrite(left_motor, HIGH);
    digitalWrite(right_motor, HIGH);
    Serial.println("Motor on high voltage");
    delay(2000);
    digitalWrite(left_motor, LOW);
    digitalWrite(right_motor, LOW);
    Serial.println("Motor on low voltage");
}