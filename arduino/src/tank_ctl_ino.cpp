/** TANK_CTL **
 *
 * Abstract:
 *
 * Questo programma e` una simulazioe di un sistema di controllo di un
 * serbatoio. Misurando da un sensore se il serbatoio dovesse essere pieno
 * quest'ultimo deve suotarsi attraverso la rotazione di un servomotore.
 */

#include "serial.h"

#include <Arduino.h>
#include <Servo.h>

#include <stdio.h>

#define D_TIME 100
#define LVL_MAX 512

#define PIN_SENSOR A0
#define PIN_SERVO  10

#define PIN_RED   4
#define PIN_GREEN 3
#define PIN_BLUE  2

#define ROTATION_ANGLE 90
#define OPEN_TIMEOUT 10000


Servo servo;
bool servo_open = false;
int last_open_time;

void setup()
{
    Serial.begin(9600);
    servo.attach(PIN_SERVO);

    pinMode(PIN_SENSOR, INPUT);
    pinMode(PIN_RED, OUTPUT);
    pinMode(PIN_GREEN, OUTPUT);
    pinMode(PIN_BLUE, OUTPUT);

    digitalWrite(PIN_BLUE, HIGH);
}

void loop()
{
    // reads how dark is the envionment
    int sensor_raw = analogRead(PIN_SENSOR);

    if (sensor_raw > LVL_MAX) {
        digitalWrite(PIN_RED, LOW);
        digitalWrite(PIN_GREEN, HIGH);
    
        // Open the tank
        if (!servo_open) {
            servo_open = true;
            servo.write(ROTATION_ANGLE);
        }
    }
    else {
        digitalWrite(PIN_RED, HIGH);
        digitalWrite(PIN_GREEN, LOW);

        // Close the tank
        if (servo_open) {
            servo_open = false;
            digitalWrite(PIN_GREEN, HIGH);

            // blink while waiting
            for (int i = 0; i < 10; i++) {
                digitalWrite(PIN_RED, LOW);
                delay(500);
                digitalWrite(PIN_RED, HIGH);
                delay(500);
            }

            servo.write(0);
        }
    }

    // Serial data
    struct packet sensor_data;
    char* n = "0000";

    // n = (char*) calloc(4, sizeof(char));
    sprintf(n, "%04d", sensor_raw);
    sensor_data.msg_size = 4;
    sensor_data.msg = n;

    send_packet(sensor_data);

    delay(D_TIME);
}
