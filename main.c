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
//外部中断1解码程序
  void intersvr1(void) interrupt 2 using 1 //外部中断1
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


/************************************************************************/
//数码管显示
/************************************************************************/
unsigned char const discode[] ={ 0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xBF,0xff/*-*/};
unsigned char const positon[4]={ 0xfe,0xfd,0xfb,0xf7};
unsigned char disbuff[4]	   ={ 0,0,0,0,};

/********************************************************/
unsigned char posit=0;
    void Display_time(void)				 //数码管扫描
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
//舵机
/************************************************************************/
	#define Sevro_moto_pwm     P2_7	   //接舵机信号端输入PWM信号调节速度
  unsigned char pwm_val_left  = 0;//变量定义
	unsigned char push_val_left =14;//舵机归中，产生约，1.5MS 信�
    unsigned int S=0;
	unsigned int S1=0;
	unsigned int S2=0;
	unsigned int S3=0;
	unsigned int S4=0;
	unsigned int  time=0;		    //时间变量
	unsigned int  timer=0;			//延时基准变量
	unsigned char timer1=0;			//扫描时间变量	


/************************************************************************/
/*                    PWM调制电机转速                                   */
/************************************************************************/
/*                    左电机调速                                        */
/*调节push_val_left的值改变电机转速,占空比            */
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
//超声波
/************************************************************************/
	#define  ECHO  P2_4				   //超声波接口定义
	#define  TRIG  P2_5				   //超声波接口定义

/************************************************************************/
     void  StartModule() 		      //启动测距信号
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
	  void Conut(void)		   //计算距离
	{
		TH0=0;
		TL0=0;
	  while(!ECHO);		       //当RX为零时等待
	  TR0=1;			       //开启计数
	  while(ECHO);			   //当RX为1计数并等待
	  TR0=0;				   //关闭计数
	  time=TH0*256+TL0;		   //读取脉宽长度
	  TH0=0;
	  TL0=0;
	  S=(time*1.7)/100;        //算出来是CM
	  //disbuff[0]=S%1000/100;   //更新显示
	 // disbuff[1]=S%1000%100/10;
	  //disbuff[2]=S%1000%10 %10;
	}
/************************************************************************/

/***************************************************/
	 void Display_length(void)				  //扫描数码管
	{
	 if(posit==0)
	 {P0=(discode[disbuff[posit]])&0x7f;}//产生点
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

	
	
	
	//定时器1中断
 	void time1()interrupt 3   using 2
{	
   TH1=(65536-100)/256;	  //100US定时
	 TL1=(65536-100)%256;
	 timer++;				  //定时器100US为准。在这个基础上延时
	 pwm_val_left++;
	 pwm_Servomoto();

	 timer1++;				 //2MS扫一次数码管
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
 void  COMM( void ) 	//方向函数	      
  {
  	     
		 
		  push_val_left=9;	  //舵机向左转90度
		  timer=0;
		  while(timer<=2000); //延时400MS让舵机转到其位置		 4000
		  StartModule();	  //启动超声波测距
          Conut();	 		  //计算距离
		  S2=S;  
  
		  push_val_left=19;	  //舵机向右转90度
		  timer=0;
		  while(timer<=2000); //延时400MS让舵机转到其位置
		  StartModule();	  //启动超声波测距
          Conut();			  //计算距离
		  S4=S; 	
	

		  push_val_left=14;	  //舵机归中
		  timer=0;
		  while(timer<=2000); //延时400MS让舵机转到其位置

		  StartModule();	  //启动超声波测距
          Conut();			  //计算距离
		  S1=S; 	

          if((S2<40)||(S4<40)) //只要左右各有距离小于，20CM小车后退
		  {
		  backrun();		   //后退
		  timer=0;
		  while(timer<=1000);
		  }
		   
		  if(S2>S4)		 
		     {
				rightrun();  	//车的左边比车的右边距离小	右转	
		        timer=0;
		        while(timer<=1000);
		     }				      
		       else
		     {
		       leftrun();		//车的左边比车的右边距离大	左转
		       timer=0;
		       while(timer<=1000);
		     }
		  			   

}





void length_count(void)
{
	EX1=0;
	TH1=0;
	TL1=0;
	while(!ECHO);		       //当RX为零时等待
	TR1=1;
	while(ECHO);			   //当RX为1计数并等待
	TR1=0;				   //关闭计数
	time=TH1*256+TL1;		   //读取脉宽长度
  S=(time*1.7)/100;        //算出来是CM
	TH1=0;
	TL1=0;
	m=0;
	f=0;
	EX1=1;
	TR1=1;
}






void main(void){
//系统处于红外接受状态，等待接收遥控信号启动工作
	//需要用到的功能有：红外接受解码,外部中断1和计数器1
	TMOD=0x01;//设置定时器1为16bit模式

	IT1=1;//设置外部中断1为边沿触发
	EX1=1;//允许外部中断1中断
	EA=1;//开启总中断
	TR0=1;//启动定时器1
	P2_6=0;
	while(stage_flag==0);
	EA=0;//关闭总中断
	EX1=0;//禁止外部中断1
	TR0=0;//关闭定时器1
	P2_6=1;
//系统开始工作，超声波打开避障，并倒计时1分钟
	//需要用到的功能有：超声波避障，数码管显示
//
	TMOD=0x11;//设置定时器0和定时器1为16bit模式
	TH1=(65536-100)/256;	  //100US定时
	TL1=(65536-100)%256;
	EX1=1;
	ET1= 1;//允许定时器1中断
	EA = 1;//开启总中断
	TR1= 1;//启动定时器1
	TR0=1;
	push_val_left=14;
	delay(100);
	while(stage_flag==1);
	while(stage_flag==2)		       /*无限循环*/
	{ 
	 if(timer>=500)	  //100MS检测启动检测一次	 1000
	   {
	       timer=0;
		   StartModule(); //启动检测
           Conut();		  //计算距离
           if(S<20)		  //距离小于20CM
		   {
		   stoprun();	  //小车停止
		   COMM(); 		  //方向函数
		   }
		   else
		   if(S>40)		  //距离大于，30CM往前走
		   run();
	   }

	}
		stoprun();
	 {P2_1=1;P2_2=1;P2_3=1;}
	push_val_left=14;
	delay(100);
	ET1= 0;//关闭定时器1中断
	EA=0;//关闭总中断
	TR1=0;//停止定时器1
	P2_6=0;
		
//倒计时结束，小车打开开始寻找充电点返回充电
	//需要用到的功能有：超声测距，红外接受
	//外部中断1 和定时器1 定时器0 外部中断1用来检测红外信号，定时器1用来记录超声波的时长 定时器0用来固定定时
	//外部中断1中断序号2   定时器1对应中断序号3 定时器0对应中断序号1
	
	TMOD=0x11;//设置定时器0和1为16bit定时模式
	TH1=0;
	TL1=0;
	IT1=1;//设置外部中断1为边沿触发
	EX1=1;//允许外部中断1中断
	TH0=0;
	TL0=0;
	Im[4]='\0';
	EA = 1;//开启总中断
	TR1=1;//启动定时器1
	//TR0=1;//启动定时器0


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
		StartModule();	  //启动超声波测距
    Conut();			  //计算距离
		delay(100);
	}
	stoprun();
	

}





// void main(void){
// //系统处于红外接受状态，等待接收遥控信号启动工作
// 	//需要用到的功能有：红外接受解码,外部中断1和计数器1
// 	TMOD=0x10;//设置定时器1为16bit模式

// 	IT1=1;//设置外部中断1为边沿触发
// 	EX1=1;//允许外部中断1中断
// 	EA=1;//开启总中断
// 	TR1=1;//启动定时器1
// 	P2_6=0;
// 	while(stage_flag==0);
// 	EA=0;//关闭总中断
// 	EX1=0;//禁止外部中断1
// 	TR1=0;//关闭定时器1
// 	P2_6=1;
// //系统开始工作，超声波打开避障，并倒计时1分钟
// 	//需要用到的功能有：超声波避障，数码管显示
// //
// 	TMOD=0x11;//设置定时器0和定时器1为16bit模式
// 	TH1=(65536-100)/256;	  //100US定时
// 	TL1=(65536-100)%256;
// 	ET1= 1;//允许定时器1中断
// 	EA = 1;//开启总中断
// 	TR1= 1;//启动定时器1

// 	push_val_left=14;
// 	delay(100);
// 	while(stage_flag==1);
// 	while(stage_flag==2)		       /*无限循环*/
// 	{ 
// 	 if(timer>=500)	  //100MS检测启动检测一次	 1000
// 	   {
// 	       timer=0;
// 		   StartModule(); //启动检测
//            Conut();		  //计算距离
//            if(S<20)		  //距离小于20CM
// 		   {
// 		   stoprun();	  //小车停止
// 		   COMM(); 		  //方向函数
// 		   }
// 		   else
// 		   if(S>40)		  //距离大于，30CM往前走
// 		   run();
// 	   }

// 	}
// 		stoprun();
// 	 {P2_1=1;P2_2=1;P2_3=1;}
// 	push_val_left=14;
// 	delay(100);
// 	ET1= 0;//关闭定时器1中断
// 	EA=0;//关闭总中断
// 	TR1=0;//停止定时器1
// 	P2_6=0;
// 		
// //倒计时结束，小车打开开始寻找充电点返回充电
// 	//需要用到的功能有：超声测距，红外接受
// 	//外部中断1 和定时器1 定时器0 外部中断1用来检测红外信号，定时器1用来记录超声波的时长 定时器0用来固定定时
// 	//外部中断1中断序号2   定时器1对应中断序号3 定时器0对应中断序号1
// 	
// 	TMOD=0x11;//设置定时器0和1为16bit定时模式
// 	TH1=0;
// 	TL1=0;
// 	IT1=1;//设置外部中断1为边沿触发
// 	EX1=1;//允许外部中断1中断
// 	TH0=0;
// 	TL0=0;
// 	Im[4]='\0';
// 	EA = 1;//开启总中断
// 	TR1=1;//启动定时器1
// 	//TR0=1;//启动定时器0


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
// 		StartModule();	  //启动超声波测距
//     Conut();			  //计算距离
// 		delay(100);
// 	}
// 	stoprun();
// 	

// }













