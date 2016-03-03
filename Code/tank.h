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

	LPDIRECT3DDEVICE9	_device;			//direct3D�豸�ӿ�
	LPD3DXMESH			_mesh;				//�������
	D3DMATERIAL9		*_materials;		//����Ĳ�����Ϣ
	LPDIRECT3DTEXTURE9  *_textures;			//�����������Ϣ
	DWORD				_NumMaterials;		//���ʵ���Ŀ
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
	//��¼̹�˵�λ�ã�̹��ֻ����ZOXƽ���˶����������ϵ�����ʼ�ղ���
	D3DXVECTOR3			_position;			//��¼���������̹�˵�λ��
	D3DXVECTOR3			_look;				//̹��ǰ���ķ���
	D3DXVECTOR3			_right;				//̹�˵��ҷ���

	D3DXMATRIX			_rotation;			//��¼̹����ת�ĽǶ�
	float				_rotation_sign;		//��¼̹���Ƿ��������ת
	int					_life;				//��̹������
	const int			_life_add;
	const int			_life_time;
	const int			_life_cut;
	float				_mul;				//�ٶȷŴ���
	const float			_turn_scale;		//̹�˹���ʱ��������ǰ���ı���
public:
	TANK(LPDIRECT3DDEVICE9);
	virtual void render();					//�����������

	//̹��ƽ��
	virtual void TranslationFront(CAMERA &camera);
	virtual void TranslationBack(CAMERA &camera);
	//̹����ת
	virtual void TurnRight(CAMERA &camera);
	virtual void TurnLeft(CAMERA &camera);
	//---------��Ԫ----------------//
	friend int select();
	friend class GUN_CHILD;
	friend class WALL;
	friend class EXPLORE_BILLBOARD;
	friend void draw_tank_gun();
};
//---------------------------------
//�ӵ�
//---------------------------------
class TANK_GUN:public MESHXFILE,public POINTSYSTEM
{
protected:
	list<D3DXMATRIX>	_velocity_view;					//�Ӿ��ϵ�������
	const int			_exit_time;
	const int			_time_cut;						
	const float			_mul;							//�ٶȵķŴ���
public:
	//���ߵĹ��캯����ͬʱ���û���ĳ�ʼ������
	TANK_GUN(LPDIRECT3DDEVICE9 device,wchar_t *name_gun,wchar_t *name_point);					
	//�����ӵ�													
	virtual void add_gun(D3DXVECTOR3 &position,D3DXVECTOR3 &look,D3DXMATRIX &velocity_view);

	virtual void reset_particle(ATTRIBUTE *attribute);	//��ʱ����Ϊ�պ���
	virtual void update(float time);					//����ϵͳǿ�Ƹ���
	virtual void render();
};	
//----------------------------------
//�����ӵ���
//----------------------------------
struct GUN_EXPLORE 
{
	int _time;
	D3DXVECTOR3 _position;
};
class GUN_CHILD:public TANK_GUN
{
protected:
	list<char>			_who;				//��¼�ӵ��Ĺ���
	list<GUN_EXPLORE>	_explore_num;		//��Ҫ��Ⱦ��ը������
	BILLBOARD			_billboard;			//�����
	LPDIRECT3DTEXTURE9	_texture_gun[25];
	const float			_explore_life;		//�ӵ���ը������ʱ�䣬
	const float			_explore_cut;		//�ӵ���ըʱ��������ٶ�
	const float			_hit_critical;		//�������ƽ��ֵ������С�ڴ������Ϊ����
public:
	GUN_CHILD(LPDIRECT3DDEVICE9 device,wchar_t *name_gun,wchar_t *name_point);
	~GUN_CHILD();
	//Ĭ�������˾����ӵ�����Ϊ�˾�û�б�־
	void add(D3DXVECTOR3 &position,D3DXVECTOR3 &look,D3DXMATRIX &velocity_view,char who = ALLIED);
	virtual void update(float time);
	void update(TANK *,PROPERTY*);
	virtual void render();
	void add_explore(D3DXVECTOR3 &position);
	bool is_explore(TANK*,list<ATTRIBUTE>::iterator);		//�����ӵ��ж��Ƿ�����˾�
	bool is_explore(PROPERTY *,TANK *,list<ATTRIBUTE>::iterator);	//�˾��ӵ��ж��Ƿ���е���

	void explore(const D3DXMATRIX &);						//��ָ���ĵص������ը
	//---------��Ԫ----------------///
	friend void draw_tank_gun();
	friend int select();
	friend class WALL;
};
//-------------------------------------------
//̹��
//-------------------------------------------

//�з�̹������

class TANK_ENEMY:public MESHXFILE
{
protected:
	const float			_angle_life_time;		//̹��ת����ʣ�µ�ʱ��		
	const float			_run_time_min;			//���ߵ���Сֵ
	const float			_run_time_max;			//���ߵ����ֵ
	const float			_time_cut;				//ʱ���˥���ٶ�
	const int			_sign;					//̹����������־
	int					_mul;					//̹���ٶȵķŴ���
	const float			_hit_critical;			//̹����ǽ�ڵľ���С�ڸ�ֵʱ��Ϊ̹�������巢����ײ
	const BOUNDINGBOX	_range;					//̹�������Χ
	PROPERTY			_imfor[8];				//�����з�̹�˵���Ϣ
public:
	TANK_ENEMY(LPDIRECT3DDEVICE9,BOUNDINGBOX &,int sign);
	TANK_ENEMY(LPDIRECT3DDEVICE9,const BOUNDINGBOX &box,wchar_t *);					//���캯��
	~TANK_ENEMY();									//��������

	void add(PROPERTY &);							//̹������������Ϸ��ʼʱ���̹�˵ĺ���
	void reset(PROPERTY &);							//���¼���̹����ת�ĺ���
	void update();									//����̹����Ϣ�ĺ���	
	void render();									//�������ез�̹�˵ĺ���
	//-------------��Ԫ-------------------------//
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

	//-----------------��Ԫ------------------------//
	friend class TANK;
};
# endif