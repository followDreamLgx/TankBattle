/****************************************
˵��
1.������������ϸ˵���Լ���ϸ�÷��ں����Ķ��崦��������ֻ˵���ú���������
2.Ϊ�˷�����ң����е�ȫ�ֱ�����winmain.cpp�ж��壬��Ҫ�õ���ʱ��������չ
3.Ϊ�˲�����ǩ��ʹ��Pn(֣����)��PLn(����)����ʽ��¼��ǩ
****************************************/
# include "head.h"										//����������ͷ�ļ�
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

wchar_t				*g_pClassName = L"7-25";							//��ʼ���������Լ��������ڵ�ʱ���õ�������һ��
wchar_t				*g_pWindowName = L"̹�˴�ս";						//��������
WNDCLASSEX			wndclass;											//ȫ�ֱ������˱����ڳ�ʼ�������Լ��������ڵ�ʱ��Ҫ�õ���
LPDIRECT3DDEVICE9	g_pd3dDevice = NULL;								//Direct3DDevice9�豸�ӿ�

CAMERA				*camera = NULL;
TERRAIN				*terrain = NULL;
SKYBOX				*sky = NULL;
ALPHA				*pAlphaMesh = NULL;
SNOW				*snow = NULL;									//ѩ���׷�
KEYBOARDCLASS		*keyboard = NULL;								//����
MOUSECLASS			*mouse = NULL;									//���
TANK				*tank_host = NULL;								//��̹��
TANK_ENEMY			*tank_enemy = NULL;								//�з�̹��		
TANK_GUN			*tank_gun = NULL;								//̹�˷�����ӵ�
GUN_CHILD			*gun_child = NULL;								//ʶ���ܵ��ӵ�
wchar_t				wall_name[6][50] = {{L"WoodCutter.X"},//
										{L"555.X"},			//
										{L"text222.X"},		//
										{L"text222.X"},		//
										{L"WoodCutter.X"},	//
										{L"TIGER_L.X"}		
														};			//ǽ�ڵ�x�ļ�
WALL				*wall = NULL;									//ǽ��
BILLBOARD			*billboard = NULL;								//�������
EXPLORE_BILLBOARD	*ex_board = NULL;								//�����Ĺ����
LPD3DXFONT TextStates = NULL;										//2d
bool				tree_or_explore = 1;				
RECT rect;															//���λ�õ�ȷ��
wchar_t a[3][50] = {	{L"�Ƿ������"},
						{L"�ǣ�Y��"},
						{L"��N��"}};
D3DGUIClass		*g_MainGUI = NULL;		//������
D3DGUIClass		*g_StartGUI = NULL;		//��Ϸ��ʼ����
D3DGUIClass		*g_LoadGUI = NULL;		//��Ϸ���봰��
D3DGUIClass		*g_OptionGUI = NULL;	//��Ϸ���ô���
int						g_MainGUIFontID = -1;						//  GUI����������ID
int						g_StartGUIFontID = -1;						//  GUI����������ID
int						g_LoadGUIFontID = -1;						//  GUI����������ID
int						g_OptionGUIFontID = -1;						//  GUI����������ID
int						g_currentGUI = GUI_MAIN_SCREEN;				//һ����ǰ��GUI��ʶ
bool					g_LMBDown = false;					 // GUI�е����״̬��Ϣ���������Ƿ��µı�ʶ
int						g_MouseX = 0, g_MouseY = 0;					//�洢����������������


//������������------------------------------------------------------------------------------------
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
	hWnd = Init_Window(hInstance,nShowCmd);						//��ʼ�����ڣ�����ʼ��Direct3D		
	Everything(hWnd,hInstance);									//������������ģ��

	//��Ϣѭ��
	MSG msg;	
	ZeroMemory(&msg,sizeof(msg));
	while (msg.message!=WM_QUIT)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);				//ת����Ϣ
			DispatchMessage(&msg);				//Ͷ����Ϣ������Ϣ���ݸ���Ӧ���ڣ�
		}
		else
		{
			Direct3DRender();					//��Ⱦ
		}
	}

	//ע��������
	UnregisterClass(g_pClassName,wndclass.hInstance);
return 0;
}

/**************************
)����Ϣ������
**************************/
LRESULT CALLBACK WndProc(HWND hWnd,UINT message,
						WPARAM wParam,LPARAM lParam)
{
	switch (message)
	{
	//�ͻ����ػ���Ϣ
	case WM_PAINT:
		ValidateRect(hWnd,NULL);								//���¿ͻ�������ʾ
		break;
	//����������Ϣ
	case WM_DESTROY:											//����������Ϣ
		PostQuitMessage(0);										//�˳�����
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
	//Ĭ�ϵ���Ϣ����
	return DefWindowProc(hWnd,message,wParam,lParam);
}

VOID Direct3DClearUp()
{
	if(g_pd3dDevice)
		g_pd3dDevice->Release();									//�ͷ�Direct3D9Device�豸�ӿ�
}
