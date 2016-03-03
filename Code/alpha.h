# ifndef _ALPHA_H
# define _ALPHA_H

# include "head.h"

class ALPHA
{
protected:
	LPDIRECT3DDEVICE9 _device;

public:
	ALPHA(LPDIRECT3DDEVICE9 );						//只初始化device，用来绘制各种网格
	VOID prepare();											//准备阶段
	VOID draw(LPD3DXMESH,LPDIRECT3DTEXTURE9*,INT);			//绘制
};

# endif