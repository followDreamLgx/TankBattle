# include "global_value.h"
# include "definition.h"



//�����˹�Դ��û�����ö��㷨�ߣ�

void light(int swi)
{
	g_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE,TRUE);			//�������淴��
	D3DLIGHT9 light;
	ZeroMemory(&light,sizeof(light));
	D3DXVECTOR3 X(100.0,0.0,0.0),_X(-100.0,0.0,0.0);
	D3DXVECTOR3 Y(0.0,100.0,0.0),_Y(0.0,-100.0,0.0);
	D3DXVECTOR3 Z(0.0,0.0,100.0),_Z(0.0,0.0,-100.0);
	if(swi <= 3)
	{
		if(swi == 1)														
		{
			light.Type = D3DLIGHT_POINT;									//�������Դ
			light.Ambient = D3DXCOLOR(0.8f,0.8f,0.8f,1.0f);					//���淴���ǿ��
			light.Diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);					//�������ǿ��	
			light.Specular = D3DXCOLOR(0.3f,0.3f,0.3f,1.0f);				//���淴���ǿ��
			light.Position = 3 * X + Z;										//��Դλ��
			light.Attenuation0 = 0.10f;										//˥��ϵ��
			light.Attenuation1 = 0.0f;										
			light.Attenuation2 = 0.0f;
			light.Range = 300000.0f;											//���շ�Χ
		}
		else if(swi == 2)													
		{
			light.Type =	D3DLIGHT_DIRECTIONAL;							//���������,û��˥��ϵ����û�й�Դλ�ã�Ҳû�й��շ�Χ
			light.Ambient = D3DXCOLOR(0.8f,0.8f,0.8f,1.0f);					//���淴���ǿ��
			light.Diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);					//�������ǿ��	
			light.Specular = D3DXCOLOR(0.3f,0.3f,0.3f,1.0f);				//���淴���ǿ��
			light.Direction = -3 * X - 3 * Z;
		}
		else if(swi == 3)													//�����۹��
		{
			light.Type =	D3DLIGHT_SPOT;									//�������Դ
			light.Ambient = D3DXCOLOR(0.8f,0.8f,0.8f,1.0f);					//���淴���ǿ��
			light.Diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);					//�������ǿ��	
			light.Specular = D3DXCOLOR(0.3f,0.3f,0.3f,1.0f);				//���淴���ǿ��
			light.Position = -Y * 3;												//��Դλ��
			light.Direction = Y;											//��Դ����
			light.Attenuation0 = 1.0f;										//˥��ϵ��
			light.Attenuation1 = 0.0f;										
			light.Attenuation2 = 0.0f;
			light.Range = 300000.0f;										//���շ�Χ
			light.Falloff = 0.1f;											//����ϵ��
			light.Phi = D3DX_PI / 2.0f;										//��׶���С
			light.Theta = D3DX_PI / 4.0f;									//��׵���С
		}
		g_pd3dDevice->SetRenderState(D3DRS_LIGHTING,true);					//����������Ⱦģʽ
		g_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS,true);			//
		g_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE,true);			//
		g_pd3dDevice->SetLight(0,&light);									//���ù�Դ
		g_pd3dDevice->LightEnable(0,true);									//���ù�Դ
		g_pd3dDevice->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_XRGB(50,50,50));
	}
}