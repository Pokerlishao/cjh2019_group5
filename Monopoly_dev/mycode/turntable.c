/***********************************************************
	turntable.c 

	函数说明: 在起始位置画一个的转盘 ，按下空格开始旋转，按下回车停止旋转  ,并返回前进步数 
			转盘部分颜色可直接在  drawturntable 函数中更改； 
	调用接口:int DrawTurnTable(double turntable_x,double turntable_y,double turntable_radius);
			turntable_x：			转盘起始横坐标 
			turntable_y:			转盘起始纵坐标 
			turntable_radius：		转盘半径 
			
			键盘回调 void Keyboard_Turntable(int key,int event); 
	创建时间 :2019/5/8  22:27
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

#define step  6   //转盘产生随机数的最大值 
#define pi 3.14159265

//=========================================================================================================================================================================================================

static double nowangle = 0;
static double start_tablex;
static double start_tabley;
static double radius; //转盘半径 

int resultX = 0;
bool isResult = FALSE; 

//=========================================================================================================================================================================================================

static int randomturntable();//1到6随机数 
static void drawturntable(int result);//画转盘
static void turn(double angle);
static void forward(double distance);/*沿当前方向移动笔划线*/
static void move(double distance); /*沿当前方向移动笔不划线*/ 

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
	drawturntable(0);//擦除之前的	
    SetEraseMode(FALSE);
    turn(-5);
	drawturntable(0);//画新的 
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
						startTimer(1, 10);/*定时器触发，时间间隔为10ms*/  
						isBlink = TRUE;
						isResult = FALSE;
					}
                    break;
				case VK_RETURN:
					if (isBlink != TRUE)
					{
						cancelTimer(1);/*定时器取消*/ 
						SetEraseMode(TRUE);
						drawturntable(0);//擦除之前的						
						MovePen(start_tablex+5,start_tabley-3);
						SetPenSize(50);
						DrawLine(0,0); 
						SetEraseMode(FALSE);
						
						isBlink = FALSE;
						resultX = randomturntable();
						result_table[0] = resultX + '0';//更新结果 
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




//返回生成1到最大步长之间的随机整数 
static int randomturntable(){
	int number;
	srand((unsigned)time(NULL));
	number = rand()%step+1;
	return number;
}

/*********************************************
画转盘 
定时器调用的时候 传入参数为0，此时每次旋转5度，角度可在在定时器函数中更改 
输出结果时 调整转盘角度至结果需要的角度
**********************************************/
static void drawturntable(int result){
/*判断如果输出结果，调整转盘角度至结果需要的角度*/ 
	if(result!=0){
		nowangle = 0;
		turn(130 - 60 * result);
	}
	int i;
/*画圆*/ 
	MovePen(start_tablex,start_tabley);
	SetPenColor ("red");
	SetPenSize(3);
	StartFilledRegion(0.5);
	DrawArc(radius,0,360);
	EndFilledRegion();
/*画点数图案，每次画完转过60度*/
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
				turn(30);/*画一个点*/ 
				break;
			case 2:
				MovePen(start_tablex-radius,start_tabley);
				SetPenSize(10);	
				turn(20);
				move(radius/1.5);
				DrawLine(0,0);/*第一个点*/ 
				MovePen(start_tablex-radius,start_tabley);
				turn(20);
				move(radius/1.5);
				DrawLine(0,0);/*第二个点*/ 
				turn(20);
				break;
			case 3:
				MovePen(start_tablex-radius,start_tabley);
				SetPenSize(10);	
				turn(18);
				move(radius/2.2);
				DrawLine(0,0);/*第一个点*/ 
				MovePen(start_tablex-radius,start_tabley);
				turn(10);
				move(radius/1.5);
				DrawLine(0,0);/*第二个点*/ 
				MovePen(start_tablex-radius,start_tabley);
				turn(18);
				move(radius/2.2);
				DrawLine(0,0);/*第三个点*/			
				turn(14);
				break;
			case 4:
				MovePen(start_tablex-radius,start_tabley);
				SetPenSize(10);	
				turn(18);
				move(radius/1.5);
				DrawLine(0,0);/*第一个点*/ 
				MovePen(start_tablex-radius,start_tabley);
				turn(12);
				move(radius/2);
				DrawLine(0,0);/*第二个点*/ 
				move(radius/3);
				DrawLine(0,0);/*第三个点*/			
				MovePen(start_tablex-radius,start_tabley);
				turn(12);
				move(radius/1.5);
				DrawLine(0,0);/*第四个点*/			
				turn(18);
				break;
			case 5:
				MovePen(start_tablex-radius,start_tabley);
				SetPenSize(9);	
				turn(12);
				move(radius/1.7);
				DrawLine(0,0);/*第一个点*/ 
				MovePen(start_tablex-radius,start_tabley);
				turn(3);
				move(radius/1.3);
				DrawLine(0,0);/*第二个点*/ 
				MovePen(start_tablex-radius,start_tabley);				
				turn(15); 
				move(radius/1.5);
				DrawLine(0,0);/*第三个点*/			
				MovePen(start_tablex-radius,start_tabley);
				turn(10);
				move(radius/1.3);
				DrawLine(0,0);/*第四个点*/
				MovePen(start_tablex-radius,start_tabley);
				turn(5);
				move(radius/1.7);
				DrawLine(0,0);/*第五个点*/			
				turn(15);
				break;
			case 6:
				MovePen(start_tablex-radius,start_tabley);
				SetPenSize(10);	
				turn(15);
				move(radius/1.5);
				DrawLine(0,0);/*第一个点*/ 
				move(radius/4);
				DrawLine(0,0);/*第二个点*/	
				MovePen(start_tablex-radius,start_tabley);
				turn(15);
				move(radius/1.5);
				DrawLine(0,0);/*第三个点*/ 
				move(radius/4);
				DrawLine(0,0);/*第四个点*/			
				MovePen(start_tablex-radius,start_tabley);
				turn(15);
				move(radius/1.5);
				DrawLine(0,0);/*第五个点*/	
				move(radius/4);
				DrawLine(0,0);/*第六个点*/			
				turn(15);
				break;
		}
		MovePen(start_tablex-radius,start_tabley);		
	}
//画指针 
	MovePen(start_tablex-radius,start_tabley);
	SetPenSize(0);
	DrawLine(0,radius/2);

	SetPenColor ("purple");	//指针颜色 
	SetPenSize(radius*1.5);
	DrawLine(0,0);
	SetPenSize(radius/3);
	DrawLine(0,-radius/2);

	SetPenSize(radius*3);
	SetPenColor ("babypink");//转盘中心颜色 
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

