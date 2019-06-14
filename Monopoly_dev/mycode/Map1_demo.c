/***********************************************************
	map1_demo.c 
	
	���ص� 	void Mouse_Map1(int x, int y, int button, int event);
	���̻ص� 	void Keyboard_Turntable(int key,int event); 

	����˵��: ����ͼ����ʼ����ͼ 
			�����ͼʱ��ʾ��ǰλ����Ϣ 
	��ͼ�ṹ��
		cell-> 
			string name;                 ���� 
			int cell_kind;               �ж϶����� ��	|***************************
			int owner;//ӵ���� ,ֵΪ-1ʱ�������� 		|	0����� 
			int construt_fee;//������ 					|	1������ 
			int house;//���Ӹ��� ������Ϊ4 				|	2Ϊ����  
			int tolls[5];//���ݵ�ǰ���Ӹ�����ȡ��·�� 	|	3Ϊ����˰  
			int mortgage_fee;//��Ѻ�� 					|	4Ϊ��վ 
														|	5Ϊ������˾������ˮ��  
														|	6ΪҽԺ  
														|	7Ϊ����
														|	8Ϊͣ���� 
														|**************************

	����ʱ�� :2019/5/17  22:24
	�޸ģ�2019/5/25 19:52
		  2019/5/29 23:36
		
*************************************************************/

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
#include<time.h>

#include "spiral.h" 
#include "Map1_demo.h"
#include "turntable.h"
#include "DrawMenuBar.h"
#include "setting.h"

#define start_map1x 70
#define start_map1y 20
#define cell_lenth 15
#define cell_width 10

//=========================================================================================================================================================================================================

extern struct PlayPeople * Player[4];
extern int act;
extern int resultX;
extern degree;

//=========================================================================================================================================================================================================

static int nowcell = -1;
Cell_AllT cell[38];
static int forward_step = 1;
int turn_player = 0;//��ǰ�ֵ��ڼ������ 
static bool purchase = FALSE;
bool game_over = FALSE;//��Ϸ������־ 
static int  last_destiny = 0; 
static int buy = -1;//�����־
static int Event = 0;// : 1 - �������أ�2 - �����ݣ�3 - ���������أ�4 - �����˷��ݣ�5 - ������6 - ���˳���7 - ��Ǯ��8 - �Ʋ���9-��������10-��Ǯ��11-������12-������13-סԺ��14-��Ժ��15-����Ǯ��16-�����ӣ�17-����

//=========================================================================================================================================================================================================

static void DrawCellMenu();//����ͼ��Ϣ�˵� 
static void DrawPlayerMenu();//�������Ϣ�˵� 
static void DrawPlayer();//����� 
static void ispurchase(int isdown);//����˵� 
static void DrawCell(int,int);//��ÿһ������ 
static void InitCell(Cell_AllT celli,string name,	int cell_kind,int owner,int construt_fee, int house,
			 int tolls0,int tolls1, int tolls2, int tolls3, int tolls4, int mortgage_fee);//������ʼ�� 
static void sellland(int playerid,int difference);//����
static void sellhouse(int playerid); //������ 
static void ErectText(string text);//����д�ı� ��Ȼ�󻭱ʻص���ʼλ�� 
static void numtostring(int num,char* numstring);//����ת�����ַ��� 
static void destiny(int playerid);//���� 

//=========================================================================================================================================================================================================

/*����ͼ1*/
void MainMap1(){
	int i = 0;
	InitCelldata();/*��ʼ����ͼ����*/ 
	while (Player[i]->mode == 0) i++;
	turn_player = i;
	DrawMap1(1);/*����ͼ*/ 
}

/*��Map1*/
void DrawMap1(int fuck) {
	InitGraphics();

	int i;
	/*����ͼ����*/
	MovePen(start_map1x, start_map1y);
	SetPenSize(2);
	SetPenColor("pink");/*������ɫ*/
	DrawCell(5, 0);
	MovePen(start_map1x + cell_width * 10 + cell_lenth, start_map1y);
	DrawCell(5, 11);
	MovePen(start_map1x + cell_width * 10 + cell_lenth, start_map1y + cell_width * 7 + cell_lenth);
	DrawCell(5, 19);
	MovePen(start_map1x, start_map1y + cell_width * 7 + cell_lenth);
	DrawCell(5, 30);
	/*����ͼ�ı� */
	MovePen(start_map1x, start_map1y);
	SetPenColor("grass");/*�ı���ɫ*/
	for (i = 1; i < 11; i++) {
		DrawCell(1, i);
	}/*��*/
	for (i = 12; i < 19; i++) {
		DrawCell(2, i);
	}/*�ұ�*/
	for (i = 20; i < 30; i++) {
		DrawCell(3, i);
	}/*����*/
	for (i = 31; i < 38; i++) {
		DrawCell(4, i);
	}/*����*/

	MovePen(74, 87);
	SetPenColor("smoke");
	StartFilledRegion(0.6);
	DrawLine(55, 0);
	DrawLine(0, -64);
	DrawLine(-55, 0);
	DrawLine(0, 64);
	EndFilledRegion();

	SetPenColor("orange");
	MovePen(85, 78);
	DrawTextString("���¿ո������ʼת��ת��");
	MovePen(85, 72);
	DrawTextString("���»س�����ֹͣת��ת��");
	SetPointSize(1);

	MovePen(123, 54);
	SetPenColor("deepblue");
	ErectText("ǰ��������");

	SetPenSize(1);
	MenuBar();
	DrawTurnTable(120, 45, 18.0);/*��ת��*/
	DrawCellMenu();/*����ͼ��Ϣ�˵�*/
	DrawPlayer();//�������λ�� 
	DrawPlayerMenu();//���������Ϣ�˵� 
	/*��ʼ����ͼ����Ϊ 1�� ˢ��ʱ����Ϊ2 */
	if (fuck == 1) {
		destiny(-1);//���� 
	}
	else if (fuck == 2) {
		destiny(-2);
	}
}

/*������Ϣ*/
int KeyBoard_Map1(int key, int event)
{
	if (game_over){
		switch (event) {
	 		case KEY_DOWN:
	 			cancelTimer(0);
	 			InitGraphics();/*��ʼ��ͼ�ν��� */
	 			start_menu();
				degree = 0;
				break;
		}
	}
	if (!Event)
		Keyboard_Turntable(key, event);
	return KeyBoard_about_MenuBar(key, event);
}

/*��ʱ����Ϣ*/
void Timer_Map1()
{
	Timer_Turntable();
}

/*�����Ϣ*/
int Mouse_Map1(int x, int y, int button, int event) {

	bool isdown = FALSE;

	double mouse_x, mouse_y;
	mouse_x = ScaleXInches(x);/*pixels --> inches*/
	mouse_y = ScaleYInches(y);/*pixels --> inches*/
	switch (event) {
	case BUTTON_DOWN:
		if (button == LEFT_BUTTON) {
			isdown = TRUE;
		}
		break;
	case BUTTON_UP:
		if (button == LEFT_BUTTON) {
			isdown = FALSE;
		}
		break;
		/*�ж�����Ƿ��ڵ�ͼ��*/
	case MOUSEMOVE:
		if ((mouse_x < 185 && mouse_x>55) && (mouse_y < 105 && mouse_y>5)) {
			/*ת������ͼ����*/
			int mx = mouse_x - 55;
			int my = mouse_y - 5;
			/*����ڵ���*/
			if (my < cell_lenth) {
				if (mx < cell_lenth) {
					nowcell = 0;
				}
				else if (mx > 115) {
					nowcell = 11;
				}
				else {
					nowcell = (mx - cell_lenth) / cell_width + 1;
				}
			}
			/*����ڶ���*/
			else if (my > 85) {
				if (mx < cell_lenth) {
					nowcell = 30;
				}
				else if (mx > 115) {
					nowcell = 19;
				}
				else {
					nowcell = 29 - (mx - cell_lenth) / cell_width;
				}
			}
			/*���������*/
			else if (mx < cell_lenth) {
				nowcell = 37 - (my - cell_lenth) / cell_width;
			}
			/*���������*/
			else if (mx > 115) {
				nowcell = 12 + (my - cell_lenth) / cell_width;
			}
			else {
				nowcell = -1;
			}
		}
		break;
	}

	if (isdown) {
		if (nowcell > -1) {
			DrawCellMenu();
		}
		switch (Event)
		{
		case 1:
		case 2:
		case 5:
			/*�ж��Ƿ���*/
			if ((mouse_x < 22 && mouse_x>10) && (mouse_y < 64 && mouse_y>57)) {
				buy = 1;
				ispurchase(1);
			}//��
			else if ((mouse_x < 42 && mouse_x>30) && (mouse_y < 64 && mouse_y>57)) {
				buy = 0;
				ispurchase(2);
			}//�� 
			else {
				buy = -1;
			}//���ڷ�Χ�� 
		}
	}
	else {
		switch (Event)
		{
		case 1://���
			if (buy == 1)
			{
				Player[turn_player]->money -= cell[Player[turn_player]->place]->construt_fee;
				cell[Player[turn_player]->place]->owner = turn_player;
				DrawMap1(2);
				Event = 3;
				ispurchase(0);
				turn_player++;
				if (turn_player > 3) {
					turn_player %= 4;
				}//��һ��ѭ�� 
				while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
					turn_player++;
					if (turn_player > 3) {
						turn_player %= 4;
					}//��һ��ѭ�� 
				}
				DrawPlayerMenu();
			}
			else if (buy == 0)
			{
				DrawMap1(2);
				Event = 9;
				ispurchase(0);
				turn_player++;
				if (turn_player > 3) {
					turn_player %= 4;
				}//��һ��ѭ�� 
				while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
					turn_player++;
					if (turn_player > 3) {
						turn_player %= 4;
					}//��һ��ѭ�� 
				}
				DrawPlayerMenu();
			}
			buy = -1;
			break;
		case 2://��
			if (buy == 1)
			{
				cell[Player[turn_player]->place]->house++;
				Player[turn_player]->money -= cell[Player[turn_player]->place]->construt_fee;
				DrawMap1(2);
				Event = 4;
				ispurchase(0);
				turn_player++;
				if (turn_player > 3) {
					turn_player %= 4;
				}//��һ��ѭ�� 
				while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
					turn_player++;
					if (turn_player > 3) {
						turn_player %= 4;
					}//��һ��ѭ�� 
				}
				DrawPlayerMenu();
			}
			else if (buy == 0)
			{
				DrawMap1(2);
				Event = 9;
				ispurchase(0);
				turn_player++;
				if (turn_player > 3) {
					turn_player %= 4;
				}//��һ��ѭ�� 
				while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
					turn_player++;
					if (turn_player > 3) {
						turn_player %= 4;
					}//��һ��ѭ�� 
				}
				DrawPlayerMenu();
			}
			buy = -1;
			break;
		case 5://��Ʊ
			if (buy == 1)
			{
				Player[turn_player]->money -= 500;
				Player[cell[Player[turn_player]->place]->owner]->money += 500;
				DrawMap1(2);
				Event = 6;
				ispurchase(0);
				if (Player[turn_player]->place == 5 || Player[turn_player]->place == 15) {
					Player[turn_player]->place += 10;
				}
				else if (Player[turn_player]->place == 25) {
					Player[turn_player]->place = 33;
				}
				else if (Player[turn_player]->place == 33) {
					Player[turn_player]->place = 5;
				}
				turn_player++;
				if (turn_player > 3) {
					turn_player %= 4;
				}//��һ��ѭ�� 
				while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
					turn_player++;
					if (turn_player > 3) {
						turn_player %= 4;
					}//��һ��ѭ�� 
				}
				DrawPlayerMenu();
			}
			else if (buy == 0)
			{
				DrawMap1(2);
				Event = 9;
				ispurchase(0);
				turn_player++;
				if (turn_player > 3) {
					turn_player %= 4;
				}//��һ��ѭ�� 
				while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
					turn_player++;
					if (turn_player > 3) {
						turn_player %= 4;
					}//��һ��ѭ�� 
				}
				DrawPlayerMenu();
			}
			buy = -1;
			break;
		}
	}
	return Mouse_about_MenuBar(x, y, button, event);
}

/***************************************************************
	��ʼ����ͼ����
	cell_kind����*�ж����� �� 
	0����� 
	1������
	2������  
	3������˰ 
	4����վ  
	5��������˾������ˮ��  
	6��ҽԺ  
	7������ 
	8��ͣ����
**************************************************************/ 

void InitCelldata(){
	int i;

	for(i = 0;i<38;i++){
		cell[i] = (Cell_AllT)malloc(sizeof(struct Cell_All));
	}
	/*��һ�У��������������ͣ�ӵ���ߣ�������ã�������Ŀ 
	�ڶ��У���·��(�ӿյص���������) ����Ѻ��*/ 
//���� 
	InitCell(cell[0],"��  ��",0,-1,0,0,0,0,0,0,0,0);/*����*/
	InitCell(cell[1],"  ��  ��",1,-1,2000,0,
		350,1750,5000,11000,13000,2000);
	InitCell(cell[2],"  ��  ��",2,-1,0,0,0,0,0,0,0,1);
	InitCell(cell[3],"  ���ô�",1,-1,2000,0,
		350,1750,5000,11000,13000,2000);
	InitCell(cell[4],"  �Ʋ�˰",3,-1,0,0,0,0,0,0,0,2);
	InitCell(cell[5],"  ��վ",4,-1,2000,
		0,0,0,0,0,0,2);
	InitCell(cell[6],"  ����͢",1,-1,500,0,
		60,300,900,2700,4000,500);
	InitCell(cell[7],"  ��  ��",2,-1,0,0,0,0,0,0,0,1);
	InitCell(cell[8],"  ī����",1,-1,500,0,
		60,300,900,2700,4000,500);
	InitCell(cell[9],"  ��  ��",1,-1,2000,0,
		260,1300,3900,9000,11000,1500);
	InitCell(cell[10],"  ��  ��",1,-1,2000,0,
		260,1300,3900,9000,11000,2000);
//���� 
	InitCell(cell[11],"��  ��",7,-1,0,0,0,0,0,0,0,2);/*����*/ 
	InitCell(cell[12],"������˾",5,-1,1500,0,
		0,0,0,0,0,3);
	InitCell(cell[13],"��  ��",1,-1,1500,0,
		200,1000,3000,7500,9250,1500);
	InitCell(cell[14],"�����",1,-1,1000,0,
		100,500,1500,4500,6250,700);
	InitCell(cell[15],"��վ",4,-1,2000,
		0,0,0,0,0,0,2);		
	InitCell(cell[16],"������",1,-1,1000,0,
		100,500,1500,4500,6250,700);
	InitCell(cell[17],"��  ��",2,-1,0,0,0,0,0,0,0,0);
	InitCell(cell[18],"ϣ  ��",1,-1,1000,0,
		120,600,1800,5000,7000,800);
//���� 
	InitCell(cell[19],"���ͣ����",8,-1,0,0,0,0,0,0,0,2);/*����*/ 
	InitCell(cell[20],"  ��  ��",1,-1,1500,0,
		200,1000,3000,7500,9250,1200);
	InitCell(cell[21],"  Ӣ  ��",1,-1,1500,0,
		180,900,2500,7000,8750,1100);
	InitCell(cell[22],"  ��  ��",2,-1,0,0,0,0,0,0,0,1);	
	InitCell(cell[23],"  ����˹",1,-1,500,0,
		60,300,900,2700,4000,500);
	InitCell(cell[24],"  ̩  ��",1,-1,1000,0,
		140,700,2000,5500,7500,900);
	InitCell(cell[25],"  ��վ",4,-1,2000,
		0,0,0,0,0,0,2);
	InitCell(cell[26],"  ������",1,-1,2000,0,
		350,1750,5000,11000,13000,1750);
	InitCell(cell[27]," �Ĵ�����",1,-1,2000,0,
		280,1500,4500,10000,12000,2000);
	InitCell(cell[28]," ����ˮ��",5,-1,1500,0,
		0,0,0,0,0,3);
	InitCell(cell[29],"  �¼���",1,-1,2000,0,
		260,1300,3900,9000,11000,1500);
//���� 
	InitCell(cell[30],"ҽ  Ժ",6,-1,0,0,0,0,0,0,0,2);/*����*/ 
	InitCell(cell[31],"��  ��",1,-1,500,0,
		60,300,900,2700,4000,500);
	InitCell(cell[32],"��  ��",1,-1,2000,0,
		500,2000,6000,14000,17000,2000);
	InitCell(cell[33],"��վ",4,-1,2000,
		0,0,0,0,0,0,2);
	InitCell(cell[34],"��  ��",2,-1,0,0,0,0,0,0,0,0);
	InitCell(cell[35],"��  ��",1,-1,500,0,
		60,300,900,2700,4000,500);
	InitCell(cell[36],"�Ʋ�˰",3,-1,0,0,0,0,0,0,0,2);
	InitCell(cell[37],"��  ��",1,-1,1000,0,
		140,700,2000,5500,7500,900);
}

/*��ʼ����ͼ*/
static void InitCell(Cell_AllT celli,string name,int cell_kind,int owner,int construt_fee, int house,
			 int tolls0,int tolls1, int tolls2, int tolls3, int tolls4, int mortgage_fee){
	celli->name = name;
	celli->cell_kind = cell_kind;
	celli->owner = owner;
	celli->construt_fee = construt_fee;
	celli->house = house;
	celli->tolls[0] = tolls0;
	celli->tolls[1] = tolls1;
	celli->tolls[2] = tolls2;
	celli->tolls[3] = tolls3;
	celli->tolls[4] = tolls4;
	celli->mortgage_fee = mortgage_fee;
}

/*������*/
static void DrawCell(int choice,int number){
	
	double prex,prey;
	prex = GetCurrentX(); 
	prey = GetCurrentY();
	char house[15];
	SetPointSize(1);//�����С 

	switch(choice){
		case 1:
		/*������ĸ���*/
			if (cell[number]->owner == -1)
				SetPenColor("khaki");//������ɫ
			else SetPenColor(Player[cell[number]->owner]->color);
			StartFilledRegion(0.6);
			DrawLine(0,-cell_lenth*0.25);
			DrawLine(cell_width,0);
			DrawLine(0,cell_lenth*0.25);
			DrawLine(-cell_width,0);
			EndFilledRegion();//��Ϳ��ɫ 
			DrawLine(0,-2.5);
			SetPenColor("grey");//������ɫ 
			if (cell[number]->owner != -1 && cell[number]->cell_kind == 1) {
				sprintf(house, "%s*%d", cell[number]->name, cell[number]->house);
				DrawTextString(house);
				SetPenColor(Player[cell[number]->owner]->color);
			}
			else {
				DrawTextString(cell[number]->name);
				SetPenColor("khaki");//������ɫ
			}
			MovePen(prex,prey);
			DrawLine(0,-cell_lenth);
			DrawLine(cell_width,0);
			DrawLine(0,cell_lenth);
			break;
		case 2:
		/*���Ҳ�ĸ���*/ 
			if (cell[number]->owner == -1)
				SetPenColor("khaki");//������ɫ
			else SetPenColor(Player[cell[number]->owner]->color);
			StartFilledRegion(0.6); 
			DrawLine(0,cell_width);
			DrawLine(cell_lenth*0.25,0);
			DrawLine(0,-cell_width);
			DrawLine(-cell_lenth*0.25,0);
			EndFilledRegion();//��Ϳ��ɫ 
			DrawLine(cell_lenth,0);
			DrawLine(0,cell_width);
			DrawLine(-cell_lenth,0);
			SetPenColor("grey");//������ɫ 
			if (cell[number]->owner != -1 && cell[number]->cell_kind == 1)
			{
				sprintf(house, "%s  *%d", cell[number]->name, cell[number]->house);
				ErectText(house);
			}
			else
			{
				ErectText(cell[number]->name);
			}
			break;
		case 3:
		/*������ĸ���*/ 
			if (cell[number]->owner == -1)
				SetPenColor("khaki");//������ɫ
			else SetPenColor(Player[cell[number]->owner]->color);
			StartFilledRegion(0.6);
			DrawLine(-cell_width,0);
			DrawLine(0,cell_lenth*0.25);
			DrawLine(cell_width,0);
			DrawLine(0,-cell_lenth*0.25);
			EndFilledRegion();//��Ϳ��ɫ 
			MovePen(prex-cell_width,prey+1);
			SetPenColor("grey");//������ɫ 
			if (cell[number]->owner != -1 && cell[number]->cell_kind == 1)
			{
				sprintf(house, "%s*%d", cell[number]->name, cell[number]->house);
				DrawTextString(house);
				SetPenColor(Player[cell[number]->owner]->color);
			}
			else {
				DrawTextString(cell[number]->name);
				SetPenColor("khaki");//������ɫ
			}
			MovePen(prex,prey);
			DrawLine(0,cell_lenth);
			DrawLine(-cell_width,0);
			DrawLine(0,-cell_lenth);
			break;
		case 4:
		/*�����ĸ���*/ 
			if (cell[number]->owner == -1)
				SetPenColor("khaki");//������ɫ
			else SetPenColor(Player[cell[number]->owner]->color);
			StartFilledRegion(0.6);
			DrawLine(0,-cell_width);
			DrawLine(-cell_lenth*0.25,0);
			DrawLine(0,cell_width);
			DrawLine(cell_lenth*0.25,0);
			EndFilledRegion();//��Ϳ��ɫ 
			MovePen(prex-4,prey);	
			SetPenColor("grey");//������ɫ 
			if (cell[number]->owner != -1 && cell[number]->cell_kind == 1)
			{
				sprintf(house, "%s  *%d", cell[number]->name, cell[number]->house);
				ErectText(house);
				SetPenColor(Player[cell[number]->owner]->color);
			}
			else
			{
				ErectText(cell[number]->name);
				SetPenColor("khaki");//������ɫ
			}
			MovePen(prex,prey);	
			DrawLine(-cell_lenth,0);
			DrawLine(0,-cell_width);
			DrawLine(cell_lenth,0);
			break;
		case 5:
		/*������*/ 
			SetPenColor("pink");//������ɫ 
			StartFilledRegion(0.6);
			DrawLine(-cell_lenth,0);
			DrawLine(0,-cell_lenth);
			DrawLine(cell_lenth,0);
			DrawLine(0,cell_lenth);
			EndFilledRegion();//��Ϳ��ɫ
			MovePen(prex-cell_lenth+4,prey-cell_lenth*0.5);	
			SetPenColor("grey");//������ɫ 
			DrawTextString(cell[number]->name);
			MovePen(prex,prey);	
	}
}


/*����ͼ��Ϣ�˵� */
static void DrawCellMenu(){
	
	double prex,prey;
	char numstring[10];
	SetPenSize(3);
	MovePen(start_map1x+67,start_map1y+40);
	
	SetPenColor("smoke"); 
	StartFilledRegion(1);
	DrawLine(cell_lenth*2,0);
	DrawLine(0,-cell_lenth*2.5);
	DrawLine(-cell_lenth*2,0);
	DrawLine(0,cell_lenth*2.5);
	EndFilledRegion();
	
	prex = GetCurrentX(); 
	prey = GetCurrentY();

	SetPenColor("black"); 
	SetPointSize(1);
	MovePen(prex+2,prey-3);
	DrawTextString("               ��ͼ��Ϣ");
	if(nowcell>-1){
		/*���*/ 
		if(cell[nowcell]->cell_kind==0){
		 	SetPenColor("rred"); 
			MovePen(prex+2,prey-6);
			DrawTextString("                       ���");
			MovePen(prex+2,prey-12);
			DrawTextString("ÿ���˾������ʱ������");
			MovePen(prex+2,prey-15);
			DrawTextString("������ȡ2000Ԫ.ͣ�����");
			MovePen(prex + 2, prey - 18);
			DrawTextString("ʱ���޷���ȡ�����ɻ��");
			MovePen(prex + 2, prey - 21);
			DrawTextString("����һ�غϡ�");
		} 
		/*����*/ 
		else if(cell[nowcell]->cell_kind==1){
			SetPenColor("deepblue"); 
			MovePen(prex+2,prey-6);
			DrawTextString("��ǰλ�ã�");	
			DrawTextString(cell[nowcell]->name);	
			MovePen(prex+2,prey-9);
			DrawTextString("ӵ���ߣ�");
			if(cell[nowcell]->owner!=-1){
				DrawTextString(Player[cell[nowcell]->owner]->name);
			}//������� 
			else{
				DrawTextString("���˹���");
			}
			MovePen(prex+2,prey-12);
			DrawTextString("���Ӹ�����");
			numtostring(cell[nowcell]->house,numstring);
			DrawTextString(numstring);					
			MovePen(prex+2,prey-15);
			DrawTextString("��·��-�յأ�");
			numtostring(cell[nowcell]->tolls[0],numstring);
			DrawTextString(numstring);
			MovePen(prex+2,prey-18);
			DrawTextString("��·��-һ�����ݣ�");
			numtostring(cell[nowcell]->tolls[1],numstring);
			DrawTextString(numstring);
			MovePen(prex+2,prey-21);
			DrawTextString("��·��-�������ݣ�");
			numtostring(cell[nowcell]->tolls[2],numstring);
			DrawTextString(numstring);
			MovePen(prex+2,prey-24);
			DrawTextString("��·��-�������ݣ�");
			numtostring(cell[nowcell]->tolls[3],numstring);
			DrawTextString(numstring);
			MovePen(prex+2,prey-27);
			DrawTextString("��·��-�Ĵ����ݣ�");
			numtostring(cell[nowcell]->tolls[4],numstring);
			DrawTextString(numstring);
			MovePen(prex+2,prey-30);
			DrawTextString("���ݽ����ѣ�");
			numtostring(cell[nowcell]->construt_fee,numstring);
			DrawTextString(numstring);
			MovePen(prex+2,prey-33);
			DrawTextString("�յص�Ѻ�ѣ�");
			numtostring(cell[nowcell]->mortgage_fee,numstring);
			DrawTextString(numstring);
		}
		/*����*/ 
		else if(cell[nowcell]->cell_kind==2){
			SetPenColor("deepblue"); 
			MovePen(prex+2,prey-6);
			DrawTextString("���᣺");
			SetPenColor("rred"); 
			MovePen(prex+2,prey-9);
			DrawTextString("�ߵ����˸����ҽ���");
			MovePen(prex+2,prey-12);
			DrawTextString("����ȡһ�����˿�����");
			MovePen(prex + 2, prey - 15);
			DrawTextString("�����������ִ��");
		}
		/*����˰*/ 
		else if(cell[nowcell]->cell_kind==3){
			SetPenColor("deepblue"); 
			MovePen(prex+2,prey-6);
			DrawTextString("����˰��");
			SetPenColor("rred");
			MovePen(prex+2,prey-9);
			DrawTextString("�ɽ�10%�ֽ������");
		}
		/*��վ*/ 
		else if(cell[nowcell]->cell_kind==4){
			SetPenColor("deepblue"); 
			MovePen(prex+2,prey-6);
			DrawTextString(cell[nowcell]->name);
			MovePen(prex+2,prey-9);
			DrawTextString("���÷��ã� ");
			numtostring(cell[nowcell]->construt_fee,numstring);
			DrawTextString(numstring);
			MovePen(prex+2,prey-12);
			DrawTextString("ӵ���ߣ� ");
			if(cell[nowcell]->owner!=-1){
				DrawTextString(Player[cell[nowcell]->owner]->name);
			}//������� 
			else{
				DrawTextString("���˹���");
			}
			SetPenColor("rred"); 
			MovePen(prex+2,prey-16);
			DrawTextString("���·��������ѡ���Ƿ�");
			MovePen(prex+2,prey-19);
			DrawTextString("����500Ԫȥ���ڳ�վ");
		}
		/*����ˮ��˾�������˾*/ 
		else if(cell[nowcell]->cell_kind==5){
			SetPenColor("deepblue"); 
			MovePen(prex+2,prey-6);
			DrawTextString(cell[nowcell]->name);
			MovePen(prex+2,prey-9);
			DrawTextString("���÷��ã� ");
			numtostring(cell[nowcell]->construt_fee,numstring);
			DrawTextString(numstring);
			MovePen(prex+2,prey-12);
			DrawTextString("ӵ���ߣ� ");
			if(cell[nowcell]->owner!=-1){
				DrawTextString(Player[cell[nowcell]->owner]->name);
			}//������� 
			else{
				DrawTextString("���˹���");
			}
			
			SetPenColor("rred");
			MovePen(prex+2,prey-16);
			DrawTextString("�������·��������500Ԫ");
			MovePen(prex+2,prey-19);
			DrawTextString("���(��ӵ�е�����˾)��");
			MovePen(prex+2,prey-22);
			DrawTextString("500Ԫˮ��(��ӵ������ˮ");
			MovePen(prex + 2, prey - 25);
			DrawTextString("��˾)��ӵ����");
		}
		/*ҽԺ*/ 
		else if(cell[nowcell]->cell_kind==6){
			SetPenColor("deepblue"); 
			MovePen(prex+2,prey-6);
			DrawTextString(cell[nowcell]->name);
			MovePen(prex+2,prey-9);
			SetPenColor("rred"); 
			DrawTextString("���ǳ鵽���˿�Ƭ����ҽԺ��");
			MovePen(prex+2,prey-12);
			DrawTextString("���򾭹���ͣ����ҽԺ�񲻻�");
			MovePen(prex+2,prey-15);
			DrawTextString("�����κ�Ӱ��");		
		}
		/*����*/ 
		else if(cell[nowcell]->cell_kind==7){
			SetPenColor("deepblue"); 
			MovePen(prex+2,prey-6);
			DrawTextString(cell[nowcell]->name);
			MovePen(prex+2,prey-9);
			SetPenColor("rred"); 
			DrawTextString("���ǳ鵽���˿�Ƭ���������");
			MovePen(prex+2,prey-12);
			DrawTextString("���򾭹���ͣ����ҽԺ�񲻻�");
			MovePen(prex+2,prey-15);
			DrawTextString("�����κ�Ӱ��");		
		}
		/*���ͣ����*/ 
		else if(cell[nowcell]->cell_kind==8){
			SetPenColor("deepblue"); 
			MovePen(prex+2,prey-6);
			DrawTextString(cell[nowcell]->name);
			MovePen(prex+2,prey-9);
			SetPenColor("rred"); 
			DrawTextString("�ߵ�ͣ���������·���");		
		}
	}
}

/*�������Ϣ�˵�*/ 
static void DrawPlayerMenu(){
	int i;
	MovePen(start_map1x-65,start_map1y+30);	
	double prex,prey;
	prex = GetCurrentX(); 
	prey = GetCurrentY();
	char numstring[10];
	
	SetPenSize(3);

	SetPenColor("smoke"); 
	StartFilledRegion(1);
	DrawLine(cell_lenth*2.5,0);
	DrawLine(0,-cell_lenth*3);
	DrawLine(-cell_lenth*2.5,0);
	DrawLine(0,cell_lenth*3);
	EndFilledRegion();
	
	SetPenSize(20);
		MovePen(prex+28,prey-6-9*turn_player);
		SetPenColor("pink"); 	
		DrawLine(2,2);
		DrawLine(-2,-2);
		DrawLine(2,-2);
		DrawLine(-2,2);
		DrawLine(7,0);

	
	SetPenColor("black"); 
	SetPointSize(10);
	MovePen(prex+2,prey-3);
	DrawTextString("               �����Ϣ");
	SetPenColor("deepblue");
	
	SetPointSize(10);
	for(i=0;i<4;i++)
	{
		if(Player[i]->mode!=0){
			MovePen(prex+2,prey-6-9*i);
			DrawTextString("����ǳƣ�");
			DrawTextString(Player[i]->name);
			if (Player[i]->mode == 2)
				DrawTextString("  -AI");
			MovePen(prex+2,prey-9-9*i);
			if(Player[i]->isDefeated==TRUE){
	
				DrawTextString("����ѱ���̭");
			}
			else{
				numtostring(Player[i]->money,numstring);
				DrawTextString("��ҽ�Ǯ��");
				DrawTextString(numstring);	
			}
		}
	}
} 

/*��Ϸ����*/
void begingame(){
	int all_player = 0;
	int i;
	for(i = 0;i<4;i++){
		if((Player[i]->mode!=0)&&(Player[i]->isDefeated==FALSE)){
			all_player++;
		}
	}
	if(all_player==1){
		MovePen(start_map1x+50,start_map1y+50);
		SetPointSize(25);
		//string overprint = Player[turn_player]->name;
//		sprintf(a,"%s ��Ϊ���̣���Ϸ������",Player[turn_player]->name);
		InitGraphics();
		game_over = TRUE; 
		degree=0;
		spiral(Player[turn_player]->name);
		startTimer(0,75);
		
		return;//��Ϸ���� 
	}
	if (Player[turn_player]->wait)
	{
		if (--Player[turn_player]->wait == 0);
		else if (Player[turn_player]->place == 11)
		{
			Event = 12;
			ispurchase(0);
			turn_player++;
			if (turn_player > 3) {
				turn_player %= 4;
			}//��һ��ѭ�� 
			while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
				turn_player++;
				if (turn_player > 3) {
					turn_player %= 4;
				}//��һ��ѭ�� 
			}
			DrawPlayerMenu();
			return;
		}
		else if (Player[turn_player]->place == 30)
		{
			Event = 14;
			ispurchase(0);
			if (Player[turn_player]->money < 500)
				Player[turn_player]->money = 0;
			else Player[turn_player]->money -= 500;
			turn_player++;
			if (turn_player > 3) {
				turn_player %= 4;
			}//��һ��ѭ�� 
			while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
				turn_player++;
				if (turn_player > 3) {
					turn_player %= 4;
				}//��һ��ѭ�� 
			}
			DrawPlayerMenu();
			return;
		}
	}
	Player[turn_player]->place+=resultX;

	int pplace = Player[turn_player]->place;
	if(Player[turn_player]->place>37){
		Player[turn_player]->place%=38;
		if(Player[turn_player]->place>0){
			Player[turn_player]->money += 2000;
		}
	}//������㲻ͣ����ȡ2000Ԫ 
		
	nowcell = Player[turn_player]->place;
	if(nowcell>-1){
		DrawCellMenu();	
	} //���µ�ǰ��Ϸλ����Ϣ�˵� 

	DrawMap1(2);//����ͼ 

	switch(cell[Player[turn_player]->place]->cell_kind){
		case 1://���� 
			if(cell[Player[turn_player]->place]->owner==-1){
					if(Player[turn_player]->money - cell[Player[turn_player]->place]->construt_fee<0){
						//Ǯ������������
						turn_player++;
						if (turn_player > 3) {
							turn_player %= 4;
						}//��һ��ѭ�� 
						while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
							turn_player++;
							if (turn_player > 3) {
								turn_player %= 4;
							}//��һ��ѭ�� 
						}
						DrawPlayerMenu();
					}
					else{
						if (Player[turn_player]->mode == 2)
						{
							if(Player[turn_player]->money - cell[Player[turn_player]->place]->construt_fee>2000) 
							{
								Player[turn_player]->money -= cell[Player[turn_player]->place]->construt_fee;
							cell[Player[turn_player]->place]->owner = turn_player;
							DrawMap1(2);//����ͼ 
							Event = 3;
							ispurchase(0);
							}
							
							turn_player++;
							if (turn_player > 3) {
								turn_player %= 4;
							}//��һ��ѭ�� 
							while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//��һ��ѭ�� 
							}
							DrawPlayerMenu();
						}
						else
						{
							Event = 1;
							ispurchase(0);
						}
					}
			}//�յ� 
			else if(cell[Player[turn_player]->place]->owner==turn_player){//�Լ��ĵ�
				if(cell[Player[turn_player]->place]->house<4){
						if(Player[turn_player]->money - cell[Player[turn_player]->place]->construt_fee<0){
							//Ǯ������������ 
							turn_player++;
							if (turn_player > 3) {
								turn_player %= 4;
							}//��һ��ѭ�� 
							while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//��һ��ѭ�� 
							}
							DrawPlayerMenu();
						}
						else{
							if (Player[turn_player]->mode == 2)
							{
								if(Player[turn_player]->money - cell[Player[turn_player]->place]->construt_fee>2000){
									cell[Player[turn_player]->place]->house++;
									Player[turn_player]->money -= cell[Player[turn_player]->place]->construt_fee;
									DrawMap1(2);//����ͼ 
									Event = 4;
									ispurchase(0);
								}
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//��һ��ѭ�� 
								while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
									turn_player++;
									if (turn_player > 3) {
										turn_player %= 4;
									}//��һ��ѭ�� 
								}
								DrawPlayerMenu();
							}
							else
							{
								Event = 2;
								ispurchase(0);
							}
						}//�������� 
				}//���Լ������� 
			}//�Լ������� 
			else{//�����Լ��ĵ�
				if (Player[cell[Player[turn_player]->place]->owner]->wait > 0 && Player[cell[Player[turn_player]->place]->owner]->place == 11)//����������
				{
					Event = 15;
					ispurchase(0);
					turn_player++;
					if (turn_player > 3) {
						turn_player %= 4;
					}//��һ��ѭ�� 
					while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
						turn_player++;
						if (turn_player > 3) {
							turn_player %= 4;
						}//��һ��ѭ�� 
					}
				}
				else if(Player[turn_player]->money - cell[Player[turn_player]->place]->tolls[cell[Player[turn_player]->place]->house]<0){//Ǯ����
						sellhouse(turn_player); //������
						if(Player[turn_player]->money - cell[Player[turn_player]->place]->tolls[cell[Player[turn_player]->place]->house]<0){//����Ҳ����
							sellland(turn_player,Player[turn_player]->money - cell[Player[turn_player]->place]->tolls[cell[Player[turn_player]->place]->house]); //����
							if (Player[turn_player]->money - cell[Player[turn_player]->place]->tolls[cell[Player[turn_player]->place]->house] < 0) {//����Ҳ����
								Player[turn_player]->isDefeated = TRUE;//�����̭
								all_player--;
								DrawMap1(2);//����ͼ 
								Event = 8;
								ispurchase(0);
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//��һ��ѭ�� 
								while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
									turn_player++;
									if (turn_player > 3) {
										turn_player %= 4;
									}//��һ��ѭ�� 
								}
							}
							else {//���ع���
								Player[turn_player]->money -= cell[Player[turn_player]->place]->tolls[cell[Player[turn_player]->place]->house];
								Player[cell[Player[turn_player]->place]->owner]->money += cell[Player[turn_player]->place]->tolls[cell[Player[turn_player]->place]->house];
								DrawMap1(2);//����ͼ 
								Event = 17;
								ispurchase(0);
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//��һ��ѭ�� 
								while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
									turn_player++;
									if (turn_player > 3) {
										turn_player %= 4;
									}//��һ��ѭ�� 
								}
							}
						}//����
						else//��������
						{
							Player[turn_player]->money -= cell[Player[turn_player]->place]->tolls[cell[Player[turn_player]->place]->house];
							Player[cell[Player[turn_player]->place]->owner]->money += cell[Player[turn_player]->place]->tolls[cell[Player[turn_player]->place]->house];
							DrawMap1(2);//����ͼ 
							Event = 16;
							ispurchase(0);
						}
				}
				else{//Ǯ��
					Player[turn_player]->money -= cell[Player[turn_player]->place]->tolls[cell[Player[turn_player]->place]->house];
					Player[cell[Player[turn_player]->place]->owner]->money += cell[Player[turn_player]->place]->tolls[cell[Player[turn_player]->place]->house];
					DrawMap1(2);//����ͼ 
					Event = 7;
					ispurchase(0);
					turn_player++;
					if (turn_player > 3) {
						turn_player %= 4;
					}//��һ��ѭ�� 
					while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
						turn_player++;
						if (turn_player > 3) {
							turn_player %= 4;
						}//��һ��ѭ�� 
					}
				}//�Լ���Ǯ��ӵ���߼�Ǯ 
				DrawPlayerMenu();
			}//���˵ĵ� 
			break;
		case 2:
			destiny(turn_player);
			turn_player++;
			if (turn_player > 3) {
				turn_player %= 4;
			}//��һ��ѭ�� 
			while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
				turn_player++;
				if (turn_player > 3) {
					turn_player %= 4;
				}//��һ��ѭ�� 
			}
			DrawPlayerMenu();
			break; //���� 
		case 3:
			Player[turn_player]->money -= Player[turn_player]->money/10;
			Event = 7;
			ispurchase(0);
			turn_player++;
			if (turn_player > 3) {
				turn_player %= 4;
			}//��һ��ѭ�� 
			while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
				turn_player++;
				if (turn_player > 3) {
					turn_player %= 4;
				}//��һ��ѭ�� 
			}
			DrawPlayerMenu();
			break;//����˰ �۳�10%�ֽ� 
		case 4:
			if(cell[Player[turn_player]->place]->owner==-1){
					if(Player[turn_player]->money - cell[Player[turn_player]->place]->construt_fee<0){
						//Ǯ���� 
						turn_player++;
						if (turn_player > 3) {
							turn_player %= 4;
						}//��һ��ѭ�� 
						while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
							turn_player++;
							if (turn_player > 3) {
								turn_player %= 4;
							}//��һ��ѭ�� 
						}
						DrawPlayerMenu();
					}
					else{
						if (Player[turn_player]->mode == 2)
						{
							if(Player[turn_player]->money - cell[Player[turn_player]->place]->construt_fee>2000){
								Player[turn_player]->money -= cell[Player[turn_player]->place]->construt_fee;
								cell[Player[turn_player]->place]->owner = turn_player;
								DrawMap1(2);//����ͼ 
								Event = 3;
								ispurchase(0);
							}
							turn_player++;
							if (turn_player > 3) {
								turn_player %= 4;
							}//��һ��ѭ�� 
							while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//��һ��ѭ�� 
							}
							DrawPlayerMenu();
						}
						else
						{
							Event = 1;
							ispurchase(0);
						}
					}
			}//�յ� 
			else{
				if (Player[turn_player]->money - 500 < 0)
				{
					turn_player++;
					if (turn_player > 3) {
						turn_player %= 4;
					}//��һ��ѭ�� 
					while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
						turn_player++;
						if (turn_player > 3) {
							turn_player %= 4;
						}//��һ��ѭ�� 
					}
					DrawPlayerMenu();
				}
				else{
					if (Player[turn_player]->mode == 2)
					{
						if (Player[turn_player]->money - 500 > 2000)
						{
							Player[turn_player]->money -= 500;
							Player[cell[Player[turn_player]->place]->owner]->money += 500;
							DrawMap1(2);//����ͼ
							Event = 6;
							ispurchase(0);
							if (Player[turn_player]->place == 5 || Player[turn_player]->place == 15) {
								Player[turn_player]->place += 10;
							}
							else if (Player[turn_player]->place == 25) {
								Player[turn_player]->place = 33;
							}
							else if (Player[turn_player]->place == 33) {
								Player[turn_player]->place = 5;
							}
						}
						turn_player++;
						if (turn_player > 3) {
							turn_player %= 4;
						}//��һ��ѭ�� 
						while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
							turn_player++;
							if (turn_player > 3) {
								turn_player %= 4;
							}//��һ��ѭ�� 
						}
						DrawPlayerMenu();
					}
					else if (Player[turn_player]->mode == 1)
					{
						Event = 5;
						ispurchase(0);
					}
				}
			}
			break;//��վ
		case 5:
			if(cell[Player[turn_player]->place]->owner==-1){
				if(1){
					if(Player[turn_player]->money - cell[Player[turn_player]->place]->construt_fee<0){
						//Ǯ���� 
						turn_player++;
						if (turn_player > 3) {
							turn_player %= 4;
						}//��һ��ѭ�� 
						while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
							turn_player++;
							if (turn_player > 3) {
								turn_player %= 4;
							}//��һ��ѭ�� 
						}
						DrawPlayerMenu();
					}
					else{
						if (Player[turn_player]->mode == 2)
						{
							if(Player[turn_player]->money - cell[Player[turn_player]->place]->construt_fee>2000){
								Player[turn_player]->money -= cell[Player[turn_player]->place]->construt_fee;
								cell[Player[turn_player]->place]->owner = turn_player;
								DrawMap1(2);//����ͼ 
								Event = 3;
								ispurchase(0);
							}
							turn_player++;
							if (turn_player > 3) {
								turn_player %= 4;
							}//��һ��ѭ�� 
							while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//��һ��ѭ�� 
							}
							DrawPlayerMenu();
						}
						else
						{
							Event = 1;
							ispurchase(0);
						}
					}
				}
			}//�յ� 
			else if (cell[Player[turn_player]->place]->owner == turn_player)//���Լ���
			{
				turn_player++;
				if (turn_player > 3) {
					turn_player %= 4;
				}//��һ��ѭ�� 
				while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
					turn_player++;
					if (turn_player > 3) {
						turn_player %= 4;
					}//��һ��ѭ�� 
				}
				DrawPlayerMenu();
			}
			else{//-500
				if (Player[cell[Player[turn_player]->place]->owner]->wait > 0 && Player[cell[Player[turn_player]->place]->owner]->place == 11)//����������
				{
					Event = 15;
					ispurchase(0);
					turn_player++;
					if (turn_player > 3) {
						turn_player %= 4;
					}//��һ��ѭ�� 
					while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
						turn_player++;
						if (turn_player > 3) {
							turn_player %= 4;
						}//��һ��ѭ�� 
					}
				}
				else if(cell[12]->owner!=cell[28]->owner)
				{
					if (Player[turn_player]->money - 500 < 0) {//Ǯ����
						sellhouse(turn_player); //������
						if (Player[turn_player]->money - 500 < 0) {//������Ҳ����
							sellland(turn_player, Player[turn_player]->money - 500); //����
							if (Player[turn_player]->money - 500 < 0) {//����Ҳ����
								Player[turn_player]->isDefeated = TRUE;//�����̭ 
								all_player--;
								DrawMap1(2);//����ͼ 
								Event = 8;
								ispurchase(0);
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//��һ��ѭ�� 
								while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
									turn_player++;
									if (turn_player > 3) {
										turn_player %= 4;
									}//��һ��ѭ�� 
								}
							}
							else {//���ع���
								Player[turn_player]->money -= 500;
								Player[cell[Player[turn_player]->place]->owner]->money += 500;
								DrawMap1(2);//����ͼ 
								Event = 17;
								ispurchase(0);
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//��һ��ѭ�� 
								while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
									turn_player++;
									if (turn_player > 3) {
										turn_player %= 4;
									}//��һ��ѭ�� 
								}
							}
						}
						else//��������
						{
							Player[turn_player]->money -= 500;
							Player[cell[Player[turn_player]->place]->owner]->money += 500;
							DrawMap1(2);//����ͼ 
							Event = 16;
							ispurchase(0);
							turn_player++;
							if (turn_player > 3) {
								turn_player %= 4;
							}//��һ��ѭ�� 
							while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//��һ��ѭ�� 
							}
						}
					}
					else//Ǯ��
					{
						Player[turn_player]->money -= 500;
						Player[cell[Player[turn_player]->place]->owner]->money += 500;
						DrawMap1(2);//����ͼ 
						Event = 7;
						ispurchase(0);
						turn_player++;
						if (turn_player > 3) {
							turn_player %= 4;
						}//��һ��ѭ�� 
						while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
							turn_player++;
							if (turn_player > 3) {
								turn_player %= 4;
							}//��һ��ѭ�� 
						}
					}
				}
				else{//-1000
					if (Player[turn_player]->money - 1000 < 0) {//Ǯ����
						sellhouse(turn_player); //������
						if (Player[turn_player]->money - 1000 < 0) {//������Ҳ����
							sellland(turn_player, Player[turn_player]->money - 1000); //����
							if (Player[turn_player]->money - 1000 < 0) {//����Ҳ����
								Player[turn_player]->isDefeated = TRUE;//�����̭ 
								all_player--;
								DrawMap1(2);//����ͼ 
								Event = 8;
								ispurchase(0);
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//��һ��ѭ�� 
								while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
									turn_player++;
									if (turn_player > 3) {
										turn_player %= 4;
									}//��һ��ѭ�� 
								}
							}
							else {//���ع���
								Player[turn_player]->money -= 1000;
								Player[cell[Player[turn_player]->place]->owner]->money += 1000;
								DrawMap1(2);//����ͼ 
								Event = 17;
								ispurchase(0);
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//��һ��ѭ�� 
								while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
									turn_player++;
									if (turn_player > 3) {
										turn_player %= 4;
									}//��һ��ѭ�� 
								}
							}
						}
						else//��������
						{
							Player[turn_player]->money -= 1000;
							Player[cell[Player[turn_player]->place]->owner]->money += 1000;
							DrawMap1(2);//����ͼ 
							Event = 16;
							ispurchase(0);
							turn_player++;
							if (turn_player > 3) {
								turn_player %= 4;
							}//��һ��ѭ�� 
							while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//��һ��ѭ�� 
							}
						}
					}
					else//Ǯ��
					{
						Player[turn_player]->money -= 1000;
						Player[cell[Player[turn_player]->place]->owner]->money += 1000;
						DrawMap1(2);//����ͼ 
						Event = 7;
						ispurchase(0);
						turn_player++;
						if (turn_player > 3) {
							turn_player %= 4;
						}//��һ��ѭ�� 
						while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
							turn_player++;
							if (turn_player > 3) {
								turn_player %= 4;
							}//��һ��ѭ�� 
						}
					}
				}
				DrawPlayerMenu();
			}
			break;//������˾������ˮ��
		case 6:
			turn_player++;
			if (turn_player > 3) {
				turn_player %= 4;
			}//��һ��ѭ�� 
			while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
				turn_player++;
				if (turn_player > 3) {
					turn_player %= 4;
				}//��һ��ѭ�� 
			}
			DrawPlayerMenu();
			break;//ҽԺ
		case 7:
			turn_player++;
			if (turn_player > 3) {
				turn_player %= 4;
			}//��һ��ѭ�� 
			while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
				turn_player++;
				if (turn_player > 3) {
					turn_player %= 4;
				}//��һ��ѭ�� 
			}
			DrawPlayerMenu();
			break;//���� 
		case 8:
			turn_player++;
			if (turn_player > 3) {
				turn_player %= 4;
			}//��һ��ѭ�� 
			while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
				turn_player++;
				if (turn_player > 3) {
					turn_player %= 4;
				}//��һ��ѭ�� 
			}
			DrawPlayerMenu();
			break;//ͣ����					
	}
}

/*���˳齱*/
void destiny(int playerid){
	int numdest;
	MovePen(start_map1x+67,start_map1y+65);
	
	SetPenColor("smoke"); 
	StartFilledRegion(1);
	DrawLine(cell_lenth*2,0);
	DrawLine(0,-cell_lenth);
	DrawLine(-cell_lenth*2,0);
	DrawLine(0,cell_lenth);
	EndFilledRegion();	

	if(playerid==-1){
		numdest = 0;
	}
	else if(playerid==-2){
		numdest = last_destiny;
	}
	else{
		numdest = rand()%15+1;
		last_destiny = numdest;
	}
	SetPointSize(1);
	SetPenColor("black");
	MovePen(start_map1x + 75, start_map1y + 64);
	DrawTextString("           ����");
	
	switch(numdest){
		case 0:
			SetPenColor("rred");  
			MovePen(start_map1x + 75, start_map1y + 60);
			DrawTextString("    ����֮�ﰡ");
			MovePen(start_map1x + 75, start_map1y + 57);
			DrawTextString("��ѭ��֮ħ�ȵ�ָ��");
			MovePen(start_map1x + 75, start_map1y + 54);
			DrawTextString("  ������֮������");
			break;
		case 1:
			SetPenColor("rred");  
			MovePen(start_map1x + 70, start_map1y + 60);
			DrawTextString(" �ڰ��������ϴ󹫹���С��");
			MovePen(start_map1x + 70, start_map1y + 57);
			DrawTextString("����������ˮ");
			MovePen(start_map1x + 70, start_map1y + 54);
			DrawTextString(" �⳥500Ԫ");
			if(playerid<0) break;
			if (Player[playerid]->money < 500)
				Player[playerid]->money = 0;
			else Player[playerid]->money -= 500;
			Event = 7;
			ispurchase(0);
			break;
		case 2:
			SetPenColor("rred");  
			MovePen(start_map1x + 70, start_map1y + 60);
			DrawTextString("�ڷ�����ֹ���˵���");
			MovePen(start_map1x + 70, start_map1y + 57);
			DrawTextString("������Ķ�����");
			MovePen(start_map1x + 70, start_map1y + 54);
			DrawTextString("����800Ԫ");
			if(playerid<0) break;
			Player[playerid]->money+=800;
			Event = 10;
			ispurchase(0);
			break;
		case 3:
			SetPenColor("rred");  
			MovePen(start_map1x + 70, start_map1y + 60);
			DrawTextString("���¼��¹۹�ʱ�Ҷ�����");
			MovePen(start_map1x + 70, start_map1y + 57);
			DrawTextString("����400Ԫ");
			if(playerid<0) break;
			if (Player[playerid]->money < 400)
				Player[playerid]->money = 0;
			else Player[playerid]->money -= 400;
			Event = 7;
			ispurchase(0);
			break;
		case 4:
			SetPenColor("rred");  
			MovePen(start_map1x + 70, start_map1y + 60);
			DrawTextString("���ϼ���½̽�տ��������ļ���");
			MovePen(start_map1x + 75, start_map1y + 57);
			DrawTextString("����700Ԫ");
			if(playerid<0) break;
			if (Player[playerid]->money < 700)
				Player[playerid]->money = 0;
			else Player[playerid]->money -= 700;
			Event = 7;
			ispurchase(0);
			break;
		case 5:
			SetPenColor("rred");  
			MovePen(start_map1x + 70, start_map1y + 60);
			DrawTextString("��ˡ�ŷ��֮�ǡ�");
			MovePen(start_map1x + 70, start_map1y + 57);
			DrawTextString("ֱ���ƶ���Ӣ��");
			if(playerid<0) break;
			Player[playerid]->place = 21;
			break;
		case 6:
			SetPenColor("rred");  
			MovePen(start_map1x + 70, start_map1y + 60);
			DrawTextString("��̩������̩������û��");
			MovePen(start_map1x + 70, start_map1y + 57);
			DrawTextString("��������");
			MovePen(start_map1x + 70, start_map1y + 54);
			DrawTextString("����600Ԫ");
			if(playerid<0) break;
			if (Player[playerid]->money < 600)
				Player[playerid]->money = 0;
			else Player[playerid]->money -= 600;
			Event = 7;
			ispurchase(0);
			break;
		case 7:
			SetPenColor("rred");  
			MovePen(start_map1x + 70, start_map1y + 60);
			DrawTextString("�ڼ�Ȧ�ı�������ʧ���");
			MovePen(start_map1x + 70, start_map1y + 57);
			DrawTextString("���������ȡ�");
			MovePen(start_map1x + 70, start_map1y + 54);
			DrawTextString("����700Ԫ");
			if(playerid<0) break;
			Player[playerid]->money+=700;
			Event = 10;
			ispurchase(0);
			break;
		case 8:
			SetPenColor("rred");  
			MovePen(start_map1x+70,start_map1y+60);
			DrawTextString("���ձ�������ӣ������");
			MovePen(start_map1x + 70, start_map1y + 57);
			DrawTextString("�ù�¶�޽�ͷ");
			MovePen(start_map1x + 70, start_map1y + 54);
			DrawTextString("��ο��200Ԫ");
			if(playerid<0) break;
			Player[playerid]->money+=200;
			Event = 10;
			ispurchase(0);
			break;
		case 9:
			SetPenColor("rred");  
			MovePen(start_map1x+70,start_map1y+60);
			DrawTextString("����ٸ��о���������ʷ");
			MovePen(start_map1x + 70, start_map1y + 57);
			DrawTextString("����1000Ԫ");
			if(playerid<0) break;
			Player[playerid]->money+=1000;
			Event = 10;
			ispurchase(0);
			break;
		case 10:
			SetPenColor("rred");  
			MovePen(start_map1x+70,start_map1y+60);
			DrawTextString("������˹��ͷ���ͷ����");
			MovePen(start_map1x + 70, start_map1y + 57);
			DrawTextString("һ�����");
			MovePen(start_map1x + 70, start_map1y + 54);
			DrawTextString("����600Ԫ");
			if(playerid<0) break;
			Player[playerid]->money+=600;
			Event = 10;
			ispurchase(0);
			break;
		case 11:
			SetPenColor("rred");  
			MovePen(start_map1x+70,start_map1y+60);
			DrawTextString("��������Ͷ�ʵĿ�����Ʒ");
			MovePen(start_map1x + 70, start_map1y + 57);
			DrawTextString("��������������");
			MovePen(start_map1x + 70, start_map1y + 54);
			DrawTextString("����2000Ԫ");
			if(playerid<0) break;
			Player[playerid]->money+=2000;
			Event = 10;
			ispurchase(0);
			break;
		case 12:
			SetPenColor("rred");  
			MovePen(start_map1x+70,start_map1y+60);
			DrawTextString("�����޴򹤶ȼ�");
			MovePen(start_map1x+70,start_map1y+57);
			DrawTextString("����900Ԫ");
			if(playerid<0) break;
			Player[playerid]->money+=900;
			Event = 10;
			ispurchase(0);
			break;
		case 13:
			SetPenColor("rred");  
			MovePen(start_map1x+70,start_map1y+60);
			DrawTextString("��������·�ı����ܱ���");
			MovePen(start_map1x+70,start_map1y+57);
			DrawTextString("����800Ԫ");
			if(playerid<0) break;
			Player[playerid]->money+=800;
			Event = 10;
			ispurchase(0);
			break;
		case 14:
			SetPenColor("rred");
			MovePen(start_map1x + 70, start_map1y + 60);
			DrawTextString("�÷Ƿ��ֶ����Ʊ��淢");
			MovePen(start_map1x + 70, start_map1y + 57);
			DrawTextString("��������ͣ���غ�");
			if (playerid < 0) break;
			Player[playerid]->place = 11;
			Player[playerid]->wait = 3;
			Event = 11;
			ispurchase(0);
			break;
		case 15:
			SetPenColor("rred");
			MovePen(start_map1x + 70, start_map1y + 60);
			DrawTextString("���ڼ�����������������");
			MovePen(start_map1x + 70, start_map1y + 57);
			DrawTextString("������סԺ����ͣ���غ�");
			if (playerid < 0) break;
			Player[playerid]->place = 30;
			Player[playerid]->wait = 2;
			Event = 13;
			ispurchase(0);
			break;
	}
} 

/*�������¼�����*/ 
void ispurchase(int isdown){//Event:1-�������أ�2-�����ݣ�3-���������أ�4-�����˷��ݣ�5-������6-���˳���7-��Ǯ��8-�Ʋ���9-��������

	double prex,prey;
	char text[100];
	
	SetPenSize(1);
	MovePen(start_map1x-65,start_map1y+50);//5  70
	
	SetPenColor("smoke"); 
	StartFilledRegion(1);
	DrawLine(cell_lenth*3,0);
	DrawLine(0,-cell_lenth);
	DrawLine(-cell_lenth*3,0);
	DrawLine(0,cell_lenth);
	EndFilledRegion();
	
	prex = GetCurrentX(); //5
	prey = GetCurrentY();//70
	
	MovePen(prex+5,prey-3);
	SetPenColor("deepblue");
	switch (Event)
	{
		case 1:
			sprintf(text, "    �Ƿ��� %s ?", cell[Player[turn_player]->place]->name);
			DrawTextString(text);
			break;
		case 2:
			sprintf(text, "    �Ƿ��� %s ������?", cell[Player[turn_player]->place]->name);
			DrawTextString(text);
			break;
		case 3:
			DrawTextString("        �� �� �� �� ��");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "    %s ������ %s", Player[turn_player]->name, cell[Player[turn_player]->place]->name);
			DrawTextString(text);
			break;
		case 4:
			DrawTextString("        �� �� �� �� ��");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "    %s �� %s �����˷���", Player[turn_player]->name, cell[Player[turn_player]->place]->name);
			DrawTextString(text);
			break;
		case 5:
			DrawTextString("        �Ƿ���Ʊ��");
			break;
		case 6:
			DrawTextString("        �� �� �� �� ��");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "        %s �����˻�", Player[turn_player]->name);
			DrawTextString(text);
			if (cell[Player[turn_player]->place]->owner != turn_player)
			{
				MovePen(prex + 5, prey - 12);
				sprintf(text, "        %s �������", Player[cell[Player[turn_player]->place]->owner]->name);
				DrawTextString(text);
			}
			break;
		case 7:
			DrawTextString("        �� �� �� �� ��");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "        %s �������ֱ���", Player[turn_player]->name);
			DrawTextString(text);
			if (cell[Player[turn_player]->place]->owner != -1)
			{
				MovePen(prex + 5, prey - 12);
				sprintf(text, "        %s �������", Player[cell[Player[turn_player]->place]->owner]->name);
				DrawTextString(text);
			}
			break;
		case 8:
			DrawTextString("        �� �� �� �� ��");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "        %s �Ʋ��ˣ�", Player[turn_player]->name);
			DrawTextString(text);
			break;
		case 9:
			DrawTextString("        �� �� �� �� ��");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "        %s �����˹���", Player[turn_player]->name);
			DrawTextString(text);
			break;
		case 10:
			DrawTextString("        �� �� �� �� ��");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "        %s �������", Player[turn_player]->name);
			DrawTextString(text);
			break;
		case 11:
			DrawTextString("        �� �� �� �� ��");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "        %s �������", Player[turn_player]->name);
			DrawTextString(text);
			break;
		case 12:
			DrawTextString("        �� �� �� �� ��");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "        %s ������ʹ��ǰ��", Player[turn_player]->name);
			DrawTextString(text);
			MovePen(prex + 5, prey - 12);
			sprintf(text, "        ��������� %d �غ�", Player[turn_player]->wait);
			DrawTextString(text);
			break;
		case 13:
			DrawTextString("        �� �� �� �� ��");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "        %s סԺ��", Player[turn_player]->name);
			DrawTextString(text);
			break;
		case 14:
			DrawTextString("        �� �� �� �� ��");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "        %s ����סԺ��500", Player[turn_player]->name);
			DrawTextString(text);
			MovePen(prex + 5, prey - 12);
			sprintf(text, "        ���Ժ���� %d �غ�", Player[turn_player]->wait);
			DrawTextString(text);
			break;
		case 15:
			DrawTextString("        �� �� �� �� ��");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "        %s ��������", Player[cell[Player[turn_player]->place]->owner]->name);
			DrawTextString(text);
			MovePen(prex + 5, prey - 12);
			sprintf(text, "        %s ���һ��", Player[turn_player]->name);
			DrawTextString(text);
			break;
		case 16:
			DrawTextString("        �� �� �� �� ��");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "        %s �ҹ��������ڻ���ծ", Player[turn_player]->name);
			DrawTextString(text);
			break;
		case 17:
			DrawTextString("        �� �� �� �� ��");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "        %s ��ҵ������ڻ���ծ", Player[turn_player]->name);
			DrawTextString(text);
			break;
	}
	
	if (Event == 1 || Event == 2 || Event == 5)
	{//���û�в��� 
		if (isdown == 0) {

			MovePen(prex + 5, prey - 6);//10  64
			SetPenColor("matcha");
			StartFilledRegion(1);
			DrawLine(cell_lenth*0.8, 0);//x 10  22  y 64  57
			DrawLine(0, -cell_lenth * 0.5);
			DrawLine(-cell_lenth * 0.8, 0);
			DrawLine(0, cell_lenth*0.5);
			EndFilledRegion();
			MovePen(prex + 5, prey - 12);
			SetPenColor("deepblue");
			DrawTextString("     ��");


			MovePen(prex + 25, prey - 6);
			SetPenColor("matcha");
			StartFilledRegion(1);
			DrawLine(cell_lenth*0.8, 0);
			DrawLine(0, -cell_lenth * 0.5);
			DrawLine(-cell_lenth * 0.8, 0);
			DrawLine(0, cell_lenth*0.5);
			EndFilledRegion();
			MovePen(prex + 25, prey - 12);
			SetPenColor("deepblue");
			DrawTextString("     ��");
		}
		//������ 
		else if (isdown == 1) {
			MovePen(prex + 4, prey - 7);
			SetPenColor("navyblue");
			StartFilledRegion(1);
			DrawLine(cell_lenth*0.8, 0);
			DrawLine(0, -cell_lenth * 0.5);
			DrawLine(-cell_lenth * 0.8, 0);
			DrawLine(0, cell_lenth*0.5);
			EndFilledRegion();
			MovePen(prex + 4, prey - 13);
			SetPenColor("babypink");
			DrawTextString("     ��");

			MovePen(prex + 25, prey - 6);
			SetPenColor("matcha");
			StartFilledRegion(1);
			DrawLine(cell_lenth*0.8, 0);
			DrawLine(0, -cell_lenth * 0.5);
			DrawLine(-cell_lenth * 0.8, 0);
			DrawLine(0, cell_lenth*0.5);
			EndFilledRegion();
			MovePen(prex + 25, prey - 12);
			SetPenColor("deepblue");
			DrawTextString("     ��");
		}
		//���·� 
		else if (isdown == 2) {
			MovePen(prex + 5, prey - 6);//10  64
			SetPenColor("matcha");
			StartFilledRegion(1);
			DrawLine(cell_lenth*0.8, 0);//x 10  22  y 64  71
			DrawLine(0, -cell_lenth * 0.5);
			DrawLine(-cell_lenth * 0.8, 0);
			DrawLine(0, cell_lenth*0.5);
			EndFilledRegion();
			MovePen(prex + 5, prey - 12);
			SetPenColor("deepblue");
			DrawTextString("     ��");

			MovePen(prex + 24, prey - 7);
			SetPenColor("navyblue");
			StartFilledRegion(1);
			DrawLine(cell_lenth*0.8, 0);
			DrawLine(0, -cell_lenth * 0.5);
			DrawLine(-cell_lenth * 0.8, 0);
			DrawLine(0, cell_lenth*0.5);
			EndFilledRegion();
			MovePen(prex + 24, prey - 13);
			SetPenColor("babypink");
			DrawTextString("     ��");
		}
	}
	else Event = 0;
}

/*����㿪ʼѭ�������أ�ֱ��Ǯ��*/ 
void sellland(int playerid,int difference){
	if(difference<0)  difference = -difference;
	int i=0;
	for(i = 0;i<38;i++){
		if(cell[i]->owner==playerid){
			Player[playerid]->money += cell[i]->mortgage_fee;//��Ѻ�۸���� 
			cell[i]->owner = -1;//�����ջ�			 
		}
	}
}

/*����������з��ӣ���۳���*/ 
void sellhouse(int playerid){
	int i=0;
	for(i = 0;i<38;i++){
		if(cell[i]->owner==playerid){
			while(cell[i]->house!=0){
				Player[playerid]->money += cell[i]->tolls[cell[i]->house]/2;
				cell[i]->house--;
			}
		}
	}
}

void DrawPlayer()
{
	double l = 2;
	double x, y;
	char a[2];
	int i;
	for (i = 0; i < 4; i++)
	{
		if (Player[i]->mode)
		{
			sprintf(a, "%d", i + 1);
			if (Player[i]->place == 0)
			{
				x = 58 + i % 2 * 4;
				y = 12 - i / 2 * 4;
			}
			else if (Player[i]->place < 11)
			{
				x = 73 + i % 2 * 4 + (Player[i]->place - 1) * 10;
				y = 12 - i / 2 * 4;
			}
			else if (Player[i]->place == 11)
			{
				x = 178 + i % 2 * 4;
				y = 12 - i / 2 * 4;
			}
			else if (Player[i]->place < 19)
			{
				x = 178 + i % 2 * 4;
				y = 27 - i / 2 * 4 + (Player[i]->place - 12) * 10;
			}
			else if (Player[i]->place == 19)
			{
				x = 178 + i % 2 * 4;
				y = 102 - i / 2 * 4;
			}
			else if (Player[i]->place < 30)
			{
				x = 163 + i % 2 * 4 - (Player[i]->place - 20) * 10;
				y = 102 - i / 2 * 4;
			}
			else if (Player[i]->place == 30)
			{
				x = 58 + i % 2 * 4;
				y = 102 - i / 2 * 4;
			}
			else if (Player[i]->place < 38)
			{
				x = 58 + i % 2 * 4;
				y = 87 - i / 2 * 4 - (Player[i]->place - 31) * 10;
			}

			MovePen(x, y);
			SetPenColor(Player[i]->color);
			SetPenSize(1);

			StartFilledRegion(1);
			{
				switch (Player[i]->shape) {
				case 1:
					MovePen(x + 2, y);
					DrawArc(l, 0, 360);
					break;
				case 2:
					MovePen(x + 1.5, y - 1.5);
					DrawLine(0, 3);
					DrawLine(-3, 0);
					DrawLine(0, -3);
					DrawLine(3, 0);
					break;
				case 3:
					MovePen(x - l, y - l);
					DrawLine(2 * l, 0);
					DrawLine(-l, 2 * l);
					DrawLine(-l, -2 * l);
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
		}
	}
}

/*������ת�����ַ���*/
void numtostring(int num,char* numstring){
	int digit = 0;
	if(num<0){
		numstring[0] = '-';
		num = -num; 
		digit++;
		return;
	}//�Ǹ��� 
	int onum = num;
	do{
		onum/=10;
		digit++;
	}while(onum!=0);
	numstring[digit] = 0;
	digit--;
	do{
		numstring[digit] = num%10 + '0';
		digit--;
		num/=10;
	}while(num!=0);
} 

/*����д�ı���Ȼ�󻭱ʻص���ʼλ��*/ 
void ErectText(string text){
	double prex,prey;
	prex = GetCurrentX(); 
	prey = GetCurrentY();
	int num = 0;
	char te[3] = {0,0,0};
	double lenth;
	while(text[num]!=0){
		te[0] = text[num];
		te[1] = text[num+1];
		num += 2;
		lenth = TextStringWidth(te);
		MovePen(prex+1,prey - lenth*num*0.5);
		DrawTextString(te);
	}
	MovePen(prex,prey);
}
