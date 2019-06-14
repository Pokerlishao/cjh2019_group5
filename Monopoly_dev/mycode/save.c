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

#include"setting.h"
#include"Map1_demo.h"
#include"save.h"

//=========================================================================================================================================================================================================

typedef struct SaveButton {
	int num;
	double StartX, StartY;
	string text;
	bool isOn;
	bool isDown;
}*ButtonT;

typedef struct file {
	int num;//编号
	double StartX, StartY;
	long namelen;
	char name[10];
	int playermode[4];
	bool playercondition[4];
	int playershap[4];
	char color[4][15];
	int map;
	bool isOn;
	bool isDown;
	FILE *fp;
	struct file *next;
}files;

typedef struct SaveText {/*文本类型*/
	char text[10];/*文本指针*/
	double x, y;/*文本显示起始位置坐标*/
	int PointSize; /*文字大小*/
	int curPos; /*光标位置*/
	bool isDisplayed; /*光标是否处于显示状态*/
} *TextT;

//=========================================================================================================================================================================================================

extern struct PlayPeople *Player[4];
extern Cell_AllT cell[38];
extern int Map;
extern int turn_player;

//=========================================================================================================================================================================================================

static string BackColor = "gingeryellow";//背景色 姜黄色
static string BackColorO = "Yellow";//背景色 On 黄色
static string BackColorD = "khaki";//背景色 Down 浅卡其色
static string BorderColor = "grass";//边框色 草绿
static string TextColor = "purple";//文本色 紫
static int Pensize = 5;//笔粗细
static int Pointsize = 5;//字大小
static double bw = 12;//按钮宽
static double bh = 5;//按钮高
static double fw = 70;//存档宽
static double fh = 8;//存档高
static struct SaveText* NAME = NULL;//存档文件命名
static files *savehead = NULL;//文件链表头
static files *savetail = NULL;//文件链表尾

static int allSaveFile = 0;//存档总数
static int startSaveFile = 0;//存档开始位置
static int size = sizeof(files);//空间大小
static struct SaveButton * ButtonSave[3] = { NULL };//按钮
static bool MouseisDown = FALSE;//鼠标按下标志
static struct SaveButton * curSaveButton = NULL;//实时刷新当前按钮
static files * curSaveFile = NULL;//当前存档
static files * preSaveFile = NULL;//替换存档

//=========================================================================================================================================================================================================

static void GetSaveFiles();//得到文件链表
static files* InsertFiles();//插入结点
static void DrawBack();//画背景
static void InitSaveButton();//初始化按钮
static void DrawSaveButton(struct SaveButton* button);//画按钮
static void DrawSaveFiles();//画存档
static void DrawSaveFiles1(files *pf);
static struct SaveButton* IfOnSaveButton(int x, int y);//判断鼠标位置
static int IfOnText(int x, int y);//判断鼠标位置
static files * IfOnFiles(int x, int y);//判断鼠标位置
static void DrawPlayer(files *p);//画玩家棋子
static void InitText();//初始化文本框
static int Replace();//替换存档
static int Newfile();//新建存档
static void RenewFiles();//刷新files文件
static void Delete();//删除存档

#define CURSOR "_"          /*光标符号*/
static void DrawCursor(string str, int curPos, double startx, double starty);//
static void InsertCharToString(string str, int pos, char c);//
static void DeleteCharFromString(string str, int pos);//

#define TEXTLEN  9//
static bool inText = FALSE; /*是否处于当前文本编辑状态*/
static char textbuf[TEXTLEN + 1];/*当前文本缓冲区*/

//=========================================================================================================================================================================================================

/*画保存界面*/
void DrawSave()
{
	allSaveFile = 0;
	InitSaveButton();
	InitText();
	DrawBack();

}

/*鼠标消息*/
int Mouse_about_Save(int x, int y, int button, int event)
{
	files* p;
	switch (event) {
	case MOUSEMOVE:
		if (!MouseisDown)
		{
			if (curSaveButton)
			{
				curSaveButton->isOn = FALSE;
				DrawSaveButton(curSaveButton);
			}
			else if (curSaveFile)
			{
				curSaveFile->isOn = FALSE;
				DrawSaveFiles();
			}
			if (curSaveButton = IfOnSaveButton(x, y))
			{
				DrawSaveButton(curSaveButton);
			}
			else if (curSaveFile = IfOnFiles(x, y))
			{
				DrawSaveFiles();
			}
		}
		break;
	case BUTTON_DOWN:
		if (!MouseisDown)
		{
			MouseisDown = TRUE;
			if (button == LEFT_BUTTON)
			{
				if (IfOnText(x, y))
				{
					inText = TRUE;
					MovePen(NAME->x - 0.5, NAME->y - 0.5);
					strcpy(textbuf, NAME->text);
					SetPenColor("White");
					StartFilledRegion(1);
					{
						DrawLine(54, 0);
						DrawLine(0, 5);
						DrawLine(-54, 0);
						DrawLine(0, -5);
					}
					EndFilledRegion();
					SetPenColor("Black"); /*设置选择文本的颜色*/
					MovePen(NAME->x, NAME->y);
					SetPointSize(NAME->PointSize);
					DrawTextString(NAME->text);
					DrawCursor(textbuf, NAME->curPos, NAME->x, NAME->y);/*显示当前文本光标*/
					NAME->isDisplayed = TRUE; /*设置当前文本光标闪烁标志*/
				}
				else if (inText)
				{
					inText = FALSE;/*退出当前文本输入*/
					//
					SetEraseMode(TRUE);
					MovePen(NAME->x, NAME->y);
					SetPointSize(NAME->PointSize);
					DrawTextString(textbuf);
					DrawCursor(textbuf, NAME->curPos, NAME->x, NAME->y);/*擦除当前光标*/
					NAME->isDisplayed = FALSE;
					strcpy(NAME->text, textbuf);/*申请字符串空间*/
					SetEraseMode(FALSE);
					//
					MovePen(NAME->x, NAME->y);
					SetPointSize(NAME->PointSize);
					if (NAME->text[0] == '\0') { SetPointSize(5); DrawTextString("文件名为空"); }
					else DrawTextString(NAME->text);
					//
				}
				if (curSaveButton = IfOnSaveButton(x, y))
				{
					curSaveButton->isDown = TRUE;
					DrawSaveButton(curSaveButton);
				}
				else if (curSaveFile = IfOnFiles(x, y))
				{
					curSaveFile->isDown = !curSaveFile->isDown;
					if (curSaveFile->isDown)
					{
						preSaveFile = curSaveFile;
						MovePen(NAME->x - 0.5, NAME->y - 0.5);
						SetPenColor("White");
						StartFilledRegion(1);
						{
							DrawLine(54, 0);
							DrawLine(0, 5);
							DrawLine(-54, 0);
							DrawLine(0, -5);
						}
						EndFilledRegion();
						SetPenColor("Black"); /*设置选择文本的颜色*/
						MovePen(NAME->x, NAME->y);
						SetPointSize(5);
						DrawTextString("替换存档，无需命名");
					}
					else
					{
						preSaveFile = NULL;
						MovePen(NAME->x - 0.5, NAME->y - 0.5);
						SetPenColor("White");
						StartFilledRegion(1);
						{
							DrawLine(54, 0);
							DrawLine(0, 5);
							DrawLine(-54, 0);
							DrawLine(0, -5);
						}
						EndFilledRegion();
						SetPenColor("Black"); /*设置选择文本的颜色*/
						MovePen(NAME->x, NAME->y);
						if (NAME->text[0] == '\0') { SetPointSize(5); DrawTextString("请输入文件名,<10字符"); }
						else { SetPointSize(24); DrawTextString(NAME->text); }
					}
					p = savehead;
					while (p != NULL) {
						if (p != curSaveFile)
							p->isDown = FALSE;
						p = p->next;
					}
					DrawSaveFiles();
				}

			}
		}
		break;
	case BUTTON_UP:
		if (MouseisDown)
		{
			MouseisDown = FALSE;
			startSaveFile--;
			if (button == LEFT_BUTTON)
			{
				if (curSaveButton)
				{
					curSaveButton->isDown = FALSE;
					DrawSaveButton(curSaveButton);
					DrawSaveButton(IfOnSaveButton(x, y));
					if (curSaveButton->num == 1)//点击了返回
					{
						//清空数据
						if (savehead != NULL)
						{
							files *p1, *p2;
							p1 = savehead;
							p2 = savehead->next;
							while (p2 != NULL)
							{
								free(p1);
								p1 = p2;
								p2 = p2->next;
							}
							free(p1);
							savehead = savetail = NULL;
							curSaveFile = NULL;
							preSaveFile = NULL;
						}
						curSaveButton = NULL;
						startSaveFile = 0;
						return 1;
					}
					else if (curSaveButton->num == 0)//点击了删除
					{
						if (preSaveFile)
						{
							if (MessageBox(NULL, TEXT("确定删除该存档？"), TEXT("删除"), MB_YESNO) == 6)
							{
								Delete();//删除结点
								RenewFiles();
								//刷新显示
								//清空数据
								files *p1, *p2;
								p1 = savehead;
								p2 = savehead->next;
								while (p2 != NULL)
								{
									free(p1);
									p1 = p2;
									p2 = p2->next;
								}
								free(p1);
								savehead = savetail = NULL;
								curSaveFile = NULL;
								preSaveFile = NULL;
								curSaveButton = NULL;
								GetSaveFiles();
								DrawSaveFiles();
							}
						}
						curSaveButton = NULL;
					}
					else if (curSaveButton->num == 2)//点击了保存
					{
						if (preSaveFile)
						{
							int i = MessageBox(NULL, TEXT("确定替换该存档？"), TEXT("替换"), MB_YESNO);
							if (i == 6)
							{
								//替换存档
								if (Replace())
								{
									//更新files.txt
									RenewFiles();
									//清空数据
									files *p1, *p2;
									p1 = savehead;
									p2 = savehead->next;
									while (p2 != NULL)
									{
										free(p1);
										p1 = p2;
										p2 = p2->next;
									}
									free(p1);
									savehead = savetail = NULL;
									curSaveFile = NULL;
									preSaveFile = NULL;
									curSaveButton = NULL;
									startSaveFile = 0;
									return 2;
								}
							}
							else if (i == 7)
							{
								return 0;//取消，返回
							}
						}
						else
						{
							if (NAME->text[0] == '\0')
							{
								MessageBox(NULL, TEXT("文件名不能为空！"), TEXT("Error"), MB_OK);
								return 0;
							}
							p = savehead;
							while (p != NULL)
							{
								if (strcmp(p->name, NAME->text) == 0)
								{
									MessageBox(NULL, TEXT("文件名重复！"), TEXT("Error"), MB_OK);
									return 0;
								}
								p = p->next;
							}

							{
								//保存存档
								if (Newfile())
								{
									//更新files.txt
									RenewFiles();
									//清空数据
									files *p1, *p2;
									p1 = savehead;
									p2 = savehead->next;
									while (p2 != NULL)
									{
										free(p1);
										p1 = p2;
										p2 = p2->next;
									}
									free(p1);
									savehead = savetail = NULL;
									curSaveFile = NULL;
									preSaveFile = NULL;
									curSaveButton = NULL;
									startSaveFile = 0;
									DrawSave();
									return 2;
								}
							}
						}
					}
				}
			}
		}
	case ROLL_DOWN:
		if (!MouseisDown && allSaveFile - startSaveFile > 5)
		{
			startSaveFile++;
			DrawSaveFiles();
		}
		break;
	case ROLL_UP:
		if (!MouseisDown && startSaveFile > 0)
		{
			startSaveFile--;
			DrawSaveFiles();
		}
		break;
	}
	return 0;
}

/*字符消息*/
void Char_about_Save(char c)
{
	int len;

	if (!inText) return;
	if (MouseisDown) return;
	switch (c) {
	case 27: /*ESC*/
	case '\r':  /* 注意：回车在这里返回的字符是'\r'，不是'\n'*/
		inText = FALSE;/*退出当前文本输入*/
		//
		SetEraseMode(TRUE);
		MovePen(NAME->x, NAME->y);
		SetPointSize(NAME->PointSize);
		DrawTextString(textbuf);
		DrawCursor(textbuf, NAME->curPos, NAME->x, NAME->y);/*擦除当前光标*/
		NAME->isDisplayed = FALSE;
		strcpy(NAME->text, textbuf);/*申请字符串空间*/
		SetEraseMode(FALSE);
		//
		MovePen(NAME->x, NAME->y);
		SetPointSize(5);
		if (preSaveFile != NULL)
			DrawTextString("替换存档，无需命名");
		else if (NAME->text[0] == '\0')
			DrawTextString("文件名为空");
		else { SetPointSize(NAME->PointSize); DrawTextString(NAME->text); }
		//

		break;
	case '\b':/*BACKSPACE*/
		if ((len = strlen(textbuf)) == 0) break;//
		//
		SetEraseMode(TRUE);
		MovePen(NAME->x, NAME->y);
		DrawTextString(textbuf);
		DrawCursor(textbuf, NAME->curPos, NAME->x, NAME->y);/*擦除当前光标*/
		DeleteCharFromString(textbuf, NAME->curPos - 1);
		SetEraseMode(FALSE);
		//
		MovePen(NAME->x, NAME->y);
		DrawTextString(textbuf);
		//
		if (NAME->curPos > 0) NAME->curPos--;
		DrawCursor(textbuf, NAME->curPos, NAME->x, NAME->y);/*显示当前光标*/
		break;
	default:
		if ((len = strlen(textbuf)) >= TEXTLEN) break;//
		//
		SetEraseMode(TRUE);
		MovePen(NAME->x, NAME->y);
		DrawTextString(textbuf);
		DrawCursor(textbuf, NAME->curPos, NAME->x, NAME->y);/*擦除当前光标*/
		InsertCharToString(textbuf, NAME->curPos, c);/*将当前字符插入到光标位置*/
		SetEraseMode(FALSE);
		//
		MovePen(NAME->x, NAME->y);
		DrawTextString(textbuf);
		NAME->curPos++;
		DrawCursor(textbuf, NAME->curPos, NAME->x, NAME->y);/*显示当前光标*/
		break;
	}
}

/*键盘消息*/
void Keyboard_about_Save(int key, int event)
{
	switch (event) {//
	case KEY_DOWN:
		if (!MouseisDown)//
			switch (key) {
			case VK_LEFT://
				if (!inText) break;
				SetEraseMode(TRUE);
				MovePen(NAME->x, NAME->y);
				DrawTextString(textbuf);/*擦除当前文本*/
				DrawCursor(textbuf, NAME->curPos, NAME->x, NAME->y); /*擦除光标*/
				if (NAME->curPos > 0) NAME->curPos--;/*左移光标一个字符位置*/
				SetEraseMode(FALSE);
				MovePen(NAME->x, NAME->y);
				DrawTextString(textbuf);/*重新显示当前文本*/
				DrawCursor(textbuf, NAME->curPos, NAME->x, NAME->y); /*显示光标*/
				break;
			case VK_RIGHT: /*右移键*/
				if (!inText) break;/*不处于文本编辑状态*/
				SetEraseMode(TRUE);
				MovePen(NAME->x, NAME->y);
				DrawTextString(textbuf);/*擦除当前文本*/
				DrawCursor(textbuf, NAME->curPos, NAME->x, NAME->y); /*擦除光标*/
				if (NAME->curPos < strlen(textbuf)) NAME->curPos++;/*右移光标一个字符位置*/
				SetEraseMode(FALSE);
				MovePen(NAME->x, NAME->y);
				DrawTextString(textbuf);/*重新显示当前文本*/
				DrawCursor(textbuf, NAME->curPos, NAME->x, NAME->y); /*显示光标*/
				break;
			}
		break;
	case KEY_UP:
		break;
	}
}

/*判断鼠标位置*/
static files * IfOnFiles(int x, int y)
{
	double mx, my;
	int i;
	files* curSaveFile;
	mx = ScaleXInches(x);/*pixels --> inches*/
	my = ScaleYInches(y);/*pixels --> inches*/
	for (i = 1; i <= allSaveFile; i++)
	{
		if (mx > 61 && mx < 131 && my > 85 - 10 * (i - startSaveFile) && my < 85 - 10 * (i - startSaveFile) + fh)
		{
			curSaveFile = savehead;
			while (curSaveFile != NULL)
			{
				if (curSaveFile->num == i)
				{
					curSaveFile->isOn = TRUE;
					return curSaveFile;
				}
				curSaveFile = curSaveFile->next;
			}
		}
	}
	return NULL;
}

/*判断鼠标位置*/
static struct SaveButton* IfOnSaveButton(int x, int y)
{
	double mx, my;
	int i;
	mx = ScaleXInches(x);/*pixels --> inches*/
	my = ScaleYInches(y);/*pixels --> inches*/
	for (i = 0; i < 3; i++)
		if (mx > ButtonSave[i]->StartX && mx < ButtonSave[i]->StartX + bw && my > ButtonSave[i]->StartY && my < ButtonSave[i]->StartY + bh)
		{
			ButtonSave[i]->isOn = TRUE;
			return ButtonSave[i];
		}
	return NULL;
}

/*画存档*/
static void DrawSaveFiles()
{
	int i;
	struct file *pf = savehead;
	int pensize = GetPenSize();
	int pointsize = GetPointSize();
	string color = GetPenColor();
	if (startSaveFile + 1 > allSaveFile && allSaveFile) return;
	while (pf != NULL && pf->num != startSaveFile + 1) pf = pf->next;
	if (allSaveFile - startSaveFile >= 6) {
		while (pf != NULL && pf->num - startSaveFile <= 6)
		{
			DrawSaveFiles1(pf);
			pf = pf->next;
		}
	}
	else
	{
		while (pf != NULL)
		{
			DrawSaveFiles1(pf);
			pf = pf->next;
		}
		for (i = allSaveFile - startSaveFile + 1; i <= 6; i++)
		{
			MovePen(61, 85 - 10 * i);
			SetPenColor(BackColor);
			StartFilledRegion(1);
			{
				DrawLine(0, fh);
				DrawLine(fw, 0);
				DrawLine(0, -fh);
				DrawLine(-fw, 0);
			}
			EndFilledRegion();
			SetPenColor(BorderColor);
			SetPenSize(Pensize);
			DrawLine(0, fh);
			DrawLine(fw, 0);
			DrawLine(0, -fh);
			DrawLine(-fw, 0);
		}
	}
	//还原Pen
	SetPenSize(pensize);
	SetPenColor(color);
	SetPointSize(pointsize);
}

static void DrawSaveFiles1(files *pf)
{
	MovePen(pf->StartX, pf->StartY - 10 * (pf->num - startSaveFile));
	if (!pf->isOn && !pf->isDown)
	{
		SetPenColor(BackColor);
		SetPenSize(1);
		StartFilledRegion(1);
		{
			DrawLine(0, fh);
			DrawLine(fw, 0);
			DrawLine(0, -fh);
			DrawLine(-fw, 0);
		}
		EndFilledRegion();
		SetPenColor("purple");
		SetPenSize(5);
		DrawLine(0, fh);
		DrawLine(fw, 0);
		DrawLine(0, -fh);
		DrawLine(-fw, 0);
		//画文本
		SetPointSize(30);
		SetPenColor(TextColor);
		MovePen(pf->StartX + 10, pf->StartY - 10 * (pf->num - startSaveFile) + fh / 3);
		DrawTextString(pf->name);
	}
	else if (pf->isDown)
	{
		SetPenColor("khaki");
		SetPenSize(1);
		StartFilledRegion(1);
		{
			DrawLine(0, fh);
			DrawLine(fw, 0);
			DrawLine(0, -fh);
			DrawLine(-fw, 0);
		}
		EndFilledRegion();
		SetPenColor("purple");
		SetPenSize(5);
		DrawLine(0, fh);
		DrawLine(fw, 0);
		DrawLine(0, -fh);
		DrawLine(-fw, 0);
		//画文本
		SetPointSize(30);
		SetPenColor(TextColor);
		MovePen(pf->StartX + 10, pf->StartY - 10 * (pf->num - startSaveFile) + fh / 4);
		DrawTextString(pf->name);
	}
	else if (pf->isOn)
	{
		SetPenColor("babypink");
		SetPenSize(1);
		StartFilledRegion(1);
		{
			DrawLine(0, fh);
			DrawLine(fw, 0);
			DrawLine(0, -fh);
			DrawLine(-fw, 0);
		}
		EndFilledRegion();
		SetPenColor("purple");
		SetPenSize(5);
		DrawLine(0, fh);
		DrawLine(fw, 0);
		DrawLine(0, -fh);
		DrawLine(-fw, 0);
		//画文本
		SetPointSize(30);
		SetPenColor(TextColor);
		MovePen(pf->StartX + 10, pf->StartY - 10 * (pf->num - startSaveFile) + fh / 3);
		DrawTextString(pf->name);
	}
	DrawPlayer(pf);
	//
	char a[2];
	SetPenSize(2);
	MovePen(pf->StartX + 7, pf->StartY - 10 * (pf->num - startSaveFile) + fh / 2);
	SetPenColor("Black");
	DrawArc(2, 0, 360);
	sprintf(a, "%d", pf->map);
	SetPointSize(5);
	MovePen(pf->StartX + 4.5, pf->StartY - 10 * (pf->num - startSaveFile) + fh / 2.5);
	DrawTextString(a);
}
/*画按钮*/
static void DrawSaveButton(struct SaveButton* button)
{
	struct SaveButton* bptr = button;
	int pensize = GetPenSize();
	int pointsize = GetPointSize();
	string color = GetPenColor();
	if (bptr == NULL) return;
	MovePen(bptr->StartX, bptr->StartY);
	SetPenSize(1);
	if (!bptr->isOn && !bptr->isDown)
	{
		SetPenColor(BackColor);
		StartFilledRegion(1);
		{
			DrawLine(0, bh);
			DrawLine(bw, 0);
			DrawLine(0, -bh);
			DrawLine(-bw, 0);
		}
		EndFilledRegion();
		SetPenColor(BorderColor);
		SetPenSize(Pensize);
		DrawLine(0, bh);
		DrawLine(bw, 0);
		DrawLine(0, -bh);
		DrawLine(-bw, 0);
		//画文本
		SetPointSize(Pointsize);
		SetPenColor(TextColor);
		MovePen(bptr->StartX + (bw - TextStringWidth(bptr->text)) / 2, bptr->StartY + bh / 3);
		DrawTextString(bptr->text);
	}
	else if (bptr->isDown)
	{
		SetPenColor(BackColorD);
		StartFilledRegion(1);
		{
			DrawLine(0, bh);
			DrawLine(bw, 0);
			DrawLine(0, -bh);
			DrawLine(-bw, 0);
		}
		EndFilledRegion();
		SetPenColor(BorderColor);
		SetPenSize(Pensize);
		DrawLine(0, bh);
		DrawLine(bw, 0);
		DrawLine(0, -bh);
		DrawLine(-bw, 0);
		//画文本
		SetPointSize(Pointsize);
		SetPenColor(TextColor);
		MovePen(bptr->StartX + (bw - TextStringWidth(bptr->text)) / 2, bptr->StartY + bh / 4);
		DrawTextString(bptr->text);
	}
	else if (bptr->isOn)
	{
		SetPenColor(BackColorO);
		StartFilledRegion(1);
		{
			DrawLine(0, bh);
			DrawLine(bw, 0);
			DrawLine(0, -bh);
			DrawLine(-bw, 0);
		}
		EndFilledRegion();
		SetPenColor(BorderColor);
		SetPenSize(Pensize);
		DrawLine(0, bh);
		DrawLine(bw, 0);
		DrawLine(0, -bh);
		DrawLine(-bw, 0);
		//画文本
		SetPointSize(Pointsize);
		SetPenColor(TextColor);
		MovePen(bptr->StartX + (bw - TextStringWidth(bptr->text)) / 2, bptr->StartY + bh / 3);
		DrawTextString(bptr->text);
	}

	//还原Pen
	SetPenSize(pensize);
	SetPenColor(color);
	SetPointSize(pointsize);
}

/*初始化按钮*/
static void InitSaveButton()
{
	int i;
	for (i = 0; i < 3; i++)
	{
		ButtonSave[i] = GetBlock(sizeof(*ButtonSave[i]));
		ButtonSave[i]->isDown = ButtonSave[i]->isOn = FALSE;
		ButtonSave[i]->num = i;
		ButtonSave[i]->StartX = 89 + 15 * i;
		ButtonSave[i]->StartY = 9;
	}
	ButtonSave[0]->text = "删除";
	ButtonSave[1]->text = "返回";
	ButtonSave[2]->text = "保存";
}

/*画背景*/
static void DrawBack()
{
	int pensize = GetPenSize();
	string color = GetPenColor();
	int pointsize = GetPointSize();
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
	SetPenSize(Pensize);
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
	SetPenColor("Red");
	SetPointSize(25);
	MovePen(87, 90);
	DrawTextString("保存游戏");
	MovePen(61, 85);
	SetPointSize(5);
	DrawTextString("已有存档：");
	MovePen(61, 20);
	DrawTextString("命名存档：");

	SetPenSize(2);
	SetPenColor("White");
	MovePen(76, 17);
	StartFilledRegion(1);
	{
		DrawLine(55, 0);
		DrawLine(0, 6);
		DrawLine(-55, 0);
		DrawLine(0, -6);
	}
	EndFilledRegion();
	SetPenColor("Black");
	DrawLine(55, 0);
	DrawLine(0, 6);
	DrawLine(-55, 0);
	DrawLine(0, -6);
	MovePen(77, 18);
	if (NAME->text[0] == '\0')
		DrawTextString("请输入文件名,<10字符");
	else
		DrawTextString(NAME->text);

	DrawSaveButton(ButtonSave[0]);
	DrawSaveButton(ButtonSave[1]);
	DrawSaveButton(ButtonSave[2]);

	SetPenSize(pensize);
	SetPenColor(color);
	SetPointSize(pointsize);

	GetSaveFiles();
	DrawSaveFiles();
}

/*获得存档*/
static void GetSaveFiles()
{
	FILE *p;							//当前文件指针
	files *pf;							//当前结点指针
	int i;
	allSaveFile = 0;
	if ((p = fopen("files.txt", "r")) == NULL)
	{
		MessageBox(NULL, TEXT("缺少“files.txt”文件"), TEXT("Error"), MB_OK);
		p = fopen("files.txt", "w");
		fclose(p);
		p=NULL;
		return;
	}
	while (fgetc(p) != -1)
	{
		fseek(p, -1L, 1);
		allSaveFile++;
		pf = InsertFiles();
		//令pf为当前节点
		pf->namelen = fgetc(p) - '0';
		fseek(p, 1L, 1);
		fgets(pf->name, pf->namelen + 1, p);
		fseek(p, 1L, 1);
		for (i = 0; i < 4; i++)
		{
			pf->playermode[i] = fgetc(p) - '0';
			fseek(p, 1L, 1);
			if (pf->playermode[i])
			{
				if (fgetc(p) - '0' == 0) pf->playercondition[i] = FALSE;
				else pf->playercondition[i] = TRUE;
				fseek(p, 1L, 1);
				pf->playershap[i] = fgetc(p) - '0';
				fseek(p, 1L, 1);
				fscanf(p, "%s", pf->color[i]);
				fseek(p, 1L, 1);
			}
		}
		pf->map = fgetc(p) - '0';
		pf->isDown = pf->isOn = FALSE;
		pf->StartX = 61;
		pf->StartY = 85;
		if (fgetc(p) == -1)//吸收掉换行符
			break;
	}
	fclose(p);
	p=NULL;
}

/*新增结点*/
static files* InsertFiles()
{
	files *pf = (files *)malloc(size);
	pf->num = allSaveFile;
	pf->next = NULL;
	if (savehead == NULL) savehead = pf;
	else savetail->next = pf;
	savetail = pf;
	return pf;
}

/*画棋子*/
static void DrawPlayer(files *p)
{
	files *pf = p;
	char a[2];
	int i;
	double x, y;
	double l = 2;
	y = pf->StartY - 10 * (pf->num - startSaveFile) + fh / 3;
	for (i = 0; i < 4; i++)
	{
		if (pf->playermode[i])
		{
			sprintf(a, "%d", i + 1);
			x = 111 + 5 * i;
			MovePen(x, y);
			SetPenColor(pf->color[i]);
			SetPenSize(1);
			StartFilledRegion(1);
			{
				switch (pf->playershap[i]) {
				case 1:
					MovePen(x + 2, y);
					DrawArc(2, 0, 360);
					break;
				case 2:
					MovePen(x + 1.5, y - 1.5);
					DrawLine(0, 3);
					DrawLine(-3, 0);
					DrawLine(0, -3);
					DrawLine(3, 0);
					break;
				case 3:
					MovePen(x - 2, y - 2);
					DrawLine(2 * 2, 0);
					DrawLine(-2, 2 * 2);
					DrawLine(-2, -2 * 2);
					break;
				case 4:
					MovePen(x + l, y);
					DrawLine(-l, l);
					DrawLine(-l, -l);
					DrawLine(l, -l);
					DrawLine(l, l);
					break;
				case 5:
					MovePen(x + l, y);
					DrawLine(-l * 2 / 3, l / 3);
					DrawLine(-l / 3, l * 2 / 3);
					DrawLine(-l / 3, -l * 2 / 3);
					DrawLine(-l * 2 / 3, -l / 3);
					DrawLine(l * 2 / 3, -l / 3);
					DrawLine(l / 3, -l * 2 / 3);
					DrawLine(l / 3, l * 2 / 3);
					DrawLine(l * 2 / 3, l / 3);
					break;
				case 6:
					MovePen(x + 2, y - 1);
					DrawLine(0, 2);
					DrawLine(-1, 1);
					DrawLine(-2, 0);
					DrawLine(-1, -1);
					DrawLine(0, -2);
					DrawLine(1, -1);
					DrawLine(2, 0);
					DrawLine(1, 1);
					break;
				}
			}
			EndFilledRegion();

			MovePen(x - 0.5, y - 1);
			SetPointSize(1);
			SetPenColor("Black");
			DrawTextString(a);
			if (pf->playermode[i] == 2)
			{
				MovePen(x - 2, y + 2);
				DrawTextString("AI");
			}
			if (pf->playercondition[i])
			{
				MovePen(x, y + 2);
				DrawTextString("败");
			}
		}
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

/*判断鼠标位置*/
static int IfOnText(int x, int y)
{
	double mx, my;//当前鼠标坐标
	mx = ScaleXInches(x);/*pixels --> inches*/
	my = ScaleYInches(y);/*pixels --> inches*/
	if (mx > 76 && mx < 131 && my > 17 && my < 22)
	{
		return 1;
	}
	return 0;
}

/*初始化文本*/
static void InitText()
{
	NAME = GetBlock(sizeof(*NAME));
	NAME->x = 77;
	NAME->y = 18;
	NAME->curPos = 0;
	NAME->PointSize = 24;
	NAME->isDisplayed = FALSE;
	textbuf[0] = '\0';
	strcpy(NAME->text, textbuf);/*申请字符串空间*/
}

/*替换存档*/
static int Replace()
{
	char name[14];
	int i;
	sprintf(name, "%s.txt", preSaveFile->name);
	if ((preSaveFile->fp = fopen(name, "w")) == NULL)
	{
		MessageBox(NULL, TEXT("替换存档失败"), TEXT("Error"), MB_OK);
		return 0;
	}
	else
	{
		for (i = 0; i < 4; i++)
		{
			fputc(Player[i]->mode + '0', preSaveFile->fp);
			preSaveFile->playermode[i] = Player[i]->mode;
			if (Player[i]->mode)
			{
				fprintf(preSaveFile->fp, " %d %s %d %d %d", strlen(Player[i]->name), Player[i]->name, Player[i]->place, Player[i]->money, Player[i]->wait);
				preSaveFile->playershap[i] = Player[i]->shape;
				preSaveFile->playercondition[i] = Player[i]->isDefeated;
				strcpy(preSaveFile->color[i], Player[i]->color);
			}
			fputc('\n', preSaveFile->fp);
		}
		fprintf(preSaveFile->fp, "%d\n", turn_player);
		for (i = 0; i < 38; i++)
		{
			fprintf(preSaveFile->fp, "%d %d", cell[i]->owner, cell[i]->house);
			if (i != 37) fputc('\n', preSaveFile->fp);
		}
		fclose(preSaveFile->fp);
		preSaveFile->fp=NULL;
		preSaveFile->map = Map;
	}
	return 1;
}

/*新建存档*/
static int Newfile()
{
	char name[14];
	FILE *fp;
	files* p;
	int i;
	p = InsertFiles();
	sprintf(name, "%s.txt", NAME->text);
	if ((fp = fopen(name, "w")) == NULL)
	{
		MessageBox(NULL, TEXT("新建存档失败"), TEXT("Error"), MB_OK);
		return 0;
	}
	else
	{
		for (i = 0; i < 4; i++)
		{
			fputc(Player[i]->mode + '0', fp);
			p->playermode[i] = Player[i]->mode;
			if (Player[i]->mode)
			{
				fprintf(fp, " %d %s %d %d %d", strlen(Player[i]->name), Player[i]->name, Player[i]->place, Player[i]->money, Player[i]->wait);
				p->playercondition[i] = Player[i]->isDefeated;
				p->playershap[i] = Player[i]->shape;
				strcpy(p->color[i], Player[i]->color);
			}
			fputc('\n', fp);
		}
		fprintf(fp, "%d\n", turn_player);
		for (i = 0; i < 38; i++)
		{
			fprintf(fp, "%d %d", cell[i]->owner, cell[i]->house);
			if (i != 37) fputc('\n', fp);
		}
		fclose(fp);
		fp=NULL;
		strcpy(p->name, NAME->text);
		p->isDown = p->isOn = FALSE;
		p->map = Map;
		p->namelen = strlen(p->name);
		p->num = allSaveFile + 1;
		allSaveFile++;
	}
	return 1;
}

/*刷新存档*/
static void RenewFiles()
{
	FILE *fp;
	files* p;
	int i;

	fp = fopen("files.txt", "w");
	p = savehead;
	while (p != NULL)
	{
		if (p->num != 1)
			fputc('\n', fp);
		fprintf(fp, "%c %s ", p->namelen + '0', p->name);
		for (i = 0; i < 4; i++)
		{
			fprintf(fp, "%c ", p->playermode[i] + '0');
			if (p->playermode[i])
			{
				if (p->playercondition[i])
					fputc('1', fp);
				else fputc('0', fp);
				fprintf(fp, " %c %s ", p->playershap[i] + '0', p->color[i]);
			}
		}

		fputc(p->map + '0', fp);
		p = p->next;
	}
	fclose(fp);
	fp=NULL;
}
/*删除存档*/
static void Delete()
{
	files * p;
	if (preSaveFile == savehead)
	{
		savehead = savehead->next;
		if (savehead == NULL)//删除后链表为空
			savetail = NULL;
	}
	else {
		p = savehead;
		while (p->next != preSaveFile)
			p = p->next;
		p->next = preSaveFile->next;
		while (p->next != NULL)//更新节点编号、tail
		{
			p->next->num--;
			p = p->next;
		}
		savetail = p;
	}
	free(preSaveFile);//释放prefile
	preSaveFile = NULL;
	allSaveFile--;
}
