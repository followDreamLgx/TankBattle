# ifndef _BILLBOARD_H
# define _BILLBOARD_H

# include "head.h"



class BILLBOARD
{
protected:
	LPDIRECT3DDEVICE9				_device;
	LPDIRECT3DVERTEXBUFFER9			_vertex;
	LPDIRECT3DTEXTURE9				_texture;
	float *_x;
	float *_z;
	bool sign;
	const int _random;
public:
	BILLBOARD(LPDIRECT3DDEVICE9 device,const BOUNDINGBOX &box,wchar_t *name);
	BILLBOARD(LPDIRECT3DDEVICE9 device,wchar_t *name);
	BILLBOARD(LPDIRECT3DDEVICE9 device,const BOUNDINGBOX &box);
	~BILLBOARD();
	//绘制前至少一次调用此函数
	virtual void init(wchar_t *name);
	virtual void init();
	void render(D3DXMATRIX &view);
	virtual void render();
	friend class GUN_CHILD;
};


# endif