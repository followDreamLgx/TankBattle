# ifndef _POINTSYSTEM_H
# define _POINTSYSTEM_H
# include "head.h"
# include <list>


//------------------------------------------------------------
//����ϵͳ�ĸ���
//------------------------------------------------------------
class POINTSYSTEM
{
protected:	
	LPDIRECT3DDEVICE9		_device;			//
	D3DXVECTOR3				_origin;			//����Դ 

	float					_add_rate;			//���Ӳ������ٶ�
	float					_size;				//���ӵĴ�С
	LPDIRECT3DTEXTURE9		_texture;			//���ӵ�����
	LPDIRECT3DVERTEXBUFFER9	_vertex;			//
	list<ATTRIBUTE>			_particles;			//ע����Ҫͷ�ļ�list�����ӵ�����
	int						_max_particles;		//ͬʱ���ڵ�����������Ŀ
	//���㻺�������
	DWORD					_vertexbuf_size;	//���㻺��Ĵ�С
	DWORD					_vertexbuf_offset;	//���㻺�����ʼƫ����
	DWORD					_vertexbuf_num;		//ÿ����䵽���㻺��Ķ�������

public:
	POINTSYSTEM();
	virtual ~POINTSYSTEM();

	virtual bool init(LPDIRECT3DDEVICE9 device,wchar_t *text_name);			//û������
	virtual void reset();			//�����趨ϵͳ��ÿ�����ӵ�����P236
	virtual void reset_particle(ATTRIBUTE *attribute) = 0;		//
	virtual void add_particle();
	//
	virtual void update(float time) = 0;
	//
	virtual void prepare_render();
	virtual void render();
	virtual void post_render();

	bool is_empty();
	bool is_dead();

protected:
	virtual void remove_dead_particles();
};

# endif