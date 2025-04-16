#include "Arduino.h"

// this #ifndef stops this file from being included mored than once by the compiler.
#ifndef _BUMPSENSORS_H
#define _BUMPSENSORS_H

// We will use all 2 bump sensors (BL & BR) and so define a constant here, 
// rather than type '2' in lots of places.
#define NUM_SENSORS 2

// Pin definitions
// This time, we will use an array to store the pin definitions. 
// This is a bit like a list. This way, we can either loop through the list automatically, 
// or we can ask for a pin by indexing,.
const int sensor_pins[ NUM_SENSORS ] = { 4, 5 }; // 4 - BL, 5 - BR

// This is the pin used to turn on the infra-red LEDs.
#define EMIT_PIN   11

// Class to operate the linesensors.
class BumpSensors_c {
  
  public:
    // Store your readings into this array.
    // You can then access these readings elsewhere by using the syntax bump_sensors.readings[n];
    // Where n is a value [0:1]
    float readings[ NUM_SENSORS ];

    float minimum[ NUM_SENSORS ];
    float maximum[ NUM_SENSORS ];
    float scaling[ NUM_SENSORS ];
    bool status[ NUM_SENSORS ]; // stores the status of every IR sensor, true -> black and false -> not black

    float calibrated[ NUM_SENSORS ];

    // Constructor, must exist.
    BumpSensors_c() {
      // leave this empty
    }

    // Part of the Advanced Exercises for Labsheet 2 (Ignore for now)
    void initialiseForDigital() {      
      // Ensure that the IR LEDs are on for Bump sensing
      pinMode( EMIT_PIN, OUTPUT );
      digitalWrite( EMIT_PIN, LOW );
    } // End of initialiseForDigital()

    // Part of the Advanced Exercises for Labsheet 2 (Ignore for now)
    unsigned long readSensorDigital(int sensor_index) {
      initialiseForDigital();

      pinMode(sensor_pins[sensor_index], OUTPUT);
      digitalWrite(sensor_pins[sensor_index], HIGH); // starts charging capacitor

      delayMicroseconds(10); // waiting for capacitor to charge fully 

      pinMode(sensor_pins[sensor_index], INPUT); // starts to dischage capacitor

      unsigned long start_time = micros(); // noting down start time of discharge

      while (digitalRead(sensor_pins[sensor_index])) {
        if (micros() - start_time >= 6000){ // 6000us was experimentally found.
          digitalWrite(sensor_pins[sensor_index], LOW); // if the status didn't change for long time, change it manually
        }
      }

      unsigned long end_time = micros(); // note end time of discharge

      pinMode(EMIT_PIN, INPUT); // turn off emissions

      unsigned long time_diff = end_time - start_time; // measure time difference

      return time_diff;   
    } // End of readSensorsDigital()

    void calcCalibratedDigital(){
      readings[0] = readSensorDigital(0);
      readings[1] = readSensorDigital(1);

      for (int sensor = 0; sensor < NUM_SENSORS; sensor++){
        if (scaling[sensor] != 0) {  // Ensure scaling is not zero to prevent division by zero
          calibrated[sensor] = (readings[sensor] - minimum[sensor]) / scaling[sensor];
        } else {
          calibrated[sensor] = 0; // Assign a default value if scaling is zero
        }
      }
    }

}; // End of BumpSensors_c class defintion

#endif