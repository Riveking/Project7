#pragma once

#include "T_Engine.h"
#include "T_Graph.h"
#include "resource.h"

struct FISH
{
	int x;
	int y;
	int oldy;
	float ratio;
	int fcount;
	int dir;
	int updown;//定义鱼的上下浮动
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
	//重载T_Engine类中的虚函数
	void GameInit();			//游戏初始化
	void GameLogic();			//游戏逻辑处理
	void GameEnd();				//游戏结束处理
	void GamePaint(HDC hdc);	//游戏显示
	void GameKeyAction(int Action = KEY_SYS_NONE);//按键行为处理
	void GameMouseAction(int x, int y, int Action);//鼠标行为处理


private:
	//记录游戏窗口宽高
	int wnd_width, wnd_height;
	
	HBITMAP hBitmap;//保存已加载的位图的句柄
	int bmpWidth, bmpHeight;//已加载位图的宽高
	HBITMAP hbitmap;//当前位图句柄


	//实验六
	T_Graph seaFloor;
	T_Graph seaBed;
	T_Graph msgFrame;

	static float rand_size[5];
	static LPCTSTR fish_files[4];
	static LPCTSTR fish_name[4];
	static int NUM;
	static int FISH_WIDTH;
	static int FISH_HEIGHT;

	vector<FISH> vecFish;

	HBITMAP GetBmpHandle()
	{
		return hbitmap;
	};

};
