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

#include "help.h"

//=========================================================================================================================================================================================================

typedef struct {
	int num;
	double StartX, StartY;
	
	string text;
	int state;//0-正常;1-鼠标停留;2-鼠标按下;
}*HelpButtonT;

//=========================================================================================================================================================================================================

static string BackColor = "gingeryellow";
static string BackColorO = "Yellow";
static string BackColorD = "khaki";
static string BorderColor = "grass";
static string TextColor = "purple";
static int pensize = 5;
static int pointsize = 5;
static double w = 12;
static double h = 5;
static int page = 1;

static HelpButtonT ButtonHelp[3] = { NULL };
static HelpButtonT curHelpButton = NULL;
static bool isDown = FALSE;

static void InitHelpButton();
static void DrawHelpButton(HelpButtonT b);
static void DrawRule();
static void DrawBox();

static HelpButtonT IfOnHelpButton(int x, int y);

//=========================================================================================================================================================================================================

void DrawHelp()
{
	InitHelpButton();
	DrawBox();
}

int Mouse_about_Help(int x, int y, int button, int event)
{
	switch (event) {
	case MOUSEMOVE:
		if (!isDown)
		{
			if (curHelpButton != NULL)
			{
				curHelpButton->state = 0;
				switch (page) {
				case 1:
					if (curHelpButton->num == 0) break;
					DrawHelpButton(curHelpButton);
				case 3:
					if (curHelpButton->num == 1) break;
					DrawHelpButton(curHelpButton);
				case 2:
					DrawHelpButton(curHelpButton);
				}
			}
			if (curHelpButton = IfOnHelpButton(x, y))
			{
				DrawHelpButton(curHelpButton);
			}
		}
		break;
	case BUTTON_DOWN:
		if (!isDown)
		{
			isDown = TRUE;
			if (button == LEFT_BUTTON)
			{
				if (curHelpButton = IfOnHelpButton(x, y))
				{
					curHelpButton->state = 2;
					DrawHelpButton(curHelpButton);
				}
			}
		}
		break;
	case BUTTON_UP:
		if (isDown)
		{
			isDown = FALSE;
			if (button == LEFT_BUTTON)
			{
				if (curHelpButton)
				{
					curHelpButton->state = 0;
					DrawHelpButton(curHelpButton);
					DrawHelpButton(IfOnHelpButton(x, y));
					Pause(0.2);
					if (curHelpButton->num == 2)
					{
						curHelpButton = NULL;
						return 1;
					}
					switch (page)
					{
					case 1:
					case 3:
						page = 2;
						break;
					case 2:
						if (curHelpButton->num == 0)
							page = 1;
						else page = 3;
						break;
					}
					DrawBox();
				}
			}
		}
		break;
	}
	return 0;
}

static void InitHelpButton()
{
	int i;
	for (i = 0; i < 3; i++)
	{
		ButtonHelp[i] = GetBlock(sizeof(*ButtonHelp[i]));
		ButtonHelp[i]->num = i;
		ButtonHelp[i]->StartY = 9;
		ButtonHelp[i]->state = 0;
	}
	ButtonHelp[0]->StartX = 89;
	ButtonHelp[1]->StartX = 104;
	ButtonHelp[2]->StartX = 119;
	ButtonHelp[0]->text = "上一页";
	ButtonHelp[1]->text = "下一页";
	ButtonHelp[2]->text = "我懂了";
}

static void DrawHelpButton(HelpButtonT b)
{
	HelpButtonT bptr = b;
	int Pensize = GetPenSize();//保存当前大小
	int Pointsize = GetPointSize();
	string color = GetPenColor();//保存当前颜色
	if (b)
	{
		if (!bptr->state)
		{
			SetPenSize(pensize);
			//画背景
			SetPenColor(BackColor);
			MovePen(bptr->StartX, bptr->StartY);
			StartFilledRegion(1);
			{
				DrawLine(0, h);
				DrawLine(w, 0);
				DrawLine(0, -h);
				DrawLine(-w, 0);
			}
			EndFilledRegion();
			//画文本
			SetPointSize(pointsize);
			SetPenColor(TextColor);
			MovePen(bptr->StartX + (w - TextStringWidth(bptr->text)) / 2, bptr->StartY + h / 3);
			DrawTextString(bptr->text);
		}
		if (bptr->state == 1)//鼠标停留
		{
			SetPenSize(pensize);
			//画背景
			SetPenColor(BackColorO);
			MovePen(bptr->StartX, bptr->StartY);
			StartFilledRegion(1);
			{
				DrawLine(0, h);
				DrawLine(w, 0);
				DrawLine(0, -h);
				DrawLine(-w, 0);
			}
			EndFilledRegion();
			//画文本
			SetPointSize(pointsize);
			SetPenColor(TextColor);
			MovePen(bptr->StartX + (w - TextStringWidth(bptr->text)) / 2, bptr->StartY + h / 3);
			DrawTextString(bptr->text);
		}

		if (bptr->state == 2)//鼠标按下
		{
			SetPenSize(pensize);
			//画背景
			SetPenColor(BackColorD);
			MovePen(bptr->StartX, bptr->StartY);
			StartFilledRegion(1);
			{
				DrawLine(0, h);
				DrawLine(w, 0);
				DrawLine(0, -h);
				DrawLine(-w, 0);
			}
			EndFilledRegion();
			//画文本
			SetPointSize(pointsize);
			SetPenColor(TextColor);
			MovePen(bptr->StartX + (w - TextStringWidth(bptr->text)) / 2, bptr->StartY + h / 4);
			DrawTextString(bptr->text);
		}
		//画边框
		SetPenSize(pensize);
		SetPenColor(BorderColor);
		MovePen(bptr->StartX, bptr->StartY);
		DrawLine(w, 0);
		DrawLine(0, h);
		DrawLine(-w, 0);
		DrawLine(0, -h);

		//还原Pen
		SetPenSize(Pensize);
		SetPenColor(color);
		SetPointSize(Pointsize);
	}
}

static void DrawBox()//57,78;6,90;2
{
	SetPenSize(pensize + 5);
	SetPenColor("smoke");
	MovePen(59, 6);
	StartFilledRegion(1);
	{
		DrawLine(74, 0);
		DrawArc(2, -90, 90);
		DrawLine(0, 86);
		DrawArc(2, 0, 90);
		DrawLine(-74, 0);
		DrawArc(2, 90, 90);
		DrawLine(0, -86);
		DrawArc(2, 180, 90);
	}
	EndFilledRegion();
	SetPenColor("purple");
	{
		DrawLine(74, 0);
		DrawArc(2, -90, 90);
		DrawLine(0, 86);
		DrawArc(2, 0, 90);
		DrawLine(-74, 0);
		DrawArc(2, 90, 90);
		DrawLine(0, -86);
		DrawArc(2, 180, 90);
	}
	SetPenColor("rred");
	SetPointSize(25);
	MovePen(87, 90);
	DrawTextString("游戏规则");
	if (page == 1)
	{
		DrawHelpButton(ButtonHelp[1]);//the next page
		SetPointSize(5);
		MovePen(73, 12);
		DrawTextString("1 / 3");
	}
	else if(page == 3)
	{
		DrawHelpButton(ButtonHelp[0]);//the last page
		SetPointSize(5);
		MovePen(73, 12);
		DrawTextString("3 / 3");
	}
	else if(page == 2)
	{
		DrawHelpButton(ButtonHelp[0]);//the last page
		DrawHelpButton(ButtonHelp[1]);//the next page
		SetPointSize(5);
		MovePen(73, 12);
		DrawTextString("2 / 3");
	}
	DrawHelpButton(ButtonHelp[2]);
	DrawRule();
}

static void DrawRule()
{
	SetPointSize(1);
	
	if (page == 1)
	{
		SetPenColor("Black");
		MovePen(65, 87);
		DrawTextString("这是一款简易的大富翁多人游戏。玩家可以选择不同游戏地图、");
		MovePen(60, 82);
		DrawTextString("设置游戏角色后进入游戏。每位玩家有20000元初始资产，一同从起");
		MovePen(60, 77);
		DrawTextString("点出发，通过转动转盘及交易策略，买地、盖房以赚取租金。游戏");
		MovePen(60, 72);
		DrawTextString("最后一位没有破产的玩家得胜。");
		MovePen(60, 67);
		DrawTextString("游戏进行:");
		MovePen(60, 42);
		DrawTextString("地图事件说明:");
		MovePen(60, 37);
		DrawTextString("1. 起点格");
		MovePen(60, 27);
		DrawTextString("2. 土地格");

		SetPenColor("rred");
		MovePen(60, 62);
		DrawTextString("每次轮到你时，游戏依照以下的顺序进行:");
		MovePen(60, 57);
		DrawTextString("1. 顺序敲击「空格」-「回车」，旋转数字转盘。");
		MovePen(60, 52);
		DrawTextString("2. 棋子依照点数前进。");
		MovePen(60, 47);
		DrawTextString("3. 执行抵达停留地点格的事件。");
		MovePen(60, 32);
		DrawTextString("每个人经过「起点格」时，可向银行领取两千金币。");
		MovePen(60, 22);
		DrawTextString("a. 停留「空地」时，可以按照标示价钱向银行买下土地。");
		MovePen(60, 17);
		DrawTextString("b. 停留别人的土地时，必须依照这块土地当时的状况，给予对方地");
	}
	else if (page == 2)
	{
		SetPenColor("rred");
		MovePen(60, 87);
		DrawTextString("产卡上规定的「过路费」。");
		MovePen(60, 82);
		DrawTextString("c.停留自己的土地时，可选择是否缴纳地产卡上面规定的金额以加");
		MovePen(60, 77);
		DrawTextString("盖「房屋」，一块土地至多盖四栋房屋。");
		MovePen(60, 67);
		DrawTextString("停留命运格的玩家将随机抽取一张命运卡，依照卡片的叙述执行。");
		MovePen(60, 57);
		DrawTextString("停留所得税格时，缴交10%现金给银行。");
		MovePen(60, 47);
		DrawTextString("停留时可照游戏盘上标示的价钱购买。购买火车站的玩家为其拥有");
		MovePen(60, 42);
		DrawTextString("者，玩家停留可以选择是否花费500元去相邻车站或1000元去相对");
		MovePen(60, 37);
		DrawTextString("车站。车费归土地主人。");
		MovePen(60, 27);
		DrawTextString("可照游戏盘上的价钱购买。玩家停留将付出500元电/水费给拥有者，");
		MovePen(60, 22);
		DrawTextString("若拥有者同时持有两厂地产，其他玩家停留需支付1000元给拥有者。");

		SetPenColor("Black");
		MovePen(60, 72);
		DrawTextString("3. 命运格");
		MovePen(60, 62);
		DrawTextString("4. 所得税");
		MovePen(60, 52);
		DrawTextString("5. 火车站");
		MovePen(60, 32);
		DrawTextString("6. 电力公司与自来水厂");
		MovePen(60, 17);
		DrawTextString("7. 医院格");
	}
	else if (page == 3)
	{
		SetPenColor("rred");
		MovePen(60, 87);
		DrawTextString("玩家住院处，单纯路过或停留无事发生");
		MovePen(60, 77);
		DrawTextString("玩家服刑处，单纯路过或停留无事发生");
		MovePen(60, 67);
		DrawTextString("无事发生");
		MovePen(60, 52);
		DrawTextString("系统自动以半价收购该玩家所有房屋，若无房可卖则系统自动将");
		MovePen(60, 47);
		DrawTextString("该玩家持有的所有土地以半价卖给银行直至无地可卖为止。");
		MovePen(60, 37);
		DrawTextString("当玩家变卖所有资产亦不足以支付欠款时，该玩家破产，退出游戏。");

		SetPenColor("Black");
		MovePen(60, 82);
		DrawTextString("8. 监狱格");
		MovePen(60, 72);
		DrawTextString("9. 停车场");
		MovePen(60, 62);
		DrawTextString("其他");
		MovePen(60, 57);
		DrawTextString("1. 现金不足");
		MovePen(60, 42);
		DrawTextString("2. 破产");
	}
}

static HelpButtonT IfOnHelpButton(int x, int y)
{
	double mx, my;//当前鼠标坐标
	mx = ScaleXInches(x);/*pixels --> inches*/
	my = ScaleYInches(y);/*pixels --> inches*/
	if (page == 1)
	{
		if (mx > ButtonHelp[1]->StartX && mx < ButtonHelp[1]->StartX + w && my > ButtonHelp[1]->StartY && my < ButtonHelp[1]->StartY + h)
		{
			ButtonHelp[1]->state = 1;
			return ButtonHelp[1];
		}
	}
	else if (page == 2)
	{
		if (mx > ButtonHelp[1]->StartX && mx < ButtonHelp[1]->StartX + w && my > ButtonHelp[1]->StartY && my < ButtonHelp[1]->StartY + h)
		{
			ButtonHelp[1]->state = 1;
			return ButtonHelp[1];
		}
		else if (mx > ButtonHelp[0]->StartX && mx < ButtonHelp[0]->StartX + w && my > ButtonHelp[0]->StartY && my < ButtonHelp[0]->StartY + h)
		{
			ButtonHelp[0]->state = 1;
			return ButtonHelp[0];
		}
	}
	else if (page == 3)
	{
		if (mx > ButtonHelp[0]->StartX && mx < ButtonHelp[0]->StartX + w && my > ButtonHelp[0]->StartY && my < ButtonHelp[0]->StartY + h)
		{
			ButtonHelp[0]->state = 1;
			return ButtonHelp[0];
		}
	}
	if (mx > ButtonHelp[2]->StartX && mx < ButtonHelp[2]->StartX + w && my > ButtonHelp[2]->StartY && my < ButtonHelp[2]->StartY + h)
	{
		ButtonHelp[2]->state = 1;
		return ButtonHelp[2];
	}
	return NULL;
}