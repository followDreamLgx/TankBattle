# include "alpha.h"


ALPHA::ALPHA(LPDIRECT3DDEVICE9	device)							//只初始化device，用来绘制各种网格
{
	_device = device;
}
VOID ALPHA::prepare()											//准备阶段,与绘制其它无须alpha的物体之前
{
	_device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	_device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);		
	_device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);	
	_device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);	
}
VOID ALPHA::draw(LPD3DXMESH mesh,LPDIRECT3DTEXTURE9* textures,INT n)			//绘制
{
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);				//打开Alpha融合
	for(int i = 0;i < n;i++)
	{
		_device->SetTexture(0,textures[i]);
		mesh->DrawSubset(i);
	}
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE,false);				//关闭Alpha融合
}