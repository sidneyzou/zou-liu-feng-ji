#include "control.h"
#include "motor.h"
#include "math.h"
#include "stmflash.h"


u16 moto1,moto2,moto3,moto4;

u8 ARMED;
PID PID_ROL,PID_PIT,PID_YAW;


void PID_Init(void)
{
	//Pֵ
	PID_ROL.P = 4.15;
	PID_PIT.P = PID_ROL.P;
	PID_YAW.P = 4.5;
	
	//Iֵ
	PID_ROL.I = 0.008;
	PID_PIT.I = PID_ROL.I;
	PID_YAW.I = PID_ROL.I;
	
	//IMAX
	PID_ROL.IMAX = 25;
	PID_PIT.IMAX = PID_ROL.IMAX;
	PID_YAW.IMAX = PID_ROL.IMAX;
	
	//Dֵ
	PID_ROL.D = 1.1;
	PID_PIT.D = PID_ROL.D;
	PID_YAW.D = PID_ROL.D;
	
	//PID_ReadFlash();
}

float Get_MxMi(float num,float max,float min)
{
	if(num>max)
		return max;
	else if(num<min)
		return min;
	else
		return num;
}

//����ת��Ϊ���ٶ�(�Ƕ�ÿ��)
float number_to_dps(s16 number)
{
	float temp;
	temp = (float)number*Gyro_Gain;
	return temp;
}

//����ת��Ϊ���ٶ�(����ÿ��)
float number_to_dps1(s16 number)
{
	float temp;
	temp = (float)number*Gyro_GainR;
	return temp;
}

//����ת��Ϊ���ٶ�
float number_to_g(s16 number)
{
	float temp;
	temp = (float)number*Acc_Gain;
	return temp;
}


void CONTROL(float rol_now, float pit_now, float yaw_now, u16 throttle, float rol_tar, float pit_tar, s16 yaw_gyro_tar)
{
	//static u8 thr_yaw_flag = 0;//ң��������YAW��־λ  1Ϊ��
	//static float temp_yaw = 0;//ÿ����ң��������YAW����ʱ���������ʱYAWֵ
	
	float rol_error = rol_now - rol_tar;//����ǶȲ�ֵ
	float pit_error = pit_now - pit_tar;
	
	float yaw_error = yaw_now - (yaw_gyro_tar/12);//YAW��ֵ��yaw_gyro_tar/12 = 0-30��
	float yaw_gyro_now = number_to_dps(0/*gyro[2]*/);//YAW��ĵ�ǰ���ٶ�
	
	
	//-----------------------Pitch\Roll��PID����-START-----------------------------
	PID_ROL.pout = PID_ROL.P * rol_error; //�����Pֵ
	PID_PIT.pout = PID_PIT.P * pit_error;
	
	//�����Iֵ
	PID_ROL.iout += PID_ROL.I * rol_error;
	PID_ROL.iout = Get_MxMi(PID_ROL.iout,PID_ROL.IMAX,-PID_ROL.IMAX);//�ж�I�Ƿ񳬳���Χ
	PID_PIT.iout += PID_PIT.I * pit_error;
	PID_PIT.iout = Get_MxMi(PID_PIT.iout,PID_PIT.IMAX,-PID_PIT.IMAX);
	if(throttle<1100)//������С��1100ʱ��Iֵ����
	{
		PID_ROL.iout = 0;
		PID_PIT.iout = 0;
	}

	PID_ROL.dout = PID_ROL.D * number_to_dps(0/*gyro[0]*/);//�����Dֵ����ң�����������޹أ�
	PID_PIT.dout = PID_PIT.D * number_to_dps(0/*gyro[1]*/);
	//-----------------------Pitch\Roll��PID����-END-------------------------------
	
	
	//-------------------YAW��PID����-START-----------------------------
	if(yaw_gyro_tar>=-5 && yaw_gyro_tar<= 5)//YAWҡ����������
	{
		yaw_gyro_tar = 0;
	}
	else
	{
		//Q_ANGLE.YAW = 0;//YAW�Ƕ�����
		
		//YAW�Ƕȱ��ֲ���(IMUupdate��)
	}
	
	PID_YAW.pout = PID_YAW.P * yaw_error;//P
	PID_YAW.iout = 0;//I
	PID_YAW.dout = PID_YAW.D * (yaw_gyro_now-yaw_gyro_tar);//D ��ֹYAW��ת��
	//---------------------YAW��PID����-END-----------------------------
	
	
	PID_ROL.OUT = PID_ROL.pout + PID_ROL.iout + PID_ROL.dout;////PIDֵ���
	PID_PIT.OUT = PID_PIT.pout + PID_PIT.iout + PID_PIT.dout;
	PID_YAW.OUT = PID_YAW.pout + PID_YAW.iout + PID_YAW.dout;
	
	
	if(ARMED == 1)
	{
		if(throttle>=1100)//�����Ž��н����ж�<1100�����ŵ����ת������������(Ϊ�˰�ȫ����Ҫ������)
		{
			moto1 = throttle + PID_ROL.OUT - PID_PIT.OUT + PID_YAW.OUT;  moto1 = Get_MxMi(moto1,2000,1000);
			moto2 = throttle - PID_ROL.OUT - PID_PIT.OUT - PID_YAW.OUT;  moto2 = Get_MxMi(moto2,2000,1000);
			moto3 = throttle - PID_ROL.OUT + PID_PIT.OUT + PID_YAW.OUT;  moto3 = Get_MxMi(moto3,2000,1000);
			moto4 = throttle + PID_ROL.OUT + PID_PIT.OUT - PID_YAW.OUT;  moto4 = Get_MxMi(moto4,2000,1000);
			
			Set_Motor(moto1,moto2,moto3,moto4);//���PWM���
		}
		else
		{
			moto1 =1000;moto2 =1000;moto3 =1000;moto4 =1000;
			Set_Motor(moto1,moto2,moto3,moto4);//���PWM���
		}
	}
	else
	{
		moto1 =1000;moto2 =1000;moto3 =1000;moto4 =1000;
		Set_Motor(moto1,moto2,moto3,moto4);//���PWM���
	}
	
}




//����
u8 Is_Armed(u16 CH3,u16 CH4)//������С CH4��С
{
	if( (CH3<1100 && CH3>900) && (CH4<2100 && CH4>1900) )//�Ƿ�����������
	{
		ARMED = 1;
		return 1;//�����ɹ�
	}
	else 
	{
		return 0;//û�н���
	}
}

//����
u8 Is_DisArmed(u16 CH3,u16 CH4)//������С CH4���
{
	if( (CH3<1100 && CH3>900) && (CH4<1100 && CH4>900) )
	{
		ARMED = 0;
		return 1;//�����ɹ�
	}
	else
	{
		return 0;//û�м���
	}
}


//д��PIDֵ
void PID_WriteFlash(void)
{
	u8 point=29;
	u16 flash_data[39];
	
	//����Flash����
	STMFLASH_Read(PAGE1,flash_data,39);
	
	//��������д��PID���ݣ�
	flash_data[point++] = ADJUST_FLAG;
	
	flash_data[point++] = (u16)(PID_ROL.P*1000);
	flash_data[point++] = (u16)(PID_ROL.I*1000);
	flash_data[point++] = (u16)(PID_ROL.D*1000);
	
	flash_data[point++] = (u16)(PID_PIT.P*1000);
	flash_data[point++] = (u16)(PID_PIT.I*1000);
	flash_data[point++] = (u16)(PID_PIT.D*1000);
	
	flash_data[point++] = (u16)(PID_YAW.P*1000);
	flash_data[point++] = (u16)(PID_YAW.I*1000);
	flash_data[point++] = (u16)(PID_YAW.D*1000);
	
	//д��Flash����
	STMFLASH_Write(PAGE1,flash_data,39);
	
}

//��ȡFlash��ĵ�PIDֵ
void PID_ReadFlash(void)
{
	u8 point=29;
	u16 flash_data[39];
	
	//����Flash����
	STMFLASH_Read(PAGE1,flash_data,39);
	
	if(flash_data[point++] == ADJUST_FLAG)//�ж�PID�Ƿ��Ѿ�����У׼
	{
		//������ǰPIDֵ
		PID_ROL.P = (float)flash_data[point++]/1000;
		PID_ROL.I = (float)flash_data[point++]/1000;
		PID_ROL.D = (float)flash_data[point++]/1000;
		
		PID_PIT.P = (float)flash_data[point++]/1000;
		PID_PIT.I = (float)flash_data[point++]/1000;
		PID_PIT.D = (float)flash_data[point++]/1000;
		
		PID_YAW.P = (float)flash_data[point++]/1000;
		PID_YAW.I = (float)flash_data[point++]/1000;
		PID_YAW.D = (float)flash_data[point++]/1000;
	}
	else
	{
		//д�뵱ǰPIDֵ
		PID_WriteFlash();
	}
}








