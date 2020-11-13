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


//WinMain����
int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	LPCTSTR WinTitle = L"������� T_Engine ���Գ���";//��Ϸ���ڱ���//�Ҹ�
	EngineTest* test = new EngineTest(h_instance, WIN_CLASS, WinTitle,IDI_SHARK, IDI_SHARK, 800, 600);
	T_Engine::pEngine = test;//������ľ�ָ̬��ָ����õ�����
	test->SetFrame(24);//���û���ˢ��Ƶ��

	//myApp->SetFullScreen(TRUE);//ȫ������

	test->StartEngine();
	return TRUE;

}

EngineTest::EngineTest(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title, WORD icon, WORD sm_icon, int winwidth, int winheight)
	:T_Engine(h_instance,sz_winclass,sz_title,icon,sm_icon,winwidth,winheight)
{
	srand((unsigned)time(NULL));
	wnd_width = winwidth;
	wnd_height = winheight;//��ȡ���ڿ��
}

EngineTest::~EngineTest(void){}


void EngineTest::GameInit(){

	IntMyMenu();
	GameState = GAME_START;

}				//��Ϸ��ʼ���ӿں���
void EngineTest::GameLogic()
{
	LogicMyFish();//�ж���������λ��

}//��Ϸ�߼�����  ����GameState


void EngineTest::GamePaint(HDC hdc)	//��Ϸ��ʾ	����ֵΪGameState
{
	/*ʵ���߲���*/
	DrawMyMenu(hdc);

	/*ʵ��������*/
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
}				//��Ϸ��������


void EngineTest::GameKeyAction(int Action)//��Ϸ��������
{//���ģ������Ϸ������״̬�����Լ������̣����������״̬����t_menu����
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
		if (t_menu.GetMenuIndex() >= 0)//������Ϸ��index�ĳ�-1
		{
			switch (t_menu.GetMenuIndex())
			{
			case 0://�������Ϸ����
				t_menu.DestroyAll();
				//EngineTest::IsPaintText = 1;
				SetMyFish();
				IntMyFish();

				break;
			case 1://��ӹ��ڴ���
				t_menu.DestroyAll();
				SetMyAbout();
				IntMyMenu2();
				break;
			case 2://��Ӱ�������
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
//��Ϸ�����Ϊ����	����ֵΪKEY_MOUSE_ACTION
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
				case 0://�������Ϸ����
					t_menu.DestroyAll();
					//EngineTest::IsPaintText = 1;
					SetMyFish();
					IntMyFish();

					break;
				case 1://��ӹ��ڴ���
					t_menu.DestroyAll();
					SetMyAbout();
					IntMyMenu2();
					break;
				case 2://��Ӱ�������
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
		tmpFish.oldy = tmpFish.y;//��¼��ԭ��Y��λ��
		tmpFish.updown = rand() % 2;//�����������Σ�0���Σ�1����
		int d = rand() % 2;
		tmpFish.dir = d;//��ķ���
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
	LPCTSTR caption = L"�������λͼģ���ͼʵ��";
	T_Graph::PaintText(hdc, captionRec, caption, 22, L"΢���ź�", Color::White, FontStyleBold, StringAlignmentNear);
	RectF infoRec;
	infoRec.X = 0.00;
	infoRec.Y = (REAL)(wnd_height - 60);
	infoRec.Width = (float)wnd_width;
	infoRec.Height = 60;
	LPCTSTR info = L"�༶����1809�� ѧ��8002118239 ������������ ʱ�䣺2020��11��12��";
	T_Graph::PaintText(hdc, infoRec, info, 14, L"����", Color::White, FontStyleBold, StringAlignmentCenter);
	}
}



void EngineTest::PrintMyFish(HDC hdc)//����ͱ���
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
				rot = 0;//0����
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
			it->x = it->x - 5 * (it->ratio);//�����ƶ�
			if (it->x < -100)
			{
				it->x = wnd_width;
			}
		}

		if (it->dir == 1)
		{
			it->x = it->x + 5 * (it->ratio);//�����ƶ�
			if (it->x >wnd_width)
			{
				it->x = -100;
			}
		}

		if (it->updown == 1)//����
		{
			it->y = it->y + (rand() % 3) * it->ratio;
			if (it->y > (it->oldy + 100 * it->ratio) || it->y>(wnd_height - FISH_HEIGHT))
			{
				it->updown = 0;
			}
		}

		if (it->updown == 0)//����
		{
			it->y = it->y - (rand() % 3)*it->ratio;
			if (it->y < (it->oldy - 100 * it->ratio))
			{
				it->updown = 1;
			}
		}
	}
}

/*��x��y�����޸����£�����ʵ��ˮƽ���в���
x =i*(btn_ width+MENU_SPACE)+(wndWidth - 4*btn_width - 3*MENU_SPACE)/2;
y = wnd_height-2 *btn_height;
*/

/*��x��y�����޸����£�����ʵ��˫��˫�в���
int col=i%2;
int row=i/2;
x =col*(btn_ width+MENU_SPACE)+(wndWidth - 2*btn_width - *MENU_SPACE)/2;
y = row*(btn_height+MENU_SPACE)+(wnd_height-2*btn_height-MENU_SPACE)/2;
*/

/*����ť�޸����£����Ըĳ�Բ�β���Ч��
btn_ width= 120;
btn_ height= 120;
normalClr = Color::White;
focusClr = Color:: Yellow;
t_menu.SetBtnBmp(L".\\res\\redroundbtn.png", btn_width, btn_height);// �˵���ͼƬ
*/
void EngineTest::IntMyMenu()
{
	/*ʵ���ߴ���*/
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"����Ϸ",L"����",L"����",L"�˳�" };
	t_menu.SetMenuBkg(L".\\res\\menubkg.jpg");
	//�����εİ�ť�˵���
	btn_width = 250;
	btn_height = 70;
	normalClr = Color::Red;
	focusClr = Color::White;
	t_menu.SetBtnBmp(L".\\res\\button.png", btn_width, btn_height);
	//���ò˵���Ϣ
	MENU_INFO menuInfo;
	menuInfo.align = 1;//���ж���
	menuInfo.space = MENU_SPACE;//�˵���֮��ļ������
	menuInfo.width = btn_width;//�˵�����
	menuInfo.height = btn_height;
	menuInfo.fontName = L"����";
	menuInfo.isBold = true;
	menuInfo.normalTextColor = normalClr;
	menuInfo.focusTextColor = focusClr;
	t_menu.SetMenuInfo(menuInfo);

	for (int i = 0; i < 4; i++)
	{
		//��ֱ���в��ֵ�����
		x = (wndWidth - btn_width) / 2;
		y = i*(btn_height + MENU_SPACE) + (wnd_height - 4 * btn_height - 3 * MENU_SPACE) / 2;

		MENUITEM mItem;
		mItem.pos.x = x;//��ǰ�˵���xy����
		mItem.pos.y = y;
		mItem.ItemName = menuItems[i];
		t_menu.AddMenuItem(mItem);
	}
}

void EngineTest::IntMyMenu2()
{
	/*���ڲ˵�*/
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"����Ϸ",L"����",L"����",L"�˳�" };
	t_menu.SetMenuBkg(L".\\res\\beach.jpg");
	//�����εİ�ť�˵���
	btn_width = 120;
	btn_height = 120;
	normalClr = Color::White;
	focusClr = Color::Yellow;
	t_menu.SetBtnBmp(L".\\res\\redroundbtn.png", btn_width, btn_height);// �˵���ͼƬ
	//���ò˵���Ϣ
	MENU_INFO menuInfo;
	menuInfo.align = 1;//���ж���
	menuInfo.space = MENU_SPACE;//�˵���֮��ļ������
	menuInfo.width = btn_width;//�˵�����
	menuInfo.height = btn_height;
	menuInfo.fontName = L"����";
	menuInfo.isBold = true;
	menuInfo.normalTextColor = normalClr;
	menuInfo.focusTextColor = focusClr;
	t_menu.SetMenuInfo(menuInfo);

	for (int i = 0; i < 4; i++)
	{
		//��ֱ���в��ֵ�����
		x = i*(btn_width + MENU_SPACE) + (wndWidth - 4 * btn_width - 3 * MENU_SPACE) / 2;
		y = wnd_height - 2 * btn_height;

		MENUITEM mItem;
		mItem.pos.x = x;//��ǰ�˵���xy����
		mItem.pos.y = y;
		mItem.ItemName = menuItems[i];
		t_menu.AddMenuItem(mItem);
	}
}

void EngineTest::IntMyMenu3()
{
	/*���ڲ˵�*/
	int x = 0, y = 0;
	int btn_width = 0, btn_height = 0;
	Color normalClr, focusClr;
	wstring menuItems[] = { L"����Ϸ",L"����",L"����",L"�˳�" };
	t_menu.SetMenuBkg(L".\\res\\stage.jpg");
	//�����εİ�ť�˵���
	btn_width = 250;
	btn_height = 70;
	normalClr = Color::Red;
	focusClr = Color::White;
	t_menu.SetBtnBmp(L".\\res\\button.png", btn_width, btn_height);
	//���ò˵���Ϣ
	MENU_INFO menuInfo;
	menuInfo.align = 1;//���ж���
	menuInfo.space = MENU_SPACE;//�˵���֮��ļ������
	menuInfo.width = btn_width;//�˵�����
	menuInfo.height = btn_height;
	menuInfo.fontName = L"����";
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
		mItem.pos.x = x;//��ǰ�˵���xy����
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
			T_Graph::PaintText(hdc, textRec, L"������Ϣ", 36, L"����", Color::White, FontStyleBold, StringAlignmentCenter);
			PaintMyAbout(hdc);
		}
		else if (EngineTest::IsPaintHelp == 1)
		{
			T_Graph::PaintText(hdc, textRec, L"����˵��", 36, L"����", Color::White, FontStyleBold, StringAlignmentCenter);
			PaintMyHelp(hdc);
		}
		else {
			T_Graph::PaintText(hdc, textRec, L"��Ϸ�˵�", 36, L"����", Color::White, FontStyleBold, StringAlignmentCenter);
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
			LPCTSTR caption = L"����";
			T_Graph::PaintText(hdc, captionRec, caption, 22, L"΢���ź�", Color::White, FontStyleBold, StringAlignmentCenter);
			RectF infoRec;
			infoRec.X = 0.00;
			infoRec.Y = (REAL)(wnd_height / 2 - 80);
			infoRec.Width = (float)wnd_width;
			infoRec.Height = 60;
			LPCTSTR info = L"������������ ʱ�䣺2020��11��12��\n �༶����1809�� ѧ��8002118239";
			T_Graph::PaintText(hdc, infoRec, info, 14, L"����", Color::White, FontStyleBold, StringAlignmentCenter);
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
		LPCTSTR caption = L"����";
		T_Graph::PaintText(hdc, captionRec, caption, 22, L"΢���ź�", Color::White, FontStyleBold, StringAlignmentCenter);
		RectF infoRec;
		infoRec.X = 0.00;
		infoRec.Y = (REAL)(wnd_height / 2 - 80);
		infoRec.Width = (float)wnd_width;
		infoRec.Height = 60;
		LPCTSTR info = L"ͨ���������ҷ��������С�㣬\n��ܴ��㣬�Ե�С��";
		T_Graph::PaintText(hdc, infoRec, info, 14, L"����", Color::White, FontStyleBold, StringAlignmentCenter);
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