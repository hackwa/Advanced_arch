#include "mbed.h"

BusOut myled(LED1);
InterruptIn buttin(USER_BUTTON);

void button_ISR(){
    buttin.disable_irq();
    myled = !myled;
    buttin.enable_irq();
    }
 
int main() {
    buttin.fall(&button_ISR);
    while(1)
    {
       sleep();
    }
}
