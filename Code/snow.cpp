# include "snow.h"
# include "head.h"

//ָ��ѩ��������ѩ����������С�������С��Ƭ�δ�С			P244
SNOW::SNOW(BOUNDINGBOX *box,int num_particles):_box(*box)
{
	_size = 50.0f;							//ѩ���Ĵ�С
	_vertexbuf_size = 2048;					//����ϵͳ��GPU�Ŀ��ô�С
	_vertexbuf_offset = 0;					//���㻺���ƫ����
	_vertexbuf_num = 512;					//ÿ����䶥�㻺�涥����
	for(int i = 0;i < num_particles;i++)	//���������������ѩ��
		add_particle();
}

//������û��
//������������						P245
//��ָ����������ڴ����˾������xz�����ѩ�����ӣ��������ӵ�y������Ϊ
//�����߶�ֵ������ѩ��һ��������Ʈ���ٶ�
void SNOW::reset_particle(ATTRIBUTE *attribute)
{
	attribute->_alive = true;
	GetRandomVector(&attribute->_position,			//����ѩ��Ʈ����ٶȴ�С�Լ�����
		&_box._min,
		&_box._max);

	attribute->_position.y = _box._max.y;			//����ѩ�����ٶ�
	attribute->_velocity.x = GetRandomFloat(0.0f,1.0f) * (-3.0f);
	attribute->_velocity.y = GetRandomFloat(0.0f,1.0f) * (-10.0f);
	attribute->_velocity.z = 0.0f;

	attribute->_color = WHITE;						//�趨ѩ������ɫ��ʵ����ѩ������ɫ���������ɫ��
}
void SNOW::update(float time)				//������ѩ�����и��£����˵�ѩ�����临��
{
	list<ATTRIBUTE>::iterator i;
	for(i = _particles.begin();i != _particles.end();i++)
	{
		i->_position += i->_velocity * time;			//�ٶȳ���ʱ�����λ��
		if(_box.IsPointInside(i->_position) == false)
			reset_particle(&(*i));
	}
}


/*------------------------------------------------------------
fire system
���ϵͳ
-------------------------------------------------------------*/

FIRE::FIRE(D3DXVECTOR3 *origin,int num_particles)
{
	_origin				= *origin;
	_size				= 0.9f;
	_vertexbuf_size     = 2048;
	_vertexbuf_offset   = 0;   
	_vertexbuf_num		= 512; 
	for(int i = 0; i < num_particles; i++)
		add_particle();	
}

/*
1. �����Ӹ��������������Դ��
2. ��ȡ���ⷽ�򣬴�С�ڹ涨��Χ�ڵ��ٶȣ�������淶�����Ա���Ƴ���������
3. �������ӵ���ɫΪ�����ɫ
4. �������ӵ���������Ϊ2s
*/
void FIRE::reset_particle(ATTRIBUTE *attribute)
{
	attribute->_alive  = true;
	attribute->_position = _origin;

	D3DXVECTOR3 min = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	D3DXVECTOR3 max = D3DXVECTOR3( 1.0f,  1.0f,  1.0f);
	//��ȡ���ⷽ����ٶ�
	GetRandomVector(
		&attribute->_velocity,
		&min,
		&max);

	//���ٶȹ淶�����ܻ��Ƴ�����
	D3DXVec3Normalize(
		&attribute->_velocity,
		&attribute->_velocity);

	attribute->_velocity *= 100.0f;
	//����������Ϊһ���������ɫ
	attribute->_color = D3DXCOLOR(
		GetRandomFloat(0.0f, 1.0f),
		GetRandomFloat(0.0f, 1.0f),
		GetRandomFloat(0.0f, 1.0f),
		1.0f);

	attribute->_age      = 0.0f;
	attribute->_life_time = 2.0f; // lives for 2 seconds
}
/* �������е�����
1. ���������������
2. ���������Ǵ��ģ�����ʱ���ƶ�����
3. ���������Ѿ������������ڣ�ɱ������
*/
void FIRE::update(float time)
{
	list<ATTRIBUTE>::iterator i;
	for(i = _particles.begin(); i != _particles.end(); i++)
	{
		// ֻ���´�������only update living particles
		if( i->_alive )
		{
			i->_position += i->_velocity * time;
			i->_age += time;
			if(i->_age > i->_life_time) // ɱ����������������kill 
				i->_alive = false;
		}
	}
}
void FIRE::prepare_render()
{
	//������prepare_render����
	POINTSYSTEM::prepare_render();
	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);		//Ŀ��������Դ�����������
    _device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// read, but don't write particles to z-buffer
	//����д������Ȼ��棬�����ӽӽ��������ʱ�򣬺ܶೡ�����ᱻĨɱ
	_device->SetRenderState(D3DRS_ZWRITEENABLE, false);
}
void FIRE::post_render()
{
	POINTSYSTEM::post_render();
	_device->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

/*------------------------------------------------------
������
--------------------------------------------------------*/
GUN::GUN(CAMERA* camera)
{
	_camera				= camera;
	_size				= 8.0f;
	_vertexbuf_size     = 2048;
	_vertexbuf_offset   = 0;  
	_vertexbuf_num		= 512; 
}
/* 
1.��������ǹ��λ��
2.��������ǹ���ٶ�
3.��������ǹ����ɫ������
*/
void GUN::reset_particle(ATTRIBUTE* attribute)
{
	attribute->_alive  = true;
	D3DXVECTOR3 cameraPos;
	cameraPos = _camera->Position();
	D3DXVECTOR3 cameraDir;
	cameraDir = _camera->Look();
	// change to camera position
	attribute->_position = cameraPos;
	attribute->_position.y -= 10.0f;	// ���ӵ�λ�ñ��������λ���Ե�	slightly below camera
										// �����������������Ǵ���һ��ǹ	so its like we're carrying a gun
	// travels in the direction the camera is looking
	attribute->_velocity = cameraDir * 100.0f;
	// green
	attribute->_color = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	attribute->_age      = 0.0f; 
	attribute->_life_time = 1.0f; // lives for 1 seconds
}

/*������������
1.�����������������������������ӵ�λ���Լ�����
2.�������Ѿ����������ڣ�����ɱ��
3.���Ѿ��������������ӽ��и���
*/
void GUN::update(float time)
{
	list<ATTRIBUTE>::iterator i;
	for(i = _particles.begin(); i != _particles.end(); i++)
	{
		i->_position += i->_velocity * time;
		i->_age += time;

		if(i->_age > i->_life_time) // kill 
			i->_alive = false;
	}
	remove_dead_particles();
}