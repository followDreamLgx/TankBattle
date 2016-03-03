# ifndef _TERRAIN_H
# define _TERRAIN_H

# include "head.h"
# include <vector>

class TERRAIN					//P267
{
protected:
	LPDIRECT3DDEVICE9 			device;			//
	LPDIRECT3DTEXTURE9 			texture;		//地形纹理
	LPDIRECT3DINDEXBUFFER9		IndexBuf;		//索引缓存
	LPDIRECT3DVERTEXBUFFER9		VertexBuf;		//顶点缓存

	INT		CellRow;							//每行个数，初始化的时候赋值
	INT		CellCol;							//每列个数，初始化的时候赋值
	INT		VerRow;								//每行顶点数
	INT		VerCol;								//每列顶点数
	INT		NumVertices;						//顶点总数
	
	FLOAT	width;								//地形的宽度
	FLOAT	depth;								//地形的深度
	FLOAT	spacing;							//单元格的间距,初始化时赋值
	FLOAT	HeightScale;						//高度缩放系数，初始化时赋值
	vector<FLOAT>		m_vHeightInfo;			// 用于存放高度信息

public:
	//构造函数
	TERRAIN(LPDIRECT3DDEVICE9 _device);
	//析构函数尚未完成
	~TERRAIN();
	//加载高度图和纹理（纹理暂时木有）
	BOOL LoadTerrain(wchar_t *raw,wchar_t *Texture);		//课本函数原型声明
	BOOL LoadTerrain(wchar_t *raw);
	//初始化地形
	BOOL InitTerrain(INT nRows,INT nCols,FLOAT space,FLOAT scale);
	//绘制地形
	//BOOL Drawterrain(D3DXMATRIX *MatWorld,BOOL DrawFrame = NULL);		//课本函数原型声明
	BOOL DrawTerrain();
};

# endif