#include "EngineTest.h"
#include <stdio.h>

LPCTSTR EngineTest::fish_files[4] = { L".\\res\\red.png",L".\\res\\green.png" ,L".\\res\\blue.png" ,L".\\res\\yellow.png" };
LPCTSTR EngineTest::fish_name[4] = { L"red",L"green",L"blue",L"yellow" };
float EngineTest::rand_size[5] = { 1.0f,0.9f,0.8f,0.7f,0.6f };
int EngineTest::NUM = 10;
int EngineTest::FISH_WIDTH = 143;
int EngineTest::FISH_HEIGHT = 84;
int EngineTest::MYCHOOSEMENU = 1;
int EngineTest::IsPaintAbout = 0;
int EngineTest::IsPaintHelp = 0;
int EngineTest::IsPaintFish = 0;


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

	IntMyMenu();
	GameState = GAME_START;

}				//游戏初始化接口函数
void EngineTest::GameLogic()
{
	LogicMyFish();//判断鱼的游向和位置

}//游戏逻辑处理  处理GameState


void EngineTest::GamePaint(HDC hdc)	//游戏显示	参数值为GameState
{
	/*实验七部分*/
	DrawMyMenu(hdc);

	/*实验六部分*/
	//seaFloor.PaintImage(hdc, 0, 0, wnd_width, wnd_height, 255);
	//seaBed.PaintImage(hdc, 0, 0, wnd_width, wnd_height, 120);
	PrintMyFish(hdc);
	

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
{//更改，如果游戏是启动状态，可以监听键盘，如果是游玩状态，则t_menu不管
	if (GetAsyncKeyState(VK_UP) < 0)
	{
		t_menu.MenuKeyDown(VK_UP);
	}
	if (GetAsyncKeyState(VK_DOWN) < 0)
	{
		t_menu.MenuKeyDown(VK_DOWN);
	}
	if (GetAsyncKeyState(VK_LEFT) < 0)
	{
		t_menu.MenuKeyDown(VK_LEFT);
	}
	if (GetAsyncKeyState(VK_RIGHT) < 0)
	{
		t_menu.MenuKeyDown(VK_RIGHT);
	}
	if (GetAsyncKeyState(VK_RETURN) < 0)
	{
		if (t_menu.GetMenuIndex() >= 0)//进入游戏后index改成-1
		{
			switch (t_menu.GetMenuIndex())
			{
			case 0://添加新游戏代码
				t_menu.DestroyAll();
				//EngineTest::IsPaintText = 1;
				SetMyFish();
				IntMyFish();

				break;
			case 1://添加关于代码
				t_menu.DestroyAll();
				SetMyAbout();
				IntMyMenu2();
				break;
			case 2://添加帮助代码
				t_menu.DestroyAll();
				SetMyHelp();
				IntMyMenu3();
				break;
			case 3:
				SetMyReset();
				SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
				break;
			}
			t_menu.SetMenuIndex(-1);
		}
	}
}
//游戏鼠标行为处理	参数值为KEY_MOUSE_ACTION
void EngineTest::GameMouseAction(int x,int y,int Action)
{
	if (Action == MOUSE_MOVE&&GameState != GAME_RUN)
	{
		t_menu.MenuMouseMove(x, y);
	}
	if (Action == MOUSE_LCLICK) {
		if (GameState == GAME_START)
		{
			int index = t_menu.MenuMouseClick(x, y);
			if (index >= 0)
			{
				switch (index)
				{
				case 0://添加新游戏代码
					t_menu.DestroyAll();
					//EngineTest::IsPaintText = 1;
					SetMyFish();
					IntMyFish();

					break;
				case 1://添加关于代码
					t_menu.DestroyAll();
					SetMyAbout();
					IntMyMenu2();
					break;
				case 2://添加帮助代码
					t_menu.DestroyAll();
					SetMyHelp();
					IntMyMenu3();
					break;
				case 3:
					SetMyReset();
					SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
					break;
				}
			}
		}
	}
}

void EngineTest::IntMyFish()
{
	seaBed.LoadImageFile(L".\\res\\seabed.jpg");
	seaFloor.LoadImageFile(L".\\res\\seafloor.jpg");
	msgFrame.LoadImageFile(L".\\res\\msg.png");
	for (int i = 0; i < NUM; i++)
	{
		FISH tmpFish;
		int s = rand() % 5;
		tmpFish.ratio = rand_size[s];
		int w = (int)(rand_size[s] * FISH_WIDTH);
		int h = (int)(rand_size[s] * FISH_HEIGHT);
		tmpFish.x = rand() % (wnd_width - w);
		tmpFish.y = h + rand() % (wnd_height - 2 * h);
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
}

void EngineTest::PaintMyText(HDC hdc)
{
	if(EngineTest::IsPaintFish==1){
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
	LPCTSTR info = L"班级：软工1809班 学号8002118239 姓名：李文齐 时间：2020年11月12日";
	T_Graph::PaintText(hdc, infoRec, info, 14, L"宋体", Color::White, FontStyleBold, StringAlignmentCenter);
	}
}



void EngineTest::PrintMyFish(HDC hdc)//画鱼和背景
{
	if (EngineTest::IsPaintFish == 1) {
		seaFloor.PaintImage(hdc, 0, 0, wnd_width, wnd_height, 255);
		seaBed.PaintImage(hdc, 0, 0, wnd_width, wnd_height, 120);
		PaintMyText(hdc);
		vector<FISH>::iterator it;
		for (it = vecFish.begin(); it < vecFish.end(); it++)
		{
			int rot = 0;
			int dir = it->dir;

			if (dir == 0)
			{
				rot = 0;//0朝左
			}
			else if (dir == 1)
			{
				rot = TRANS_HFLIP_NOROT;
			}
			RectF msgRect;
			msgRect.Width = (REAL)(msgFrame.GetImageWidth());
			msgRect.Height = (REAL)(msgFrame.GetImageHeight());
			msgRect.X = (REAL)(it->x + (it->ratio*FISH_WIDTH - msgFrame.GetImageWidth()) / 2);
			msgRect.Y = (REAL)(it->y - msgFrame.GetImageHeight());
			msgFrame.PaintImage(hdc, (int)msgRect.X, (int)msgRect.Y, (int)msgRect.Width, (int)msgRect.Height, 255);

			T_Graph::PaintText(hdc, msgRect, it->name, 10, L"Arial", Color::White, FontStyleBold, StringAlignmentCenter);


			it->fish.PaintFrame(it->fish.GetBmpHandle(), hdc, it->x, it->y, it->fcount, 20, FISH_WIDTH, FISH_HEIGHT, it->ratio, rot);
		}
	}
}

void EngineTest::LogicMyFish()
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
			it->fcount = 0;
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
			it->y = it->y - (rand() % 3)*it->ratio;
			if (it->y < (it->oldy - 100 * it->ratio))
			{
				it->updown = 1;
			}
		}
	}
}

/*将x、y坐标修改如下，可以实现水平居中布局
x =i*(btn_ width+MENU_SPACE)+(wndWidth - 4*btn_width - 3*MENU_SPACE)/2;
y = wnd_height-2 *btn_height;
*/

/*将x、y坐标修改如下，可以实现双行双列布局
int col=i%2;
int row=i/2;
x =col*(btn_ width+MENU_SPACE)+(wndWidth - 2*btn_width - *MENU_SPACE)/2;
y = row*(btn_height+MENU_SPACE)+(wnd_height-2*btn_height-MENU_SPACE)/2;
*/

/*将按钮修改如下，可以改成圆形布局效果
btn_ width= 120;
btn_ height= 120;
normalClr = Color::White;
focusClr = Color:: Yellow;
t_menu.SetBtnBmp(L".\\res\\redroundbtn.png", btn_width, btn_height);// 菜单项图片
*/
void EngineTest::IntMyMenu()
{
	/*实验七代码*/
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"新游戏",L"关于",L"帮助",L"退出" };
	t_menu.SetMenuBkg(L".\\res\\menubkg.jpg");
	//长条形的按钮菜单项
	btn_width = 250;
	btn_height = 70;
	normalClr = Color::Red;
	focusClr = Color::White;
	t_menu.SetBtnBmp(L".\\res\\button.png", btn_width, btn_height);
	//设置菜单信息
	MENU_INFO menuInfo;
	menuInfo.align = 1;//居中对齐
	menuInfo.space = MENU_SPACE;//菜单项之间的间隔距离
	menuInfo.width = btn_width;//菜单项宽高
	menuInfo.height = btn_height;
	menuInfo.fontName = L"黑体";
	menuInfo.isBold = true;
	menuInfo.normalTextColor = normalClr;
	menuInfo.focusTextColor = focusClr;
	t_menu.SetMenuInfo(menuInfo);

	for (int i = 0; i < 4; i++)
	{
		//垂直居中布局的坐标
		x = (wndWidth - btn_width) / 2;
		y = i*(btn_height + MENU_SPACE) + (wnd_height - 4 * btn_height - 3 * MENU_SPACE) / 2;

		MENUITEM mItem;
		mItem.pos.x = x;//当前菜单项xy坐标
		mItem.pos.y = y;
		mItem.ItemName = menuItems[i];
		t_menu.AddMenuItem(mItem);
	}
}

void EngineTest::IntMyMenu2()
{
	/*关于菜单*/
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"新游戏",L"关于",L"帮助",L"退出" };
	t_menu.SetMenuBkg(L".\\res\\beach.jpg");
	//长条形的按钮菜单项
	btn_width = 120;
	btn_height = 120;
	normalClr = Color::White;
	focusClr = Color::Yellow;
	t_menu.SetBtnBmp(L".\\res\\redroundbtn.png", btn_width, btn_height);// 菜单项图片
	//设置菜单信息
	MENU_INFO menuInfo;
	menuInfo.align = 1;//居中对齐
	menuInfo.space = MENU_SPACE;//菜单项之间的间隔距离
	menuInfo.width = btn_width;//菜单项宽高
	menuInfo.height = btn_height;
	menuInfo.fontName = L"黑体";
	menuInfo.isBold = true;
	menuInfo.normalTextColor = normalClr;
	menuInfo.focusTextColor = focusClr;
	t_menu.SetMenuInfo(menuInfo);

	for (int i = 0; i < 4; i++)
	{
		//垂直居中布局的坐标
		x = i*(btn_width + MENU_SPACE) + (wndWidth - 4 * btn_width - 3 * MENU_SPACE) / 2;
		y = wnd_height - 2 * btn_height;

		MENUITEM mItem;
		mItem.pos.x = x;//当前菜单项xy坐标
		mItem.pos.y = y;
		mItem.ItemName = menuItems[i];
		t_menu.AddMenuItem(mItem);
	}
}

void EngineTest::IntMyMenu3()
{
	/*关于菜单*/
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"新游戏",L"关于",L"帮助",L"退出" };
	t_menu.SetMenuBkg(L".\\res\\stage.jpg");
	//长条形的按钮菜单项
	btn_width = 250;
	btn_height = 70;
	normalClr = Color::Red;
	focusClr = Color::White;
	t_menu.SetBtnBmp(L".\\res\\button.png", btn_width, btn_height);
	//设置菜单信息
	MENU_INFO menuInfo;
	menuInfo.align = 1;//居中对齐
	menuInfo.space = MENU_SPACE;//菜单项之间的间隔距离
	menuInfo.width = btn_width;//菜单项宽高
	menuInfo.height = btn_height;
	menuInfo.fontName = L"黑体";
	menuInfo.isBold = true;
	menuInfo.normalTextColor = normalClr;
	menuInfo.focusTextColor = focusClr;
	t_menu.SetMenuInfo(menuInfo);

	for (int i = 0; i < 4; i++)
	{
		int col = i % 2;
		int row = i / 2;
		x = col*(btn_width + MENU_SPACE) + (wndWidth - 2 * btn_width - MENU_SPACE) / 2;
		y = row*(btn_height + MENU_SPACE) + (wnd_height - 2 * btn_height - MENU_SPACE);

		MENUITEM mItem;
		mItem.pos.x = x;//当前菜单项xy坐标
		mItem.pos.y = y;
		mItem.ItemName = menuItems[i];
		t_menu.AddMenuItem(mItem);
	}
}

void EngineTest::DrawMyMenu(HDC hdc)
{

	if (GameState == GAME_START)
	{
		t_menu.DrawMenu(hdc);
		RectF textRec;
		textRec.X = 0.00;
		textRec.Y = 0.00;
		textRec.Width = (float)wnd_width;
		textRec.Height = (float)wnd_height / 4;
		if (EngineTest::IsPaintAbout == 1)
		{
			T_Graph::PaintText(hdc, textRec, L"作者信息", 36, L"黑体", Color::White, FontStyleBold, StringAlignmentCenter);
			PaintMyAbout(hdc);
		}
		else if (EngineTest::IsPaintHelp == 1)
		{
			T_Graph::PaintText(hdc, textRec, L"操作说明", 36, L"黑体", Color::White, FontStyleBold, StringAlignmentCenter);
			PaintMyHelp(hdc);
		}
		else {
			T_Graph::PaintText(hdc, textRec, L"游戏菜单", 36, L"黑体", Color::White, FontStyleBold, StringAlignmentCenter);
		}
		

	}
}


void EngineTest::PaintMyAbout(HDC hdc)
{
		if (EngineTest::IsPaintAbout == 1)
		{
			RectF captionRec;
			captionRec.X = 0.00;
			captionRec.Y = 0.00;
			captionRec.Width = (float)wnd_width;
			captionRec.Height = 60.0f;
			LPCTSTR caption = L"关于";
			T_Graph::PaintText(hdc, captionRec, caption, 22, L"微软雅黑", Color::White, FontStyleBold, StringAlignmentCenter);
			RectF infoRec;
			infoRec.X = 0.00;
			infoRec.Y = (REAL)(wnd_height / 2 - 80);
			infoRec.Width = (float)wnd_width;
			infoRec.Height = 60;
			LPCTSTR info = L"姓名：李文齐 时间：2020年11月12日\n 班级：软工1809班 学号8002118239";
			T_Graph::PaintText(hdc, infoRec, info, 14, L"宋体", Color::White, FontStyleBold, StringAlignmentCenter);
		}
}

void EngineTest::PaintMyHelp(HDC hdc)
{
	if (EngineTest::IsPaintHelp == 1)
	{
		RectF captionRec;
		captionRec.X = 0.00;
		captionRec.Y = 0.00;
		captionRec.Width = (float)wnd_width;
		captionRec.Height = 60.0f;
		LPCTSTR caption = L"帮助";
		T_Graph::PaintText(hdc, captionRec, caption, 22, L"微软雅黑", Color::White, FontStyleBold, StringAlignmentCenter);
		RectF infoRec;
		infoRec.X = 0.00;
		infoRec.Y = (REAL)(wnd_height / 2 - 80);
		infoRec.Width = (float)wnd_width;
		infoRec.Height = 60;
		LPCTSTR info = L"通过上下左右方向键控制小鱼，\n躲避大鱼，吃掉小鱼";
		T_Graph::PaintText(hdc, infoRec, info, 14, L"宋体", Color::White, FontStyleBold, StringAlignmentCenter);
	}
}

void EngineTest::SetMyAbout()
{
	EngineTest::IsPaintAbout = 1;
	EngineTest::IsPaintFish = 0;
	EngineTest::IsPaintHelp = 0;
}

void EngineTest::SetMyHelp()
{
	EngineTest::IsPaintAbout = 0;
	EngineTest::IsPaintFish = 0;
	EngineTest::IsPaintHelp = 1;
}

void EngineTest::SetMyFish()
{
	EngineTest::IsPaintAbout = 0;
	EngineTest::IsPaintFish = 1;
	EngineTest::IsPaintHelp = 0;
}

void EngineTest::SetMyReset()
{
	EngineTest::IsPaintAbout = 0;
	EngineTest::IsPaintFish = 0;
	EngineTest::IsPaintHelp = 0;
}