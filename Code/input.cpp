# include "input.h"

ENTER::ENTER(HWND hwnd,HINSTANCE hinstance,REFGUID guid,LPCDIDATAFORMAT format):_guid(guid),_format(format)
{
	//����IDirectInput8�ӿڶ���
	LPDIRECTINPUT8 direct_input = NULL;
	HRESULT hr;
	hr = DirectInput8Create(
		hinstance,
		0x0800,						//ͨ������ȡ��ֵ
		IID_IDirectInput8,			//
		(void**)&direct_input,		//
		NULL);						//
	if(FAILED(hr))
	{
		::MessageBox(0,L"Create IDirectInput8 - FAILED",0,0);
	}
	 
	//����IDirectInputDevice8�ӿڶ���
	//_guid�ڳ�ʼ��ʱָ������껹�Ǽ���
	_input_device = NULL;
	hr = direct_input->CreateDevice(_guid,
		&_input_device,
		NULL);
	if(FAILED(hr))
	{
		direct_input->Release();
		::MessageBox(0,L"Create IDirectInputDevice8 - FAILED",0,0);
	}

	//����Э������					
	hr = _input_device->SetCooperativeLevel(hwnd,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if(FAILED(hr))								
	{
		direct_input->Release();
		_input_device->Release();
		::MessageBox(0,L"Create operator level - FAILED",0,0);
	}

	//�����豸���ݸ�ʽ
	hr = _input_device->SetDataFormat(_format);
	if(FAILED(hr))
	{
		direct_input->Release();
		_input_device->Release();
		::MessageBox(0,L"Create data format - FAILED",0,0);
	}

	//��ȡ�豸
	hr = _input_device->Acquire();
	_input_device->Poll();
}

ENTER::~ENTER()
{
	_input_device->Release();
}


//-----------------------------------------
//���
//-----------------------------------------
MOUSECLASS::MOUSECLASS(HWND hwnd,HINSTANCE hinstance):ENTER(hwnd,hinstance,GUID_SysMouse,&c_dfDIMouse)
{
}
MOUSECLASS::~MOUSECLASS()
{
	if(_input_device != NULL)
		_input_device->Unacquire();
}
BOOL MOUSECLASS::ReadInput()
{
	ZeroMemory(&_mouse_state,sizeof(_mouse_state));
	_input_device->Acquire();  
	//ע�⣬�ú����ɹ�ʱ���ᷴ��S_OK��������FAILED����
	HRESULT hr = _input_device->GetDeviceState(sizeof(_mouse_state), (LPVOID)&_mouse_state);	//��ȡ�豸����
	//��ȡ����������Ϣ
	_input_device->GetDeviceState( sizeof(_mouse_state),(LPVOID)&_mouse_state);
	return true;
}

//--------------------------------------
//����
//--------------------------------------
KEYBOARDCLASS::KEYBOARDCLASS(HWND hwnd,HINSTANCE hinstance):ENTER(hwnd,hinstance,GUID_SysKeyboard,&c_dfDIKeyboard)
{}
KEYBOARDCLASS::~KEYBOARDCLASS()
{
	if(_input_device != NULL)
		_input_device->Unacquire();
}
BOOL KEYBOARDCLASS::ReadInput()
{
	ZeroMemory(_key_state,sizeof(_key_state));
	_input_device->Poll();
	_input_device->Acquire();
	_input_device->GetDeviceState(sizeof(_key_state), (LPVOID)_key_state);			//��ȡ�豸����
	//��ȡ���������Ϣ
	_input_device->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)_key_state);
	return true;
}