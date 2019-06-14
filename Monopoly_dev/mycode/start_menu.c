/***********************************************************
	start_menu.c 
	
	函数说明: 画开始界面 ,并返回四个按键的数值
			返回0 为开始键
			返回1 为BUTTON_1 
			返回2 为BUTTON_2
			返回3 为BUTTON_3 
	调用接口:int start_menu(); 
			int Mouse_StartMenu(int x, int y, int button, int event); 鼠标调用	
	
	BUG:目前没有办法停止鼠标函数...导致这个函数会一直运行，不返回结果 
	创建时间 :2019/5/12  0:43
	Copyright ? 2019  Group_5
*************************************************************/

#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "random.h"
#include "strlib.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <windows.h>
#include <olectl.h>
#include <stdio.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include <math.h>

#include "start_menu.h"

#define start_side 23
#define pi 3.14159265

//=========================================================================================================================================================================================================

int start_button = -1;

static double start_x=80;
static double start_y=55;

static double nowangle;
static double screen_height;
static double screen_width;
//=========================================================================================================================================================================================================

static void turn(double angle);/*转动角度*/
static void forward(double distance);/*沿当前方向移动笔划线*/
static void move(double distance); /*沿当前方向移动笔不划线*/ 

static void DrawMenu();/*画菜单*/ 
static void background();/*背景颜色*/ 
static void button_start(int mode);/*画开始按键*/ 
static void button_1(int mode);/*画BUTTON_1*/ 
static void button_2(int mode);/*画BUTTON_2*/ 
static void button_3(int mode);/*画BUTTON_3*/

//=========================================================================================================================================================================================================

/*画主菜单*/
void start_menu()
{
	screen_height = GetWindowHeight();/*获取屏幕高度*/ 
	screen_width = GetWindowWidth();/*获取屏幕宽度*/ 
    DrawMenu();
}

/*画主菜单*/
static void DrawMenu(){
	background();	
	button_start(0);
	button_1(0);
 	button_2(0);
	button_3(0);/*画按钮*/ 
}

/*鼠标信息*/
int Mouse_StartMenu(int x, int y, int button, int event){

    double mouse_x, mouse_y;

 	mouse_x = ScaleXInches(x);/*pixels --> inches*/
 	mouse_y = ScaleYInches(y);/*pixels --> inches*/
 	switch (event) {
    	case BUTTON_DOWN:
   			if(button == LEFT_BUTTON) {
   				if((mouse_x + mouse_y>start_x + start_y)&&(mouse_x + mouse_y<start_x + start_y + start_side*2)&&
   				(mouse_y - mouse_x<-25)&&(mouse_y  - mouse_x>-70))
				{
					background();
  		 			button_start(1);
					button_1(0);
					button_2(0);
					button_3(0);
  		 			start_button = 0;
				}
				/*判断鼠标是否在BUTTON_1区域里面， 
				这里偷个懒...其实只是判断鼠标是否在按钮中的一个矩形区域里面，比较省事23333*/ 
				else if((mouse_x>start_x-40)&&(mouse_x<start_x- 40 + start_side*1.7)&&(mouse_y<start_y+25)&&(mouse_y>start_y+25-start_side/2))
				{
					background();
					button_start(0);
  	   				button_1(1);
					button_2(0);
					button_3(0);
  		 			start_button = 1;	   				
				}
				/*判断鼠标是否在BUTTON_2区域里面,方法同上*/
				else if((mouse_x<start_x-19)&&(mouse_x>start_x- 19 - start_side*2)&&(mouse_y<start_y+9)&&(mouse_y>start_y-9))
				{
					background();
					button_start(0);
					button_1(0);
  	   				button_2(1);
					button_3(0);
  		 			start_button = 2; 	   				
				}
				/*判断鼠标是否在BUTTON_3区域里面,方法同上*/
				else if((mouse_x>start_x-40)&&(mouse_x<start_x- 40 + start_side*1.7)&&(mouse_y>start_y-25)&&(mouse_y<start_y-25+start_side/2))
				{
					background();
					button_start(0);
					button_1(0);
					button_2(0);
  	   				button_3(1);
  		 			start_button = 3;
				}
   			}
            break;
    	case BUTTON_UP:
    		/*松开鼠标，还原界面*/ 
  		 	if (button == LEFT_BUTTON) {
				DrawMenu();
				return start_button + 1;
  		 	}
			break;
 /* 		 	if(start_button >-1){ 
  		 		Pause(0.2);//如果按下按键，注销鼠标函数，等待0.2s后清空界面 
	  			InitGraphics();
	  		 	return;
			}*/
	}
	return 0;
}


/*填充背景颜色*/ 
static void background(){
	SetPenSize(0);
	MovePen(0,0);
	SetPenColor("matcha");/*设置背景颜色*/	
	StartFilledRegion(1);/*开始填充颜色*/ 
	DrawLine(0,screen_height);
	DrawLine(screen_width,0);		
	DrawLine(0,-screen_height);	
	DrawLine(-screen_width,0);	
	EndFilledRegion();/*结束填充颜色*/ 	
} 

/*画开始按钮*/ 
static void button_start(int mode)
{
	SetPenSize(5);/*边框大小*/ 
	nowangle = 0;
	if(mode){
		SetPenColor("orange");
		MovePen(start_x-1,start_y-1);
	}/*按下颜色*/ 
	else{
		SetPenColor("gingeryellow");
		MovePen(start_x,start_y);		
	}/*松开颜色以及初始化位置*/ 
	StartFilledRegion(0.8);/*开始填充颜色*/ 
	turn(90);
	DrawLine(start_side,start_side);
	DrawLine(start_side,-start_side);
	DrawLine(-start_side,-start_side);
	DrawLine(-start_side,start_side);
	EndFilledRegion();/*结束填充颜色*/ 
	move(-2);
	SetPenColor("rred");
	SetPointSize(50);/*文本大小*/ 
	DrawTextString("       START");	
}

/*按钮1 */
static void button_1(int mode)
{
	SetPenSize(5);/*边框大小*/	
	nowangle = 0;
	if(mode){
		SetPenColor("orange");
		MovePen(start_x-41,start_y+25);
	}/*按下颜色及初始化位置*/ 
	else{
		SetPenColor("navyblue");
		MovePen(start_x-40,start_y+25);		
	}/*松开颜色及初始化位置*/ 
	StartFilledRegion(0.4);/*开始填充颜色*/ 
	DrawLine(start_side*2,0);	
	turn(-135);
	forward(start_side/1.5);
	DrawLine(-start_side*2,0);
	forward(-start_side/1.5);
	EndFilledRegion();/*结束填充颜色*/ 
	move(10);
	SetPointSize(50);/*文本大小*/ 
	DrawTextString("       LOAD");
}

/*按钮2 */
static void button_2(int mode)
{
	SetPenSize(5);/*边框大小*/	
	nowangle = 0;
	if(mode){
		SetPenColor("orange");
		MovePen(start_x-20,start_y);
	}/*按下颜色以及初始化位置*/ 
	else{
		SetPenColor("navyblue");
		MovePen(start_x-19,start_y);		
	}/*松开颜色以及初始化位置*/ 
	StartFilledRegion(0.4);/*开始填充颜色*/ 
	turn(45);
	forward(start_side/2);
	DrawLine(-start_side*2,0);
	forward(-start_side/2);
	turn(-90);
	forward(start_side/2);
	DrawLine(start_side*2,0);
	forward(-start_side/2);
	EndFilledRegion();/*结束填充颜色*/ 
	nowangle = 0;
	move(-50);
	turn(90);
	move(-2);	
	SetPointSize(50);/*文本大小*/ 
	DrawTextString("         HELP");
} 

/*按钮3 */
static void button_3(int mode)
{
	SetPenSize(5);/*边框大小*/	
	nowangle = 0; 
	if(mode){
		SetPenColor("orange");
		MovePen(start_x-41,start_y-25);
	}/*按下颜色以及初始化位置*/ 
	else{
		SetPenColor("navyblue");
		MovePen(start_x-40,start_y-25);		
	}/*松开颜色以及初始化位置*/	
	StartFilledRegion(0.4);/*开始填充颜色*/ 
	DrawLine(start_side*2,0);	
	turn(135);
	forward(start_side/1.5);
	DrawLine(-start_side*2,0);
	forward(-start_side/1.5);
	EndFilledRegion();/*结束填充颜色*/ 	
	move(5);
	SetPointSize(50);/*文本大小*/ 
	DrawTextString("   EXIT");
} 

/*沿当前方向移动笔划线*/
static void forward(double distance){
	DrawLine(distance*cos(nowangle),distance*sin(nowangle));
}

/*转动角度*/
static void turn (double angle){
	nowangle += angle / 180 * pi;
}

/*沿当前方向移动笔不划线*/
static void move(double distance){
	double now_x = GetCurrentX();
	double now_y = GetCurrentY();
	MovePen(now_x + distance * cos(nowangle),now_y + distance*sin(nowangle));	
}

