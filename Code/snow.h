# ifndef _SNOW_H
# define _SNOW_H

# include "pointsystem.h"
# include "camera.h"

class SNOW:public POINTSYSTEM
{
protected:
	BOUNDINGBOX				_box;				//���ӵĻ��Χ
public:
	//ָ��ѩ��������ѩ������
	SNOW(BOUNDINGBOX *box,int num_particles);
	//������������
	void reset_particle(ATTRIBUTE *attribute);
	void update(float time);
};


class FIRE:public POINTSYSTEM
{
public:
	FIRE(D3DXVECTOR3 *origin,int num_particles);
	void reset_particle(ATTRIBUTE *attribute);
	void update(float time);
	void prepare_render();
	void post_render();
};

class GUN: public POINTSYSTEM
{
public:
	GUN(CAMERA* camera);
	void reset_particle(ATTRIBUTE* attribute);
	void update(float timeDelta);
private:
	CAMERA* _camera;
};
# endif