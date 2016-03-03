# ifndef _HEAD_H
# define _HEAD_H

# include <windows.h>						//��ʼ��windowʱ���
# include <d3d9.h>							//��ʼ��direct3Dʱ���
# include <d3dx9.h>
# include <fstream>							//���������õ�
using namespace std;						//���������õ�ifstream

//D3DXCOLOR��ɫֵ
const D3DXCOLOR      WHITE( D3DCOLOR_XRGB(255, 255, 255) );
const D3DXCOLOR      BLACK( D3DCOLOR_XRGB(  0,   0,   0) );
const D3DXCOLOR        RED( D3DCOLOR_XRGB(255,   0,   0) );
const D3DXCOLOR      GREEN( D3DCOLOR_XRGB(  0, 255,   0) );
const D3DXCOLOR       BLUE( D3DCOLOR_XRGB(  0,   0, 255) );
const D3DXCOLOR     YELLOW( D3DCOLOR_XRGB(255, 255,   0) );
const D3DXCOLOR       CYAN( D3DCOLOR_XRGB(  0, 255, 255) );
const D3DXCOLOR    MAGENTA( D3DCOLOR_XRGB(255,   0, 255) );

//�����������ʽ
struct CUSTOMVERTEX
{
	D3DXVECTOR3 _position;
	D3DXVECTOR3 _n;
	FLOAT _u1,_v1;				//ʹ���������������Ҫ�ı�궨��

	CUSTOMVERTEX(D3DXVECTOR3 position,D3DXVECTOR3 n,
					FLOAT u1,FLOAT v1):
						_position(position),
						_n(n),
						_u1(u1),_v1(v1)
	{}
};
# define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)
//����,��պе������ʽ
//�����Ҳ������
struct TERRAINVERTEX
{
	FLOAT _x,_y,_z;
	FLOAT _u,_v;
	TERRAINVERTEX(FLOAT x,FLOAT y,FLOAT z,FLOAT u,FLOAT v):
					_x(x),_y(y),_z(z),_u(u),_v(v)
					{}
};

# define D3DFVF_TERRAINVERTEX (D3DFVF_XYZ |  D3DFVF_TEX1)


//ö�ٱ���
enum NUM{A = 0,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z};

struct BOUNDINGBOX
{
public:
	D3DXVECTOR3 _min;
	D3DXVECTOR3 _max;

public:
	BOUNDINGBOX(D3DXVECTOR3&);
	bool IsPointInside(D3DXVECTOR3 &);				//��������ĵ�ַ
};

//-------------------------------------
//���ӵ�����
//-------------------------------------
struct ATTRIBUTE
{
	D3DXVECTOR3 _position;
	D3DXVECTOR3 _velocity;		//���ӵ��ٶ�
	D3DXVECTOR3	_acceleration;	//���ӵļ��ٶ�
	float		_life_time;		//���Ӵ�������
	float		_age;			//���ӵĵ�ǰ����
	D3DXCOLOR	_color;
	D3DXCOLOR	_color_fade;	//������ɫ�����ʱ�����
	bool		_alive;			//��¼�����Ƿ���
};

//����ϵͳ����ṹ
struct PARTICLE
{
	D3DXVECTOR3 _position;
	D3DCOLOR _color;
	static const DWORD FVF;
};
struct PROPERTY
{
	D3DXVECTOR3			_position;			//��¼̹�˵�λ��
	D3DXVECTOR3			_look;				//̹��ǰ���ķ���
	D3DXVECTOR3			_right;				//̹�˵��ҷ���
	D3DXVECTOR3			_velocity;			//�ٶ�
	D3DXMATRIX			_rotation;			//��¼̹����ת�ĽǶ�
	float				_angle;				//��¼̹����ת�ĽǶ�
	float				_life;				//̹�˼������������µ�ʱ��
											//����ֵΪ-100ʱ��ʾ̹�˱�����
	float				_angle_life;		//��ת��ʱ��
};

//��float�洢������ԭ�ⲻ��ת��ΪDWORD
DWORD FtoDw(float);
//����һ���ڹ涨��Χ�������
float GetRandomFloat(float lowbound,float highbound);
//����һ���ڹ涨��Χ���������
void GetRandomVector(D3DXVECTOR3 *out,D3DXVECTOR3 *min,D3DXVECTOR3 *max);


# endif