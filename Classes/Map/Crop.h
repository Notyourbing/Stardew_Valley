#ifndef __CROP_H__
#define __CROP_H__

#include "cocos2d.h"
#include "../DateManage/DateManage.h"
#include "../Constant/Constant.h"

// 作物的种类
enum class CropType {
	APPLE,
	CORN,
	CARROT,
};

// 季节
enum class Season {
	SPRING,
	SUMMER,
	AUTUMN,
	WINTER,
};

// 作物的基类
class Crop {
private:
	int maxGrowthDay;		// 最长的成熟天数

protected:
	int currentGID;			// 作物当前的GID值
	CropType cropType;		// 作物的种类
	int growedDay;			// 已经成长的天数
	int growthStage;		// 当前的成长阶段
	bool isInfested;		// 是否遭受虫害
	int infestedDay;		// 遭受虫害天数
	bool isDrought;			// 是否遭受干旱
	int droughtDay;			// 干旱天数
	bool isFertilized;		// 当前的作物是否施肥
	int cropGID;			// 当前图层对应图块的GID
	cocos2d::Vec2 position;	// 当前作物在瓦片地图上的位置
	int maxAliveDay;		// 作物成熟后最长成熟天数
public:

	// 构造函数
	Crop(const CropType& cropType, const int& maxGrowthDay,const int& cropGID, cocos2d::Vec2 position);

	// 处理作物的生长,虚函数
	virtual void grow() {};

	// 获取当前作物需要的成熟天数
	int getMaturedDay() const;

	// 判断是否成熟
	bool isMature() const;

	// 浇水
	void irrigate();

	// 施肥
	void fertilize();

	// 使用农药
	void applyPesticide();

	// 作物的丰收
	void harvest();

	// 获取当前的GID
	int getCurrentGID();

	// 作物每天的更新
	void cropUpdateByDay();

	// 更新当前图块的GID
	virtual void updateGID() {};
};

//具体子类的继承

// Apple类：表示苹果
class Apple : public Crop {
private:
	std::vector<Season> season;				// 适合种植的季节
public:
	
	// 构造函数
	Apple(const cocos2d::Vec2& position);

	// 苹果的生长函数
	void grow() override;

	// 更新图块的重写
	void updateGID() override;
};

// Corn类：表示玉米
class Corn : public Crop {
private:
	std::vector<Season> season;				// 适合种植的季节
public:
	
	// 构造函数
	Corn(const cocos2d::Vec2& position);

	// 更新图块的重合
	void updateGID() override{}
};

// Carrot类：表述胡萝卜
class Carrot : public Crop {
private:
	std::vector<Season> season;				// 适合种植的季节
public:

	// 构造函数
	Carrot(const cocos2d::Vec2& position);

	// 更新图块的重写
	void updateGID() override {};
};

#endif