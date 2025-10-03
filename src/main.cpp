#include <Arduino.h>

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    static int counter {};
    Serial.println("Counter: " + String(counter++));
}
