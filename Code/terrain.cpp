# include "terrain.h"



//构造函数
TERRAIN::TERRAIN(LPDIRECT3DDEVICE9 _device)
{
	device	=	_device;
	texture	=	NULL;		//地形纹理
	IndexBuf =	NULL;		//索引缓存
	VertexBuf = NULL;		//顶点缓存
	CellRow =	0;			//每行个数，初始化的时候赋值
	CellCol =	0;			//每列个数，初始化的时候赋值
	VerRow =	0;			//每行顶点数
	VerCol =	0;			//每列顶点数
	NumVertices= 0;			//顶点总数
	width =		0.0;		//地形的宽度
	depth =		0.0;		//地形的深度
	spacing =	0.0;		//单元格的间距,初始化时赋值
	HeightScale = 0.0;		//高度缩放系数，初始化时赋值
}

//加载高度图和纹理
BOOL TERRAIN::LoadTerrain(wchar_t *raw)
{
	ifstream infile;
	infile.open(raw,ios::binary);					//二进制读取
	infile.seekg(0,std::ios::end);
	vector<BYTE> inData(infile.tellg());
	infile.seekg(ios::beg);							//将文件指针移动到文件开头
	infile.read((char*)&inData[0], inData.size());	//关键的一步，读取整个高度信息
	m_vHeightInfo.resize(inData.size());			//将m_vHeightInfo尺寸取为缓冲区的尺寸
	for (unsigned int i=0; i<inData.size(); i++)		
        m_vHeightInfo[i] = inData[i];
	infile.close();
return true;
}
BOOL TERRAIN::LoadTerrain(wchar_t *raw,wchar_t *Texture)
{
	LoadTerrain(raw);
	D3DXCreateTextureFromFile(device,Texture,&texture);
return true;
}
//初始化地形
BOOL TERRAIN::InitTerrain(INT nRows,INT nCols,FLOAT space,FLOAT scale)
{
	CellRow = nRows;
	CellCol = nCols;
	spacing = space;
	HeightScale = scale;
	VerRow = CellRow + 1;						//地形每行的顶点数
	VerCol = CellCol + 1;						//地形每列的顶点数
	NumVertices = VerRow * VerCol;				//顶点总数
	width = CellRow * spacing;					//宽度
	depth = CellCol * spacing;					//深度
	for(unsigned int i=0; i<m_vHeightInfo.size(); i++)
        m_vHeightInfo[i] *= scale;
	//填充顶点缓存	
	device->CreateVertexBuffer(NumVertices * sizeof(TERRAINVERTEX),
		0,
		D3DFVF_TERRAINVERTEX,
		D3DPOOL_MANAGED,
		&VertexBuf,
		0);
	TERRAINVERTEX *pVertices = NULL;			
	VertexBuf->Lock(0,0,(void**)&pVertices,0);
	//注意:	宽度从 –宽度 / 2 到 宽度 / 2；
	//		深度从 深度 / 2   到 -深度 / 2；
	int Index = 0;
	int StartX = -width / 2, EndX = width / 2;
	int StartZ = depth / 2, EndZ = -depth / 2;
	//填充顶点信息
	for(int z = StartZ,i = 0;z > EndZ;z -= spacing,i++)
		for(int x = StartX,j = 0;x < EndX; x += spacing,j++)
		{
			Index = i * CellRow + j;						//该顶点在顶点缓存中的位置
			pVertices[Index]._x = x;						//顶点在x方向上的坐标
			pVertices[Index]._y = m_vHeightInfo[Index];		//顶点对应的高度值
			pVertices[Index]._z = z;						//顶点在z方向上的坐标
			pVertices[Index]._u = (float)j * 3.0f / CellRow;		//顶点在u方向上的纹理坐标
			pVertices[Index]._v = (float)i * 3.0f / CellCol;		//顶点在v方向上的纹理坐标
			Index++;			
		}
	VertexBuf->Unlock();									//解锁顶点缓存
	//创建索引缓存
	device->CreateIndexBuffer(NumVertices * 6 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&IndexBuf,
		0);
	//填充索引缓存
	WORD *pIndices = NULL;
	IndexBuf->Lock(0,0,(void**)&pIndices,0);
	Index = 0;
	for(int row = 0;row < CellRow - 1;row++)
		for(int col = 0;col < CellCol - 1;col++)
		{
		pIndices[Index] = row * CellRow + col;
		pIndices[Index + 1] = row * CellRow + col + 1;
		pIndices[Index + 2] = (row + 1) * CellRow + col;
		pIndices[Index + 3] = (row + 1)* CellRow + col;
		pIndices[Index + 4] = row * CellRow + col + 1;
		pIndices[Index + 5] = (row + 1) * CellRow + col + 1;
		Index += 6;
		}
	IndexBuf->Unlock();
return true;
}
//绘制地形
BOOL TERRAIN::DrawTerrain()
{
	D3DXMATRIX matWorld;
	D3DXMATRIX tran;
	D3DXMatrixTranslation(&tran,0,-85,-29000);
	D3DXMatrixIdentity(&matWorld);
	matWorld = tran * matWorld;
	device->SetTransform(D3DTS_WORLD,&matWorld);
	device->SetStreamSource(0,VertexBuf,0,sizeof(TERRAINVERTEX));
	device->SetFVF(D3DFVF_TERRAINVERTEX);							//设置地形的灵活顶点格式
	device->SetIndices(IndexBuf);									//设置索引缓存
	if(texture != NULL)
		device->SetTexture(0,texture);								//设置地形的纹理
	device->SetRenderState(D3DRS_LIGHTING,FALSE);					//关闭光照（地形没有设置法线）
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,			//绘制地形
		NumVertices,
		0,
		NumVertices * 2);	
	if(texture != NULL)
		device->SetTexture(0,NULL);									//这是干嘛用的		
	device->SetRenderState(D3DRS_LIGHTING,TRUE);					//重新开启光照
return true;
}