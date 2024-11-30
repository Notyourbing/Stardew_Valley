#ifndef __BAG_H__
#define __BAG_H__
#include "cocos2d.h"
#include "../Tool/Tool.h"

class Bag : public cocos2d::Node {
public:
	// 创建背包实例
	static Bag* create();

	// 初始化背包
	bool init();

	// 添加工具到背包
	bool addTool(Tool* tool);

	// 移除工具
	void removeTool(int index);

	// 获取某个工具
	Tool* getTool(int index) const;

	// 设置选中的工具
	void selectTool(int index);

	// 获取当前选中的工具
	Tool* getSelectedTool() const;

private:
	// 存储工具的容器
	std::vector<Tool*> tools;

	// 背包容量
	static const int capacity = 9;

	// 当前选中的工具索引
	int selectedIndex;

	// 用于显示背包的矩形框和工具
	cocos2d::DrawNode* bagBackground; // 背景框
	std::vector<cocos2d::Sprite*> toolIcons; // 工具图标

	void updateDisplay();

	static const int iconSize = 54; // 每个图标的宽度/高度
	static const int spacing = 10; // 图标间距
};
#endif __BAG_H__