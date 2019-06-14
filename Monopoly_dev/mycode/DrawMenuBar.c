/********************
*   DrawMenuBar.c
*
*   �������ܣ��������˵����Լ������ꡢ���̻ص�
*
*   ����ʱ�䣺2019.5.12  10:30
*   �����ߣ���Ԫ��  Grp 5
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

typedef struct button {//��ť����
	double StartX, StartY, h, w;//��ť���½�Ϊ��ʼX��Y���߶�h�����w
	string BorderColor;//�߿���ɫ
	string StringColor;//������ɫ
	string BorderColorOn;//��ѡ��ʱ�߿���ɫ
	string StringColorOn;//��ѡ��ʱ������ɫ
	string BorderColorDown;
	string StringColorDown;
	string BackColor;//���������ɫ
	string BackColorOn;//��ѡ��ʱ���������ɫ
	string BackColorDown;
	int PenSize;//�߿��ϸ
	int PointSize;//���ִ�С
	char hot;//��ݼ�
	string text;//��ť�ı�
	bool isOn;//���ͣ����־
	bool isDown;//ѡ���־
	bool isAct;//ִ�б�־
}*MenuBarButtonT;

//=========================================================================================================================================================================================================

//���ļ�����

static MenuBarButtonT cueBarButton = NULL;//��ǰ��ťָ��
static MenuBarButtonT preBarButton = NULL;//Ϊʵ����������������رն���ı���

static bool MouseisDown = FALSE;//��갴ť���±�־
static bool KeyisDown = FALSE;//���̰������±�־

static MenuBarButtonT MenuBar0[3] = { NULL };//�˵������̡�����
static MenuBarButtonT MenuBar1[4] = { NULL };//����Ϸ�����롢���桢�˳�
static MenuBarButtonT MenuBar2[2] = { NULL,NULL };//��ͣ������

//=========================================================================================================================================================================================================

//��ʼ����ť
static void InitButton(MenuBarButtonT bptr,
	double w,//��ť��
	double h,//��ť��
	string BorderColor,//�߿���ɫ
	string BorderColorOn,//��ѡ��ʱ�߿���ɫ
	string BorderColorDown,//������ʱ�߿���ɫ
	string StringColor,//������ɫ
	string StringColorOn,//��ѡ��ʱ������ɫ
	string StringColorDown,//������ʱ������ɫ
	string BackColor,//���������ɫ
	string BackColorOn,//��ѡ��ʱ���������ɫ
	string BackColorDown,//������ʱ������ɫ
	int PenSize,//�߿��ϸ
	int PointSize//���ִ�С
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

�������ܣ������˵���
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

�������ܣ���ʼ���˵���������ѡ�
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

�������ܣ���ʼ���˵���
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
static void InitMenuBar0()
{
	for (int i = 0; i < 3; i++)
	{
		MenuBar0[i] = GetBlock(sizeof(*MenuBar0[i]));
		InitButton(MenuBar0[i], TextStringWidth("����Ϸ(N)")*1.2 - ScaleXInches(GetPenSize()) * 2, 5, "Black", "Black", "Black", "Black", "Black", "Black", "gingeryellow", "VC", "lavender", 2, 5);
		MenuBar0[i]->StartX = TextStringWidth("����Ϸ(N)")*1.2*i;
		MenuBar0[i]->StartY = GetWindowHeight() - 5;
	}
	//
	MenuBar0[0]->text = "�˵�(M)";
	MenuBar0[0]->hot = 'm';

	//
	MenuBar0[1]->text = "����(P)";
	MenuBar0[1]->hot = 'p';

	//
	MenuBar0[2]->text = "����(H)";
	MenuBar0[2]->hot = 'h';
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawMenuBar0();

�������ܣ������˵���
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

�������ܣ���ʼ���˵�����ѡ�
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
static void InitMenuBar1()
{
	for (int i = 0; i < 4; i++) {
		MenuBar1[i] = GetBlock(sizeof(*MenuBar1[0]));
		InitButton(MenuBar1[i], TextStringWidth("����Ϸ(N)")*1.2 - ScaleXInches(GetPenSize()) * 2, 5, "Black", "Black", "Black", "Black", "Black", "Black", "gingeryellow", "VC", "lavender", 2, 5);
		MenuBar1[i]->StartX = 0;
		MenuBar1[i]->StartY = GetWindowHeight() - 10 - 5 * i;
	}
	//
	MenuBar1[0]->text = "����Ϸ(N)";
	MenuBar1[0]->hot = 'n';

	//
	MenuBar1[1]->text = "����(L)";
	MenuBar1[1]->hot = 'l';

	//
	MenuBar1[2]->text = "����(S)";
	MenuBar1[2]->hot = 's';

	//
	MenuBar1[3]->text = "�˳�(E)";
	MenuBar1[3]->hot = 'e';
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawMenuBar1();

�������ܣ������˵�����ѡ�
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

�������ܣ���ʼ����������ѡ�
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
static void InitMenuBar2()
{
	for (int i = 0; i < 2; i++) {
		MenuBar2[i] = GetBlock(sizeof(*MenuBar2[i]));
		InitButton(MenuBar2[i], TextStringWidth("����Ϸ(N)")*1.2 - ScaleXInches(GetPenSize()) * 2, 5, "Black", "Black", "Black", "Black", "Black", "Black", "Red", "VC", "lavender", 2, 5);
		MenuBar2[i]->StartX = TextStringWidth("����Ϸ(N)")*1.2;
		MenuBar2[i]->StartY = GetWindowHeight() - 10;
	}
	//
	MenuBar2[0]->text = "��ͣ(W)";
	MenuBar2[0]->hot = 'w';

	//
	MenuBar2[1]->text = "����(C)";
	MenuBar2[1]->hot = 'c';
	MenuBar2[1]->isAct = TRUE;
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawMenuBar2();

�������ܣ�������������ѡ�
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
	double w,//��ť��
	double h,//��ť��
	string BorderColor,//�߿���ɫ
	string BorderColorOn,//��ѡ��ʱ�߿���ɫ
	string BorderColorDown,//������ʱ�߿���ɫ
	string StringColor,//������ɫ
	string StringColorOn,//��ѡ��ʱ������ɫ
	string StringColorDown,//������ʱ������ɫ
	string BackColor,//���������ɫ
	string BackColorOn,//��ѡ��ʱ���������ɫ
	string BackColorDown,//������ʱ������ɫ
	int PenSize,//�߿��ϸ
	int PointSize//���ִ�С
	);

�������ܣ���ʼ����ť���԰�ť��StartX��StartY��text��hot֮��ĳ�Ա��ֵ
����˵��������
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

�������ܣ���һ����ťbutton
����˵����Ҫ���İ�ť��ָ��
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
void DrawButton(MenuBarButtonT button)
{
	MenuBarButtonT bptr = button;
	int pensize = GetPenSize();//���浱ǰ��С
	int pointsize = GetPointSize();
	string color = GetPenColor();//���浱ǰ��ɫ

	if ((!bptr->isOn) && (!bptr->isDown))//���û��ͣ����û�б�ѡ��
	{
		SetPenSize(bptr->PenSize);
		//������
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
		//���߿�
		SetPenColor(bptr->BorderColor);
		MovePen(bptr->StartX, bptr->StartY);
		DrawLine(bptr->w, 0);
		DrawLine(0, bptr->h);
		DrawLine(-bptr->w, 0);
		DrawLine(0, -bptr->h);
		//���ı�
		SetPointSize(bptr->PointSize);
		SetPenColor(bptr->StringColor);
		MovePen(bptr->StartX + (bptr->w - TextStringWidth(bptr->text)) / 2, bptr->StartY + bptr->h / 2);
		DrawTextString(bptr->text);
	}
	if (bptr->isOn)//���ͣ��
	{
		SetPenSize(bptr->PenSize);
		//������
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
		//���߿�
		SetPenColor(bptr->BorderColorOn);
		MovePen(bptr->StartX, bptr->StartY);
		DrawLine(bptr->w, 0);
		DrawLine(0, bptr->h);
		DrawLine(-bptr->w, 0);
		DrawLine(0, -bptr->h);
		//���ı�
		SetPointSize(bptr->PointSize);
		SetPenColor(bptr->StringColorOn);
		MovePen(bptr->StartX + (bptr->w - TextStringWidth(bptr->text)) / 2, bptr->StartY + bptr->h / 2);
		DrawTextString(bptr->text);
	}
	if (bptr->isDown)//��갴��
	{
		SetPenSize(bptr->PenSize);
		//������
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
		//���߿�
		SetPenColor(bptr->BorderColorDown);
		MovePen(bptr->StartX, bptr->StartY);
		DrawLine(bptr->w, 0);
		DrawLine(0, bptr->h);
		DrawLine(-bptr->w, 0);
		DrawLine(0, -bptr->h);
		//���ı�
		SetPointSize(bptr->PointSize);
		SetPenColor(bptr->StringColorDown);
		MovePen(bptr->StartX + (bptr->w - TextStringWidth(bptr->text)) / 2, bptr->StartY + bptr->h / 2);
		DrawTextString(bptr->text);
	}
	//��ԭPen
	SetPenSize(pensize);
	SetPenColor(color);
	SetPointSize(pointsize);
}

/*
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
MenuBarButtonT IfOnButton(int x,int y);

�������ܣ��ж�����Ƿ�ͣ���ڰ�ť��
����˵����
	x:��귵�صĺ�����
	y;��귵�ص�������
	-->������ͬ���ص�����
����ֵ����굱ǰλ�����ڵİ�ťָ�����NULL
---------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
MenuBarButtonT IfOnButton(int x, int y)
{
	double mx, my;//��ǰ�������
	double w = MenuBar0[0]->w;
	double h = MenuBar0[0]->h;
	int i;
	mx = ScaleXInches(x);/*pixels --> inches*/
	my = ScaleYInches(y);/*pixels --> inches*/
	for (i = 0; i < 3; i++) {//���˵���
		if (mx > MenuBar0[i]->StartX&&mx<MenuBar0[i]->StartX + w && my>MenuBar0[i]->StartY) {
			MenuBar0[i]->isOn = TRUE;
			return MenuBar0[i];
		}
	}
	if (MenuBar0[0]->isDown) {//���˵�������
		for (i = 0; i < 4; i++) {
			if (mx<w&&my>MenuBar1[i]->StartY&&my < MenuBar1[i]->StartY + h) {
				MenuBar1[i]->isOn = TRUE;
				return MenuBar1[i];
			}
		}
	}
	if (MenuBar0[1]->isDown) {//������
		if (mx > w&&mx<w * 2 && my>MenuBar2[0]->StartY&&my < MenuBar0[1]->StartY) {
			if (MenuBar2[0]->isAct) {//��ͣ�У���ʾ����
				MenuBar2[1]->isOn = TRUE;
				return MenuBar2[1];
			}
			else if (MenuBar2[1]->isAct) {//�����У���ʾ��ͣ
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

�������ܣ����ص��е��ô˺������ж������˵�����ص���Ϊ
����˵����
	x:��귵�صĺ�����
	y:��귵�ص�������
	button:��갴ť
	event:����¼�
	-->������ͬ���ص�����
����ֵ��
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
						switch (MessageBox(NULL, TEXT("�Ƿ񱣴浱ǰ��Ϸ���ȣ�"), TEXT("��ʼ����Ϸ"), MB_YESNOCANCEL)) {
						case 6://ѡ������
							MenuBar1[0]->isDown = FALSE;
							DrawMenuBar0();
							//ִ�б��沢����
							return 1;
						case 7://ѡ���˷�
							MenuBar1[0]->isDown = FALSE;
							DrawMenuBar0();
							//ִ������
							return 2;
						case 2://ѡ����ȡ����������Ϸ
							//
							break;
						}
						MenuBar1[0]->isDown = FALSE;
						break;
					case 'l':
						switch (MessageBox(NULL, TEXT("�Ƿ񱣴浱ǰ��Ϸ���ȣ�"), TEXT("����浵"), MB_YESNOCANCEL)) {
						case 6://ѡ������
							MenuBar1[1]->isDown = FALSE;
							DrawMenuBar0();
							//ִ�б��沢����
							return 3;
						case 7://ѡ���˷�
							MenuBar1[1]->isDown = FALSE;
							DrawMenuBar0();
							//ִ������
							return 4;
						case 2://ѡ����ȡ����������Ϸ
							//
							break;
						}
						MenuBar1[1]->isDown = FALSE;
						break;
					case 's':
						//ִ�б���
						MenuBar1[2]->isDown = FALSE;
						DrawMenuBar0();
						return 5;
					case 'e':
						switch (MessageBox(NULL, TEXT("�Ƿ񱣴���Ϸ���ȣ�"), TEXT("�˳�"), MB_YESNOCANCEL)) {
						case 6://ѡ������
							MenuBar1[3]->isDown = FALSE;
							DrawMenuBar0();
							//ִ�б��沢�˳�
							return 6;
						case 7://ѡ���˷�ֱ��ִ���˳�
							exit(-1);
						case 2://ѡ����ȡ����������Ϸ
							MenuBar1[3]->isDown = FALSE;
							break;
						}
						break;
					case 'h'://ѡ���������ʾ����
						//��ʾ����
						MenuBar0[2]->isDown = FALSE;
						DrawMenuBar0();
						return 7;
					case 'w'://ѡ����ͣ
						//��ͣ
						cueBarButton->isDown = FALSE;
						cueBarButton = MenuBar2[1];
						DrawMenuBar0();
						return 8;
					case 'c'://ѡ�����
						//����
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

�������ܣ����̻ص��е��ô˺������жϼ�����˵�����ص���Ϊ
����˵����
	key:���̰���
	event:�����¼�
	-->������ͬ���̻ص�����
����ֵ��
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
				switch (MessageBox(NULL, TEXT("�Ƿ񱣴浱ǰ��Ϸ���ȣ�"), TEXT("��ʼ����Ϸ"), MB_YESNOCANCEL)) {
				case 6://ѡ������
					MenuBar1[0]->isDown = FALSE;
					DrawMenuBar0();
					cueBarButton = NULL;
					//ִ�б��沢����
					return 1;
				case 7://ѡ���˷�
					MenuBar1[0]->isDown = FALSE;
					DrawMenuBar0();
					cueBarButton = NULL;
					//ִ������
					return 2;
				case 2://ѡ����ȡ����������Ϸ
					//
					break;
				}
				MenuBar1[0]->isDown = FALSE;
				break;
			case 'L':
				switch (MessageBox(NULL, TEXT("�Ƿ񱣴浱ǰ��Ϸ���ȣ�"), TEXT("����浵"), MB_YESNOCANCEL)) {
				case 6://ѡ������
					MenuBar1[1]->isDown = FALSE;
					DrawMenuBar0();
					cueBarButton = NULL;
					//ִ�б��沢����
					return 3;
				case 7://ѡ���˷�
					MenuBar1[1]->isDown = FALSE;
					DrawMenuBar0();
					cueBarButton = NULL;
					//ִ������
					return 4;
				case 2://ѡ����ȡ����������Ϸ
					//
					break;
				}
				MenuBar1[1]->isDown = FALSE;
				break;
			case 'S':
				MenuBar1[2]->isDown = FALSE;
				DrawMenuBar0();
				cueBarButton = NULL;
				//ִ�б���
				return 5;
			case 'E':
				switch (MessageBox(NULL, TEXT("�Ƿ񱣴���Ϸ���ȣ�"), TEXT("�˳�"), MB_YESNOCANCEL)) {
				case 6://ѡ������
					MenuBar1[3]->isDown = FALSE;
					DrawMenuBar0();
					cueBarButton = NULL;
					//ִ�б��沢�˳�
					return 6;
				case 7://ѡ���˷�ֱ���˳�
					exit(-1);
				case 2://ѡ����ȡ����������Ϸ
					MenuBar1[3]->isDown = FALSE;
					break;
				}
				break;
			case 'H'://ѡ���������ʾ����
				cueBarButton->isDown = FALSE;
				DrawMenuBar0();
				cueBarButton = NULL;
				return 7;
			case 'W'://ѡ����ͣ
				if (MenuBar2[1]->isAct) {
					//��ͣ
					MenuBar2[0]->isAct = TRUE;
					MenuBar2[1]->isAct = FALSE;
					cueBarButton->isDown = FALSE;
					DrawMenuBar0();
					cueBarButton = NULL;
					return 8;
				}
				break;
			case 'C'://ѡ�����
				if (MenuBar2[0]->isAct) {
					//����
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