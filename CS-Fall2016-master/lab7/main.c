/* main.c ---
*
* Filename: main.c
* Description:
* Author:
* Maintainer:
* Created: Thu Jan 10 11:23:43 2013
/* Code: */

#include <stm32f30x.h> // Pull in include files for F30x standard drivers
#include <f3d_uart.h>
#include <f3d_user_btn.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <stdio.h>
#include <f3d_led.h>
#include <math.h>

#define PI 3.14159265
#define TIMER 20000

void delay(int count){
	while (count-- > 0) {
		int i = 10000;
    while (i-- > 0) {
      asm("nop");
	}
}
}

int main(void) {
   setvbuf(stdin, NULL, _IONBF, 0);
   setvbuf(stdout, NULL, _IONBF, 0);
   setvbuf(stderr, NULL, _IONBF, 0);

  // Set up your inits before you go ahead
  f3d_led_init(); 
  f3d_lcd_init(); 
  f3d_user_btn_init(); 
  f3d_uart_init(); 
  delay(10);
  f3d_i2c1_init();
  delay(10);
  f3d_accel_init();
  delay(10);
  f3d_mag_init();
  delay(10);

  float data [3];
  int display;
  
  float pit;
  float rol;
  float magnetX;
  float magnetY;
  float magnetZ;
  float Heading;
  char str_tilt[12];
  f3d_lcd_fillScreen(RED);
  f3d_lcd_drawChar(10, 35, 'P', GREEN, RED); 
  f3d_lcd_drawChar(10, 70, 'R', GREEN, RED);
  
while(1){
	while (user_btn_read()){
	if(display)
	display = 0;
	else 
	display = 1;
	}
	if (display == 1){
	f3d_accel_read(data);
	pit = atan(data[0]/(sqrt(pow(data[1],2) + pow(data[2], 2))));
	rol = atan(data[1]/(sqrt(pow(data[0],2) + pow(data[2], 2))));
	f3d_mag_read(data);
	magnetX = data[0]*cos(pit)+data[2]*sin(pit);
	magnetY = data[0]*sin(rol)*sin(pit)+data[1]*cos(rol)-data[2]*sin(rol)*cos(pit);
	magnetZ = -data[0]*cos(rol)*sin(pit)+data[1]*sin(rol)+data[2]*cos(rol)*cos(pit);
	
	if (magnetX > 0 && magnetY >= 0){
		Heading = atan(magnetY/magnetX);
		}
	if (magnetX < 0){
        Heading = PI + atan(magnetY/magnetX);
        }
    if (magnetX > 0 && magnetY <= 0){
		Heading = 2*PI + atan(magnetY/magnetX);
        }
    if (magnetX == 0 && magnetY < 0){
		Heading = PI/2;
        }
    if (magnetX == 0 && magnetY > 0){
        Heading = PI*1.5;
        }
	print("%f\n", Heading);
	if (Heading < 1.15 && Heading > 0.3) {
    f3d_led_off(4);
    f3d_led_off(2);
    f3d_led_on(3);
        }
    if (Heading < 2.2 && Heading >= 1.15) {
    f3d_led_off(3);
    f3d_led_off(1);
    f3d_led_on(2);
        }
    if (Heading < 3.05 && Heading >= 2.2) {
    f3d_led_off(2);
    f3d_led_off(0);
    f3d_led_on(1);
        }
    if (Heading < 3.7 && Heading >= 3.05) {
    f3d_led_off(7);
    f3d_led_off(1);
    f3d_led_on(0);
        }
    if (Heading < 4.4 && Heading >= 3.7) {
    f3d_led_off(0);
    f3d_led_off(6);
    f3d_led_on(7);
        }
    if (Heading < 5.2 && Heading >= 4.4) {
    f3d_led_off(7);
    f3d_led_off(5);
    f3d_led_on(6);
        }
    if (Heading < 5.8 && Heading >= 5.2){
    f3d_led_off(6);
    f3d_led_off(4);
    f3d_led_on(5);
        }
    if (Heading < 0.3 || Heading >= 5.8) {
    f3d_led_off(5);
    f3d_led_off(3);
    f3d_led_on(4);
        }  
}
	else{
	f3d_accel_read(data);
    pit = atan(data[0]/(sqrt(pow(data[1],2) + pow(data[2], 2)))); 
	rol = atan(data[1]/(sqrt(pow(data[0],2) + pow(data[2], 2))));
	
	if (pit < 0) pit *= -1; 
	if (rol < 0) rol *= -1;
	sprintf(str_tilt, "%.3f", pit);
    f3d_lcd_drawString(30, 40, "     ", RED, RED);
    f3d_lcd_drawString(30, 40, str_tilt, GREEN, RED);
	sprintf(str_tilt, "%.3f", rol);
    f3d_lcd_drawString(30, 80, "     ", RED, RED);
    f3d_lcd_drawString(30, 80, str_tilt, GREEN, RED);
	
	pit = pit * 100;
	rol = rol * 100;
	f3d_lcd_fillBar(45, (int)pit, BLUE);
	f3d_lcd_fillBar(85, (int)rol, YELLOW);
	}
	}
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
