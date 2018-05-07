
#include<AT89x51.H>
#include "stdio.h"


unsigned char receive_buff[50]=0;


void delay(unsigned int x){
	unsigned char i;
	while(x--){
		for(i=0;i<120;i++);
	}
}


void send_char (unsigned char c){
	SBUF=c;
	while(TI==0);
	TI=0;
}

void send_str(unsigned char *s){
	while(*s != '\0'){
		send_char(*s);
		s++;
		delay(5);
	}
}

void PutString(unsigned char *TXStr)  
{                
    ES=0;     
     while(*TXStr!=0) 
    {                      
        SBUF=*TXStr;
        while(TI==0);
        TI=0;    
        TXStr++;
    }
    ES=1; 
}  
unsigned char * RecStr;
unsigned char count;  
unsigned char num=0;
//
bit ReceiveString()    
{
    RecStr=receive_buff;
		num=0;
    loop:    
    *RecStr=SBUF;
    count=0;
    RI=0;    
    //if(num<10)  //
    //{
       // num++;
        RecStr++;    
        while(!RI)
        {
            count++;
            if(count>130)
						{
							*RecStr='\0';
						//	RI=0;
							return 0;
						}    //
        }
        goto loop;
    //}
		*RecStr='\0';
	//	RI=0;
		
    return 1;
}






void sint() interrupt 4
{
    if(ReceiveString())    
    {
        send_str(receive_buff);   
    }
		else 
		{
        send_str(receive_buff);		
		}

    RI=0;  //

		
}


/*********************************************************************/		


	void main(void)
{
		TMOD=0x20;  
    TH1=0xFd;  		   //11.0592M¾§Õñ£¬9600²¨ÌØÂÊ
    TL1=0xFd;
    SCON=0x50;  
		//TMOD &=~0x10;
		PCON=0x00;
		//PCON |=0x80;
    TR1=1;
		ES=1;  
		TI=0;
		RI=0;
    EA=1;   
	
		while(1){
		
		}
}