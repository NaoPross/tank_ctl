#include <Arduino.h>
#include <Servo.h>

#include <stdio.h>

#define D_TIME 100
#define LV_MAX 512

#define PIN_SENSOR A0
#define PIN_SERVO  10

#define PIN_RED   4
#define PIN_GREEN 3
#define PIN_BLUE  2

#define ROTATION_ANGLE 90
#define OPEN_TIMEOUT 100000

Servo servo;
bool servo_open = false;

int last_open_time;

void setup()
{
    Serial.begin(9600);
    servo.attach(PIN_SERVO);

    pinMode(PIN_SENSOR, INPUT);
    pinMode(PIN_SERVO, OUTPUT);

    pinMode(PIN_RED, OUTPUT);
    pinMode(PIN_GREEN, OUTPUT);
    pinMode(PIN_BLUE, OUTPUT);

    digitalWrite(PIN_BLUE, HIGH);
}

void loop()
{
    int sensor_raw = 1024 - analogRead(PIN_SENSOR);

    if (sensor_raw > LV_MAX) {
        digitalWrite(PIN_RED, LOW);
        digitalWrite(PIN_GREEN, HIGH);
    
        if (!servo_open) {
            servo_open = true;
            servo.write(ROTATION_ANGLE);
        }
    }
    else {
        digitalWrite(PIN_RED, HIGH);
        digitalWrite(PIN_GREEN, LOW);

        if (servo_open) {
            servo_open = false;
            servo.write(0);
        }
    }

    // Serial data
    char* n = "0000";
    // n = (char*) calloc(4, sizeof(char));
    sprintf(n, "%04d", sensor_raw);
    Serial.print(n);

    delay(D_TIME);
}
