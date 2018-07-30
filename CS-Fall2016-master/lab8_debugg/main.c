/* main.c --- 
 * 
 * Filename: main.c
 * Description: For the STM board, this file runs basic xor encryption.
 * Author: Yongtao Chu
 * Maintainer:  
 * Created: 
 * Last-Updated: 
 *           By: 
 *     Update #: 1
 * Keywords: GDB, xor
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */
/* Code: */

#include <stm32f30x.h>  
#include <f3d_uart.h>  
#include <string.h>  
#include <stdio.h>    
#include <f3d_led.h>
#include <f3d_gyro.h> 
#include <f3d_lcd_sd.h> 
#include <f3d_delay.h>
#include <stdlib.h> 
#include <math.h>
#include <f3d_i2c.h>
#include <f3d_nunchuk.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#define TIMER = 20000

void blockGen(int mag, int y){
  int start;
  if (mag >= 0){
    for (start = 5; start <= mag; start++){
	f3d_lcd_drawPixel(start, y+1, BLUE);
	f3d_lcd_drawPixel(start, y+2, BLUE);
    }	
    for (start = mag; start <= 123; start++){
	f3d_lcd_drawPixel(start, y+1, WHITE);
	f3d_lcd_drawPixel(start, y+2, WHITE);
    }
  }

  if (mag <= -0){
    mag = mag * -1;
    for (start = 5; start <= mag; start++){
	f3d_lcd_drawPixel(start, y+1, YELLOW);
	f3d_lcd_drawPixel(start, y+2, YELLOW);
	} 
    for (start = mag; start<=123; start++)
      {
	f3d_lcd_drawPixel(start, y+1, WHITE);
	f3d_lcd_drawPixel(start, y+2, WHITE);
      }
  }
}
//set print label function so I don't do it every time 
void printLabel(char * label)
{
  f3d_lcd_drawString(20,150,label,BLACK,WHITE);
}


int main(void) { 
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  
  //init uart
  f3d_led_init();
  delay(10);
  f3d_uart_init();
  delay(10);
  f3d_lcd_init();
  delay(10);
  f3d_accel_init();
  delay(10);
  f3d_mag_init();
  delay(10);
  f3d_gyro_init();
  delay(10);
  f3d_i2c1_init();
  delay(10);
  f3d_nunchuk_init();
  delay(10);

  //reset button
  printf("Reset\n");

  int width = ST7735_width;
  char data[8];
  int i, j;
  int dataX;
  int dataY;
  int dataZ;
  float Cdata[3];
  float Cdata2[3];
  float A, B, X, Y, heading;
  char direction[10];
  char empty[8];
  float pitch, roll;
  char * labels[4] = {"Gyro Display", "Compass Display", "Tilt Display", "Nunchuk Display"};
  float axis[3];
  nunchuk_t Ndata;
  int state = 0;  
  f3d_lcd_fillScreen2(WHITE);
  printLabel(labels[state]);
  
  while(1)
    {
      f3d_nunchuk_read(&Ndata);
      if(Ndata.z || (Ndata.jx == 255  && state != 3))
	{
	  state++;
	  if(state>3) state=0;
	  
	  while(Ndata.z || (Ndata.jx == 255 && state != 3))
	    {
	      f3d_nunchuk_read(&Ndata);
	    }
	  f3d_lcd_fillScreen2(WHITE);
	  printLabel(labels[state]);
	  f3d_led_all_off();
	}

      if(Ndata.c || (Ndata.jx == 0 && state != 3))
	{
	  state--;
	  if(state<0) state=3;
	  
  	  while(Ndata.c || (Ndata.jx == 0 && state!= 3))
	    {
	      f3d_nunchuk_read(&Ndata);
	    }
	  f3d_lcd_fillScreen2(WHITE);
	  printLabel(labels[state]);
	  f3d_led_all_off();
	}

      switch(state)
      	{
			//GYRO
      	case 0:

	   f3d_lcd_drawString(6,0,"X: ",RED,WHITE);
	   f3d_lcd_drawString(6,10,"Y: ",GREEN,WHITE);
	   f3d_lcd_drawString(6,2*10,"Z: ",BLUE,WHITE);      
	   
	   f3d_lcd_drawString(width/2, 45, "X-Axis", RED, WHITE);
	   f3d_lcd_drawString(width/2, 65, "Y-Axis", GREEN, WHITE);
	   f3d_lcd_drawString(width/2, 85, "Z-Axis", BLUE, WHITE);
	      
	   f3d_lcd_drawString(6, 110, "Positive", BLUE, WHITE);
       f3d_lcd_drawString(6, 130, "Negative", YELLOW, WHITE);
	    

	  f3d_gyro_getdata(axis);
	  
	  sprintf(data, "%f", axis[0]);
	  f3d_lcd_drawString(4*6,0,data,RED,WHITE);
	  
	  sprintf(data, "%f", axis[1]);
	  f3d_lcd_drawString(4*6,10,data,GREEN,WHITE);
	  
	  sprintf(data, "%f", axis[2]);
	  f3d_lcd_drawString(4*6,2*10,data,BLUE,WHITE);

	  dataX = (int)axis[0] / 3.0;
	  blockGen(dataX, 55);	  
	  dataY = (int)axis[1] / 3.0;
	  blockGen(dataY, 75);	  
	  dataZ = (int)axis[2] / 3.0;
	  blockGen(dataZ, 95);
	  
	  break;
      	case 1:
	  //Compass
	  f3d_accel_read(Cdata);
	  f3d_mag_read(Cdata2);
	  
	  A = atan2f(Cdata[0], sqrt((powf(Cdata[1], 2.0)) + (powf(Cdata[2], 2.0))));
	  B = atan2f(Cdata[1],  sqrt((powf(Cdata[1], 2.0)) + (powf(Cdata[2], 2.0))));
	  
	  X = Cdata2[0]*cos(A) + Cdata2[2]*sin(A);
	  Y = Cdata2[0]*sin(A)*sin(B) + Cdata2[1]*cos(B) - Cdata2[2]*sin(B)*cos(A);
	  
	  heading = 183 + atan2f(Y, X)*180/M_PI;
	  
	  f3d_led_all_off();
	  f3d_led_on((9 - (int)heading / 45) % 8);

	  switch((int)heading / 45)
	    {
	    case 0: 
	      f3d_lcd_drawString(50, 50, "North    ", BLACK, WHITE);
	      break;
	    case 1: 
	      f3d_lcd_drawString(50, 50, "Northeast", BLACK, WHITE);
	      break;
	    case 2: 
	      f3d_lcd_drawString(50, 50, "East     ", BLACK, WHITE);
	      break;
	    case 3: 
	      f3d_lcd_drawString(50, 50, "Southeast", BLACK, WHITE);
	      break;
	    case 4: 
	      f3d_lcd_drawString(50, 50, "South    ", BLACK, WHITE);
	      break;
	    case 5: 
	      f3d_lcd_drawString(50, 50, "Southwest", BLACK, WHITE);
	      break;
	    case 6: 
	      f3d_lcd_drawString(50, 50, "West     ", BLACK, WHITE);
	      break;
	    case 7: 
	      f3d_lcd_drawString(50, 50, "Northwest", BLACK, WHITE);
	      break;
	    }
	  
	  break;
      	case 2:
	  //Tilt
	  f3d_accel_read(Cdata);
	  f3d_mag_read(Cdata2);
	  A = atan2f(Cdata[0], sqrt((Cdata[1]*Cdata[1]) + (Cdata[2]*Cdata[2])));
	  B = atan2f(Cdata[1],  sqrt((Cdata[0]*Cdata[0]) + (Cdata[2]*Cdata[2])));

	  pitch = atan2f(Cdata[0],(sqrtf (powf(Cdata[1], 2.0)) + (powf(Cdata[2], 2.0))));
	  roll = atan2f(Cdata[1],(sqrtf (powf(Cdata[0], 2.0)) + (powf(Cdata[2], 2.0))));
	  sprintf(empty,"%f",pitch);
	  f3d_lcd_drawString(30, 50,empty ,BLACK, WHITE);
	  sprintf(empty,"%f",roll);
	  f3d_lcd_drawString(30, 70,empty, BLACK, WHITE);
	  break;
      	case 3:
	  //Nunchuk	  
	  f3d_lcd_drawPixel(70 + ((Ndata.jx - 127) / 5), 100 + (-1 * ((Ndata.jy - 127) / 5)), GREEN);
	  f3d_lcd_drawPixel(69 + ((Ndata.jx - 127) / 5), 100 + (-1 * ((Ndata.jy - 127) / 5)), GREEN);
	  f3d_lcd_drawPixel(71 + ((Ndata.jx - 127) / 5), 100 + (-1 * ((Ndata.jy - 127) / 5)), GREEN);
	  f3d_lcd_drawPixel(70 + ((Ndata.jx - 127) / 5), 99 + (-1 * ((Ndata.jy - 127) / 5)), GREEN);
	  f3d_lcd_drawPixel(70 + ((Ndata.jx - 127) / 5), 101 + (-1 * ((Ndata.jy - 127) / 5)), GREEN);

	  pitch = atan2f(Ndata.ax-512,(sqrtf (powf(Ndata.ay-512, 2.0)) + (powf(Ndata.az-512, 2.0))));
	  roll = atan2f(Ndata.ax-512,(sqrtf (powf(Ndata.ay-512, 2.0)) + (powf(Ndata.az-512, 2.0))));

	  sprintf(empty,"%d  ",Ndata.jx);
	  f3d_lcd_drawString(25, 10, empty, BLACK, WHITE);
	  f3d_lcd_drawString(10, 10, "X:", BLACK, WHITE);
	  sprintf(empty,"%d  ",Ndata.jy);
	  f3d_lcd_drawString(25, 20, empty, BLACK, WHITE);
	  f3d_lcd_drawString(10, 20, "Y:", BLACK, WHITE);

	  f3d_lcd_drawString(10, 30, "Pitch:", BLACK, WHITE);
	  sprintf(empty,"%f",pitch*10.0);
	  f3d_lcd_drawString(45, 30, empty, BLACK, WHITE);
	  f3d_lcd_drawString(10, 40, "Roll:", BLACK, WHITE); 
	  sprintf(empty,"%f",roll*10.0);
	  f3d_lcd_drawString(45, 40, empty, BLACK, WHITE);
	  
	  break;
      	}

    }

  return(0);


}



#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're herey */
  while (1);
}
#endif

/* main.c ends here */
