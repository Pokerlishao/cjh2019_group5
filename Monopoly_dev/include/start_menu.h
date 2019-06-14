#pragma once
#ifndef _start_menu_h
#define _start_menu_h

/*
--------------------------------------------------------------------------------------
void start_menu();

函数说明：开始画主菜单
--------------------------------------------------------------------------------------
*/
void start_menu();

/*
--------------------------------------------------------------------------------------
int Mouse_StartMenu(int x, int y, int button, int event);

函数说明：在鼠标回调函数中调用此函数；
返回值：
1：点击开始游戏
2：点击载入
3：点击帮助
4：点击退出
0：others
*/
int Mouse_StartMenu(int x, int y, int button, int event);/*鼠标调用*/

#endif // !_start_menu_h

