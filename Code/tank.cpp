
# include "tank.h"
# include <stdio.h>

MESHXFILE::MESHXFILE(LPDIRECT3DDEVICE9 device)
{
	_device				= device;			//direct3D设备接口
	_mesh				= NULL;				//网格对象
	_textures			= NULL;				//网格的纹理信息
	_NumMaterials		= 0;				//材质的数目
	pAdjBuffer = NULL;
	pMtrlBuffer = NULL;
}

void MESHXFILE::Init(wchar_t *xfile,int i)	//传入x文件名，创建网格对象与纹理
{
	//读取x文件中的网格数据
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
	//读取材质和纹理数据
	wchar_t a[10][50] = {{},
						{L"KRLbody.bmp"},			//主战坦克
						{L"PANZAUSA.JPG"},//敌方坦克
						{L"FEUROASH.JPG"},			//子弹
						{L""},	//墙壁
						{L"3 (1).JPG"}	//BOSS	
	};	
	D3DXMATERIAL *pMaterial = (D3DXMATERIAL*)pMtrlBuffer->GetBufferPointer();
	_materials = new D3DMATERIAL9[_NumMaterials];			//创建记录材质信息的对象
	_textures = new LPDIRECT3DTEXTURE9[_NumMaterials];		//创建记录纹理的指针

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
			a[j],										//从pMtrBuffer得到起始地址
			&_textures[i]);								//存储纹理
		
		else
			D3DXCreateTextureFromFileA(_device,
				pMaterial[i].pTextureFilename,				//从pMtrBuffer得到起始地址
				&_textures[i]);								//存储纹理
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
//坦克
//----------------------------------------
TANK::TANK(LPDIRECT3DDEVICE9 device):MESHXFILE(device),
									_position(0.0f,0.0f,0.0f),_look(0.0f,0.0f,-1.0f),_right(-1.0f,0.0f,0.0f),
									_mul(10),_turn_scale(0.1),_life_cut(3),_life_time(5000),_life_add(200)
{
	_rotation_sign		= 0;
	_life				= _life_time;
	D3DXMatrixIdentity(&_rotation);
}
void TANK::render()				//绘制网格对象
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
	//				缩放		旋转		转弯			位置
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

//坦克前进方向
void TANK::TranslationFront(CAMERA &camera)
{
	_position += _mul * _look;
	camera.Translation(_look * _mul);
}
//后退
void TANK::TranslationBack(CAMERA &camera)
{
	_position -= _mul * _look;
	camera.Translation(_look * (-_mul));
}
void TANK::TurnRight(CAMERA &camera)
{
	const float angles = D3DX_PI / 180.0f;
	float len_look,len_right,len_target,camera_y = camera.position.y,target_y = camera.target.y;

	_rotation_sign = 10;				//该值在渲染中体现了坦克旋转时摆动的角度，向右需要比向左大

	len_right = _look.z * camera.position.x - _look.x * camera.position.z - _look.z * _position.x + _look.x * _position.z;
	len_right = len_right * len_right / (_look.z * _look.z + _look.x * _look.x);
	len_right = sqrt(len_right);														//摄像机到look轴的水平距离

	len_look = _right.z * camera.position.x - _right.x * camera.position.z - _right.z * _position.x + _right.x * _position.z;
	len_look = len_look * len_look / (_right.z * _right.z + _right.x * _right.x);
	len_look = sqrt(len_look);															//摄像机到right轴的水平距离
										
	len_target = sqrt((camera.target.z - _position.z) * (camera.target.z - _position.z) + 
		(camera.target.x - _position.x) * (camera.target.x - _position.x));				//摄像机观察点与坦克的距离

	_position += _mul * _look ;														//计算坦克的位置
	_position += _mul * _right * _turn_scale;

	D3DXMATRIX mid;
	D3DXMatrixRotationAxis(&mid,&D3DXVECTOR3(0.0,1.0f,0.0f),angles);
	_rotation  = _rotation * mid;					//记录坦克旋转的角度

	D3DXVec3TransformNormal(&_right,&_right,&mid);	//将坦克的right和look分量旋转同样的角度
	D3DXVec3TransformNormal(&_look,&_look,&mid);
	D3DXVec3Normalize(&_right,&_right);
	D3DXVec3Normalize(&_look,&_look);

	D3DXVec3TransformNormal(&camera.right,&camera.right,&mid);	//将摄像机的right和look分量旋转同样的角度
	D3DXVec3TransformNormal(&camera.look,&camera.look,&mid);
	D3DXVec3Normalize(&camera.right,&camera.right);
	D3DXVec3Normalize(&camera.look,&camera.look);


	camera.position = _position - _look * len_look - _right * len_right;				//计算摄像机的位置
	camera.position.y = camera_y;

	camera.target = _position + len_target * _look;										//计算摄像机的观察点
	camera.target.y = target_y;
}
void TANK::TurnLeft(CAMERA &camera)
{
	const float angles = -D3DX_PI / 180.0f;

	float len_look,len_right,len_target,camera_y = camera.position.y,target_y = camera.target.y;

	_rotation_sign = -2;		//该值在渲染中体现了坦克旋转时摆动的角度，向右需要比向左大

	len_right = _look.z * camera.position.x - _look.x * camera.position.z - _look.z * _position.x + _look.x * _position.z;
	len_right = len_right * len_right / (_look.z * _look.z + _look.x * _look.x);
	len_right = sqrt(len_right);														//摄像机到look轴的水平距离

	len_look = _right.z * camera.position.x - _right.x * camera.position.z - _right.z * _position.x + _right.x * _position.z;
	len_look = len_look * len_look / (_right.z * _right.z + _right.x * _right.x);
	len_look = sqrt(len_look);															//摄像机到right轴的水平距离
										
	len_target = sqrt((camera.target.z - _position.z) * (camera.target.z - _position.z) + 
		(camera.target.x - _position.x) * (camera.target.x - _position.x));				//摄像机观察点与坦克的距离

	_position += _mul * _look;														//计算坦克的位置
	_position += _mul * (-_right) * _turn_scale;

	D3DXMATRIX mid;
	D3DXMatrixRotationAxis(&mid,&D3DXVECTOR3(0.0,1.0f,0.0f),angles);
	_rotation  = _rotation * mid;					//记录坦克旋转的角度

	D3DXVec3TransformNormal(&_right,&_right,&mid);	//将坦克的right和look分量旋转同样的角度
	D3DXVec3TransformNormal(&_look,&_look,&mid);
	D3DXVec3Normalize(&_right,&_right);
	D3DXVec3Normalize(&_look,&_look);

	D3DXVec3TransformNormal(&camera.right,&camera.right,&mid);	//将摄像机的right和look分量旋转同样的角度
	D3DXVec3TransformNormal(&camera.look,&camera.look,&mid);
	D3DXVec3Normalize(&camera.right,&camera.right);
	D3DXVec3Normalize(&camera.look,&camera.look);

	camera.position = _position - _look * len_look - _right * len_right;				//计算摄像机的位置
	camera.position.y = camera_y;

	camera.target = _position + len_target * _look;										//计算摄像机的观察点
	camera.target.y = target_y;
}

//----------------------------------------------------------
//子弹类
//----------------------------------------------------------
//三者的构造函数，同时调用基类的初始化函数
TANK_GUN::TANK_GUN(LPDIRECT3DDEVICE9 device,wchar_t *name_gun,wchar_t *name_point):_exit_time(10.0f),
		MESHXFILE(device),POINTSYSTEM(),		//调用基类的构造函数
		_mul(200),_time_cut(1)
{
	_vertexbuf_size = 512;
	MESHXFILE::Init(name_gun,3);
	POINTSYSTEM::init(device,name_point);
}
//增加子弹以及爆炸				*****此处使用到子弹的寿命，子弹爆炸时也使用了子弹的寿命，等于0的点在爆炸使用												
//									但是爆炸后，需要将子弹的寿命再次降低，或者赋负值，才能在此处绝处逢生
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
		if(i->_life_time <= 0)				//若导弹生命结束，引爆，并剔除
		{	
			i = _particles.erase(i);
			j = _velocity_view.erase(j);
		}
		else
			i++,j++;
	}
return ;
}
//只绘制了子弹的飞行轨迹
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

void TANK_GUN::reset_particle(ATTRIBUTE *attribute)	//暂时更新为空函数
{}

//-----------------------------------------------
//派生子弹类
//-----------------------------------------------
GUN_CHILD::GUN_CHILD(LPDIRECT3DDEVICE9 device,wchar_t *name_gun,wchar_t *name_point):
				TANK_GUN(device,name_gun,name_point),
				_explore_life(24),		//爆炸的总时间
				_explore_cut(1),		//爆炸时间的缩减速度
				_hit_critical(10000)	//命中误差平方值
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
	
	for(int i = 0;i < 25;i++)			//创建纹理
		D3DXCreateTextureFromFile(POINTSYSTEM::_device,explore[i],&_texture_gun[i]);  
}
GUN_CHILD::~GUN_CHILD()
{

}
//默认增加盟军的子弹，因为盟军没有标志
//即盟军发射子弹时，无须传入子弹标志，但是地方发射子弹时，需要规定是哪一方的子弹
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
		if((char)(*k) == ALLIED)		//这个值很有疑问，调试的时候看一下
		{
			if((is_explore(enemy,host,i) == false) && (i->_life_time > 0))				//子弹存活,没有爆炸也没有结束生命
			{
				i->_life_time -= _time_cut;				//子弹基类的时间衰减系数
				i->_position += _time_cut * i->_velocity;
				i++,j++,k++;
				continue;
			}
		}
		else if((char)(*k) == NAZI || (char)(*k) == BOSS)			
		{
			if((is_explore(host,i) == false) && (i->_life_time > 0))				//子弹存活,没有爆炸也没有结束生命
			{
				i->_life_time -= _time_cut;
				i->_position += _time_cut * i->_velocity;
				i++,j++,k++;
				continue;
			}
		}
		add_explore(i->_position);
		i = _particles.erase(i);		//子弹可能打中坦克，也有可能自行爆炸
		j = _velocity_view.erase(j);
		k = _who.erase(k);
	}
}
bool GUN_CHILD::is_explore(TANK *host,list<ATTRIBUTE>::iterator i)		//敌人子弹判断是否打中盟军				
{
	if((host->_position.x - i->_position.x) * (host->_position.x - i->_position.x) + 
		(host->_position.z - i->_position.z) * (host->_position.z - i->_position.z) 
		< _hit_critical)
	{	//命中盟军
		host->_life -= _time_cut * 50;			//我方坦克生命值降低
		add_explore(host->_position);
		return true;
	}
return false;
}
bool GUN_CHILD::is_explore(PROPERTY *imfor,TANK *host,list<ATTRIBUTE>::iterator i)	//盟军子弹判断是否打中敌人						
{
	//所有坦克的位置进行比较
	PROPERTY *p;
	byte sign = 0;
	float critical,max = 100000000000000000;
	for(int j = 0;j < 8;j++)			//寻找与子弹最近的坦克
	{	//计算坦克与子弹的距离
		critical = (imfor[j]._position.x - i->_position.x) * (imfor[j]._position.x - i->_position.x) + 
			(imfor[j]._position.z - i->_position.z) * (imfor[j]._position.z - i->_position.z);
		if(critical < _hit_critical)	//子弹命中坦克
			if(critical < max)			//是否是最近的坦克
			{	
				max = critical;
				sign = 1;
				p = &imfor[j];
			}
	}
	if(sign == 1)				//打中BOSS不会恢复血
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
void GUN_CHILD::explore(const D3DXMATRIX &view)					//在指定的地点产生爆炸
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
	//绘制子弹行踪
	TANK_GUN::render();
}
void GUN_CHILD::update(float time)
{
	//本函数由于POINTSYSTEM规定必须定义，所以定义为空函数
}
//-----------------------------------------------
//敌方作战坦克
//-----------------------------------------------

//构造函数
TANK_ENEMY::TANK_ENEMY(LPDIRECT3DDEVICE9 device,const BOUNDINGBOX &box,wchar_t *name):MESHXFILE(device),
						_sign(NAZI),/*作战方*/
						_mul(4),/*速度放大倍数*///该值修改时，在坦克遇到墙壁转弯的函数需要修改，因为形参没有这个值
						_hit_critical(100),/*有效撞击*/
						_range(box),/*最大活动范围*/
						_angle_life_time(180),/*坦克转弯剩下的时间*/
						_run_time_min(2 * _angle_life_time),
						_run_time_max(5 * _angle_life_time),
						_time_cut(1)
{
	MESHXFILE::Init(name,2);				//初始化坦克的网格
	for(int i = 0;i < 8;i++)
		_imfor[i]._position = D3DXVECTOR3(0,0,0);		//由于添加坦克的时候会随机添加坦克
														//为了能够将坦克随机添加到八个顶点
	for(int i = 0;i < 8;i++)
		add(_imfor[i]);
}
TANK_ENEMY::TANK_ENEMY(LPDIRECT3DDEVICE9 device,BOUNDINGBOX &box,int sign):MESHXFILE(device),
	_sign(sign),/*作战方*/
	_hit_critical(100),/*有效撞击*/
	_angle_life_time(180),/*坦克转弯剩下的时间*/
	_run_time_min(2 * _angle_life_time),
	_run_time_max(5 * _angle_life_time),
	_time_cut(1),
	_range(box)
{

}
TANK_ENEMY::~TANK_ENEMY()									//析构函数
{

}

//添加坦克
void TANK_ENEMY::add(PROPERTY &imfor)						//坦克死亡或者游戏开始时添加坦克的函数
{
	int i,j,sign = 1;
	while(1)
	{
		i = (int)GetRandomFloat(0,3);						//计算坦克的随机位置
		j = (int)GetRandomFloat(0,3);
		i *= 1000;
		j *= 1000;
		if(i != j)
		{	//观察是否有坦克位于当前即将重生的位置，若有，则将重生标志改为0，重新计算重生位置
			for(int k = 0;k < 8;k++)
			{
				if(((i + _range._min.x - _imfor[k]._position.x) * (i + _range._min.x - _imfor[k]._position.x)
					+ (j + _range._min.z - _imfor[k]._position.z) * (j + _range._min.x - _imfor[k]._position.z))
					< ((float)_hit_critical * _hit_critical))
				{
					sign == 0;			//发现至少有一辆坦克位于即将再生的地方，跳出循环
					break;
				}
			}
		}
		//已经找到合适重生地点(i,j)表示
		if(sign == 1)
		{		//-------------------------------------------------------------------
			imfor._position.x = _range._min.x / 3 + i + 1500;			//改变坦克的水平位置
			imfor._position.z = _range._min.z / 3 + j + 1500;
			i = i % 360;
			imfor._look = D3DXVECTOR3(0,0,-1);				//重新获取look与right分量，并随机旋转
			imfor._right = D3DXVECTOR3(-1,0,0);
			D3DXMatrixRotationAxis(&imfor._rotation,&D3DXVECTOR3(0,1,0),i);				
			D3DXVec3TransformNormal(&imfor._look,&imfor._look,&imfor._rotation);		//随机旋转已有的方向
			D3DXVec3TransformNormal(&imfor._right,&imfor._right,&imfor._rotation);
			D3DXVec3Normalize(&imfor._look,&imfor._look);								//规范化已有的方向
			D3DXVec3Normalize(&imfor._right,&imfor._right);
			reset(imfor);																//重新设置坦克的声明前进方向
			break;
		}
		sign = 1;		//重生失败，重新计算再生位置
	}
return ;
}
/*------------------------------
//重新设置坦克的各个分量
------------------------------*/
void TANK_ENEMY::reset(PROPERTY &imfor)						//重新计算坦克旋转的函数
{
	D3DXVECTOR3 target;				//记录旋转之后的速度
	D3DXMATRIX	ro;
	imfor._angle = GetRandomFloat(0,D3DX_PI * 2);					//坦克即将旋转的角度
	D3DXMatrixRotationAxis(&ro,&D3DXVECTOR3(0,1,0),imfor._angle);
	D3DXVec3TransformNormal(&target,&imfor._look,&ro);
	target = _mul * target + imfor._position;
	if(target.x > _range._min.x + _hit_critical && target.x < _range._max.x - _hit_critical
		&& target.z > _range._min.z + _hit_critical  && target.z < _range._max.z - _hit_critical)
	{	//产生了合适的随机数，记录朝这个方向行走的时间与拐弯时间
		imfor._angle_life = _angle_life_time;					//_angle_sign_time的默认值在构造函数修改
		imfor._life = GetRandomFloat(_run_time_min,_run_time_max);		//生成坦克行走的随机时间
																//该值为-100时表示坦克已经死亡
		return ;
	}
	imfor._angle = D3DX_PI;
	imfor._angle_life = _angle_life_time;						//_angle_sign_time的默认值在构造函数修改
	imfor._life = GetRandomFloat(1 * _angle_life_time,4 * _angle_life_time);		//生成坦克行走的随机时间
}
//更新坦克的状态
void TANK_ENEMY::update()			//粒子系统强制更新，不使用参数
{
	D3DXMATRIX ro;
	for(int i = 0;i < 8;i++)
	{
		//检测坦克在同一个方向上行走的时间是否过长或者位置已经超出范围
		//需要注意的是，坦克死亡也是如此检测，应该剔除
		if((_imfor[i]._life <= 0 && _imfor[i]._life > -50)) 
		{
			reset(_imfor[i]);					//坦克的活动范围需要更新
		}
		else if(_imfor[i]._life == -100)		//坦克被打爆，增加一辆坦克
		{
			add(_imfor[i]);
		}
		else //坦克存活
		{	
			if(_imfor[i]._angle_life >= 0)
			{	
				_imfor[i]._angle_life -= _time_cut;
				D3DXMatrixRotationAxis(&ro,&D3DXVECTOR3(0,1,0),_imfor[i]._angle / _angle_life_time);
				_imfor[i]._rotation = _imfor[i]._rotation * ro;			//补充坦克的视觉旋转量
																		//当程序可运行时，实验一下能否缩写
				D3DXVec3TransformNormal(&_imfor[i]._look,&_imfor[i]._look,&ro);
				_imfor[i]._velocity = _mul * _imfor[i]._look;
			}
			_imfor[i]._life -= _time_cut;
			_imfor[i]._position += _imfor[i]._velocity;
		}
	}
}

//渲染 + 更新
void TANK_ENEMY::render()									//绘制所有敌方坦克的函数
{
	update();					//粒子系统强制更新，不使用参数
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