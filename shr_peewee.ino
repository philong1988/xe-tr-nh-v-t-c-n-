/*
   Basic Arduino obstacle avoider.

   Goal in life...
      Moves forward looking for obstacles to avoid :-)

   Use to get started for a SHR (Start here robot) (http://letsmakerobots.com/)

   Written by Scott Beasley - 2015
   Free to use or modify. Enjoy.
*/

/*
   Uses the MotorShield library. It works with the Arduino Motor shield R3.

   Download from https://github.com/jscottb/MotorShield or clone the zip from
   https://github.com/jscottb/MotorShield/archive/master.zip
   Unzip and copy to your Arduino library folder or follow the instructions
   here: http://arduino.cc/en/guide/libraries

   Note: You may need to remove the '-master' from the archive file name to add
   the library
*/

// Using the Arduino Motor shield R3 (http://arduino.cc/en/Main/ArduinoMotorShieldR3)
#include <MotorShield.h>
#include <Servo.h>

// Defines for the distance reading function of the bot.
#define turn() (left_dist >= right_dist) ? go_left () : go_right ()
#define microsecondsToCentimeters(microseconds) (unsigned long)microseconds / 29.1 / 2.0
#define MIN_ACTION_DIST 5 // 5 cm
// Change these defines if you use differnt pins
#define ECHO_PIN 2  // Digital pin 2.
#define TRIGGER_PIN 5 // Digital pin 5

// Speed defines
#define MAXFORWARDSPEED 190 // Max speed we want moving forward
#define MAXBACKWARDSPEED 130 // Max reverse speed

// Various time delays used for driving and servo
#define TURNDELAY 450
#define BACKUPDELAY 300
#define SERVOMOVEDELAY 200
#define SERVOSEARCHDELAY 85

/*
   Globals area. Try to keep them to a minimum :-)
*/

// Create the motor and servo objects with use to interface with
MS_DCMotor motor_left (MOTOR_A); // Create Left motor object
MS_DCMotor motor_right (MOTOR_B); // Create Right motor object
Servo sensor_servo; // Create a servo object for our distance sensor

byte sweep_pos = 0; // Current position of the sensor servo
byte pos_index = 90;
unsigned long left_dist, right_dist; // Distance measured left and right

void setup ( )
{
   Serial.begin (9600); // Set Serial monitor at 9600 baud
   Serial.println ("My SHR bot is starting up!");

   // Make sure the motors are off at start
   halt ( );

   sensor_servo.attach (6); // Attach the servo to digital pin 6
   sensor_servo.write (90); // Set the servo to the middle (neutral) pos

   // Set modes for distance sensor pins
   pinMode (ECHO_PIN, INPUT);  // Set echo pin as input
   pinMode (TRIGGER_PIN, OUTPUT); // Set trigger pin as output

   // Delay to give user time to make adjustments.  Remove after done.
   delay (30000);
}

void loop ( )
{
   unsigned long dist_fwd;

   // Rotate the distance sensor as we drive along
   rotate_sensor ( );

   // Give the servo time to get to position and get setted
   delay (SERVOSEARCHDELAY);

   // Get a reading from the current sensor direction
   dist_fwd = ping ( );
   Serial.print ("Distance sensor reading: ");
   Serial.println (dist_fwd);

   // Go forward while nothing is in the distance sensors read area
   if (dist_fwd > MIN_ACTION_DIST || dist_fwd == 0)
   {
      go_forward ( );
   }
   else // There is something in the sensors read area
   {
      halt ( ); // Stop!
      go_backward ( ); // Back up a bit
      delay (BACKUPDELAY);
      halt ( ); // Stop!

      sensor_read ( ); // Read distance left and right

      turn ( ); // Turn toward the clearest path
      delay (TURNDELAY);
      halt ( );
   }
}

// Read the sensor after we find something in the way. This helps find a new
// path
void sensor_read ( )
{
   Serial.println ("Server at 40 deg...");
   sensor_servo.write (40);
   delay (SERVOMOVEDELAY);
   right_dist = ping ( ); //Look to the right

   Serial.println ("Servo at 140 deg...");
   sensor_servo.write (140);
   delay (SERVOMOVEDELAY);
   left_dist = ping ( ); // Look to the left

   // Set the servo back to the center pos
   Serial.println ("Servo at 90 deg...");
   sensor_servo.write (90);
}

// Rotate the sensor servo at 45deg increments
void rotate_sensor ( )
{
   if (sweep_pos <= 0) {
      pos_index = 45;
   }
   else if (sweep_pos >= 180) {
      pos_index = -45;
   }

   Serial.print ("pos_index = ");
   Serial.println (pos_index);
   sweep_pos += pos_index;
   Serial.print ("sweep_pos = ");
   Serial.println (sweep_pos);
   sensor_servo.write (sweep_pos);
}

// Read the HC-SR04 uSonic sensor
unsigned long ping ( )
{
   // Trigger the uSonic sensor (HC-SR04) to send out a ping
   digitalWrite (TRIGGER_PIN, LOW);
   delayMicroseconds (5);
   digitalWrite (TRIGGER_PIN, HIGH);
   delayMicroseconds (10);
   digitalWrite (TRIGGER_PIN, LOW);

   // Measure how long the ping took and convert to cm's
   return (microsecondsToCentimeters (pulseIn (ECHO_PIN, HIGH)));
}

void go_forward ( )
{
   Serial.println ("Going forward...");
   motor_left.setSpeed (MAXFORWARDSPEED);
   motor_right.setSpeed (MAXFORWARDSPEED);
   motor_left.run (FORWARD|RELEASE);
   motor_right.run (BACKWARD|RELEASE);
}

void go_backward ( )
{
   Serial.println ("Going backward...");
   motor_left.setSpeed (MAXBACKWARDSPEED);
   motor_right.setSpeed (MAXBACKWARDSPEED);
   motor_left.run (BACKWARD|RELEASE);
   motor_right.run (FORWARD|RELEASE);
}

void go_left ( )
{
   Serial.println ("Going left...");
   motor_left.setSpeed (MAXFORWARDSPEED);
   motor_right.setSpeed (MAXFORWARDSPEED);
   motor_left.run (BACKWARD|RELEASE);
   motor_right.run (BACKWARD|RELEASE);
}

void go_right ( )
{
   Serial.println ("Going right...");
   motor_left.setSpeed (MAXFORWARDSPEED);
   motor_right.setSpeed (MAXFORWARDSPEED);
   motor_left.run (FORWARD|RELEASE);
   motor_right.run (FORWARD|RELEASE);
}

void halt ( )
{
   Serial.println ("Halt!");
   motor_left.setSpeed (0);
   motor_right.setSpeed (0);
   motor_left.run (BRAKE);
   motor_right.run (BRAKE);
}
