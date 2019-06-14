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

#include"Map1_demo.h"
#include"setting.h"
#include"load.h"

//=========================================================================================================================================================================================================

extern struct PlayPeople * Player[4];
extern Cell_AllT cell[38];
extern int Map;
extern int turn_player;

//=========================================================================================================================================================================================================

typedef struct ReadButton{
	int num;
	double StartX, StartY;
	string text;
	bool isOn;
	bool isDown;
}*ReadButtonT;

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

//=========================================================================================================================================================================================================

static string BackColor = "gingeryellow";
static string BackColorO = "Yellow";
static string BackColorD = "khaki";
static string BorderColor = "grass";
static string TextColor = "purple";
static int Pensize = 5;
static int Pointsize = 5;
static double bw = 12;
static double bh = 5;
static double fw = 70;
static double fh = 8;

static struct file *loadhead = NULL;//文件链表
static struct file *loadtail = NULL;

static int allReadFile = 0;//存档总数
static int startReadFile=0; 
static int size = sizeof(struct file);
static struct ReadButton* ButtonRead[2] = { NULL };
static bool MouseisDown = FALSE;
static struct ReadButton* curReadButton = NULL;
static struct file * curReadFile = NULL;
static struct file * preReadFile = NULL;

//=========================================================================================================================================================================================================

static void GetReadFiles();//得到文件链表
static void InsertFiles();//插入结点
static void DrawBack();//画背景
static void InitReadButton();
static void DrawReadButton(struct ReadButton* button);
static void DrawReadFiles();
static void GetContent();
static struct ReadButton* IfOnReadButton(int x, int y);//判断鼠标位置
static struct file * IfOnFiles(int x, int y);
static void DrawPlayer(struct file *p);

//=========================================================================================================================================================================================================

void DrawRead()
{
	allReadFile = 0;
	DrawBack();
	InitReadButton();
	DrawReadButton(ButtonRead[0]);
	DrawReadButton(ButtonRead[1]);
	GetReadFiles();
	DrawReadFiles();
	
}

int Mouse_about_Read(int x, int y, int button, int event)
{
	struct file* p;
	switch (event) {
		case MOUSEMOVE:
			if (!MouseisDown)
			{
				if (curReadButton)
				{
					curReadButton->isOn = FALSE;
					DrawReadButton(curReadButton);
				}
				else if (curReadFile)
				{
					curReadFile->isOn = FALSE;
					DrawReadFiles();
				}
				if (curReadButton = IfOnReadButton(x, y))
				{
					DrawReadButton(curReadButton);
				}
				else if (curReadFile = IfOnFiles(x, y))
				{
					DrawReadFiles();
				}
			}
			break;
		case BUTTON_DOWN:
			if (!MouseisDown)
			{
				MouseisDown = TRUE;
				if (button == LEFT_BUTTON)
				{
					if (curReadButton = IfOnReadButton(x, y))
					{
						curReadButton->isDown = TRUE;
						DrawReadButton(curReadButton);
					}
					else if (curReadFile = IfOnFiles(x, y))
					{
						curReadFile->isDown = TRUE;
						preReadFile = curReadFile;
						p = loadhead;
						while (p != NULL) {
							if (p != curReadFile)
								p->isDown = FALSE;
							p = p->next;
						}
						DrawReadFiles();
					}
				}
			}
			break;
		case BUTTON_UP:
			if (MouseisDown)
			{
				MouseisDown = FALSE;
				if (button == LEFT_BUTTON)
				{
					if (curReadButton)
					{
						curReadButton->isDown = FALSE;
						DrawReadButton(curReadButton);
						DrawReadButton(IfOnReadButton(x, y));
						if (curReadButton->num == 0)
						{
							if (loadhead != NULL)
							{//清空数据
								struct file *p1, *p2;
								p1 = loadhead;
								p2 = loadhead->next;
								while (p2 != NULL)
								{
									free(p1);
									p1 = p2;
									p2 = p2->next;
								}
								free(p1);
								loadhead = loadtail = NULL;
								curReadFile = NULL;
								preReadFile = NULL;
							}
							curReadButton = NULL;
							return 1;
						}
						else if (curReadButton->num == 1)
						{
							if (preReadFile == NULL)
								return 0;
							else
							{
								//加载preReadFile
								GetContent();
								if (loadhead != NULL)
								{//清空数据
									struct file *p1, *p2;
									p1 = loadhead;
									p2 = loadhead->next;
									while (p2 != NULL)
									{
										free(p1);
										p1 = p2;
										p2 = p2->next;
									}
									free(p1);
									loadhead = loadtail = NULL;
									curReadFile = NULL;
									preReadFile = NULL;
								}
								curReadButton = NULL;
								return 2;

							}
						}
					}
				}
			}
	}
	return 0;
}

static struct file * IfOnFiles(int x, int y)
{
	double mx, my;
	struct file* curReadFile;
	mx = ScaleXInches(x);/*pixels --> inches*/
	my = ScaleYInches(y);/*pixels --> inches*/
	for (int i = 1; i <= allReadFile; i++)
	{
		if (mx > 61 && mx < 131 && my > 80 - 10 * i && my < 80 - 10 * i + fh)
		{
			curReadFile = loadhead;
			while (curReadFile != NULL)
			{
				if (curReadFile->num == i)
				{
					curReadFile->isOn = TRUE;
					return curReadFile;
				}
				curReadFile = curReadFile->next;
			}
		}
	}
	return NULL;
}

static struct ReadButton* IfOnReadButton(int x, int y)
{
	double mx, my;
	mx = ScaleXInches(x);/*pixels --> inches*/
	my = ScaleYInches(y);/*pixels --> inches*/
	for(int i=0;i<2;i++)
		if (mx > ButtonRead[i]->StartX && mx < ButtonRead[i]->StartX + bw && my > ButtonRead[i]->StartY && my < ButtonRead[i]->StartY + bh)
		{
			ButtonRead[i]->isOn = TRUE;
			return ButtonRead[i];
		}
	return NULL;
}

static void DrawReadFiles()
{
	struct file *pf = loadhead;
	int pensize = GetPenSize();
	int pointsize = GetPointSize();
	string color = GetPenColor();
	while (pf != NULL)
	{
		MovePen(pf->StartX, pf->StartY);
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
			MovePen(pf->StartX + 10, pf->StartY + fh / 3);
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
			MovePen(pf->StartX + 10, pf->StartY + fh / 4);
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
			MovePen(pf->StartX + 10, pf->StartY + fh / 3);
			DrawTextString(pf->name);
		}
		DrawPlayer(pf);
		//
		char a[2];
		SetPenSize(2);
		MovePen(pf->StartX + 7, pf->StartY + fh / 2);
		SetPenColor("Black");
		DrawArc(2, 0, 360);
		sprintf(a, "%d", pf->map);
		SetPointSize(5);
		MovePen(pf->StartX + 4.5, pf->StartY + fh / 2.5);
		DrawTextString(a);
		pf = pf->next;
	}
	for (int i = allReadFile+1; i <= 6; i++)
	{
		MovePen(61, 80 - 10 * i);
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
	//还原Pen
	SetPenSize(pensize);
	SetPenColor(color);
	SetPointSize(pointsize);
}

static void DrawReadButton(struct ReadButton* button)
{
	struct ReadButton* bptr = button;
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

static void InitReadButton()
{
	for (int i = 0; i < 2; i++)
	{
		ButtonRead[i] = GetBlock(sizeof(*ButtonRead[i]));
		ButtonRead[i]->isDown = ButtonRead[i]->isOn = FALSE;
		ButtonRead[i]->num = i;
		ButtonRead[i]->StartX = 104 + 15 * i;
		ButtonRead[i]->StartY = 9;
	}
	ButtonRead[0]->text = "返回";
	ButtonRead[1]->text = "载入";
}

static void DrawBack()
{
	int pensize = GetPenSize();
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
	DrawTextString("载入存档");
	SetPenSize(pensize);
}

static void GetReadFiles()
{
	FILE *p;							//当前文件指针
	struct file *pf;							//当前结点指针
	char name[14];
	if ((p = fopen("files.txt", "r")) == NULL)
	{
		MessageBox(NULL, TEXT("缺少“files.txt”文件"), TEXT("Error"), MB_OK);
		return;
	}
	while (fgetc(p) != -1 && allReadFile < 7)
	{
		fseek(p, -1L, 1);
		allReadFile++;
		InsertFiles();
		pf = loadhead;
		while (pf->num != allReadFile)
			pf = pf->next;				//令pf为当前节点
		pf->namelen = fgetc(p) - '0';
		fseek(p, 1L, 1);
		fgets(pf->name, pf->namelen + 1, p);
		sprintf(name, "%s.txt", pf->name);
		if ((pf->fp = fopen(name, "r")) == NULL)
		{
			MessageBox(NULL, TEXT("存档文件打开失败"), TEXT("Error"), MB_OK);
		}
		else fclose(pf->fp);
		fseek(p, 1L, 1);
		for (int i = 0; i < 4; i++)
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
				int j = 0;
				while ((pf->color[i][j] = fgetc(p)) != ' ') {
					j++;
				}
				pf->color[i][j] = '\0';
			}
		}
		pf->map = fgetc(p) - '0';
		pf->isDown = pf->isOn = FALSE;
		pf->StartX = 61;
		pf->StartY = 80 - 10 * pf->num;
		if (fgetc(p) == -1)//吸收掉换行符
			break;
	}
	fclose(p);
}

static void InsertFiles()
{
	struct file *pf = (struct file *)malloc(size);
	pf->num = allReadFile;
	pf->next = NULL;
	if (loadhead == NULL) 
		loadhead = pf;
	else 
		loadtail->next = pf;
	loadtail = pf;
}

static void DrawPlayer(struct file *p)
{
	struct file *pf = p;
	char a[2];
	double x, y;
	double l = 2;
	y = pf->StartY + fh / 3;
	for (int i = 0; i < 4; i++)
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

static void GetContent()
{
	char name[14];
	char playername[10];
	int playernamelen;
	int i;
	FILE *fp;
	sprintf(name, "%s.txt", preReadFile->name);
	if ((fp = fopen(name, "r")) == NULL)
	{
		MessageBox(NULL, TEXT("存档文件打开失败"), TEXT("Error"), MB_OK);
		return;
	}
	InitPlayer();
	for (i = 0; i < 4; i++)
	{
		Player[i]->mode = preReadFile->playermode[i];
		if (Player[i]->mode)
		{
			Player[i]->shape = preReadFile->playershap[i];
			Player[i]->isDefeated = preReadFile->playercondition[i];
			Player[i]->color = CopyString(preReadFile->color[i]);
		}
		if (fgetc(fp) - '0' != 0)
		{
			fscanf(fp, "%d", &playernamelen);
			fseek(fp, 1L, 1);//跳过空格
			fgets(playername, playernamelen + 1, fp);//用fgets而不用fscanf是为了避免玩家昵称中含有空格
			Player[i]->name = CopyString(playername);
			fscanf(fp, "%d%d%d", &Player[i]->place, &Player[i]->money, &Player[i]->wait);
		}
		fgetc(fp);//跳过换行符
	}
	turn_player = fgetc(fp) - '0';
	InitCelldata();
	for (i = 0; i < 38; i++)
	{
		fgetc(fp);//跳过换行符与空格
		fscanf(fp, "%d%d", &cell[i]->owner, &cell[i]->house);
	}
	fclose(fp);
	Map = preReadFile->map;
}
