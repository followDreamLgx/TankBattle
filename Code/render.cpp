# include "head.h"
# include "definition.h"
# include "global_value.h"


int select();
extern void light(int swi);													//光照
void environment();															//绘制所有环境
void draw_tank_gun();
extern void GUICallback(int id, int state);

VOID Direct3DRender()
{
	//先将键盘记录清空
	camera->View();
	g_pd3dDevice->Clear(0,NULL,D3DCLEAR_TARGET								//后台
		| D3DCLEAR_ZBUFFER,													//深度
		D3DCOLOR_XRGB(50,100,150),1.0f,
		0);																	//模板缓存设置为0
	g_pd3dDevice->BeginScene();							
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);	//设置渲染状态(逆时针消隐）

	if(g_currentGUI == GUI_MAIN_SCREEN)
		ProcessGUI(g_MainGUI, g_LMBDown, g_MouseX,
		g_MouseY, GUICallback);
	else if(g_currentGUI == GUI_START_SCREEN)
	{
		draw_tank_gun();
		environment();								//绘制所有环境
		gun_child->explore(camera->GetView());
		ProcessGUI(g_StartGUI, g_LMBDown, g_MouseX,
		g_MouseY, GUICallback);
	}
	else if(g_currentGUI == GUI_LOAD_SCREEN)
	{
		draw_tank_gun();
		environment();								//绘制所有环境
		gun_child->explore(camera->GetView());
		ProcessGUI(g_LoadGUI, g_LMBDown, g_MouseX,
		g_MouseY, GUICallback);
	}
	else if(g_currentGUI == GUI_OPTION_SCREEN)
	{	
		draw_tank_gun();
		environment();								//绘制所有环境
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
			NULL,															//指定字符串所属的ID3DXSprite接口，用于帮助实现2D渲染，可设为NULL
			a[i],															//输出的字符串，长度由下一个参数决定
			-1,																//字符创长度，-1表示全部输出
			&rect,															//指定字符串的矩形区域
			DT_SINGLELINE | DT_NOCLIP | DT_LEFT,							//指定字符串在矩形区域中的位置
			D3DXCOLOR(0.0f,0.5f,1.0f,1.0f));	
			rect.top;
			rect.top += 50;
		}
		rect.top  = j;
		if(keyboard->ReadInput())
		{	if(keyboard->_key_state[DIK_Y] & 0x80)		//Y
			{	
				tank_host->_life = tank_host->_life_time;
				tank_host->_mul = 10;						//恢复坦克的初始速度
				wall->_boss._alive = false;
				wall->_boss._life = 0;
				wall->_boss._position.x = 60000;
			}
		}
	}
	else
	{	tank_host->render();						//绘制坦克
		select();
		wchar_t *a = NULL;
		char b[15] = {"RP:"};
		int i,ch_wc;
		for(i = 1;(int)tank_host->_life / i != 0;i *= 10);	//这里有个分号
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
			NULL,															//指定字符串所属的ID3DXSprite接口，用于帮助实现2D渲染，可设为NULL
			a,																//输出的字符串，长度由下一个参数决定
			-1,																//字符创长度，-1表示全部输出
			&rect,															//指定字符串的矩形区域
			DT_SINGLELINE | DT_NOCLIP | DT_LEFT,							//指定字符串在矩形区域中的位置
			D3DXCOLOR(1.0f,0.0f,0.0f,1.0f));	
		delete []a;
	}
	tank_enemy->render();						//绘制敌方坦克
	for(int i = 0;i < 8;i++)
	{
		if(GetRandomFloat(0,1000) > 950)						//产生子弹的几率为1 / 10
			gun_child->add(tank_enemy->_imfor[i]._position,
			tank_enemy->_imfor[i]._look,tank_enemy->_imfor[i]._rotation,NAZI);
	}
	//增加boss的子弹
	if(wall->_boss._alive == true)
		if(GetRandomFloat(0,1000) > 950)						
			gun_child->add(wall->_boss._position,wall->_boss._velocity,wall->_boss._rotation,BOSS);

	gun_child->update(tank_host,tank_enemy->_imfor);
	gun_child->render();
	//绘制墙体以及第三方坦克
	wall->test(tank_enemy->_imfor,gun_child,tank_host);
	wall->update(gun_child,tank_enemy,tank_host);
	wall->render();

}
void environment()
{
	static int i = 0;
	terrain->DrawTerrain();										//绘制地形
	sky->DrawSkyBox(camera->Position());						//绘制天空	
	//绘制树木
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
//光照选择
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


		if(keyboard->_key_state[DIK_LEFTARROW] & 0x80)		//左
			camera->TranslationRight(-3);
		if(keyboard->_key_state[DIK_RIGHTARROW] & 0x80)		//右
			camera->TranslationRight(3);
		if(keyboard->_key_state[DIK_UPARROW] & 0x80)		//上
			camera->TranslationLook(1);
		if(keyboard->_key_state[DIK_DOWNARROW] & 0x80)		//下
			camera->TranslationLook(-1);

		//光照
		if(keyboard->_key_state[DIK_NUMPAD1] & 0x80)				//1
			light(1);
		if(keyboard->_key_state[DIK_NUMPAD2] & 0x80)				//2
			light(2);
		if(keyboard->_key_state[DIK_NUMPAD3] & 0x80)				//3
			light(3);
	}
	if(mouse->ReadInput())
	{	if(mouse->_mouse_state.rgbButtons[0] & 0x80)				//鼠标左键
		{
			static int num = 0;
			if(num == 0)
				gun_child->add(tank_host->_position,tank_host->_look,tank_host->_rotation);
			else 
				num++;
			if(num == 200)
				num = 0;
		}
		if(mouse->_mouse_state.rgbButtons[1] & 0x80)				//鼠标右键
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
		if(keyboard->_key_state[DIK_LSHIFT] & 0x80)			//左SHIFT
			camera->RotationTargetRight(D3DX_PI / (-500.0));
		if(keyboard->_key_state[DIK_LCONTROL] & 0x80)		//左CTRL
			camera->RotationTargetRight(D3DX_PI / (-500.0));
		if(keyboard->_key_state[DIK_RSHIFT] & 0x80)			//右SHIFT
			camera->RotationTargetRight(D3DX_PI / (-500.0));
		if(keyboard->_key_state[DIK_RCONTROL] & 0x80)		//右CTRL
			camera->RotationTargetRight(D3DX_PI / (-500.0));
		*/