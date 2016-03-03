# include "mirror_shadow.h"


//引用必须使用初始化
MIRROR_SHADOW::MIRROR_SHADOW(LPDIRECT3DDEVICE9 device,ID3DXMesh *mesh_d3d,ID3DXMesh *wall)
{
	_device = device;
	_mesh_d3d = mesh_d3d;
	_wall = wall;
}
VOID MIRROR_SHADOW::DrawMirror(D3DXMATRIX &world_actor,LPDIRECT3DTEXTURE9 *textures,INT num,
					D3DXMATRIX &world_wall,D3DXPLANE &plane)
{
	_device->SetRenderState(D3DRS_STENCILENABLE,true);					//开启模板测试
	//模板比较函数总是成功的，在接下来的绘制内容总能够通过模板测试
	_device->SetRenderState(D3DRS_STENCILFUNC,D3DCMP_ALWAYS);
	_device->SetRenderState(D3DRS_STENCILREF,0x1);						//REF设置为1
	_device->SetRenderState(D3DRS_STENCILMASK,0xffffffff);				//设置MASK
	_device->SetRenderState(D3DRS_STENCILWRITEMASK,0xffffffff);
	//当模板测试成功时用模板参考值(上面指定为0x1)
	_device->SetRenderState(D3DRS_STENCILPASS,D3DSTENCILOP_REPLACE);	//深度与模板都成功，将模板值设置为0x1
	_device->SetRenderState(D3DRS_STENCILFAIL,D3DSTENCILOP_KEEP);		//模板测试失败，不改变模板缓存
	_device->SetRenderState(D3DRS_STENCILZFAIL,D3DSTENCILOP_KEEP);		//深度测试失败，不改变深度
	//绘制镜面
	_device->SetRenderState(D3DRS_ZWRITEENABLE,false);					//关闭深度缓存写操作
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);				//启用alpha融合
	//通过以下这两步，可以实现模板缓存改变但后台缓存不改变
	_device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ZERO);				//源融合因子
	_device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);				//目标融合因子
	//绘制模板缓存
	_device->SetTransform(D3DTS_WORLD,&world_wall);
	_wall->DrawSubset(0);
	
	_device->SetRenderState(D3DRS_ZWRITEENABLE,true);					//重新开启开启深度缓存写操作
	//1. 测试点像素的模板值等于模板缓存中的值时成功
	_device->SetRenderState(D3DRS_STENCILFUNC,D3DCMP_EQUAL);
	//2. 深度缓存与模板缓存测试都通过时不改变模板缓存，这两者决定了只有当模板缓存中的数值等于1时才能测试成功				
	_device->SetRenderState(D3DRS_STENCILPASS,D3DSTENCILOP_KEEP);
	_device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_DESTCOLOR);
	_device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
	//由于四面体的镜像深度大于镜面，镜面的图元遮挡了镜像，必须将深度深度缓存清空，否则无法看到镜像
	_device->Clear(0,0,D3DCLEAR_ZBUFFER,0,1.0f,0);	
	//四面体镜像与镜面的融合方程
	_device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_DESTCOLOR);
	_device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);

	D3DXMATRIX X,mul;
	D3DXMatrixReflect(&X,&plane);									//镜面变换的矩阵
	mul = world_actor * X;
	_device->SetTransform(D3DTS_WORLD,&mul);
	_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);			//开启背部顺时针消隐
	for(int i = 0;i < num;i++)
	{
		_device->SetTexture(0,textures[i]);
		_mesh_d3d->DrawSubset(i);
	}
	_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			//开启背部逆时针消隐
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE,false);			//关闭Alpha融合
	_device->SetRenderState(D3DRS_STENCILENABLE,false);				//禁用模板缓存，
}
//绘制阴影的成员函数
//必须传入四面体的世界坐标，四面体的纹理二级指针，纹理的数量
//投影平面，光源方向（方向光或者聚光灯由向量的第四个分量决定，1为点光源，0为方向光）

VOID MIRROR_SHADOW::DrawShadow(D3DXMATRIX &world_actor,LPDIRECT3DTEXTURE9 *textures,INT num,
				D3DXPLANE &plane,D3DXVECTOR4 &light)
{
	_device->SetRenderState(D3DRS_STENCILENABLE,    true);						//打开模板缓存
    _device->SetRenderState(D3DRS_STENCILFUNC,      D3DCMP_EQUAL);				//模板测试函数只有相等时才通过
    _device->SetRenderState(D3DRS_STENCILREF,       0x0);						//模板缓存已经清零，设置为0 
																				//可以保证首次像素阴影写入能通过
	_device->SetRenderState(D3DRS_STENCILMASK,      0xffffffff);
    _device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	_device->SetRenderState(D3DRS_STENCILZFAIL,     D3DSTENCILOP_KEEP);
    _device->SetRenderState(D3DRS_STENCILFAIL,      D3DSTENCILOP_KEEP);				
    _device->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_INCR);		//通过了模板与深度缓存，模板缓存中的值就增加
																				//增加的结果是导致模板缓存的值不为0，不能通过模板测试
																				//所以在同一个地方的像素只能通过一次，防止二次融合
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);					//打开alpha融合
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
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE,false);			//关闭Alpha融合
	_device->SetRenderState(D3DRS_STENCILENABLE,false);				//禁用模板缓存，
	_device->SetRenderState(D3DRS_ZENABLE,true);
}