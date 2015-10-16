#include "work.h"
#include "bsp_key.h" 
#include "OLED_display.h"
#include "OLED_I2C.h"
#include "motor.h"
#include "imu.h"
#include "led.h"
#include "math.h"
#include "delay.h"


u8 mode;
u8 mode_ok,Readkey,flag_ok=0;
u8 lenth=30,R=15,angle=0;
u8 data_add=0;
float Kp=2,Ki=0.01,Kd=0,pit_Kp=50,pit_Kd=0;
float hight_increment;
int16_t mot1=1000,mot2=1000,mot3=1000,mot4=1000;


/////////////
//Run_mode1()变量
//////////
void Run_mode1()
{
	
}



float errgyro=0,targyro,errgyroold;
float rollmax;
float errpitold,errpit,tarpit;
int32_t PITOUT,GRYOOUT;
u8 flag_qidong=1;

u8 a=1;
void Run_mode2()
{
	LED1(0);
	
	if(flag_qidong)
	{
		targyro=atanf(((float)lenth/176))*57.296;
		mot4=targyro*10+10;
		mot3=lenth;
		flag_qidong=0;
		Set_Motor(1500,1000,1000,1000);
		delay_ms(1000);
		while(a)
		{
			Read_mpu();
			errpit=Pitch-tarpit;
			PITOUT=Kp*errpit+Kd*(errpit-errpitold)/*+Ki*hight_increment*/;	
			errpitold=errpit;
			mot3=1052-PITOUT;
			mot4=1058+PITOUT;
			
			if(gyro[0]>0)
			{
				mot1=1000;
				mot2=1200;

			}
			else  
			{
				mot1=1200;
				mot2=1000;
					
			}
			if(Roll>(targyro-1))  a=0;
			Set_Motor(mot1,mot2,mot3,mot4);
		}
	}
	
	
	errpit=Pitch-tarpit;
	PITOUT=Kp*errpit+Kd*(errpit-errpitold)/*+Ki*hight_increment*/;	
	errpitold=errpit;
	mot3=1052-PITOUT;
	mot4=1058+PITOUT;
	
	if(rollmax)
	{
		rollmax=fabs(rollmax);
		errgyro=rollmax-targyro;
		GRYOOUT=Kp*errgyro;//+Kd*(errgyro-errgyroold)+Ki*hight_increment;	
		errgyroold=errgyro;	
		rollmax=0;		
	}
	
	if(gyro[0]>0)
	{		
			if(GRYOOUT>0)  GRYOOUT=58;
			mot1=1000;
		  mot2=1058-GRYOOUT;  if(mot2>1300) mot2=1300;
			Set_Motor(mot1,mot2,mot3,mot4);	
		//	Set_Motor(mot1,mot2,1000,1000);	
	}
	else if(gyro[0]<0)
	{
		if(GRYOOUT>0)  GRYOOUT=58;
		mot1=1052-GRYOOUT;  if(mot1>1300) mot1=1300;
		mot2=1000;
		Set_Motor(mot1,mot2,mot3,mot4);	
		//Set_Motor(mot1,mot2,1000,1000);			
	}
	else Set_Motor(1000,1000,mot3,mot4);	
		//Set_Motor(mot1,mot2,1000,1000);	
}
short gyro_[3];
float Pitch_,Roll_;
float goal_angle;
void Run_mode3()
{
	gyro_[0]=gyro[0]*cosf(goal_angle*3.14/180)+gyro[1]*sinf(goal_angle*3.14/180);//
	gyro_[1]=gyro[0]*sinf(goal_angle*3.14/180)-gyro[1]*cosf(goal_angle*3.14/180);//

//	Pitch_=atanf(sqrt(pow( Roll,2)+pow(Pitch ,2)));
//	mot1=Pitch_*100;
	if(flag_qidong)
	{
		if(angle<90)
		{
		goal_angle=angle;
		mot1=1052+300*sinf(goal_angle*3.14/180);   
		mot2=1000;
		mot3=1052+300*cosf(goal_angle*3.14/180);
		mot4=1000;;
		}
		else
		{			
			goal_angle=angle-90;
			mot1=1052+300*cosf(goal_angle*3.14/180);   
			mot2=1000;
			mot3=1000;
			mot4=1000+300*sinf(goal_angle*3.14/180);
		}
		Set_Motor(mot1,mot2,mot3,mot4);	
	//	while(1);
		delay_ms(500);
		flag_qidong=0;
	}
	
	if(angle<90)
	{
		if(gyro_[0]<0)
		{		
				
				mot1=1052+100*sinf(goal_angle*3.14/180);   
				mot2=1000;
				mot3=1100+100*cosf(goal_angle*3.14/180);
				mot4=1000;
				Set_Motor(mot1,mot2,mot3,mot4);	
		}
		else 
		{
			Set_Motor(1000,1000,1000,1000);		
		}
	}
	else 
	{
		if(gyro_[1]<0)
		{		
				
				mot1=1070+150*cosf(goal_angle*3.14/180);   
				mot2=1000;
				mot3=1000;
				mot4=1070+150*sinf(goal_angle*3.14/180);
				Set_Motor(mot1,mot2,mot3,mot4);	
		}
		else 
		{
			Set_Motor(1000,1000,1000,1000);		
		}
	}
	mot1=gyro_[1];
	mot2=gyro_[0];

}


float rol_error,rol_tar;
float  pit_error, pit_tar;
float ROL_P,ROL_D=30,ROL_I,PIT_P=0,PIT_I,PIT_D=23;
float ROL_pout,ROL_dout,ROL_iout,PIT_pout,PIT_dout,PIT_iout;
float PID_ROLLOUT,PID_PITOUT;
char m,n;
void Run_mode4()
{
	
   rol_error = Roll - rol_tar;//
	 pit_error = Pitch - pit_tar;


	ROL_pout = ROL_P * rol_error; 
	PIT_pout = PIT_P * pit_error;
	
	if(gyro[0]<10&&gyro[0]>(-10)) 
		m++;
	if(m<=6)
	{
		ROL_P=0;//3;
		ROL_D=8;
	}
	else
	{
		ROL_P=0;
		ROL_D=30;
	}
	
		if(gyro[1]<10&&gyro[1]>(-10)) 
		n++;
	if(n<=6)
	{
		PIT_P=0;//3;
		PIT_D=8;
	}
	else
	{
		PIT_P=0;
		PIT_D=35;
	}
//	ROL_iout += ROL_I * rol_error;
////	ROL_iout = Get_MxMi(ROL.iout,PID_ROL.IMAX,-PID_ROL.IMAX)
//	PIT_iout += PIT_I * pit_error;
////	PIT_iout = Get_MxMi(PIT.iout,PID_PIT.IMAX,-PID_PIT.IMAX);
//	if(0)
//	{
//		ROL_iout = 0;
//		PIT_iout = 0;
//	}

	ROL_dout = ROL_D * gyro[0]/16.4;//???D?(?????????)
	PIT_dout = PIT_D * gyro[1]/16.4;
	//-----------------------Pitch\Roll?PID??-END-------------------------------
	
	
	
	PID_ROLLOUT = ROL_pout + ROL_iout + ROL_dout;////PID???  0.3
	PID_PITOUT  = PIT_pout + PIT_iout + PIT_dout;

		mot1=PID_ROLLOUT+1052;
		mot2=-PID_ROLLOUT+1058;
	
		mot3=PID_PITOUT +1052;
		mot4=-PID_PITOUT+1058;
	
	if(Roll>-1&&Roll<1)
	{
		mot1=1000;
		mot2=1000;
	}
	
	if(Pitch>-1&&Pitch<1)
	{
		mot3=1000;
		mot4=1000;
	}
	
		Set_Motor(mot1,mot2,mot3,mot4);	
}

void Run_mode5()
{
	
}

void Run_mode6()
{
	
}

void Run_mode7()
{
	
}

void RUN()
{
	switch (mode)
	{
		case 0:Run_mode1();break;      //第一步任务：不短于50cm直线段
		case 1:Run_mode2();break;			 //第二部任务：固定方向30-60cm直线
		case 2:Run_mode3();break;			 //第三部任务：任意方向，大于20cm直线
		case 3:Run_mode4();break;			 //第四步任务：拉起角度，可以达到静止
		case 4:break;			 //第五步任务: 重复三次可设置半径15-35的圆环内
		case 5:break;			 //
		case 6:break;
	}
}

u8 Get_MxMi1(u8 num,u8 max,u8 min)
{
	if(num>max)
		return max;
	else if(num<min)
		return min;
	else
		return num;
}

// ok  nest  +1   -1   +10    -10
// 4		5			3		6		  1      2

void Get_mode()
{
	while(!mode_ok)
	{
		Readkey=Read_Key();
		if(Readkey!=0)
		{
			if(Readkey==4) {flag_ok++;}
			if(Readkey==5) 
			{
				clear();
				mode++;
				//data_add=0;
				flag_ok=0;
				if(mode==7) mode=0;
			}
			
			Oled_Display();
			
			if(flag_ok==1)
			{
				if(Readkey==3) data_add++;
				else if(Readkey==6) data_add--;
				else if(Readkey==1) data_add+=10;
				else if(Readkey==2) data_add-=10;
				
				switch (mode)
				{
					case 1:lenth+=data_add; lenth=Get_MxMi1(lenth,60,30); mode2_Display(lenth); break;     
					case 2:angle+=data_add; angle=Get_MxMi1(angle,180,0); mode3_Display(angle); break;			
					case 4:R+=data_add;			R    =Get_MxMi1(R    ,35,15); mode5_Display(R);     break;			
					default:clear();break;										
				}
				data_add=0;	
			}
			else if(flag_ok==2) 
			{
				OLED_ShowStr(16,0,"   Runing   ",2);
				mode_ok=1;
			}
				
		}
	}
}
