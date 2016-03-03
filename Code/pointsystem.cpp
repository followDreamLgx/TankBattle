# include "pointsystem.h"
		
POINTSYSTEM::POINTSYSTEM()
{
	_device = 0;
	_vertex = 0;
	_texture = 0;
}	


bool POINTSYSTEM::init(LPDIRECT3DDEVICE9 device,wchar_t *text_name)
{
	_device = device;
	HRESULT hr = 0;
	hr = _device->CreateVertexBuffer(
		_vertexbuf_size * sizeof(PARTICLE),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,	//�˴�����ʹ���й��ڴ��
		PARTICLE::FVF,
		D3DPOOL_DEFAULT,
		&_vertex,
		0);
	if(FAILED(hr))
	{
		::MessageBox(0, L"CreateVertexBuffer() - FAILED", L"PSystem", 0);
		return false;
	}
	hr = D3DXCreateTextureFromFile(
		device,
		text_name,
		&_texture);
	if(FAILED(hr))
	{
		MessageBox(0,L"D3DXCreateTextureFromFile() - FAILED",L"PSystem", 0);
		return false;
	}
	return true;
}
//�Ѿ����Ա�
//�����趨ϵͳ��ÿ�����ӵ�����			P236
void POINTSYSTEM::reset()
{
	list<ATTRIBUTE>::iterator i;			
	for(i = _particles.begin();i != _particles.end();i++)
		reset_particle(&(*i));				//ΪʲôҪ�Ƚ�������ȡ��ַ
}


//����һ������							P236
//��������һ������
//�����ӽ����б�ǰ������reset_particle�����ӽ��г�ʼ��
void POINTSYSTEM::add_particle()
{
	ATTRIBUTE attribute;
	reset_particle(&attribute);
	_particles.push_back(attribute);
}
//��������������麯��
//void POINTSYSTEM::reset_particle(ATTRIBUTE *attribute)
//{}


//���ڻ���֮ǰ�����б�Ҫ�ĳ�ʼ������״̬��������
//���ڸ÷�����ʵ�������ھ����ϵͳ�����Զ���Ϊ�麯��
//Ĭ�϶�������
void POINTSYSTEM::prepare_render()
{
	_device->SetRenderState(D3DRS_LIGHTING,FALSE);				//Ĭ�Ϲرչ�Դ
	_device->SetRenderState(D3DRS_POINTSPRITEENABLE,true);		//�㾫��Ļ���״̬������������ӳ�䵽�㾫��P231
	_device->SetRenderState(D3DRS_POINTSCALEENABLE,true);		//�㾫��ĳߴ��ù۲�����ϵ�ĵ�λ������	P231
	_device->SetRenderState(D3DRS_POINTSIZE,FtoDw(_size));		//�㾫��ĳߴ�
	_device->SetRenderState(D3DRS_POINTSIZE_MIN,FtoDw(0.0f));	//�㾫���ȡ����Сֵ

	_device->SetRenderState(D3DRS_POINTSCALE_A,FtoDw(0.0f));	//�����㾫����������ı�ߴ�
	_device->SetRenderState(D3DRS_POINTSCALE_B,FtoDw(0.0f));	//
	_device->SetRenderState(D3DRS_POINTSCALE_C,FtoDw(1.0f));	//
	//ʹ��ͼƬ������
	_device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	_device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);
	//����alpha�ں�
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	_device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	_device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
}

//�洢һ���ض�������ϵͳ���������úõ��κλ���״ֵ̬��Ϊ�麯��
//Ĭ��ʵ������
void POINTSYSTEM::post_render()
{
	_device->SetRenderState(D3DRS_LIGHTING,true);
	_device->SetRenderState(D3DRS_POINTSPRITEENABLE,false);		//�㾫��Ļ���״̬������������ӳ�䵽�㾫��P231
	_device->SetRenderState(D3DRS_POINTSCALEENABLE,false);		//�㾫��ĳߴ��ù۲�����ϵ�ĵ�λ������	P231
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE,false);
}
/*---------------------------------------------------------
1.������������Ϊ�յ�ʱ��������
2.����״̬������
3.�����㻺�������������趨ƫ����Ϊ0
4.�����䶥�㻺�棬�����㻺��ﵽƬ�ε�����ʱ���л���
5.����һ��Ƭ�ο�ʼ�������㻺��.��ƫ���������˻���������
	��ƫ�����ƶ���������ʼλ��
�ظ�4,5����ֱ�������ж����������㻺��
6.�ж����һ�λ����Ƿ������������û������˵����λ���û�л���
	��Ҫ�ٻ���һ��
7.�����л���״̬��ԭ
----------------------------------------------------------*/
void POINTSYSTEM::render()
{

	if( !_particles.empty() )			//���empty�����������Դ���
	{
		prepare_render();				//���û���״̬
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		_device->SetTransform(D3DTS_WORLD,&world);

		_device->SetTexture(0,_texture);
		_device->SetFVF(PARTICLE::FVF);
		_device->SetStreamSource(0,_vertex,0,sizeof(PARTICLE));

		if(_vertexbuf_offset >= _vertexbuf_size)		//ƫ�����Ѿ������˶��㻺����
			_vertexbuf_offset = 0;

		PARTICLE *pVertex = 0;
		_vertex->Lock(									//�������㻺��
			_vertexbuf_offset * sizeof(PARTICLE),
			_vertexbuf_num * sizeof(PARTICLE),
			(void**)&pVertex,
			_vertexbuf_offset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD num_in_buf = 0;
		list<ATTRIBUTE>::iterator i;					//�������������ĵ�����
		for(i = _particles.begin();i != _particles.end();i++)
		{
			if(i->_alive == true)
			{
				pVertex->_position = i->_position;
				pVertex->_color = (D3DCOLOR)i->_color;
				pVertex++;
				num_in_buf++;

				if(num_in_buf == _vertexbuf_num)		//Ƭ���ж����������������
				{	
					_vertex->Unlock();
					_device->DrawPrimitive(D3DPT_POINTLIST,
						_vertexbuf_offset,
						_vertexbuf_num);
				
					_vertexbuf_offset += _vertexbuf_num;
					if(_vertexbuf_offset >= _vertexbuf_size)			//��¼ƫ�����Ƿ񳬹��ڴ��޶�
						_vertexbuf_offset = 0;
					_vertex->Lock(
						_vertexbuf_offset * sizeof(PARTICLE),
						_vertexbuf_num * sizeof(PARTICLE),
						(void**)&pVertex,
						_vertexbuf_offset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
					num_in_buf = 0;
				}
			}
		}
		_vertex->Unlock();
		if(num_in_buf != 0)						//������һ��Ƭ�λ���
			_device->DrawPrimitive(
			D3DPT_POINTLIST,
			_vertexbuf_offset,
			num_in_buf);
		_vertexbuf_offset += _vertexbuf_num;
		post_render();
	}
}

//����ɱ��������Χ�������������Ѿ�����������
void POINTSYSTEM::remove_dead_particles()
{
	//���ȶ���һ��������
	list<ATTRIBUTE>::iterator i;
	i = _particles.begin();
	while(i != _particles.end())			//������������
	{
		if(i->_alive == false)				//���������������������
			i = _particles.erase(i);		//erase�������Զ�������������һλ
		else
			i++;
	}
}

//���������ӽ��и���,�������붨��˺���
//void POINTSYSTEM::update(float time)


//��������Դ�Ƿ�Ϊ�յ���Ϣ
bool POINTSYSTEM::is_empty()
{
	return _particles.empty();
}
//�ж�����Դ�����е������Ƿ��Ѿ�ȫ������
//����һ�����Ӵ����Ƿ񼴷���true
//����������������ʱ�򷵻�false
bool POINTSYSTEM::is_dead()
{
	std::list<ATTRIBUTE>::iterator i;
	for(i = _particles.begin(); i != _particles.end(); i++)
	{
		// is there at least one living particle?  If yes,
		// the system is not dead.
		if( i->_alive )
			return false;
	}
	// no living particles found, the system must be dead.
	return true;
}
POINTSYSTEM::~POINTSYSTEM()
{
	if(_vertex)
		_vertex->Release();
	if(_texture)
		_texture->Release();
}