# include "wall.h"
extern bool	tree_or_explore;
//����Ķ���ָ��ΪX�ļ��ṩ�����ѡ��Χ
//��紫���������ͬʱ����Ҫ�����char���������ȡ��
//��������洢�������ά����ͬʱ�洢��ǽ������� / ����
WALL::WALL(LPDIRECT3DDEVICE9 device,wchar_t (*x_name)[50],BOUNDINGBOX &box,int ran):
			_random(ran),			//ȡ����Ϊ�˱������������
			_distance(10000),		//��ǽ�巢��ײ���ľ����ƽ��
			_life_time(500),			//����ǽ�������ֵ
			_life_cut(1)			//ǽ���յ����ʱ����ֵ��˥��ϵ��
			,_mul(10)
			,_box(box)
			,_x_name(x_name)
			,_device(device)
			,_life_mul(10)
{
	list<IMFOR_WALL>::iterator index;
	IMFOR_WALL wall;
	int j;
	TANK a(device);
	wall._life = _life_time;				//��ʼ��һ�������wall
	wall._position = D3DXVECTOR3(0,0,0);
	wall._mesh = new MESHXFILE(device);
	j = GetRandomFloat(0,_random);
	wall._mesh->Init(x_name[j],4);
	a._position = D3DXVECTOR3(0,0,0);
	PROPERTY enemy[8];				
	for(int i = 0;i < 8;i++)		//����з�̹�˵���Ϣ
		enemy[i]._position = D3DXVECTOR3(0,0,0);
	_imfor.push_back(wall);
	index = _imfor.begin();
	reset(index,enemy,&a);
	for(int i = 0;i < _random * _mul - 1;i++)
	{
		_imfor.push_back(wall);
		index++;
		reset(index,enemy,&a);
	}
	//BOSS̹�˵���Ϣ
	_boss._target = D3DXVECTOR3(0,0,-1);		//Ŀ��
	_boss._position = D3DXVECTOR3(5000,5000,5000);		//λ��
	_boss._look = D3DXVECTOR3(0,0,-1);			//�۲췽��
	D3DXMatrixIdentity(&_boss._rotation);
	_boss._mesh = new MESHXFILE(device);		//x�ļ�
	_boss._mesh->Init(x_name[5],BOSS);			//��ʼ��BOSS��x�ļ�������
	_boss._velocity = _boss._look * 0;
	_boss._run_time = 0;
	_boss._run_cut = 1;
	_boss._alive = false;
	_boss._mul = 10;								
	_boss._life = 0;	//��ʼ������ֵ
}
WALL::~WALL()		//��������
{}
//���뵱ǰ��Ҫ�������õ�ǽ��ĵ�����������̹������λ��
//��̹�ˣ��з�̹�ˣ�������̹�ˣ�ע�⣺������̹��Ϊ��ĳ�Ա
void WALL::reset(list<IMFOR_WALL>::iterator index,PROPERTY *pro,TANK *host)
{
	float x,z;
	int sign = 1;
	list<IMFOR_WALL>::iterator i;
	PROPERTY *p;
	x = GetRandomFloat(_box._min.x + 1000,_box._max.x - 1000);
	z = GetRandomFloat(_box._min.z + 1000,_box._max.z - 1000);
	while(1)
	{
		for(i = _imfor.begin();i != _imfor.end();i++)
		{
			if(i != index)
				if((x - i->_position.x) * (x - i->_position.x) + 
					(z - i->_position.z) * (z - i->_position.z) < _distance * 80)
				{
					x = GetRandomFloat(_box._min.x + 1000,_box._max.x - 1000);
					z = GetRandomFloat(_box._min.z + 1000,_box._max.z - 1000);
					i = _imfor.begin();
				}
		}
		//�ж��Ƿ��������̹��λ���ظ�--------------
		
		//�Ƿ���з�̹������λ���ظ�
		for(p = pro;p != pro + 8;p++)
			if((x - p->_position.x) * (x - p->_position.x) + 
				(z - p->_position.z) * (z - p->_position.z) < _distance)
				sign = 0;
		if(sign == 1)		//�������̹��Ҳû�����������ظ����õ����
			if((x - host->_position.x) * (x - host->_position.x) + 
				(z - host->_position.z) * (z - host->_position.z) > _distance)
			{
				index->_position = D3DXVECTOR3(x,0,z);		//λ��
				index->_life = _life_time;					//����
				index->_mesh->mesh_release();
				sign = (int)GetRandomFloat(0,20) % _random;	//x�ļ�
				index->_mesh->Init(_x_name[sign],4);
				return ;
			}
		sign = 1;
	}
}
void WALL::update_boss(const D3DXVECTOR3 &position)
{
	if(_boss._life > 0)
	{	
		if(_boss._run_time <= 0)			//��̹�˴�����ʱ��С��0
			reset_boss(position);
		else
			_boss._run_time -= _boss._run_cut;
		_boss._position += _boss._velocity;
	}
	else
	{	_boss._alive = false;
		_boss._position.x = 600000;
	}
}
void WALL::reset_boss(const D3DXVECTOR3 &position)
{
	_boss._run_time = GetRandomFloat(_life_time / 4,_life_time / 2);
	_boss._target = position;
	_boss._look = _boss._target - _boss._position;
	D3DXVec3Normalize(&_boss._look,&_boss._look);	
	_boss._velocity = _boss._look * _boss._mul;
	float angle;
	angle = D3DXVec3Dot(&D3DXVECTOR3(0,0,1),&_boss._look) / D3DXVec3Length(&_boss._look);
	if(_boss._look.x < 0)
		angle = D3DX_PI - acos(angle);
	else 
		angle = D3DX_PI + acos(angle);
	D3DXMatrixRotationAxis(&_boss._rotation,&D3DXVECTOR3(0,1,0),angle);		//��¼��ת�Ƕ�
}
//����ǽ�帽���ĵз��������̹���Ƿ���Ҫ������������·��
//�����ӵ��Ƿ�ǽ��򱬣�ֻ����ҵ��ӵ��ܹ���ǽ��
void WALL::test(PROPERTY *imfor,GUN_CHILD *gun,TANK *host)
{
	list<IMFOR_WALL>::iterator index;		//ǽ��
	list<ATTRIBUTE>::iterator par ;			//�������ӵ�λ��
	list<char>::iterator who;				//�ӵ�����
	list<D3DXMATRIX>::iterator ro;			//�ӵ���ת����

	//��һ��ǽ�������ез�̹�˱Ƚ�
	//�ٽ����������ӵ��Ƚ�

	for(index = _imfor.begin();index != _imfor.end();index++)
	{
		//�жϵз�̹����ǽ�ڵĹ�ϵ
		compare(imfor,index);
		//�ж��ӵ���ǽ��Ĺ�ϵ��ֻ����ҵ�̹�˻��ǽ������˺�
		//�������ӵ������У��ӵ���ʧ��������һ����ߣ��������ӱ�ը��������Ҫͬ������
		par = gun->_particles.begin();				//ȷ���ӵ�λ��
		who = gun->_who.begin();					//ȷ���ӵ�����
		ro  = gun->_velocity_view.begin();			//ȷ���ӵ���ת����
		for( ;par != gun->_particles.end(); )		//�ӵ�����
		{
			if((index->_position.x - par->_position.x) * (index->_position.x - par->_position.x) + 
				(index->_position.z - par->_position.z) * (index->_position.z - par->_position.z) < 
				_distance * 5)			//�ӵ�ײ����ǽ��
			{
				if((char)(*who) == ALLIED)				//����ӵ���ǽ�ڵ�Ѫ
					index->_life -= _life_cut;

				gun->add_explore(par->_position);		//�޳��ӵ���������ԣ������ӱ�ը
				par = gun->_particles.erase(par);		
				who = gun->_who.erase(who);
				ro	= gun->_velocity_view.erase(ro);
			}

			//�ӵ�ײ��boss̹��
			
			else if((_boss._alive == true) && (_boss._position.x - par->_position.x) * 
				(_boss._position.x - par->_position.x) + 
				(_boss._position.z - par->_position.z) * 
				(_boss._position.z - par->_position.z) 
				< _distance * 20)
			{		if((char)(*who) != BOSS)					//��boss�ӵ�����boss
					{	_boss._life -= _life_cut;
						gun->add_explore(par->_position);		//�޳��ӵ���������ԣ������ӱ�ը
						par = gun->_particles.erase(par);		
						who = gun->_who.erase(who);
						ro	= gun->_velocity_view.erase(ro);
						if(_boss._life <= 0)					//boss����
							host->_life += host->_life_add * host->_life_cut;
					}
			}
			else
				par++,who++,ro++;
		}
	}
}

//���µ�����̹����ǽ��λ�õ���Ϣ��ע�⣬��Ҫ���÷����ӵ�
void WALL::update(GUN_CHILD *gun,TANK_ENEMY *pro,TANK *host)
{
	if((_boss._position.x - host->_position.x) * (_boss._position.x - host->_position.x) + 
		(_boss._position.z - host->_position.z) * (_boss._position.z - host->_position.z) < (_distance))
	{
		host->_life = 0;
		return;
	}
	//����ǽ��
	list<IMFOR_WALL>::iterator index;
	for(index = _imfor.begin();index != _imfor.end();index++)
	{
		if(index->_life <= 0)			//��ǽ���Ѿ��򱬣������ֳ�����̹�˵�λ��ȷ���µ�ǽ�ڵ�λ��
		{	
			int i;
			i = GetRandomFloat(0,13);
			if(i < 4)
			{	
				if(_boss._alive == false)		//��ǽ��ʱ����boss�����ڣ�����
				{	_boss._alive = true;
					_boss._position = index->_position;
					_boss._life = _life_time * _life_mul;
					_boss._run_time = 0;
				}
			}
			else if(i < 5)
			{	
				if(pro->_mul > 1)			//�з�����
					pro->_mul -= 1;
			}
			else if(i < 7)				//�ҷ�̹���ٶ�����
				host->_mul += 1;
			else 
				tree_or_explore = !tree_or_explore;
			reset(index,pro->_imfor,host);
		}
	}
	if(_boss._alive == true)
		update_boss(host->_position);
}

void WALL::render()
{
	list<IMFOR_WALL>::iterator index;
	D3DXMATRIX world;
	D3DXMATRIX tran;
	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale,0.8f,0.8f,0.8f);
	for(index = _imfor.begin();index != _imfor.end();index++)
	{
		D3DXMatrixIdentity(&world);
		D3DXMatrixTranslation(&tran,index->_position.x,0,index->_position.z);
		world = world * scale * tran;
		index->_mesh->_device->SetTransform(D3DTS_WORLD,&world);
		index->_mesh->render();		//ֻ���ƣ������κα任
	}
	//����boss̹��
	D3DXMATRIX ro;
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranslation(&tran,_boss._position.x,0,_boss._position.z);
	D3DXMatrixScaling(&scale,0.001f,0.001f,0.001f);
	world = world  * _boss._rotation *scale * tran;
	/*world = world  *scale;*/
	_boss._mesh->_device->SetTransform(D3DTS_WORLD,&world);
	if(_boss._alive == true)
		_boss._mesh->render();
}
void WALL::compare(PROPERTY *imfor,list<IMFOR_WALL>::iterator index)
{
	//ÿ�αȽ�8ֻ�з�̹��
	for(int i = 0;i < 8;i++)
	{
		if((index->_position.x - imfor[i]._position.x) * (index->_position.x - imfor[i]._position.x) + 
			(index->_position.z - imfor[i]._position.z) * (index->_position.z - imfor[i]._position.z) 
			< _distance * 80)		//̹�˼���ײ��ǽ�ڣ���̹�˲���ײ��ǽ�ڣ�����ͨ��
		{
			//̹����ǽ�ڵ�������̹��ǰ���ķ������ǣ�̹�˱������
			if((D3DXVec3Dot(&(index->_position - imfor[i]._position),&imfor[i]._velocity)) > 0)
			{
				D3DXVECTOR3 ve = imfor[i]._velocity;
				//�ڶ���������Ҫ��̹�����ߵķŴ������,ʱ��Ҳ���
				//�������ַ������֪������ʱ��̹����ײ��ǽ��ʱ�ú�����Ҫ��������
				D3DXVec3Cross(&imfor[i]._velocity,&D3DXVECTOR3(0,10,0),&imfor[i]._velocity);
				imfor[i]._angle = D3DX_PI / 2;
				imfor[i]._angle_life = 360;								//_angle_sign_time��Ĭ��ֵ�ڹ��캯���޸�
				//imfor[i]._life = GetRandomFloat(1 * 360,4 * 360);		//����̹�����ߵ����ʱ��
			}
		}
	}
}