# include "head.h"
# include "definition.h"
# include "global_value.h"
# include "D3DGUIClass.h"



INT Everything(HWND hWnd,HINSTANCE hInstance)
{
	srand(0);
	keyboard = new KEYBOARDCLASS(hWnd,hInstance);		//�������
	mouse = new MOUSECLASS(hWnd,hInstance);

	camera = new CAMERA(g_pd3dDevice);					//�����

	terrain = new TERRAIN(g_pd3dDevice);				//����
	terrain->LoadTerrain(L"dixing.raw",L"green.jpg");

	terrain->InitTerrain(300,500,200.0f,1.0f);

	wchar_t *name[6] = {{L"backsnow1.jpg"},				//��պ�
						{L"leftsnow1.jpg"},
						{L"frontsnow1.jpg"},
						{L"rightsnow1.jpg"},
						{L"topsnow1.jpg"},
						{L"groundsnow1.jpg"}};
	sky = new SKYBOX(g_pd3dDevice);
	sky->InitSkyBox(name);

	BOUNDINGBOX box(D3DXVECTOR3(5000,5000,5000));
	snow = new SNOW(&box,1000);
	snow->init(g_pd3dDevice,L"snowflake.dds");
	//��̹��
	tank_host = new TANK(g_pd3dDevice);
	tank_host->Init(L"M60_L.X",1);
	//�з�̹��
	BOUNDINGBOX box1(D3DXVECTOR3(5000,5000,5000));		
	tank_enemy = new TANK_ENEMY(g_pd3dDevice,box1,L"M60_L.X");
	//�ӵ��������������û��ʹ��
	tank_gun = new TANK_GUN(g_pd3dDevice,L"222.X",L"snowflake.dds");
	PlaySound(L"�ɽ���.wav", NULL, SND_FILENAME | SND_ASYNC|SND_LOOP);
	//����ʶ���ܵ��ӵ�
	gun_child = new GUN_CHILD(g_pd3dDevice,L"222.X",L"snowflake.dds");

	//ǽ��
	int ran;
	ran = (int)GetRandomFloat(0,5);		//�������������1����������һ��Ԫ���±�û�д��룬��Ϊ
										//BOSS̹�˵�����
	wall = new WALL(g_pd3dDevice,wall_name,box,ran);

	//�������
	billboard = new BILLBOARD(g_pd3dDevice,box,L"tree.dds");
	//�����Ĺ����
	ex_board = new EXPLORE_BILLBOARD(g_pd3dDevice,box);
	//����˵��
	D3DXCreateFont(g_pd3dDevice,						//P277     2d����
		50,												//�߶�
		30,												//ƽ�����
		1000,											//����Ȩ��ֵ
		0,												//��������
		false,											//б��
		DEFAULT_CHARSET,								//�ַ�����ʹ��Ĭ�ϵ��ַ���
		OUT_DEFAULT_PRECIS,								//����ı��ľ���
		DEFAULT_QUALITY,								//�ַ����������
		0,												//ָ�������������ͨ����Ϊ0
		L"����",										//���壬��Ϊ����
		&TextStates);									//UFO
	GetClientRect(hWnd,&rect);						//��ȡ�����ھ���


//--------------------------------------------------��GUIϵͳ��ش��롿-------------------------------------------------------

	// ����һЩGUIϵͳ
	g_MainGUI = new D3DGUIClass(g_pd3dDevice, SCREEN_WIDTH, SCREEN_HEIGHT); //���˵�ҳ��
	g_StartGUI = new D3DGUIClass(g_pd3dDevice, SCREEN_WIDTH, SCREEN_HEIGHT); //��ʼ����Ϸҳ��
	g_LoadGUI = new D3DGUIClass(g_pd3dDevice, SCREEN_WIDTH, SCREEN_HEIGHT); //������Ϸҳ��
	g_OptionGUI = new D3DGUIClass(g_pd3dDevice, SCREEN_WIDTH, SCREEN_HEIGHT); //����ҳ��

	// ���ĸ�ҳ��ֱ���ӱ���ͼ
	if(!g_MainGUI->AddBackground(L"guidmain3.jpg")) return false;  
	if(!g_StartGUI->AddBackground(L"startgui.jpg")) return false;
	if(!g_LoadGUI->AddBackground(L"loadgui.jpg")) return false;
	if(!g_OptionGUI->AddBackground(L"optiongui.jpg")) return false;


	// �ֱ���ĸ�ҳ���������
	if(!g_MainGUI->CreateTextFont(L"΢���ź�", 50, &g_MainGUIFontID)) return false;
	if(!g_StartGUI->CreateTextFont(L"΢���ź�", 38, &g_StartGUIFontID)) return false;
	if(!g_LoadGUI->CreateTextFont(L"΢���ź�", 38, &g_LoadGUIFontID)) return false;
	if(!g_OptionGUI->CreateTextFont(L"΢���ź�", 38, &g_OptionGUIFontID)) return false;




	//---------------------------------�����˵�mainҳ����ص�ҳ�沼�ִ��롿---------------------------------
	// ��Ӿ�̬�ı���ҳ����
	if(!g_MainGUI->AddStaticText(STATIC_TEXT_ID, L"������Ϸ",
		1170, 735, D3DCOLOR_XRGB(55,155,255), g_MainGUIFontID)) return false;

	if(!g_MainGUI->AddStaticText(STATIC_TEXT_ID, L"�޵�̹�˴�ս����������",
		500, 250, D3DCOLOR_XRGB(255,255,255), g_MainGUIFontID)) return false;


	// ���4����ť���ֱ��ǿ�ʼ��Ϸ��������ȣ�ѡ����˳���Ϸ��ÿ����ť��Ӧ3��ͼ
	if(!g_MainGUI->AddButton(BUTTON_START_ID, 650, 340, L"startUp.png",
		L"StartOver.png", L"startDown.png")) return false;

	if(!g_MainGUI->AddButton(BUTTON_LOAD_ID, 650, 385, L"loadUp.png",
		L"loadOver.png", L"loadDown.png")) return false;

	if(!g_MainGUI->AddButton(BUTTON_OPTION_ID, 650, 430, L"optionsUp.png",
		L"optionsOver.png", L"optionsDown.png")) return false;

	if(!g_MainGUI->AddButton(BUTTON_QUIT_ID, 650, 475, L"quitUp.png",
		L"quitOver.png", L"quitDown.png")) return false;


	//------------------------����ʼ����Ϸstartҳ����ص�ҳ�沼�ִ��롿------------------------
	// ��Ӱ�ť��ҳ����
	//if(!g_StartGUI->AddButton(BUTTON_LEVEL_1_ID,550, 380,  L"level1Up.png", L"level1Over.png",
	//	L"level1Down.png")) return false;

	if(!g_StartGUI->AddButton(BUTTON_BACK_ID, 100, 50, L"backUp.png", L"backOver.png",
		L"backDown.png")) return false;


	//------------------------��������Ϸloadҳ����ص�ҳ�沼�ִ��롿------------------------
	//��Ӿ�̬�ı���ҳ����
	//if(!g_LoadGUI->AddStaticText(STATIC_TEXT_ID, L"������load gameҳ��",
	//	411, 340, D3DCOLOR_XRGB(33,255,55), g_LoadGUIFontID)) return false;
	// ��Ӱ�ť��ҳ����
	if(!g_LoadGUI->AddButton(BUTTON_BACK_ID, 100, 50, L"backUp.png", L"backOver.png",
		L"backDown.png")) return false;


	//------------------------����Ϸ����optionҳ����ص�ҳ�沼�ִ��롿------------------------
	// ��Ӱ�ť��ҳ����
	if(!g_OptionGUI->AddButton(BUTTON_BACK_ID, 100, 50, L"backUp.png", L"backOver.png",
		L"backDown.png")) return false;
	//��Ӿ�̬�ı���ҳ����
	 //if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"������Optionҳ��",
	 //		540, 60, D3DCOLOR_XRGB(33,55,255), g_OptionGUIFontID)) return false;
return 0;
}

//-----------------------------------��GUICallback( )������---------------------------------------
//	������GUIϵͳ�Ļص���������д��ť���º����ش������
//--------------------------------------------------------------------------------------------------
void GUICallback(int id, int state)
{
	switch(id)
	{
	case BUTTON_START_ID:   //start game��ʼ��Ϸ��ť
		if(state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_START_SCREEN;
		break;

	case BUTTON_LOAD_ID:  //load game������Ϸ��ť
		if(state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_LOAD_SCREEN;
		break;

	case BUTTON_OPTION_ID: //option���ð�ť
		if(state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_OPTION_SCREEN;
		break;

	case BUTTON_BACK_ID: //back���ذ�ť
		if(state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_MAIN_SCREEN;
		break;

	case BUTTON_QUIT_ID://quit�˳���ť
		if(state == UGP_BUTTON_DOWN)
			PostQuitMessage(0);
		break;

	case BUTTON_LEVEL_1_ID:  //start game��ʼ��Ϸҳ���У�Level1��ť
		//�ȼ�һ����Ϸ�����￪ʼд����
		break;
	}
}