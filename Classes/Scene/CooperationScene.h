/****************************************************************
 * File Function: 游戏合作场景类，用于搜寻合作玩家场景的展示
 * Author:        韦瑾钰
 ****************************************************************/
#ifndef __COOPERATION_SCENE_H__
#define __COOPERATION_SCENE_H__

#include "cocos2d.h"

class Cooperation : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    // 初始化
    bool init();

    // 实现静态 Cooperation* create()函数
    CREATE_FUNC(Cooperation);

private:
    // 返回项的回调函数
    void backCallBack(cocos2d::Ref* pSender);

    // 点击向上按钮的动画
    void onUpButtonClicked(cocos2d::Ref* pSender);

    // 点击向下按钮的动画
    void onDownButtonClicked(cocos2d::Ref* pSender);

    // 存储 coopText 的指针
    cocos2d::Label* coopText;
};

#endif // __COOPERATION_SCENE_H__