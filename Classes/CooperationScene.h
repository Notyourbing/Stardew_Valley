#ifndef __COOPERATION_SCENE_H__
#define __COOPERATION_SCENE_H__

#include "cocos2d.h"

class Cooperation : public cocos2d::Scene {
public:
	static cocos2d::Scene* createScene();
	
	virtual bool init();

	// ʵ�־�̬ Cooperation* create()����
	CREATE_FUNC(Cooperation);

	// ������Ļص�����
	void backCallBack(cocos2d::Ref* pSender);
};


#endif