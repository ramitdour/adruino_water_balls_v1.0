#include <Arduino.h>
#include "Ticker.h"
#include <EEPROM.h>
#include "WaterBalls.h"

// #define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
// #define trigPin 3 // attach pin D3 Arduino to pin Trig of HC-SR04

uint32_t water_falling_time_ms = 5000; // in ms;

uint16_t analog_sensor_read_value = 0;

// <Interrupts>
//-common-                                            // Volatile because it is changed by ISR ,
unsigned long currrentMillis_interrupt = 0;

// Interrupts For built-in-button
unsigned long lastValidInterruptTime_1 = 0; // When was the last valid millis on which board push button was pressed
volatile bool myISR1_flag = 0;              // ISR flag for on board button press

// Interrupt service routine , very light weight
// ICACHE_RAM_ATTR void myIRS1() // this is for node mcu
void myIRS1()
{
  myISR1_flag = 1;
}

void myIRS1_method()
{
  Serial.println(F("==myIRS1_method called=="));
  // To read analog values from pot , and store them in memory
  read_analog_value_from_sensor();
  write_analog_store_value_to_EEPROM();
}

// Iterrupt 1 method call check
void myIRS_check()
{
  // Main part of your loop code.
  currrentMillis_interrupt = millis();

  // IRS for in-built button
  if (myISR1_flag)
  {
    if (currrentMillis_interrupt - lastValidInterruptTime_1 > debounceDuration)
    {
      myISR1_flag = 0;
      lastValidInterruptTime_1 = currrentMillis_interrupt;
      myIRS1_method();
    }
    else
    {
      myISR1_flag = 0;
    }
  }
}

void setupISR1()
{
  // Attach an interrupt to the pin, assign the onChange function as a handler and trigger on changes (LOW or HIGH).
  // attachInterrupt(builtInButton, myIRS1 , FALLING);

  attachInterrupt(digitalPinToInterrupt(builtInButton), myIRS1, RISING);

  Serial.print("setupISR1 at pin  = ");
  Serial.println(builtInButton);
}

unsigned long cool_off_duration_millis[nos_channel];

long durations[nos_channel];
float distances[nos_channel];
bool output_channel_state[nos_channel];

bool hand_put_state[nos_channel];

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to storeunsigned long previousMillis = 0;        // will store last time LED was updated

// const long interval_bw_readings[nos_channel] = {1000};

void write_analog_store_value_to_EEPROM()
{
  uint8_t start_index = start_index_add;

  uint8_t block_16t[4] = {0, 0, 0, 0};

  uint16_t temp = analog_sensor_read_value;

  for (uint8_t i = 0; i < 4; i++)
  {
    if (temp > 255)
    {
      block_16t[i] = 255;
      temp = temp - 255;
    }
    else
    {
      block_16t[i] = temp;
      temp = 0;
    }
  }

  for (uint8_t i = 0; i < 4; i++)
  {
    EEPROM.write(start_index + i, block_16t[i]);

    Serial.print(start_index + i);
    Serial.print("\t");
    Serial.print(block_16t[i]);
    Serial.println();
  }

  // if (analog_sensor_read_value)

  //   if (analog_sensor_read_value >)
  //     EEPROM.write(0, val);
}

void read_analog_value_from_EEPROM()
{
  analog_sensor_read_value = start_index_add;
  uint8_t start_index = 0;
  for (uint8_t i = 0; i < 4; i++)
  {
    analog_sensor_read_value = analog_sensor_read_value + EEPROM.read(start_index + i);

    Serial.print(start_index + i);
    Serial.print("\t");
    Serial.print(analog_sensor_read_value);
    Serial.println();
  }

  water_falling_time_ms = map(analog_sensor_read_value, 0, 1023, 2000, 8000);
  analogWrite(analog_read_pot_LED_pin, analog_sensor_read_value / 4);

  Serial.print("EEPROM analog_sensor_read_value = ");
  Serial.println(analog_sensor_read_value);
}

void setup_ultrasonic_sensors()
{
  Serial.println("Setup Ultrasonic Sensor HC-SR04 Test");
  for (uint8_t i = 0; i < nos_channel; i++)
  {
    Serial.print("setup_ultrasonic_sensor= ");
    Serial.print(i);
    Serial.print(" \t");
    Serial.print(trigPins[i]);
    Serial.print(" \t");
    Serial.println(echoPins[i]);

    pinMode(trigPins[i], OUTPUT); // Sets the trigPin as an OUTPUT
    pinMode(echoPins[i], INPUT);  // Sets the echoPin as an INPUT
  }
}

void setup_output_channels()
{
  Serial.println("setup_output_channels");
  for (uint8_t i = 0; i < nos_channel; i++)
  {
    Serial.print("setup_output_channels= ");
    Serial.println(i);
    pinMode(output_channels[i], OUTPUT); // Sets the trigPin as an OUTPUT

    digitalWrite(output_channels[i], false);
    output_channel_state[i] = false;
  }
}

void setup_push_buttons()
{
  Serial.println("setup_push_buttons");
}

void setup_analog_read()
{
  Serial.println("setup_analog_read");
}

void read_analog_value_from_sensor()
{
  // read atlest 10 values and average it out.
  long sum_analog_values = 0;

  for (uint8_t i = 0; i < analog_reading_nos_to_avg; i++)
  {
    sum_analog_values = sum_analog_values + analogRead(analog_read_pot_pin);
  }

  analog_sensor_read_value = (sum_analog_values) / analog_reading_nos_to_avg;

  water_falling_time_ms = map(analog_sensor_read_value, 0, 1023, 2000, 8000);

  analogWrite(analog_read_pot_LED_pin, analog_sensor_read_value / 4);

  Serial.print("AVG 10 read_analog_value_from_sensor = ");
  Serial.println(analog_sensor_read_value);
}

// void read_analog_store_value_from_EEPROM(){}
// void write_analog_store_value_to_EEPROM(){} // attach Interrupt from push button

Ticker tickerObject_output_0(callbackFunction_0, water_falling_time_ms, water_falling_repeats, MILLIS);
Ticker tickerObject_output_1(callbackFunction_1, water_falling_time_ms, water_falling_repeats, MILLIS);
Ticker tickerObject_output_2(callbackFunction_2, water_falling_time_ms, water_falling_repeats, MILLIS);
Ticker tickerObject_output_3(callbackFunction_3, water_falling_time_ms, water_falling_repeats, MILLIS);

void callbackFunction_0()
{
  toggle_ith_output_channel(0);
  stop_ith_ticker_timer(0);
}
void callbackFunction_1()
{
  toggle_ith_output_channel(1);
  stop_ith_ticker_timer(1);
}
void callbackFunction_2()
{
  toggle_ith_output_channel(2);
  stop_ith_ticker_timer(2);
}
void callbackFunction_3()
{
  toggle_ith_output_channel(3);
  stop_ith_ticker_timer(3);
}

// Ticker tickerObject_output_0;
// Ticker tickerObject_output_1;
// Ticker tickerObject_output_2;
// Ticker tickerObject_output_3;

void setup_ticker_timers()
{
  // Ticker::Ticker(fptr callback, uint32_t timer, uint16_t repeats, interval_t mode)
  // Ticker tickerObject_output_0(callbackFunction_0, water_falling_time_ms, water_falling_repeats, MILLIS);
  // Ticker tickerObject_output_1(callbackFunction_1, water_falling_time_ms, water_falling_repeats, MILLIS);
  // Ticker tickerObject_output_2(callbackFunction_2, water_falling_time_ms, water_falling_repeats, MILLIS);
  // Ticker tickerObject_output_3(callbackFunction_3, water_falling_time_ms, water_falling_repeats, MILLIS);
  // // tickerObject_output_0.start(); // start the ticker.
}

void setup()
{
  Serial.begin(serial_baud_rate);                   // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");

  // setup_lcd_diaplay(); // FUTURE req;
  setup_ticker_timers();
  setup_ultrasonic_sensors();
  setup_output_channels();
  setup_push_buttons();
  setup_analog_read();

  read_analog_value_from_sensor();
  read_analog_value_from_EEPROM();

  setupISR1();

  Serial.println("\n \n SETUP COMPLETED! \n \n");
}

void toggle_ith_output_channel(uint8_t channel_no)
{
  Serial.print("toggle_ith_output_channel = ");
  Serial.println(channel_no);

  output_channel_state[channel_no] = !(output_channel_state[channel_no]);
  digitalWrite(output_channels[channel_no], output_channel_state[channel_no]);

  Serial.print("output_channel_state[channel_no] = ");
  Serial.println(output_channel_state[channel_no]);
  Serial.println("");
}

void start_ith_ticker_timer(uint8_t channel_no)
{
  Serial.print("start_ith_ticker_timer i = ");
  Serial.println(channel_no);

  Serial.print("water_falling_time_ms i = ");
  Serial.println(water_falling_time_ms);

  switch (channel_no)
  {
  case 0:
    tickerObject_output_0.start();
    break;
  case 1:
    tickerObject_output_1.start();
    break;
  case 2:
    tickerObject_output_2.start();
    break;
  case 3:
    tickerObject_output_3.start();
    break;

  default:
    break;
  }
}

void stop_ith_ticker_timer(uint8_t channel_no)
{

  Serial.print("stop_ith_ticker_timer i = ");
  Serial.println(channel_no);

  unsigned long current_millis = millis();

  switch (channel_no)
  {
  case 0:
    tickerObject_output_0.stop();
    cool_off_duration_millis[0] = current_millis;
    break;
  case 1:
    tickerObject_output_1.stop();
    cool_off_duration_millis[1] = current_millis;
    break;
  case 2:
    tickerObject_output_2.stop();
    cool_off_duration_millis[2] = current_millis;
    break;
  case 3:
    tickerObject_output_3.stop();
    cool_off_duration_millis[3] = current_millis;
    break;

  default:
    break;
  }
}

void check_ith_channel_distace_and_take_action(uint8_t channel_no)
{
  // check distance and then switch on relay for given millisecons then turn off relay
  if (distances[channel_no] <= activation_min_dist && distances[channel_no] > activation_safe_zone_dist)
  {
    if (output_channel_state[channel_no] == false)
    {
      unsigned long currentMillis = millis();
      // if (((currentMillis  - (cool_off_duration_millis[channel_no] + dealy_in_action_ms + water_falling_time_ms) >= cool_off_duration_ms)) && (hand_put_state[channel_no] == false))
      if (((currentMillis - (cool_off_duration_millis[channel_no]) >= cool_off_duration_ms)) && (hand_put_state[channel_no] == false))
      {
        hand_put_state[channel_no] = true;
        // cool_off_duration_millis[channel_no] = currentMillis;

        delay(dealy_in_action_ms);
        toggle_ith_output_channel(channel_no);
        start_ith_ticker_timer(channel_no);

        Serial.print("Distance: ");
        Serial.print(distances[channel_no]);
        Serial.println(" cm");
      }
#ifdef DEBUG_CODE
      else
      {
        if ((currentMillis - (cool_off_duration_millis[channel_no]) < cool_off_duration_ms))
        {
          if (millis() - previousMillis_cool_off_print > previousMillis_cool_off_print_interval)
          {
            previousMillis_cool_off_print = millis();
            Serial.print("wait for cool off ");
            // Serial.print(cool_off_duration_ms - (millis() - (cool_off_duration_millis[channel_no] + dealy_in_action_ms + water_falling_time_ms)));
            Serial.print(cool_off_duration_ms - (millis() - (cool_off_duration_millis[channel_no])));
            Serial.println(" ms");
          }
        }
        else
        {
          if (millis() - previousMillis_cool_off_print > 600)
          {
            previousMillis_cool_off_print = millis();
            Serial.print(" Cool off ho gaya hai , BC haath hata ab! \t");
            Serial.print("hand_put_state[channel_no] = ");
            Serial.println(hand_put_state[channel_no]);
          }
        }
      }
#endif
    }
    // else
    // {
    //   if ((distances[channel_no] >= default_safe_zone_dist))
    //   {
    //     Serial.print("haath given paani bharne ke duration se pehele hata lia ,  hand_put_state[channel_no] = ");
    //     Serial.println(channel_no);
    //     toggle_ith_output_channel(channel_no);
    //     stop_ith_ticker_timer(channel_no);
    //     hand_put_state[channel_no] = false; // not required , can be tested and later deleted.
    //   }
    // }
  }
  else if ((distances[channel_no] >= default_safe_zone_dist) && (distances[channel_no] < default_min_dist) && (output_channel_state[channel_no] == true))
  {
    toggle_ith_output_channel(channel_no);
    stop_ith_ticker_timer(channel_no);
  }

  if (hand_put_state[channel_no] && (distances[channel_no] >= default_safe_zone_dist))
  {
    hand_put_state[channel_no] = false;

    if (output_channel_state[channel_no] == true)
    {
      Serial.print("haath given paani bharne ke duration se pehele hata lia ,  hand_put_state[channel_no] = ");
      Serial.println(channel_no);
      toggle_ith_output_channel(channel_no);
      stop_ith_ticker_timer(channel_no);
    }

    // hand_put_state[channel_no] = false; // not required , can be tested and later deleted.
  }
}

void check_all_channels_distace_and_take_action()
{
  for (uint8_t i = 0; i < nos_channel; i++)
  {
    check_ith_channel_distace_and_take_action(i);
  }
}

void read_ith_sensor_distance(uint8_t sensor_no)
{
  // Serial.print(trigPins[sensor_no]);
  // Serial.print(" ");
  // Serial.print(echoPins[sensor_no]);
  // Serial.print(" ");
  // Serial.print(durations[sensor_no]);
  // Serial.print(" ");
  // Serial.print(distances[sensor_no]);
  // Serial.println(" ");
  // Clears the trigPin condition
  digitalWrite(trigPins[sensor_no], LOW);
  delayMicroseconds(US_start_delay_ms);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPins[sensor_no], HIGH);
  delayMicroseconds(US_pulse_duration_ms);
  digitalWrite(trigPins[sensor_no], LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  durations[sensor_no] = pulseIn(echoPins[sensor_no], HIGH);
  // Calculating the distance
  distances[sensor_no] = (durations[sensor_no] * US_sound_speed) / 2.0; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  // Serial.print("Distance: ");
  // Serial.print(distances[sensor_no]);
  // Serial.println(" cm");
}

void read_all_sensor_data()
{
  for (uint8_t i = 0; i < nos_channel; i++)
  {
    read_ith_sensor_distance(i);
  }
}

void loop_ticker_timers()
{
  tickerObject_output_0.update(); // it will check the Ticker and if necessary, it will run the callback function.
  tickerObject_output_1.update();
  tickerObject_output_2.update();
  tickerObject_output_3.update();
}

void loop()
{
  // check flags is there any interrupt calls made

  myIRS_check();

  loop_ticker_timers();
  read_all_sensor_data();
  check_all_channels_distace_and_take_action();
}