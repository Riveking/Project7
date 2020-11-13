#pragma once

#include "T_Engine.h"
#include "T_Graph.h"
#include "T_Menu.h"
#include "resource.h"

struct FISH
{
	int x;
	int y;
	int oldy;
	float ratio;
	int fcount;
	int dir;
	int updown;//����������¸���
	LPCTSTR name;
	T_Graph fish;
};

class EngineTest :public T_Engine
{
public:
	virtual ~EngineTest(void);
	EngineTest(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title,
		WORD icon = IDI_SHARK, WORD sm_icon = IDI_SHARK,
		int winwidth = WIN_WIDTH, int winheight = WIN_HEIGHT);
	//����T_Engine���е��麯��
	void GameInit();			//��Ϸ��ʼ��
	void GameLogic();			//��Ϸ�߼�����
	void GameEnd();				//��Ϸ��������
	void GamePaint(HDC hdc);	//��Ϸ��ʾ
	void GameKeyAction(int Action = KEY_SYS_NONE);//������Ϊ����
	void GameMouseAction(int x, int y, int Action);//�����Ϊ����

	/*ʵ����*/
	void PrintMyFish(HDC hdc);//���ҵ���
	void IntMyFish();//��ʼ���ҵ���
	void LogicMyFish();//�߼����ҵ���
	void PaintMyText(HDC hdc);//���ҵ���
	void IntMyMenu();//��ʼ���ҵĲ˵�
	void IntMyMenu2();//���ڲ˵�
	void IntMyMenu3();//�����˵�

	void DrawMyMenu(HDC hdc);//���ҵĲ˵�



private:
	//��¼��Ϸ���ڿ��
	int wnd_width, wnd_height;
	T_Menu t_menu;//��Ϸ�˵���Ķ���
	HBITMAP hBitmap;//�����Ѽ��ص�λͼ�ľ��
	int bmpWidth, bmpHeight;//�Ѽ���λͼ�Ŀ��
	HBITMAP hbitmap;//��ǰλͼ���


	//ʵ����
	T_Graph seaFloor;
	T_Graph seaBed;
	T_Graph msgFrame;

	static float rand_size[5];
	static LPCTSTR fish_files[4];
	static LPCTSTR fish_name[4];
	static int NUM;
	static int FISH_WIDTH;
	static int FISH_HEIGHT;

	static int MYCHOOSEMENU;//��¼�Ҳ˵���ѡ��

	vector<FISH> vecFish;

	HBITMAP GetBmpHandle()
	{
		return hbitmap;
	};

};
