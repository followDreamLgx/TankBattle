/****************************************
说明
1.函数参数的详细说明以及详细用法在函数的定义处，声明处只说明该函数的作用
2.为了方便查找，所有的全局变量在winmain.cpp中定义，需要用到的时候自行拓展
3.为了查找书签，使用Pn(郑阿奇)和PLn(龙书)的形式记录书签
****************************************/
# include "head.h"										//包含了所有头文件
# include "definition.h"
# include "camera.h"
# include "terrain.h"
# include "skybox.h"
# include "alpha.h"
# include "mirror_shadow.h"
# include "billboard.h"
# include "snow.h"
# include "input.h"
# include "tank.h"
# include "wall.h"
# include "D3DGUIClass.h"

wchar_t				*g_pClassName = L"7-25";							//初始化窗口类以及创建窗口的时候用到，必须一致
wchar_t				*g_pWindowName = L"坦克大战";						//窗口名称
WNDCLASSEX			wndclass;											//全局变量，此变量在初始化窗口以及撤销窗口的时候都要用到，
LPDIRECT3DDEVICE9	g_pd3dDevice = NULL;								//Direct3DDevice9设备接口

CAMERA				*camera = NULL;
TERRAIN				*terrain = NULL;
SKYBOX				*sky = NULL;
ALPHA				*pAlphaMesh = NULL;
SNOW				*snow = NULL;									//雪花纷飞
KEYBOARDCLASS		*keyboard = NULL;								//键盘
MOUSECLASS			*mouse = NULL;									//鼠标
TANK				*tank_host = NULL;								//主坦克
TANK_ENEMY			*tank_enemy = NULL;								//敌方坦克		
TANK_GUN			*tank_gun = NULL;								//坦克发射的子弹
GUN_CHILD			*gun_child = NULL;								//识别功能的子弹
wchar_t				wall_name[6][50] = {{L"WoodCutter.X"},//
										{L"555.X"},			//
										{L"text222.X"},		//
										{L"text222.X"},		//
										{L"WoodCutter.X"},	//
										{L"TIGER_L.X"}		
														};			//墙壁的x文件
WALL				*wall = NULL;									//墙壁
BILLBOARD			*billboard = NULL;								//广告牌树
EXPLORE_BILLBOARD	*ex_board = NULL;								//进击的广告牌
LPD3DXFONT TextStates = NULL;										//2d
bool				tree_or_explore = 1;				
RECT rect;															//输出位置的确定
wchar_t a[3][50] = {	{L"是否继续？"},
						{L"是（Y）"},
						{L"否（N）"}};
D3DGUIClass		*g_MainGUI = NULL;		//主窗口
D3DGUIClass		*g_StartGUI = NULL;		//游戏开始窗口
D3DGUIClass		*g_LoadGUI = NULL;		//游戏载入窗口
D3DGUIClass		*g_OptionGUI = NULL;	//游戏设置窗口
int						g_MainGUIFontID = -1;						//  GUI中字体对象的ID
int						g_StartGUIFontID = -1;						//  GUI中字体对象的ID
int						g_LoadGUIFontID = -1;						//  GUI中字体对象的ID
int						g_OptionGUIFontID = -1;						//  GUI中字体对象的ID
int						g_currentGUI = GUI_MAIN_SCREEN;				//一个当前的GUI标识
bool					g_LMBDown = false;					 // GUI中的鼠标状态信息，鼠标左键是否按下的标识
int						g_MouseX = 0, g_MouseY = 0;					//存储鼠标坐标的两个变量


//各个函数声明------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
extern HWND Init_Window(HINSTANCE hInstance,int nShowCmd);
extern HRESULT Init_Direct3D(HWND hWnd);
extern INT Buffer();
extern VOID Direct3DRender();
VOID Direct3DClearUp();
//-------------------------------------------------------------------------------------------------

extern INT Everything(HWND hWnd,HINSTANCE hInstance);
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevinstance,
					LPSTR lpCmdLine,int nShowCmd)
{
	HWND hWnd;
	hWnd = Init_Window(hInstance,nShowCmd);						//初始化窗口，并初始化Direct3D		
	Everything(hWnd,hInstance);									//创建所有物体模型

	//消息循环
	MSG msg;	
	ZeroMemory(&msg,sizeof(msg));
	while (msg.message!=WM_QUIT)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);				//转换消息
			DispatchMessage(&msg);				//投递消息（将消息传递给相应窗口）
		}
		else
		{
			Direct3DRender();					//渲染
		}
	}

	//注销窗口类
	UnregisterClass(g_pClassName,wndclass.hInstance);
return 0;
}

/**************************
)口消息处理函数
**************************/
LRESULT CALLBACK WndProc(HWND hWnd,UINT message,
						WPARAM wParam,LPARAM lParam)
{
	switch (message)
	{
	//客户区重绘消息
	case WM_PAINT:
		ValidateRect(hWnd,NULL);								//更新客户区的显示
		break;
	//窗口销毁消息
	case WM_DESTROY:											//窗口销毁消息
		PostQuitMessage(0);										//退出程序
		Direct3DClearUp();
		break;

	case WM_LBUTTONDOWN:
		g_LMBDown = true;
		break;

	case WM_LBUTTONUP:
		g_LMBDown = false;
		break;

	case WM_MOUSEMOVE:
		g_MouseX = LOWORD (lParam);
		g_MouseY = HIWORD (lParam);
		break;
	}
	//默认的消息处理
	return DefWindowProc(hWnd,message,wParam,lParam);
}

VOID Direct3DClearUp()
{
	if(g_pd3dDevice)
		g_pd3dDevice->Release();									//释放Direct3D9Device设备接口
}
