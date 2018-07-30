#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_user_btn.h>

// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main(void) {
  f3d_user_btn_init();
  f3d_led_init();
  f3d_led_all_on();
  while(1){
	  // for (i = 0; i < 8; i++){
		  // f3d_led_on(i);
	  // }
	  // while(user_btn_read());
	      // delay();
	  // f3d_led_all_on();
	  // delay();
	  // for (i = 0; i < 8; i++){
		  // f3d_led_off(i);
	  // }
	  // while(user_btn_read());
	      // delay();
	  // f3d_led_all_on();
	  // delay();
  
	f3d_led_on(0);
	while(user_btn_read());
	delay();
	f3d_led_on(1);
	while(user_btn_read());
	delay();
	f3d_led_on(2);
	while(user_btn_read());
	delay();
	f3d_led_on(3);
	while(user_btn_read());
	delay();
	f3d_led_on(4);
	while(user_btn_read());
	delay();
	f3d_led_on(5);
	while(user_btn_read());
	delay();
	f3d_led_on(6);
	while(user_btn_read());
	delay();
	f3d_led_on(7);
	while(user_btn_read());
	delay();
	f3d_led_all_on();
	delay();
	f3d_led_off(0);
	while(user_btn_read());
	delay();
	f3d_led_off(1);
	while(user_btn_read());
	delay();
	f3d_led_off(2);
	while(user_btn_read());
	delay();
	f3d_led_off(3);
	while(user_btn_read());
	delay();
	f3d_led_off(4);
	while(user_btn_read());
	delay();
	f3d_led_off(5);
	while(user_btn_read());
	delay();
	f3d_led_off(6);
	while(user_btn_read());
	delay();
	f3d_led_off(7);
	while(user_btn_read());
	delay();
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
