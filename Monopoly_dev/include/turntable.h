#pragma once
#ifndef _turntable_h
#define _turntable_h

/*
---------------------------------------------------------------------------------------------------
全局变量：
int resultX;
bool isResult;
---------------------------------------------------------------------------------------------------
*/

/*
---------------------------------------------------------------------------------------------------
void DrawTurnTable(double turntable_x,double turntable_y,double turntable_radius);

函数说明：在x,y位置画转盘
参数说明：
			turntable_x：			转盘起始横坐标
			turntable_y:			转盘起始纵坐标
			turntable_radius：		转盘半径
---------------------------------------------------------------------------------------------------
*/
void DrawTurnTable(double turntable_x, double turntable_y, double turntable_radius);

/*
---------------------------------------------------------------------------------------------------
void Timer_Turntable();

函数说明：定时器回调函数中调用，执行转盘转动
---------------------------------------------------------------------------------------------------
*/
void Timer_Turntable();

/*
---------------------------------------------------------------------------------------------------
void Keyboard_Turntable(int key, int event);

函数说明：键盘回调函数中调用此函数，执行与转盘相关的键盘行为
			按下空格开始转动转盘；
			按下回车停止转动，并使resultX为结果
参数说明：同键盘回调函数
---------------------------------------------------------------------------------------------------
*/
void Keyboard_Turntable(int key, int event);

#endif // !_turntable_h

