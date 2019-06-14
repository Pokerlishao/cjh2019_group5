#pragma once
#ifndef _Map1_demo_h
#define _Map1_demo_h

typedef struct Cell_All {
	string name;//���� 
	int cell_kind;/*�ж�����  0Ϊ��� 1Ϊ����  2Ϊ����  3Ϊ����˰  
				  4Ϊ��վ  5Ϊ������˾������ˮ��  6ΪҽԺ  
				  7Ϊ���� 8Ϊͣ����*/
	int owner;//ӵ���� ,ֵΪ-1ʱ�������� 
	int construt_fee;//������ 
	int house;//���Ӹ��� ������Ϊ4 
	int tolls[5];//���ݵ�ǰ���Ӹ�����ȡ��·�� 
	int mortgage_fee;//��Ѻ�� 

}*Cell_AllT;

/*
--------------------------------------------------------------------------------------------------
void MainMap1();

����˵��������Ϸ ��ʼ��Map1
--------------------------------------------------------------------------------------------------
*/
void MainMap1();//Map1������ 

/*
--------------------------------------------------------------------------------------------------
void DrawMap1();

����˵�������� ��Map1
--------------------------------------------------------------------------------------------------
*/
void DrawMap1(int);

/*
--------------------------------------------------------------------------------------------------
int Mouse_Map1(int x, int y, int button, int event);

����˵�������ص������е��ô˺�����ִ�����ͼ�йص������Ϊ
����˵����ͬ���ص�����
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
--------------------------------------------------------------------------------------------------
*/
int Mouse_Map1(int x, int y, int button, int event);

/*
--------------------------------------------------------------------------------------------------
int KeyBoard_Map1(int key, int event);

����˵�������̻ص������е��ô˺�����ִ����Map1��صļ�����Ϊ
����˵����ͬ���̻ص�����
����ֵ��ͬ�˵���
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
--------------------------------------------------------------------------------------------------
*/
int KeyBoard_Map1(int key, int event);

/*
--------------------------------------------------------------------------------------------------
void Timer_Map1();

����˵������ʱ���ص������е��ô˺�����תת��
--------------------------------------------------------------------------------------------------
*/
void Timer_Map1();

/*
--------------------------------------------------------------------------------------------------
void InitCelldata();

����˵������ʼ����ͼ����
--------------------------------------------------------------------------------------------------
*/
void InitCelldata();//��ʼ����ͼ���� 

/*
--------------------------------------------------------------------------------------------------
void begingame();

�������ܣ�ʵ����Ϸ
--------------------------------------------------------------------------------------------------
*/
void begingame();

#endif // !_Map1_demo_h
