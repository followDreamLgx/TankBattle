# include "head.h"
# include "definition.h"
# include "global_value.h"
# include "D3DGUIClass.h"



INT Everything(HWND hWnd,HINSTANCE hInstance)
{
	srand(0);
	keyboard = new KEYBOARDCLASS(hWnd,hInstance);		//键盘鼠标
	mouse = new MOUSECLASS(hWnd,hInstance);

	camera = new CAMERA(g_pd3dDevice);					//摄像机

	terrain = new TERRAIN(g_pd3dDevice);				//地形
	terrain->LoadTerrain(L"dixing.raw",L"green.jpg");

	terrain->InitTerrain(300,500,200.0f,1.0f);

	wchar_t *name[6] = {{L"backsnow1.jpg"},				//天空盒
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
	//主坦克
	tank_host = new TANK(g_pd3dDevice);
	tank_host->Init(L"M60_L.X",1);
	//敌方坦克
	BOUNDINGBOX box1(D3DXVECTOR3(5000,5000,5000));		
	tank_enemy = new TANK_ENEMY(g_pd3dDevice,box1,L"M60_L.X");
	//子弹烟雾的纹理最终没有使用
	tank_gun = new TANK_GUN(g_pd3dDevice,L"222.X",L"snowflake.dds");
	PlaySound(L"仙剑三.wav", NULL, SND_FILENAME | SND_ASYNC|SND_LOOP);
	//具有识别功能的子弹
	gun_child = new GUN_CHILD(g_pd3dDevice,L"222.X",L"snowflake.dds");

	//墙壁
	int ran;
	ran = (int)GetRandomFloat(0,5);		//比数组的行数大1，数组的最后一个元素下标没有传入，作为
										//BOSS坦克的纹理
	wall = new WALL(g_pd3dDevice,wall_name,box,ran);

	//广告牌树
	billboard = new BILLBOARD(g_pd3dDevice,box,L"tree.dds");
	//进击的广告牌
	ex_board = new EXPLORE_BILLBOARD(g_pd3dDevice,box);
	//文字说明
	D3DXCreateFont(g_pd3dDevice,						//P277     2d字体
		50,												//高度
		30,												//平均宽度
		1000,											//字体权重值
		0,												//过滤属性
		false,											//斜体
		DEFAULT_CHARSET,								//字符集，使用默认的字符集
		OUT_DEFAULT_PRECIS,								//输出文本的精度
		DEFAULT_QUALITY,								//字符的输出质量
		0,												//指定字体的索引，通常设为0
		L"隶书",										//字体，此为宋体
		&TextStates);									//UFO
	GetClientRect(hWnd,&rect);						//获取主窗口矩形


//--------------------------------------------------【GUI系统相关代码】-------------------------------------------------------

	// 创建一些GUI系统
	g_MainGUI = new D3DGUIClass(g_pd3dDevice, SCREEN_WIDTH, SCREEN_HEIGHT); //主菜单页面
	g_StartGUI = new D3DGUIClass(g_pd3dDevice, SCREEN_WIDTH, SCREEN_HEIGHT); //开始新游戏页面
	g_LoadGUI = new D3DGUIClass(g_pd3dDevice, SCREEN_WIDTH, SCREEN_HEIGHT); //载入游戏页面
	g_OptionGUI = new D3DGUIClass(g_pd3dDevice, SCREEN_WIDTH, SCREEN_HEIGHT); //设置页面

	// 给四个页面分别添加背景图
	if(!g_MainGUI->AddBackground(L"guidmain3.jpg")) return false;  
	if(!g_StartGUI->AddBackground(L"startgui.jpg")) return false;
	if(!g_LoadGUI->AddBackground(L"loadgui.jpg")) return false;
	if(!g_OptionGUI->AddBackground(L"optiongui.jpg")) return false;


	// 分别给四个页面添加字体
	if(!g_MainGUI->CreateTextFont(L"微软雅黑", 50, &g_MainGUIFontID)) return false;
	if(!g_StartGUI->CreateTextFont(L"微软雅黑", 38, &g_StartGUIFontID)) return false;
	if(!g_LoadGUI->CreateTextFont(L"微软雅黑", 38, &g_LoadGUIFontID)) return false;
	if(!g_OptionGUI->CreateTextFont(L"微软雅黑", 38, &g_OptionGUIFontID)) return false;




	//---------------------------------【主菜单main页面相关的页面布局代码】---------------------------------
	// 添加静态文本到页面中
	if(!g_MainGUI->AddStaticText(STATIC_TEXT_ID, L"健康游戏",
		1170, 735, D3DCOLOR_XRGB(55,155,255), g_MainGUIFontID)) return false;

	if(!g_MainGUI->AddStaticText(STATIC_TEXT_ID, L"无敌坦克大战——健康版",
		500, 250, D3DCOLOR_XRGB(255,255,255), g_MainGUIFontID)) return false;


	// 添加4个按钮，分别是开始游戏，载入进度，选项和退出游戏，每个按钮对应3幅图
	if(!g_MainGUI->AddButton(BUTTON_START_ID, 650, 340, L"startUp.png",
		L"StartOver.png", L"startDown.png")) return false;

	if(!g_MainGUI->AddButton(BUTTON_LOAD_ID, 650, 385, L"loadUp.png",
		L"loadOver.png", L"loadDown.png")) return false;

	if(!g_MainGUI->AddButton(BUTTON_OPTION_ID, 650, 430, L"optionsUp.png",
		L"optionsOver.png", L"optionsDown.png")) return false;

	if(!g_MainGUI->AddButton(BUTTON_QUIT_ID, 650, 475, L"quitUp.png",
		L"quitOver.png", L"quitDown.png")) return false;


	//------------------------【开始新游戏start页面相关的页面布局代码】------------------------
	// 添加按钮到页面中
	//if(!g_StartGUI->AddButton(BUTTON_LEVEL_1_ID,550, 380,  L"level1Up.png", L"level1Over.png",
	//	L"level1Down.png")) return false;

	if(!g_StartGUI->AddButton(BUTTON_BACK_ID, 100, 50, L"backUp.png", L"backOver.png",
		L"backDown.png")) return false;


	//------------------------【载入游戏load页面相关的页面布局代码】------------------------
	//添加静态文本到页面中
	//if(!g_LoadGUI->AddStaticText(STATIC_TEXT_ID, L"这里是load game页面",
	//	411, 340, D3DCOLOR_XRGB(33,255,55), g_LoadGUIFontID)) return false;
	// 添加按钮到页面中
	if(!g_LoadGUI->AddButton(BUTTON_BACK_ID, 100, 50, L"backUp.png", L"backOver.png",
		L"backDown.png")) return false;


	//------------------------【游戏设置option页面相关的页面布局代码】------------------------
	// 添加按钮到页面中
	if(!g_OptionGUI->AddButton(BUTTON_BACK_ID, 100, 50, L"backUp.png", L"backOver.png",
		L"backDown.png")) return false;
	//添加静态文本到页面中
	 //if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"这里是Option页面",
	 //		540, 60, D3DCOLOR_XRGB(33,55,255), g_OptionGUIFontID)) return false;
return 0;
}

//-----------------------------------【GUICallback( )函数】---------------------------------------
//	描述：GUI系统的回调函数，填写按钮按下后的相关处理代码
//--------------------------------------------------------------------------------------------------
void GUICallback(int id, int state)
{
	switch(id)
	{
	case BUTTON_START_ID:   //start game开始游戏按钮
		if(state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_START_SCREEN;
		break;

	case BUTTON_LOAD_ID:  //load game载入游戏按钮
		if(state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_LOAD_SCREEN;
		break;

	case BUTTON_OPTION_ID: //option设置按钮
		if(state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_OPTION_SCREEN;
		break;

	case BUTTON_BACK_ID: //back返回按钮
		if(state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_MAIN_SCREEN;
		break;

	case BUTTON_QUIT_ID://quit退出按钮
		if(state == UGP_BUTTON_DOWN)
			PostQuitMessage(0);
		break;

	case BUTTON_LEVEL_1_ID:  //start game开始游戏页面中，Level1按钮
		//等级一的游戏从这里开始写代码
		break;
	}
}