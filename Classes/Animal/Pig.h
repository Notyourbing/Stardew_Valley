/****************************************************************
 * File Function: 农场动物猪类，继承自Animal类，实现农场上的猪的控制
 * Author:        高鑫
 ****************************************************************/
#ifndef __PIG_H__
#define __PIG_H__

#include "Animal.h"
#include "cocos2d.h"

// Pig类：继承自Animal
class Pig :public Animal {
public:
	// 静态成员函数，用于创建并返回一个新的Pig类型对象的实例
	static Pig* create(const cocos2d::Vec2& position);

	// 初始化对象
	bool init() override;

	// 获得当前松露数量
	int getTruffleCount() const;
private:
	// 构造函数
	Pig(const cocos2d::Vec2& position);
	
	// 析构函数
	~Pig();

	// 松露数量
	int truffleCount;

	// 设置移动行为
	void setMovementBehavior(MovementBehavior* behavior) override;

	// 移动行为
	MovementBehavior* movementBehavior;
};

#endif 