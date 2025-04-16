#include "BumpSensors.h"
#include "Motors.h"

Motors_c motors;
BumpSensors_c bs;
int max_bump = 2;
bool calibrating_bs, calibrated, facing = true;
unsigned long prev_time;
float b0, b1, br0, br1, d, q, prox, proxr, e, pwm = 20;

// Moving average variables
const int numReadings = 10;     // Taking last 10 readings for moving average
float b0Readings[numReadings];  // Array to store last 10 readings of b0
float b1Readings[numReadings];  // Array to store last 10 readings of b1
int b0Index = 0;                // Current index for b0 readings
int b1Index = 0;                // Current index for b1 readings
float b0Sum = 0;                // Sum of the last 10 readings of b0
float b1Sum = 0;                // Sum of the last 10 readings of b1
float b0Average = 0;            // Moving average for b0
float b1Average = 0;            // Moving average for b1

void setup() {
  Serial.begin(9600);
  delay(1500);
  Serial.println("**RESET**");

  motors.initialise();

  for (int i = 0; i < max_bump; i++) {
    bs.maximum[i] = 0;
    bs.minimum[i] = 25000;
  }

  calibrating_bs = true;
  calibrated = false;
  prev_time = millis();

  // Initialize the moving average arrays
  for (int i = 0; i < numReadings; i++) {
    b0Readings[i] = 0;
    b1Readings[i] = 0;
  }
}

void loop() {
  calibrate_bump_sensors();

  if (millis() - prev_time > 5000) {
    calibrating_bs = false;
    // motors.setPWM(0.0, 0.0);
  }

  if (calibrated) {
    bs.calcCalibratedDigital(facing);
    br0 = bs.readings[0]/100;
    br1 = bs.readings[1]/100;
    // Note that calibrated values are scaled 100x in the library.
    float new_b0 = bs.calibrated[0];
    float new_b1 = bs.calibrated[1];

    // Update moving average for b0
    b0Sum -= b0Readings[b0Index];
    b0Readings[b0Index] = new_b0;
    b0Sum += b0Readings[b0Index];
    b0Index = (b0Index + 1) % numReadings;
    b0Average = b0Sum / numReadings;

    // Update moving average for b1
    b1Sum -= b1Readings[b1Index];
    b1Readings[b1Index] = new_b1;
    b1Sum += b1Readings[b1Index];
    b1Index = (b1Index + 1) % numReadings;
    b1Average = b1Sum / numReadings;

    // Ensure minimum values for b0Average and b1Average
    if (b0Average < 1) { // ensuring no division by 0
      b0Average = 1;
    }
    if (b1Average < 1) {
      b1Average = 1;
    }

    // Update the global b0 and b1 variables with the moving averages
    b0 = b0Average;
    b1 = b1Average;

    leader_direction();

    Serial.print(br0);
    Serial.print(",");
    Serial.print(br1);
    Serial.print(",");
    Serial.println(proxr);
    // Serial.print(",");
    // Serial.print(e);
    // Serial.print(",");
    // Serial.println(q);
  }
}

void calibrate_bump_sensors() {
  if (calibrating_bs) {
    motors.setPWM(-pwm, pwm);
    bs.readings[0] = bs.readSensorDigital(0, facing);
    bs.readings[1] = bs.readSensorDigital(1, facing);

    for (int i = 0; i < max_bump; i++) {
      if (bs.readings[i] > bs.maximum[i]) {
        bs.maximum[i] = bs.readings[i];
      }
      if (bs.readings[i] < bs.minimum[i]) {
        bs.minimum[i] = bs.readings[i];
      }
    }

    Serial.print(bs.readings[0]);
    Serial.print(",");
    Serial.println(bs.readings[1]);
    delayMicroseconds(1);
  } 
  else if (!calibrating_bs && !calibrated) {
    for (int i = 0; i < max_bump; i++) {
      bs.scaling[i] = bs.maximum[i] - bs.minimum[i];
    }
    calibrated = true;
    motors.setPWM(0, 0);
  }
}

void leader_direction() {
  // 0 represents close | 1 and above represents open
  d = b0 - b1; // left - right sensor values
  q = (b0 / b1);

  // exponential function to classify left and right
  // Constants such as 1.25 and 100 were experimentally found from data
  e = pow(1.25, d)*100; 
  prox = sqrt(b0 * b0 + b1 * b1); // any further conditions will be found experimentally
  proxr = sqrt((br0*br0) + (br1*br1));

  // if (prox = 5) { // leader is quite far away from follower
  //   pwm = 17;
  // } 
  // else if (prox > 5){
  //   pwm += 1;
  //   if (pwm > 30){
  //     pwm = 30;
  //   }
  // }
  // else { // leader is very close to the follower
  //   motors.setPWM(0, 0); // move reverse
  //   // Serial.print("S: ");
  //   // Serial.print(",");
  //   // Serial.println(q);
  // }

  // if (q < 0.85) { 
  //     motors.setPWM(-pwm, pwm); // Turn left
  //     // Serial.print("L: ");
  //     // Serial.print(",");
  //     // Serial.println(q);
  // } 
  // else if (q > 1.15) {
  //   motors.setPWM(pwm, -pwm); // Turn right
  //   // Serial.print("R: ");
  //   // Serial.print(",");
  //   // Serial.println(q);
  // } 
  // else {
  //   motors.setPWM(pwm, pwm); // move forwards
  //   // Serial.print("F: ");
  //   // Serial.print(",");
  //   // Serial.println(q);
  // }
}
