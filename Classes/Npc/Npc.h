/****************************************************************
 * File Function: Npc类，完成npc创建，随地图一起加载
 * Author:        韦瑾钰
 ****************************************************************/
#ifndef NPC_H
#define NPC_H

#include "cocos2d.h"
#include <string>
#include <vector>
#include "../Constant/Constant.h"

enum class RelationshipStatus {
    None,       // 陌生
    Friendship, // 友谊
    Romance     // 浪漫
};

class Npc : public cocos2d::Sprite {
private:
    std::string name;                   // NPC的名字
    std::string birthday;               // NPC的生日
    int friendshipLevel;                // NPC与玩家的亲密度（0-100）
    std::vector<std::string> gifts;     // NPC喜欢的礼物
    std::vector<std::string> dislikes;  // NPC讨厌的礼物
    std::vector<std::string> dialogues; // NPC的对话列表
    std::string image;                  // 图像的路径
    RelationshipStatus playerRelation;  // 与玩家的关系（None, Friendship, Romance）
    std::string task;                   // npc的特定任务
    bool isTaskCompleted;               // 任务是否完成

    // 对话框相关成员
    cocos2d::Sprite* dialogueBox;                // 对话框背景
    cocos2d::Label* dialogueLabel;               // 对话框中的文本标签
    bool isDialogueVisible;                      // 用于标记对话框是否显示

public:

    static Npc* create(const NpcInfo& npc_info);

    bool init(const NpcInfo& npc_info);

    //task相关获取
    std::string getTask() { return task; }
    bool getTaskStatus() { return isTaskCompleted; }
    void setTaskStatus(bool status) { isTaskCompleted = status; 
    }
    // 获取NPC的名字
    std::string getNpcName() const;

    // 获取NPC的生日
    std::string getBirthday() const;

    // 获取NPC的亲密度
    int getFriendshipLevel() const;

    // 设置NPC的亲密度
    void setFriendshipLevel(int level);

    // 与NPC互动
    void interactWithPlayer();

    // 给NPC送礼物
    std::string giveGift(const std::string& gift);

    // 判断NPC是否喜欢某个礼物
    bool likesGift(const std::string& gift) const;

    // 判断NPC是否讨厌某个礼物
    bool dislikesGift(const std::string& gift) const;

    // 增加NPC的亲密度
    void increaseFriendship(const int amount);

    // 减少NPC的亲密度
    void decreaseFriendship(const int amount);

    // 获取NPC的所有喜欢的礼物
    std::vector<std::string> getGifts() const;

    // 获取NPC的所有讨厌的礼物
    std::vector<std::string> getDislikes() const;

    // 设置NPC的喜欢的礼物
    void setGifts(const std::vector<std::string>& giftList);

    // 设置NPC的讨厌的礼物
    void setDislikes(const std::vector<std::string>& dislikeList);

    // 获取NPC的对话
    std::string printDialogue() const;

    // 获取NPC与玩家的关系状态
    RelationshipStatus getPlayerRelation();

    // 设置与玩家的关系
    void setPlayerRelation(RelationshipStatus status);

    // 输出NPC的当前状态
    std::string printStatus() const;
};

#endif // NPC_H