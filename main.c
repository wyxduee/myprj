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
		#define Imax 14000    //´Ë´¦Îª¾§ÕñÎª11.0592Ê±µÄÈ¡Öµ, 
    #define Imin 8000    //ÈçÓÃÆäËüÆµÂÊµÄ¾§ÕñÊ±,
    #define Inum1 1450    //Òª¸Ä±äÏàÓ¦µÄÈ¡Öµ¡£
    #define Inum2 700 
    #define Inum3 3000 

	unsigned char f=0;
    unsigned char Im[4]={0x00,0x00,0x00,0x00};
    unsigned char show[2]={0,0};
    unsigned long m,Tc;
    unsigned char IrOK;

   
/************************************************************************/	
/************************************************************************/	
//Íâ²¿ÖĞ¶Ï1½âÂë³ÌĞò
  void intersvr1(void) interrupt 2 using 1 //Íâ²¿ÖĞ¶Ï1
{
    Tc=TH0*256+TL0;                                               //ÌáÈ¡ÖĞ¶ÏÊ±¼ä¼ä¸ôÊ±³¤
    TH0=0; 
    TL0=0;              //¶¨Ê±ÖĞ¶ÏÖØĞÂÖÃÁã
 if((Tc>Imin)&&(Tc<Imax))
      { 
        m=0;
        f=1;
        return;
      }       //ÕÒµ½ÆôÊ¼Âë

   if(f==1)
      {
        if(Tc>Inum1&&Tc<Inum3) 
    {
   Im[m/8]=Im[m/8]>>1|0x80; m++; 
       }
      if(Tc>Inum2&&Tc<Inum1) 
        {
         Im[m/8]=Im[m/8]>>1; m++; //È¡Âë
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
        else IrOK=0;   //È¡ÂëÍê³ÉºóÅĞ¶Ï¶ÁÂëÊÇ·ñÕıÈ·
     }
               //×¼±¸¶ÁÏÂÒ»Âë
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
//ÓÃ¼òµ¥µÄ·½·¨¶¨ÒåĞ¡³µÔË¶¯Óï¾ä---Ö±½ÓIO¿Ú¶¨Òå
	#define Left_moto_go      {P1_4=1,P1_5=0,P1_6=0,P1_7=1;}    //×ó±ßÁ½¸öµç»úÏòÇ°×ß
	#define Left_moto_back    {P1_4=0,P1_5=1,P1_6=1,P1_7=0;} 	//×ó±ßÁ½¸öµç»úÏòºó×ª
	#define Left_moto_Stop    {P1_4=0,P1_5=0,P1_6=0,P1_7=0;}    //×ó±ßÁ½¸öµç»úÍ£×ª                     
	#define Right_moto_go     {P1_0=1,P1_1=0,P1_2=0,P1_3=1;}	//ÓÒ±ßÁ½¸öµç»úÏòÇ°×ß
	#define Right_moto_back   {P1_0=0,P1_1=1,P1_2=1,P1_3=0;}	//ÓÒ±ßÁ½¸öµç»úÏòºó×ß
	#define Right_moto_Stop   {P1_0=0,P1_1=0,P1_2=0,P1_3=0;}	//ÓÒ±ßÁ½¸öµç»úÍ£×ª  

/************************************************************************/
//Ç°ËÙÇ°½ø
     void  run(void)
{
    
	 Left_moto_go ;   //×óµç»úÍùÇ°×ß
	 Right_moto_go ;  //ÓÒµç»úÍùÇ°×ß
}

//Ç°ËÙºóÍË
     void  backrun(void)
{
    
	 Left_moto_back ;   //×óµç»úÍùÇ°×ß
	 Right_moto_back ;  //ÓÒµç»úÍùÇ°×ß
}

//×ó×ª
     void  leftrun(void)
{
    
	 Left_moto_back ;   //×óµç»úÍùÇ°×ß
	 Right_moto_go ;  //ÓÒµç»úÍùÇ°×ß
}

//ÓÒ×ª
     void  rightrun(void)
{
    
	 Left_moto_go ;   //×óµç»úÍùÇ°×ß
	 Right_moto_back ;  //ÓÒµç»úÍùÇ°×ß
}
//STOP
     void  stoprun(void)
{
    
	 Left_moto_Stop ;   //×óµç»úÍùÇ°×ß
	 Right_moto_Stop ;  //ÓÒµç»úÍùÇ°×ß
}


/************************************************************************/
//ÊıÂë¹ÜÏÔÊ¾
/************************************************************************/
unsigned char const discode[] ={ 0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xBF,0xff/*-*/};
unsigned char const positon[4]={ 0xfe,0xfd,0xfb,0xf7};
unsigned char disbuff[4]	   ={ 0,0,0,0,};

/********************************************************/
unsigned char posit=0;
    void Display_time(void)				 //ÊıÂë¹ÜÉ¨Ãè
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
//¶æ»ú
/************************************************************************/
	#define Sevro_moto_pwm     P2_7	   //½Ó¶æ»úĞÅºÅ¶ËÊäÈëPWMĞÅºÅµ÷½ÚËÙ¶È
  unsigned char pwm_val_left  = 0;//±äÁ¿¶¨Òå
	unsigned char push_val_left =14;//¶æ»ú¹éÖĞ£¬²úÉúÔ¼£¬1.5MS ĞÅº
    unsigned int S=0;
	unsigned int S1=0;
	unsigned int S2=0;
	unsigned int S3=0;
	unsigned int S4=0;
	unsigned int  time=0;		    //Ê±¼ä±äÁ¿
	unsigned int  timer=0;			//ÑÓÊ±»ù×¼±äÁ¿
	unsigned char timer1=0;			//É¨ÃèÊ±¼ä±äÁ¿	


/************************************************************************/
/*                    PWMµ÷ÖÆµç»ú×ªËÙ                                   */
/************************************************************************/
/*                    ×óµç»úµ÷ËÙ                                        */
/*µ÷½Úpush_val_leftµÄÖµ¸Ä±äµç»ú×ªËÙ,Õ¼¿Õ±È            */
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
//³¬Éù²¨
/************************************************************************/
	#define  ECHO  P2_4				   //³¬Éù²¨½Ó¿Ú¶¨Òå
	#define  TRIG  P2_5				   //³¬Éù²¨½Ó¿Ú¶¨Òå

/************************************************************************/
     void  StartModule() 		      //Æô¶¯²â¾àĞÅºÅ
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
	  void Conut(void)		   //¼ÆËã¾àÀë
	{
		TH0=0;
		TL0=0;
	  while(!ECHO);		       //µ±RXÎªÁãÊ±µÈ´ı
	  TR0=1;			       //¿ªÆô¼ÆÊı
	  while(ECHO);			   //µ±RXÎª1¼ÆÊı²¢µÈ´ı
	  TR0=0;				   //¹Ø±Õ¼ÆÊı
	  time=TH0*256+TL0;		   //¶ÁÈ¡Âö¿í³¤¶È
	  TH0=0;
	  TL0=0;
	  S=(time*1.7)/100;        //Ëã³öÀ´ÊÇCM
	  //disbuff[0]=S%1000/100;   //¸üĞÂÏÔÊ¾
	 // disbuff[1]=S%1000%100/10;
	  //disbuff[2]=S%1000%10 %10;
	}
/************************************************************************/

/***************************************************/
	 void Display_length(void)				  //É¨ÃèÊıÂë¹Ü
	{
	 if(posit==0)
	 {P0=(discode[disbuff[posit]])&0x7f;}//²úÉúµã
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

	
	
	
	//¶¨Ê±Æ÷1ÖĞ¶Ï
 	void time1()interrupt 3   using 2
{	
   TH1=(65536-100)/256;	  //100US¶¨Ê±
	 TL1=(65536-100)%256;
	 timer++;				  //¶¨Ê±Æ÷100USÎª×¼¡£ÔÚÕâ¸ö»ù´¡ÉÏÑÓÊ±
	 pwm_val_left++;
	 pwm_Servomoto();

	 timer1++;				 //2MSÉ¨Ò»´ÎÊıÂë¹Ü
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
 void  COMM( void ) 	//·½Ïòº¯Êı	      
  {
  	     
		 
		  push_val_left=9;	  //¶æ»úÏò×ó×ª90¶È
		  timer=0;
		  while(timer<=2000); //ÑÓÊ±400MSÈÃ¶æ»ú×ªµ½ÆäÎ»ÖÃ		 4000
		  StartModule();	  //Æô¶¯³¬Éù²¨²â¾à
          Conut();	 		  //¼ÆËã¾àÀë
		  S2=S;  
  
		  push_val_left=19;	  //¶æ»úÏòÓÒ×ª90¶È
		  timer=0;
		  while(timer<=2000); //ÑÓÊ±400MSÈÃ¶æ»ú×ªµ½ÆäÎ»ÖÃ
		  StartModule();	  //Æô¶¯³¬Éù²¨²â¾à
          Conut();			  //¼ÆËã¾àÀë
		  S4=S; 	
	

		  push_val_left=14;	  //¶æ»ú¹éÖĞ
		  timer=0;
		  while(timer<=2000); //ÑÓÊ±400MSÈÃ¶æ»ú×ªµ½ÆäÎ»ÖÃ

		  StartModule();	  //Æô¶¯³¬Éù²¨²â¾à
          Conut();			  //¼ÆËã¾àÀë
		  S1=S; 	

          if((S2<40)||(S4<40)) //Ö»Òª×óÓÒ¸÷ÓĞ¾àÀëĞ¡ÓÚ£¬20CMĞ¡³µºóÍË
		  {
		  backrun();		   //ºóÍË
		  timer=0;
		  while(timer<=1000);
		  }
		   
		  if(S2>S4)		 
		     {
				rightrun();  	//³µµÄ×ó±ß±È³µµÄÓÒ±ß¾àÀëĞ¡	ÓÒ×ª	
		        timer=0;
		        while(timer<=1000);
		     }				      
		       else
		     {
		       leftrun();		//³µµÄ×ó±ß±È³µµÄÓÒ±ß¾àÀë´ó	×ó×ª
		       timer=0;
		       while(timer<=1000);
		     }
		  			   

}





void length_count(void)
{
	EX1=0;
	TH1=0;
	TL1=0;
	while(!ECHO);		       //µ±RXÎªÁãÊ±µÈ´ı
	TR1=1;
	while(ECHO);			   //µ±RXÎª1¼ÆÊı²¢µÈ´ı
	TR1=0;				   //¹Ø±Õ¼ÆÊı
	time=TH1*256+TL1;		   //¶ÁÈ¡Âö¿í³¤¶È
  S=(time*1.7)/100;        //Ëã³öÀ´ÊÇCM
	TH1=0;
	TL1=0;
	m=0;
	f=0;
	EX1=1;
	TR1=1;
}






void main(void){
//ÏµÍ³´¦ÓÚºìÍâ½ÓÊÜ×´Ì¬£¬µÈ´ı½ÓÊÕÒ£¿ØĞÅºÅÆô¶¯¹¤×÷
	//ĞèÒªÓÃµ½µÄ¹¦ÄÜÓĞ£ººìÍâ½ÓÊÜ½âÂë,Íâ²¿ÖĞ¶Ï1ºÍ¼ÆÊıÆ÷1
	TMOD=0x01;//ÉèÖÃ¶¨Ê±Æ÷1Îª16bitÄ£Ê½

	IT1=1;//ÉèÖÃÍâ²¿ÖĞ¶Ï1Îª±ßÑØ´¥·¢
	EX1=1;//ÔÊĞíÍâ²¿ÖĞ¶Ï1ÖĞ¶Ï
	EA=1;//¿ªÆô×ÜÖĞ¶Ï
	TR0=1;//Æô¶¯¶¨Ê±Æ÷1
	P2_6=0;
	while(stage_flag==0);
	EA=0;//¹Ø±Õ×ÜÖĞ¶Ï
	EX1=0;//½ûÖ¹Íâ²¿ÖĞ¶Ï1
	TR0=0;//¹Ø±Õ¶¨Ê±Æ÷1
	P2_6=1;
//ÏµÍ³¿ªÊ¼¹¤×÷£¬³¬Éù²¨´ò¿ª±ÜÕÏ£¬²¢µ¹¼ÆÊ±1·ÖÖÓ
	//ĞèÒªÓÃµ½µÄ¹¦ÄÜÓĞ£º³¬Éù²¨±ÜÕÏ£¬ÊıÂë¹ÜÏÔÊ¾
//
	TMOD=0x11;//ÉèÖÃ¶¨Ê±Æ÷0ºÍ¶¨Ê±Æ÷1Îª16bitÄ£Ê½
	TH1=(65536-100)/256;	  //100US¶¨Ê±
	TL1=(65536-100)%256;
	EX1=1;
	ET1= 1;//ÔÊĞí¶¨Ê±Æ÷1ÖĞ¶Ï
	EA = 1;//¿ªÆô×ÜÖĞ¶Ï
	TR1= 1;//Æô¶¯¶¨Ê±Æ÷1
	TR0=1;
	push_val_left=14;
	delay(100);
	while(stage_flag==1);
	while(stage_flag==2)		       /*ÎŞÏŞÑ­»·*/
	{ 
	 if(timer>=500)	  //100MS¼ì²âÆô¶¯¼ì²âÒ»´Î	 1000
	   {
	       timer=0;
		   StartModule(); //Æô¶¯¼ì²â
           Conut();		  //¼ÆËã¾àÀë
           if(S<20)		  //¾àÀëĞ¡ÓÚ20CM
		   {
		   stoprun();	  //Ğ¡³µÍ£Ö¹
		   COMM(); 		  //·½Ïòº¯Êı
		   }
		   else
		   if(S>40)		  //¾àÀë´óÓÚ£¬30CMÍùÇ°×ß
		   run();
	   }

	}
		stoprun();
	 {P2_1=1;P2_2=1;P2_3=1;}
	push_val_left=14;
	delay(100);
	ET1= 0;//¹Ø±Õ¶¨Ê±Æ÷1ÖĞ¶Ï
	EA=0;//¹Ø±Õ×ÜÖĞ¶Ï
	TR1=0;//Í£Ö¹¶¨Ê±Æ÷1
	P2_6=0;
		
//µ¹¼ÆÊ±½áÊø£¬Ğ¡³µ´ò¿ª¿ªÊ¼Ñ°ÕÒ³äµçµã·µ»Ø³äµç
	//ĞèÒªÓÃµ½µÄ¹¦ÄÜÓĞ£º³¬Éù²â¾à£¬ºìÍâ½ÓÊÜ
	//Íâ²¿ÖĞ¶Ï1 ºÍ¶¨Ê±Æ÷1 ¶¨Ê±Æ÷0 Íâ²¿ÖĞ¶Ï1ÓÃÀ´¼ì²âºìÍâĞÅºÅ£¬¶¨Ê±Æ÷1ÓÃÀ´¼ÇÂ¼³¬Éù²¨µÄÊ±³¤ ¶¨Ê±Æ÷0ÓÃÀ´¹Ì¶¨¶¨Ê±
	//Íâ²¿ÖĞ¶Ï1ÖĞ¶ÏĞòºÅ2   ¶¨Ê±Æ÷1¶ÔÓ¦ÖĞ¶ÏĞòºÅ3 ¶¨Ê±Æ÷0¶ÔÓ¦ÖĞ¶ÏĞòºÅ1
	
	TMOD=0x11;//ÉèÖÃ¶¨Ê±Æ÷0ºÍ1Îª16bit¶¨Ê±Ä£Ê½
	TH1=0;
	TL1=0;
	IT1=1;//ÉèÖÃÍâ²¿ÖĞ¶Ï1Îª±ßÑØ´¥·¢
	EX1=1;//ÔÊĞíÍâ²¿ÖĞ¶Ï1ÖĞ¶Ï
	TH0=0;
	TL0=0;
	Im[4]='\0';
	EA = 1;//¿ªÆô×ÜÖĞ¶Ï
	TR1=1;//Æô¶¯¶¨Ê±Æ÷1
	//TR0=1;//Æô¶¯¶¨Ê±Æ÷0


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
		StartModule();	  //Æô¶¯³¬Éù²¨²â¾à
    Conut();			  //¼ÆËã¾àÀë
		delay(100);
	}
	stoprun();
	

}





// void main(void){
// //ÏµÍ³´¦ÓÚºìÍâ½ÓÊÜ×´Ì¬£¬µÈ´ı½ÓÊÕÒ£¿ØĞÅºÅÆô¶¯¹¤×÷
// 	//ĞèÒªÓÃµ½µÄ¹¦ÄÜÓĞ£ººìÍâ½ÓÊÜ½âÂë,Íâ²¿ÖĞ¶Ï1ºÍ¼ÆÊıÆ÷1
// 	TMOD=0x10;//ÉèÖÃ¶¨Ê±Æ÷1Îª16bitÄ£Ê½

// 	IT1=1;//ÉèÖÃÍâ²¿ÖĞ¶Ï1Îª±ßÑØ´¥·¢
// 	EX1=1;//ÔÊĞíÍâ²¿ÖĞ¶Ï1ÖĞ¶Ï
// 	EA=1;//¿ªÆô×ÜÖĞ¶Ï
// 	TR1=1;//Æô¶¯¶¨Ê±Æ÷1
// 	P2_6=0;
// 	while(stage_flag==0);
// 	EA=0;//¹Ø±Õ×ÜÖĞ¶Ï
// 	EX1=0;//½ûÖ¹Íâ²¿ÖĞ¶Ï1
// 	TR1=0;//¹Ø±Õ¶¨Ê±Æ÷1
// 	P2_6=1;
// //ÏµÍ³¿ªÊ¼¹¤×÷£¬³¬Éù²¨´ò¿ª±ÜÕÏ£¬²¢µ¹¼ÆÊ±1·ÖÖÓ
// 	//ĞèÒªÓÃµ½µÄ¹¦ÄÜÓĞ£º³¬Éù²¨±ÜÕÏ£¬ÊıÂë¹ÜÏÔÊ¾
// //
// 	TMOD=0x11;//ÉèÖÃ¶¨Ê±Æ÷0ºÍ¶¨Ê±Æ÷1Îª16bitÄ£Ê½
// 	TH1=(65536-100)/256;	  //100US¶¨Ê±
// 	TL1=(65536-100)%256;
// 	ET1= 1;//ÔÊĞí¶¨Ê±Æ÷1ÖĞ¶Ï
// 	EA = 1;//¿ªÆô×ÜÖĞ¶Ï
// 	TR1= 1;//Æô¶¯¶¨Ê±Æ÷1

// 	push_val_left=14;
// 	delay(100);
// 	while(stage_flag==1);
// 	while(stage_flag==2)		       /*ÎŞÏŞÑ­»·*/
// 	{ 
// 	 if(timer>=500)	  //100MS¼ì²âÆô¶¯¼ì²âÒ»´Î	 1000
// 	   {
// 	       timer=0;
// 		   StartModule(); //Æô¶¯¼ì²â
//            Conut();		  //¼ÆËã¾àÀë
//            if(S<20)		  //¾àÀëĞ¡ÓÚ20CM
// 		   {
// 		   stoprun();	  //Ğ¡³µÍ£Ö¹
// 		   COMM(); 		  //·½Ïòº¯Êı
// 		   }
// 		   else
// 		   if(S>40)		  //¾àÀë´óÓÚ£¬30CMÍùÇ°×ß
// 		   run();
// 	   }

// 	}
// 		stoprun();
// 	 {P2_1=1;P2_2=1;P2_3=1;}
// 	push_val_left=14;
// 	delay(100);
// 	ET1= 0;//¹Ø±Õ¶¨Ê±Æ÷1ÖĞ¶Ï
// 	EA=0;//¹Ø±Õ×ÜÖĞ¶Ï
// 	TR1=0;//Í£Ö¹¶¨Ê±Æ÷1
// 	P2_6=0;
// 		
// //µ¹¼ÆÊ±½áÊø£¬Ğ¡³µ´ò¿ª¿ªÊ¼Ñ°ÕÒ³äµçµã·µ»Ø³äµç
// 	//ĞèÒªÓÃµ½µÄ¹¦ÄÜÓĞ£º³¬Éù²â¾à£¬ºìÍâ½ÓÊÜ
// 	//Íâ²¿ÖĞ¶Ï1 ºÍ¶¨Ê±Æ÷1 ¶¨Ê±Æ÷0 Íâ²¿ÖĞ¶Ï1ÓÃÀ´¼ì²âºìÍâĞÅºÅ£¬¶¨Ê±Æ÷1ÓÃÀ´¼ÇÂ¼³¬Éù²¨µÄÊ±³¤ ¶¨Ê±Æ÷0ÓÃÀ´¹Ì¶¨¶¨Ê±
// 	//Íâ²¿ÖĞ¶Ï1ÖĞ¶ÏĞòºÅ2   ¶¨Ê±Æ÷1¶ÔÓ¦ÖĞ¶ÏĞòºÅ3 ¶¨Ê±Æ÷0¶ÔÓ¦ÖĞ¶ÏĞòºÅ1
// 	
// 	TMOD=0x11;//ÉèÖÃ¶¨Ê±Æ÷0ºÍ1Îª16bit¶¨Ê±Ä£Ê½
// 	TH1=0;
// 	TL1=0;
// 	IT1=1;//ÉèÖÃÍâ²¿ÖĞ¶Ï1Îª±ßÑØ´¥·¢
// 	EX1=1;//ÔÊĞíÍâ²¿ÖĞ¶Ï1ÖĞ¶Ï
// 	TH0=0;
// 	TL0=0;
// 	Im[4]='\0';
// 	EA = 1;//¿ªÆô×ÜÖĞ¶Ï
// 	TR1=1;//Æô¶¯¶¨Ê±Æ÷1
// 	//TR0=1;//Æô¶¯¶¨Ê±Æ÷0


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
// 		StartModule();	  //Æô¶¯³¬Éù²¨²â¾à
//     Conut();			  //¼ÆËã¾àÀë
// 		delay(100);
// 	}
// 	stoprun();
// 	

// }













