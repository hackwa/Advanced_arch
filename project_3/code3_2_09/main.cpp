#include "mbed.h"

AnalogIn pot1(PA_0);
AnalogIn pot2(PA_1);
PwmOut speaker(PB_10);
Serial pc(USBTX, USBRX);
char p1[4],p2[4];

float readValue( char *arr){
    // As there is no actual potentiometer, we read 
    // through serial port.
	int i = 0;
	while(i < sizeof(arr))
	{
		if(pc.readable())
		{
			arr[i] = pc.getc();
			i++;
		}
	}
	float x = atof(arr);
	pc.printf("Value Entered is : %f\n",x);
	return x;
}

void genWave(float v1, float v2)
{
	float frequency, volume, temp, waitPeriod,timePeriod;
	frequency = 320 + 7680 * v1;
    timePeriod = 1 / frequency;
	pc.printf("Calculated Frequency : %f Hz\n",frequency);
	volume = v1;
	temp = (volume/0.05) * frequency;
	waitPeriod = 1000/temp;
	pc.printf("Wait Period per sample : %f milliseconds\n",waitPeriod);
    speaker = volume;
    speaker.period(timePeriod);
}

int main() {
//		AnalogOut speaker(PB_10);
    pc.printf("Enter Potentiometer Values...\n");
    float freq, volume;
    while(1)
    {
			pc.printf("First value :\n");
			freq = readValue(p1);
			pc.printf("Second value :\n");
			volume = readValue(p2);
			genWave(freq,volume);
    }
}
