/****************************************************************
 * File Function: 实现农场地图类
 * Author:        赵卓冰 高鑫
 ****************************************************************/
#include "FarmMap.h"
#include "../Animal/Cow.h"
#include "../Animal/Chicken.h"
#include "../Animal/Sheep.h"
#include "../Animal/Pig.h"
#include "../Skill/SkillTree.h"
#include "../Item/StoneItem.h"
#include "../Player/Player.h"
#include "../Market/MarketState.h"
#include "../Crop/FarmResourceManager.h"
#include<string>

USING_NS_CC;

// 初始化静态实例
FarmMap* FarmMap::instance = nullptr;

// 构造函数
FarmMap::FarmMap() : grassLayer(nullptr), soilLayer(nullptr), obstacleLayer(nullptr),
moldLayer(nullptr), waterLayer(nullptr), stoneLayer(nullptr),
animalManager(nullptr) {}

// 析构函数
FarmMap::~FarmMap() {
    instance = nullptr;
    animalManager=nullptr;
}

// 获取单例实例
FarmMap* FarmMap::getInstance() {
    if (!instance) {
        instance = new (std::nothrow) FarmMap();
        if (instance && instance->init("Maps/farmSpring/farmMap.tmx")) {
            instance->autorelease();
        }
        else {
            CC_SAFE_DELETE(instance);
        }
    }
    return instance;
}

bool FarmMap::init(const std::string& tmxFile) {
    if (!SceneMap::init(tmxFile)) {
        return false;
    }

    // 获取地图的各个图层
    grassLayer = tiledMap->getLayer("Grass");
    soilLayer = tiledMap->getLayer("Soil");
    obstacleLayer = tiledMap->getLayer("Obstacles");
    moldLayer = tiledMap->getLayer("Mold");
    waterLayer = tiledMap->getLayer("Water");
    stoneLayer = tiledMap->getLayer("Stone");

    // 添加动物管理
    animalManager = AnimalManager::create();
    addChild(animalManager);

    // 获取瓦点地图的长宽
    int width = tiledMap->getMapSize().width;
    int height = tiledMap->getMapSize().height;
        
    // 遍历图层为每一个图层
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (stoneLayer->getTileGIDAt(Vec2(x, y))) {
                mapTileNode[x][y] = Stone::create(Vec2(x,y));
            }
            else if (waterLayer->getTileGIDAt(Vec2(x, y))) {
                mapTileNode[x][y] = Water::create(Vec2(x, y));
            }
            else if (moldLayer->getTileGIDAt(Vec2(x, y))) {
                mapTileNode[x][y] = Mold::create(Vec2(x, y));
            }
            else if(obstacleLayer->getTileGIDAt(Vec2(x,y))){
                mapTileNode[x][y] = Obstacle::create(Vec2(x, y));  
            }
            else if (soilLayer->getTileGIDAt(Vec2(x, y))) {
                mapTileNode[x][y] = Soil::create(Vec2(x, y));
            }
            else {
                mapTileNode[x][y] = Grass::create(Vec2(x, y));
            }
        }
    }

    // 这个lambda函数会在FarmMap的生存期内每帧时间调用一次
    schedule([this](float dt) {
        // 检查目标位置是否是障碍物
        auto player = Player::getInstance();

        // 人在地图坐标中下一步会到达的位置
        Vec2 playerSize2 = Vec2(0.0f, player->getContentSize().height * 1.0f);

        auto playerPositionInMap = player->getPosition() - getPosition() - playerSize2 * 0.5f + player->getVelocity() / MAP_MOVE_SPEED * 10.0f;
        if (isCollidable(playerPositionInMap)) {
             velocity = Vec2::ZERO;
             player->stopMoving();
        }
        auto position = getPosition() + velocity * dt;
        // max保证大于等于下界， min保证小于等于上界
        position.x = std::max(WINSIZE.width - getMapSize().width, std::min(position.x, 0.0f));
        position.y = std::max(WINSIZE.height - getMapSize().height, std::min(position.y, 0.0f));

        setPosition(position);
        }, "farm_map");
    return true;
}

// 随地图创建 NPC
bool FarmMap::npcInit(const Vec2& position, Npc* npc) {
    npc->setPosition(position);
    addChild(npc, 5);
     return true;
}

// 更新地图位置
void FarmMap::moveMapByDirection(const Vec2& direction) {
    velocity = direction * MAP_MOVE_SPEED;
}

// 碰撞检测：检查给定位置是否是障碍物, positon是人物在地图坐标系（原点在左下角）中的坐标
bool FarmMap::isCollidable(const Vec2& position) const {
    // 获取地图的瓦片大小
    const Size tileSize = tiledMap->getTileSize();
    const Size mapSize = tiledMap->getMapSize();

    // 将人物在地图中的坐标转换为瓦片地图坐标（原点在左上角）
    int x = position.x / tileSize.width;
    int y = (mapSize.height * tileSize.height - position.y) / tileSize.height;

    // 检查是否在地图范围内
    if (x < 0 || x >= mapSize.width || y < 0 || y >= mapSize.height) {
        return true; // 超出地图范围视为障碍物
    }

    // 碰撞检测
    TileType tileType = mapTileNode[x][y]->getTileType();
    if (tileType == TileType::Stone || tileType == TileType::Obstacle || tileType == TileType::Water||tileType == TileType::Mold) {
 
        return true;
    }
    else {
        return false;
    }
}

// 玩家与农场的接口
void FarmMap::interactWithMap() {
    // 获取玩家、地图、背包实例
    Player* player = Player::getInstance();
    FarmMap* farmMap = FarmMap::getInstance();
    Bag* bag = Bag::getInstance();

    if (!player->getUseItemEnable()) {
        return;
    }

    // 如果是特定的工具的话，我们就是和动物交互

    // 获取要交互的土块位置
    Vec2 playerPosition = player->getPosition();
    const Size tileSize = farmMap->tiledMap->getTileSize();
    const Size mapSize = farmMap->tiledMap->getMapSize();
    playerPosition = playerPosition - farmMap->getPosition();
    playerPosition.y = playerPosition.y - player->getContentSize().height / 2 + 10.0f;
    playerPosition.x = playerPosition.x / tileSize.width;
    playerPosition.y = (mapSize.height * tileSize.height - playerPosition.y) / tileSize.height;
    const Vec2 lastDirection = player->getLastDirection();

    // 当前人物前面的瓦点
    if (lastDirection == Vec2(1, 0) && playerPosition.x + 1 < mapSize.width - 1) {
        playerPosition.x++;
    }
    else if (lastDirection == Vec2(0, 1) && playerPosition.y - 1 >= 0) {
        playerPosition.y--;
    }
    else if (lastDirection == Vec2(-1, 0) && playerPosition.x - 1 >= 0) {
        playerPosition.x--;
    }
    else if (lastDirection == Vec2(0, -1) && playerPosition.y + 1 < mapSize.height - 1) {
        playerPosition.y++;
    }

    const int x = playerPosition.x;
    const int y = playerPosition.y;

    // 获得当前人物所使用的工具
    std::string currentItemName = player->getCurrentItemName();

    if (mapTileNode[x][y]->getTileType() == TileType::Soil) {
        interactWithSoil(currentItemName, x, y);
    }
    else if (mapTileNode[x][y]->getTileType() == TileType::Grass) {
        interactWithGrass(currentItemName, x, y);
    }
    else if (mapTileNode[x][y]->getTileType() == TileType::Mold) {
        interactWithMold(currentItemName, x, y);
    }
    else if (mapTileNode[x][y]->getTileType() == TileType::Stone) {
        interactWithStone(currentItemName, x, y);
    }
    else {
        return;
    }
}

// 与土壤的交互
void FarmMap::interactWithSoil(std::string itemName, const int& x, const int& y) {
    // 根据工具去分类
    if (itemName == "hoe") {
        dynamic_cast<Soil*>(mapTileNode[x][y])->soilHoe();
    }
    else if (itemName == "fertilizer") {
        dynamic_cast<Soil*>(mapTileNode[x][y])->soilFertilize();
        FarmResourceManager* farmResourceManager = FarmResourceManager::create();
        farmResourceManager->useFertilize();
    }
    else if (itemName == "kettle") {
        dynamic_cast<Soil*>(mapTileNode[x][y])->soilWater();
        FarmResourceManager* farmResourceManager = FarmResourceManager::create();
        farmResourceManager->useWater();
    }
    else if (itemName == "dogbaneSeed" || itemName == "cornSeed" || itemName == "carrotSeed") {
        SkillTree::getInstance()->updatePlantingCount(1);
        dynamic_cast<Soil*>(mapTileNode[x][y])->plantCrop(itemName);
        FarmResourceManager* farmResourceManager = FarmResourceManager::create();
        farmResourceManager->useSeed();
    }
    else if (itemName == "scythe") {
        dynamic_cast<Soil*>(mapTileNode[x][y])->harvest();
    }
    else if (itemName == "agrochemical") {
        dynamic_cast<Soil*>(mapTileNode[x][y])->applyAgrochemical();
    }
    else {
        return;
    }

    // 操作之后更新soil土壤块
    dynamic_cast<Soil*>(mapTileNode[x][y])->updateGID();
    int soilGID = dynamic_cast<Soil*>(mapTileNode[x][y])->getCurrentGID();
    soilLayer->setTileGID(soilGID,Vec2(x,y));
}

// 与草地的交互
void FarmMap::interactWithGrass(std::string itemName, const int& x, const int& y) {
    
    // 将瓦点坐标转化为地图坐标
    Vec2 animalPosition;
    const Size tileSize = tiledMap->getTileSize();                      
    const Size mapSize = tiledMap->getMapSize();
    animalPosition.x = x * tileSize.width;
    animalPosition.y = mapSize.height * tileSize.height - y * tileSize.height;

    // 添加动物
    if (itemName == "cow") {
        Cow* cow = Cow::create(animalPosition);
        animalManager->addAnimal(cow, animalPosition);
    }
    else if (itemName == "chicken") {
        Chicken* chicken = Chicken::create(animalPosition);
        animalManager->addAnimal(chicken, animalPosition);
    }
    else if (itemName == "sheep") {
        Sheep* sheep = Sheep::create(animalPosition);
        animalManager->addAnimal(sheep,animalPosition);
    }
    else if(itemName=="pig") {
        Pig* pig = Pig::create(animalPosition);
        animalManager->addAnimal(pig,animalPosition);
    }
    else {
        return;
    }
}

// 与箱子的交互
void FarmMap::interactWithMold(std::string itemName, const int& x, const int& y) {
    // 开启市场系统进行交易
    Player* player = Player::getInstance();
    MarketState* marketState = MarketState::create();

    // 市场交易
    int getMoney = marketState->sellingGoods(itemName);
    player->addMoney(getMoney);
}

// 与石头的交互
void FarmMap::interactWithStone(std::string itemName, const int& x, const int& y) {
    // 镐子敲击石头
    if (itemName == "pickaxe") {
        // 敲击石头
        dynamic_cast<Stone*>(mapTileNode[x][y])->knockRock();

        // 判断石头是否被击碎
        if (dynamic_cast<Stone*>(mapTileNode[x][y])->isBroken()) {
            delete mapTileNode[x][y];
            mapTileNode[x][y] = nullptr;
            mapTileNode[x][y] = Soil::create(Vec2(x, y));

            // 更新地图图块
            stoneLayer->setTileGID(0, Vec2(x, y));
            soilLayer->setTileGID(SOIL_GID, Vec2(x, y));


            //采矿次数增加
            auto bag = Bag::getInstance();
            srand(static_cast<unsigned int>(time(NULL)));
            auto skills = SkillTree::getInstance()->getAllSkills();
            int level = skills["Mining"]->getCurrentLevel();
            int random = level * rand() % 100;  // 随机索引
            StoneItem* stoneItem = StoneItem::create(STONE_ITEM);
            // 概率获取第二块石头
            if (random > 50) {
                SkillTree::getInstance()->updateMiningCount(1);
                bag->addItem(stoneItem);
            }
            SkillTree::getInstance()->updateMiningCount(1);
            bag->addItem(stoneItem);
        }
    }
}

// 地图时间更新
void FarmMap::mapUpdateByTime() {
    // 动物生长
    // 植物生长
    // 获取瓦点地图的长宽
    int width = tiledMap->getMapSize().width;
    int height = tiledMap->getMapSize().height;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (mapTileNode[x][y]->getTileType() == TileType::Soil) {
                dynamic_cast<Soil*>(mapTileNode[x][y])->updateByTime();
                int soilGID = dynamic_cast<Soil*>(mapTileNode[x][y])->getCurrentGID();
                soilLayer->setTileGID(soilGID, Vec2(x, y));
            }
        }
    }
}