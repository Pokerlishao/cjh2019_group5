/*************************************************************
主函数 

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
int degree = 0; //界面：0-动画;1-主菜单;2-设置;3-游戏;4-载入;5-保存;6-主菜单下帮助;7-游戏下帮助;8-暂停;
int act=0;/*选项：
1 : 点击了新游戏，执行保存并设置
2 : 点击了新游戏，执行设置
3 : 点击了载入，执行保存并载入
4 : 点击了载入，执行载入
5 : 点击了保存，执行保存
6 : 点击了退出，执行保存并退出
7 : 点击了帮助，执行显示规则
8 : 点击了暂停，执行暂停
9 : 点击了继续，执行继续
*/
int isact = 0;//功能正在执行标志

//=========================================================================================================================================================================================================

void KeyboardEventProcess(int key,int event);/*键盘消息回调函数*/
void CharEventProcess(char c);/*字符消息回调函数*/
void MouseEventProcess(int x, int y, int button, int event);/*鼠标消息回调函数*/
void TimerEventProcess(int timerID);/*定时器消息回调函数*/

//=========================================================================================================================================================================================================

void Main(){
	SetWindowSize(192.0,108.0);
	SetWindowTitle("Monopoly");
	InitGraphics();/*初始化图形界面 */
	MoreColor();/*更多颜色 */
	mciSendString("play start_save.wav",NULL,0,NULL) ;
	spiral(" ");/*画开始动画*/ 
	startTimer(0, 75);/*定时器触发*/
	registerMouseEvent(MouseEventProcess);/*注册鼠标回调函数 */
	registerTimerEvent(TimerEventProcess);/*注册定时器回调函数*/
	registerKeyboardEvent(KeyboardEventProcess);/*注册键盘消息回调函数*/ 
	registerCharEvent(CharEventProcess);/*注册字符消息回调函数*/ 
}

//=========================================================================================================================================================================================================

/*鼠标消息回调函数*/
void MouseEventProcess(int x, int y, int button, int event){
	int choose=0;
	switch(degree){//界面：0-动画;1-主菜单;2-设置;3-游戏;4-载入;5-保存;6-主菜单下帮助;7-游戏下帮助;8-暂停;
		/*开始界面*/ 
		case 0:
			break;
		case 1:
			choose = Mouse_StartMenu( x, y,button,event);
			/*
			1：点击开始游戏
			2：点击载入
			3：点击帮助
			4：点击退出
			0：others
			*/
			if(choose){
				switch(choose){
					case 1://开始游戏
						DrawSet();
						degree = 2;/*进入设置界面 */
						break;
					case 2://载入
						DrawRead();
						degree = 4;
						break;
					case 3://帮助
						DrawHelp();
						degree = 6;
						break;
					case 4://退出
						exit(-1);
				}
				isact = 0;
			}
			break;
		/*设置界面*/
		case 2:
			choose = Mouse_about_Setting(x, y, button, event);
			/*1-开始游戏;2-返回主菜单;0-others*/
			if (choose == 1) {//开始游戏
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
			else if (choose == 2) {//返回
				start_button = -1;
				start_menu();
				degree = 1;
				isact = 0;
			}
			break;
		/*地图1*/ 
		case 3:
			act = Mouse_Map1(x, y, button, event);
			/*选项：
				1 : 点击了新游戏，执行保存并设置
				2 : 点击了新游戏，执行设置
				3 : 点击了载入，执行保存并载入
				4 : 点击了载入，执行载入
				5 : 点击了保存，执行保存
				6 : 点击了退出，执行保存并退出
				7 : 点击了帮助，执行显示规则
				8 : 点击了暂停，执行暂停
			*/
			if(!isact)
			switch (act) {
				case 1://保存并新游戏
				case 3://保存并载入
				case 5://保存
				case 6://保存并退出
					isact = 1;
					DrawSave();
					degree = 5;
					break;
				case 2://新游戏
					isact = 1;
					DrawSet();
					degree = 2;
					act = 0;
					break;
				case 4://载入
					isact = 1;
					DrawRead();
					degree = 4;
					act = 0;
					break;
				case 7://帮助
					isact = 1;
					DrawHelp();
					degree = 7;
					act = 0;
					break;
				case 8://暂停
					degree = 8;
					act = 0;
					break;
			}
			break;
		/*载入*/
		case 4:
			choose = Mouse_about_Read(x, y, button, event);
			/*
			1-返回
			2-载入
			0-others
			*/
			if (choose == 1)//返回主菜单
			{
				start_button = -1;
				start_menu();
				degree = 1;
			}
			else if (choose == 2)//载入
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
		/*保存*/
		case 5:
			choose = Mouse_about_Save(x, y, button, event);
			/*
			1-点击返回
			2-完成保存
			0-others
			*/
			if (choose == 1)//返回
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
					case 1://保存并设置
						DrawSet();
						degree = 2;
						act = 0;
						isact = 0;
						break;
					case 3://保存并载入
						DrawRead();
						degree = 4;
						act = 0;
						isact = 0;
						break;
					case 5://保存并返回游戏
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
					case 6://保存并退出
						act = 0;
						isact = 0;
						exit(-1);
					case 10://在暂停中保存，返回暂停
						act = 0;
						degree = 8;
						DrawMap1(2);
						isact = 0;
				}
				act = 0;
			}
			break;
		/*主菜单下帮助*/
		case 6:
			choose = Mouse_about_Help(x, y, button, event);
			/*
			1-点击我懂了
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
		/*游戏下帮助*/
		case 7:
			choose = Mouse_about_Help(x, y, button, event);
			/*
			1-点击我懂了
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
		/*暂停中*/
		case 8:
			act = Mouse_Map1(x, y, button, event);
			/*选项：
				1 : 点击了新游戏，执行保存并设置
				2 : 点击了新游戏，执行设置
				3 : 点击了载入，执行保存并载入
				4 : 点击了载入，执行载入
				5 : 点击了保存，执行保存
				6 : 点击了退出，执行保存并退出
				7 : 点击了帮助，执行显示规则
				9 : 点击了继续，执行继续
			*/
			if (!isact)
				switch (act) {
				case 5://保存
					act = 10;
				case 1://保存并新游戏
				case 3://保存并载入
				case 6://保存并退出
					isact = 1;
					DrawSave();
					degree = 5;
					break;
				case 2://新游戏
					isact = 1;
					DrawSet();
					degree = 2;
					act = 0;
					break;
				case 4://载入
					isact = 1;
					DrawRead();
					degree = 4;
					act = 0;
					break;
				case 7://帮助
					isact = 1;
					DrawHelp();
					degree = 7;
					act = 0;
					break;
				case 9://继续
					degree = 3;
					DrawMap1(2);
					act = 0;
					break;
				}
	}
}

/*定时器回调函数*/
void TimerEventProcess(int timerID){
	switch(degree){
		case 0:
			if(Timer_spiral(0)==1)
			{
				degree=1;
				start_menu(); /*画开始菜单*/
			}
			break;
		case 3:
			if(timerID==1)
				Timer_Map1();/*画旋转的转盘*/ 
			break;
	}
}

/*键盘回调函数*/
void KeyboardEventProcess(int key,int event){
	switch(degree){
		case 0:
			if(Keyboard_spiral(key,event)==1)
			{
				degree=1;
				start_menu(); /*画开始菜单*/
			}
			break;
		case 2://设置
			KeyBoard_about_Setting(key, event);
			break;
		case 3://游戏中
			act = KeyBoard_Map1(key, event);
			/*
			1:点击了新游戏，执行保存并设置
			2:点击了新游戏，执行设置
			3:点击了载入，执行保存并载入
			4:点击了载入，执行载入
			5:点击了保存，执行保存
			6:点击了退出，执行保存并退出
			7:点击了帮助，执行显示规则
			8:点击了暂停，执行暂停
			9:点击了继续，执行继续
			0:others
			*/
			if(!isact)
			switch (act) {
				case 1://保存并设置
				case 3://保存并载入
				case 5://保存
				case 6://保存并退出
					isact = 1;
					DrawSave();
					degree = 5;
					break;
				case 2://新游戏
					isact = 1;
					InitGraphics();
					DrawSet();
					degree = 2;
					act = 0;
					break;
				case 4://载入
					isact = 1;
					DrawRead();
					degree = 4;
					act = 0;
					break;
				case 7://帮助
					isact = 1;
					DrawHelp();
					degree = 7;
					act = 0;
					break;
				case 8://暂停
					degree = 8;
					act = 0;
					break;
			}
			break;
		case 8:
			act = KeyBoard_about_MenuBar(key, event);
			/*选项：
				1 : 点击了新游戏，执行保存并设置
				2 : 点击了新游戏，执行设置
				3 : 点击了载入，执行保存并载入
				4 : 点击了载入，执行载入
				5 : 点击了保存，执行保存
				6 : 点击了退出，执行保存并退出
				7 : 点击了帮助，执行显示规则
				9 : 点击了继续，执行继续
			*/
			if (!isact)
				switch (act) {
				case 5://保存
					act = 10;
				case 1://保存并新游戏
				case 3://保存并载入
				case 6://保存并退出
					isact = 1;
					DrawSave();
					degree = 5;
					break;
				case 2://新游戏
					isact = 1;
					DrawSet();
					degree = 2;
					act = 0;
					break;
				case 4://载入
					isact = 1;
					DrawRead();
					degree = 4;
					act = 0;
					break;
				case 7://帮助
					isact = 1;
					DrawHelp();
					degree = 7;
					act = 0;
					break;
				case 9://继续
					degree = 3;
					DrawMap1(2);
					act = 0;
					break;
				}
	}
}

/*字符回调函数*/
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
