/********************
*   DrawMenuBar.c
*
*   函数介绍：包含画菜单栏以及相关鼠标、键盘回调
*
*   创建时间：2019.5.12  10:30
*   创建者：管元铭  Grp 5
*
*
******************/

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

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include "DrawMenuBar.h"


//=========================================================================================================================================================================================================

typedef struct button {//按钮类型
	double StartX, StartY, h, w;//按钮左下角为起始X，Y；高度h，宽度w
	string BorderColor;//边框颜色
	string StringColor;//文字颜色
	string BorderColorOn;//被选中时边框颜色
	string StringColorOn;//被选中时文字颜色
	string BorderColorDown;
	string StringColorDown;
	string BackColor;//背景填充颜色
	string BackColorOn;//被选中时背景填充颜色
	string BackColorDown;
	int PenSize;//边框粗细
	int PointSize;//文字大小
	char hot;//快捷键
	string text;//按钮文本
	bool isOn;//鼠标停留标志
	bool isDown;//选择标志
	bool isAct;//执行标志
}*MenuBarButtonT;

//=========================================================================================================================================================================================================

//本文件变量

static MenuBarButtonT cueBarButton = NULL;//当前按钮指针
static MenuBarButtonT preBarButton = NULL;//为实现鼠标移走下拉栏关闭而设的变量

static bool MouseisDown = FALSE;//鼠标按钮按下标志
static bool KeyisDown = FALSE;//键盘按键按下标志

static MenuBarButtonT MenuBar0[3] = { NULL };//菜单、进程、帮助
static MenuBarButtonT MenuBar1[4] = { NULL };//新游戏、载入、保存、退出
static MenuBarButtonT MenuBar2[2] = { NULL,NULL };//暂停、继续

//=========================================================================================================================================================================================================

//初始化按钮
static void InitButton(MenuBarButtonT bptr,
	double w,//按钮宽
	double h,//按钮高
	string BorderColor,//边框颜色
	string BorderColorOn,//被选中时边框颜色
	string BorderColorDown,//被按下时边框颜色
	string StringColor,//文字颜色
	string StringColorOn,//被选中时文字颜色
	string StringColorDown,//被按下时文字颜色
	string BackColor,//背景填充颜色
	string BackColorOn,//被选中时背景填充颜色
	string BackColorDown,//被按下时背景颜色
	int PenSize,//边框粗细
	int PointSize//文字大小
);
static void InitMenuBar();
static void InitMenuBar0();
static void DrawMenuBar0();
static void DrawMenuBar1();
static void InitMenuBar1();
static void InitMenuBar2();
static void DrawMenuBar2();
static MenuBarButtonT IfOnButton(int x, int y);
static void DrawButton(MenuBarButtonT button);

//=========================================================================================================================================================================================================


/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MenuBar();

函数功能：画出菜单栏
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void MenuBar()
{
	if (MenuBar0[0] == NULL)
		InitMenuBar();
	DrawMenuBar0();
	return;
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InitMenuBar();

函数功能：初始化菜单栏及下拉选项卡
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
static void InitMenuBar()
{

	InitMenuBar1();
	InitMenuBar2();
	InitMenuBar0();
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InitMenuBar0();

函数功能：初始化菜单栏
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
static void InitMenuBar0()
{
	for (int i = 0; i < 3; i++)
	{
		MenuBar0[i] = GetBlock(sizeof(*MenuBar0[i]));
		InitButton(MenuBar0[i], TextStringWidth("新游戏(N)")*1.2 - ScaleXInches(GetPenSize()) * 2, 5, "Black", "Black", "Black", "Black", "Black", "Black", "gingeryellow", "VC", "lavender", 2, 5);
		MenuBar0[i]->StartX = TextStringWidth("新游戏(N)")*1.2*i;
		MenuBar0[i]->StartY = GetWindowHeight() - 5;
	}
	//
	MenuBar0[0]->text = "菜单(M)";
	MenuBar0[0]->hot = 'm';

	//
	MenuBar0[1]->text = "进程(P)";
	MenuBar0[1]->hot = 'p';

	//
	MenuBar0[2]->text = "帮助(H)";
	MenuBar0[2]->hot = 'h';
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawMenuBar0();

函数功能：画出菜单栏
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
static void DrawMenuBar0()
{
	DrawMenuBar1();
	DrawMenuBar2();
	for (int i = 0; i < 3; i++)
		DrawButton(MenuBar0[i]);
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InitMenuBar1();

函数功能：初始化菜单下拉选项卡
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
static void InitMenuBar1()
{
	for (int i = 0; i < 4; i++) {
		MenuBar1[i] = GetBlock(sizeof(*MenuBar1[0]));
		InitButton(MenuBar1[i], TextStringWidth("新游戏(N)")*1.2 - ScaleXInches(GetPenSize()) * 2, 5, "Black", "Black", "Black", "Black", "Black", "Black", "gingeryellow", "VC", "lavender", 2, 5);
		MenuBar1[i]->StartX = 0;
		MenuBar1[i]->StartY = GetWindowHeight() - 10 - 5 * i;
	}
	//
	MenuBar1[0]->text = "新游戏(N)";
	MenuBar1[0]->hot = 'n';

	//
	MenuBar1[1]->text = "载入(L)";
	MenuBar1[1]->hot = 'l';

	//
	MenuBar1[2]->text = "保存(S)";
	MenuBar1[2]->hot = 's';

	//
	MenuBar1[3]->text = "退出(E)";
	MenuBar1[3]->hot = 'e';
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawMenuBar1();

函数功能：画出菜单下拉选项卡
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
static void DrawMenuBar1()
{
	if (MenuBar0[0]->isDown)
	{
		for (int i = 0; i < 4; i++)
			DrawButton(MenuBar1[i]);
	}
	else {
		SetEraseMode(TRUE);
		for (int i = 0; i < 4; i++)
			DrawButton(MenuBar1[i]);
		SetEraseMode(FALSE);
	}
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InitMenuBar2();

函数功能：初始化进程下拉选项卡
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
static void InitMenuBar2()
{
	for (int i = 0; i < 2; i++) {
		MenuBar2[i] = GetBlock(sizeof(*MenuBar2[i]));
		InitButton(MenuBar2[i], TextStringWidth("新游戏(N)")*1.2 - ScaleXInches(GetPenSize()) * 2, 5, "Black", "Black", "Black", "Black", "Black", "Black", "Red", "VC", "lavender", 2, 5);
		MenuBar2[i]->StartX = TextStringWidth("新游戏(N)")*1.2;
		MenuBar2[i]->StartY = GetWindowHeight() - 10;
	}
	//
	MenuBar2[0]->text = "暂停(W)";
	MenuBar2[0]->hot = 'w';

	//
	MenuBar2[1]->text = "继续(C)";
	MenuBar2[1]->hot = 'c';
	MenuBar2[1]->isAct = TRUE;
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawMenuBar2();

函数功能：画出进程下拉选项卡
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
static void DrawMenuBar2()
{
	if (MenuBar0[1]->isDown)
	{
		if (MenuBar2[0]->isAct) {
			DrawButton(MenuBar2[1]);
		}
		else if (MenuBar2[1]->isAct) {
			DrawButton(MenuBar2[0]);
		}
	}
	else {
		SetEraseMode(TRUE);
		if (MenuBar2[0]->isAct) {
			DrawButton(MenuBar2[1]);
		}
		else if (MenuBar2[1]->isAct) {
			DrawButton(MenuBar2[0]);
		}
		SetEraseMode(FALSE);
	}
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InitButton(MenuBarButtonT bptr,
	double w,//按钮宽
	double h,//按钮高
	string BorderColor,//边框颜色
	string BorderColorOn,//被选中时边框颜色
	string BorderColorDown,//被按下时边框颜色
	string StringColor,//文字颜色
	string StringColorOn,//被选中时文字颜色
	string StringColorDown,//被按下时文字颜色
	string BackColor,//背景填充颜色
	string BackColorOn,//被选中时背景填充颜色
	string BackColorDown,//被按下时背景颜色
	int PenSize,//边框粗细
	int PointSize//文字大小
	);

函数功能：初始化按钮，对按钮除StartX、StartY、text、hot之外的成员赋值
参数说明：见上
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void InitButton(MenuBarButtonT bptr, double w, double h, string BorderColor, string BorderColorOn, string BorderColorDown, string StringColor, string StringColorOn, string StringColorDown, string BackColor, string BackColorOn, string BackColorDown, int PenSize, int PointSize)
{

	bptr->h = h;
	bptr->w = w;
	bptr->BackColor = BackColor;
	bptr->BackColorOn = BackColorOn;
	bptr->BackColorDown = BackColorDown;
	bptr->BorderColor = BorderColor;
	bptr->BorderColorOn = BorderColorOn;
	bptr->BorderColorDown = BorderColorDown;
	bptr->StringColor = StringColor;
	bptr->StringColorOn = StringColorOn;
	bptr->StringColorDown = StringColorDown;
	bptr->PenSize = PenSize;
	bptr->PointSize = PointSize;
	bptr->isOn = FALSE;
	bptr->isDown = FALSE;
	bptr->isAct = FALSE;
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawButton(MenuBarButtonT button);

函数功能：画一个按钮button
参数说明：要画的按钮的指针
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void DrawButton(MenuBarButtonT button)
{
	MenuBarButtonT bptr = button;
	int pensize = GetPenSize();//保存当前大小
	int pointsize = GetPointSize();
	string color = GetPenColor();//保存当前颜色

	if ((!bptr->isOn) && (!bptr->isDown))//鼠标没有停留且没有被选中
	{
		SetPenSize(bptr->PenSize);
		//画背景
		SetPenColor(bptr->BackColor);
		MovePen(bptr->StartX, bptr->StartY);
		StartFilledRegion(1);
		{
			DrawLine(0, bptr->h);
			DrawLine(bptr->w, 0);
			DrawLine(0, -bptr->h);
			DrawLine(-bptr->w, 0);
		}
		EndFilledRegion();
		//画边框
		SetPenColor(bptr->BorderColor);
		MovePen(bptr->StartX, bptr->StartY);
		DrawLine(bptr->w, 0);
		DrawLine(0, bptr->h);
		DrawLine(-bptr->w, 0);
		DrawLine(0, -bptr->h);
		//画文本
		SetPointSize(bptr->PointSize);
		SetPenColor(bptr->StringColor);
		MovePen(bptr->StartX + (bptr->w - TextStringWidth(bptr->text)) / 2, bptr->StartY + bptr->h / 2);
		DrawTextString(bptr->text);
	}
	if (bptr->isOn)//鼠标停留
	{
		SetPenSize(bptr->PenSize);
		//画背景
		SetPenColor(bptr->BackColorOn);
		MovePen(bptr->StartX, bptr->StartY);
		StartFilledRegion(1);
		{
			DrawLine(0, bptr->h);
			DrawLine(bptr->w, 0);
			DrawLine(0, -bptr->h);
			DrawLine(-bptr->w, 0);
		}
		EndFilledRegion();
		//画边框
		SetPenColor(bptr->BorderColorOn);
		MovePen(bptr->StartX, bptr->StartY);
		DrawLine(bptr->w, 0);
		DrawLine(0, bptr->h);
		DrawLine(-bptr->w, 0);
		DrawLine(0, -bptr->h);
		//画文本
		SetPointSize(bptr->PointSize);
		SetPenColor(bptr->StringColorOn);
		MovePen(bptr->StartX + (bptr->w - TextStringWidth(bptr->text)) / 2, bptr->StartY + bptr->h / 2);
		DrawTextString(bptr->text);
	}
	if (bptr->isDown)//鼠标按下
	{
		SetPenSize(bptr->PenSize);
		//画背景
		SetPenColor(bptr->BackColorDown);
		MovePen(bptr->StartX, bptr->StartY);
		StartFilledRegion(1);
		{
			DrawLine(0, bptr->h);
			DrawLine(bptr->w, 0);
			DrawLine(0, -bptr->h);
			DrawLine(-bptr->w, 0);
		}
		EndFilledRegion();
		//画边框
		SetPenColor(bptr->BorderColorDown);
		MovePen(bptr->StartX, bptr->StartY);
		DrawLine(bptr->w, 0);
		DrawLine(0, bptr->h);
		DrawLine(-bptr->w, 0);
		DrawLine(0, -bptr->h);
		//画文本
		SetPointSize(bptr->PointSize);
		SetPenColor(bptr->StringColorDown);
		MovePen(bptr->StartX + (bptr->w - TextStringWidth(bptr->text)) / 2, bptr->StartY + bptr->h / 2);
		DrawTextString(bptr->text);
	}
	//还原Pen
	SetPenSize(pensize);
	SetPenColor(color);
	SetPointSize(pointsize);
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
MenuBarButtonT IfOnButton(int x,int y);

函数功能：判断鼠标是否停留在按钮上
参数说明：
	x:鼠标返回的横坐标
	y;鼠标返回的纵坐标
	-->》参数同鼠标回调函数
返回值：鼠标当前位置所在的按钮指针或是NULL
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
MenuBarButtonT IfOnButton(int x, int y)
{
	double mx, my;//当前鼠标坐标
	double w = MenuBar0[0]->w;
	double h = MenuBar0[0]->h;
	int i;
	mx = ScaleXInches(x);/*pixels --> inches*/
	my = ScaleYInches(y);/*pixels --> inches*/
	for (i = 0; i < 3; i++) {//检查菜单栏
		if (mx > MenuBar0[i]->StartX&&mx<MenuBar0[i]->StartX + w && my>MenuBar0[i]->StartY) {
			MenuBar0[i]->isOn = TRUE;
			return MenuBar0[i];
		}
	}
	if (MenuBar0[0]->isDown) {//检查菜单下拉栏
		for (i = 0; i < 4; i++) {
			if (mx<w&&my>MenuBar1[i]->StartY&&my < MenuBar1[i]->StartY + h) {
				MenuBar1[i]->isOn = TRUE;
				return MenuBar1[i];
			}
		}
	}
	if (MenuBar0[1]->isDown) {//检查进程
		if (mx > w&&mx<w * 2 && my>MenuBar2[0]->StartY&&my < MenuBar0[1]->StartY) {
			if (MenuBar2[0]->isAct) {//暂停中，显示继续
				MenuBar2[1]->isOn = TRUE;
				return MenuBar2[1];
			}
			else if (MenuBar2[1]->isAct) {//继续中，显示暂停
				MenuBar2[0]->isOn = TRUE;
				return MenuBar2[0];
			}
		}
	}
	return NULL;
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Mouse_about_MenuBar(int x,int y,int button,int event);

函数功能：鼠标回调中调用此函数，判断鼠标与菜单栏相关的行为
参数说明：
	x:鼠标返回的横坐标
	y:鼠标返回的纵坐标
	button:鼠标按钮
	event:鼠标事件
	-->》参数同鼠标回调函数
返回值：
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
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
int Mouse_about_MenuBar(int x, int y, int button, int event)
{
	switch (event) {
	case MOUSEMOVE:
		if (MouseisDown == FALSE)
		{
			if (cueBarButton != NULL)
			{
				cueBarButton->isOn = FALSE;
				DrawButton(cueBarButton);
			}
			if (cueBarButton = IfOnButton(x, y)) {
				DrawButton(cueBarButton);
			}
			else if (preBarButton != NULL)
			{
				preBarButton->isDown = FALSE;
				DrawMenuBar0();
				preBarButton = NULL;
			}
		}
		break;
	case BUTTON_DOWN:
		if (!MouseisDown && !KeyisDown)
		{
			MouseisDown = TRUE;
			if (button == LEFT_BUTTON) {
				if (cueBarButton = IfOnButton(x, y)) {
					cueBarButton->isDown = !cueBarButton->isDown;
					DrawMenuBar0();
					if (cueBarButton->isDown)
						switch (cueBarButton->hot) {
						case 'm':
							if (MenuBar0[0]->isDown)
								preBarButton = cueBarButton;
							else
								preBarButton = NULL;
							break;
						case 'p':
							if (MenuBar0[1]->isDown)
								preBarButton = cueBarButton;
							else
								preBarButton = NULL;
							break;
						case 'n':
							//
							break;
						case 'l':
							//
							break;
						case 's':
							//
							break;
						case 'e':
							//
							break;
						case 'w':
							MenuBar2[1]->isAct = FALSE;
							MenuBar2[0]->isAct = TRUE;
							//
							break;
						case 'c':
							MenuBar2[0]->isAct = FALSE;
							MenuBar2[1]->isAct = TRUE;
							//
							break;
						}
				}
				else {
					for (int i = 0; i < 3; i++) {
						MenuBar0[i]->isDown = FALSE;
					}
				}
			}
		}
		break;
	case BUTTON_UP:
		if (MouseisDown && !KeyisDown)
		{
			MouseisDown = FALSE;
			if (button == LEFT_BUTTON)
			{
				if (cueBarButton) {
					switch (cueBarButton->hot) {
					case 'n':
						switch (MessageBox(NULL, TEXT("是否保存当前游戏进度？"), TEXT("开始新游戏"), MB_YESNOCANCEL)) {
						case 6://选择了是
							MenuBar1[0]->isDown = FALSE;
							DrawMenuBar0();
							//执行保存并设置
							return 1;
						case 7://选择了否
							MenuBar1[0]->isDown = FALSE;
							DrawMenuBar0();
							//执行设置
							return 2;
						case 2://选择了取消，返回游戏
							//
							break;
						}
						MenuBar1[0]->isDown = FALSE;
						break;
					case 'l':
						switch (MessageBox(NULL, TEXT("是否保存当前游戏进度？"), TEXT("载入存档"), MB_YESNOCANCEL)) {
						case 6://选择了是
							MenuBar1[1]->isDown = FALSE;
							DrawMenuBar0();
							//执行保存并载入
							return 3;
						case 7://选择了否
							MenuBar1[1]->isDown = FALSE;
							DrawMenuBar0();
							//执行载入
							return 4;
						case 2://选择了取消，返回游戏
							//
							break;
						}
						MenuBar1[1]->isDown = FALSE;
						break;
					case 's':
						//执行保存
						MenuBar1[2]->isDown = FALSE;
						DrawMenuBar0();
						return 5;
					case 'e':
						switch (MessageBox(NULL, TEXT("是否保存游戏进度？"), TEXT("退出"), MB_YESNOCANCEL)) {
						case 6://选择了是
							MenuBar1[3]->isDown = FALSE;
							DrawMenuBar0();
							//执行保存并退出
							return 6;
						case 7://选择了否，直接执行退出
							exit(-1);
						case 2://选择了取消，返回游戏
							MenuBar1[3]->isDown = FALSE;
							break;
						}
						break;
					case 'h'://选择帮助，显示规则
						//显示规则
						MenuBar0[2]->isDown = FALSE;
						DrawMenuBar0();
						return 7;
					case 'w'://选择暂停
						//暂停
						cueBarButton->isDown = FALSE;
						cueBarButton = MenuBar2[1];
						DrawMenuBar0();
						return 8;
					case 'c'://选择继续
						//继续
						cueBarButton->isDown = FALSE;
						cueBarButton = MenuBar2[0];
						DrawMenuBar0();
						return 9;
					}
				}
				DrawMenuBar0();
			}
		}
		break;
	}
	return 0;
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
int KeyBoard_about_MenuBar(int key,int event);

函数功能：键盘回调中调用此函数，判断键盘与菜单栏相关的行为
参数说明：
	key:键盘按键
	event:键盘事件
	-->》参数同键盘回调函数
返回值：
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
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
int KeyBoard_about_MenuBar(int key, int event)
{
	switch (event) {
	case KEY_DOWN:
		if (!KeyisDown && !MouseisDown)
		{
			KeyisDown = TRUE;
			cueBarButton = NULL;
			switch (key) {
			case 'M':
				cueBarButton = MenuBar0[0];
				break;
			case 'P':
				cueBarButton = MenuBar0[1];
				break;
			case 'H':
				cueBarButton = MenuBar0[2];
				break;
			case 'N':
				cueBarButton = MenuBar1[0];
				break;
			case 'L':
				cueBarButton = MenuBar1[1];
				break;
			case 'S':
				cueBarButton = MenuBar1[2];
				break;
			case 'E':
				cueBarButton = MenuBar1[3];
				break;
			case 'W':
				if (MenuBar2[1]->isAct)
					cueBarButton = MenuBar2[0];
				break;
			case 'C':
				if (MenuBar2[0]->isAct)
					cueBarButton = MenuBar2[1];
				break;
			}
			if (cueBarButton)
				cueBarButton->isDown = !cueBarButton->isDown;
			DrawMenuBar0();
		}
	case KEY_UP:
		if (KeyisDown && !MouseisDown)
		{
			KeyisDown = FALSE;
			switch (key) {
			case 'N':
				switch (MessageBox(NULL, TEXT("是否保存当前游戏进度？"), TEXT("开始新游戏"), MB_YESNOCANCEL)) {
				case 6://选择了是
					MenuBar1[0]->isDown = FALSE;
					DrawMenuBar0();
					cueBarButton = NULL;
					//执行保存并设置
					return 1;
				case 7://选择了否
					MenuBar1[0]->isDown = FALSE;
					DrawMenuBar0();
					cueBarButton = NULL;
					//执行设置
					return 2;
				case 2://选择了取消，返回游戏
					//
					break;
				}
				MenuBar1[0]->isDown = FALSE;
				break;
			case 'L':
				switch (MessageBox(NULL, TEXT("是否保存当前游戏进度？"), TEXT("载入存档"), MB_YESNOCANCEL)) {
				case 6://选择了是
					MenuBar1[1]->isDown = FALSE;
					DrawMenuBar0();
					cueBarButton = NULL;
					//执行保存并载入
					return 3;
				case 7://选择了否
					MenuBar1[1]->isDown = FALSE;
					DrawMenuBar0();
					cueBarButton = NULL;
					//执行载入
					return 4;
				case 2://选择了取消，返回游戏
					//
					break;
				}
				MenuBar1[1]->isDown = FALSE;
				break;
			case 'S':
				MenuBar1[2]->isDown = FALSE;
				DrawMenuBar0();
				cueBarButton = NULL;
				//执行保存
				return 5;
			case 'E':
				switch (MessageBox(NULL, TEXT("是否保存游戏进度？"), TEXT("退出"), MB_YESNOCANCEL)) {
				case 6://选择了是
					MenuBar1[3]->isDown = FALSE;
					DrawMenuBar0();
					cueBarButton = NULL;
					//执行保存并退出
					return 6;
				case 7://选择了否，直接退出
					exit(-1);
				case 2://选择了取消，返回游戏
					MenuBar1[3]->isDown = FALSE;
					break;
				}
				break;
			case 'H'://选择帮助，显示规则
				cueBarButton->isDown = FALSE;
				DrawMenuBar0();
				cueBarButton = NULL;
				return 7;
			case 'W'://选择暂停
				if (MenuBar2[1]->isAct) {
					//暂停
					MenuBar2[0]->isAct = TRUE;
					MenuBar2[1]->isAct = FALSE;
					cueBarButton->isDown = FALSE;
					DrawMenuBar0();
					cueBarButton = NULL;
					return 8;
				}
				break;
			case 'C'://选择继续
				if (MenuBar2[0]->isAct) {
					//继续
					MenuBar2[0]->isAct = FALSE;
					MenuBar2[1]->isAct = TRUE;
					cueBarButton->isDown = FALSE;
					DrawMenuBar0();
					cueBarButton = NULL;
					return 9;
				}
				break;
			}
			DrawMenuBar0();
			cueBarButton = NULL;
		}
	}
	return 0;
}