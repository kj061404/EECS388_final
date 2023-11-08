#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "eecs388_lib.h"

// void steering(int gpio, int pos)
// {
//     // Task-3: 
//     // Your code goes here (Use Lab 05 for reference)
//     // Check the project document to understand the task
// }

// int read_from_pi(int devid)
// {
//     // Task-2: 
//     //after performing Task-2 at dnn.py code, modify this part to read angle values from Raspberry Pi
//     // You code goes here (Use Lab 09 for reference)

// }

void auto_brake(int devid)
{
    // Task-1: 
    // Your code here (Use Lab 02 - Lab 04 for reference)
    // You must use the directions given in the project document to recieve full credit
    gpio_mode(RED_LED, OUTPUT);
    gpio_mode(GREEN_LED, OUTPUT);
    ser_setup(devid);

    ser_printline(devid,"Setup completed.\n"); //ser_printline() can be used to print to the serial monitor

    /* 
        LIDAR distance data is 16 bits.
    */
    uint16_t dist = 0;

    while (1) { //infinite loop to continuosly read data from tfmini

        if ('Y' == ser_read(devid) && 'Y' == ser_read(devid)) {
        
            //read the next bytes
            uint8_t low_byte = ser_read(devid); // high byte of distance
            uint8_t high_byte = ser_read(devid); // low byte of distance

            dist = ((uint16_t)high_byte) << 8 | low_byte; // calculate the distance value from the high and low bytes
            //convert high byte and low byte into one 16 bit because that is what distance is measured in


            printf("Distance: %d cm\n", dist); // print distance

            //control LEDs based on distance
            if (dist > 200) {
                gpio_write(RED_LED, OFF); //tuen red LED on
                gpio_write(GREEN_LED, ON); //turn green LED off
            }
            if (dist > 100 && 200 > dist) {
                gpio_write(RED_LED, ON); //turn red LED on
                gpio_write(GREEN_LED, ON); //tuen green LED on
            }
            if (dist > 60 && 100 > dist) {
                gpio_write(RED_LED, ON); //turn red LED on
                gpio_write(GREEN_LED, OFF); //tuen green LED off            
            }
            if (dist < 60) {
                while (1) {
                    gpio_write(GREEN_LED, OFF);
                    gpio_write(RED_LED, ON);
                    delay(100);
                    gpio_write(RED_LED, OFF);
                    delay(100);
                    break;
                }                
            }


        }
    }
}

int main()
{
    // initialize UART channels
    ser_setup(0); // uart0
    ser_setup(1); // uart1
    int pi_to_hifive = 1; //The connection with Pi uses uart 1
    int lidar_to_hifive = 0; //the lidar uses uart 0
    
    printf("\nUsing UART %d for Pi -> HiFive", pi_to_hifive);
    printf("\nUsing UART %d for Lidar -> HiFive", lidar_to_hifive);
    
    //Initializing PINs
    gpio_mode(PIN_19, OUTPUT);
    gpio_mode(RED_LED, OUTPUT);
    gpio_mode(BLUE_LED, OUTPUT);
    gpio_mode(GREEN_LED, OUTPUT);

    printf("Setup completed.\n");
    printf("Begin the main loop.\n");

    while (1) {

        auto_brake(lidar_to_hifive); // measuring distance using lidar and braking
        // int angle = read_from_pi(pi_to_hifive); //getting turn direction from pi
        // printf("\nangle=%d", angle) 
        // int gpio = PIN_19; 
        // for (int i = 0; i < 10; i++){
        //     // Here, we set the angle to 180 if the prediction from the DNN is a positive angle
        //     // and 0 if the prediction is a negative angle.
        //     // This is so that it is easier to see the movement of the servo.
        //     // You are welcome to pass the angle values directly to the steering function.
        //     // If the servo function is written correctly, it should still work,
        //     // only the movements of the servo will be more subtle
        //     if(angle>0){
        //         steering(gpio, 180);
        //     }
        //     else {
        //         steering(gpio,0);
        //     }
        // }

    }
    return 0;
}
