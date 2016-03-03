# include "skybox.h"

SKYBOX::SKYBOX(LPDIRECT3DDEVICE9 device)
{
	_device = device;
	_indexbuf	= NULL;
	_vertexbuf = NULL;
}

VOID SKYBOX::InitSkyBox(wchar_t **name)
{
	_device->CreateVertexBuffer(24 * sizeof(TERRAINVERTEX),
		0,
		D3DFVF_TERRAINVERTEX,
		D3DPOOL_MANAGED,
		&_vertexbuf,
		0);
	TERRAINVERTEX *pVertices = NULL;
	_vertexbuf->Lock(0,0,(void**)&pVertices,0);
	//前面
	pVertices[A] = TERRAINVERTEX(-30000,56000,-30000,0,0);
	pVertices[B] = TERRAINVERTEX(30000,56000,-30000,1,0);
	pVertices[C] = TERRAINVERTEX(30000,-4000,-30000,1,1);
	pVertices[D] = TERRAINVERTEX(-30000,-4000,-30000,0,1);
	//右面
	pVertices[E] = TERRAINVERTEX(30000,56000,-30000,0,0);
	pVertices[F] = TERRAINVERTEX(30000,56000,30000,1,0);
	pVertices[G] = TERRAINVERTEX(30000,-4000,30000,1,1);
	pVertices[H] = TERRAINVERTEX(30000,-4000,-30000,0,1);
	//背面
	pVertices[I] = TERRAINVERTEX(30000,56000,30000,0,0);
	pVertices[J] = TERRAINVERTEX(-30000,56000,30000,1,0);
	pVertices[K] = TERRAINVERTEX(-30000,-4000,30000,1,1);
	pVertices[L] = TERRAINVERTEX(30000,-4000,30000,0,1);
	//左面
	pVertices[M] = TERRAINVERTEX(-30000,56000,30000,0,0);
	pVertices[N] = TERRAINVERTEX(-30000,56000,-30000,1,0);
	pVertices[O] = TERRAINVERTEX(-30000,-4000,-30000,1,1);
	pVertices[P] = TERRAINVERTEX(-30000,-4000,30000,0,1);
	//顶面
	pVertices[Q] = TERRAINVERTEX(-30000,56000,30000,1,1);
	pVertices[R] = TERRAINVERTEX(30000,56000,30000,0,1);
	pVertices[S] = TERRAINVERTEX(30000,56000,-30000,0,0);
	pVertices[T] = TERRAINVERTEX(-30000,56000,-30000,1,0);
	//地面
	pVertices[X] = TERRAINVERTEX(-30000,-4000,30000,0,0);
	pVertices[W] = TERRAINVERTEX(30000,-4000,30000,0,1);
	pVertices[V] = TERRAINVERTEX(30000,-4000,-30000,1,1);
	pVertices[U] = TERRAINVERTEX(-30000,-4000,-30000,1,0);

	_vertexbuf->Unlock();
	_device->CreateIndexBuffer(36 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_indexbuf,
		0);
	//填充索引缓存
	WORD *pIndices = NULL;
	_indexbuf->Lock(0,0,(void**)&pIndices,0);
	//第一重循环控制面的数量
	//第二重循环控制每个面的三角形数量
	//第三重循环控制每个三角形索引对应的顶点缓存
	for(int i = 0,num = 0;i < 23;i += 4)
		for(int j = 2;j > 0;j--)
		{
			pIndices[num++] = i;
			for(int k = 1;k >= 0;k--)
				pIndices[num++] = i + j + k;
		}
	_indexbuf->Unlock();
	//加载纹理
	for(int i = 0;i < 6;i++)
		D3DXCreateTextureFromFile(_device,name[i],&_texture[i]);  
}

VOID SKYBOX::DrawSkyBox(D3DXVECTOR3 position)
{
	D3DXMATRIX matWorld,tran,z;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation(&tran,position.x,position.y,position.z);
	D3DXMatrixTranslation(&z,0,0,0);

	_device->SetTransform(D3DTS_WORLD,&matWorld);
	_device->SetRenderState(D3DRS_LIGHTING,FALSE);					//关闭光照
	_device->SetStreamSource(0,_vertexbuf,0,sizeof(TERRAINVERTEX));
	_device->SetFVF(D3DFVF_TERRAINVERTEX);				
	_device->SetIndices(_indexbuf);		
	for(int i = 0;i < 6;i++)
	{
		//_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		//_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		_device->SetTexture(0,_texture[i]);
		_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
			0,			//起始顶点位置
			0,			//允许使用最小索引值
			24, 		//顶点数量	
			i * 6,		//起始索引位置
			2			//图元数量
			);
	}
	_device->SetRenderState(D3DRS_LIGHTING,TRUE);					//重新开启光照
}