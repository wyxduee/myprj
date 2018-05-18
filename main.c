#include <REGX52.H>
//#include <AT89x51.H>
#include "stdio.h"
#include <intrins.h>


unsigned char receive_buff[5];
unsigned char stage_flag=0;
 unsigned char work_time=10;
void delay(unsigned int x){
	unsigned char i;
	while(x--){
		for(i=0;i<120;i++);
	}
}


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
//�ⲿ�ж�1�������
  void intersvr1(void) interrupt 2 using 1 //�ⲿ�ж�1
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
					if(stage_flag==0)
					{
							if(Im[2]==0x16)
							{
								stage_flag=1;//1
							}
					}
					else if(stage_flag==1)
					{
							if(Im[2]==0x18)
							{
									work_time++;
							}
							else if(Im[2]==0x52)
								{
									if(work_time>1)
										work_time--;
										
								}
										
								else if(Im[2]==0x1C)
											stage_flag=2;
							}

					else if(stage_flag==3)//
					{
						  if(Im[2]==0x0C)
							{
								stage_flag=4;//
							}
					}
					
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


/************************************************************************/
//�������ʾ
/************************************************************************/
unsigned char const discode[] ={ 0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xBF,0xff/*-*/};
unsigned char const positon[4]={ 0xfe,0xfd,0xfb,0xf7};
unsigned char disbuff[4]	   ={ 0,0,0,0,};

/********************************************************/
unsigned char posit=0;
    void Display_time(void)				 //�����ɨ��
	{
	 P0=(discode[disbuff[posit]]);
		  if(posit==0)
	 { P2_0=0,P2_1=1;P2_2=1;P2_3=1;}
	  if(posit==1)
	 { P2_0=1,P2_1=0;P2_2=1;P2_3=1;}
	  if(posit==2)
	 { P2_0=1,P2_1=1;P2_2=0;P2_3=1;}
		if(posit==3)
	 { P2_0=1,P2_1=1;P2_2=1;P2_3=0;}
		
	 //P2=positon[posit];
	  if(++posit>=4)
	  posit=0;
	}



/************************************************************************/
//���
/************************************************************************/
	#define Sevro_moto_pwm     P2_7	   //�Ӷ���źŶ�����PWM�źŵ����ٶ�
  unsigned char pwm_val_left  = 0;//��������
	unsigned char push_val_left =14;//������У�����Լ��1.5MS �ź
    unsigned int S=0;
	unsigned int S1=0;
	unsigned int S2=0;
	unsigned int S3=0;
	unsigned int S4=0;
	unsigned int  time=0;		    //ʱ�����
	unsigned int  timer=0;			//��ʱ��׼����
	unsigned char timer1=0;			//ɨ��ʱ�����	


/************************************************************************/
/*                    PWM���Ƶ��ת��                                   */
/************************************************************************/
/*                    ��������                                        */
/*����push_val_left��ֵ�ı���ת��,ռ�ձ�            */
		void pwm_Servomoto(void)
{  
 
    if(pwm_val_left<=push_val_left)
	       Sevro_moto_pwm=1; 
	else 
	       Sevro_moto_pwm=0;
	if(pwm_val_left>=200)
	pwm_val_left=0;
 
}
/************************************************************************/
//������
/************************************************************************/
	#define  ECHO  P2_4				   //�������ӿڶ���
	#define  TRIG  P2_5				   //�������ӿڶ���

/************************************************************************/
     void  StartModule() 		      //��������ź�
   {
	  TRIG=1;			                
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  TRIG=0;
   }
 /***************************************************/

 /***************************************************/
	  void Conut(void)		   //�������
	{
		TH0=0;
		TL0=0;
	  while(!ECHO);		       //��RXΪ��ʱ�ȴ�
	  TR0=1;			       //��������
	  while(ECHO);			   //��RXΪ1�������ȴ�
	  TR0=0;				   //�رռ���
	  time=TH0*256+TL0;		   //��ȡ������
	  TH0=0;
	  TL0=0;
	  S=(time*1.7)/100;        //�������CM
	  //disbuff[0]=S%1000/100;   //������ʾ
	 // disbuff[1]=S%1000%100/10;
	  //disbuff[2]=S%1000%10 %10;
	}
/************************************************************************/

/***************************************************/
	 void Display_length(void)				  //ɨ�������
	{
	 if(posit==0)
	 {P0=(discode[disbuff[posit]])&0x7f;}//������
	 else
	 {P0=discode[disbuff[posit]];}

	  if(posit==0)
	 { P2_1=0;P2_2=1;P2_3=1;}
	  if(posit==1)
	  {P2_1=1;P2_2=0;P2_3=1;}
	  if(posit==2)
	  {P2_1=1;P2_2=1;P2_3=0;}
	  if(++posit>=3)
	  posit=0;
	}
	


 unsigned int timer_int_count=0;

	
	
	
	//��ʱ��1�ж�
 	void time1()interrupt 3   using 2
{	
   TH1=(65536-100)/256;	  //100US��ʱ
	 TL1=(65536-100)%256;
	 timer++;				  //��ʱ��100USΪ׼���������������ʱ
	 pwm_val_left++;
	 pwm_Servomoto();

	 timer1++;				 //2MSɨһ�������
	 if(timer1>=20)
	 {
	 timer1=0;
	 Display_time();
	timer_int_count++;
	if(timer_int_count==500)
	 {
		 timer_int_count=0;
		 if(work_time==0)
		 {
				//work_time=20;
				stage_flag++;
			 return;
		 }
		 if(stage_flag==2)
				work_time--;
		 disbuff[2]=work_time/10;
		 disbuff[3]=work_time%10;
	 }		 
	 }


 }
/***************************************************/
 unsigned int timer0_count=0;

 	void timer0()interrupt 1   using 0
{	
	
	TH0=0xf8;
	TL0=0x30;
	timer0_count++; 
 }

 /***************************************************/
/************************************************************************/
 void  COMM( void ) 	//������	      
  {
  	     
		 
		  push_val_left=9;	  //�������ת90��
		  timer=0;
		  while(timer<=2000); //��ʱ400MS�ö��ת����λ��		 4000
		  StartModule();	  //�������������
          Conut();	 		  //�������
		  S2=S;  
  
		  push_val_left=19;	  //�������ת90��
		  timer=0;
		  while(timer<=2000); //��ʱ400MS�ö��ת����λ��
		  StartModule();	  //�������������
          Conut();			  //�������
		  S4=S; 	
	

		  push_val_left=14;	  //�������
		  timer=0;
		  while(timer<=2000); //��ʱ400MS�ö��ת����λ��

		  StartModule();	  //�������������
          Conut();			  //�������
		  S1=S; 	

          if((S2<40)||(S4<40)) //ֻҪ���Ҹ��о���С�ڣ�20CMС������
		  {
		  backrun();		   //����
		  timer=0;
		  while(timer<=1000);
		  }
		   
		  if(S2>S4)		 
		     {
				rightrun();  	//������߱ȳ����ұ߾���С	��ת	
		        timer=0;
		        while(timer<=1000);
		     }				      
		       else
		     {
		       leftrun();		//������߱ȳ����ұ߾����	��ת
		       timer=0;
		       while(timer<=1000);
		     }
		  			   

}





void length_count(void)
{
	EX1=0;
	TH1=0;
	TL1=0;
	while(!ECHO);		       //��RXΪ��ʱ�ȴ�
	TR1=1;
	while(ECHO);			   //��RXΪ1�������ȴ�
	TR1=0;				   //�رռ���
	time=TH1*256+TL1;		   //��ȡ������
  S=(time*1.7)/100;        //�������CM
	TH1=0;
	TL1=0;
	m=0;
	f=0;
	EX1=1;
	TR1=1;
}






void main(void){
//ϵͳ���ں������״̬���ȴ�����ң���ź���������
	//��Ҫ�õ��Ĺ����У�������ܽ���,�ⲿ�ж�1�ͼ�����1
	TMOD=0x01;//���ö�ʱ��1Ϊ16bitģʽ

	IT1=1;//�����ⲿ�ж�1Ϊ���ش���
	EX1=1;//�����ⲿ�ж�1�ж�
	EA=1;//�������ж�
	TR0=1;//������ʱ��1
	P2_6=0;
	while(stage_flag==0);
	EA=0;//�ر����ж�
	EX1=0;//��ֹ�ⲿ�ж�1
	TR0=0;//�رն�ʱ��1
	P2_6=1;
//ϵͳ��ʼ�������������򿪱��ϣ�������ʱ1����
	//��Ҫ�õ��Ĺ����У����������ϣ��������ʾ
//
	TMOD=0x11;//���ö�ʱ��0�Ͷ�ʱ��1Ϊ16bitģʽ
	TH1=(65536-100)/256;	  //100US��ʱ
	TL1=(65536-100)%256;
	EX1=1;
	ET1= 1;//����ʱ��1�ж�
	EA = 1;//�������ж�
	TR1= 1;//������ʱ��1
	TR0=1;
	push_val_left=14;
	delay(100);
	while(stage_flag==1);
	while(stage_flag==2)		       /*����ѭ��*/
	{ 
	 if(timer>=500)	  //100MS����������һ��	 1000
	   {
	       timer=0;
		   StartModule(); //�������
           Conut();		  //�������
           if(S<20)		  //����С��20CM
		   {
		   stoprun();	  //С��ֹͣ
		   COMM(); 		  //������
		   }
		   else
		   if(S>40)		  //������ڣ�30CM��ǰ��
		   run();
	   }

	}
		stoprun();
	 {P2_1=1;P2_2=1;P2_3=1;}
	push_val_left=14;
	delay(100);
	ET1= 0;//�رն�ʱ��1�ж�
	EA=0;//�ر����ж�
	TR1=0;//ֹͣ��ʱ��1
	P2_6=0;
		
//����ʱ������С���򿪿�ʼѰ�ҳ��㷵�س��
	//��Ҫ�õ��Ĺ����У�������࣬�������
	//�ⲿ�ж�1 �Ͷ�ʱ��1 ��ʱ��0 �ⲿ�ж�1�����������źţ���ʱ��1������¼��������ʱ�� ��ʱ��0�����̶���ʱ
	//�ⲿ�ж�1�ж����2   ��ʱ��1��Ӧ�ж����3 ��ʱ��0��Ӧ�ж����1
	
	TMOD=0x11;//���ö�ʱ��0��1Ϊ16bit��ʱģʽ
	TH1=0;
	TL1=0;
	IT1=1;//�����ⲿ�ж�1Ϊ���ش���
	EX1=1;//�����ⲿ�ж�1�ж�
	TH0=0;
	TL0=0;
	Im[4]='\0';
	EA = 1;//�������ж�
	TR1=1;//������ʱ��1
	//TR0=1;//������ʱ��0


while(stage_flag==3)
	{
		rightrun();
		delay(50);
		stoprun();
		delay(300);
}
	stoprun();
	EX1=0;
	TR1=0;
	TR0=0;
	while(S>20)
	{
		run();
		delay(100);
		stoprun();
		StartModule();	  //�������������
    Conut();			  //�������
		delay(100);
	}
	stoprun();
	

}





// void main(void){
// //ϵͳ���ں������״̬���ȴ�����ң���ź���������
// 	//��Ҫ�õ��Ĺ����У�������ܽ���,�ⲿ�ж�1�ͼ�����1
// 	TMOD=0x10;//���ö�ʱ��1Ϊ16bitģʽ

// 	IT1=1;//�����ⲿ�ж�1Ϊ���ش���
// 	EX1=1;//�����ⲿ�ж�1�ж�
// 	EA=1;//�������ж�
// 	TR1=1;//������ʱ��1
// 	P2_6=0;
// 	while(stage_flag==0);
// 	EA=0;//�ر����ж�
// 	EX1=0;//��ֹ�ⲿ�ж�1
// 	TR1=0;//�رն�ʱ��1
// 	P2_6=1;
// //ϵͳ��ʼ�������������򿪱��ϣ�������ʱ1����
// 	//��Ҫ�õ��Ĺ����У����������ϣ��������ʾ
// //
// 	TMOD=0x11;//���ö�ʱ��0�Ͷ�ʱ��1Ϊ16bitģʽ
// 	TH1=(65536-100)/256;	  //100US��ʱ
// 	TL1=(65536-100)%256;
// 	ET1= 1;//����ʱ��1�ж�
// 	EA = 1;//�������ж�
// 	TR1= 1;//������ʱ��1

// 	push_val_left=14;
// 	delay(100);
// 	while(stage_flag==1);
// 	while(stage_flag==2)		       /*����ѭ��*/
// 	{ 
// 	 if(timer>=500)	  //100MS����������һ��	 1000
// 	   {
// 	       timer=0;
// 		   StartModule(); //�������
//            Conut();		  //�������
//            if(S<20)		  //����С��20CM
// 		   {
// 		   stoprun();	  //С��ֹͣ
// 		   COMM(); 		  //������
// 		   }
// 		   else
// 		   if(S>40)		  //������ڣ�30CM��ǰ��
// 		   run();
// 	   }

// 	}
// 		stoprun();
// 	 {P2_1=1;P2_2=1;P2_3=1;}
// 	push_val_left=14;
// 	delay(100);
// 	ET1= 0;//�رն�ʱ��1�ж�
// 	EA=0;//�ر����ж�
// 	TR1=0;//ֹͣ��ʱ��1
// 	P2_6=0;
// 		
// //����ʱ������С���򿪿�ʼѰ�ҳ��㷵�س��
// 	//��Ҫ�õ��Ĺ����У�������࣬�������
// 	//�ⲿ�ж�1 �Ͷ�ʱ��1 ��ʱ��0 �ⲿ�ж�1�����������źţ���ʱ��1������¼��������ʱ�� ��ʱ��0�����̶���ʱ
// 	//�ⲿ�ж�1�ж����2   ��ʱ��1��Ӧ�ж����3 ��ʱ��0��Ӧ�ж����1
// 	
// 	TMOD=0x11;//���ö�ʱ��0��1Ϊ16bit��ʱģʽ
// 	TH1=0;
// 	TL1=0;
// 	IT1=1;//�����ⲿ�ж�1Ϊ���ش���
// 	EX1=1;//�����ⲿ�ж�1�ж�
// 	TH0=0;
// 	TL0=0;
// 	Im[4]='\0';
// 	EA = 1;//�������ж�
// 	TR1=1;//������ʱ��1
// 	//TR0=1;//������ʱ��0


// while(stage_flag==3)
// 	{
// 		rightrun();
// 		delay(50);
// 		stoprun();
// 		delay(300);
// }
// 	stoprun();
// 	EX1=0;
// 	TR1=0;
// 	TR0=0;
// 	while(S>20)
// 	{
// 		run();
// 		delay(100);
// 		stoprun();
// 		StartModule();	  //�������������
//     Conut();			  //�������
// 		delay(100);
// 	}
// 	stoprun();
// 	

// }













