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
	int num;//��ť���XXX
	double StartX, StartY, w, h;//��ť��С
	string BorCol;//�߿���ɫ
	string BorColO;//
	string BorColD;//
	string BacCol;//������ɫ
	string BacColO;//
	string BacColD;//
	string TexCol;//������ɫ
	string TexColO;//
	string TexColD;//
	int PenSize;//�߿��ϸ
	int PointSize;//���ִ�С
	string text;//��������
	bool isOn;//��껮����־
	bool isDown;//��갴�±�־

}*SetButtonT;

typedef struct {/*�ı�����*/
	int num;//��ţ�
	string text;/*�ı�ָ��*/
	double x, y;/*�ı���ʾ��ʼλ������*/
	int PointSize; /*���ִ�С*/
	string color; /*��ɫ*/
	bool isSelected; /*ѡ��*/
	int curPos; /*���λ��*/
	bool isDisplayed; /*����Ƿ�����ʾ״̬*/
} *TextT;

//=========================================================================================================================================================================================================

static void InitB(//��ʼ����ť
	SetButtonT button, //��ťָ��
	int num, //��ť���
	double w, //��ť��
	double h,//��ť��
	string BorCol,//�߿���ɫ
	string BorColO,//
	string BorColD,//
	string BacCol,//������ɫ
	string BacColO,//
	string BacColD,//
	string TexCol,//�ı���ɫ
	string TexColO,//
	string TexColD,//
	int pensize,//�߿��
	int pointsize,//�ı���С
	string text//�ı�
);
static void InitBlockButton();//��ʼ��������ð�ť
static void InitMapButton();//��ʼ����ͼѡ��ť
static void InitStep();//��ʼ��step��ť
static void InitName();//��ʼ���ı���

static void DrawB(SetButtonT button);//����ť
static void DrawBlock(int i);//��������ý���
static int ButtonAct(int i, int j);//ִ�а�ť��Ϊ

static SetButtonT IfOnButton(int x, int y);//�ж����λ��
static TextT IfOnText(int x, int y);//�ж����λ��

//=========================================================================================================================================================================================================

static SetButtonT curSetButton = NULL;//ʵʱˢ�µ�ǰ��ť
static SetButtonT preSetButton = NULL;//ִ����Ϊ�İ�ť
static SetButtonT BlockButton[4][18] = { NULL };//������ý��水ť
static SetButtonT MapButton[3] = { NULL };//��ͼѡ��ť
static SetButtonT StepButton[2] = { NULL };//step��ť
static string SetButtonText[18] = { "����","����","��","Բ","��","����","��","��","�˱�","��","����","��","��","��","��","��","��","��" };//��������ı�
static string ButtonColor[9] = { "lightgrey","navyblue","grass","Yellow","Red","orange","purple","Blue","pink" };//��������ı�
static TextT name[4] = { NULL };//����ǳ��ı�
static TextT curtext = NULL;/*��ǰ�༭�ı�����ָ��*/

struct PlayPeople * Player[4] = { NULL };//��ҽṹ
int Map = 0;//ѡ���ͼ

static bool MouseisDown = FALSE;//��갴�±�־

#define CURSOR "_"          /*������*/
static void DrawCursor(string str, int curPos, double startx, double starty);//�����
static void InsertCharToString(string str, int pos, char c);//posλ�ò����ַ�
static void DeleteCharFromString(string str, int pos);//posλ��ɾ���ַ�

#define TEXTLEN  10//
static bool inText = FALSE; /*�Ƿ��ڵ�ǰ�ı��༭״̬*/
static char textbuf[TEXTLEN + 1];/*��ǰ�ı�������*/

//=========================================================================================================================================================================================================

/*�����ý���*/
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
	DrawTextString("��Ϸ����");

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

�������ܣ����ص��е��ô˺������ж���������ý�����ص���Ϊ
����˵����ͬ���ص�����
����ֵ��1-��ʼ��Ϸ��2-�������˵�,0-others
--------------------------------------------------------------------------------------------------------------------------------------------------
*/
int Mouse_about_Setting(int x, int y, int button, int event)/*�����Ϣ�ص�����*/
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
					inText = FALSE;/*�˳���ǰ�ı�����*/
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
					if (curtext->text[0] == '\0' && Player[curtext->num]->mode) DrawTextString("�ǳ�Ϊ�գ�");
					else if (!Player[curtext->num]->mode) DrawTextString("�޴���ң�");
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
					strcpy(textbuf, curtext->text);/*����ǰ�ı��ַ�����������ʱ�ı��������Ա��༭*/
					SetPenColor("White");
					StartFilledRegion(1);
					{
						DrawLine(23, 0);
						DrawLine(0, 4);
						DrawLine(-23, 0);
						DrawLine(0, -4);
					}
					EndFilledRegion();
					SetPenColor(curtext->color); /*����ѡ���ı�����ɫ*/
					MovePen(curtext->x, curtext->y);
					DrawTextString(curtext->text);
					DrawCursor(textbuf, curtext->curPos, curtext->x, curtext->y);/*��ʾ��ǰ�ı����*/
					curtext->isDisplayed = TRUE; /*���õ�ǰ�ı������˸��־*/
				}
			}
		}
		break;
	case BUTTON_UP:
		if (MouseisDown)
		{
			MouseisDown = FALSE;//ȡ��mouse down��״̬
			if (curSetButton)
			{
				if (curSetButton != IfOnButton(x, y))//�������ϵ���ǰ��ť��Χ��
					curSetButton->isOn = FALSE;

				preSetButton = curSetButton;

				i = ButtonAct(curSetButton->num / 100, curSetButton->num % 100);

				DrawB(preSetButton);//ˢ����ʾ
				preSetButton = NULL;

				curSetButton = IfOnButton(x, y);//ˢ�µ�ǰ��ť
				if (curSetButton)//�������ϵ���һ����ť�Ϸ�ʱ
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

����˵�������̻ص��е��ô˺������жϼ��������ý�����ص���Ϊ
����˵����ͬ���̻ص�����
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
						DrawTextString(textbuf);/*������ǰ�ı�*/
						DrawCursor(textbuf, curtext->curPos, curtext->x, curtext->y); /*�������*/
						if (curtext->curPos > 0) curtext->curPos--;/*���ƹ��һ���ַ�λ��*/
						SetEraseMode(FALSE);
						MovePen(curtext->x, curtext->y);
						DrawTextString(textbuf);/*������ʾ��ǰ�ı�*/
						DrawCursor(textbuf, curtext->curPos, curtext->x, curtext->y); /*��ʾ���*/
						break;
					case VK_RIGHT: /*���Ƽ�*/
						if (!inText) break;/*�������ı��༭״̬*/
						SetEraseMode(TRUE);
						MovePen(curtext->x, curtext->y);
						DrawTextString(textbuf);/*������ǰ�ı�*/
						DrawCursor(textbuf, curtext->curPos, curtext->x, curtext->y); /*�������*/
						if (curtext->curPos < strlen(textbuf)) curtext->curPos++;/*���ƹ��һ���ַ�λ��*/
						SetEraseMode(FALSE);
						MovePen(curtext->x, curtext->y);
						DrawTextString(textbuf);/*������ʾ��ǰ�ı�*/
						DrawCursor(textbuf, curtext->curPos, curtext->x, curtext->y); /*��ʾ���*/
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

����˵�����ַ��ص��е��ô˺�����ִ���ַ������ý�����ص���Ϊ
����˵����ͬ�ַ��ص�����
--------------------------------------------------------------------------------------------------------------------------------------------------
*/
void Char_about_Setting(char c)
{
	int len;

	if (!inText) return;
	switch (c) {
	case 27: /*ESC*/
	case '\r':  /* ע�⣺�س������ﷵ�ص��ַ���'\r'������'\n'*/
		inText = FALSE;/*�˳���ǰ�ı�����*/
		//
		SetEraseMode(TRUE);
		MovePen(curtext->x, curtext->y);
		DrawTextString(textbuf);
		DrawCursor(textbuf, curtext->curPos, curtext->x, curtext->y);/*������ǰ���*/
		curtext->isDisplayed = FALSE;
		curtext->text = CopyString(textbuf);/*�����ַ����ռ�*/
		SetEraseMode(FALSE);
		//
		MovePen(curtext->x, curtext->y);
		if (curtext->text[0] == '\0' && Player[curtext->num]->mode) DrawTextString("�ǳ�Ϊ�գ�");
		else if (!Player[curtext->num]->mode) DrawTextString("�޴���ң�");
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
		DrawCursor(textbuf, curtext->curPos, curtext->x, curtext->y);/*������ǰ���*/
		DeleteCharFromString(textbuf, curtext->curPos - 1);
		SetEraseMode(FALSE);
		//
		MovePen(curtext->x, curtext->y);
		DrawTextString(textbuf);
		//
		if (curtext->curPos > 0) curtext->curPos--;
		DrawCursor(textbuf, curtext->curPos, curtext->x, curtext->y);/*��ʾ��ǰ���*/
		break;
	default:
		if ((len = strlen(textbuf)) >= TEXTLEN) break;//
		//
		SetEraseMode(TRUE);
		MovePen(curtext->x, curtext->y);
		DrawTextString(textbuf);
		DrawCursor(textbuf, curtext->curPos, curtext->x, curtext->y);/*������ǰ���*/
		InsertCharToString(textbuf, curtext->curPos, c);/*����ǰ�ַ����뵽���λ��*/
		SetEraseMode(FALSE);
		//
		MovePen(curtext->x, curtext->y);
		DrawTextString(textbuf);
		curtext->curPos++;
		DrawCursor(textbuf, curtext->curPos, curtext->x, curtext->y);/*��ʾ��ǰ���*/
		break;
	}
}

/*Insert char c to the string str at the position pos*/
static void InsertCharToString(string str, int pos, char c)
{
	int len;

	if (pos < 0 || pos >= TEXTLEN) return;
	len = strlen(str);
	*(str + len + 1) = '\0';/*������γ��ַ���*/
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

/*�����*/
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

����˵�����ж����λ��
--------------------------------------------------------------------------------------------------------------------------------------------------
*/
static SetButtonT IfOnButton(int x, int y)
{
	double mx, my;//��ǰ�������
	int i, j;
	mx = ScaleXInches(x);/*pixels --> inches*/
	my = ScaleYInches(y);/*pixels --> inches*/
	//�������
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
	//��ͼѡ��
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

����˵�����ж����λ��
--------------------------------------------------------------------------------------------------------------------------------------------------
*/
static TextT IfOnText(int x, int y)
{
	double mx, my;//��ǰ�������
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

����˵����ִ�а�ť��Ϊ
--------------------------------------------------------------------------------------------------------------------------------------------------
*/
static int ButtonAct(int i, int j)
{
	int k, l;
	int pensize = GetPenSize();//���浱ǰ��С
	int pointsize = GetPointSize();
	string color = GetPenColor();//���浱ǰ��ɫ

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
				if (Player[k]->mode)//k����
				{
					l++;
					if (Player[k]->color == "White" || Player[k]->shape == 0)
					{
						MessageBox(NULL, TEXT("�������δ��������������������"), TEXT("�����ӣ������ˣ�"), MB_OK);
						return 0;
					}
					if (Player[k]->name[0] == '\0')
					{
						MessageBox(NULL, TEXT("����ǳ�Ϊ�գ�����������"), TEXT("�����ӣ������ˣ�"), MB_OK);
						return 0;
					}
				}
			if (l <= 1)
			{
				MessageBox(NULL, TEXT("����������٣�����������"), TEXT("�����ӣ������ˣ�"), MB_OK); 
				return 0;
			}

			for (k = 0; k < 3; k++)
				for (l = k + 1; l < 4; l++)
					if (Player[k]->mode&&Player[l]->mode)//k��l������
					{
						if (Player[k]->color == Player[l]->color)
						{
							MessageBox(NULL, TEXT("�����ɫ���ص�������������"), TEXT("�����ӣ������ˣ�"), MB_OK);
							return 0;
						}
						if (StringEqual(Player[k]->name, Player[l]->name))
						{
							MessageBox(NULL, TEXT("����ǳ����ص�������������"), TEXT("�����ӣ������ˣ�"), MB_OK);
							return 0;
						}
					}
			if (!Map) 
			{
				MessageBox(NULL, TEXT("δѡ���ͼ������������"), TEXT("�����ӣ������ˣ�"), MB_OK);
				return 0;
			}
			return 1;
		}
		else
			return 2;

	}

	//��ԭPen
	SetPenSize(pensize);
	SetPenColor(color);
	SetPointSize(pointsize);
	return 0;
}

/*��ʼ����ť*/
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

/*����ť*/
static void DrawB(SetButtonT button)
{
	SetButtonT bptr = button;
	int pensize = GetPenSize();//���浱ǰ��С
	int pointsize = GetPointSize();
	string color = GetPenColor();//���浱ǰ��ɫ
	if ((!bptr->isOn) && (!bptr->isDown))
	{
		SetPenSize(bptr->PenSize);
		//������
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
		//���߿�
		SetPenColor(bptr->BorCol);
		MovePen(bptr->StartX, bptr->StartY);
		DrawLine(bptr->w, 0);
		DrawLine(0, bptr->h);
		DrawLine(-bptr->w, 0);
		DrawLine(0, -bptr->h);
		//���ı�
		SetPointSize(bptr->PointSize);
		SetPenColor(bptr->TexCol);
		MovePen(bptr->StartX + (bptr->w - TextStringWidth(bptr->text)) / 2, bptr->StartY + bptr->h / 3);
		DrawTextString(bptr->text);
	}
	if (bptr->isOn)//���ͣ��
	{
		SetPenSize(bptr->PenSize);
		//������
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
		//���߿�
		SetPenColor(bptr->BorColO);
		MovePen(bptr->StartX, bptr->StartY);
		DrawLine(bptr->w, 0);
		DrawLine(0, bptr->h);
		DrawLine(-bptr->w, 0);
		DrawLine(0, -bptr->h);
		//���ı�
		SetPointSize(bptr->PointSize);
		SetPenColor(bptr->TexColO);
		MovePen(bptr->StartX + (bptr->w - TextStringWidth(bptr->text)) / 2, bptr->StartY + bptr->h / 3);
		DrawTextString(bptr->text);
	}
	if (bptr->isDown)//��갴��
	{
		SetPenSize(bptr->PenSize);
		//������
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
		//���߿�
		SetPenColor(bptr->BorColD);
		MovePen(bptr->StartX, bptr->StartY);
		DrawLine(bptr->w, 0);
		DrawLine(0, bptr->h);
		DrawLine(-bptr->w, 0);
		DrawLine(0, -bptr->h);
		//���ı�
		SetPointSize(bptr->PointSize);
		SetPenColor(bptr->TexColD);
		MovePen(bptr->StartX + (bptr->w - TextStringWidth(bptr->text)) / 2, bptr->StartY + bptr->h / 4);
		DrawTextString(bptr->text);
	}
	//��ԭPen
	SetPenSize(pensize);
	SetPenColor(color);
	SetPointSize(pointsize);
	
}

/*��������ý���*/
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
	DrawTextString("����ǳ�");
	MovePen(6 + i * 48, 66);
	DrawTextString("���ģʽ");
	MovePen(6 + i * 48, 57);
	DrawTextString("������״");
	MovePen(6 + i * 48, 39);
	DrawTextString("������ɫ");

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
	DrawTextString("�������ǳ�,<10�ַ�");

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
		DrawTextString("ѡ���ͼ");
	}
}

/*��ʼ��������ð�ť*/
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

/*��ʼ��ѡ���ͼ��ť*/
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

/*��ʼ��step��ť*/
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
		if (i == 0) StepButton[i]->text = "�����˵�";
		else StepButton[i]->text = "��ʼ��Ϸ";
		StepButton[i]->StartX = 33 + 114 * i;
		StepButton[i]->StartY = 6;
		StepButton[i]->isDown = FALSE;
		StepButton[i]->isOn = FALSE;
		DrawB(StepButton[i]);
	}
}

/*��ʼ���ǳ�*/
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

/*��ʼ���������*/
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