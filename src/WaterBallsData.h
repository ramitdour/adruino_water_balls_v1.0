#include <Arduino.h>

#ifndef WaterBallsData_h
#define WaterBallsData_h

#define DEBUG_CODE 1 // TODO:comment in productions

#ifdef DEBUG_CODE

#define serial_baud_rate 115200
#define nos_channel 1
const uint8_t echoPins[] = {9};
const uint8_t trigPins[] = {8};
const uint8_t output_channels[] = {13};
unsigned long previousMillis_cool_off_print = 0;
#define previousMillis_cool_off_print_interval 250

#else

#define nos_channel 4
const uint8_t echoPins[] = {1, 5, 7, 9};
const uint8_t trigPins[] = {0, 4, 6, 8};
const uint8_t output_channels[] = {10, 11, 12, 13};

#endif
// 

#define analog_read_pot_pin A1
#define analog_read_pot_LED_pin 3

const uint16_t debounceDuration = 300; // Debounce duration for any interrupt
const uint8_t builtInButton = 2;       // On board button of Node MCU 1.0

#define activation_min_dist 15      // in cms
#define activation_safe_zone_dist 5 // in cms
//#define water_falling_time_ms 5000  // in ms

#define water_falling_repeats 1
#define dealy_in_action_ms 1000   // ms
#define cool_off_duration_ms 2000 // ms

#define default_min_dist 40       // in cms
#define default_safe_zone_dist 30 // in cms

// constants won't change:
const long interval_bw_reading = 1000; // interval at which to blink (milliseconds)

#define start_index_add 0
#define analog_reading_nos_to_avg 10

// DO NOT CHANGE , ultra_sonic_settings

#define US_start_delay_ms 2
#define US_pulse_duration_ms 10
#define US_sound_speed 0.034

#ifdef DEBUG_CODE

#endif

#endif