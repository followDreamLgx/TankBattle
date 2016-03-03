# ifndef _GLOBAL_VALUE_H
# define _GLOBAL_VALUE_H

# include "head.h"
# include "skybox.h"
# include "camera.h"
# include "terrain.h"
# include "alpha.h"
# include "mirror_shadow.h"
# include "billboard.h"
# include "snow.h"
# include "input.h"
# include "tank.h"
# include "wall.h"
# include "D3DGUIClass.h"

//全局变量作用域拓展
extern WNDCLASSEX wndclass;						//全局变量，此变量在初始化窗口以及撤销窗口的时候都要用到，
extern wchar_t *g_pClassName;					//初始化窗口类以及创建窗口的时候用到，必须一致
extern wchar_t *g_pWindowName;					//窗口名称
extern LPDIRECT3DDEVICE9	g_pd3dDevice;		//Direct3DDevice9设备接口
extern CAMERA *camera;							//摄像机
extern TERRAIN *terrain;						//地形
extern SKYBOX *sky;
extern ALPHA *pAlphaMesh;			
extern SNOW *snow;								//雪花纷飞
extern KEYBOARDCLASS *keyboard;					//键盘
extern MOUSECLASS	*mouse;						//鼠标
extern TANK	*tank_host;							//主坦克
extern TANK_ENEMY *tank_enemy;					//敌方坦克
extern TANK_GUN	 *tank_gun;						//坦克发射的子弹
extern GUN_CHILD *gun_child;					//识别功能的子弹
extern wchar_t			wall_name[5][50];		//墙壁的x文件
extern WALL				*wall;					//墙壁
extern BILLBOARD		*billboard;				//广告牌树
extern EXPLORE_BILLBOARD	*ex_board;			//进击的广告牌
extern bool tree_or_explore;
extern LPD3DXFONT TextStates;
extern RECT rect;								//输出位置的确定
extern wchar_t a[3][50];
extern D3DGUIClass		*g_MainGUI;
extern int				g_MainGUIFontID;				//  GUI中字体对象的ID
extern int				g_StartGUIFontID;				//  GUI中字体对象的ID
extern int				g_LoadGUIFontID;				//  GUI中字体对象的ID
extern int				g_OptionGUIFontID;				//  GUI中字体对象的ID
extern bool				g_LMBDown;					 // GUI中的鼠标状态信息，鼠标左键是否按下的标识
extern int				g_MouseX,g_MouseY;					//存储鼠标坐标的两个变量
extern int				g_currentGUI;		//一个当前的GUI标识
extern D3DGUIClass		*g_StartGUI; //游戏开始窗口
extern D3DGUIClass		*g_LoadGUI; //游戏载入窗口
extern D3DGUIClass		*g_OptionGUI; //游戏设置窗口
# endif