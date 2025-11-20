#include<lpc21xx.h>
#include "lcddriver.h"
#define LED1 1<<19
#define LED2 1<<20
#define SW1 11
#define SW2 12
#define u8 unsigned int
typedef struct can_tx
{
u8 id;
u8 rtr;
u8 dlc;
u8 data1;
u8 data2;
}tx;
void can_init(void);
void can_tx(tx);

int main()
{
	tx m1;	
	IODIR0|=LED1|LED2;
	IOSET0=LED1|LED2;
	can_init();
	LCD_INIT();
	m1.id=0x1AF;;
	m1.rtr=0;
	m1.dlc=4;
	while(1)
	{
	  if(((IOPIN0>>SW1)&1)==0)
	  {
	    LCD_CMD(0X01);
		LCD_CMD(0X80);
		STRING("GROUND FLOOR");
	  	IOSET0=LED2;
		IOCLR0=LED1;
		m1.data1=0x01;
		m1.data2=0;
	    can_tx(m1);
	  }
	  else if(((IOPIN0>>SW2)&1)==0)
	  {
	  LCD_CMD(0X01);
	  	IOSET0=LED1;
		IOCLR0=LED2;
		m1.data1=0x02;
		m1.data2=0;
		can_tx(m1);
	  }
	}
}
void can_init(void)
{
	PINSEL1|=0X14000;
	VPBDIV=1;
	C2MOD=0X01;
	AFMR=0X02;
	C2BTR=0X001C001D;
	C2MOD=0X00;
}
void can_tx(tx m1)
{
	C2TID1=m1.id;
	C2TFI1=(m1.dlc<<16);
	if(m1.rtr==0)
	{
	C2TFI1&=~(1<<30);
	C2TDA1=m1.data1;
	C2TDB1=m1.data2;
	}
	else
	C2TFI1|=(1<<30);
	C2CMR=(1<<0)|(1<<5);
while((C2GSR&(1<<3))==0);
}







