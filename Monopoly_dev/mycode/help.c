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
	int state;//0-����;1-���ͣ��;2-��갴��;
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
	ButtonHelp[0]->text = "��һҳ";
	ButtonHelp[1]->text = "��һҳ";
	ButtonHelp[2]->text = "�Ҷ���";
}

static void DrawHelpButton(HelpButtonT b)
{
	HelpButtonT bptr = b;
	int Pensize = GetPenSize();//���浱ǰ��С
	int Pointsize = GetPointSize();
	string color = GetPenColor();//���浱ǰ��ɫ
	if (b)
	{
		if (!bptr->state)
		{
			SetPenSize(pensize);
			//������
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
			//���ı�
			SetPointSize(pointsize);
			SetPenColor(TextColor);
			MovePen(bptr->StartX + (w - TextStringWidth(bptr->text)) / 2, bptr->StartY + h / 3);
			DrawTextString(bptr->text);
		}
		if (bptr->state == 1)//���ͣ��
		{
			SetPenSize(pensize);
			//������
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
			//���ı�
			SetPointSize(pointsize);
			SetPenColor(TextColor);
			MovePen(bptr->StartX + (w - TextStringWidth(bptr->text)) / 2, bptr->StartY + h / 3);
			DrawTextString(bptr->text);
		}

		if (bptr->state == 2)//��갴��
		{
			SetPenSize(pensize);
			//������
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
			//���ı�
			SetPointSize(pointsize);
			SetPenColor(TextColor);
			MovePen(bptr->StartX + (w - TextStringWidth(bptr->text)) / 2, bptr->StartY + h / 4);
			DrawTextString(bptr->text);
		}
		//���߿�
		SetPenSize(pensize);
		SetPenColor(BorderColor);
		MovePen(bptr->StartX, bptr->StartY);
		DrawLine(w, 0);
		DrawLine(0, h);
		DrawLine(-w, 0);
		DrawLine(0, -h);

		//��ԭPen
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
	DrawTextString("��Ϸ����");
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
		DrawTextString("����һ����׵Ĵ��̶�����Ϸ����ҿ���ѡ��ͬ��Ϸ��ͼ��");
		MovePen(60, 82);
		DrawTextString("������Ϸ��ɫ�������Ϸ��ÿλ�����20000Ԫ��ʼ�ʲ���һͬ����");
		MovePen(60, 77);
		DrawTextString("�������ͨ��ת��ת�̼����ײ��ԣ���ء��Ƿ���׬ȡ�����Ϸ");
		MovePen(60, 72);
		DrawTextString("���һλû���Ʋ�����ҵ�ʤ��");
		MovePen(60, 67);
		DrawTextString("��Ϸ����:");
		MovePen(60, 42);
		DrawTextString("��ͼ�¼�˵��:");
		MovePen(60, 37);
		DrawTextString("1. ����");
		MovePen(60, 27);
		DrawTextString("2. ���ظ�");

		SetPenColor("rred");
		MovePen(60, 62);
		DrawTextString("ÿ���ֵ���ʱ����Ϸ�������µ�˳�����:");
		MovePen(60, 57);
		DrawTextString("1. ˳���û����ո�-���س�������ת����ת�̡�");
		MovePen(60, 52);
		DrawTextString("2. �������յ���ǰ����");
		MovePen(60, 47);
		DrawTextString("3. ִ�еִ�ͣ���ص����¼���");
		MovePen(60, 32);
		DrawTextString("ÿ���˾���������ʱ������������ȡ��ǧ��ҡ�");
		MovePen(60, 22);
		DrawTextString("a. ͣ�����յء�ʱ�����԰��ձ�ʾ��Ǯ�������������ء�");
		MovePen(60, 17);
		DrawTextString("b. ͣ�����˵�����ʱ����������������ص�ʱ��״��������Է���");
	}
	else if (page == 2)
	{
		SetPenColor("rred");
		MovePen(60, 87);
		DrawTextString("�����Ϲ涨�ġ���·�ѡ���");
		MovePen(60, 82);
		DrawTextString("c.ͣ���Լ�������ʱ����ѡ���Ƿ���ɵز�������涨�Ľ���Լ�");
		MovePen(60, 77);
		DrawTextString("�ǡ����ݡ���һ������������Ķ����ݡ�");
		MovePen(60, 67);
		DrawTextString("ͣ�����˸����ҽ������ȡһ�����˿������տ�Ƭ������ִ�С�");
		MovePen(60, 57);
		DrawTextString("ͣ������˰��ʱ���ɽ�10%�ֽ�����С�");
		MovePen(60, 47);
		DrawTextString("ͣ��ʱ������Ϸ���ϱ�ʾ�ļ�Ǯ���򡣹����վ�����Ϊ��ӵ��");
		MovePen(60, 42);
		DrawTextString("�ߣ����ͣ������ѡ���Ƿ񻨷�500Ԫȥ���ڳ�վ��1000Ԫȥ���");
		MovePen(60, 37);
		DrawTextString("��վ�����ѹ��������ˡ�");
		MovePen(60, 27);
		DrawTextString("������Ϸ���ϵļ�Ǯ�������ͣ��������500Ԫ��/ˮ�Ѹ�ӵ���ߣ�");
		MovePen(60, 22);
		DrawTextString("��ӵ����ͬʱ���������ز����������ͣ����֧��1000Ԫ��ӵ���ߡ�");

		SetPenColor("Black");
		MovePen(60, 72);
		DrawTextString("3. ���˸�");
		MovePen(60, 62);
		DrawTextString("4. ����˰");
		MovePen(60, 52);
		DrawTextString("5. ��վ");
		MovePen(60, 32);
		DrawTextString("6. ������˾������ˮ��");
		MovePen(60, 17);
		DrawTextString("7. ҽԺ��");
	}
	else if (page == 3)
	{
		SetPenColor("rred");
		MovePen(60, 87);
		DrawTextString("���סԺ��������·����ͣ�����·���");
		MovePen(60, 77);
		DrawTextString("��ҷ��̴�������·����ͣ�����·���");
		MovePen(60, 67);
		DrawTextString("���·���");
		MovePen(60, 52);
		DrawTextString("ϵͳ�Զ��԰���չ���������з��ݣ����޷�������ϵͳ�Զ���");
		MovePen(60, 47);
		DrawTextString("����ҳ��е����������԰����������ֱ���޵ؿ���Ϊֹ��");
		MovePen(60, 37);
		DrawTextString("����ұ��������ʲ��಻����֧��Ƿ��ʱ��������Ʋ����˳���Ϸ��");

		SetPenColor("Black");
		MovePen(60, 82);
		DrawTextString("8. ������");
		MovePen(60, 72);
		DrawTextString("9. ͣ����");
		MovePen(60, 62);
		DrawTextString("����");
		MovePen(60, 57);
		DrawTextString("1. �ֽ���");
		MovePen(60, 42);
		DrawTextString("2. �Ʋ�");
	}
}

static HelpButtonT IfOnHelpButton(int x, int y)
{
	double mx, my;//��ǰ�������
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