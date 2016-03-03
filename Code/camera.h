#ifndef _CAMERA_H
# define _CAMERA_H

# include "head.h"


class CAMERA
{
protected:
	D3DXVECTOR3 position;					//摄像机的位置
	D3DXVECTOR3 look;						//观察分量
	D3DXVECTOR3 right;						//右分量
	D3DXVECTOR3 up;							//上分量
	D3DXVECTOR3 target;						//目标观察点
	D3DXMATRIX  proj;						//投影变换		projector（投影仪）
	D3DXMATRIX  view;						//取景变换
	LPDIRECT3DDEVICE9 device;				//COM接口

public:
	//构造函数
	CAMERA(LPDIRECT3DDEVICE9 _device);
	//沿着三个分量平移
	VOID TranslationLook(FLOAT units);
	VOID TranslationUp(FLOAT units);
	VOID TranslationRight(FLOAT units);
	//给定一个平移向量，进行平移
	VOID Translation(D3DXVECTOR3 &trans);
	//沿着三个分量旋转
	VOID RotationLook(FLOAT angles);
	VOID RotationUp(FLOAT angles);
	VOID RotationRight(FLOAT angles);
	//绕着物体旋转
	VOID RotationTargetLook(FLOAT angles);
	VOID RotationTargetRight(FLOAT angles);
	VOID RotationTargetUp(FLOAT angles);
	//给定旋转矩阵，将摄像机旋转
	VOID Rotation(D3DXMATRIX &rotation);
	//设置取景变换
	D3DXMATRIX &GetView();
	VOID View();
	VOID View(D3DXMATRIX &viewport);		//为实现
	//设置投影变换
	//析构函数
	//获取摄像机位置以及分量
	D3DXVECTOR3 Position();
	D3DXVECTOR3 Look();

	//友元类
	friend class TANK;			//是否需要天机头文件呢？
};

#endif