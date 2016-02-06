#include <Arduino.h>
#include <stdio.h>
#include <math.h>

#define D_TIME 100

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    char* n = "000";
    for (int i = 0; i < 250; i++) {
        sprintf(n, "%03d", i);
        Serial.print(n);
        delay(D_TIME);
    }

    delay(D_TIME);
}
