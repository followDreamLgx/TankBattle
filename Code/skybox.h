# ifndef _SKYBOX_H
# define _SKYBOX_H

# include "head.h"

	
class  SKYBOX
{
protected:
	LPDIRECT3DDEVICE9		_device;
	LPDIRECT3DVERTEXBUFFER9	_vertexbuf;
	LPDIRECT3DINDEXBUFFER9	_indexbuf;
	LPDIRECT3DTEXTURE9		_texture[6];
public:
	SKYBOX(LPDIRECT3DDEVICE9 );
	VOID InitSkyBox(wchar_t **name);
	VOID DrawSkyBox(D3DXVECTOR3);
};

# endif