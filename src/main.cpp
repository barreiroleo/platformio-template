#include <Arduino.h>

#include <utils/adc.hpp>

constexpr uint8_t SENSOR_INPUT_PIN = A0;

void setup()
{
    Serial.begin(9600);
    pinMode(SENSOR_INPUT_PIN, INPUT);

    Serial.println("ADC; Voltage;");
}

void loop()
{
    const auto adc_raw_value = core::adc::read_raw(SENSOR_INPUT_PIN);
    Serial.println(core::adc::to_string(adc_raw_value));
}
