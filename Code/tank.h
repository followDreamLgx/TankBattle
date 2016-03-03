# ifndef _TANK_H
# define _TANK_H

# include "head.h"
# include "camera.h"
# include "pointsystem.h"
# include "billboard.h"


# define NAZI	1
# define ALLIED 2
# define OTHERS	3
# define BOSS	5
class WALL;
//-------------------------
class MESHXFILE
{
protected:
	LPD3DXBUFFER pAdjBuffer;
	LPD3DXBUFFER pMtrlBuffer;

	LPDIRECT3DDEVICE9	_device;			//direct3D设备接口
	LPD3DXMESH			_mesh;				//网格对象
	D3DMATERIAL9		*_materials;		//网格的材质信息
	LPDIRECT3DTEXTURE9  *_textures;			//网格的纹理信息
	DWORD				_NumMaterials;		//材质的数目
public:
	MESHXFILE(LPDIRECT3DDEVICE9);
	virtual void mesh_release();
	virtual void Init(wchar_t*,int i);
	virtual void read_texture(int i);
	virtual void render();
	friend class WALL;
};
extern int select();
class TANK:public MESHXFILE
{
protected:
	//记录坦克的位置，坦克只能在ZOX平面运动，所以向上的向量始终不变
	D3DXVECTOR3			_position;			//记录摄像机，即坦克的位置
	D3DXVECTOR3			_look;				//坦克前进的方向
	D3DXVECTOR3			_right;				//坦克的右方向

	D3DXMATRIX			_rotation;			//记录坦克旋转的角度
	float				_rotation_sign;		//记录坦克是否进行了旋转
	int					_life;				//主坦克寿命
	const int			_life_add;
	const int			_life_time;
	const int			_life_cut;
	float				_mul;				//速度放大倍数
	const float			_turn_scale;		//坦克拐弯时，侧向与前进的比例
public:
	TANK(LPDIRECT3DDEVICE9);
	virtual void render();					//绘制网格对象

	//坦克平移
	virtual void TranslationFront(CAMERA &camera);
	virtual void TranslationBack(CAMERA &camera);
	//坦克旋转
	virtual void TurnRight(CAMERA &camera);
	virtual void TurnLeft(CAMERA &camera);
	//---------友元----------------//
	friend int select();
	friend class GUN_CHILD;
	friend class WALL;
	friend class EXPLORE_BILLBOARD;
	friend void draw_tank_gun();
};
//---------------------------------
//子弹
//---------------------------------
class TANK_GUN:public MESHXFILE,public POINTSYSTEM
{
protected:
	list<D3DXMATRIX>	_velocity_view;					//视觉上导弹方向
	const int			_exit_time;
	const int			_time_cut;						
	const float			_mul;							//速度的放大倍数
public:
	//三者的构造函数，同时调用基类的初始化函数
	TANK_GUN(LPDIRECT3DDEVICE9 device,wchar_t *name_gun,wchar_t *name_point);					
	//增加子弹													
	virtual void add_gun(D3DXVECTOR3 &position,D3DXVECTOR3 &look,D3DXMATRIX &velocity_view);

	virtual void reset_particle(ATTRIBUTE *attribute);	//暂时更新为空函数
	virtual void update(float time);					//粒子系统强制更新
	virtual void render();
};	
//----------------------------------
//派生子弹类
//----------------------------------
struct GUN_EXPLORE 
{
	int _time;
	D3DXVECTOR3 _position;
};
class GUN_CHILD:public TANK_GUN
{
protected:
	list<char>			_who;				//记录子弹的归属
	list<GUN_EXPLORE>	_explore_num;		//需要渲染爆炸的总数
	BILLBOARD			_billboard;			//广告牌
	LPDIRECT3DTEXTURE9	_texture_gun[25];
	const float			_explore_life;		//子弹爆炸的最终时间，
	const float			_explore_cut;		//子弹爆炸时间的增长速度
	const float			_hit_critical;		//命中误差平方值，距离小于此误差视为命中
public:
	GUN_CHILD(LPDIRECT3DDEVICE9 device,wchar_t *name_gun,wchar_t *name_point);
	~GUN_CHILD();
	//默认增加盟军的子弹，因为盟军没有标志
	void add(D3DXVECTOR3 &position,D3DXVECTOR3 &look,D3DXMATRIX &velocity_view,char who = ALLIED);
	virtual void update(float time);
	void update(TANK *,PROPERTY*);
	virtual void render();
	void add_explore(D3DXVECTOR3 &position);
	bool is_explore(TANK*,list<ATTRIBUTE>::iterator);		//敌人子弹判断是否打中盟军
	bool is_explore(PROPERTY *,TANK *,list<ATTRIBUTE>::iterator);	//盟军子弹判断是否打中敌人

	void explore(const D3DXMATRIX &);						//在指定的地点产生爆炸
	//---------友元----------------///
	friend void draw_tank_gun();
	friend int select();
	friend class WALL;
};
//-------------------------------------------
//坦克
//-------------------------------------------

//敌方坦克属性

class TANK_ENEMY:public MESHXFILE
{
protected:
	const float			_angle_life_time;		//坦克转弯所剩下的时间		
	const float			_run_time_min;			//行走的最小值
	const float			_run_time_max;			//行走的最大值
	const float			_time_cut;				//时间的衰减速度
	const int			_sign;					//坦克所属方标志
	int					_mul;					//坦克速度的放大倍数
	const float			_hit_critical;			//坦克与墙壁的距离小于该值时认为坦克与物体发生碰撞
	const BOUNDINGBOX	_range;					//坦克最大活动范围
	PROPERTY			_imfor[8];				//八辆敌方坦克的信息
public:
	TANK_ENEMY(LPDIRECT3DDEVICE9,BOUNDINGBOX &,int sign);
	TANK_ENEMY(LPDIRECT3DDEVICE9,const BOUNDINGBOX &box,wchar_t *);					//构造函数
	~TANK_ENEMY();									//析构函数

	void add(PROPERTY &);							//坦克死亡或者游戏开始时添加坦克的函数
	void reset(PROPERTY &);							//重新计算坦克旋转的函数
	void update();									//更新坦克信息的函数	
	void render();									//绘制所有敌方坦克的函数
	//-------------友元-------------------------//
	friend class GUN_CHILD;
	friend void draw_tank_gun();
	friend class WALL;
	friend void environment();
};

class EXPLORE_BILLBOARD:public BILLBOARD
{
protected:
	LPDIRECT3DTEXTURE9	_texture_ex[25];
	const int			_render_life_amount;
	int					_render_life;
	const int			_render_add;
	const float			_distance;
public:
	EXPLORE_BILLBOARD(LPDIRECT3DDEVICE9 device,const BOUNDINGBOX &box);
	~EXPLORE_BILLBOARD();
	virtual void init();
	virtual void render(const D3DXMATRIX &view);
	virtual void test(PROPERTY *,TANK *,WALL *);

	//-----------------友元------------------------//
	friend class TANK;
};
# endif