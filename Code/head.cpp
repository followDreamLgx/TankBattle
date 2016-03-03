# include "head.h"
const DWORD PARTICLE::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
BOUNDINGBOX::BOUNDINGBOX(D3DXVECTOR3 &MAX)
{
	_max.x = MAX.x;
	_max.y = MAX.y;
	_max.z = MAX.z;

	_min.x = -MAX.x;
	_min.y = -MAX.y;
	_min.z = -MAX.z;
}
bool BOUNDINGBOX::IsPointInside(D3DXVECTOR3 &position)				//��������ĵ�ַ
{
	if( position.x >= _min.x && position.y >= _min.y && position.z >= _min.z &&
		position.x <= _max.x && position.y <= _max.y && position.z <= _max.z )
		return true;
	else
		return false;
}

//��float�洢������ԭ�ⲻ��ת��ΪDWORD
DWORD FtoDw(float f)
{
	return *((DWORD*)&f);
}
//����һ���ڹ涨��Χ�������
//ʹ��֮ǰ��Ҫ��srand()������������
float GetRandomFloat(float lowbound,float highbound)
{
	if(lowbound >= highbound)		//С�����ȴ�����������ꡣ����
		return lowbound;

	float f = (rand() % 10000) * 0.0001f;
return (f * (highbound - lowbound)) + lowbound;
}
//����һ���ڹ涨��Χ���������
void GetRandomVector(D3DXVECTOR3 *out,D3DXVECTOR3 *min,D3DXVECTOR3 *max)
{
	out->x = GetRandomFloat(min->x,max->x);
	out->y = GetRandomFloat(min->y,max->y);
	out->z = GetRandomFloat(min->z,max->z);
}
