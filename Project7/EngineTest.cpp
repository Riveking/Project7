#include "EngineTest.h"
#include <stdio.h>

LPCTSTR EngineTest::fish_files[4] = { L".\\res\\red.png",L".\\res\\green.png" ,L".\\res\\blue.png" ,L".\\res\\yellow.png" };
LPCTSTR EngineTest::fish_name[4] = { L"red",L"green",L"blue",L"yellow" };
float EngineTest::rand_size[5] = { 1.0f,0.9f,0.8f,0.7f,0.6f };
int EngineTest::NUM = 10;
int EngineTest::FISH_WIDTH = 143;
int EngineTest::FISH_HEIGHT = 84;


//WinMain函数
int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	LPCTSTR WinTitle = L"李文齐的 T_Engine 测试程序";//游戏窗口标题//我改
	EngineTest* test = new EngineTest(h_instance, WIN_CLASS, WinTitle,IDI_SHARK, IDI_SHARK, 800, 600);
	T_Engine::pEngine = test;//将父类的静态指针指向构造好的子类
	test->SetFrame(24);//设置画面刷新频率

	//myApp->SetFullScreen(TRUE);//全屏启动

	test->StartEngine();
	return TRUE;

}

EngineTest::EngineTest(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title, WORD icon, WORD sm_icon, int winwidth, int winheight)
	:T_Engine(h_instance,sz_winclass,sz_title,icon,sm_icon,winwidth,winheight)
{
	srand((unsigned)time(NULL));
	wnd_width = winwidth;
	wnd_height = winheight;//获取窗口宽高
}

EngineTest::~EngineTest(void){}


void EngineTest::GameInit(){
	seaBed.LoadImageFile(L".\\res\\seabed.jpg");
	seaFloor.LoadImageFile(L".\\res\\seafloor.jpg");
	msgFrame.LoadImageFile(L".\\res\\msg.png");
	for (int i = 0; i < NUM; i++)
	{
		FISH tmpFish;		
		int s = rand() % 5;
		tmpFish.ratio = rand_size[s];
		int w = (int)(rand_size[s] * FISH_WIDTH  );
		int h = (int)(rand_size[s] * FISH_HEIGHT );
		tmpFish.x = rand()%(wnd_width - w);
		tmpFish.y = h  + rand() % (wnd_height - 2 * h);
		tmpFish.oldy = tmpFish.y;//记录鱼原本Y轴位置
		tmpFish.updown = rand() % 2;//鱼的随机上下游，0上游，1下游
		int d = rand() % 2;
		tmpFish.dir = d;//鱼的方向
		int f = rand() % 4;
		LPCTSTR str = fish_files[f];
		tmpFish.fish.LoadImageFile(str);
		tmpFish.name = fish_name[f];
		tmpFish.fcount = 0;
		vecFish.push_back(tmpFish);
	}
}				//游戏初始化接口函数
void EngineTest::GameLogic()
{
	
	vector<FISH>::iterator it;
	for (it = vecFish.begin(); it < vecFish.end(); it++)
	{

		if (it->fcount < 20)
		{
			it->fcount = it->fcount + 1;
			
		}
		if (it->fcount >= 20)
		{
			it -> fcount = 0;
		}

		if (it->dir == 0)
		{
			it->x = it->x - 5 * (it->ratio);//朝左移动
			if (it->x < -100)
			{
				it->x = wnd_width;
			}
		}

		if (it->dir == 1)
		{
			it->x = it->x + 5 * (it->ratio);//朝右移动
			if (it->x >wnd_width)
			{
				it->x = -100;
			}
		}

		if (it->updown == 1)//向下
		{
			it->y = it->y + (rand() % 3) * it->ratio;
			if (it->y > (it->oldy + 100 * it->ratio) || it->y>(wnd_height - FISH_HEIGHT))
			{
				it->updown = 0;
			}
		}
		
		 if (it->updown == 0)//向上
		{
			it->y = it->y - (rand()%3)*it->ratio;
			if (it->y < (it->oldy - 100 * it->ratio))
			{
				it->updown = 1;
			}
		}
	}
}//游戏逻辑处理  处理GameState


void EngineTest::GamePaint(HDC hdc)	//游戏显示	参数值为GameState
{
	seaFloor.PaintImage(hdc, 0, 0, wnd_width, wnd_height, 255);
	seaBed.PaintImage(hdc, 0, 0, wnd_width, wnd_height, 120);

	vector<FISH>::iterator it;
	for (it = vecFish.begin(); it < vecFish.end(); it++)
	{
		int rot = 0;
		int dir = it->dir;

		if (dir == 0)
		{
			rot = 0;//0朝左
		}
		else if(dir==1)
		{
			rot = TRANS_HFLIP_NOROT;
		}
		RectF msgRect;
		msgRect.Width = (REAL)(msgFrame.GetImageWidth());
		msgRect.Height = (REAL)(msgFrame.GetImageHeight());
		msgRect.X = (REAL)(it->x + (it->ratio*FISH_WIDTH - msgFrame.GetImageWidth()) / 2);
		msgRect.Y = (REAL)(it->y - msgFrame.GetImageHeight());
		msgFrame.PaintImage(hdc,(int)msgRect.X,(int)msgRect.Y,(int)msgRect.Width,(int)msgRect.Height,255);

		T_Graph::PaintText(hdc, msgRect, it->name, 10, L"Arial", Color::White, FontStyleBold, StringAlignmentCenter);
		
		
		it->fish.PaintFrame(it->fish.GetBmpHandle(), hdc, it->x, it->y, it->fcount, 20, FISH_WIDTH, FISH_HEIGHT, it->ratio, rot);
	}

	RectF captionRec;
	captionRec.X = 0.00;
	captionRec.Y = 0.00;
	captionRec.Width = (float)wnd_width;
	captionRec.Height = 60.0f;
	LPCTSTR caption = L"李文齐的位图模块绘图实验";
	T_Graph::PaintText(hdc, captionRec, caption, 22, L"微软雅黑", Color::White, FontStyleBold, StringAlignmentNear);
	RectF infoRec;
	infoRec.X = 0.00;
	infoRec.Y = (REAL)(wnd_height - 60);
	infoRec.Width = (float)wnd_width;
	infoRec.Height = 60;
	LPCTSTR info = L"班级：软工1809班 学号8002118239 姓名：李文齐 时间：2020年11月7日";
	T_Graph::PaintText(hdc, infoRec, info, 14, L"宋体", Color::White, FontStyleBold, StringAlignmentCenter);

}

void EngineTest::GameEnd(){
	seaFloor.Destroy();
	seaBed.Destroy();
	msgFrame.Destroy();
	vector<FISH>::iterator it;
	for (it = vecFish.begin(); it < vecFish.end(); it++)
	{
		it->fish.Destroy();
	}
}				//游戏结束处理
void EngineTest::GameKeyAction(int Action)//游戏按键处理
{

}
//游戏鼠标行为处理	参数值为KEY_MOUSE_ACTION
void EngineTest::GameMouseAction(int x,int y,int Action)
{
	
}
