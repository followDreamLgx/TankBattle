# ifndef _TERRAIN_H
# define _TERRAIN_H

# include "head.h"
# include <vector>

class TERRAIN					//P267
{
protected:
	LPDIRECT3DDEVICE9 			device;			//
	LPDIRECT3DTEXTURE9 			texture;		//��������
	LPDIRECT3DINDEXBUFFER9		IndexBuf;		//��������
	LPDIRECT3DVERTEXBUFFER9		VertexBuf;		//���㻺��

	INT		CellRow;							//ÿ�и�������ʼ����ʱ��ֵ
	INT		CellCol;							//ÿ�и�������ʼ����ʱ��ֵ
	INT		VerRow;								//ÿ�ж�����
	INT		VerCol;								//ÿ�ж�����
	INT		NumVertices;						//��������
	
	FLOAT	width;								//���εĿ��
	FLOAT	depth;								//���ε����
	FLOAT	spacing;							//��Ԫ��ļ��,��ʼ��ʱ��ֵ
	FLOAT	HeightScale;						//�߶�����ϵ������ʼ��ʱ��ֵ
	vector<FLOAT>		m_vHeightInfo;			// ���ڴ�Ÿ߶���Ϣ

public:
	//���캯��
	TERRAIN(LPDIRECT3DDEVICE9 _device);
	//����������δ���
	~TERRAIN();
	//���ظ߶�ͼ������������ʱľ�У�
	BOOL LoadTerrain(wchar_t *raw,wchar_t *Texture);		//�α�����ԭ������
	BOOL LoadTerrain(wchar_t *raw);
	//��ʼ������
	BOOL InitTerrain(INT nRows,INT nCols,FLOAT space,FLOAT scale);
	//���Ƶ���
	//BOOL Drawterrain(D3DXMATRIX *MatWorld,BOOL DrawFrame = NULL);		//�α�����ԭ������
	BOOL DrawTerrain();
};

# endif