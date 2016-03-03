
# include "tank.h"
# include <stdio.h>

MESHXFILE::MESHXFILE(LPDIRECT3DDEVICE9 device)
{
	_device				= device;			//direct3D�豸�ӿ�
	_mesh				= NULL;				//�������
	_textures			= NULL;				//�����������Ϣ
	_NumMaterials		= 0;				//���ʵ���Ŀ
	pAdjBuffer = NULL;
	pMtrlBuffer = NULL;
}

void MESHXFILE::Init(wchar_t *xfile,int i)	//����x�ļ����������������������
{
	//��ȡx�ļ��е���������
	D3DXLoadMeshFromX(xfile,
		D3DXMESH_MANAGED,
		_device,
		&pAdjBuffer,
		&pMtrlBuffer,
		NULL,
		&_NumMaterials,
		&_mesh);
	read_texture(i);
}
void MESHXFILE::read_texture(int j)
{
	//��ȡ���ʺ���������
	wchar_t a[10][50] = {{},
						{L"KRLbody.bmp"},			//��ս̹��
						{L"PANZAUSA.JPG"},//�з�̹��
						{L"FEUROASH.JPG"},			//�ӵ�
						{L""},	//ǽ��
						{L"3 (1).JPG"}	//BOSS	
	};	
	D3DXMATERIAL *pMaterial = (D3DXMATERIAL*)pMtrlBuffer->GetBufferPointer();
	_materials = new D3DMATERIAL9[_NumMaterials];			//������¼������Ϣ�Ķ���
	_textures = new LPDIRECT3DTEXTURE9[_NumMaterials];		//������¼�����ָ��

	for(DWORD i = 0;i < _NumMaterials;i++)
	{
		_materials[i] = pMaterial[i].MatD3D;
		_materials[i].Diffuse = D3DXCOLOR(0.10f,0.20f,0.50f,1.0f);
		_materials[i].Ambient = D3DXCOLOR(0.10f,0.20f,0.30f,1.0f);
		_materials[i].Specular = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
		_materials[i].Emissive = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
		_textures[i] = NULL;
		if(j >= 1 && j <= 3 || j == 5)
			D3DXCreateTextureFromFile(_device,
			a[j],										//��pMtrBuffer�õ���ʼ��ַ
			&_textures[i]);								//�洢����
		
		else
			D3DXCreateTextureFromFileA(_device,
				pMaterial[i].pTextureFilename,				//��pMtrBuffer�õ���ʼ��ַ
				&_textures[i]);								//�洢����
	}
	pAdjBuffer->Release();
	pMtrlBuffer->Release();
}
void MESHXFILE::render()
{
	for(DWORD i = 0;i < _NumMaterials;i++)
	{
		_device->SetMaterial(&_materials[i]);
		_device->SetTexture(0,_textures[i]);
		_mesh->DrawSubset(i);
	}
}
void MESHXFILE::mesh_release()
{
	_mesh->Release();
	//for(DWORD i = 0;i < _NumMaterials;i++)
	//{
	//	_textures[i]->Release();
	//}
}
//----------------------------------------
//̹��
//----------------------------------------
TANK::TANK(LPDIRECT3DDEVICE9 device):MESHXFILE(device),
									_position(0.0f,0.0f,0.0f),_look(0.0f,0.0f,-1.0f),_right(-1.0f,0.0f,0.0f),
									_mul(10),_turn_scale(0.1),_life_cut(3),_life_time(5000),_life_add(200)
{
	_rotation_sign		= 0;
	_life				= _life_time;
	D3DXMatrixIdentity(&_rotation);
}
void TANK::render()				//�����������
{
	D3DXMATRIX world,trans,mathaha,rotation_sign;
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranslation(&trans,_position.x,_position.y,_position.z);
	D3DXMatrixIdentity(&rotation_sign);
	if(_rotation_sign)
	{
		D3DXMatrixRotationAxis(&rotation_sign,&D3DXVECTOR3(0,1,0),D3DX_PI * _rotation_sign / 180);
		_rotation_sign = 0;
	}
	D3DXMatrixScaling(&mathaha,0.0005f,0.0005f,0.0005f);
	world = world * mathaha * _rotation * rotation_sign * trans;
	//				����		��ת		ת��			λ��
	_device->SetTransform(D3DTS_WORLD,&world);
	
	for(DWORD i = 0;i < _NumMaterials;i++)
	{
		_device->SetMaterial(&_materials[i]);
		_device->SetTexture(0,_textures[i]);
		_mesh->DrawSubset(i);
	}
	_device->SetRenderState(D3DRS_LIGHTING,true);
	_life -= _life_cut;
}

//̹��ǰ������
void TANK::TranslationFront(CAMERA &camera)
{
	_position += _mul * _look;
	camera.Translation(_look * _mul);
}
//����
void TANK::TranslationBack(CAMERA &camera)
{
	_position -= _mul * _look;
	camera.Translation(_look * (-_mul));
}
void TANK::TurnRight(CAMERA &camera)
{
	const float angles = D3DX_PI / 180.0f;
	float len_look,len_right,len_target,camera_y = camera.position.y,target_y = camera.target.y;

	_rotation_sign = 10;				//��ֵ����Ⱦ��������̹����תʱ�ڶ��ĽǶȣ�������Ҫ�������

	len_right = _look.z * camera.position.x - _look.x * camera.position.z - _look.z * _position.x + _look.x * _position.z;
	len_right = len_right * len_right / (_look.z * _look.z + _look.x * _look.x);
	len_right = sqrt(len_right);														//�������look���ˮƽ����

	len_look = _right.z * camera.position.x - _right.x * camera.position.z - _right.z * _position.x + _right.x * _position.z;
	len_look = len_look * len_look / (_right.z * _right.z + _right.x * _right.x);
	len_look = sqrt(len_look);															//�������right���ˮƽ����
										
	len_target = sqrt((camera.target.z - _position.z) * (camera.target.z - _position.z) + 
		(camera.target.x - _position.x) * (camera.target.x - _position.x));				//������۲����̹�˵ľ���

	_position += _mul * _look ;														//����̹�˵�λ��
	_position += _mul * _right * _turn_scale;

	D3DXMATRIX mid;
	D3DXMatrixRotationAxis(&mid,&D3DXVECTOR3(0.0,1.0f,0.0f),angles);
	_rotation  = _rotation * mid;					//��¼̹����ת�ĽǶ�

	D3DXVec3TransformNormal(&_right,&_right,&mid);	//��̹�˵�right��look������תͬ���ĽǶ�
	D3DXVec3TransformNormal(&_look,&_look,&mid);
	D3DXVec3Normalize(&_right,&_right);
	D3DXVec3Normalize(&_look,&_look);

	D3DXVec3TransformNormal(&camera.right,&camera.right,&mid);	//���������right��look������תͬ���ĽǶ�
	D3DXVec3TransformNormal(&camera.look,&camera.look,&mid);
	D3DXVec3Normalize(&camera.right,&camera.right);
	D3DXVec3Normalize(&camera.look,&camera.look);


	camera.position = _position - _look * len_look - _right * len_right;				//�����������λ��
	camera.position.y = camera_y;

	camera.target = _position + len_target * _look;										//����������Ĺ۲��
	camera.target.y = target_y;
}
void TANK::TurnLeft(CAMERA &camera)
{
	const float angles = -D3DX_PI / 180.0f;

	float len_look,len_right,len_target,camera_y = camera.position.y,target_y = camera.target.y;

	_rotation_sign = -2;		//��ֵ����Ⱦ��������̹����תʱ�ڶ��ĽǶȣ�������Ҫ�������

	len_right = _look.z * camera.position.x - _look.x * camera.position.z - _look.z * _position.x + _look.x * _position.z;
	len_right = len_right * len_right / (_look.z * _look.z + _look.x * _look.x);
	len_right = sqrt(len_right);														//�������look���ˮƽ����

	len_look = _right.z * camera.position.x - _right.x * camera.position.z - _right.z * _position.x + _right.x * _position.z;
	len_look = len_look * len_look / (_right.z * _right.z + _right.x * _right.x);
	len_look = sqrt(len_look);															//�������right���ˮƽ����
										
	len_target = sqrt((camera.target.z - _position.z) * (camera.target.z - _position.z) + 
		(camera.target.x - _position.x) * (camera.target.x - _position.x));				//������۲����̹�˵ľ���

	_position += _mul * _look;														//����̹�˵�λ��
	_position += _mul * (-_right) * _turn_scale;

	D3DXMATRIX mid;
	D3DXMatrixRotationAxis(&mid,&D3DXVECTOR3(0.0,1.0f,0.0f),angles);
	_rotation  = _rotation * mid;					//��¼̹����ת�ĽǶ�

	D3DXVec3TransformNormal(&_right,&_right,&mid);	//��̹�˵�right��look������תͬ���ĽǶ�
	D3DXVec3TransformNormal(&_look,&_look,&mid);
	D3DXVec3Normalize(&_right,&_right);
	D3DXVec3Normalize(&_look,&_look);

	D3DXVec3TransformNormal(&camera.right,&camera.right,&mid);	//���������right��look������תͬ���ĽǶ�
	D3DXVec3TransformNormal(&camera.look,&camera.look,&mid);
	D3DXVec3Normalize(&camera.right,&camera.right);
	D3DXVec3Normalize(&camera.look,&camera.look);

	camera.position = _position - _look * len_look - _right * len_right;				//�����������λ��
	camera.position.y = camera_y;

	camera.target = _position + len_target * _look;										//����������Ĺ۲��
	camera.target.y = target_y;
}

//----------------------------------------------------------
//�ӵ���
//----------------------------------------------------------
//���ߵĹ��캯����ͬʱ���û���ĳ�ʼ������
TANK_GUN::TANK_GUN(LPDIRECT3DDEVICE9 device,wchar_t *name_gun,wchar_t *name_point):_exit_time(10.0f),
		MESHXFILE(device),POINTSYSTEM(),		//���û���Ĺ��캯��
		_mul(200),_time_cut(1)
{
	_vertexbuf_size = 512;
	MESHXFILE::Init(name_gun,3);
	POINTSYSTEM::init(device,name_point);
}
//�����ӵ��Լ���ը				*****�˴�ʹ�õ��ӵ����������ӵ���ըʱҲʹ�����ӵ�������������0�ĵ��ڱ�ըʹ��												
//									���Ǳ�ը����Ҫ���ӵ��������ٴν��ͣ����߸���ֵ�������ڴ˴���������
void TANK_GUN::add_gun(D3DXVECTOR3 &position,D3DXVECTOR3 &look,D3DXMATRIX &velocity_view)
{
	ATTRIBUTE new_gun;
	new_gun._position = position;
	new_gun._velocity = look * _mul;
	new_gun._life_time = _exit_time;
	_particles.push_back(new_gun);
	_velocity_view.push_back(velocity_view);
return ;
}
void TANK_GUN::update(float time)
{
	list<ATTRIBUTE>::iterator i;
	list<D3DXMATRIX>::iterator j;
	for(i = _particles.begin(),j = _velocity_view.begin();i != _particles.end();)
	{
		i->_life_time -= _time_cut;
		i->_position += _time_cut * i->_velocity;
		if(i->_life_time <= 0)				//�������������������������޳�
		{	
			i = _particles.erase(i);
			j = _velocity_view.erase(j);
		}
		else
			i++,j++;
	}
return ;
}
//ֻ�������ӵ��ķ��й켣
void TANK_GUN::render()
{
	D3DXMATRIX world,tran,rota,scale;
	D3DXMatrixScaling(&scale,0.30f,0.30f,0.50f);
	list<ATTRIBUTE>::iterator i;
	list<D3DXMATRIX>::iterator j;
	for(i = _particles.begin(),j = _velocity_view.begin();i != _particles.end();i++,j++)
	{
		D3DXMatrixIdentity(&world);
		D3DXMatrixTranslation(&tran,i->_position.x,i->_position.y,i->_position.z);
		world = world * scale * (D3DXMATRIX)(*j) * tran;
		MESHXFILE::_device->SetTransform(D3DTS_WORLD,&world);
		for(DWORD k = 0;k < _NumMaterials;k++)
		{
			MESHXFILE::_device->SetMaterial(&_materials[k]);
			MESHXFILE::_device->SetTexture(0,_textures[k]);
			MESHXFILE::_mesh->DrawSubset(k);
		}
	}
}

void TANK_GUN::reset_particle(ATTRIBUTE *attribute)	//��ʱ����Ϊ�պ���
{}

//-----------------------------------------------
//�����ӵ���
//-----------------------------------------------
GUN_CHILD::GUN_CHILD(LPDIRECT3DDEVICE9 device,wchar_t *name_gun,wchar_t *name_point):
				TANK_GUN(device,name_gun,name_point),
				_explore_life(24),		//��ը����ʱ��
				_explore_cut(1),		//��ըʱ��������ٶ�
				_hit_critical(10000)	//�������ƽ��ֵ
				,_billboard(device,name_point)
{
	wchar_t explore[25][20] = {{L"explode00.dds"},{L"explode01.dds"},{L"explode02.dds"},{L"explode03.dds"},
	{L"explode04.dds"},{L"explode05.dds"},{L"explode06.dds"},{L"explode07.dds"},{L"explode08.dds"},
	{L"explode09.dds"},{L"explode10.dds"},{L"explode11.dds"},{L"explode12.dds"},{L"explode13.dds"},
	{L"explode14.dds"},{L"explode15.dds"},{L"explode16.dds"},{L"explode17.dds"},{L"explode18.dds"},
	{L"explode19.dds"},{L"explode20.dds"},{L"explode21.dds"},{L"explode22.dds"},{L"explode23.dds"},
	{L"explode24.dds"}};
	_vertexbuf_size = 512;
	MESHXFILE::Init(name_gun,3);
	POINTSYSTEM::init(device,name_point);
	
	for(int i = 0;i < 25;i++)			//��������
		D3DXCreateTextureFromFile(POINTSYSTEM::_device,explore[i],&_texture_gun[i]);  
}
GUN_CHILD::~GUN_CHILD()
{

}
//Ĭ�������˾����ӵ�����Ϊ�˾�û�б�־
//���˾������ӵ�ʱ�����봫���ӵ���־�����ǵط������ӵ�ʱ����Ҫ�涨����һ�����ӵ�
void GUN_CHILD::add(D3DXVECTOR3 &position,D3DXVECTOR3 &look,D3DXMATRIX &velocity_view,char who)
{
	add_gun(position,look,velocity_view);
	_who.push_back(who);
}
void GUN_CHILD::update(TANK *host,PROPERTY *enemy)
{
	list<ATTRIBUTE>::iterator	i = _particles.begin();
	list<D3DXMATRIX>::iterator	j = _velocity_view.begin();
	list<char>::iterator		k = _who.begin();
	for( ;i != _particles.end(); )
	{
		if((char)(*k) == ALLIED)		//���ֵ�������ʣ����Ե�ʱ��һ��
		{
			if((is_explore(enemy,host,i) == false) && (i->_life_time > 0))				//�ӵ����,û�б�ըҲû�н�������
			{
				i->_life_time -= _time_cut;				//�ӵ������ʱ��˥��ϵ��
				i->_position += _time_cut * i->_velocity;
				i++,j++,k++;
				continue;
			}
		}
		else if((char)(*k) == NAZI || (char)(*k) == BOSS)			
		{
			if((is_explore(host,i) == false) && (i->_life_time > 0))				//�ӵ����,û�б�ըҲû�н�������
			{
				i->_life_time -= _time_cut;
				i->_position += _time_cut * i->_velocity;
				i++,j++,k++;
				continue;
			}
		}
		add_explore(i->_position);
		i = _particles.erase(i);		//�ӵ����ܴ���̹�ˣ�Ҳ�п������б�ը
		j = _velocity_view.erase(j);
		k = _who.erase(k);
	}
}
bool GUN_CHILD::is_explore(TANK *host,list<ATTRIBUTE>::iterator i)		//�����ӵ��ж��Ƿ�����˾�				
{
	if((host->_position.x - i->_position.x) * (host->_position.x - i->_position.x) + 
		(host->_position.z - i->_position.z) * (host->_position.z - i->_position.z) 
		< _hit_critical)
	{	//�����˾�
		host->_life -= _time_cut * 50;			//�ҷ�̹������ֵ����
		add_explore(host->_position);
		return true;
	}
return false;
}
bool GUN_CHILD::is_explore(PROPERTY *imfor,TANK *host,list<ATTRIBUTE>::iterator i)	//�˾��ӵ��ж��Ƿ���е���						
{
	//����̹�˵�λ�ý��бȽ�
	PROPERTY *p;
	byte sign = 0;
	float critical,max = 100000000000000000;
	for(int j = 0;j < 8;j++)			//Ѱ�����ӵ������̹��
	{	//����̹�����ӵ��ľ���
		critical = (imfor[j]._position.x - i->_position.x) * (imfor[j]._position.x - i->_position.x) + 
			(imfor[j]._position.z - i->_position.z) * (imfor[j]._position.z - i->_position.z);
		if(critical < _hit_critical)	//�ӵ�����̹��
			if(critical < max)			//�Ƿ��������̹��
			{	
				max = critical;
				sign = 1;
				p = &imfor[j];
			}
	}
	if(sign == 1)				//����BOSS����ָ�Ѫ
	{
		p->_life = -100;
		add_explore(p->_position);
		host->_life += host->_life_add;
		return true;
	}
return false;
}
void GUN_CHILD::add_explore(D3DXVECTOR3 &position)
{
	GUN_EXPLORE ex;
	ex._position = position;
	ex._time = 0;
	_explore_num.push_back(ex);
}
void GUN_CHILD::explore(const D3DXMATRIX &view)					//��ָ���ĵص������ը
{
	list<GUN_EXPLORE>::iterator	i;
	D3DXMATRIX world,tran,ro,billboard,scale;
	D3DXMatrixScaling(&scale,10.0f,10.0f,10.0f);
	for(i = _explore_num.begin();i != _explore_num.end(); )
	{
		i->_time += _explore_cut;
		if(i->_time > _explore_life)
			i = _explore_num.erase(i);
		else
		{
			D3DXMatrixIdentity(&billboard);
			D3DXMatrixIdentity(&world);
			billboard._11 = view._11;
			billboard._13 = view._13;
			billboard._31 = view._31;
			billboard._33 = view._33;
			D3DXMatrixInverse(&billboard, NULL, &billboard);
			D3DXMatrixTranslation(&tran,i->_position.x,0,i->_position.z);
			world = world * billboard * scale * tran ;
			_billboard._device->SetTransform(D3DTS_WORLD,&world);
			_billboard._device->SetTexture(0,_texture_gun[i->_time]);
			_billboard.render();
			i++;
		}
	}
}
void GUN_CHILD::render()
{
	//�����ӵ�����
	TANK_GUN::render();
}
void GUN_CHILD::update(float time)
{
	//����������POINTSYSTEM�涨���붨�壬���Զ���Ϊ�պ���
}
//-----------------------------------------------
//�з���ս̹��
//-----------------------------------------------

//���캯��
TANK_ENEMY::TANK_ENEMY(LPDIRECT3DDEVICE9 device,const BOUNDINGBOX &box,wchar_t *name):MESHXFILE(device),
						_sign(NAZI),/*��ս��*/
						_mul(4),/*�ٶȷŴ���*///��ֵ�޸�ʱ����̹������ǽ��ת��ĺ�����Ҫ�޸ģ���Ϊ�β�û�����ֵ
						_hit_critical(100),/*��Чײ��*/
						_range(box),/*�����Χ*/
						_angle_life_time(180),/*̹��ת��ʣ�µ�ʱ��*/
						_run_time_min(2 * _angle_life_time),
						_run_time_max(5 * _angle_life_time),
						_time_cut(1)
{
	MESHXFILE::Init(name,2);				//��ʼ��̹�˵�����
	for(int i = 0;i < 8;i++)
		_imfor[i]._position = D3DXVECTOR3(0,0,0);		//�������̹�˵�ʱ���������̹��
														//Ϊ���ܹ���̹�������ӵ��˸�����
	for(int i = 0;i < 8;i++)
		add(_imfor[i]);
}
TANK_ENEMY::TANK_ENEMY(LPDIRECT3DDEVICE9 device,BOUNDINGBOX &box,int sign):MESHXFILE(device),
	_sign(sign),/*��ս��*/
	_hit_critical(100),/*��Чײ��*/
	_angle_life_time(180),/*̹��ת��ʣ�µ�ʱ��*/
	_run_time_min(2 * _angle_life_time),
	_run_time_max(5 * _angle_life_time),
	_time_cut(1),
	_range(box)
{

}
TANK_ENEMY::~TANK_ENEMY()									//��������
{

}

//���̹��
void TANK_ENEMY::add(PROPERTY &imfor)						//̹������������Ϸ��ʼʱ���̹�˵ĺ���
{
	int i,j,sign = 1;
	while(1)
	{
		i = (int)GetRandomFloat(0,3);						//����̹�˵����λ��
		j = (int)GetRandomFloat(0,3);
		i *= 1000;
		j *= 1000;
		if(i != j)
		{	//�۲��Ƿ���̹��λ�ڵ�ǰ����������λ�ã����У���������־��Ϊ0�����¼�������λ��
			for(int k = 0;k < 8;k++)
			{
				if(((i + _range._min.x - _imfor[k]._position.x) * (i + _range._min.x - _imfor[k]._position.x)
					+ (j + _range._min.z - _imfor[k]._position.z) * (j + _range._min.x - _imfor[k]._position.z))
					< ((float)_hit_critical * _hit_critical))
				{
					sign == 0;			//����������һ��̹��λ�ڼ��������ĵط�������ѭ��
					break;
				}
			}
		}
		//�Ѿ��ҵ����������ص�(i,j)��ʾ
		if(sign == 1)
		{		//-------------------------------------------------------------------
			imfor._position.x = _range._min.x / 3 + i + 1500;			//�ı�̹�˵�ˮƽλ��
			imfor._position.z = _range._min.z / 3 + j + 1500;
			i = i % 360;
			imfor._look = D3DXVECTOR3(0,0,-1);				//���»�ȡlook��right�������������ת
			imfor._right = D3DXVECTOR3(-1,0,0);
			D3DXMatrixRotationAxis(&imfor._rotation,&D3DXVECTOR3(0,1,0),i);				
			D3DXVec3TransformNormal(&imfor._look,&imfor._look,&imfor._rotation);		//�����ת���еķ���
			D3DXVec3TransformNormal(&imfor._right,&imfor._right,&imfor._rotation);
			D3DXVec3Normalize(&imfor._look,&imfor._look);								//�淶�����еķ���
			D3DXVec3Normalize(&imfor._right,&imfor._right);
			reset(imfor);																//��������̹�˵�����ǰ������
			break;
		}
		sign = 1;		//����ʧ�ܣ����¼�������λ��
	}
return ;
}
/*------------------------------
//��������̹�˵ĸ�������
------------------------------*/
void TANK_ENEMY::reset(PROPERTY &imfor)						//���¼���̹����ת�ĺ���
{
	D3DXVECTOR3 target;				//��¼��ת֮����ٶ�
	D3DXMATRIX	ro;
	imfor._angle = GetRandomFloat(0,D3DX_PI * 2);					//̹�˼�����ת�ĽǶ�
	D3DXMatrixRotationAxis(&ro,&D3DXVECTOR3(0,1,0),imfor._angle);
	D3DXVec3TransformNormal(&target,&imfor._look,&ro);
	target = _mul * target + imfor._position;
	if(target.x > _range._min.x + _hit_critical && target.x < _range._max.x - _hit_critical
		&& target.z > _range._min.z + _hit_critical  && target.z < _range._max.z - _hit_critical)
	{	//�����˺��ʵ����������¼������������ߵ�ʱ�������ʱ��
		imfor._angle_life = _angle_life_time;					//_angle_sign_time��Ĭ��ֵ�ڹ��캯���޸�
		imfor._life = GetRandomFloat(_run_time_min,_run_time_max);		//����̹�����ߵ����ʱ��
																//��ֵΪ-100ʱ��ʾ̹���Ѿ�����
		return ;
	}
	imfor._angle = D3DX_PI;
	imfor._angle_life = _angle_life_time;						//_angle_sign_time��Ĭ��ֵ�ڹ��캯���޸�
	imfor._life = GetRandomFloat(1 * _angle_life_time,4 * _angle_life_time);		//����̹�����ߵ����ʱ��
}
//����̹�˵�״̬
void TANK_ENEMY::update()			//����ϵͳǿ�Ƹ��£���ʹ�ò���
{
	D3DXMATRIX ro;
	for(int i = 0;i < 8;i++)
	{
		//���̹����ͬһ�����������ߵ�ʱ���Ƿ��������λ���Ѿ�������Χ
		//��Ҫע����ǣ�̹������Ҳ����˼�⣬Ӧ���޳�
		if((_imfor[i]._life <= 0 && _imfor[i]._life > -50)) 
		{
			reset(_imfor[i]);					//̹�˵Ļ��Χ��Ҫ����
		}
		else if(_imfor[i]._life == -100)		//̹�˱��򱬣�����һ��̹��
		{
			add(_imfor[i]);
		}
		else //̹�˴��
		{	
			if(_imfor[i]._angle_life >= 0)
			{	
				_imfor[i]._angle_life -= _time_cut;
				D3DXMatrixRotationAxis(&ro,&D3DXVECTOR3(0,1,0),_imfor[i]._angle / _angle_life_time);
				_imfor[i]._rotation = _imfor[i]._rotation * ro;			//����̹�˵��Ӿ���ת��
																		//�����������ʱ��ʵ��һ���ܷ���д
				D3DXVec3TransformNormal(&_imfor[i]._look,&_imfor[i]._look,&ro);
				_imfor[i]._velocity = _mul * _imfor[i]._look;
			}
			_imfor[i]._life -= _time_cut;
			_imfor[i]._position += _imfor[i]._velocity;
		}
	}
}

//��Ⱦ + ����
void TANK_ENEMY::render()									//�������ез�̹�˵ĺ���
{
	update();					//����ϵͳǿ�Ƹ��£���ʹ�ò���
	D3DXMATRIX world,tran,scale;
	D3DXMatrixScaling(&scale,0.0005f,0.0005f,0.0005f);
	for(int i = 0;i < 8;i++)
	{
		D3DXMatrixIdentity(&world);
		D3DXMatrixTranslation(&tran,_imfor[i]._position.x,_imfor[i]._position.y,_imfor[i]._position.z);
		world = world * scale * _imfor[i]._rotation * tran;
		_device->SetTransform(D3DTS_WORLD,&world);
		for(DWORD k = 0;k < _NumMaterials;k++)
		{
			_device->SetMaterial(&_materials[k]);
			_device->SetTexture(0,_textures[k]);
			_mesh->DrawSubset(k);
		}
	}
}