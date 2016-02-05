#include <Arduino.h>
#include <stdio.h>

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    char* n = "000";
    for (int i = 0; i < 300; i++) {
        sprintf(n, "%03d", i);
        Serial.print(n);
        delay(500);
    }

    delay(2000);
}
