# include "head.h"
# include "definition.h"
# include "global_value.h"


int select();
extern void light(int swi);													//����
void environment();															//�������л���
void draw_tank_gun();
extern void GUICallback(int id, int state);

VOID Direct3DRender()
{
	//�Ƚ����̼�¼���
	camera->View();
	g_pd3dDevice->Clear(0,NULL,D3DCLEAR_TARGET								//��̨
		| D3DCLEAR_ZBUFFER,													//���
		D3DCOLOR_XRGB(50,100,150),1.0f,
		0);																	//ģ�建������Ϊ0
	g_pd3dDevice->BeginScene();							
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);	//������Ⱦ״̬(��ʱ��������

	if(g_currentGUI == GUI_MAIN_SCREEN)
		ProcessGUI(g_MainGUI, g_LMBDown, g_MouseX,
		g_MouseY, GUICallback);
	else if(g_currentGUI == GUI_START_SCREEN)
	{
		draw_tank_gun();
		environment();								//�������л���
		gun_child->explore(camera->GetView());
		ProcessGUI(g_StartGUI, g_LMBDown, g_MouseX,
		g_MouseY, GUICallback);
	}
	else if(g_currentGUI == GUI_LOAD_SCREEN)
	{
		draw_tank_gun();
		environment();								//�������л���
		gun_child->explore(camera->GetView());
		ProcessGUI(g_LoadGUI, g_LMBDown, g_MouseX,
		g_MouseY, GUICallback);
	}
	else if(g_currentGUI == GUI_OPTION_SCREEN)
	{	
		draw_tank_gun();
		environment();								//�������л���
		gun_child->explore(camera->GetView());
		ProcessGUI(g_OptionGUI, g_LMBDown, g_MouseX,
		g_MouseY, GUICallback);
	}
	else
		ProcessGUI(g_MainGUI, g_LMBDown, g_MouseX,
		g_MouseY, GUICallback);

	g_pd3dDevice->EndScene();
	g_pd3dDevice->Present(NULL,NULL,NULL,NULL);
}
void draw_tank_gun()
{
	if(tank_host->_life <= 0)
	{
		long j = rect.top;
		for(int i = 0;i < 3;i++)
		{
			TextStates->DrawText(												//P279
			NULL,															//ָ���ַ���������ID3DXSprite�ӿڣ����ڰ���ʵ��2D��Ⱦ������ΪNULL
			a[i],															//������ַ�������������һ����������
			-1,																//�ַ������ȣ�-1��ʾȫ�����
			&rect,															//ָ���ַ����ľ�������
			DT_SINGLELINE | DT_NOCLIP | DT_LEFT,							//ָ���ַ����ھ��������е�λ��
			D3DXCOLOR(0.0f,0.5f,1.0f,1.0f));	
			rect.top;
			rect.top += 50;
		}
		rect.top  = j;
		if(keyboard->ReadInput())
		{	if(keyboard->_key_state[DIK_Y] & 0x80)		//Y
			{	
				tank_host->_life = tank_host->_life_time;
				tank_host->_mul = 10;						//�ָ�̹�˵ĳ�ʼ�ٶ�
				wall->_boss._alive = false;
				wall->_boss._life = 0;
				wall->_boss._position.x = 60000;
			}
		}
	}
	else
	{	tank_host->render();						//����̹��
		select();
		wchar_t *a = NULL;
		char b[15] = {"RP:"};
		int i,ch_wc;
		for(i = 1;(int)tank_host->_life / i != 0;i *= 10);	//�����и��ֺ�
		for(int j = 3;i > 1;j++)
		{
			ch_wc = (int)tank_host->_life % i - (int)tank_host->_life % (i / 10);
			i /= 10;
			b[j] = ch_wc / i + 48;
		}
		DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, b, -1, NULL, 0);
		a = new wchar_t[dwNum];
		MultiByteToWideChar (CP_ACP, 0, b,(strlen(b) + 1), a, dwNum);

		TextStates->DrawText(												//P279
			NULL,															//ָ���ַ���������ID3DXSprite�ӿڣ����ڰ���ʵ��2D��Ⱦ������ΪNULL
			a,																//������ַ�������������һ����������
			-1,																//�ַ������ȣ�-1��ʾȫ�����
			&rect,															//ָ���ַ����ľ�������
			DT_SINGLELINE | DT_NOCLIP | DT_LEFT,							//ָ���ַ����ھ��������е�λ��
			D3DXCOLOR(1.0f,0.0f,0.0f,1.0f));	
		delete []a;
	}
	tank_enemy->render();						//���Ƶз�̹��
	for(int i = 0;i < 8;i++)
	{
		if(GetRandomFloat(0,1000) > 950)						//�����ӵ��ļ���Ϊ1 / 10
			gun_child->add(tank_enemy->_imfor[i]._position,
			tank_enemy->_imfor[i]._look,tank_enemy->_imfor[i]._rotation,NAZI);
	}
	//����boss���ӵ�
	if(wall->_boss._alive == true)
		if(GetRandomFloat(0,1000) > 950)						
			gun_child->add(wall->_boss._position,wall->_boss._velocity,wall->_boss._rotation,BOSS);

	gun_child->update(tank_host,tank_enemy->_imfor);
	gun_child->render();
	//����ǽ���Լ�������̹��
	wall->test(tank_enemy->_imfor,gun_child,tank_host);
	wall->update(gun_child,tank_enemy,tank_host);
	wall->render();

}
void environment()
{
	static int i = 0;
	terrain->DrawTerrain();										//���Ƶ���
	sky->DrawSkyBox(camera->Position());						//�������	
	//������ľ
	if(tree_or_explore == 1)
	{	billboard->render(camera->GetView());
		i = GetRandomFloat(500,2000);
	}
	else
	{
		ex_board->test(tank_enemy->_imfor,tank_host,wall);
		ex_board->render(camera->GetView());
		i--;
		if(i <= 0)
			tree_or_explore = ! tree_or_explore;
	}
	snow->update(1);
	snow->render();
}
//����ѡ��
int select()
{
	if(keyboard->ReadInput())
	{
		if(keyboard->_key_state[DIK_A] & 0x80)		//A
			tank_host->TurnLeft(*camera);
		if(keyboard->_key_state[DIK_D] & 0x80)		//D
			tank_host->TurnRight(*camera);

		if(keyboard->_key_state[DIK_W] & 0x80)		//W
			tank_host->TranslationFront(*camera);
		if(keyboard->_key_state[DIK_S] & 0x80)		//S
			tank_host->TranslationBack(*camera);

		if(keyboard->_key_state[DIK_R] & 0x80)		//R
			camera->TranslationUp(3);
		if(keyboard->_key_state[DIK_F] & 0x80)		//F
			camera->TranslationUp(-3);


		if(keyboard->_key_state[DIK_LEFTARROW] & 0x80)		//��
			camera->TranslationRight(-3);
		if(keyboard->_key_state[DIK_RIGHTARROW] & 0x80)		//��
			camera->TranslationRight(3);
		if(keyboard->_key_state[DIK_UPARROW] & 0x80)		//��
			camera->TranslationLook(1);
		if(keyboard->_key_state[DIK_DOWNARROW] & 0x80)		//��
			camera->TranslationLook(-1);

		//����
		if(keyboard->_key_state[DIK_NUMPAD1] & 0x80)				//1
			light(1);
		if(keyboard->_key_state[DIK_NUMPAD2] & 0x80)				//2
			light(2);
		if(keyboard->_key_state[DIK_NUMPAD3] & 0x80)				//3
			light(3);
	}
	if(mouse->ReadInput())
	{	if(mouse->_mouse_state.rgbButtons[0] & 0x80)				//������
		{
			static int num = 0;
			if(num == 0)
				gun_child->add(tank_host->_position,tank_host->_look,tank_host->_rotation);
			else 
				num++;
			if(num == 200)
				num = 0;
		}
		if(mouse->_mouse_state.rgbButtons[1] & 0x80)				//����Ҽ�
		{
		
		}
		if(mouse->_mouse_state.lZ & 0x80)
		{
		
		}		
	}		
return 0;
}






		/*
		if(keyboard->_key_state[DIK_J] & 0x80)				//J
			camera->RotationTargetRight(D3DX_PI / (-500.0));
		if(keyboard->_key_state[DIK_K] & 0x80)				//K
			camera->RotationTargetRight(D3DX_PI / (-500.0));
		if(keyboard->_key_state[DIK_LSHIFT] & 0x80)			//��SHIFT
			camera->RotationTargetRight(D3DX_PI / (-500.0));
		if(keyboard->_key_state[DIK_LCONTROL] & 0x80)		//��CTRL
			camera->RotationTargetRight(D3DX_PI / (-500.0));
		if(keyboard->_key_state[DIK_RSHIFT] & 0x80)			//��SHIFT
			camera->RotationTargetRight(D3DX_PI / (-500.0));
		if(keyboard->_key_state[DIK_RCONTROL] & 0x80)		//��CTRL
			camera->RotationTargetRight(D3DX_PI / (-500.0));
		*/