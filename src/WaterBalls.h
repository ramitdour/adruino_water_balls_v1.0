
#include <Arduino.h>
#include <WaterBallsData.h>
#ifndef WaterBalls_h
#define WaterBalls_h

void write_analog_store_value_to_EEPROM();
void read_analog_value_from_EEPROM();

void myIRS1();
void myIRS1_method();
void myIRS_check();
void setupISR1();


void write_analog_store_value_to_EEPROM();
void read_analog_value_from_EEPROM();
void setup_ultrasonic_sensors();
void setup_output_channels();
void setup_push_buttons();
void setup_analog_read();
void read_analog_value_from_sensor();


void callbackFunction_0();
void callbackFunction_1();
void callbackFunction_2();
void callbackFunction_3();

void toggle_ith_output_channel(uint8_t channel_no);
void stop_ith_ticker_timer(uint8_t channel_no);

void setup_ticker_timers();




void start_ith_ticker_timer(uint8_t channel_no);
void stop_ith_ticker_timer(uint8_t channel_no);
void check_ith_channel_distace_and_take_action(uint8_t channel_no);
void check_all_channels_distace_and_take_action();
void read_ith_sensor_distance(uint8_t sensor_no);
void read_all_sensor_data();
void loop_ticker_timers();

#endif