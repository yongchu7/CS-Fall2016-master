/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: 
 * Maintainer: 
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 
 *           By: 
 *     Update #: 0
 * Keywords: 
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

#include <f3d_uart.h>
#include <stdio.h>
#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers

// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main(void) {
  f3d_uart_init();

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  
  while (1){
    //Variables for counting lines, words, and characters
    int lcount = 0;
    int wcount = 0;
    int ccount = 0;

    //Variable for storing each read character
    int c;

    //Looping through and counting lines, words, and characters
    while((c = getchar()) != 0x1b) {
      ccount++;

      if(c == '\n') {
	lcount++;
	wcount++;
      }

      if(c == ' ' || c == '\t' || c == '\r' || c == '\f' || c == '\v') {
	wcount++;
      }
    }
    
  //Printing the number of lines, words, and characters
  printf("%d %d %d", lcount, wcount, ccount);
 }
 //while(1) {
 // putchar('a');
 //test for putchar
 
 //putchar(getchar());
 //test for getchar
 
 //putstring('hello world!');
 //test for putstring 
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
