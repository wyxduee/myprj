#include <REGX52.H>
//#include<AT89x51.H>
#include "stdio.h"


unsigned char receive_buff[50];


void delay(unsigned int x){
	unsigned char i;
	while(x--){
		for(i=0;i<120;i++);
	}
}


// void send_char (unsigned char c){
// 	SBUF=c;
// 	while(TI==0);
// 	TI=0;
// }
// 	unsigned char *s;
// void send_str(unsigned char *k){
// 	s=k;
// 	while(*s != '\0'){
// 		send_char(*s);
// 		s++;
// 		//delay(5);
// 	}
// 	s=NULL;
// 	
// }
void send_str(unsigned char *s)
{
	ES=0;
	if(*s=='\0')
	{
			return;
	}
	else 
	{
		while(*s!=0){
			SBUF=*s;
			while(TI==0);
			TI=0;
			s++;		
	}
	ES=1;
	
}
return;
}

void send_char(unsigned char c)
{
		ES=0;
		SBUF=c;
		while(TI==0);
		TI=0;
		ES=1;
		return;
}

// void PutString(unsigned char *TXStr)  
// {                
//     ES=0;     
//      while(*TXStr!=0) 
//     {                      
//         SBUF=*TXStr;
//         while(TI==0);
//         TI=0;    
//         TXStr++;
//     }
//     ES=1; 
// }  
//unsigned char * RecStr;
//unsigned int count;  
//unsigned char num=0;
//



/************************************************************************/
		#define Imax 14000    //此处为晶振为11.0592时的取值, 
    #define Imin 8000    //如用其它频率的晶振时,
    #define Inum1 1450    //要改变相应的取值。
    #define Inum2 700 
    #define Inum3 3000 

	unsigned char f=0;
    unsigned char Im[4]={0x00,0x00,0x00,0x00};
    unsigned char show[2]={0,0};
    unsigned long m,Tc;
    unsigned char IrOK;

   
/************************************************************************/	
/************************************************************************/	
//外部中断解码程序
  void intersvr1(void) interrupt 2 using 1
{
    Tc=TH0*256+TL0;                                               //提取中断时间间隔时长
    TH0=0; 
    TL0=0;              //定时中断重新置零
 if((Tc>Imin)&&(Tc<Imax))
      { 
        m=0;
        f=1;
        return;
      }       //找到启始码

   if(f==1)
      {
        if(Tc>Inum1&&Tc<Inum3) 
    {
   Im[m/8]=Im[m/8]>>1|0x80; m++; 
       }
      if(Tc>Inum2&&Tc<Inum1) 
        {
         Im[m/8]=Im[m/8]>>1; m++; //取码
  }
  if(m==32) 
   {
         m=0;  
         f=0;
         if(Im[2]==~Im[3]) 
      {
           IrOK=1; 
   }
        else IrOK=0;   //取码完成后判断读码是否正确
     }
               //准备读下一码
   }
 
}
/************************************************************************/
unsigned char i;
unsigned int count;
void rsint() interrupt 4 using 0
{		
		i=0;
		count=0;
		receive_buff[i]=SBUF;
		RI=0;
	loop:
		i++;
		while(!RI){
				count++;
				if(count>130){
					receive_buff[i]='\0';
					send_str(receive_buff);
					return ;
				}
			receive_buff[i]=SBUF;
		}
		RI=0;
		count=0;
		goto loop;
}




/*********************************************************************/	
//用简单的方法定义小车运动语句---直接IO口定义
	#define Left_moto_go      {P1_4=1,P1_5=0,P1_6=0,P1_7=1;}    //左边两个电机向前走
	#define Left_moto_back    {P1_4=0,P1_5=1,P1_6=1,P1_7=0;} 	//左边两个电机向后转
	#define Left_moto_Stop    {P1_4=0,P1_5=0,P1_6=0,P1_7=0;}    //左边两个电机停转                     
	#define Right_moto_go     {P1_0=1,P1_1=0,P1_2=0,P1_3=1;}	//右边两个电机向前走
	#define Right_moto_back   {P1_0=0,P1_1=1,P1_2=1,P1_3=0;}	//右边两个电机向后走
	#define Right_moto_Stop   {P1_0=0,P1_1=0,P1_2=0,P1_3=0;}	//右边两个电机停转  

/************************************************************************/
//前速前进
     void  run(void)
{
    
	 Left_moto_go ;   //左电机往前走
	 Right_moto_go ;  //右电机往前走
}

//前速后退
     void  backrun(void)
{
    
	 Left_moto_back ;   //左电机往前走
	 Right_moto_back ;  //右电机往前走
}

//左转
     void  leftrun(void)
{
    
	 Left_moto_back ;   //左电机往前走
	 Right_moto_go ;  //右电机往前走
}

//右转
     void  rightrun(void)
{
    
	 Left_moto_go ;   //左电机往前走
	 Right_moto_back ;  //右电机往前走
}
//STOP
     void  stoprun(void)
{
    
	 Left_moto_Stop ;   //左电机往前走
	 Right_moto_Stop ;  //右电机往前走
}



	void main(void)
{
		TMOD=0x21;  
    TH1=0xFd;  		   //11.0592M??????9600??????
    TL1=0xFd;
    SCON=0x50;  
		//TMOD &=~0x11;
		PCON=0x00;
		//PCON |=0x80;
    TR1=1;
		ES=1;  
		TI=0;
		RI=0;
    EA=1;  
	
	  IT1=1;
    EX1=1;
 
  //  TMOD|=0x11;  
    TH0=0;
		TL0=0;
    TR0=1;
		EA=1;
		Im[4]='\0';

	while(1)							/*无限循环*/
	{ 

	    if(IrOK==1) 
     { 
			 send_char(Im[2]);
//            	switch(Im[2])
//    		{
//      case 0x18:  run(); 			     //前进
//              break;
//      case 0x52:  backrun();  			 //后退	 
//              break;
//      case 0x5a:  leftrun(); 			 //左转
//              break;
// 	 case 0x08:  rightrun(); 			 //右转
//              break;
// 	 case 0x1C:  stoprun();			 //停止
//              break;
// 	 default:break;

   	//	}

           IrOK=0;
     }
	 
					 
	 }
}