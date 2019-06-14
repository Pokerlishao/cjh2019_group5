/***********************************************************
	turntable.c 

	����˵��: ����ʼλ�û�һ����ת�� �����¿ո�ʼ��ת�����»س�ֹͣ��ת  ,������ǰ������ 
			ת�̲�����ɫ��ֱ����  drawturntable �����и��ģ� 
	���ýӿ�:int DrawTurnTable(double turntable_x,double turntable_y,double turntable_radius);
			turntable_x��			ת����ʼ������ 
			turntable_y:			ת����ʼ������ 
			turntable_radius��		ת�̰뾶 
			
			���̻ص� void Keyboard_Turntable(int key,int event); 
	����ʱ�� :2019/5/8  22:27
	Last :2019/5/10 16:46
		  2019/5/21 17:12
		  2019/5/28 19:16 
	Copyright ? 2019  Group_5
*************************************************************/

#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "strlib.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include "time.h"
#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include <string.h>

#include"turntable.h"
#include"Map1_demo.h"

#define step  6   //ת�̲�������������ֵ 
#define pi 3.14159265

//=========================================================================================================================================================================================================

static double nowangle = 0;
static double start_tablex;
static double start_tabley;
static double radius; //ת�̰뾶 

int resultX = 0;
bool isResult = FALSE; 

//=========================================================================================================================================================================================================

static int randomturntable();//1��6����� 
static void drawturntable(int result);//��ת��
static void turn(double angle);
static void forward(double distance);/*�ص�ǰ�����ƶ��ʻ���*/
static void move(double distance); /*�ص�ǰ�����ƶ��ʲ�����*/ 

//=========================================================================================================================================================================================================


void DrawTurnTable(double turntable_x,double turntable_y,double turntable_radius){
	start_tablex = turntable_x;
	start_tabley = turntable_y;
	radius = turntable_radius;
	if (!resultX)
		drawturntable(1);
	else
	{
		char result_table[2] = { '\0','\0' };
		result_table[0] = resultX + '0';
		drawturntable(resultX);
		MovePen(start_tablex + 5, start_tabley - 3);
		SetPenSize(10);
		SetPenColor("blue");
		DrawTextString(result_table);
	}
}


void Timer_Turntable(){	
    SetEraseMode(TRUE);
	drawturntable(0);//����֮ǰ��	
    SetEraseMode(FALSE);
    turn(-5);
	drawturntable(0);//���µ� 
}

void Keyboard_Turntable(int key,int event){

	char result_table[2] = {'\0','\0'};

	bool isBlink=FALSE;
	
	switch (event) {
	 	case KEY_DOWN:
			 switch (key){
			 	case VK_SPACE:
					if (isBlink == FALSE)
					{
						drawturntable(0);
						startTimer(1, 10);/*��ʱ��������ʱ����Ϊ10ms*/  
						isBlink = TRUE;
						isResult = FALSE;
					}
                    break;
				case VK_RETURN:
					if (isBlink != TRUE)
					{
						cancelTimer(1);/*��ʱ��ȡ��*/ 
						SetEraseMode(TRUE);
						drawturntable(0);//����֮ǰ��						
						MovePen(start_tablex+5,start_tabley-3);
						SetPenSize(50);
						DrawLine(0,0); 
						SetEraseMode(FALSE);
						
						isBlink = FALSE;
						resultX = randomturntable();
						result_table[0] = resultX + '0';//���½�� 
						drawturntable(resultX);
						MovePen(start_tablex + 5, start_tabley - 3);
						SetPenSize(10);
						SetPenColor("blue");
						DrawTextString(result_table);
						begingame(); 
						
						isResult = TRUE;
					}
					break;
			}
			break;
	}
}




//��������1����󲽳�֮���������� 
static int randomturntable(){
	int number;
	srand((unsigned)time(NULL));
	number = rand()%step+1;
	return number;
}

/*********************************************
��ת�� 
��ʱ�����õ�ʱ�� �������Ϊ0����ʱÿ����ת5�ȣ��Ƕȿ����ڶ�ʱ�������и��� 
������ʱ ����ת�̽Ƕ��������Ҫ�ĽǶ�
**********************************************/
static void drawturntable(int result){
/*�ж����������������ת�̽Ƕ��������Ҫ�ĽǶ�*/ 
	if(result!=0){
		nowangle = 0;
		turn(130 - 60 * result);
	}
	int i;
/*��Բ*/ 
	MovePen(start_tablex,start_tabley);
	SetPenColor ("red");
	SetPenSize(3);
	StartFilledRegion(0.5);
	DrawArc(radius,0,360);
	EndFilledRegion();
/*������ͼ����ÿ�λ���ת��60��*/
	MovePen(start_tablex-radius,start_tabley);
	SetPenSize(3);	
	for	(i = 1;i<7;i++){
		SetPenColor ("grass");
		SetPenSize(3);
		forward(radius);
			
		SetPenColor ("black");
		switch (i){
			case 1:
				MovePen(start_tablex-radius,start_tabley);
				SetPenSize(15);
				turn(30);
				move(radius/1.5);
				DrawLine(0,0);
				move(-radius/1.5);
				turn(30);/*��һ����*/ 
				break;
			case 2:
				MovePen(start_tablex-radius,start_tabley);
				SetPenSize(10);	
				turn(20);
				move(radius/1.5);
				DrawLine(0,0);/*��һ����*/ 
				MovePen(start_tablex-radius,start_tabley);
				turn(20);
				move(radius/1.5);
				DrawLine(0,0);/*�ڶ�����*/ 
				turn(20);
				break;
			case 3:
				MovePen(start_tablex-radius,start_tabley);
				SetPenSize(10);	
				turn(18);
				move(radius/2.2);
				DrawLine(0,0);/*��һ����*/ 
				MovePen(start_tablex-radius,start_tabley);
				turn(10);
				move(radius/1.5);
				DrawLine(0,0);/*�ڶ�����*/ 
				MovePen(start_tablex-radius,start_tabley);
				turn(18);
				move(radius/2.2);
				DrawLine(0,0);/*��������*/			
				turn(14);
				break;
			case 4:
				MovePen(start_tablex-radius,start_tabley);
				SetPenSize(10);	
				turn(18);
				move(radius/1.5);
				DrawLine(0,0);/*��һ����*/ 
				MovePen(start_tablex-radius,start_tabley);
				turn(12);
				move(radius/2);
				DrawLine(0,0);/*�ڶ�����*/ 
				move(radius/3);
				DrawLine(0,0);/*��������*/			
				MovePen(start_tablex-radius,start_tabley);
				turn(12);
				move(radius/1.5);
				DrawLine(0,0);/*���ĸ���*/			
				turn(18);
				break;
			case 5:
				MovePen(start_tablex-radius,start_tabley);
				SetPenSize(9);	
				turn(12);
				move(radius/1.7);
				DrawLine(0,0);/*��һ����*/ 
				MovePen(start_tablex-radius,start_tabley);
				turn(3);
				move(radius/1.3);
				DrawLine(0,0);/*�ڶ�����*/ 
				MovePen(start_tablex-radius,start_tabley);				
				turn(15); 
				move(radius/1.5);
				DrawLine(0,0);/*��������*/			
				MovePen(start_tablex-radius,start_tabley);
				turn(10);
				move(radius/1.3);
				DrawLine(0,0);/*���ĸ���*/
				MovePen(start_tablex-radius,start_tabley);
				turn(5);
				move(radius/1.7);
				DrawLine(0,0);/*�������*/			
				turn(15);
				break;
			case 6:
				MovePen(start_tablex-radius,start_tabley);
				SetPenSize(10);	
				turn(15);
				move(radius/1.5);
				DrawLine(0,0);/*��һ����*/ 
				move(radius/4);
				DrawLine(0,0);/*�ڶ�����*/	
				MovePen(start_tablex-radius,start_tabley);
				turn(15);
				move(radius/1.5);
				DrawLine(0,0);/*��������*/ 
				move(radius/4);
				DrawLine(0,0);/*���ĸ���*/			
				MovePen(start_tablex-radius,start_tabley);
				turn(15);
				move(radius/1.5);
				DrawLine(0,0);/*�������*/	
				move(radius/4);
				DrawLine(0,0);/*��������*/			
				turn(15);
				break;
		}
		MovePen(start_tablex-radius,start_tabley);		
	}
//��ָ�� 
	MovePen(start_tablex-radius,start_tabley);
	SetPenSize(0);
	DrawLine(0,radius/2);

	SetPenColor ("purple");	//ָ����ɫ 
	SetPenSize(radius*1.5);
	DrawLine(0,0);
	SetPenSize(radius/3);
	DrawLine(0,-radius/2);

	SetPenSize(radius*3);
	SetPenColor ("babypink");//ת��������ɫ 
	DrawLine(0,0);

	MovePen(start_tablex,start_tabley);
	SetPenColor ("grass");
	SetPenSize(4);
	DrawArc(radius,0,360);
}


static void forward(double distance){
	DrawLine(distance*cos(nowangle),distance*sin(nowangle));
}

static void turn (double angle){
	nowangle += angle / 180 * pi;
}

static void move(double distance){
	double now_x = GetCurrentX();
	double now_y = GetCurrentY();
	MovePen(now_x + distance * cos(nowangle),now_y + distance*sin(nowangle));	
}

