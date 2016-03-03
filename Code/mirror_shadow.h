# ifndef _MIRROR_SHADOW_H 
# define _MIRROR_SHADOW_H

# include "head.h"

class MIRROR_SHADOW
{
protected:
	LPDIRECT3DDEVICE9	_device;
	ID3DXMesh			*_wall;					//绘制镜子
	ID3DXMesh			*_mesh_d3d;
//	D3DXMATRIX&			_world_actor;
//	D3DXPLANE			_plane;					//虚拟镜面的位置，计算镜像变换矩阵
//	D3DXMATRIX&			_world_wall;
public:
	MIRROR_SHADOW(LPDIRECT3DDEVICE9 device,ID3DXMesh *mesh_d3d,ID3DXMesh *wall);

	VOID DrawMirror(D3DXMATRIX &world_actor,LPDIRECT3DTEXTURE9 *textures,INT num,
				D3DXMATRIX &world_wall,D3DXPLANE &plane);
	VOID MIRROR_SHADOW::DrawShadow(D3DXMATRIX &world_actor,LPDIRECT3DTEXTURE9 *textures,INT num,
				D3DXPLANE &plane,D3DXVECTOR4 &light);
};

# endif