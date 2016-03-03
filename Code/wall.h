# ifndef _WALL_H
# define _WALL_H

# include "tank.h"

struct IMFOR_WALL			//墙体的属性
{
	D3DXVECTOR3 _position;
	float		_life;
	MESHXFILE	*_mesh;
};
struct BOSS_STRUCT
{
	D3DXVECTOR3 _position;
	D3DXVECTOR3 _look;
	D3DXVECTOR3	_velocity;
	D3DXVECTOR3 _target;
	D3DXMATRIX	_rotation;
	bool		_alive;
	int			_life;
	int			_run_time;
	int			_run_cut;
	int			_mul;
	MESHXFILE	*_mesh;
};
class WALL
{
protected:
	list<IMFOR_WALL> _imfor;
	LPDIRECT3DDEVICE9	_device;
	wchar_t		(*_x_name)[50];
	const BOUNDINGBOX _box;
	const int	_random;
	const int   _mul;
	BOSS_STRUCT	_boss;
	const float	_distance;
	const float _life_time;
	const float _life_cut;
	const int	_life_mul;
public:
	//传入的二级指针为X文件提供了随机选择范围
	//最后为random提供一个常随机数，保证在使用过程不被修改
	WALL(LPDIRECT3DDEVICE9,wchar_t (*x_name)[50],BOUNDINGBOX&box,int ran);
	~WALL();
	//传入当前需要重新设置的墙体的迭代器与所有坦克所在位置
	//主坦克，敌方坦克，第三方坦克，注意：第三方坦克为类的成员
	void reset(list<IMFOR_WALL>::iterator index,PROPERTY *pro,TANK *host);
	//测试墙体附近的敌方与第三方坦克是否需要重新设置行走路径
	//测试子弹是否将墙体打爆，只有玩家的子弹能够打爆墙体
	void test(PROPERTY *imfor,GUN_CHILD *,TANK *);
	//比较函数,由于需要大量与敌方坦克比较，估计定义此函数
	void compare(PROPERTY *imfor,list<IMFOR_WALL>::iterator index);
	void compare(list<IMFOR_WALL>::iterator index);
	//更新第三方坦克与墙体位置等消息，注意，需要设置发射子弹
	void update(GUN_CHILD *gun,TANK_ENEMY *pro,TANK *host);
	void render();			//由于需要添加子弹，所以传入子弹的接口

	void reset_boss(const D3DXVECTOR3 &);
	void update_boss(const D3DXVECTOR3 &);

	friend void draw_tank_gun();
	friend class EXPLORE_BILLBOARD;
};

# endif