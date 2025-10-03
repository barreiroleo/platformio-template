#include <Arduino.h>
#include <unity.h>

void setUp(void) { }

void tearDown(void) { }

void test_led_builtin_pin_number(void)
{
    TEST_ASSERT_EQUAL(13, LED_BUILTIN);
}

void test_led_state_high(void)
{
    digitalWrite(LED_BUILTIN, HIGH);
    TEST_ASSERT_EQUAL(HIGH, digitalRead(LED_BUILTIN));
}

void test_led_state_low(void)
{
    digitalWrite(LED_BUILTIN, LOW);
    TEST_ASSERT_EQUAL(LOW, digitalRead(LED_BUILTIN));
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    UNITY_BEGIN();
    RUN_TEST(test_led_builtin_pin_number);
}

void loop()
{
    static uint8_t i = 0;
    static uint8_t max_blinks = 2;

    if (i < max_blinks) {
        RUN_TEST(test_led_state_high);
        delay(50);
        RUN_TEST(test_led_state_low);
        delay(50);
        i++;
    } else if (i == max_blinks) {
        UNITY_END();
    }
}
