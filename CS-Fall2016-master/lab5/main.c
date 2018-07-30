//main.c for lab6
#include <f3d_uart.h>
#include <stdio.h>
#include <f3d_gyro.h>
#include <f3d_led.h>
#include <f3d_user_btn.h>

void delay(void) {
  int i = 2000000;
  while (i-- > 0){
    asm("nop");
  }
}

int main(void){
  f3d_uart_init();
  delay();
  f3d_gyro_init();
  f3d_led_init();
  f3d_user_btn_init();
  delay();
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  float Data[3];
  int axis = 0;
  char caxis[] = {'X','Y','Z'};
  while(1){
    int  ch = getcharub();
      switch(ch){
      case 'x':
      axis = 0;
      break;
      case 'y':
      axis = 1;
      break;
      case 'z':
      axis = 2;
      break;
      }
      ch=0;
    f3d_gyro_getdata(Data);
    //printf("\n %f %f %f\n", Data[0], Data[1], Data[2]);
    if(user_btn_read()){ 
      axis++; 
       if(axis==3) 
	 axis=0; 
    } 
    printf("%c-axis:%f\n",caxis[axis],Data[axis]);
    f3d_led_all_off(); 
    // f3d_led_on();
    if(Data[axis]<0){
      if(Data[axis]<-20)
	f3d_led_on(1);
      if(Data[axis]<-40)
	 f3d_led_on(2);
      if(Data[axis]<-60)
	f3d_led_on(3);
      if(Data[axis]<-80)
	f3d_led_on(4);
      if(Data[axis]<-100)
	f3d_led_on(5);
    }else{
      if(Data[axis]>20)
	 f3d_led_on(1);
      if(Data[axis]>40)
	 f3d_led_on(0);
      if(Data[axis]>60)
	 f3d_led_on(7);
      if(Data[axis]>80)
	f3d_led_on(6);
      if(Data[axis]>100)
	f3d_led_on(5);
    }
    delay();
  }
}


void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
