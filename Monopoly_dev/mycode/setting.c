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

#include "setting.h"

#define SYSCOLOR "Black"

//=========================================================================================================================================================================================================

typedef struct {
	int num;//按钮编号XXX
	double StartX, StartY, w, h;//按钮大小
	string BorCol;//边框颜色
	string BorColO;//
	string BorColD;//
	string BacCol;//背景颜色
	string BacColO;//
	string BacColD;//
	string TexCol;//文字颜色
	string TexColO;//
	string TexColD;//
	int PenSize;//边框粗细
	int PointSize;//文字大小
	string text;//文字内容
	bool isOn;//鼠标划过标志
	bool isDown;//鼠标按下标志

}*SetButtonT;

typedef struct {/*文本类型*/
	int num;//编号；
	string text;/*文本指针*/
	double x, y;/*文本显示起始位置坐标*/
	int PointSize; /*文字大小*/
	string color; /*颜色*/
	bool isSelected; /*选中*/
	int curPos; /*光标位置*/
	bool isDisplayed; /*光标是否处于显示状态*/
} *TextT;

//=========================================================================================================================================================================================================

static void InitB(//初始化按钮
	SetButtonT button, //按钮指针
	int num, //按钮编号
	double w, //按钮宽
	double h,//按钮高
	string BorCol,//边框颜色
	string BorColO,//
	string BorColD,//
	string BacCol,//背景颜色
	string BacColO,//
	string BacColD,//
	string TexCol,//文本颜色
	string TexColO,//
	string TexColD,//
	int pensize,//边框宽
	int pointsize,//文本大小
	string text//文本
);
static void InitBlockButton();//初始化玩家设置按钮
static void InitMapButton();//初始化地图选择按钮
static void InitStep();//初始化step按钮
static void InitName();//初始化文本框

static void DrawB(SetButtonT button);//画按钮
static void DrawBlock(int i);//画玩家设置界面
static int ButtonAct(int i, int j);//执行按钮行为

static SetButtonT IfOnButton(int x, int y);//判断鼠标位置
static TextT IfOnText(int x, int y);//判断鼠标位置

//=========================================================================================================================================================================================================

static SetButtonT curSetButton = NULL;//实时刷新当前按钮
static SetButtonT preSetButton = NULL;//执行行为的按钮
static SetButtonT BlockButton[4][18] = { NULL };//玩家设置界面按钮
static SetButtonT MapButton[3] = { NULL };//地图选择按钮
static SetButtonT StepButton[2] = { NULL };//step按钮
static string SetButtonText[18] = { "真人","电脑","无","圆","方","三角","菱","星","八边","灰","深蓝","绿","黄","红","橘","紫","蓝","粉" };//玩家设置文本
static string ButtonColor[9] = { "lightgrey","navyblue","grass","Yellow","Red","orange","purple","Blue","pink" };//玩家设置文本
static TextT name[4] = { NULL };//玩家昵称文本
static TextT curtext = NULL;/*当前编辑文本对象指针*/

struct PlayPeople * Player[4] = { NULL };//玩家结构
int Map = 0;//选择地图

static bool MouseisDown = FALSE;//鼠标按下标志

#define CURSOR "_"          /*光标符号*/
static void DrawCursor(string str, int curPos, double startx, double starty);//画光标
static void InsertCharToString(string str, int pos, char c);//pos位置插入字符
static void DeleteCharFromString(string str, int pos);//pos位置删除字符

#define TEXTLEN  10//
static bool inText = FALSE; /*是否处于当前文本编辑状态*/
static char textbuf[TEXTLEN + 1];/*当前文本缓冲区*/

//=========================================================================================================================================================================================================

/*画设置界面*/
void DrawSet()
{
	double ww, wh;
	InitGraphics();
	int pensize = GetPenSize();
	int pointsize = GetPointSize();
	string pencolor = GetPenColor();
	ww = GetWindowWidth();
	wh = GetWindowHeight();
	Map = 0;

	MovePen((ww - 38) / 2, 99);
	SetPenColor("Red");
	SetPointSize(50);
	DrawTextString("游戏设置");

	DrawBlock(0);
	InitBlockButton();
	InitMapButton();
	InitStep();
	InitName();
	InitPlayer();

	SetPointSize(pointsize);
	SetPenSize(pensize);
	SetPenColor(pencolor);
}

/*
--------------------------------------------------------------------------------------------------------------------------------------------------
int Mouse_about_Setting(int x,int y,int button,int event);

函数功能：鼠标回调中调用此函数，判断鼠标与设置界面相关的行为
参数说明：同鼠标回调函数
返回值：1-开始游戏，2-返回主菜单,0-others
--------------------------------------------------------------------------------------------------------------------------------------------------
*/
int Mouse_about_Setting(int x, int y, int button, int event)/*鼠标消息回调函数*/
{
	int i = 0;
	switch (event) {
	case MOUSEMOVE:
		if (MouseisDown == FALSE)
		{
			if (curSetButton != NULL)//
			{
				curSetButton->isOn = FALSE;
				DrawB(curSetButton);
			}
			if (curSetButton = IfOnButton(x, y))//
			{
				DrawB(curSetButton);
			}
		}
		break;
	case BUTTON_DOWN:
		if (!MouseisDown)
		{

			if (button == LEFT_BUTTON) {

				if (curSetButton = IfOnButton(x, y))//
				{
					MouseisDown = TRUE;
					curSetButton->isDown = TRUE;
					DrawB(curSetButton);
				}

				if (curtext)//
				{
					inText = FALSE;/*退出当前文本输入*/
					MovePen(curtext->x - 0.5, curtext->y - 0.5);
					SetPenColor("White");
					StartFilledRegion(1);
					{
						DrawLine(23, 0);
						DrawLine(0, 4);
						DrawLine(-23, 0);
						DrawLine(0, -4);
					}
					EndFilledRegion();
					SetPenColor("Black");
					curtext->text = CopyString(textbuf);
					MovePen(curtext->x, curtext->y);
					if (curtext->text[0] == '\0' && Player[curtext->num]->mode) DrawTextString("昵称为空！");
					else if (!Player[curtext->num]->mode) DrawTextString("无此玩家！");
					else DrawTextString(curtext->text);
					switch (curtext->num) {
					case 0:
						Player[0]->name = curtext->text;
						break;
					case 1:
						Player[1]->name = curtext->text;
						break;
					case 2:
						Player[2]->name = curtext->text;
						break;
					case 3:
						Player[3]->name = curtext->text;
						break;
					}
					curtext = NULL;
				}

				if (curtext = IfOnText(x, y))//
				{
					MouseisDown = TRUE;
					curtext->isSelected = TRUE;
					curtext->isDisplayed = TRUE;
					inText = TRUE;
					MovePen(curtext->x - 0.5, curtext->y - 0.5);
					strcpy(textbuf, curtext->text);/*将当前文本字符串拷贝到临时文本缓冲区以备编辑*/
					SetPenColor("White");
					StartFilledRegion(1);
					{
						DrawLine(23, 0);
						DrawLine(0, 4);
						DrawLine(-23, 0);
						DrawLine(0, -4);
					}
					EndFilledRegion();
					SetPenColor(curtext->color); /*设置选择文本的颜色*/
					MovePen(curtext->x, curtext->y);
					DrawTextString(curtext->text);
					DrawCursor(textbuf, curtext->curPos, curtext->x, curtext->y);/*显示当前文本光标*/
					curtext->isDisplayed = TRUE; /*设置当前文本光标闪烁标志*/
				}
			}
		}
		break;
	case BUTTON_UP:
		if (MouseisDown)
		{
			MouseisDown = FALSE;//取消mouse down的状态
			if (curSetButton)
			{
				if (curSetButton != IfOnButton(x, y))//长摁并拖到当前按钮范围外
					curSetButton->isOn = FALSE;

				preSetButton = curSetButton;

				i = ButtonAct(curSetButton->num / 100, curSetButton->num % 100);

				DrawB(preSetButton);//刷新显示
				preSetButton = NULL;

				curSetButton = IfOnButton(x, y);//刷新当前按钮
				if (curSetButton)//长摁并拖到另一个按钮上方时
				{
					curSetButton->isOn = TRUE;
					DrawB(curSetButton);
				}
				curSetButton = NULL;
				if (i) return i;
			}
		}
		break;
	}
	return 0;
}

/*
--------------------------------------------------------------------------------------------------------------------------------------------------
void KeyBoard_about_Setting(int key, int event);

函数说明：键盘回调中调用此函数，判断键盘与设置界面相关的行为
参数说明：同键盘回调函数
--------------------------------------------------------------------------------------------------------------------------------------------------
*/
void KeyBoard_about_Setting(int key, int event)
{
	switch (event) {//
		case KEY_DOWN:
			if(!MouseisDown)//
				switch (key) {
					case VK_LEFT://
						if (!inText) break;
						SetEraseMode(TRUE);
						MovePen(curtext->x, curtext->y);
						DrawTextString(textbuf);/*擦除当前文本*/
						DrawCursor(textbuf, curtext->curPos, curtext->x, curtext->y); /*擦除光标*/
						if (curtext->curPos > 0) curtext->curPos--;/*左移光标一个字符位置*/
						SetEraseMode(FALSE);
						MovePen(curtext->x, curtext->y);
						DrawTextString(textbuf);/*重新显示当前文本*/
						DrawCursor(textbuf, curtext->curPos, curtext->x, curtext->y); /*显示光标*/
						break;
					case VK_RIGHT: /*右移键*/
						if (!inText) break;/*不处于文本编辑状态*/
						SetEraseMode(TRUE);
						MovePen(curtext->x, curtext->y);
						DrawTextString(textbuf);/*擦除当前文本*/
						DrawCursor(textbuf, curtext->curPos, curtext->x, curtext->y); /*擦除光标*/
						if (curtext->curPos < strlen(textbuf)) curtext->curPos++;/*右移光标一个字符位置*/
						SetEraseMode(FALSE);
						MovePen(curtext->x, curtext->y);
						DrawTextString(textbuf);/*重新显示当前文本*/
						DrawCursor(textbuf, curtext->curPos, curtext->x, curtext->y); /*显示光标*/
						break;
				}
			break;
		case KEY_UP:
			break;
	}
}

/*
--------------------------------------------------------------------------------------------------------------------------------------------------
void Char_about_Setting(char c);

函数说明：字符回调中调用此函数，执行字符与设置界面相关的行为
参数说明：同字符回调函数
--------------------------------------------------------------------------------------------------------------------------------------------------
*/
void Char_about_Setting(char c)
{
	int len;

	if (!inText) return;
	switch (c) {
	case 27: /*ESC*/
	case '\r':  /* 注意：回车在这里返回的字符是'\r'，不是'\n'*/
		inText = FALSE;/*退出当前文本输入*/
		//
		SetEraseMode(TRUE);
		MovePen(curtext->x, curtext->y);
		DrawTextString(textbuf);
		DrawCursor(textbuf, curtext->curPos, curtext->x, curtext->y);/*擦除当前光标*/
		curtext->isDisplayed = FALSE;
		curtext->text = CopyString(textbuf);/*申请字符串空间*/
		SetEraseMode(FALSE);
		//
		MovePen(curtext->x, curtext->y);
		if (curtext->text[0] == '\0' && Player[curtext->num]->mode) DrawTextString("昵称为空！");
		else if (!Player[curtext->num]->mode) DrawTextString("无此玩家！");
		else DrawTextString(curtext->text);
		//
		switch (curtext->num) {
		case 0:
			Player[0]->name = curtext->text;
			break;
		case 1:
			Player[1]->name = curtext->text;
			break;
		case 2:
			Player[2]->name = curtext->text;
			break;
		case 3:
			Player[3]->name = curtext->text;
			break;
		}
		//
		SetPenColor(SYSCOLOR);
		break;
	case '\b':/*BACKSPACE*/
		if ((len = strlen(textbuf)) == 0) break;//
		//
		SetEraseMode(TRUE);
		MovePen(curtext->x, curtext->y);
		DrawTextString(textbuf);
		DrawCursor(textbuf, curtext->curPos, curtext->x, curtext->y);/*擦除当前光标*/
		DeleteCharFromString(textbuf, curtext->curPos - 1);
		SetEraseMode(FALSE);
		//
		MovePen(curtext->x, curtext->y);
		DrawTextString(textbuf);
		//
		if (curtext->curPos > 0) curtext->curPos--;
		DrawCursor(textbuf, curtext->curPos, curtext->x, curtext->y);/*显示当前光标*/
		break;
	default:
		if ((len = strlen(textbuf)) >= TEXTLEN) break;//
		//
		SetEraseMode(TRUE);
		MovePen(curtext->x, curtext->y);
		DrawTextString(textbuf);
		DrawCursor(textbuf, curtext->curPos, curtext->x, curtext->y);/*擦除当前光标*/
		InsertCharToString(textbuf, curtext->curPos, c);/*将当前字符插入到光标位置*/
		SetEraseMode(FALSE);
		//
		MovePen(curtext->x, curtext->y);
		DrawTextString(textbuf);
		curtext->curPos++;
		DrawCursor(textbuf, curtext->curPos, curtext->x, curtext->y);/*显示当前光标*/
		break;
	}
}

/*Insert char c to the string str at the position pos*/
static void InsertCharToString(string str, int pos, char c)
{
	int len;

	if (pos < 0 || pos >= TEXTLEN) return;
	len = strlen(str);
	*(str + len + 1) = '\0';/*插入后形成字符串*/
	while (len > pos) {
		*(str + len) = *(str + len - 1);
		len--;
	}
	*(str + len) = c;
	return;
}

/*Delete the character at the pos position of string str*/
static void DeleteCharFromString(string str, int pos)
{
	int len;

	len = strlen(str);
	if (pos < 0 || pos >= len) return;

	while (*(str + pos) != '\0') {
		*(str + pos) = *(str + pos + 1);
		pos++;
	}
	return;
}

/*画光标*/
static void DrawCursor(string str, int curPos, double startx, double starty)
{
	if (curPos < 0 || curPos > strlen(str)) return;
	MovePen(startx + TextStringWidth(SubString(str, 0, curPos - 1)), starty);
	DrawTextString(CURSOR);
	return;
}

/*
--------------------------------------------------------------------------------------------------------------------------------------------------
static SetButtonT IfOnButton(int x, int y);

函数说明：判断鼠标位置
--------------------------------------------------------------------------------------------------------------------------------------------------
*/
static SetButtonT IfOnButton(int x, int y)
{
	double mx, my;//当前鼠标坐标
	int i, j;
	mx = ScaleXInches(x);/*pixels --> inches*/
	my = ScaleYInches(y);/*pixels --> inches*/
	//玩家设置
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 18; j++)
		{
			if (mx >BlockButton[i][j]->StartX&&mx < BlockButton[i][j]->StartX + 6 && my > BlockButton[i][j]->StartY&&my < BlockButton[i][j]->StartY + 5)
			{
				BlockButton[i][j]->isOn = TRUE;
				return BlockButton[i][j];
			}
		}
	}
	//地图选择
	for (i = 0; i < 3; i++)
	{
		if (mx > MapButton[i]->StartX&&mx < MapButton[i]->StartX + 12 && my > MapButton[i]->StartY&&my < MapButton[i]->StartY + 6)
		{
			MapButton[i]->isOn = TRUE;
			return MapButton[i];
		}
	}
	//step
	for (i = 0; i < 2; i++)
	{
		if (mx > StepButton[i]->StartX&&mx < StepButton[i]->StartX + 12 && my > StepButton[i]->StartY&&my < StepButton[i]->StartY + 6)
		{
			StepButton[i]->isOn = TRUE;
			return StepButton[i];
		}
	}
	
	return NULL;
}

/*
--------------------------------------------------------------------------------------------------------------------------------------------------
static TextT IfOnText(int x, int y);

函数说明：判断鼠标位置
--------------------------------------------------------------------------------------------------------------------------------------------------
*/
static TextT IfOnText(int x, int y)
{
	double mx, my;//当前鼠标坐标
	int i;
	mx = ScaleXInches(x);/*pixels --> inches*/
	my = ScaleYInches(y);/*pixels --> inches*/
	for (i = 0; i < 4; i++)
	{
		if (mx > 20 + 48 * i && mx < 44 + 48 * i && my > 74 && my < 79)
		{
			return name[i];
		}
	}
	return NULL;
}

/*
--------------------------------------------------------------------------------------------------------------------------------------------------
static int ButtonAct(int i, int j);

函数说明：执行按钮行为
--------------------------------------------------------------------------------------------------------------------------------------------------
*/
static int ButtonAct(int i, int j)
{
	int k, l;
	int pensize = GetPenSize();//保存当前大小
	int pointsize = GetPointSize();
	string color = GetPenColor();//保存当前颜色

	if (i < 4)
	{
		switch (j) {
			case 0:
				Player[i]->mode = 1; break;
			case 1:
				Player[i]->mode = 2; break;
			case 2:
				Player[i]->mode = 0; break;
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
				Player[i]->shape = j-2;
				break;
			case 9:
				Player[i]->color = "lightgrey";
				break;
			case 10:
				Player[i]->color = "navyblue";
				break;
			case 11:
				Player[i]->color = "grass";
				break;
			case 12:
				Player[i]->color = "Yellow";
				break;
			case 13:
				Player[i]->color = "Red";
				break;
			case 14:
				Player[i]->color = "orange";
				break;
			case 15:
				Player[i]->color = "purple";
				break;
			case 16:
				Player[i]->color = "Blue";
				break;
			case 17:
				Player[i]->color = "pink";
				break;
		}
		if (j < 3)
		{
			for (k = 0; k < 3; k++)
			{
				if (k != j && BlockButton[i][k]->isDown == TRUE)
				{
					BlockButton[i][k]->isDown = FALSE;
					DrawB(BlockButton[i][k]);
				}
			}
		}
		else if (j < 9)
		{
			for (k = 3; k < 9; k++)
			{
				if (k != j && BlockButton[i][k]->isDown == TRUE)
				{
					BlockButton[i][k]->isDown = FALSE;
					DrawB(BlockButton[i][k]);
				}
			}
		}
		else
		{
			for (k = 9; k < 18; k++)
			{
				if (k != j && BlockButton[i][k]->isDown == TRUE)
				{
					BlockButton[i][k]->isDown = FALSE;
					DrawB(BlockButton[i][k]);
				}
			}
		}
	}
	if (i == 4)
	{
		Map = j + 1;
		for (k = 0; k < 3; k++)
		{
			if (k != j)
			{
				MapButton[k]->isDown = FALSE;
				DrawB(MapButton[k]);
			}
		}
	}
	if (i == 5)
	{
		StepButton[j]->isDown = FALSE;
		if (j)
		{
			l = 0;
			for (k = 0; k < 4; k++)
				if (Player[k]->mode)//k存在
				{
					l++;
					if (Player[k]->color == "White" || Player[k]->shape == 0)
					{
						MessageBox(NULL, TEXT("玩家形象未设置完整，请重新设置"), TEXT("嘤嘤嘤！出错了！"), MB_OK);
						return 0;
					}
					if (Player[k]->name[0] == '\0')
					{
						MessageBox(NULL, TEXT("玩家昵称为空，请重新设置"), TEXT("嘤嘤嘤！出错了！"), MB_OK);
						return 0;
					}
				}
			if (l <= 1)
			{
				MessageBox(NULL, TEXT("玩家数量过少，请重新设置"), TEXT("嘤嘤嘤！出错了！"), MB_OK); 
				return 0;
			}

			for (k = 0; k < 3; k++)
				for (l = k + 1; l < 4; l++)
					if (Player[k]->mode&&Player[l]->mode)//k和l都存在
					{
						if (Player[k]->color == Player[l]->color)
						{
							MessageBox(NULL, TEXT("玩家颜色有重叠，请重新设置"), TEXT("嘤嘤嘤！出错了！"), MB_OK);
							return 0;
						}
						if (StringEqual(Player[k]->name, Player[l]->name))
						{
							MessageBox(NULL, TEXT("玩家昵称有重叠，请重新设置"), TEXT("嘤嘤嘤！出错了！"), MB_OK);
							return 0;
						}
					}
			if (!Map) 
			{
				MessageBox(NULL, TEXT("未选择地图，请重新设置"), TEXT("嘤嘤嘤！出错了！"), MB_OK);
				return 0;
			}
			return 1;
		}
		else
			return 2;

	}

	//还原Pen
	SetPenSize(pensize);
	SetPenColor(color);
	SetPointSize(pointsize);
	return 0;
}

/*初始化按钮*/
static void InitB(SetButtonT button, int num, double w, double h,
	string BorCol,
	string BorColO,
	string BorColD,
	string BacCol,
	string BacColO,
	string BacColD,
	string TexCol,
	string TexColO,
	string TexColD,
	int pensize,
	int pointsize,
	string text
)
{
	SetButtonT bptr = button;
	bptr->BacCol = BacCol;
	bptr->BacColD = BacColD;
	bptr->BacColO = BacColO;
	bptr->BorCol = BorCol;
	bptr->BorColD = BorColD;
	bptr->BorColO = BorColO;
	bptr->TexCol = TexCol;
	bptr->TexColD = TexColD;
	bptr->TexColO = TexColO;
	bptr->h = h;
	bptr->w = w;
	bptr->text = text;
	bptr->PenSize = pensize;
	bptr->PointSize = pointsize;
	bptr->isOn = FALSE;
	bptr->isDown = FALSE;
	bptr->num = num;
}

/*画按钮*/
static void DrawB(SetButtonT button)
{
	SetButtonT bptr = button;
	int pensize = GetPenSize();//保存当前大小
	int pointsize = GetPointSize();
	string color = GetPenColor();//保存当前颜色
	if ((!bptr->isOn) && (!bptr->isDown))
	{
		SetPenSize(bptr->PenSize);
		//画背景
		SetPenColor(bptr->BacCol);
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
		SetPenColor(bptr->BorCol);
		MovePen(bptr->StartX, bptr->StartY);
		DrawLine(bptr->w, 0);
		DrawLine(0, bptr->h);
		DrawLine(-bptr->w, 0);
		DrawLine(0, -bptr->h);
		//画文本
		SetPointSize(bptr->PointSize);
		SetPenColor(bptr->TexCol);
		MovePen(bptr->StartX + (bptr->w - TextStringWidth(bptr->text)) / 2, bptr->StartY + bptr->h / 3);
		DrawTextString(bptr->text);
	}
	if (bptr->isOn)//鼠标停留
	{
		SetPenSize(bptr->PenSize);
		//画背景
		SetPenColor(bptr->BacColO);
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
		SetPenColor(bptr->BorColO);
		MovePen(bptr->StartX, bptr->StartY);
		DrawLine(bptr->w, 0);
		DrawLine(0, bptr->h);
		DrawLine(-bptr->w, 0);
		DrawLine(0, -bptr->h);
		//画文本
		SetPointSize(bptr->PointSize);
		SetPenColor(bptr->TexColO);
		MovePen(bptr->StartX + (bptr->w - TextStringWidth(bptr->text)) / 2, bptr->StartY + bptr->h / 3);
		DrawTextString(bptr->text);
	}
	if (bptr->isDown)//鼠标按下
	{
		SetPenSize(bptr->PenSize);
		//画背景
		SetPenColor(bptr->BacColD);
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
		SetPenColor(bptr->BorColD);
		MovePen(bptr->StartX, bptr->StartY);
		DrawLine(bptr->w, 0);
		DrawLine(0, bptr->h);
		DrawLine(-bptr->w, 0);
		DrawLine(0, -bptr->h);
		//画文本
		SetPointSize(bptr->PointSize);
		SetPenColor(bptr->TexColD);
		MovePen(bptr->StartX + (bptr->w - TextStringWidth(bptr->text)) / 2, bptr->StartY + bptr->h / 4);
		DrawTextString(bptr->text);
	}
	//还原Pen
	SetPenSize(pensize);
	SetPenColor(color);
	SetPointSize(pointsize);
	
}

/*画玩家设置界面*/
static void DrawBlock(int i)
{
	char text[10];
	SetPenColor("smoke");
	StartFilledRegion(1);
	{
		MovePen(5 + i * 48, 18);
		DrawLine(38, 0);
		DrawArc(2, -90, 90);
		DrawLine(0, 68);
		DrawArc(2, 0, 90);
		DrawLine(-38, 0);
		DrawArc(2, 90, 90);
		DrawLine(0, -68);
		DrawArc(2, 180, 90);
	}EndFilledRegion();

	SetPenColor("Black");
	SetPointSize(30);
	MovePen(6 + i * 48, 81);
	sprintf(text, "Player %d", i + 1);
	DrawTextString(text);

	SetPointSize(10);
	MovePen(6 + i * 48, 75);
	DrawTextString("玩家昵称");
	MovePen(6 + i * 48, 66);
	DrawTextString("玩家模式");
	MovePen(6 + i * 48, 57);
	DrawTextString("棋子形状");
	MovePen(6 + i * 48, 39);
	DrawTextString("棋子颜色");

	SetPenColor("White");
	SetPenSize(1);
	MovePen(20 + 48 * i, 74);
	StartFilledRegion(1);
	{
		DrawLine(24, 0);
		DrawLine(0, 5);
		DrawLine(-24, 0);
		DrawLine(0, -5);
	}
	EndFilledRegion();
	SetPenColor("Black");
	DrawLine(24, 0);
	DrawLine(0, 5);
	DrawLine(-24, 0);
	DrawLine(0, -5);
	MovePen(21 + 48 * i, 75);
	DrawTextString("请输入昵称,<10字符");

	if (i != 3)
		DrawBlock(i + 1);
	else
	{
		SetPenColor("smoke");
		StartFilledRegion(1);
		{
			MovePen(63, 3);
			DrawLine(64, 0);
			DrawArc(2, -90, 90);
			DrawLine(0, 8);
			DrawArc(2, 0, 90);
			DrawLine(-64, 0);
			DrawArc(2, 90, 90);
			DrawLine(0, -8);
			DrawArc(2, 180, 90);
		}EndFilledRegion();

		SetPenColor("Black");
		SetPointSize(10);
		MovePen(66, 8);
		DrawTextString("选择地图");
	}
}

/*初始化玩家设置按钮*/
static void InitBlockButton()
{
	int i, j;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 18; j++)
		{
			BlockButton[i][j] = GetBlock(sizeof(*BlockButton[i][j]));
			BlockButton[i][j]->w = 6;
			BlockButton[i][j]->h = 5;
			BlockButton[i][j]->BorCol = "Black";
			BlockButton[i][j]->BorColO = "Black";
			BlockButton[i][j]->BorColD = "Black";
			BlockButton[i][j]->TexCol = "Black";
			BlockButton[i][j]->TexColO = "Black";
			BlockButton[i][j]->TexColD = "White";
			BlockButton[i][j]->PenSize = 1;
			BlockButton[i][j]->PointSize = 3;
			if (j == 2) BlockButton[i][j]->isDown = TRUE;
			else BlockButton[i][j]->isDown = FALSE;
			BlockButton[i][j]->isOn = FALSE;
			BlockButton[i][j]->num = i * 100 + j;
			BlockButton[i][j]->text = SetButtonText[j];
			BlockButton[i][j]->StartY = 65 - 9 * (j / 3);
			BlockButton[i][j]->StartX = 20 + 48 * i + 9 * (j % 3);
		}
	}
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 9; j++)
		{
			BlockButton[i][j]->BacCol = "smoke";
			BlockButton[i][j]->BacColO = "babyblue";
			BlockButton[i][j]->BacColD = "lavender";
		}
		for (j = 9; j < 18; j++)
		{
			BlockButton[i][j]->BacCol = BlockButton[i][j]->BacColO = BlockButton[i][j]->BacColD = ButtonColor[j - 9];
		}
		
	}
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 18; j++)
			DrawB(BlockButton[i][j]);
	}
}

/*初始化选择地图按钮*/
static void InitMapButton()
{
	int i;
	for (i = 0; i < 3; i++)
	{
		MapButton[i] = GetBlock(sizeof(*MapButton[i]));
		MapButton[i]->w = 12;
		MapButton[i]->h = 6;
		MapButton[i]->StartX = 78 + 15 * i;
		MapButton[i]->StartY = 6;
		MapButton[i]->BorCol = MapButton[i]->BorColO = MapButton[i]->BorColD = "Black";
		MapButton[i]->BacCol = "smoke";
		MapButton[i]->BacColO = "babyblue";
		MapButton[i]->BacColD = "lavender";
		MapButton[i]->TexCol = MapButton[i]->TexColO = MapButton[i]->TexColD = "Black";
		MapButton[i]->num = 400 + i;
		switch (i) {
			case 0:
				MapButton[i]->text = "Map 1";
				break;
			case 1:
				MapButton[i]->text = "Map 2";
				break;
			case 2:
				MapButton[i]->text = "Map 3";
				break;
		}
		MapButton[i]->PenSize = 1;
		MapButton[i]->PointSize = 3;
		MapButton[i]->isDown = FALSE;
		MapButton[i]->isOn = FALSE;
		DrawB(MapButton[i]);
	}
}

/*初始化step按钮*/
static void InitStep()
{
	int i;
	for (i = 0; i < 2; i++)
	{
		StepButton[i] = GetBlock(sizeof(*StepButton[i]));
		StepButton[i]->BacCol = "pink";
		StepButton[i]->BacColO = "babyblue";
		StepButton[i]->BacColD = "lavender";
		StepButton[i]->BorCol = StepButton[i]->BorColO = StepButton[i]->BorColD = "Black";
		StepButton[i]->TexCol = StepButton[i]->TexColO = StepButton[i]->TexColD = "Black";
		StepButton[i]->h = 6;
		StepButton[i]->w = 12;
		StepButton[i]->num = 500 + i;
		StepButton[i]->PenSize = 5;
		StepButton[i]->PointSize = 10;
		if (i == 0) StepButton[i]->text = "回主菜单";
		else StepButton[i]->text = "开始游戏";
		StepButton[i]->StartX = 33 + 114 * i;
		StepButton[i]->StartY = 6;
		StepButton[i]->isDown = FALSE;
		StepButton[i]->isOn = FALSE;
		DrawB(StepButton[i]);
	}
}

/*初始化昵称*/
static void InitName()
{
	int i;
	for (i = 0; i < 4; i++)
	{
		name[i] = GetBlock(sizeof(*name[i]));
		name[i]->x = 21 + 48 * i;
		name[i]->y = 75;
		name[i]->color = "Black";
		name[i]->PointSize = 10;
		name[i]->curPos = 0;
		name[i]->isSelected = FALSE;
		name[i]->isDisplayed = FALSE;
		textbuf[0] = '\0';
		name[i]->text = CopyString(textbuf);
		name[i]->num = i;
	}
}

/*初始化玩家数据*/
void InitPlayer()
{
	int i;
	for (i = 0; i < 4; i++)
	{
		Player[i] = GetBlock(sizeof(*Player[i]));
		Player[i]->color = "White";
		Player[i]->money = 20000;
		Player[i]->place = 0;
		Player[i]->shape = 0;
		switch (i) {
			case 0:
				Player[i]->name = "Player1";
				break;
			case 1:
				Player[i]->name = "Player2";
				break;
			case 2:
				Player[i]->name = "Player3";
				break;
			case 3:
				Player[i]->name = "Player4";
				break;
		}
		Player[i]->mode = 0;
		Player[i]->wait = 0;
		Player[i]->isDefeated = FALSE;
	}
}