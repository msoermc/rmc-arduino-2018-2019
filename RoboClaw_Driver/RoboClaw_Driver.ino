/*
 * This program sends a PWM signal to one of three motor controllers based on a single-byte serial input. 
 * The input consists of 3 decimal digits:
 * The smallest digit (1-3) selects which device to output to.
 * The middle digit (0-9) determines the speed of the motor.
 * The largest digit (0 or 1) determines the drive direction of the motor.
 *
 *  Created: April 27, 2019
 *  Author:  Cody Nettesheim
 */
#include <Servo.h>

//Map channels to GPIO pins
#define CHANNEL_1 2
#define CHANNEL_2 3
#define CHANNEL_3 4
//PWM parameters
#define PULSE_RANGE    250 //Motor input has a range of 1500-250us to 1500+250us
#define PULSE_NEUTRAL 1500 //Motor does not drive when the pulse width is 1500us
//Uncomment this line for debug mode:
//#define DEBUG 1

//PWM objects for each motor
Servo motor1;
Servo motor2;
Servo motor3;
//Serial input data buffer
uint8_t serial_byte  = 0;
//Motor parameters set by serial input
uint8_t drive_dir    = 0; //Direction of drive: 0 (forward) or 1 (backward)
uint16_t drive_spd   = 0; //Speed of motor: 0 (stopped) - 9 (full speed)
uint8_t motor_id     = 0; //Motor ID: selects motor 1, 2, or 3

void setup() {
    //Configure GPIO for each motor
    motor1.attach(CHANNEL_1);
    motor2.attach(CHANNEL_2);
    motor3.attach(CHANNEL_3);
    //Initialize Serial connection
    Serial.begin(9600);
    //Initially set motors to stopped
    motor1.writeMicroseconds(1500);
    motor2.writeMicroseconds(1500);
    motor3.writeMicroseconds(1500);
}

void loop() {
    //Wait for input
    if (Serial.available()) {
        //Get a single byte
        serial_byte = Serial.read();
        //Least significant digit selects motor
        motor_id = serial_byte % 10;
        serial_byte /= 10;
        //Middle digit sets motor speed
        drive_spd = serial_byte % 10;
        serial_byte /= 10;
        //Most significant digit sets motor direction
        drive_dir = serial_byte % 10;
        //Convert drive speed to PWM output value
        drive_spd *= PULSE_RANGE;
        drive_spd /= 9;
        if (drive_dir == 0) {
            drive_spd = PULSE_NEUTRAL + drive_spd;
        } else {
            drive_spd = PULSE_NEUTRAL - drive_spd;
        }
        //Send PWM signal to selected output
        switch (motor_id) {
            case 1:
                motor1.writeMicroseconds(drive_spd);
            break;
            case 2:
                motor2.writeMicroseconds(drive_spd);
            break;
            case 3:
                motor3.writeMicroseconds(drive_spd);
            break;
        };
        //Print debug output in debug mode
        #ifdef DEBUG
            Serial.print("Setting motor " + (String) motor_id + " to " + (String) drive_spd + "us.\n");
        #endif
    }
}
