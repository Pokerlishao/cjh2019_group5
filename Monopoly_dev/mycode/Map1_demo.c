/***********************************************************
	map1_demo.c 
	
	鼠标回调 	void Mouse_Map1(int x, int y, int button, int event);
	键盘回调 	void Keyboard_Turntable(int key,int event); 

	函数说明: 画地图，初始化地图 
			点击地图时显示当前位置信息 
	地图结构：
		cell-> 
			string name;                 地名 
			int cell_kind;               判断断类型 ：	|***************************
			int owner;//拥有者 ,值为-1时代表无主 		|	0：起点 
			int construt_fee;//建筑费 					|	1：土地 
			int house;//房子个数 ，上限为4 				|	2为机会  
			int tolls[5];//根据当前房子个数收取过路费 	|	3为所得税  
			int mortgage_fee;//抵押费 					|	4为火车站 
														|	5为电力公司与自来水厂  
														|	6为医院  
														|	7为监狱
														|	8为停车场 
														|**************************

	创建时间 :2019/5/17  22:24
	修改：2019/5/25 19:52
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
int turn_player = 0;//当前轮到第几个玩家 
static bool purchase = FALSE;
bool game_over = FALSE;//游戏结束标志 
static int  last_destiny = 0; 
static int buy = -1;//购买标志
static int Event = 0;// : 1 - 购买土地；2 - 购买房屋；3 - 购买了土地；4 - 购买了房屋；5 - 坐车；6 - 坐了车；7 - 交钱；8 - 破产；9-放弃购买；10-得钱；11-入狱；12-出狱；13-住院；14-出院；15-不交钱；16-卖房子；17-卖地

//=========================================================================================================================================================================================================

static void DrawCellMenu();//画地图信息菜单 
static void DrawPlayerMenu();//画玩家信息菜单 
static void DrawPlayer();//画玩家 
static void ispurchase(int isdown);//购买菜单 
static void DrawCell(int,int);//画每一个格子 
static void InitCell(Cell_AllT celli,string name,	int cell_kind,int owner,int construt_fee, int house,
			 int tolls0,int tolls1, int tolls2, int tolls3, int tolls4, int mortgage_fee);//辅助初始化 
static void sellland(int playerid,int difference);//卖地
static void sellhouse(int playerid); //卖房子 
static void ErectText(string text);//竖着写文本 ，然后画笔回到初始位置 
static void numtostring(int num,char* numstring);//数字转换成字符串 
static void destiny(int playerid);//命运 

//=========================================================================================================================================================================================================

/*画地图1*/
void MainMap1(){
	int i = 0;
	InitCelldata();/*初始化地图数据*/ 
	while (Player[i]->mode == 0) i++;
	turn_player = i;
	DrawMap1(1);/*画地图*/ 
}

/*画Map1*/
void DrawMap1(int fuck) {
	InitGraphics();

	int i;
	/*画地图顶点*/
	MovePen(start_map1x, start_map1y);
	SetPenSize(2);
	SetPenColor("pink");/*顶点颜色*/
	DrawCell(5, 0);
	MovePen(start_map1x + cell_width * 10 + cell_lenth, start_map1y);
	DrawCell(5, 11);
	MovePen(start_map1x + cell_width * 10 + cell_lenth, start_map1y + cell_width * 7 + cell_lenth);
	DrawCell(5, 19);
	MovePen(start_map1x, start_map1y + cell_width * 7 + cell_lenth);
	DrawCell(5, 30);
	/*画地图四边 */
	MovePen(start_map1x, start_map1y);
	SetPenColor("grass");/*四边颜色*/
	for (i = 1; i < 11; i++) {
		DrawCell(1, i);
	}/*底*/
	for (i = 12; i < 19; i++) {
		DrawCell(2, i);
	}/*右边*/
	for (i = 20; i < 30; i++) {
		DrawCell(3, i);
	}/*上面*/
	for (i = 31; i < 38; i++) {
		DrawCell(4, i);
	}/*左面*/

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
	DrawTextString("按下空格键来开始转动转盘");
	MovePen(85, 72);
	DrawTextString("按下回车键来停止转动转盘");
	SetPointSize(1);

	MovePen(123, 54);
	SetPenColor("deepblue");
	ErectText("前进步数：");

	SetPenSize(1);
	MenuBar();
	DrawTurnTable(120, 45, 18.0);/*画转盘*/
	DrawCellMenu();/*画地图信息菜单*/
	DrawPlayer();//更新玩家位置 
	DrawPlayerMenu();//更新玩家信息菜单 
	/*初始化地图传参为 1， 刷新时传参为2 */
	if (fuck == 1) {
		destiny(-1);//命运 
	}
	else if (fuck == 2) {
		destiny(-2);
	}
}

/*键盘信息*/
int KeyBoard_Map1(int key, int event)
{
	if (game_over){
		switch (event) {
	 		case KEY_DOWN:
	 			cancelTimer(0);
	 			InitGraphics();/*初始化图形界面 */
	 			start_menu();
				degree = 0;
				break;
		}
	}
	if (!Event)
		Keyboard_Turntable(key, event);
	return KeyBoard_about_MenuBar(key, event);
}

/*定时器信息*/
void Timer_Map1()
{
	Timer_Turntable();
}

/*鼠标信息*/
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
		/*判断鼠标是否在地图里*/
	case MOUSEMOVE:
		if ((mouse_x < 185 && mouse_x>55) && (mouse_y < 105 && mouse_y>5)) {
			/*转换到地图坐标*/
			int mx = mouse_x - 55;
			int my = mouse_y - 5;
			/*鼠标在底面*/
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
			/*鼠标在顶面*/
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
			/*鼠标在左面*/
			else if (mx < cell_lenth) {
				nowcell = 37 - (my - cell_lenth) / cell_width;
			}
			/*鼠标在右面*/
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
			/*判断是否购买*/
			if ((mouse_x < 22 && mouse_x>10) && (mouse_y < 64 && mouse_y>57)) {
				buy = 1;
				ispurchase(1);
			}//是
			else if ((mouse_x < 42 && mouse_x>30) && (mouse_y < 64 && mouse_y>57)) {
				buy = 0;
				ispurchase(2);
			}//否 
			else {
				buy = -1;
			}//不在范围内 
		}
	}
	else {
		switch (Event)
		{
		case 1://买地
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
				}//新一轮循环 
				while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
					turn_player++;
					if (turn_player > 3) {
						turn_player %= 4;
					}//新一轮循环 
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
				}//新一轮循环 
				while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
					turn_player++;
					if (turn_player > 3) {
						turn_player %= 4;
					}//新一轮循环 
				}
				DrawPlayerMenu();
			}
			buy = -1;
			break;
		case 2://买房
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
				}//新一轮循环 
				while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
					turn_player++;
					if (turn_player > 3) {
						turn_player %= 4;
					}//新一轮循环 
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
				}//新一轮循环 
				while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
					turn_player++;
					if (turn_player > 3) {
						turn_player %= 4;
					}//新一轮循环 
				}
				DrawPlayerMenu();
			}
			buy = -1;
			break;
		case 5://买车票
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
				}//新一轮循环 
				while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
					turn_player++;
					if (turn_player > 3) {
						turn_player %= 4;
					}//新一轮循环 
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
				}//新一轮循环 
				while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
					turn_player++;
					if (turn_player > 3) {
						turn_player %= 4;
					}//新一轮循环 
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
	初始化地图数据
	cell_kind用于*判断类型 ： 
	0：起点 
	1：土地
	2：机会  
	3：所得税 
	4：火车站  
	5：电力公司与自来水厂  
	6：医院  
	7：监狱 
	8：停车场
**************************************************************/ 

void InitCelldata(){
	int i;

	for(i = 0;i<38;i++){
		cell[i] = (Cell_AllT)malloc(sizeof(struct Cell_All));
	}
	/*第一行：地名，房子类型，拥有者，建造费用，房子数目 
	第二行：过路费(从空地到四座房屋) ，抵押费*/ 
//底面 
	InitCell(cell[0],"起  点",0,-1,0,0,0,0,0,0,0,0);/*顶点*/
	InitCell(cell[1],"  美  国",1,-1,2000,0,
		350,1750,5000,11000,13000,2000);
	InitCell(cell[2],"  机  会",2,-1,0,0,0,0,0,0,0,1);
	InitCell(cell[3],"  加拿大",1,-1,2000,0,
		350,1750,5000,11000,13000,2000);
	InitCell(cell[4],"  财产税",3,-1,0,0,0,0,0,0,0,2);
	InitCell(cell[5],"  火车站",4,-1,2000,
		0,0,0,0,0,0,2);
	InitCell(cell[6],"  阿根廷",1,-1,500,0,
		60,300,900,2700,4000,500);
	InitCell(cell[7],"  机  会",2,-1,0,0,0,0,0,0,0,1);
	InitCell(cell[8],"  墨西哥",1,-1,500,0,
		60,300,900,2700,4000,500);
	InitCell(cell[9],"  古  巴",1,-1,2000,0,
		260,1300,3900,9000,11000,1500);
	InitCell(cell[10],"  法  国",1,-1,2000,0,
		260,1300,3900,9000,11000,2000);
//右面 
	InitCell(cell[11],"监  狱",7,-1,0,0,0,0,0,0,0,2);/*顶点*/ 
	InitCell(cell[12],"电力公司",5,-1,1500,0,
		0,0,0,0,0,3);
	InitCell(cell[13],"德  国",1,-1,1500,0,
		200,1000,3000,7500,9250,1500);
	InitCell(cell[14],"意大利",1,-1,1000,0,
		100,500,1500,4500,6250,700);
	InitCell(cell[15],"火车站",4,-1,2000,
		0,0,0,0,0,0,2);		
	InitCell(cell[16],"西班牙",1,-1,1000,0,
		100,500,1500,4500,6250,700);
	InitCell(cell[17],"机  会",2,-1,0,0,0,0,0,0,0,0);
	InitCell(cell[18],"希  腊",1,-1,1000,0,
		120,600,1800,5000,7000,800);
//上面 
	InitCell(cell[19],"免费停车场",8,-1,0,0,0,0,0,0,0,2);/*顶点*/ 
	InitCell(cell[20],"  荷  兰",1,-1,1500,0,
		200,1000,3000,7500,9250,1200);
	InitCell(cell[21],"  英  国",1,-1,1500,0,
		180,900,2500,7000,8750,1100);
	InitCell(cell[22],"  机  会",2,-1,0,0,0,0,0,0,0,1);	
	InitCell(cell[23],"  俄罗斯",1,-1,500,0,
		60,300,900,2700,4000,500);
	InitCell(cell[24],"  泰  国",1,-1,1000,0,
		140,700,2000,5500,7500,900);
	InitCell(cell[25],"  火车站",4,-1,2000,
		0,0,0,0,0,0,2);
	InitCell(cell[26],"  土耳其",1,-1,2000,0,
		350,1750,5000,11000,13000,1750);
	InitCell(cell[27]," 澳大利亚",1,-1,2000,0,
		280,1500,4500,10000,12000,2000);
	InitCell(cell[28]," 自来水厂",5,-1,1500,0,
		0,0,0,0,0,3);
	InitCell(cell[29],"  新加坡",1,-1,2000,0,
		260,1300,3900,9000,11000,1500);
//左面 
	InitCell(cell[30],"医  院",6,-1,0,0,0,0,0,0,0,2);/*顶点*/ 
	InitCell(cell[31],"韩  国",1,-1,500,0,
		60,300,900,2700,4000,500);
	InitCell(cell[32],"中  国",1,-1,2000,0,
		500,2000,6000,14000,17000,2000);
	InitCell(cell[33],"火车站",4,-1,2000,
		0,0,0,0,0,0,2);
	InitCell(cell[34],"机  会",2,-1,0,0,0,0,0,0,0,0);
	InitCell(cell[35],"日  本",1,-1,500,0,
		60,300,900,2700,4000,500);
	InitCell(cell[36],"财产税",3,-1,0,0,0,0,0,0,0,2);
	InitCell(cell[37],"巴  西",1,-1,1000,0,
		140,700,2000,5500,7500,900);
}

/*初始化地图*/
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

/*画格子*/
static void DrawCell(int choice,int number){
	
	double prex,prey;
	prex = GetCurrentX(); 
	prey = GetCurrentY();
	char house[15];
	SetPointSize(1);//字体大小 

	switch(choice){
		case 1:
		/*画下面的格子*/
			if (cell[number]->owner == -1)
				SetPenColor("khaki");//格子颜色
			else SetPenColor(Player[cell[number]->owner]->color);
			StartFilledRegion(0.6);
			DrawLine(0,-cell_lenth*0.25);
			DrawLine(cell_width,0);
			DrawLine(0,cell_lenth*0.25);
			DrawLine(-cell_width,0);
			EndFilledRegion();//填涂颜色 
			DrawLine(0,-2.5);
			SetPenColor("grey");//字体颜色 
			if (cell[number]->owner != -1 && cell[number]->cell_kind == 1) {
				sprintf(house, "%s*%d", cell[number]->name, cell[number]->house);
				DrawTextString(house);
				SetPenColor(Player[cell[number]->owner]->color);
			}
			else {
				DrawTextString(cell[number]->name);
				SetPenColor("khaki");//格子颜色
			}
			MovePen(prex,prey);
			DrawLine(0,-cell_lenth);
			DrawLine(cell_width,0);
			DrawLine(0,cell_lenth);
			break;
		case 2:
		/*画右侧的格子*/ 
			if (cell[number]->owner == -1)
				SetPenColor("khaki");//格子颜色
			else SetPenColor(Player[cell[number]->owner]->color);
			StartFilledRegion(0.6); 
			DrawLine(0,cell_width);
			DrawLine(cell_lenth*0.25,0);
			DrawLine(0,-cell_width);
			DrawLine(-cell_lenth*0.25,0);
			EndFilledRegion();//填涂颜色 
			DrawLine(cell_lenth,0);
			DrawLine(0,cell_width);
			DrawLine(-cell_lenth,0);
			SetPenColor("grey");//字体颜色 
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
		/*画上面的格子*/ 
			if (cell[number]->owner == -1)
				SetPenColor("khaki");//格子颜色
			else SetPenColor(Player[cell[number]->owner]->color);
			StartFilledRegion(0.6);
			DrawLine(-cell_width,0);
			DrawLine(0,cell_lenth*0.25);
			DrawLine(cell_width,0);
			DrawLine(0,-cell_lenth*0.25);
			EndFilledRegion();//填涂颜色 
			MovePen(prex-cell_width,prey+1);
			SetPenColor("grey");//字体颜色 
			if (cell[number]->owner != -1 && cell[number]->cell_kind == 1)
			{
				sprintf(house, "%s*%d", cell[number]->name, cell[number]->house);
				DrawTextString(house);
				SetPenColor(Player[cell[number]->owner]->color);
			}
			else {
				DrawTextString(cell[number]->name);
				SetPenColor("khaki");//格子颜色
			}
			MovePen(prex,prey);
			DrawLine(0,cell_lenth);
			DrawLine(-cell_width,0);
			DrawLine(0,-cell_lenth);
			break;
		case 4:
		/*画左侧的格子*/ 
			if (cell[number]->owner == -1)
				SetPenColor("khaki");//格子颜色
			else SetPenColor(Player[cell[number]->owner]->color);
			StartFilledRegion(0.6);
			DrawLine(0,-cell_width);
			DrawLine(-cell_lenth*0.25,0);
			DrawLine(0,cell_width);
			DrawLine(cell_lenth*0.25,0);
			EndFilledRegion();//填涂颜色 
			MovePen(prex-4,prey);	
			SetPenColor("grey");//字体颜色 
			if (cell[number]->owner != -1 && cell[number]->cell_kind == 1)
			{
				sprintf(house, "%s  *%d", cell[number]->name, cell[number]->house);
				ErectText(house);
				SetPenColor(Player[cell[number]->owner]->color);
			}
			else
			{
				ErectText(cell[number]->name);
				SetPenColor("khaki");//格子颜色
			}
			MovePen(prex,prey);	
			DrawLine(-cell_lenth,0);
			DrawLine(0,-cell_width);
			DrawLine(cell_lenth,0);
			break;
		case 5:
		/*画顶点*/ 
			SetPenColor("pink");//格子颜色 
			StartFilledRegion(0.6);
			DrawLine(-cell_lenth,0);
			DrawLine(0,-cell_lenth);
			DrawLine(cell_lenth,0);
			DrawLine(0,cell_lenth);
			EndFilledRegion();//填涂颜色
			MovePen(prex-cell_lenth+4,prey-cell_lenth*0.5);	
			SetPenColor("grey");//字体颜色 
			DrawTextString(cell[number]->name);
			MovePen(prex,prey);	
	}
}


/*画地图信息菜单 */
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
	DrawTextString("               地图信息");
	if(nowcell>-1){
		/*起点*/ 
		if(cell[nowcell]->cell_kind==0){
		 	SetPenColor("rred"); 
			MovePen(prex+2,prey-6);
			DrawTextString("                       起点");
			MovePen(prex+2,prey-12);
			DrawTextString("每个人经过起点时，可向");
			MovePen(prex+2,prey-15);
			DrawTextString("银行领取2000元.停留起点");
			MovePen(prex + 2, prey - 18);
			DrawTextString("时则无法领取，但可获得");
			MovePen(prex + 2, prey - 21);
			DrawTextString("额外一回合。");
		} 
		/*土地*/ 
		else if(cell[nowcell]->cell_kind==1){
			SetPenColor("deepblue"); 
			MovePen(prex+2,prey-6);
			DrawTextString("当前位置：");	
			DrawTextString(cell[nowcell]->name);	
			MovePen(prex+2,prey-9);
			DrawTextString("拥有者：");
			if(cell[nowcell]->owner!=-1){
				DrawTextString(Player[cell[nowcell]->owner]->name);
			}//如果存在 
			else{
				DrawTextString("无人购买");
			}
			MovePen(prex+2,prey-12);
			DrawTextString("房子个数：");
			numtostring(cell[nowcell]->house,numstring);
			DrawTextString(numstring);					
			MovePen(prex+2,prey-15);
			DrawTextString("过路费-空地：");
			numtostring(cell[nowcell]->tolls[0],numstring);
			DrawTextString(numstring);
			MovePen(prex+2,prey-18);
			DrawTextString("过路费-一幢房屋：");
			numtostring(cell[nowcell]->tolls[1],numstring);
			DrawTextString(numstring);
			MovePen(prex+2,prey-21);
			DrawTextString("过路费-两幢房屋：");
			numtostring(cell[nowcell]->tolls[2],numstring);
			DrawTextString(numstring);
			MovePen(prex+2,prey-24);
			DrawTextString("过路费-三幢房屋：");
			numtostring(cell[nowcell]->tolls[3],numstring);
			DrawTextString(numstring);
			MovePen(prex+2,prey-27);
			DrawTextString("过路费-四幢房屋：");
			numtostring(cell[nowcell]->tolls[4],numstring);
			DrawTextString(numstring);
			MovePen(prex+2,prey-30);
			DrawTextString("房屋建筑费：");
			numtostring(cell[nowcell]->construt_fee,numstring);
			DrawTextString(numstring);
			MovePen(prex+2,prey-33);
			DrawTextString("空地抵押费：");
			numtostring(cell[nowcell]->mortgage_fee,numstring);
			DrawTextString(numstring);
		}
		/*机会*/ 
		else if(cell[nowcell]->cell_kind==2){
			SetPenColor("deepblue"); 
			MovePen(prex+2,prey-6);
			DrawTextString("机会：");
			SetPenColor("rred"); 
			MovePen(prex+2,prey-9);
			DrawTextString("走到命运格的玩家将随");
			MovePen(prex+2,prey-12);
			DrawTextString("机抽取一张命运卡，依");
			MovePen(prex + 2, prey - 15);
			DrawTextString("照上面的叙述执行");
		}
		/*所得税*/ 
		else if(cell[nowcell]->cell_kind==3){
			SetPenColor("deepblue"); 
			MovePen(prex+2,prey-6);
			DrawTextString("所得税：");
			SetPenColor("rred");
			MovePen(prex+2,prey-9);
			DrawTextString("缴交10%现金给银行");
		}
		/*火车站*/ 
		else if(cell[nowcell]->cell_kind==4){
			SetPenColor("deepblue"); 
			MovePen(prex+2,prey-6);
			DrawTextString(cell[nowcell]->name);
			MovePen(prex+2,prey-9);
			DrawTextString("购置费用： ");
			numtostring(cell[nowcell]->construt_fee,numstring);
			DrawTextString(numstring);
			MovePen(prex+2,prey-12);
			DrawTextString("拥有者： ");
			if(cell[nowcell]->owner!=-1){
				DrawTextString(Player[cell[nowcell]->owner]->name);
			}//如果存在 
			else{
				DrawTextString("无人购买");
			}
			SetPenColor("rred"); 
			MovePen(prex+2,prey-16);
			DrawTextString("玩家路过将可以选择是否");
			MovePen(prex+2,prey-19);
			DrawTextString("花费500元去相邻车站");
		}
		/*自来水公司与电力公司*/ 
		else if(cell[nowcell]->cell_kind==5){
			SetPenColor("deepblue"); 
			MovePen(prex+2,prey-6);
			DrawTextString(cell[nowcell]->name);
			MovePen(prex+2,prey-9);
			DrawTextString("购置费用： ");
			numtostring(cell[nowcell]->construt_fee,numstring);
			DrawTextString(numstring);
			MovePen(prex+2,prey-12);
			DrawTextString("拥有者： ");
			if(cell[nowcell]->owner!=-1){
				DrawTextString(Player[cell[nowcell]->owner]->name);
			}//如果存在 
			else{
				DrawTextString("无人购买");
			}
			
			SetPenColor("rred");
			MovePen(prex+2,prey-16);
			DrawTextString("其他玩家路过将付出500元");
			MovePen(prex+2,prey-19);
			DrawTextString("电费(若拥有电力公司)和");
			MovePen(prex+2,prey-22);
			DrawTextString("500元水费(若拥有自来水");
			MovePen(prex + 2, prey - 25);
			DrawTextString("公司)给拥有者");
		}
		/*医院*/ 
		else if(cell[nowcell]->cell_kind==6){
			SetPenColor("deepblue"); 
			MovePen(prex+2,prey-6);
			DrawTextString(cell[nowcell]->name);
			MovePen(prex+2,prey-9);
			SetPenColor("rred"); 
			DrawTextString("除非抽到命运卡片进入医院，");
			MovePen(prex+2,prey-12);
			DrawTextString("否则经过或停留在医院格不会");
			MovePen(prex+2,prey-15);
			DrawTextString("发生任何影响");		
		}
		/*监狱*/ 
		else if(cell[nowcell]->cell_kind==7){
			SetPenColor("deepblue"); 
			MovePen(prex+2,prey-6);
			DrawTextString(cell[nowcell]->name);
			MovePen(prex+2,prey-9);
			SetPenColor("rred"); 
			DrawTextString("除非抽到命运卡片进入监狱，");
			MovePen(prex+2,prey-12);
			DrawTextString("否则经过或停留在医院格不会");
			MovePen(prex+2,prey-15);
			DrawTextString("发生任何影响");		
		}
		/*免费停车场*/ 
		else if(cell[nowcell]->cell_kind==8){
			SetPenColor("deepblue"); 
			MovePen(prex+2,prey-6);
			DrawTextString(cell[nowcell]->name);
			MovePen(prex+2,prey-9);
			SetPenColor("rred"); 
			DrawTextString("走到停车场，无事发生");		
		}
	}
}

/*画玩家信息菜单*/ 
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
	DrawTextString("               玩家信息");
	SetPenColor("deepblue");
	
	SetPointSize(10);
	for(i=0;i<4;i++)
	{
		if(Player[i]->mode!=0){
			MovePen(prex+2,prey-6-9*i);
			DrawTextString("玩家昵称：");
			DrawTextString(Player[i]->name);
			if (Player[i]->mode == 2)
				DrawTextString("  -AI");
			MovePen(prex+2,prey-9-9*i);
			if(Player[i]->isDefeated==TRUE){
	
				DrawTextString("玩家已被淘汰");
			}
			else{
				numtostring(Player[i]->money,numstring);
				DrawTextString("玩家金钱：");
				DrawTextString(numstring);	
			}
		}
	}
} 

/*游戏进行*/
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
//		sprintf(a,"%s 成为大富翁！游戏结束！",Player[turn_player]->name);
		InitGraphics();
		game_over = TRUE; 
		degree=0;
		spiral(Player[turn_player]->name);
		startTimer(0,75);
		
		return;//游戏结束 
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
			}//新一轮循环 
			while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
				turn_player++;
				if (turn_player > 3) {
					turn_player %= 4;
				}//新一轮循环 
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
			}//新一轮循环 
			while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
				turn_player++;
				if (turn_player > 3) {
					turn_player %= 4;
				}//新一轮循环 
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
	}//经过起点不停留领取2000元 
		
	nowcell = Player[turn_player]->place;
	if(nowcell>-1){
		DrawCellMenu();	
	} //更新当前游戏位置信息菜单 

	DrawMap1(2);//画地图 

	switch(cell[Player[turn_player]->place]->cell_kind){
		case 1://土地 
			if(cell[Player[turn_player]->place]->owner==-1){
					if(Player[turn_player]->money - cell[Player[turn_player]->place]->construt_fee<0){
						//钱不够继续购买
						turn_player++;
						if (turn_player > 3) {
							turn_player %= 4;
						}//新一轮循环 
						while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
							turn_player++;
							if (turn_player > 3) {
								turn_player %= 4;
							}//新一轮循环 
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
							DrawMap1(2);//画地图 
							Event = 3;
							ispurchase(0);
							}
							
							turn_player++;
							if (turn_player > 3) {
								turn_player %= 4;
							}//新一轮循环 
							while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//新一轮循环 
							}
							DrawPlayerMenu();
						}
						else
						{
							Event = 1;
							ispurchase(0);
						}
					}
			}//空地 
			else if(cell[Player[turn_player]->place]->owner==turn_player){//自己的地
				if(cell[Player[turn_player]->place]->house<4){
						if(Player[turn_player]->money - cell[Player[turn_player]->place]->construt_fee<0){
							//钱不够继续购买 
							turn_player++;
							if (turn_player > 3) {
								turn_player %= 4;
							}//新一轮循环 
							while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//新一轮循环 
							}
							DrawPlayerMenu();
						}
						else{
							if (Player[turn_player]->mode == 2)
							{
								if(Player[turn_player]->money - cell[Player[turn_player]->place]->construt_fee>2000){
									cell[Player[turn_player]->place]->house++;
									Player[turn_player]->money -= cell[Player[turn_player]->place]->construt_fee;
									DrawMap1(2);//画地图 
									Event = 4;
									ispurchase(0);
								}
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//新一轮循环 
								while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
									turn_player++;
									if (turn_player > 3) {
										turn_player %= 4;
									}//新一轮循环 
								}
								DrawPlayerMenu();
							}
							else
							{
								Event = 2;
								ispurchase(0);
							}
						}//继续建造 
				}//可以继续建造 
			}//自己的土地 
			else{//不是自己的地
				if (Player[cell[Player[turn_player]->place]->owner]->wait > 0 && Player[cell[Player[turn_player]->place]->owner]->place == 11)//地主在狱里
				{
					Event = 15;
					ispurchase(0);
					turn_player++;
					if (turn_player > 3) {
						turn_player %= 4;
					}//新一轮循环 
					while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
						turn_player++;
						if (turn_player > 3) {
							turn_player %= 4;
						}//新一轮循环 
					}
				}
				else if(Player[turn_player]->money - cell[Player[turn_player]->place]->tolls[cell[Player[turn_player]->place]->house]<0){//钱不够
						sellhouse(turn_player); //卖房子
						if(Player[turn_player]->money - cell[Player[turn_player]->place]->tolls[cell[Player[turn_player]->place]->house]<0){//卖房也不够
							sellland(turn_player,Player[turn_player]->money - cell[Player[turn_player]->place]->tolls[cell[Player[turn_player]->place]->house]); //卖地
							if (Player[turn_player]->money - cell[Player[turn_player]->place]->tolls[cell[Player[turn_player]->place]->house] < 0) {//卖地也不够
								Player[turn_player]->isDefeated = TRUE;//玩家淘汰
								all_player--;
								DrawMap1(2);//画地图 
								Event = 8;
								ispurchase(0);
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//新一轮循环 
								while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
									turn_player++;
									if (turn_player > 3) {
										turn_player %= 4;
									}//新一轮循环 
								}
							}
							else {//卖地够了
								Player[turn_player]->money -= cell[Player[turn_player]->place]->tolls[cell[Player[turn_player]->place]->house];
								Player[cell[Player[turn_player]->place]->owner]->money += cell[Player[turn_player]->place]->tolls[cell[Player[turn_player]->place]->house];
								DrawMap1(2);//画地图 
								Event = 17;
								ispurchase(0);
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//新一轮循环 
								while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
									turn_player++;
									if (turn_player > 3) {
										turn_player %= 4;
									}//新一轮循环 
								}
							}
						}//卖地
						else//卖房够了
						{
							Player[turn_player]->money -= cell[Player[turn_player]->place]->tolls[cell[Player[turn_player]->place]->house];
							Player[cell[Player[turn_player]->place]->owner]->money += cell[Player[turn_player]->place]->tolls[cell[Player[turn_player]->place]->house];
							DrawMap1(2);//画地图 
							Event = 16;
							ispurchase(0);
						}
				}
				else{//钱够
					Player[turn_player]->money -= cell[Player[turn_player]->place]->tolls[cell[Player[turn_player]->place]->house];
					Player[cell[Player[turn_player]->place]->owner]->money += cell[Player[turn_player]->place]->tolls[cell[Player[turn_player]->place]->house];
					DrawMap1(2);//画地图 
					Event = 7;
					ispurchase(0);
					turn_player++;
					if (turn_player > 3) {
						turn_player %= 4;
					}//新一轮循环 
					while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
						turn_player++;
						if (turn_player > 3) {
							turn_player %= 4;
						}//新一轮循环 
					}
				}//自己扣钱，拥有者加钱 
				DrawPlayerMenu();
			}//别人的地 
			break;
		case 2:
			destiny(turn_player);
			turn_player++;
			if (turn_player > 3) {
				turn_player %= 4;
			}//新一轮循环 
			while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
				turn_player++;
				if (turn_player > 3) {
					turn_player %= 4;
				}//新一轮循环 
			}
			DrawPlayerMenu();
			break; //命运 
		case 3:
			Player[turn_player]->money -= Player[turn_player]->money/10;
			Event = 7;
			ispurchase(0);
			turn_player++;
			if (turn_player > 3) {
				turn_player %= 4;
			}//新一轮循环 
			while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
				turn_player++;
				if (turn_player > 3) {
					turn_player %= 4;
				}//新一轮循环 
			}
			DrawPlayerMenu();
			break;//所得税 扣除10%现金 
		case 4:
			if(cell[Player[turn_player]->place]->owner==-1){
					if(Player[turn_player]->money - cell[Player[turn_player]->place]->construt_fee<0){
						//钱不够 
						turn_player++;
						if (turn_player > 3) {
							turn_player %= 4;
						}//新一轮循环 
						while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
							turn_player++;
							if (turn_player > 3) {
								turn_player %= 4;
							}//新一轮循环 
						}
						DrawPlayerMenu();
					}
					else{
						if (Player[turn_player]->mode == 2)
						{
							if(Player[turn_player]->money - cell[Player[turn_player]->place]->construt_fee>2000){
								Player[turn_player]->money -= cell[Player[turn_player]->place]->construt_fee;
								cell[Player[turn_player]->place]->owner = turn_player;
								DrawMap1(2);//画地图 
								Event = 3;
								ispurchase(0);
							}
							turn_player++;
							if (turn_player > 3) {
								turn_player %= 4;
							}//新一轮循环 
							while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//新一轮循环 
							}
							DrawPlayerMenu();
						}
						else
						{
							Event = 1;
							ispurchase(0);
						}
					}
			}//空地 
			else{
				if (Player[turn_player]->money - 500 < 0)
				{
					turn_player++;
					if (turn_player > 3) {
						turn_player %= 4;
					}//新一轮循环 
					while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
						turn_player++;
						if (turn_player > 3) {
							turn_player %= 4;
						}//新一轮循环 
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
							DrawMap1(2);//画地图
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
						}//新一轮循环 
						while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
							turn_player++;
							if (turn_player > 3) {
								turn_player %= 4;
							}//新一轮循环 
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
			break;//火车站
		case 5:
			if(cell[Player[turn_player]->place]->owner==-1){
				if(1){
					if(Player[turn_player]->money - cell[Player[turn_player]->place]->construt_fee<0){
						//钱不够 
						turn_player++;
						if (turn_player > 3) {
							turn_player %= 4;
						}//新一轮循环 
						while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
							turn_player++;
							if (turn_player > 3) {
								turn_player %= 4;
							}//新一轮循环 
						}
						DrawPlayerMenu();
					}
					else{
						if (Player[turn_player]->mode == 2)
						{
							if(Player[turn_player]->money - cell[Player[turn_player]->place]->construt_fee>2000){
								Player[turn_player]->money -= cell[Player[turn_player]->place]->construt_fee;
								cell[Player[turn_player]->place]->owner = turn_player;
								DrawMap1(2);//画地图 
								Event = 3;
								ispurchase(0);
							}
							turn_player++;
							if (turn_player > 3) {
								turn_player %= 4;
							}//新一轮循环 
							while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//新一轮循环 
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
			}//空地 
			else if (cell[Player[turn_player]->place]->owner == turn_player)//是自己的
			{
				turn_player++;
				if (turn_player > 3) {
					turn_player %= 4;
				}//新一轮循环 
				while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
					turn_player++;
					if (turn_player > 3) {
						turn_player %= 4;
					}//新一轮循环 
				}
				DrawPlayerMenu();
			}
			else{//-500
				if (Player[cell[Player[turn_player]->place]->owner]->wait > 0 && Player[cell[Player[turn_player]->place]->owner]->place == 11)//地主在狱里
				{
					Event = 15;
					ispurchase(0);
					turn_player++;
					if (turn_player > 3) {
						turn_player %= 4;
					}//新一轮循环 
					while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
						turn_player++;
						if (turn_player > 3) {
							turn_player %= 4;
						}//新一轮循环 
					}
				}
				else if(cell[12]->owner!=cell[28]->owner)
				{
					if (Player[turn_player]->money - 500 < 0) {//钱不够
						sellhouse(turn_player); //卖房子
						if (Player[turn_player]->money - 500 < 0) {//卖房子也不够
							sellland(turn_player, Player[turn_player]->money - 500); //卖地
							if (Player[turn_player]->money - 500 < 0) {//卖地也不够
								Player[turn_player]->isDefeated = TRUE;//玩家淘汰 
								all_player--;
								DrawMap1(2);//画地图 
								Event = 8;
								ispurchase(0);
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//新一轮循环 
								while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
									turn_player++;
									if (turn_player > 3) {
										turn_player %= 4;
									}//新一轮循环 
								}
							}
							else {//卖地够了
								Player[turn_player]->money -= 500;
								Player[cell[Player[turn_player]->place]->owner]->money += 500;
								DrawMap1(2);//画地图 
								Event = 17;
								ispurchase(0);
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//新一轮循环 
								while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
									turn_player++;
									if (turn_player > 3) {
										turn_player %= 4;
									}//新一轮循环 
								}
							}
						}
						else//卖房够了
						{
							Player[turn_player]->money -= 500;
							Player[cell[Player[turn_player]->place]->owner]->money += 500;
							DrawMap1(2);//画地图 
							Event = 16;
							ispurchase(0);
							turn_player++;
							if (turn_player > 3) {
								turn_player %= 4;
							}//新一轮循环 
							while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//新一轮循环 
							}
						}
					}
					else//钱够
					{
						Player[turn_player]->money -= 500;
						Player[cell[Player[turn_player]->place]->owner]->money += 500;
						DrawMap1(2);//画地图 
						Event = 7;
						ispurchase(0);
						turn_player++;
						if (turn_player > 3) {
							turn_player %= 4;
						}//新一轮循环 
						while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
							turn_player++;
							if (turn_player > 3) {
								turn_player %= 4;
							}//新一轮循环 
						}
					}
				}
				else{//-1000
					if (Player[turn_player]->money - 1000 < 0) {//钱不够
						sellhouse(turn_player); //卖房子
						if (Player[turn_player]->money - 1000 < 0) {//卖房子也不够
							sellland(turn_player, Player[turn_player]->money - 1000); //卖地
							if (Player[turn_player]->money - 1000 < 0) {//卖地也不够
								Player[turn_player]->isDefeated = TRUE;//玩家淘汰 
								all_player--;
								DrawMap1(2);//画地图 
								Event = 8;
								ispurchase(0);
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//新一轮循环 
								while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
									turn_player++;
									if (turn_player > 3) {
										turn_player %= 4;
									}//新一轮循环 
								}
							}
							else {//卖地够了
								Player[turn_player]->money -= 1000;
								Player[cell[Player[turn_player]->place]->owner]->money += 1000;
								DrawMap1(2);//画地图 
								Event = 17;
								ispurchase(0);
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//新一轮循环 
								while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
									turn_player++;
									if (turn_player > 3) {
										turn_player %= 4;
									}//新一轮循环 
								}
							}
						}
						else//卖房够了
						{
							Player[turn_player]->money -= 1000;
							Player[cell[Player[turn_player]->place]->owner]->money += 1000;
							DrawMap1(2);//画地图 
							Event = 16;
							ispurchase(0);
							turn_player++;
							if (turn_player > 3) {
								turn_player %= 4;
							}//新一轮循环 
							while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
								turn_player++;
								if (turn_player > 3) {
									turn_player %= 4;
								}//新一轮循环 
							}
						}
					}
					else//钱够
					{
						Player[turn_player]->money -= 1000;
						Player[cell[Player[turn_player]->place]->owner]->money += 1000;
						DrawMap1(2);//画地图 
						Event = 7;
						ispurchase(0);
						turn_player++;
						if (turn_player > 3) {
							turn_player %= 4;
						}//新一轮循环 
						while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
							turn_player++;
							if (turn_player > 3) {
								turn_player %= 4;
							}//新一轮循环 
						}
					}
				}
				DrawPlayerMenu();
			}
			break;//电力公司与自来水厂
		case 6:
			turn_player++;
			if (turn_player > 3) {
				turn_player %= 4;
			}//新一轮循环 
			while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
				turn_player++;
				if (turn_player > 3) {
					turn_player %= 4;
				}//新一轮循环 
			}
			DrawPlayerMenu();
			break;//医院
		case 7:
			turn_player++;
			if (turn_player > 3) {
				turn_player %= 4;
			}//新一轮循环 
			while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
				turn_player++;
				if (turn_player > 3) {
					turn_player %= 4;
				}//新一轮循环 
			}
			DrawPlayerMenu();
			break;//监狱 
		case 8:
			turn_player++;
			if (turn_player > 3) {
				turn_player %= 4;
			}//新一轮循环 
			while (Player[turn_player]->mode == 0 || Player[turn_player]->isDefeated == TRUE) {
				turn_player++;
				if (turn_player > 3) {
					turn_player %= 4;
				}//新一轮循环 
			}
			DrawPlayerMenu();
			break;//停车场					
	}
}

/*命运抽奖*/
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
	DrawTextString("           命运");
	
	switch(numdest){
		case 0:
			SetPenColor("rred");  
			MovePen(start_map1x + 75, start_map1y + 60);
			DrawTextString("    无主之物啊");
			MovePen(start_map1x + 75, start_map1y + 57);
			DrawTextString("遵循梦之魔杖的指引");
			MovePen(start_map1x + 75, start_map1y + 54);
			DrawTextString("  化作吾之力量吧");
			break;
		case 1:
			SetPenColor("rred");  
			MovePen(start_map1x + 70, start_map1y + 60);
			DrawTextString(" 在阿拉伯联合大公国不小心");
			MovePen(start_map1x + 70, start_map1y + 57);
			DrawTextString("打翻珍贵的饮用水");
			MovePen(start_map1x + 70, start_map1y + 54);
			DrawTextString(" 赔偿500元");
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
			DrawTextString("在非洲阻止猎人盗猎");
			MovePen(start_map1x + 70, start_map1y + 57);
			DrawTextString("保育类的动物们");
			MovePen(start_map1x + 70, start_map1y + 54);
			DrawTextString("奖金800元");
			if(playerid<0) break;
			Player[playerid]->money+=800;
			Event = 10;
			ispurchase(0);
			break;
		case 3:
			SetPenColor("rred");  
			MovePen(start_map1x + 70, start_map1y + 60);
			DrawTextString("在新加坡观光时乱丢垃圾");
			MovePen(start_map1x + 70, start_map1y + 57);
			DrawTextString("罚款400元");
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
			DrawTextString("到南极大陆探险看到美丽的极光");
			MovePen(start_map1x + 75, start_map1y + 57);
			DrawTextString("花费700元");
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
			DrawTextString("搭乘“欧洲之星”");
			MovePen(start_map1x + 70, start_map1y + 57);
			DrawTextString("直接移动到英国");
			if(playerid<0) break;
			Player[playerid]->place = 21;
			break;
		case 6:
			SetPenColor("rred");  
			MovePen(start_map1x + 70, start_map1y + 60);
			DrawTextString("在泰国听到泰国国歌没有");
			MovePen(start_map1x + 70, start_map1y + 57);
			DrawTextString("马上肃立");
			MovePen(start_map1x + 70, start_map1y + 54);
			DrawTextString("罚款600元");
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
			DrawTextString("在极圈的冰岛发现失落的");
			MovePen(start_map1x + 70, start_map1y + 57);
			DrawTextString("“冰岛萨迦”");
			MovePen(start_map1x + 70, start_map1y + 54);
			DrawTextString("奖金700元");
			if(playerid<0) break;
			Player[playerid]->money+=700;
			Event = 10;
			ispurchase(0);
			break;
		case 8:
			SetPenColor("rred");  
			MovePen(start_map1x+70,start_map1y+60);
			DrawTextString("在日本京都赏樱订不到");
			MovePen(start_map1x + 70, start_map1y + 57);
			DrawTextString("旅馆露宿街头");
			MovePen(start_map1x + 70, start_map1y + 54);
			DrawTextString("安慰奖200元");
			if(playerid<0) break;
			Player[playerid]->money+=200;
			Event = 10;
			ispurchase(0);
			break;
		case 9:
			SetPenColor("rred");  
			MovePen(start_map1x+70,start_map1y+60);
			DrawTextString("到梵蒂冈研究天主教历史");
			MovePen(start_map1x + 70, start_map1y + 57);
			DrawTextString("奖金1000元");
			if(playerid<0) break;
			Player[playerid]->money+=1000;
			Event = 10;
			ispurchase(0);
			break;
		case 10:
			SetPenColor("rred");  
			MovePen(start_map1x+70,start_map1y+60);
			DrawTextString("在威尼斯街头与街头艺人");
			MovePen(start_map1x + 70, start_map1y + 57);
			DrawTextString("一起表演");
			MovePen(start_map1x + 70, start_map1y + 54);
			DrawTextString("奖金600元");
			if(playerid<0) break;
			Player[playerid]->money+=600;
			Event = 10;
			ispurchase(0);
			break;
		case 11:
			SetPenColor("rred");  
			MovePen(start_map1x+70,start_map1y+60);
			DrawTextString("在南美洲投资的咖啡树品");
			MovePen(start_map1x + 70, start_map1y + 57);
			DrawTextString("质优良销量大增");
			MovePen(start_map1x + 70, start_map1y + 54);
			DrawTextString("奖金2000元");
			if(playerid<0) break;
			Player[playerid]->money+=2000;
			Event = 10;
			ispurchase(0);
			break;
		case 12:
			SetPenColor("rred");  
			MovePen(start_map1x+70,start_map1y+60);
			DrawTextString("到澳洲打工度假");
			MovePen(start_map1x+70,start_map1y+57);
			DrawTextString("奖金900元");
			if(playerid<0) break;
			Player[playerid]->money+=900;
			Event = 10;
			ispurchase(0);
			break;
		case 13:
			SetPenColor("rred");  
			MovePen(start_map1x+70,start_map1y+60);
			DrawTextString("拯救了迷路的北极熊宝宝");
			MovePen(start_map1x+70,start_map1y+57);
			DrawTextString("奖金800元");
			if(playerid<0) break;
			Player[playerid]->money+=800;
			Event = 10;
			ispurchase(0);
			break;
		case 14:
			SetPenColor("rred");
			MovePen(start_map1x + 70, start_map1y + 60);
			DrawTextString("用非法手段敛财被告发");
			MovePen(start_map1x + 70, start_map1y + 57);
			DrawTextString("入狱，暂停三回合");
			if (playerid < 0) break;
			Player[playerid]->place = 11;
			Player[playerid]->wait = 3;
			Event = 11;
			ispurchase(0);
			break;
		case 15:
			SetPenColor("rred");
			MovePen(start_map1x + 70, start_map1y + 60);
			DrawTextString("人在家中坐，锅从天上来");
			MovePen(start_map1x + 70, start_map1y + 57);
			DrawTextString("被砸伤住院，暂停两回合");
			if (playerid < 0) break;
			Player[playerid]->place = 30;
			Player[playerid]->wait = 2;
			Event = 13;
			ispurchase(0);
			break;
	}
} 

/*购买与事件界面*/ 
void ispurchase(int isdown){//Event:1-购买土地；2-购买房屋；3-购买了土地；4-购买了房屋；5-坐车；6-坐了车；7-交钱；8-破产；9-放弃购买

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
			sprintf(text, "    是否购买 %s ?", cell[Player[turn_player]->place]->name);
			DrawTextString(text);
			break;
		case 2:
			sprintf(text, "    是否在 %s 购买房屋?", cell[Player[turn_player]->place]->name);
			DrawTextString(text);
			break;
		case 3:
			DrawTextString("        暴 富 大 事 件");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "    %s 购买了 %s", Player[turn_player]->name, cell[Player[turn_player]->place]->name);
			DrawTextString(text);
			break;
		case 4:
			DrawTextString("        暴 富 大 事 件");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "    %s 在 %s 购买了房屋", Player[turn_player]->name, cell[Player[turn_player]->place]->name);
			DrawTextString(text);
			break;
		case 5:
			DrawTextString("        是否购买车票？");
			break;
		case 6:
			DrawTextString("        暴 富 大 事 件");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "        %s 乘坐了火车", Player[turn_player]->name);
			DrawTextString(text);
			if (cell[Player[turn_player]->place]->owner != turn_player)
			{
				MovePen(prex + 5, prey - 12);
				sprintf(text, "        %s 飞来横财", Player[cell[Player[turn_player]->place]->owner]->name);
				DrawTextString(text);
			}
			break;
		case 7:
			DrawTextString("        暴 富 大 事 件");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "        %s 的腰包又瘪了", Player[turn_player]->name);
			DrawTextString(text);
			if (cell[Player[turn_player]->place]->owner != -1)
			{
				MovePen(prex + 5, prey - 12);
				sprintf(text, "        %s 飞来横财", Player[cell[Player[turn_player]->place]->owner]->name);
				DrawTextString(text);
			}
			break;
		case 8:
			DrawTextString("        暴 富 大 事 件");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "        %s 破产了！", Player[turn_player]->name);
			DrawTextString(text);
			break;
		case 9:
			DrawTextString("        暴 富 大 事 件");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "        %s 放弃了购买", Player[turn_player]->name);
			DrawTextString(text);
			break;
		case 10:
			DrawTextString("        暴 富 大 事 件");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "        %s 飞来横财", Player[turn_player]->name);
			DrawTextString(text);
			break;
		case 11:
			DrawTextString("        暴 富 大 事 件");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "        %s 锒铛入狱", Player[turn_player]->name);
			DrawTextString(text);
			break;
		case 12:
			DrawTextString("        暴 富 大 事 件");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "        %s 在狱中痛改前非", Player[turn_player]->name);
			DrawTextString(text);
			MovePen(prex + 5, prey - 12);
			sprintf(text, "        离出狱还有 %d 回合", Player[turn_player]->wait);
			DrawTextString(text);
			break;
		case 13:
			DrawTextString("        暴 富 大 事 件");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "        %s 住院了", Player[turn_player]->name);
			DrawTextString(text);
			break;
		case 14:
			DrawTextString("        暴 富 大 事 件");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "        %s 缴纳住院费500", Player[turn_player]->name);
			DrawTextString(text);
			MovePen(prex + 5, prey - 12);
			sprintf(text, "        离出院还有 %d 回合", Player[turn_player]->wait);
			DrawTextString(text);
			break;
		case 15:
			DrawTextString("        暴 富 大 事 件");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "        %s 尚在狱中", Player[cell[Player[turn_player]->place]->owner]->name);
			DrawTextString(text);
			MovePen(prex + 5, prey - 12);
			sprintf(text, "        %s 躲过一劫", Player[turn_player]->name);
			DrawTextString(text);
			break;
		case 16:
			DrawTextString("        暴 富 大 事 件");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "        %s 砸锅卖铁终于还了债", Player[turn_player]->name);
			DrawTextString(text);
			break;
		case 17:
			DrawTextString("        暴 富 大 事 件");
			MovePen(prex + 5, prey - 9);
			sprintf(text, "        %s 倾家荡产终于还了债", Player[turn_player]->name);
			DrawTextString(text);
			break;
	}
	
	if (Event == 1 || Event == 2 || Event == 5)
	{//鼠标没有操作 
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
			DrawTextString("     是");


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
			DrawTextString("     否");
		}
		//按下是 
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
			DrawTextString("     是");

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
			DrawTextString("     否");
		}
		//按下否 
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
			DrawTextString("     是");

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
			DrawTextString("     否");
		}
	}
	else Event = 0;
}

/*从起点开始循环卖土地，直至钱够*/ 
void sellland(int playerid,int difference){
	if(difference<0)  difference = -difference;
	int i=0;
	for(i = 0;i<38;i++){
		if(cell[i]->owner==playerid){
			Player[playerid]->money += cell[i]->mortgage_fee;//抵押价格出售 
			cell[i]->owner = -1;//土地收回			 
		}
	}
}

/*卖掉玩家所有房子，半价出售*/ 
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

/*把数字转换成字符串*/
void numtostring(int num,char* numstring){
	int digit = 0;
	if(num<0){
		numstring[0] = '-';
		num = -num; 
		digit++;
		return;
	}//是负数 
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

/*竖着写文本，然后画笔回到初始位置*/ 
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
