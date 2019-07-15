/*
 * TODO: Implement checksum and error handling
 * 
 * This program sends a PWM signal to one of three motor controllers based on a 12-byte serial input. 
 * The protocol is documented in PROTOCOL.markdown.
 *
 *  Created: April 27, 2019
 *  Author:  Cody Nettesheim
 */
#include <Servo.h>

//Map channels to GPIO pins
#define CHANNEL_1 2
#define CHANNEL_2 3
#define CHANNEL_3 4
#define CHANNEL_4 5
#define CHANNEL_5 6
//PWM parameters
#define PULSE_NEUTRAL 1500
//Length of a message in bytes
#define MESSAGE_LEN 12
//Uncomment this line for debug mode:
#define DEBUG 1

//PWM objects for each motor
Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;
Servo motor5;
//Serial input data buffer
uint8_t serial_bytes[MESSAGE_LEN] = {0};
//Motor parameters set by serial message
int16_t drive_spd = 0; //Speed of motor: -1000 (full reverse) to +1000 (full forward)
uint8_t motor_id  = 0; //Motor ID: selects motors 1-5



void setup() {
    //Configure GPIO for each motor
    motor1.attach(CHANNEL_1);
    motor2.attach(CHANNEL_2);
    motor3.attach(CHANNEL_3);
    motor4.attach(CHANNEL_4);
    motor5.attach(CHANNEL_5);
    //Initialize Serial connection
    Serial.begin(115200);
    //Initially set motors to stopped
    motor1.writeMicroseconds(PULSE_NEUTRAL);
    motor2.writeMicroseconds(PULSE_NEUTRAL);
    motor3.writeMicroseconds(PULSE_NEUTRAL);
    motor4.writeMicroseconds(PULSE_NEUTRAL);
    motor5.writeMicroseconds(PULSE_NEUTRAL);
}

void loop() {
    //Wait for input
    if (Serial.available()) {
        //Get a single byte
        Serial.readBytes(serial_bytes, 12);
        //Least significant digit selects motor
        motor_id = serial_bytes[5];
        //Middle digit sets motor speed
        drive_spd = (serial_bytes[7] << 8) & serial_bytes[8];
        //Convert drive speed to PWM output value
        drive_spd /= 2;
        drive_spd += PULSE_NEUTRAL;
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
            case 4:
                motor4.writeMicroseconds(drive_spd);
            break;
            case 5:
                motor5.writeMicroseconds(drive_spd);
            break;
        };
        //Print debug output in debug mode
        #ifdef DEBUG
            Serial.print("Setting motor " + (String) motor_id + " to " + (String) drive_spd + "us.\n");
        #endif
    }
}
