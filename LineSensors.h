#include "Arduino.h"
/***************************************
 ,        .       .           .     ,-.  
 |        |       |           |        ) 
 |    ,-: |-. ,-. |-. ,-. ,-. |-      /  
 |    | | | | `-. | | |-' |-' |      /   
 `--' `-` `-' `-' ' ' `-' `-' `-'   '--' 
****************************************/

// this #ifndef stops this file from being included mored than once by the compiler.
#ifndef _LINESENSORS_H
#define _LINESENSORS_H

// We will use all 5 line sensors (DN1 - 5) and so define a constant here, 
// rather than type '5' in lots of places.
#define NUM_SENSORS 5

// Pin definitions
// This time, we will use an array to store the pin definitions. 
// This is a bit like a list. This way, we can either loop through the list automatically, 
// or we can ask for a pin by indexing, e.g. sensor_pins[0] is A11, sensors_pins[1] is A0.
const int sensor_pins[ NUM_SENSORS ] = { A11, A0, A2, A3, A4 };

// This is the pin used to turn on the infra-red LEDs.
#define EMIT_PIN   11

// Class to operate the linesensors.
class LineSensors_c {
  
  public:

    // Store your readings into this array.
    // You can then access these readings elsewhere by using the syntax line_sensors.readings[n];
    // Where n is a value [0:4]
    float readings[ NUM_SENSORS ];

    // Variables to store calibration constants. 
    // Make use of these as a part of the exercises in labsheet 2.
    float minimum[ NUM_SENSORS ];
    float maximum[ NUM_SENSORS ];
    float scaling[ NUM_SENSORS ];
    bool status[ NUM_SENSORS ]; // stores the status of every IR sensor, true -> black and false -> not black

    // Variable to store the calculated calibrated (corrected) readings. 
    // Needs to be updated via a function call, which is completed in labsheet 2.
    float calibrated[ NUM_SENSORS ];

    // Constructor, must exist.
    LineSensor_c() {
      // leave this empty
    }

    // Refer to Labsheet 2: Approach 1
    // Fix areas marked ????
    // Use this function to setup the pins required to perform an read of the line sensors using the ADC.
    void initialiseForADC() {

      // Ensure that the IR LEDs are on for line sensing
      pinMode( EMIT_PIN, OUTPUT );
      digitalWrite( EMIT_PIN, HIGH );

      // Configure the line sensor pins
      // DN1, DN2, DN3, DN4, DN5.
      for( int sensor = 0; sensor < NUM_SENSORS; sensor++ ) {
        pinMode( sensor_pins[sensor], INPUT_PULLUP );
      }
      
    } // End of initialiseForADC()

    // Refer to Labsheet 2: Approach 1
    // Fix areas marked ????
    // This function is as simple as using a call to 
    // analogRead()
    void readSensorsADC() {

      // First, initialise the pins.
      // You need to complete this function (above).
      initialiseForADC();

      for( int sensor = 0; sensor < NUM_SENSORS; sensor++ ) {
        readings[sensor] = analogRead(sensor_pins[sensor]);
      }

    } // End of readSensorsADC()    

    // Use this function to apply the calibration values that were captured in your calibration routine.
    // Therefore, you will need to write a calibration routine (see Labsheet 2)
    void calcCalibratedADC() {

      // Get latest readings (raw values)
      readSensorsADC();

      // Apply calibration values, store in calibrated[]
      for( int sensor = 0; sensor < NUM_SENSORS; sensor++ ) {
        calibrated[sensor] = (readings[sensor] - minimum[sensor]) / scaling[sensor];
      }
      
    } // End of calcCalibratedADC() 

    void sensorStatus(){
      // This function updates the status of every sensor.
      // If the sensor detects a black line, it is set to true.
      // Else it is set to false.
      for (int sensor = 0; sensor < NUM_SENSORS; sensor++){
        if (calibrated[sensor] > 0.8){
          status[sensor] = true;
        }
        else{
          status[sensor] = false;
        }
      }
    }

    // Part of the Advanced Exercises for Labsheet 2 (Ignore for now)
    void initialiseForDigital() {      
      // Ensure that the IR LEDs are on for line sensing
      pinMode( EMIT_PIN, OUTPUT );
      digitalWrite( EMIT_PIN, HIGH );
    } // End of initialiseForDigital()

    // Part of the Advanced Exercises for Labsheet 2 (Ignore for now)
    unsigned long readSensorsDigital(int sensor_index) {
      initialiseForDigital();

      pinMode(sensor_pins[sensor_index], OUTPUT);
      digitalWrite(sensor_pins[sensor_index], HIGH);

      delayMicroseconds(10);

      pinMode(sensor_pins[sensor_index], INPUT);

      unsigned long start_time = micros();

      while (digitalRead(sensor_pins[sensor_index])) {
        if (micros() - start_time >= 6000){ // 6000us was experimentally found.
          digitalWrite(sensor_pins[sensor_index], LOW);
        }
      }

      unsigned long end_time = micros();

      pinMode(EMIT_PIN, INPUT);

      unsigned long time_diff = end_time - start_time;        

      return time_diff;   
    } // End of readSensorsDigital()

}; // End of LineSensor_c class defintion

#endif