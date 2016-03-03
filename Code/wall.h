# ifndef _WALL_H
# define _WALL_H

# include "tank.h"

struct IMFOR_WALL			//ǽ�������
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
	//����Ķ���ָ��ΪX�ļ��ṩ�����ѡ��Χ
	//���Ϊrandom�ṩһ�������������֤��ʹ�ù��̲����޸�
	WALL(LPDIRECT3DDEVICE9,wchar_t (*x_name)[50],BOUNDINGBOX&box,int ran);
	~WALL();
	//���뵱ǰ��Ҫ�������õ�ǽ��ĵ�����������̹������λ��
	//��̹�ˣ��з�̹�ˣ�������̹�ˣ�ע�⣺������̹��Ϊ��ĳ�Ա
	void reset(list<IMFOR_WALL>::iterator index,PROPERTY *pro,TANK *host);
	//����ǽ�帽���ĵз��������̹���Ƿ���Ҫ������������·��
	//�����ӵ��Ƿ�ǽ��򱬣�ֻ����ҵ��ӵ��ܹ���ǽ��
	void test(PROPERTY *imfor,GUN_CHILD *,TANK *);
	//�ȽϺ���,������Ҫ������з�̹�˱Ƚϣ����ƶ���˺���
	void compare(PROPERTY *imfor,list<IMFOR_WALL>::iterator index);
	void compare(list<IMFOR_WALL>::iterator index);
	//���µ�����̹����ǽ��λ�õ���Ϣ��ע�⣬��Ҫ���÷����ӵ�
	void update(GUN_CHILD *gun,TANK_ENEMY *pro,TANK *host);
	void render();			//������Ҫ����ӵ������Դ����ӵ��Ľӿ�

	void reset_boss(const D3DXVECTOR3 &);
	void update_boss(const D3DXVECTOR3 &);

	friend void draw_tank_gun();
	friend class EXPLORE_BILLBOARD;
};

# endif