/***********************************************************
	start_menu.c 
	
	����˵��: ����ʼ���� ,�������ĸ���������ֵ
			����0 Ϊ��ʼ��
			����1 ΪBUTTON_1 
			����2 ΪBUTTON_2
			����3 ΪBUTTON_3 
	���ýӿ�:int start_menu(); 
			int Mouse_StartMenu(int x, int y, int button, int event); ������	
	
	BUG:Ŀǰû�а취ֹͣ��꺯��...�������������һֱ���У������ؽ�� 
	����ʱ�� :2019/5/12  0:43
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

static void turn(double angle);/*ת���Ƕ�*/
static void forward(double distance);/*�ص�ǰ�����ƶ��ʻ���*/
static void move(double distance); /*�ص�ǰ�����ƶ��ʲ�����*/ 

static void DrawMenu();/*���˵�*/ 
static void background();/*������ɫ*/ 
static void button_start(int mode);/*����ʼ����*/ 
static void button_1(int mode);/*��BUTTON_1*/ 
static void button_2(int mode);/*��BUTTON_2*/ 
static void button_3(int mode);/*��BUTTON_3*/

//=========================================================================================================================================================================================================

/*�����˵�*/
void start_menu()
{
	screen_height = GetWindowHeight();/*��ȡ��Ļ�߶�*/ 
	screen_width = GetWindowWidth();/*��ȡ��Ļ���*/ 
    DrawMenu();
}

/*�����˵�*/
static void DrawMenu(){
	background();	
	button_start(0);
	button_1(0);
 	button_2(0);
	button_3(0);/*����ť*/ 
}

/*�����Ϣ*/
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
				/*�ж�����Ƿ���BUTTON_1�������棬 
				����͵����...��ʵֻ���ж�����Ƿ��ڰ�ť�е�һ�������������棬�Ƚ�ʡ��23333*/ 
				else if((mouse_x>start_x-40)&&(mouse_x<start_x- 40 + start_side*1.7)&&(mouse_y<start_y+25)&&(mouse_y>start_y+25-start_side/2))
				{
					background();
					button_start(0);
  	   				button_1(1);
					button_2(0);
					button_3(0);
  		 			start_button = 1;	   				
				}
				/*�ж�����Ƿ���BUTTON_2��������,����ͬ��*/
				else if((mouse_x<start_x-19)&&(mouse_x>start_x- 19 - start_side*2)&&(mouse_y<start_y+9)&&(mouse_y>start_y-9))
				{
					background();
					button_start(0);
					button_1(0);
  	   				button_2(1);
					button_3(0);
  		 			start_button = 2; 	   				
				}
				/*�ж�����Ƿ���BUTTON_3��������,����ͬ��*/
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
    		/*�ɿ���꣬��ԭ����*/ 
  		 	if (button == LEFT_BUTTON) {
				DrawMenu();
				return start_button + 1;
  		 	}
			break;
 /* 		 	if(start_button >-1){ 
  		 		Pause(0.2);//������°�����ע����꺯�����ȴ�0.2s����ս��� 
	  			InitGraphics();
	  		 	return;
			}*/
	}
	return 0;
}


/*��䱳����ɫ*/ 
static void background(){
	SetPenSize(0);
	MovePen(0,0);
	SetPenColor("matcha");/*���ñ�����ɫ*/	
	StartFilledRegion(1);/*��ʼ�����ɫ*/ 
	DrawLine(0,screen_height);
	DrawLine(screen_width,0);		
	DrawLine(0,-screen_height);	
	DrawLine(-screen_width,0);	
	EndFilledRegion();/*���������ɫ*/ 	
} 

/*����ʼ��ť*/ 
static void button_start(int mode)
{
	SetPenSize(5);/*�߿��С*/ 
	nowangle = 0;
	if(mode){
		SetPenColor("orange");
		MovePen(start_x-1,start_y-1);
	}/*������ɫ*/ 
	else{
		SetPenColor("gingeryellow");
		MovePen(start_x,start_y);		
	}/*�ɿ���ɫ�Լ���ʼ��λ��*/ 
	StartFilledRegion(0.8);/*��ʼ�����ɫ*/ 
	turn(90);
	DrawLine(start_side,start_side);
	DrawLine(start_side,-start_side);
	DrawLine(-start_side,-start_side);
	DrawLine(-start_side,start_side);
	EndFilledRegion();/*���������ɫ*/ 
	move(-2);
	SetPenColor("rred");
	SetPointSize(50);/*�ı���С*/ 
	DrawTextString("       START");	
}

/*��ť1 */
static void button_1(int mode)
{
	SetPenSize(5);/*�߿��С*/	
	nowangle = 0;
	if(mode){
		SetPenColor("orange");
		MovePen(start_x-41,start_y+25);
	}/*������ɫ����ʼ��λ��*/ 
	else{
		SetPenColor("navyblue");
		MovePen(start_x-40,start_y+25);		
	}/*�ɿ���ɫ����ʼ��λ��*/ 
	StartFilledRegion(0.4);/*��ʼ�����ɫ*/ 
	DrawLine(start_side*2,0);	
	turn(-135);
	forward(start_side/1.5);
	DrawLine(-start_side*2,0);
	forward(-start_side/1.5);
	EndFilledRegion();/*���������ɫ*/ 
	move(10);
	SetPointSize(50);/*�ı���С*/ 
	DrawTextString("       LOAD");
}

/*��ť2 */
static void button_2(int mode)
{
	SetPenSize(5);/*�߿��С*/	
	nowangle = 0;
	if(mode){
		SetPenColor("orange");
		MovePen(start_x-20,start_y);
	}/*������ɫ�Լ���ʼ��λ��*/ 
	else{
		SetPenColor("navyblue");
		MovePen(start_x-19,start_y);		
	}/*�ɿ���ɫ�Լ���ʼ��λ��*/ 
	StartFilledRegion(0.4);/*��ʼ�����ɫ*/ 
	turn(45);
	forward(start_side/2);
	DrawLine(-start_side*2,0);
	forward(-start_side/2);
	turn(-90);
	forward(start_side/2);
	DrawLine(start_side*2,0);
	forward(-start_side/2);
	EndFilledRegion();/*���������ɫ*/ 
	nowangle = 0;
	move(-50);
	turn(90);
	move(-2);	
	SetPointSize(50);/*�ı���С*/ 
	DrawTextString("         HELP");
} 

/*��ť3 */
static void button_3(int mode)
{
	SetPenSize(5);/*�߿��С*/	
	nowangle = 0; 
	if(mode){
		SetPenColor("orange");
		MovePen(start_x-41,start_y-25);
	}/*������ɫ�Լ���ʼ��λ��*/ 
	else{
		SetPenColor("navyblue");
		MovePen(start_x-40,start_y-25);		
	}/*�ɿ���ɫ�Լ���ʼ��λ��*/	
	StartFilledRegion(0.4);/*��ʼ�����ɫ*/ 
	DrawLine(start_side*2,0);	
	turn(135);
	forward(start_side/1.5);
	DrawLine(-start_side*2,0);
	forward(-start_side/1.5);
	EndFilledRegion();/*���������ɫ*/ 	
	move(5);
	SetPointSize(50);/*�ı���С*/ 
	DrawTextString("   EXIT");
} 

/*�ص�ǰ�����ƶ��ʻ���*/
static void forward(double distance){
	DrawLine(distance*cos(nowangle),distance*sin(nowangle));
}

/*ת���Ƕ�*/
static void turn (double angle){
	nowangle += angle / 180 * pi;
}

/*�ص�ǰ�����ƶ��ʲ�����*/
static void move(double distance){
	double now_x = GetCurrentX();
	double now_y = GetCurrentY();
	MovePen(now_x + distance * cos(nowangle),now_y + distance*sin(nowangle));	
}

