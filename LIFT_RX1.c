#include<lpc21xx.h>
#include "lcddriver.h"
#define LED1 1<<16
#define u8 unsigned int
typedef struct can_rx
{
u8 id;
u8 rtr;
u8 dlc;
u8 data1;
u8 data2;
}rx;
void can_rx(rx*);
void can_init(void);
int main()
{
	rx m1;
	IODIR0|=LED1;
	IOSET0=LED1;
	can_init();
	LCD_INIT();
	while(1)
	{
		can_rx(&m1);
		if(m1.data1==1)
		{
		LCD_CMD(0X01);
		LCD_CMD(0X80);
		STRING("FIRST FLOOR");
		    IOCLR0=LED1;
			delay(200);
		}
		else
		{
		LCD_CMD(0X01);
			IOSET0=LED1;
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
void can_rx(rx* m1)
{
	while((C2GSR&0X01)==0);
	m1->id=C2RID;
	m1->dlc=(C2RFS>>16)&0X0F;
	m1->rtr=(C2RFS>>30)&0X01;
	if(m1->rtr==0)
	{
	m1->data1=C2RDA;
	m1->data2=C2RDB;
	}
	C2CMR=(1<<2);
}

