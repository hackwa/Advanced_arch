#include "mbed.h"

BusOut myled(LED1);
BusIn button_press(USER_BUTTON);
 
int main() {
    myled = 1;
    while(1){
    if(button_press)
    {
    myled = 0;
    wait(0.5);
    }
   myled = 1;
    }
}
