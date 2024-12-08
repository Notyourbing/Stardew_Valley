#ifndef __SAVE_MANAGE_H__
#define __SAVE_MANAGE_H__
#include <string>
#include <vector>
#include "cocos2d.h"

struct PlayerSaveData {
	float posX;
	float posY;
	float direction;
};

class SaveManage {
public:
	static SaveManage* getInstance();

	// 保存玩家数据到文件
	bool savePlayerData(const PlayerSaveData& data);

	// 从玩家中加载玩家数据
	bool loadPlayerData(PlayerSaveData& data);

private:
	SaveManage() = default;
	~SaveManage() = default;
	
	std::string serializeToJson(const PlayerSaveData& data);

	bool deserilizeFromJson(const std::string& jsonStr, PlayerSaveData& data);

	static const std::string SAVE_FILE_NAME; // "player_save.json"
};

#endif

