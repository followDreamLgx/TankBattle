# ifndef _ALPHA_H
# define _ALPHA_H

# include "head.h"

class ALPHA
{
protected:
	LPDIRECT3DDEVICE9 _device;

public:
	ALPHA(LPDIRECT3DDEVICE9 );						//ֻ��ʼ��device���������Ƹ�������
	VOID prepare();											//׼���׶�
	VOID draw(LPD3DXMESH,LPDIRECT3DTEXTURE9*,INT);			//����
};

# endif