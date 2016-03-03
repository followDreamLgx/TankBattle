# include "terrain.h"



//���캯��
TERRAIN::TERRAIN(LPDIRECT3DDEVICE9 _device)
{
	device	=	_device;
	texture	=	NULL;		//��������
	IndexBuf =	NULL;		//��������
	VertexBuf = NULL;		//���㻺��
	CellRow =	0;			//ÿ�и�������ʼ����ʱ��ֵ
	CellCol =	0;			//ÿ�и�������ʼ����ʱ��ֵ
	VerRow =	0;			//ÿ�ж�����
	VerCol =	0;			//ÿ�ж�����
	NumVertices= 0;			//��������
	width =		0.0;		//���εĿ��
	depth =		0.0;		//���ε����
	spacing =	0.0;		//��Ԫ��ļ��,��ʼ��ʱ��ֵ
	HeightScale = 0.0;		//�߶�����ϵ������ʼ��ʱ��ֵ
}

//���ظ߶�ͼ������
BOOL TERRAIN::LoadTerrain(wchar_t *raw)
{
	ifstream infile;
	infile.open(raw,ios::binary);					//�����ƶ�ȡ
	infile.seekg(0,std::ios::end);
	vector<BYTE> inData(infile.tellg());
	infile.seekg(ios::beg);							//���ļ�ָ���ƶ����ļ���ͷ
	infile.read((char*)&inData[0], inData.size());	//�ؼ���һ������ȡ�����߶���Ϣ
	m_vHeightInfo.resize(inData.size());			//��m_vHeightInfo�ߴ�ȡΪ�������ĳߴ�
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
//��ʼ������
BOOL TERRAIN::InitTerrain(INT nRows,INT nCols,FLOAT space,FLOAT scale)
{
	CellRow = nRows;
	CellCol = nCols;
	spacing = space;
	HeightScale = scale;
	VerRow = CellRow + 1;						//����ÿ�еĶ�����
	VerCol = CellCol + 1;						//����ÿ�еĶ�����
	NumVertices = VerRow * VerCol;				//��������
	width = CellRow * spacing;					//���
	depth = CellCol * spacing;					//���
	for(unsigned int i=0; i<m_vHeightInfo.size(); i++)
        m_vHeightInfo[i] *= scale;
	//��䶥�㻺��	
	device->CreateVertexBuffer(NumVertices * sizeof(TERRAINVERTEX),
		0,
		D3DFVF_TERRAINVERTEX,
		D3DPOOL_MANAGED,
		&VertexBuf,
		0);
	TERRAINVERTEX *pVertices = NULL;			
	VertexBuf->Lock(0,0,(void**)&pVertices,0);
	//ע��:	��ȴ� �C��� / 2 �� ��� / 2��
	//		��ȴ� ��� / 2   �� -��� / 2��
	int Index = 0;
	int StartX = -width / 2, EndX = width / 2;
	int StartZ = depth / 2, EndZ = -depth / 2;
	//��䶥����Ϣ
	for(int z = StartZ,i = 0;z > EndZ;z -= spacing,i++)
		for(int x = StartX,j = 0;x < EndX; x += spacing,j++)
		{
			Index = i * CellRow + j;						//�ö����ڶ��㻺���е�λ��
			pVertices[Index]._x = x;						//������x�����ϵ�����
			pVertices[Index]._y = m_vHeightInfo[Index];		//�����Ӧ�ĸ߶�ֵ
			pVertices[Index]._z = z;						//������z�����ϵ�����
			pVertices[Index]._u = (float)j * 3.0f / CellRow;		//������u�����ϵ���������
			pVertices[Index]._v = (float)i * 3.0f / CellCol;		//������v�����ϵ���������
			Index++;			
		}
	VertexBuf->Unlock();									//�������㻺��
	//������������
	device->CreateIndexBuffer(NumVertices * 6 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&IndexBuf,
		0);
	//�����������
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
//���Ƶ���
BOOL TERRAIN::DrawTerrain()
{
	D3DXMATRIX matWorld;
	D3DXMATRIX tran;
	D3DXMatrixTranslation(&tran,0,-85,-29000);
	D3DXMatrixIdentity(&matWorld);
	matWorld = tran * matWorld;
	device->SetTransform(D3DTS_WORLD,&matWorld);
	device->SetStreamSource(0,VertexBuf,0,sizeof(TERRAINVERTEX));
	device->SetFVF(D3DFVF_TERRAINVERTEX);							//���õ��ε������ʽ
	device->SetIndices(IndexBuf);									//������������
	if(texture != NULL)
		device->SetTexture(0,texture);								//���õ��ε�����
	device->SetRenderState(D3DRS_LIGHTING,FALSE);					//�رչ��գ�����û�����÷��ߣ�
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,			//���Ƶ���
		NumVertices,
		0,
		NumVertices * 2);	
	if(texture != NULL)
		device->SetTexture(0,NULL);									//���Ǹ����õ�		
	device->SetRenderState(D3DRS_LIGHTING,TRUE);					//���¿�������
return true;
}