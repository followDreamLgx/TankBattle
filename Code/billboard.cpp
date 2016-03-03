# include "billboard.h"
# include "tank.h"
# include "wall.h"

BILLBOARD::BILLBOARD(LPDIRECT3DDEVICE9 device,const BOUNDINGBOX &box,wchar_t *name):_device(device),
	_random((int)GetRandomFloat(50,80))
{
	_x = new float[_random];
	_z = new float[_random];
	for(int i = 0;i < _random;i++)
	{
		_x[i] = GetRandomFloat(box._min.x,box._max.x);
		_z[i] = GetRandomFloat(box._min.z,box._max.z);
	}
	init(name);
}
BILLBOARD::BILLBOARD(LPDIRECT3DDEVICE9 device,wchar_t *name):_device(device),
	_random((int)GetRandomFloat(20,30))
{
	init(name);
}
BILLBOARD::BILLBOARD(LPDIRECT3DDEVICE9 device,const BOUNDINGBOX &box):_device(device)
	,_random((int)GetRandomFloat(20,30))
{
	_x = new float[_random];
	_z = new float[_random];
	for(int i = 0;i < _random;i++)
	{
		_x[i] = GetRandomFloat(box._min.x,box._max.x);
		_z[i] = GetRandomFloat(box._min.z,box._max.z);
	}
}
void BILLBOARD::init(wchar_t *name)
{
	init();
	//加载纹理
	D3DXCreateTextureFromFile(_device,name,&_texture);
}
void BILLBOARD::init()
{
	_device->CreateVertexBuffer(6 * sizeof(TERRAINVERTEX),
		0,
		D3DFVF_TERRAINVERTEX,
		D3DPOOL_MANAGED,
		&_vertex,
		0);
	TERRAINVERTEX *pVertices = NULL;
	_vertex->Lock(0,0,(void**)&pVertices,0);

	pVertices[C] = TERRAINVERTEX(-30,60,0,0,0);
	pVertices[B] = TERRAINVERTEX(-30,0,0,0,1);
	pVertices[A] = TERRAINVERTEX(30,0,0,1,1);

	pVertices[F] = TERRAINVERTEX(-30,60,0,0,0);
	pVertices[E] = TERRAINVERTEX(30,0,0,1,1);
	pVertices[D] = TERRAINVERTEX(30,60,0,1,0);
	_vertex->Unlock();
}
void BILLBOARD::render(D3DXMATRIX &view)
{
	D3DXMATRIX billboard,tran,world,scale;
    D3DXMatrixIdentity(&billboard);
    billboard._11 = view._11;
    billboard._13 = view._13;
    billboard._31 = view._31;
    billboard._33 = view._33;
    D3DXMatrixInverse(&billboard, NULL, &billboard);
	D3DXMatrixScaling(&scale,5.0f,10.0f,5.0f);

	_device->SetTexture(0,_texture);
	_device->SetStreamSource(0,_vertex,0,sizeof(TERRAINVERTEX));
	_device->SetFVF(D3DFVF_TERRAINVERTEX);

	_device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);				//打开Alpha融合
	_device->SetRenderState(D3DRS_LIGHTING,false);
	//正式绘制
	for(int i = 0;i < _random;i++)
	{
		D3DXMatrixTranslation(&tran,_x[i],20,_z[i]);
		D3DXMatrixIdentity(&world);
		world = world * billboard  * scale * tran;
		_device->SetTransform(D3DTS_WORLD,&world);
		_device->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);
	}
	_device->SetRenderState(D3DRS_LIGHTING,true);
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE,false);				//关闭Alpha融合
}
void BILLBOARD::render()
{
	_device->SetStreamSource(0,_vertex,0,sizeof(TERRAINVERTEX));
	_device->SetFVF(D3DFVF_TERRAINVERTEX);

	_device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);				//打开Alpha融合
	_device->SetRenderState(D3DRS_LIGHTING,false);
	_device->SetRenderState (D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	_device->SetRenderState (D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	//正式绘制
	_device->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);
	_device->SetRenderState(D3DRS_LIGHTING,true);
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE,false);				//关闭Alpha融合
}
BILLBOARD::~BILLBOARD()
{}


//-----------------------------------
//进击的森林
//-----------------------------------
EXPLORE_BILLBOARD::EXPLORE_BILLBOARD(LPDIRECT3DDEVICE9 device,const BOUNDINGBOX &box)
	:BILLBOARD(device,box),
	_render_life_amount(24),
	_render_add(1),
	_distance(10000)
{
	_render_life = 0;
	BILLBOARD::init();
	init();
}

void EXPLORE_BILLBOARD::init()
{
	wchar_t explore[25][20] = {{L"explode00.dds"},{L"explode01.dds"},{L"explode02.dds"},{L"explode03.dds"},
	{L"explode04.dds"},{L"explode05.dds"},{L"explode06.dds"},{L"explode07.dds"},{L"explode08.dds"},
	{L"explode09.dds"},{L"explode10.dds"},{L"explode11.dds"},{L"explode12.dds"},{L"explode13.dds"},
	{L"explode14.dds"},{L"explode15.dds"},{L"explode16.dds"},{L"explode17.dds"},{L"explode18.dds"},
	{L"explode19.dds"},{L"explode20.dds"},{L"explode21.dds"},{L"explode22.dds"},{L"explode23.dds"},
	{L"explode24.dds"}};

	for(int i = 0;i < 25;i++)			//创建纹理
		D3DXCreateTextureFromFile(BILLBOARD::_device,explore[i],&_texture_ex[i]);  
}
void EXPLORE_BILLBOARD::test(PROPERTY *imfor,TANK *host,WALL *wall)
{
	//遍历爆炸记录爆炸地点的数组
	for(int i = 0;i < _random;i++)
	{
		//地方坦克撞上了爆炸中的广告牌
		for(int j = 0;j < 8;j++)
		{
			if((imfor[j]._position.x - _x[i]) * (imfor[j]._position.x - _x[i]) + 
				(imfor[j]._position.z - _z[i]) * (imfor[j]._position.z - _z[i]) < 10 * _distance)
			{	imfor[j]._life = -100;
			host->_life += host->_life_add;
			}
		}
		//主坦克撞上了爆炸中的广告牌
		if((host->_position.x - _x[i]) * (host->_position.x - _x[i]) + 
			(host->_position.z - _z[i]) * (host->_position.z - _z[i]) < _distance)
			host->_life -= 20;
		//boss撞上了爆炸中的广告牌
		if((wall->_boss._position.x - _x[i]) * (wall->_boss._position.x - _x[i]) + 
			(wall->_boss._position.z - _z[i]) * (wall->_boss._position.z - _z[i]) < _distance)
		{
				wall->_boss._life -= wall->_life_cut;
				if(wall->_boss._life <= 0)
					host->_life += host->_life_add * host->_life_cut;
		}
	}
	//改变爆炸广告牌的渲染画面
	_render_life += _render_add;
	if(_render_life > _render_life_amount)
		_render_life = 0;
	
}
void EXPLORE_BILLBOARD::render(const D3DXMATRIX &view)
{
	D3DXMATRIX world,tran,ro,billboard,scale;
	D3DXMatrixScaling(&scale,10.0f,10.0f,10.0f);
	D3DXMatrixIdentity(&billboard);

	billboard._11 = view._11;
	billboard._13 = view._13;
	billboard._31 = view._31;
	billboard._33 = view._33;
	D3DXMatrixInverse(&billboard, NULL, &billboard);
	
	_device->SetTexture(0,_texture_ex[_render_life]);
	_device->SetStreamSource(0,_vertex,0,sizeof(TERRAINVERTEX));
	_device->SetFVF(D3DFVF_TERRAINVERTEX);
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);				//打开Alpha融合
	_device->SetRenderState(D3DRS_LIGHTING,false);
	for(int i = 0;i < _random;i++)
	{
		D3DXMatrixIdentity(&world);
		D3DXMatrixTranslation(&tran,_x[i],0,_z[i]);
		world = world * billboard * scale * tran ;
		_device->SetTransform(D3DTS_WORLD,&world);
		_device->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);
	}
	_device->SetRenderState(D3DRS_LIGHTING,true);
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE,false);				//关闭Alpha融合
}
EXPLORE_BILLBOARD::~EXPLORE_BILLBOARD()
{

}