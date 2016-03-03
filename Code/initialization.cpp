# include "head.h"
# include "global_value.h"									//����������ȫ�ֱ�������չ
# include "definition.h"
#include <tchar.h>



//�ⲿ������������չ
extern LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);			//��Ϣ������

//���ļ��к�������-------------------------------------------------------------
//1.��ʼ��direct3d
HRESULT Init_Direct3D(HWND hWnd);		
//����ĸ�������
extern INT Buffer();


//���ڳ�ʼ������
//7-25
//��ʼ������
//����������������WinMain���β�
//�����˳�ʼ��HWND��������ڣ�ע�ᴰ�ڣ���ʾ���´���
HWND Init_Window(HINSTANCE hInstance,int nShowCmd)
{
	//������������Ѿ��ı�//////
	//��ʼ�������࣬���P10			
	wndclass.cbSize = sizeof( WNDCLASSEX ) ;
	wndclass.cbClsExtra = 0;												//����ָ��Ӧ�ó��򴰿ڵ���Ϣ��������ͨ�������ã���Ϊ0
	wndclass.cbWndExtra = 0;												//������ͬ
	wndclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);			//����ָ����ʾ����ʱ�ͻ����ı���ɫ��Ĭ��Ϊ��ɫ
	wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);							//ָ�������״
	wndclass.hIcon=(HICON)::LoadImage(NULL,_T("icon.ico"),IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);
					///												//��ȫ�ֵ�::LoadImage�����ӱ��ؼ����Զ���icoͼ��
	wndclass.hInstance = hInstance;											//����Ӧ�ó���ʵ���ľ����ͨ����winmain�е���
	wndclass.lpfnWndProc = WndProc;											//����ָ��Ӧ�ó��򴰿ڵ���Ϣ������
	wndclass.lpszClassName = g_pClassName;									//���������ṹ������
	wndclass.lpszMenuName = NULL;											//���ڲ˵�������
	wndclass.style = CS_HREDRAW | CS_VREDRAW;								//ָ��������ʽ�������߿򣬱���������������
	
	//ע�ᴰ���࣬���P11
	if(::RegisterClassEx(&wndclass) == 0)
		return 0 ;
	
	//�������ڣ����P11
	HWND hWnd = CreateWindow(
		g_pClassName,								//��������
		g_pWindowName,								//���ڱ���
		WS_OVERLAPPEDWINDOW,						//������ʽ				///
		CW_USEDEFAULT,								//�������x��λ��		///
		CW_USEDEFAULT,								//�������y��λ��		///
		SCREEN_WIDTH,								//���ڵĿ��
		SCREEN_HEIGHT,								//���ڵĸ߶�
		NULL,										//�����ڵĻ���
		NULL,										//���ڵĲ˵�����
		hInstance,									//Ӧ�ó���ʵ������
		NULL);										//�������ڵĲ���
	
	//��ʾ���ڣ����´���
	MoveWindow(hWnd,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,true);
	ShowWindow(hWnd,nShowCmd);
	UpdateWindow(hWnd);	
	Init_Direct3D(hWnd);							//��ʼ��direct3d


return hWnd;
}

//��ʼ��direct3D
//����ΪWinMain���βε���
HRESULT Init_Direct3D(HWND hWnd)
{
	//����COM�ӿڣ�IDirect3D9��ָ��
	LPDIRECT3D9 pD3D = NULL;
	if( (pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return E_FAIL;

	D3DCAPS9 caps;									
	int sign = 0;

	//��ȡӲ����Ϣ������Ӳ��ʱ��֧��Ӳ������,��ȡ�����Ϣ��Ҫ��¼�������ӿڵ�ʱ����Ҫ�õ�
	pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&caps);
	if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		sign = D3DCREATE_HARDWARE_VERTEXPROCESSING;			//֧��Ӳ����������
	else
		sign = D3DCREATE_SOFTWARE_VERTEXPROCESSING;			//��֧�֣����������������

	//����Direct3d�豸�ӿ�	��ϸ���� P34
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp,sizeof(d3dpp));								//���ڴ�����
	d3dpp.BackBufferWidth = SCREEN_WIDTH;							//��̨�������Ŀ��
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//��̨�������ĸ߶�
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;						//��̨�������������صĸ�ʽ���Ⱥ��ұ���һ��ö������
	d3dpp.BackBufferCount = 1;										//��̨������������
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;					//���ز���������
	d3dpp.MultiSampleQuality = 0;									//���ز����ĸ�ʽ
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						//ָ����̨��������������θ��Ƶ�ǰ̨��������ͨ����Ϊ��ֵ��
																	//��ʾ����̨���������Ƶ�ǰ̨ʱ�����̨����������
	d3dpp.hDeviceWindow = hWnd;	
	d3dpp.Windowed = true;											//ָ������ͼ�εĴ��ھ��
	d3dpp.EnableAutoDepthStencil = true;							//��ʾDirect3D�Ƿ�ΪӦ�ó����Զ�������Ȼ��棬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;					//Ϊǰ�߳�Աָ����Ȼ�������ظ�ʽ
	d3dpp.Flags = 0;												//��ʾ��������
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;		//����ָ��ƽ��ķ�תģʽ������ģʽ�£�ȡֵΪ0
	
	//����IDirect3DDevice9�ӿڶ���		P31���� P35���
	pD3D->CreateDevice(D3DADAPTER_DEFAULT,					//�Կ�����
					   D3DDEVTYPE_HAL,						
					   hWnd,								
					   sign,									//ָ��Direct3D�豸����3D����ķ�ʽ
					   &d3dpp,
					   &g_pd3dDevice);

	pD3D->Release();											//�ͷ�Direct3D�ӿ�
return S_OK;
}