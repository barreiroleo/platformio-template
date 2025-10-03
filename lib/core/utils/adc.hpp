#pragma once

#include <Arduino.h>

namespace core::adc {

using ADC_raw = uint16_t; //< ADC value in raw format (0-1023 for 10-bit ADC).
using ADC_mv = float; //< ADC value in millivolts (0-5000 mV for 10-bit ADC with 5V reference).

/// @brief Convert raw ADC value to millivolts.
///        Assumes a 10-bit ADC (0-1023) with a 5V reference (0-5000mV).
/// @param[in] raw_adc Raw ADC value.
/// @return Voltage in millivolts
constexpr ADC_mv raw_to_mv(uint16_t raw_adc)
{
    constexpr auto adc_range = (1 << 10) - 1; // 10-bit ADC max value (1023)
    constexpr auto adc_scale = (5000U / static_cast<float>(adc_range));
    return raw_adc * adc_scale;
}

/// @brief Read ADC value in raw format from a pin.
/// @param[in] pin Analog pin to read from.
inline ADC_raw read_raw(uint8_t pin)
{
    return static_cast<ADC_raw>(analogRead(pin));
}

/// @brief Read ADC value in millivolts from a pin.
/// @param[in] pin Analog pin to read from.
inline ADC_mv read_mv(uint8_t pin)
{
    return raw_to_mv(read_raw(pin));
}

// @brief Convert ADC_Value to a string representation.
inline String to_string(ADC_raw value_raw)
{
    char buffer[16] {};

    const auto value_mv = raw_to_mv(value_raw);
    const auto result = snprintf(buffer, sizeof(buffer), "%u, %u", value_raw, static_cast<uint16_t>(value_mv));
    if (result < 0 || static_cast<size_t>(result) >= sizeof(buffer)) {
        return F("Err, Err");
    }

    return buffer;
}

} // namespace core::adc

static_assert(core::adc::raw_to_mv(0) == 0.0);
static_assert(core::adc::raw_to_mv(1023) == 5000U);
