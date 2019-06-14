/***********************************************************
	morecolor.c 

	函数说明: 定义了更多的颜色 
	调用接口:void MoreColor(); 
	创建时间 :2019/5/10  20:15
	Copyright ? 2019  Group_5
*************************************************************/
#include "extgraph.h"

#include"morecolor.h"

//=========================================================================================================================================================================================================

void MoreColor()
{	
	
	DefineColor("grey",0.325,0.322,0.325) ;/*深灰色*/
    DefineColor("lightgrey",0.325,0.322,0.325) ;/*更温柔点的灰色*/
	DefineColor("deepblue",0,0.208,0.416);/*深蓝*/
	DefineColor("navyblue",0,0.310,0.639);/*海军蓝*/  
	DefineColor("deepgreen",0,0.443,0.188);/*盛夏的树叶*/ 
	DefineColor("grass",0.561,0.765,0.122);/*草绿*/ 
	DefineColor("gingeryellow",1,0.757,0.016);/*姜黄色*/
	DefineColor("orange",0.887,0.388,0.016);/*一颗大橘子*/ 
	DefineColor("rred",0.643,0,0);/*复古的红色*/ 
	DefineColor("pink",1,0.588,0.584);/*粉色*/ 
	DefineColor("purple",0.537,0.341,0.631);/*葡萄紫*/ 

	/*下面是一些背景色，做字体太浅了看不清*/ 
	DefineColor("smoke",0.933,0.933,0.933) ;/*万能百搭浅灰色*/
	DefineColor("khaki",0.953,0.910,0.729) ;/*浅卡其色*/
	DefineColor("babyblue",0.737,0.839,0.914) ;/*baby蓝*/
	DefineColor("babypink",1,0.816,0.813) ;/*奶油粉*/
	DefineColor("matcha",0.8,0.882,0.596) ;/*抹茶拿铁*/
	DefineColor("VC",0.980,0.804,0.537) ;/*橙子味的气泡水*/ 
	DefineColor("lavender",0.862,0.812,0.883) ;/*薰衣草*/
}

