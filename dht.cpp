// Name: Justin Choi 
// FILE: dht.cpp
// PURPOSE: DHT Temperature & Humidity Sensor library for Arduino

#include "dht.h"

float MAX_TEMP = 32.0f;
#define DIRA 3
#define DIRB 4
#define ENABLE 5 

bool dht::measure_environment() {
    static unsigned long measurement_timestamp = millis();
    if( millis() - measurement_timestamp > 3000ul ) { // Measures every 4 seconds
        fan_control();
        if( measure() == true ) {
          measurement_timestamp = millis();
          return true;
        }
    }
    return false;
}

void dht::fan_sim() {
   float temp = temperature;
   temperature = MAX_TEMP;
   fan_control_sim(temp);
   
}
bool dht::measure() {
    if( read() ) {
        temperature = data[2];
        humidity =  data[0];
        return true;
    } else {
        return false;
    }
}

bool dht:: read() {
    bool status = false;
    data[ 0 ] = data[ 1 ] = data[ 2 ] = data[ 3 ] = data[ 4 ] = 0; //Clear data buffer
    status = read_data();

    return status;
}

bool dht:: read_data() {
    uint8_t cnt = 7; // Bit index for each received byte
    uint8_t idx = 0; // Index for data array

    // Set DHT pin to output and low to request data
    DDRE |= (1 << DDE4); // Set pin 2 to output
    PORTE &= ~( 1 << PE4); // Set pin 2 to low
    delay(20);
  
    PORTE |= ( 1 << PE4); // Set pin 2 to high
    delayMicroseconds(40);
    DDRE &= ~(1 << DDE4); // Set pin 2 to input

    while(expect_pulse(LOW));
    while(expect_pulse(HIGH));

    for (int i=0; i<40; i++)
    {
        while(expect_pulse(LOW));
        unsigned long t = micros();
        while(expect_pulse(HIGH));

        if ((micros() - t) > 40) data[idx] |= (1 << cnt);

        if (cnt == 0) {
            cnt = 7;   
            idx++;      
        } else cnt--;
    }
    // Check we read 40 bits and that the checksum matches.
    if( data[4] == (( data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
        return true;
    } else {
        return false;
    }
}

uint32_t dht::expect_pulse(bool level) {
    uint32_t count = 0;
    uint8_t portState = level ? digitalPinToBitMask( DHT_SENSOR_PIN ) : 0;
    while ((*portInputRegister(digitalPinToPort( DHT_SENSOR_PIN )) & digitalPinToBitMask( DHT_SENSOR_PIN )) 
                == portState) {
        if (count++ >= 1000) {
            return 0; // Timeout
        }
    }
    return count;
}

void dht::fan_control_sim(float temp) {
  if (temperature >= MAX_TEMP) {
     digitalWrite(ENABLE,HIGH); //enable on
     digitalWrite(DIRA,HIGH); //one way
     digitalWrite(DIRB,LOW);
     delay(3000);
     digitalWrite(ENABLE,LOW); //slow stop
     delay(1000);
     temperature = temp; 
   };
}

bool dht::fan_control() {
  if (temperature >= MAX_TEMP) {
     digitalWrite(ENABLE,HIGH); //enable on
     digitalWrite(DIRA,HIGH); //one way
     digitalWrite(DIRB,LOW);
     return true;
   } else {
    digitalWrite(ENABLE,LOW); //slow stop
    delay(1000);
    return false;
   };
}
