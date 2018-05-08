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
		#define Imax 14000    //�˴�Ϊ����Ϊ11.0592ʱ��ȡֵ, 
    #define Imin 8000    //��������Ƶ�ʵľ���ʱ,
    #define Inum1 1450    //Ҫ�ı���Ӧ��ȡֵ��
    #define Inum2 700 
    #define Inum3 3000 

	unsigned char f=0;
    unsigned char Im[4]={0x00,0x00,0x00,0x00};
    unsigned char show[2]={0,0};
    unsigned long m,Tc;
    unsigned char IrOK;

   
/************************************************************************/	
/************************************************************************/	
//�ⲿ�жϽ������
  void intersvr1(void) interrupt 2 using 1
{
    Tc=TH0*256+TL0;                                               //��ȡ�ж�ʱ����ʱ��
    TH0=0; 
    TL0=0;              //��ʱ�ж���������
 if((Tc>Imin)&&(Tc<Imax))
      { 
        m=0;
        f=1;
        return;
      }       //�ҵ���ʼ��

   if(f==1)
      {
        if(Tc>Inum1&&Tc<Inum3) 
    {
   Im[m/8]=Im[m/8]>>1|0x80; m++; 
       }
      if(Tc>Inum2&&Tc<Inum1) 
        {
         Im[m/8]=Im[m/8]>>1; m++; //ȡ��
  }
  if(m==32) 
   {
         m=0;  
         f=0;
         if(Im[2]==~Im[3]) 
      {
           IrOK=1; 
   }
        else IrOK=0;   //ȡ����ɺ��ж϶����Ƿ���ȷ
     }
               //׼������һ��
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
//�ü򵥵ķ�������С���˶����---ֱ��IO�ڶ���
	#define Left_moto_go      {P1_4=1,P1_5=0,P1_6=0,P1_7=1;}    //������������ǰ��
	#define Left_moto_back    {P1_4=0,P1_5=1,P1_6=1,P1_7=0;} 	//�������������ת
	#define Left_moto_Stop    {P1_4=0,P1_5=0,P1_6=0,P1_7=0;}    //����������ͣת                     
	#define Right_moto_go     {P1_0=1,P1_1=0,P1_2=0,P1_3=1;}	//�ұ����������ǰ��
	#define Right_moto_back   {P1_0=0,P1_1=1,P1_2=1,P1_3=0;}	//�ұ�������������
	#define Right_moto_Stop   {P1_0=0,P1_1=0,P1_2=0,P1_3=0;}	//�ұ��������ͣת  

/************************************************************************/
//ǰ��ǰ��
     void  run(void)
{
    
	 Left_moto_go ;   //������ǰ��
	 Right_moto_go ;  //�ҵ����ǰ��
}

//ǰ�ٺ���
     void  backrun(void)
{
    
	 Left_moto_back ;   //������ǰ��
	 Right_moto_back ;  //�ҵ����ǰ��
}

//��ת
     void  leftrun(void)
{
    
	 Left_moto_back ;   //������ǰ��
	 Right_moto_go ;  //�ҵ����ǰ��
}

//��ת
     void  rightrun(void)
{
    
	 Left_moto_go ;   //������ǰ��
	 Right_moto_back ;  //�ҵ����ǰ��
}
//STOP
     void  stoprun(void)
{
    
	 Left_moto_Stop ;   //������ǰ��
	 Right_moto_Stop ;  //�ҵ����ǰ��
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

	while(1)							/*����ѭ��*/
	{ 

	    if(IrOK==1) 
     { 
			 send_char(Im[2]);
//            	switch(Im[2])
//    		{
//      case 0x18:  run(); 			     //ǰ��
//              break;
//      case 0x52:  backrun();  			 //����	 
//              break;
//      case 0x5a:  leftrun(); 			 //��ת
//              break;
// 	 case 0x08:  rightrun(); 			 //��ת
//              break;
// 	 case 0x1C:  stoprun();			 //ֹͣ
//              break;
// 	 default:break;

   	//	}

           IrOK=0;
     }
	 
					 
	 }
}