#include "Fertilizer.h"

USING_NS_CC;

Fertilizer* Fertilizer::create() {
	Fertilizer* fertilizer = new(std::nothrow) Fertilizer();
	if (fertilizer && fertilizer->init()) {
		fertilizer->autorelease();
		return fertilizer;
	}
	CC_SAFE_DELETE(fertilizer);
	return nullptr;
}

// 
bool Fertilizer::init() {
	// 调用基类的初始化方法
	return Tool::init("fertilizer");
}

// 使用肥料
void Fertilizer::useItem() {

}