# include "wall.h"
extern bool	tree_or_explore;
//传入的二级指针为X文件提供了随机选择范围
//外界传入随机数的同时，需要将其对char数组的行数取余
//随机数即存储了数组的维数，同时存储了墙体的数量 / 倍数
WALL::WALL(LPDIRECT3DDEVICE9 device,wchar_t (*x_name)[50],BOUNDINGBOX &box,int ran):
			_random(ran),			//取余是为了避免随机数过大
			_distance(10000),		//与墙体发生撞击的距离的平方
			_life_time(500),			//再生墙体的生命值
			_life_cut(1)			//墙体收到打击时生命值的衰减系数
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
	wall._life = _life_time;				//初始化一个虚拟的wall
	wall._position = D3DXVECTOR3(0,0,0);
	wall._mesh = new MESHXFILE(device);
	j = GetRandomFloat(0,_random);
	wall._mesh->Init(x_name[j],4);
	a._position = D3DXVECTOR3(0,0,0);
	PROPERTY enemy[8];				
	for(int i = 0;i < 8;i++)		//虚拟敌方坦克的信息
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
	//BOSS坦克的信息
	_boss._target = D3DXVECTOR3(0,0,-1);		//目标
	_boss._position = D3DXVECTOR3(5000,5000,5000);		//位置
	_boss._look = D3DXVECTOR3(0,0,-1);			//观察方向
	D3DXMatrixIdentity(&_boss._rotation);
	_boss._mesh = new MESHXFILE(device);		//x文件
	_boss._mesh->Init(x_name[5],BOSS);			//初始化BOSS的x文件与纹理
	_boss._velocity = _boss._look * 0;
	_boss._run_time = 0;
	_boss._run_cut = 1;
	_boss._alive = false;
	_boss._mul = 10;								
	_boss._life = 0;	//初始化生命值
}
WALL::~WALL()		//析构函数
{}
//传入当前需要重新设置的墙体的迭代器与所有坦克所在位置
//主坦克，敌方坦克，第三方坦克，注意：第三方坦克为类的成员
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
		//判断是否与第三方坦克位置重复--------------
		
		//是否与敌方坦克所在位置重复
		for(p = pro;p != pro + 8;p++)
			if((x - p->_position.x) * (x - p->_position.x) + 
				(z - p->_position.z) * (z - p->_position.z) < _distance)
				sign = 0;
		if(sign == 1)		//最后，若主坦克也没有与再生点重复，该点可用
			if((x - host->_position.x) * (x - host->_position.x) + 
				(z - host->_position.z) * (z - host->_position.z) > _distance)
			{
				index->_position = D3DXVECTOR3(x,0,z);		//位置
				index->_life = _life_time;					//声明
				index->_mesh->mesh_release();
				sign = (int)GetRandomFloat(0,20) % _random;	//x文件
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
		if(_boss._run_time <= 0)			//若坦克存活，行走时间小于0
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
	D3DXMatrixRotationAxis(&_boss._rotation,&D3DXVECTOR3(0,1,0),angle);		//记录旋转角度
}
//测试墙体附近的敌方与第三方坦克是否需要重新设置行走路径
//测试子弹是否将墙体打爆，只有玩家的子弹能够打爆墙体
void WALL::test(PROPERTY *imfor,GUN_CHILD *gun,TANK *host)
{
	list<IMFOR_WALL>::iterator index;		//墙体
	list<ATTRIBUTE>::iterator par ;			//包含有子弹位置
	list<char>::iterator who;				//子弹归属
	list<D3DXMATRIX>::iterator ro;			//子弹旋转矩阵

	//用一面墙壁与所有敌方坦克比较
	//再将其与所有子弹比较

	for(index = _imfor.begin();index != _imfor.end();index++)
	{
		//判断敌方坦克与墙壁的关系
		compare(imfor,index);
		//判断子弹与墙体的关系，只有玩家的坦克会对墙体造成伤害
		//由于在子弹本类中，子弹消失将这三者一起带走，并且增加爆炸，所以需要同样操作
		par = gun->_particles.begin();				//确定子弹位置
		who = gun->_who.begin();					//确定子弹归属
		ro  = gun->_velocity_view.begin();			//确定子弹旋转矩阵
		for( ;par != gun->_particles.end(); )		//子弹容器
		{
			if((index->_position.x - par->_position.x) * (index->_position.x - par->_position.x) + 
				(index->_position.z - par->_position.z) * (index->_position.z - par->_position.z) < 
				_distance * 5)			//子弹撞上了墙体
			{
				if((char)(*who) == ALLIED)				//玩家子弹，墙壁掉血
					index->_life -= _life_cut;

				gun->add_explore(par->_position);		//剔除子弹的相关属性，并增加爆炸
				par = gun->_particles.erase(par);		
				who = gun->_who.erase(who);
				ro	= gun->_velocity_view.erase(ro);
			}

			//子弹撞上boss坦克
			
			else if((_boss._alive == true) && (_boss._position.x - par->_position.x) * 
				(_boss._position.x - par->_position.x) + 
				(_boss._position.z - par->_position.z) * 
				(_boss._position.z - par->_position.z) 
				< _distance * 20)
			{		if((char)(*who) != BOSS)					//非boss子弹打中boss
					{	_boss._life -= _life_cut;
						gun->add_explore(par->_position);		//剔除子弹的相关属性，并增加爆炸
						par = gun->_particles.erase(par);		
						who = gun->_who.erase(who);
						ro	= gun->_velocity_view.erase(ro);
						if(_boss._life <= 0)					//boss死亡
							host->_life += host->_life_add * host->_life_cut;
					}
			}
			else
				par++,who++,ro++;
		}
	}
}

//更新第三方坦克与墙体位置等消息，注意，需要设置发射子弹
void WALL::update(GUN_CHILD *gun,TANK_ENEMY *pro,TANK *host)
{
	if((_boss._position.x - host->_position.x) * (_boss._position.x - host->_position.x) + 
		(_boss._position.z - host->_position.z) * (_boss._position.z - host->_position.z) < (_distance))
	{
		host->_life = 0;
		return;
	}
	//更新墙壁
	list<IMFOR_WALL>::iterator index;
	for(index = _imfor.begin();index != _imfor.end();index++)
	{
		if(index->_life <= 0)			//若墙壁已经打爆，根据现场所有坦克的位置确认新的墙壁的位置
		{	
			int i;
			i = GetRandomFloat(0,13);
			if(i < 4)
			{	
				if(_boss._alive == false)		//打爆墙壁时，若boss不存在，出现
				{	_boss._alive = true;
					_boss._position = index->_position;
					_boss._life = _life_time * _life_mul;
					_boss._run_time = 0;
				}
			}
			else if(i < 5)
			{	
				if(pro->_mul > 1)			//敌方降低
					pro->_mul -= 1;
			}
			else if(i < 7)				//我方坦克速度增加
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
		index->_mesh->render();		//只绘制，不管任何变换
	}
	//绘制boss坦克
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
	//每次比较8只敌方坦克
	for(int i = 0;i < 8;i++)
	{
		if((index->_position.x - imfor[i]._position.x) * (index->_position.x - imfor[i]._position.x) + 
			(index->_position.z - imfor[i]._position.z) * (index->_position.z - imfor[i]._position.z) 
			< _distance * 80)		//坦克即将撞击墙壁，若坦克不是撞向墙壁，可以通过
		{
			//坦克与墙壁的连线与坦克前进的方向成锐角，坦克必须拐弯
			if((D3DXVec3Dot(&(index->_position - imfor[i]._position),&imfor[i]._velocity)) > 0)
			{
				D3DXVECTOR3 ve = imfor[i]._velocity;
				//第二个参数需要与坦克行走的放大倍数相等,时间也相等
				//根据左手法则可以知道，有时候坦克在撞向墙壁时该函数需要调用两次
				D3DXVec3Cross(&imfor[i]._velocity,&D3DXVECTOR3(0,10,0),&imfor[i]._velocity);
				imfor[i]._angle = D3DX_PI / 2;
				imfor[i]._angle_life = 360;								//_angle_sign_time的默认值在构造函数修改
				//imfor[i]._life = GetRandomFloat(1 * 360,4 * 360);		//生成坦克行走的随机时间
			}
		}
	}
}