# include "mirror_shadow.h"


//���ñ���ʹ�ó�ʼ��
MIRROR_SHADOW::MIRROR_SHADOW(LPDIRECT3DDEVICE9 device,ID3DXMesh *mesh_d3d,ID3DXMesh *wall)
{
	_device = device;
	_mesh_d3d = mesh_d3d;
	_wall = wall;
}
VOID MIRROR_SHADOW::DrawMirror(D3DXMATRIX &world_actor,LPDIRECT3DTEXTURE9 *textures,INT num,
					D3DXMATRIX &world_wall,D3DXPLANE &plane)
{
	_device->SetRenderState(D3DRS_STENCILENABLE,true);					//����ģ�����
	//ģ��ȽϺ������ǳɹ��ģ��ڽ������Ļ����������ܹ�ͨ��ģ�����
	_device->SetRenderState(D3DRS_STENCILFUNC,D3DCMP_ALWAYS);
	_device->SetRenderState(D3DRS_STENCILREF,0x1);						//REF����Ϊ1
	_device->SetRenderState(D3DRS_STENCILMASK,0xffffffff);				//����MASK
	_device->SetRenderState(D3DRS_STENCILWRITEMASK,0xffffffff);
	//��ģ����Գɹ�ʱ��ģ��ο�ֵ(����ָ��Ϊ0x1)
	_device->SetRenderState(D3DRS_STENCILPASS,D3DSTENCILOP_REPLACE);	//�����ģ�嶼�ɹ�����ģ��ֵ����Ϊ0x1
	_device->SetRenderState(D3DRS_STENCILFAIL,D3DSTENCILOP_KEEP);		//ģ�����ʧ�ܣ����ı�ģ�建��
	_device->SetRenderState(D3DRS_STENCILZFAIL,D3DSTENCILOP_KEEP);		//��Ȳ���ʧ�ܣ����ı����
	//���ƾ���
	_device->SetRenderState(D3DRS_ZWRITEENABLE,false);					//�ر���Ȼ���д����
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);				//����alpha�ں�
	//ͨ������������������ʵ��ģ�建��ı䵫��̨���治�ı�
	_device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ZERO);				//Դ�ں�����
	_device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);				//Ŀ���ں�����
	//����ģ�建��
	_device->SetTransform(D3DTS_WORLD,&world_wall);
	_wall->DrawSubset(0);
	
	_device->SetRenderState(D3DRS_ZWRITEENABLE,true);					//���¿���������Ȼ���д����
	//1. ���Ե����ص�ģ��ֵ����ģ�建���е�ֵʱ�ɹ�
	_device->SetRenderState(D3DRS_STENCILFUNC,D3DCMP_EQUAL);
	//2. ��Ȼ�����ģ�建����Զ�ͨ��ʱ���ı�ģ�建�棬�����߾�����ֻ�е�ģ�建���е���ֵ����1ʱ���ܲ��Գɹ�				
	_device->SetRenderState(D3DRS_STENCILPASS,D3DSTENCILOP_KEEP);
	_device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_DESTCOLOR);
	_device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
	//����������ľ�����ȴ��ھ��棬�����ͼԪ�ڵ��˾��񣬱��뽫�����Ȼ�����գ������޷���������
	_device->Clear(0,0,D3DCLEAR_ZBUFFER,0,1.0f,0);	
	//�����徵���뾵����ںϷ���
	_device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_DESTCOLOR);
	_device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);

	D3DXMATRIX X,mul;
	D3DXMatrixReflect(&X,&plane);									//����任�ľ���
	mul = world_actor * X;
	_device->SetTransform(D3DTS_WORLD,&mul);
	_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);			//��������˳ʱ������
	for(int i = 0;i < num;i++)
	{
		_device->SetTexture(0,textures[i]);
		_mesh_d3d->DrawSubset(i);
	}
	_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			//����������ʱ������
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE,false);			//�ر�Alpha�ں�
	_device->SetRenderState(D3DRS_STENCILENABLE,false);				//����ģ�建�棬
}
//������Ӱ�ĳ�Ա����
//���봫����������������꣬��������������ָ�룬���������
//ͶӰƽ�棬��Դ���򣨷������߾۹���������ĵ��ĸ�����������1Ϊ���Դ��0Ϊ����⣩

VOID MIRROR_SHADOW::DrawShadow(D3DXMATRIX &world_actor,LPDIRECT3DTEXTURE9 *textures,INT num,
				D3DXPLANE &plane,D3DXVECTOR4 &light)
{
	_device->SetRenderState(D3DRS_STENCILENABLE,    true);						//��ģ�建��
    _device->SetRenderState(D3DRS_STENCILFUNC,      D3DCMP_EQUAL);				//ģ����Ժ���ֻ�����ʱ��ͨ��
    _device->SetRenderState(D3DRS_STENCILREF,       0x0);						//ģ�建���Ѿ����㣬����Ϊ0 
																				//���Ա�֤�״�������Ӱд����ͨ��
	_device->SetRenderState(D3DRS_STENCILMASK,      0xffffffff);
    _device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	_device->SetRenderState(D3DRS_STENCILZFAIL,     D3DSTENCILOP_KEEP);
    _device->SetRenderState(D3DRS_STENCILFAIL,      D3DSTENCILOP_KEEP);				
    _device->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_INCR);		//ͨ����ģ������Ȼ��棬ģ�建���е�ֵ������
																				//���ӵĽ���ǵ���ģ�建���ֵ��Ϊ0������ͨ��ģ�����
																				//������ͬһ���ط�������ֻ��ͨ��һ�Σ���ֹ�����ں�
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);					//��alpha�ں�
	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	_device->SetRenderState(D3DRS_ZENABLE, false);

	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl,sizeof(D3DMATERIAL9));
	mtrl.Diffuse = D3DXCOLOR(0,0,0,0.5f);
	mtrl.Ambient = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	mtrl.Specular = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	mtrl.Emissive = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	_device->SetMaterial(&mtrl);

	D3DXMATRIX s;
	D3DXMatrixShadow(&s,&light,&plane);
	world_actor = world_actor * s;
	_device->SetTransform(D3DTS_WORLD,&world_actor);
	for(int i = 0;i < 4;i++)
	{
	//	_device->SetTexture(0,textures[i]);
		_mesh_d3d->DrawSubset(i);
	}
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE,false);			//�ر�Alpha�ں�
	_device->SetRenderState(D3DRS_STENCILENABLE,false);				//����ģ�建�棬
	_device->SetRenderState(D3DRS_ZENABLE,true);
}