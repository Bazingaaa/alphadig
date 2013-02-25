
#ifndef SCENE_MANAGER_H__
#define SCENE_MANAGER_H__




namespace AlphaDig
{

class SceneManager
{
public:

	///<开始主菜单场景
	static void startMainMenuScene();

	///<覆盖一个新的游戏场景
	static void replaceGamingScene();

	///<覆盖一个新的主菜单场景
	static void replaceMainMenuScene();

	///<replace a game over scene
	static void replaceGameOverScene();

private:
	SceneManager();
	~SceneManager();
};


}


#endif

