/************************************
,        .       .           .      , 
|        |       |           |     '| 
|    ,-: |-. ,-. |-. ,-. ,-. |-     | 
|    | | | | `-. | | |-' |-' |      | 
`--' `-` `-' `-' ' ' `-' `-' `-'    ' 
*************************************/

// this #ifndef stops this file
// from being included mored than
// once by the compiler.
#ifndef _MOTORS_H
#define _MOTORS_H

// Pin definitions.  By using #define we can
// switch the number here, and everywhere the
// text appears (i.e. L_PWM) it will be
// replaced.
#define L_PWM 10 // This is correct.
#define L_DIR 16 // This is the wrong pin! 
#define R_PWM 9 // This is the wrong pin! 
#define R_DIR 15 // This is the wrong pin! x

// It is a good idea to limit the maximum power
// sent to the motors. Using #define means we
// can set this value just once here, and it
// can be used in many places in the code below.
#define MAX_PWM 180.0 // This is also to ensure the robot doesn't move too fast
#define FWD LOW
#define REV HIGH

// Class to operate the motors.
class Motors_c {

  public:

    // Constructor, must exist.
    Motors_c() {
      // Leave empty. Ensure initialise() is called
      // instead.
    }

    // Use this function to initialise the pins that
    // will control the motors, and decide what first
    // value they should have.
    void initialise() {

      // Uncomment and replace the ???? with the correct
      // values, e.g  pinMode( <pin>, <INPUT or OUTPUT> );
       pinMode( L_PWM , OUTPUT );
       pinMode( L_DIR , OUTPUT );
       pinMode( R_PWM , OUTPUT );
       pinMode( R_DIR , OUTPUT );

      // Uncomment and replace ???? with the correct
      // values, e.g. digitalWrite( <pin>, <HIGH or LOW> );
      // Which pins will be either HIGH or LOW? What
      // purpose do they serve?
      digitalWrite( L_DIR, FWD ); // Robot should move forward
      digitalWrite( R_DIR, FWD );

      // Uncomment and replace ???? with the correct
      // values, e.g. analogWrite( <pin>, < 0 : 255 > );
      // Which pins will take a value in range [0:255]?
      // What purpose do they serve?
      analogWrite( L_PWM , 0 ); // Robot should be at rest
      analogWrite( R_PWM , 0 );

    } // End of initialise()


    // This function will be used to send a power value
    // to the motors.
    //
    // The power sent to the motors is created by the
    // analogWrite() function, which is producing PWM.
    // analogWrite() is intended to use a range between
    // [0:255].
    //
    // This function takes two input arguments: "left_pwr"
    // and "right_pwr", (pwr = power) and they are of the
    // type float. A float might be a value like 0.01, or
    // -150.6
    void setPWM( float left_pwr, float right_pwr ) {

      // What should happen if the request for left_pwr
      // is less than 0? Recall, how are these motors
      // operated in terms of the pins used?
//      Serial.print("Left PWM: ");
//      Serial.print(left_pwr);
//      Serial.print(" | Right PWM: ");
//      Serial.println(right_pwr);
      if ( left_pwr < 0 ) {
         digitalWrite( L_DIR, REV );
      } else {
         digitalWrite( L_DIR, FWD );
      }

      // What should happen if the request for right_pwr
      // is less than 0? Recall, how are these motors
      // operated in terms of the pins used?
      if ( right_pwr < 0 ) {
         digitalWrite( R_DIR, REV );
      } else {
         digitalWrite( R_DIR, FWD );
      }


      // analogWrite() requires a value in the range
      // [0:255], and note this is positive only!
      // Write some code here to take the value of
      // left_pwr and ensure it is:
      // - positive only
      // - within the range 0 to 255
      // Note, we have used the sign to determine
      // the direction - so we don't care about the
      // sign of the value any more.
      

      // analogWrite() requires a value in the range
      // [0:255], and note this is positive only!
      // Write some code here to take the value of
      // right_pwr and ensure it is:
      // - positive only
      // - within the range 0 to 255
      // Note, we have used the sign to determine
      // the direction - so we don't care about the
      // sign of the value any more.
      

      // Lastly, write the requested power value to
      // the motors as a PWM signal.
      // Without the code above, this is likely to
      // produce very unexpected behaviours.
      if (abs(left_pwr) > MAX_PWM){
        left_pwr = 255;
      }

      if (abs(right_pwr) > MAX_PWM){
        left_pwr = 255;
      }
      
      analogWrite( L_PWM, abs(left_pwr) );
      analogWrite( R_PWM, abs(right_pwr) );

      // Done!
      return;

    } // End of setPWM()


}; // End of Motors_c class definition.



#endif
