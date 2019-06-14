#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "random.h"
#include "strlib.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include "spiral.h"
#include "setting.h"



#define pi 3.14159265
#define mseconds_spiral 75

static double angle_spiral = 0;

static double start_len =0.4;

extern game_over;
extern struct PlayPeople * Player[4];
extern int turn_player;

void turn_spiral(double angle);
void forward_spiral(double distance);/*�ص�ǰ�����ƶ��ʻ���*/
void move_spiral(double distance); /*�ص�ǰ�����ƶ��ʲ�����*/ 

void spiral(){
	start_len = 0.4;
	SetPointSize(100);
	SetStyle(1);
	DrawSpiral();
}
		
int Timer_spiral(int timerID){
	switch (timerID) {
    	case 0:
		    SetEraseMode(TRUE);
			DrawSpiral(" ");//����֮ǰ��	
		    SetEraseMode(FALSE);
		    turn_spiral(-5);
		    start_len-=0.01;
		    DrawSpiral(" ");//���µ� 
		    if(start_len<0){
				cancelTimer(0);
				return 1;
			} 
		  	break;
    	default:
	  		break;
	}
	return 0;
}

int Keyboard_spiral(int key,int event){
	switch (event) {
	 	
		 case KEY_DOWN:
			 switch (key){
				case VK_RETURN:
						cancelTimer(0);
						return 1;
			}
			 break;
	}
	return 0;
}

void DrawSpiral(){
	MovePen(96,54);
	int i;
	SetPenSize(20);
	
	SetPointSize(100); 
	for(i=0;i<500;i++){
		
		switch (i%4){
			case 0:
				SetPenColor("khaki");
				break;
			case 1:
				SetPenColor("gingeryellow");
				break;
			case 2:
				SetPenColor("khaki");
				break;
			case 3:
				SetPenColor("gingeryellow");
				break;
		}
		forward_spiral(start_len*i);

		turn_spiral(91);
	}
	if(game_over){
		SetPenColor("deepblue");
		SetPointSize(50);
		MovePen(70,50);
		DrawTextString("���س����ص�������");
		MovePen(30,60);
		SetPointSize(100);
		SetPenColor("rred");
		DrawTextString("��ϲ");
		DrawTextString(Player[turn_player]->name);
		DrawTextString("��Ϊ���̣���������");		
	}
	else{
		MovePen(192.0/2-TextStringWidth("��  ��  ��")/2,108.0/2);
		SetPenColor("deepblue");
		DrawTextString("��  ��  ��") ;
		MovePen(192.0/2-TextStringWidth("MONOPOLY")/2,108.0/2-TextStringWidth("��"));
		DrawTextString("MONOPOLY");
	} 

		
		
	SetPointSize(20); 
	SetPenColor("orange");
	MovePen(80,10);
		DrawTextString("Copyright Group5");
	
		DrawTextString("��Ԫ��  ���  ��ͩ");
}

void forward_spiral(double distance){
	DrawLine(distance*cos(angle_spiral),distance*sin(angle_spiral));
}

void turn_spiral(double angle){
	angle_spiral += angle / 180 * pi;
}

void move_spiral(double distance){
	double now_x = GetCurrentX();
	double now_y = GetCurrentY();
	MovePen(now_x + distance * cos(angle_spiral),now_y + distance*sin(angle_spiral));	
}

