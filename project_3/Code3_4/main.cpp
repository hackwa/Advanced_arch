#include "mbed.h"
// Calculates and Prints FFT on serial port
float M_PI = 3.14159;
#define TWOPI	(2.0*PI)
#define PI	M_PI
Serial pc(USBTX,USBRX);

void four1(float data[], int nn, int isign)
{
    int n, mmax, m, j, istep, i;
    double wtemp, wr, wpr, wpi, wi, theta;
    double tempr, tempi;
    
    n = nn << 1;
    j = 1;
    for (i = 1; i < n; i += 2) {
	if (j > i) {
	    tempr = data[j];     data[j] = data[i];     data[i] = tempr;
	    tempr = data[j+1]; data[j+1] = data[i+1]; data[i+1] = tempr;
	}
	m = n >> 1;
	while (m >= 2 && j > m) {
	    j -= m;
	    m >>= 1;
	}
	j += m;
    }
    mmax = 2;
    while (n > mmax) {
	istep = 2*mmax;
	theta = TWOPI/(isign*mmax);
	wtemp = sin(0.5*theta);
	wpr = -2.0*wtemp*wtemp;
	wpi = sin(theta);
	wr = 1.0;
	wi = 0.0;
	for (m = 1; m < mmax; m += 2) {
	    for (i = m; i <= n; i += istep) {
		j =i + mmax;
		tempr = wr*data[j]   - wi*data[j+1];
		tempi = wr*data[j+1] + wi*data[j];
		data[j]   = data[i]   - tempr;
		data[j+1] = data[i+1] - tempi;
		data[i] += tempr;
		data[i+1] += tempi;
	    }
	    wr = (wtemp = wr)*wpr - wi*wpi + wr;
	    wi = wi*wpr + wtemp*wpi + wi;
	}
	mmax = istep;
    }
}


int main(){
  FILE *f;
  f = fopen("DAC.txt","w");
  float op[64] = {0}, magni, theta, y, t, output;
  int i =1;
  for (t=0;t<=0.00992;t=t+0.000992){
      theta = t*10000*1.2568;
      y = sin(theta)+1;
      output = y/2;
      if (f == NULL){
          pc.printf("\nnulll!!");
      }
//      fprintf(f,"%f\n",output);
		  op[i]= output;
          op[i+1] = 0;
		  i = i+2;
      pc.printf("\n%f",output);
      }
    four1(op, 32,1);
	for (int j = 0 ; j<32; j+= 2){
    //    pc.printf("\n%f",op[j]);
     magni = sqrt(op[j] * op[j] + op[j+1] * op[j+1]);
	pc.printf("%f\n",magni);  // will print magnitude of frequency components present in signal 	
}
    }