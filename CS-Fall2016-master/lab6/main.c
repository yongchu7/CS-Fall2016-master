/* main.c ---
*
* Filename: main.c
* Description:
* Author: Vinayak Vedantam
* Partner: Yongtao Chu
* Maintainer:
* Created: Thu Jan 10 11:23:43 2013
/* Code: */

#include <stm32f30x.h> // Pull in include files for F30x standard drivers
#include <f3d_led.h> // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <stdio.h>

#define TIMER 20000

int main(void) {
  // If you have your inits set up, this should turn your LCD screen red
  f3d_lcd_init();
  f3d_uart_init();
  f3d_gyro_init();
  f3d_user_btn_init();
  f3d_led_init();

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  
  float f[3];
  int axis = 0;
  int c;
  int lastKey = 0;

  char* ls[] = {
    "\nSWITCHING TO X\n",
    "\nSWITCHING TO Y\n",
    "\nSWITCHING TO Z\n",
  };
  f3d_lcd_fillScreen2(RED);
  f3d_lcd_drawChar(0, 0, 'X', WHITE, RED);
  f3d_lcd_drawChar(0, ST7735_height/2 , 'Y', GREEN, RED);
  f3d_lcd_drawChar(0, ST7735_height-30, 'Z', BLUE, RED);
  
  while(1) { 
    // need to getChar user input as well for x, y, z
    f3d_led_all_off();    
    if (user_btn_read()) {
      printf(ls[axis = (axis + 1) % 3]);
      while (user_btn_read());
    }
    else if ((c = pollchar() - 'w') && c-- && lastKey != c) {
      lastKey = c;
      if (axis != c && 0 <= c && c < 3) {
	printf(ls[axis = c]);
      }
    }    

    f3d_gyro_getdata(f);
    char xarray[50];
    char yarray[50];
    char zarray[50];
    sprintf(xarray, "%f\n", f[0]);
    sprintf(yarray, "%f\n", f[1]);
    sprintf(zarray, "%f\n", f[2]);
    f3d_lcd_drawString(5, 0, xarray, WHITE, RED);
    f3d_lcd_drawString(5, ST7735_height/2, yarray, GREEN, RED);
    f3d_lcd_drawString(5, ST7735_height-30, zarray, BLUE, RED);
     
    int sign = f[axis] < 0 ? -1 : +1;
    int i = (f[axis] - (5 * sign)) / 123;;

    
    if (i) {
      f3d_led_on((i + 8) % 8);
    }
    while (i != 0) {
      i -= sign;
      f3d_led_on((i + 8) % 8);
    }
  delay();
 
  f3d_lcd_fillSection(115, f[0]/2, WHITE);
  f3d_lcd_fillSection(70, f[1]/2, GREEN);
  f3d_lcd_fillSection(10, f[2]/2, BLUE);
  }
  return 0;
}


#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
