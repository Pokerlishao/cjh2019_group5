/*************************************************************
������ 

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

#include "spiral.h" 
#include "morecolor.h"
#include "start_menu.h"
#include "Map1_demo.h"
#include "setting.h"
#include "help.h"
#include "DrawMenuBar.h"
#include "load.h"
#include "save.h"


//=========================================================================================================================================================================================================

extern int start_button;
extern int Map;
extern struct PlayPeople* Player[4];
int degree = 0; //���棺0-����;1-���˵�;2-����;3-��Ϸ;4-����;5-����;6-���˵��°���;7-��Ϸ�°���;8-��ͣ;
int act=0;/*ѡ�
1 : ���������Ϸ��ִ�б��沢����
2 : ���������Ϸ��ִ������
3 : ��������룬ִ�б��沢����
4 : ��������룬ִ������
5 : ����˱��棬ִ�б���
6 : ������˳���ִ�б��沢�˳�
7 : ����˰�����ִ����ʾ����
8 : �������ͣ��ִ����ͣ
9 : ����˼�����ִ�м���
*/
int isact = 0;//��������ִ�б�־

//=========================================================================================================================================================================================================

void KeyboardEventProcess(int key,int event);/*������Ϣ�ص�����*/
void CharEventProcess(char c);/*�ַ���Ϣ�ص�����*/
void MouseEventProcess(int x, int y, int button, int event);/*�����Ϣ�ص�����*/
void TimerEventProcess(int timerID);/*��ʱ����Ϣ�ص�����*/

//=========================================================================================================================================================================================================

void Main(){
	SetWindowSize(192.0,108.0);
	SetWindowTitle("Monopoly");
	InitGraphics();/*��ʼ��ͼ�ν��� */
	MoreColor();/*������ɫ */
	mciSendString("play start_save.wav",NULL,0,NULL) ;
	spiral(" ");/*����ʼ����*/ 
	startTimer(0, 75);/*��ʱ������*/
	registerMouseEvent(MouseEventProcess);/*ע�����ص����� */
	registerTimerEvent(TimerEventProcess);/*ע�ᶨʱ���ص�����*/
	registerKeyboardEvent(KeyboardEventProcess);/*ע�������Ϣ�ص�����*/ 
	registerCharEvent(CharEventProcess);/*ע���ַ���Ϣ�ص�����*/ 
}

//=========================================================================================================================================================================================================

/*�����Ϣ�ص�����*/
void MouseEventProcess(int x, int y, int button, int event){
	int choose=0;
	switch(degree){//���棺0-����;1-���˵�;2-����;3-��Ϸ;4-����;5-����;6-���˵��°���;7-��Ϸ�°���;8-��ͣ;
		/*��ʼ����*/ 
		case 0:
			break;
		case 1:
			choose = Mouse_StartMenu( x, y,button,event);
			/*
			1�������ʼ��Ϸ
			2���������
			3���������
			4������˳�
			0��others
			*/
			if(choose){
				switch(choose){
					case 1://��ʼ��Ϸ
						DrawSet();
						degree = 2;/*�������ý��� */
						break;
					case 2://����
						DrawRead();
						degree = 4;
						break;
					case 3://����
						DrawHelp();
						degree = 6;
						break;
					case 4://�˳�
						exit(-1);
				}
				isact = 0;
			}
			break;
		/*���ý���*/
		case 2:
			choose = Mouse_about_Setting(x, y, button, event);
			/*1-��ʼ��Ϸ;2-�������˵�;0-others*/
			if (choose == 1) {//��ʼ��Ϸ
				switch (Map) {
					case 1:
					case 2:
					case 3:
						MainMap1();
						break;
				}
				degree = 3;
				isact = 0;
			}
			else if (choose == 2) {//����
				start_button = -1;
				start_menu();
				degree = 1;
				isact = 0;
			}
			break;
		/*��ͼ1*/ 
		case 3:
			act = Mouse_Map1(x, y, button, event);
			/*ѡ�
				1 : ���������Ϸ��ִ�б��沢����
				2 : ���������Ϸ��ִ������
				3 : ��������룬ִ�б��沢����
				4 : ��������룬ִ������
				5 : ����˱��棬ִ�б���
				6 : ������˳���ִ�б��沢�˳�
				7 : ����˰�����ִ����ʾ����
				8 : �������ͣ��ִ����ͣ
			*/
			if(!isact)
			switch (act) {
				case 1://���沢����Ϸ
				case 3://���沢����
				case 5://����
				case 6://���沢�˳�
					isact = 1;
					DrawSave();
					degree = 5;
					break;
				case 2://����Ϸ
					isact = 1;
					DrawSet();
					degree = 2;
					act = 0;
					break;
				case 4://����
					isact = 1;
					DrawRead();
					degree = 4;
					act = 0;
					break;
				case 7://����
					isact = 1;
					DrawHelp();
					degree = 7;
					act = 0;
					break;
				case 8://��ͣ
					degree = 8;
					act = 0;
					break;
			}
			break;
		/*����*/
		case 4:
			choose = Mouse_about_Read(x, y, button, event);
			/*
			1-����
			2-����
			0-others
			*/
			if (choose == 1)//�������˵�
			{
				start_button = -1;
				start_menu();
				degree = 1;
			}
			else if (choose == 2)//����
			{
				switch (Map) {
				case 1:
				case 2:
				case 3:
					DrawMap1(2);
					break;
				}
				degree = 3;
			}
			isact = 0;
			break;
		/*����*/
		case 5:
			choose = Mouse_about_Save(x, y, button, event);
			/*
			1-�������
			2-��ɱ���
			0-others
			*/
			if (choose == 1)//����
			{
				switch (Map)
				{
				case 1:
				case 2:
				case 3:
					DrawMap1(2);
					break;
				}
				if (act == 10)
					degree = 8;
				else
					degree = 3;
				act = 0;
				isact = 0;
			}
			else if (choose == 2)
			{
				switch (act)
				{
					case 1://���沢����
						DrawSet();
						degree = 2;
						act = 0;
						isact = 0;
						break;
					case 3://���沢����
						DrawRead();
						degree = 4;
						act = 0;
						isact = 0;
						break;
					case 5://���沢������Ϸ
						switch (Map)
						{
						case 1:
						case 2:
						case 3:
							DrawMap1(2);
							break;
						}
						degree = 3;
						act = 0;
						isact = 0;
						break;
					case 6://���沢�˳�
						act = 0;
						isact = 0;
						exit(-1);
					case 10://����ͣ�б��棬������ͣ
						act = 0;
						degree = 8;
						DrawMap1(2);
						isact = 0;
				}
				act = 0;
			}
			break;
		/*���˵��°���*/
		case 6:
			choose = Mouse_about_Help(x, y, button, event);
			/*
			1-����Ҷ���
			0-others
			*/
			if (choose)
			{
				start_button = -1;
				start_menu();
				degree = 1;
				isact = 0;
			}
			break;
		/*��Ϸ�°���*/
		case 7:
			choose = Mouse_about_Help(x, y, button, event);
			/*
			1-����Ҷ���
			0-others
			*/
			if (choose)
			{
				switch (Map)
				{
					case 1:
					case 2:
					case 3:
						DrawMap1(2);
						break;
				}
				degree = 3;
				isact = 0;
			}
			break;
		/*��ͣ��*/
		case 8:
			act = Mouse_Map1(x, y, button, event);
			/*ѡ�
				1 : ���������Ϸ��ִ�б��沢����
				2 : ���������Ϸ��ִ������
				3 : ��������룬ִ�б��沢����
				4 : ��������룬ִ������
				5 : ����˱��棬ִ�б���
				6 : ������˳���ִ�б��沢�˳�
				7 : ����˰�����ִ����ʾ����
				9 : ����˼�����ִ�м���
			*/
			if (!isact)
				switch (act) {
				case 5://����
					act = 10;
				case 1://���沢����Ϸ
				case 3://���沢����
				case 6://���沢�˳�
					isact = 1;
					DrawSave();
					degree = 5;
					break;
				case 2://����Ϸ
					isact = 1;
					DrawSet();
					degree = 2;
					act = 0;
					break;
				case 4://����
					isact = 1;
					DrawRead();
					degree = 4;
					act = 0;
					break;
				case 7://����
					isact = 1;
					DrawHelp();
					degree = 7;
					act = 0;
					break;
				case 9://����
					degree = 3;
					DrawMap1(2);
					act = 0;
					break;
				}
	}
}

/*��ʱ���ص�����*/
void TimerEventProcess(int timerID){
	switch(degree){
		case 0:
			if(Timer_spiral(0)==1)
			{
				degree=1;
				start_menu(); /*����ʼ�˵�*/
			}
			break;
		case 3:
			if(timerID==1)
				Timer_Map1();/*����ת��ת��*/ 
			break;
	}
}

/*���̻ص�����*/
void KeyboardEventProcess(int key,int event){
	switch(degree){
		case 0:
			if(Keyboard_spiral(key,event)==1)
			{
				degree=1;
				start_menu(); /*����ʼ�˵�*/
			}
			break;
		case 2://����
			KeyBoard_about_Setting(key, event);
			break;
		case 3://��Ϸ��
			act = KeyBoard_Map1(key, event);
			/*
			1:���������Ϸ��ִ�б��沢����
			2:���������Ϸ��ִ������
			3:��������룬ִ�б��沢����
			4:��������룬ִ������
			5:����˱��棬ִ�б���
			6:������˳���ִ�б��沢�˳�
			7:����˰�����ִ����ʾ����
			8:�������ͣ��ִ����ͣ
			9:����˼�����ִ�м���
			0:others
			*/
			if(!isact)
			switch (act) {
				case 1://���沢����
				case 3://���沢����
				case 5://����
				case 6://���沢�˳�
					isact = 1;
					DrawSave();
					degree = 5;
					break;
				case 2://����Ϸ
					isact = 1;
					InitGraphics();
					DrawSet();
					degree = 2;
					act = 0;
					break;
				case 4://����
					isact = 1;
					DrawRead();
					degree = 4;
					act = 0;
					break;
				case 7://����
					isact = 1;
					DrawHelp();
					degree = 7;
					act = 0;
					break;
				case 8://��ͣ
					degree = 8;
					act = 0;
					break;
			}
			break;
		case 8:
			act = KeyBoard_about_MenuBar(key, event);
			/*ѡ�
				1 : ���������Ϸ��ִ�б��沢����
				2 : ���������Ϸ��ִ������
				3 : ��������룬ִ�б��沢����
				4 : ��������룬ִ������
				5 : ����˱��棬ִ�б���
				6 : ������˳���ִ�б��沢�˳�
				7 : ����˰�����ִ����ʾ����
				9 : ����˼�����ִ�м���
			*/
			if (!isact)
				switch (act) {
				case 5://����
					act = 10;
				case 1://���沢����Ϸ
				case 3://���沢����
				case 6://���沢�˳�
					isact = 1;
					DrawSave();
					degree = 5;
					break;
				case 2://����Ϸ
					isact = 1;
					DrawSet();
					degree = 2;
					act = 0;
					break;
				case 4://����
					isact = 1;
					DrawRead();
					degree = 4;
					act = 0;
					break;
				case 7://����
					isact = 1;
					DrawHelp();
					degree = 7;
					act = 0;
					break;
				case 9://����
					degree = 3;
					DrawMap1(2);
					act = 0;
					break;
				}
	}
}

/*�ַ��ص�����*/
void CharEventProcess(char c)
{
	switch (degree) {
		case 2:
			Char_about_Setting(c);
			break;
		case 5:
			Char_about_Save(c);
			break;
	}
}
