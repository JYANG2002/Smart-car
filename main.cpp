#include "mbed.h"
#include <cstdio>

// Maximum number of element the application buffer can contain
#define MAXIMUM_BUFFER_SIZE 32

// Create a BufferedSerial object with a default baud rate.
static BufferedSerial serial_port(USBTX, USBRX);

// main() runs in its own thread in the OS
int main()
{
    // Application buffer to receive the data
    char buf[MAXIMUM_BUFFER_SIZE] = {0};

    //Ref:https://vivonomicon.com/2018/04/22/bare-metal-stm32-programming-part-3-leds-and-buttons/
    // Set pins to PA2 & 3 as output for the wheels
    // Set the clock to port A
    RCC->AHB2ENR|=0x1;
    // Set pins 2 and 3 to output mode
    GPIOA->MODER &= ~(0xA0);
    GPIOA->MODER |= (0x50);

    while (true) {
        if (uint32_t num = serial_port.read(buf, sizeof(buf))) { // if there's a char written
            char input_char = buf[0];
            printf("%c\n", input_char);//used as test case to make sure there is data written
            if(input_char == 'w'){
                GPIOA->ODR |= (0xC); //1100
            }
            else if(input_char == 'd'){ //turning right so left engine is moving
                GPIOA->ODR |= (0x8);
                GPIOA->ODR &= ~(0x4);
            }
            else if (input_char == 'a'){ //turning left so right engine is moving
                GPIOA->ODR |= (0x4);
                GPIOA->ODR &= ~(0x8);
            }
            else {
                GPIOA->ODR &= ~(0xC); // completely stop if other key input
            }
         
        }
        
    }
}

