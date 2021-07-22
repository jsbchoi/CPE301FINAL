// Name: Justin Choi 
// FILE: dht.h
// PURPOSE: DHT Temperature & Humidity Sensor library for Arduino

#ifndef dht_h
#define dht_h

#include "Arduino.h"
#define DHT_SENSOR_PIN 2

class dht
{
public:
	bool measure_environment();
  bool measure();
	bool read();
	bool read_data();
  void fan_sim();
  bool fan_control();
  void fan_control_sim(float temp);
	double humidity;
	double temperature;

private:
	uint8_t data[5];  // buffer to receive data
	uint32_t expect_pulse(bool level);
};
#endif
