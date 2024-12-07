#ifndef __Item_H__
#define __Item_H__

#include "cocos2d.h"
#include <string>

// 物品类，是一个抽象类
class Item : public cocos2d::Sprite {
public:
	// 初始化（子类实现）
	virtual bool init(const std::string& name) = 0;

	// 使用物品（纯虚函数，子类必须实现）
	virtual void useItem() = 0;

	// 获取工具名称，内联函数，直接实现
	std::string getItemName() const {
		return itemName;
	}

	// 虚析构函数
	virtual ~Item() {}
protected:
	// 工具名称
	std::string itemName;
};

#endif
