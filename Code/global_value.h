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

//ȫ�ֱ�����������չ
extern WNDCLASSEX wndclass;						//ȫ�ֱ������˱����ڳ�ʼ�������Լ��������ڵ�ʱ��Ҫ�õ���
extern wchar_t *g_pClassName;					//��ʼ���������Լ��������ڵ�ʱ���õ�������һ��
extern wchar_t *g_pWindowName;					//��������
extern LPDIRECT3DDEVICE9	g_pd3dDevice;		//Direct3DDevice9�豸�ӿ�
extern CAMERA *camera;							//�����
extern TERRAIN *terrain;						//����
extern SKYBOX *sky;
extern ALPHA *pAlphaMesh;			
extern SNOW *snow;								//ѩ���׷�
extern KEYBOARDCLASS *keyboard;					//����
extern MOUSECLASS	*mouse;						//���
extern TANK	*tank_host;							//��̹��
extern TANK_ENEMY *tank_enemy;					//�з�̹��
extern TANK_GUN	 *tank_gun;						//̹�˷�����ӵ�
extern GUN_CHILD *gun_child;					//ʶ���ܵ��ӵ�
extern wchar_t			wall_name[5][50];		//ǽ�ڵ�x�ļ�
extern WALL				*wall;					//ǽ��
extern BILLBOARD		*billboard;				//�������
extern EXPLORE_BILLBOARD	*ex_board;			//�����Ĺ����
extern bool tree_or_explore;
extern LPD3DXFONT TextStates;
extern RECT rect;								//���λ�õ�ȷ��
extern wchar_t a[3][50];
extern D3DGUIClass		*g_MainGUI;
extern int				g_MainGUIFontID;				//  GUI����������ID
extern int				g_StartGUIFontID;				//  GUI����������ID
extern int				g_LoadGUIFontID;				//  GUI����������ID
extern int				g_OptionGUIFontID;				//  GUI����������ID
extern bool				g_LMBDown;					 // GUI�е����״̬��Ϣ���������Ƿ��µı�ʶ
extern int				g_MouseX,g_MouseY;					//�洢����������������
extern int				g_currentGUI;		//һ����ǰ��GUI��ʶ
extern D3DGUIClass		*g_StartGUI; //��Ϸ��ʼ����
extern D3DGUIClass		*g_LoadGUI; //��Ϸ���봰��
extern D3DGUIClass		*g_OptionGUI; //��Ϸ���ô���
# endif