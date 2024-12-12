#pragma once
#include "Skill.h"
#include <unordered_map>
#include <vector>
#include <string>
#include "cocos2d.h"

class SkillTree : public cocos2d::Node{
public:
    // ��ȡȫ��Ψһʵ��
    static SkillTree* getInstance();

    // ��ʼ��������
    bool init();

    // ���Ӽ���
    void addSkill(const std::string& name, Skill* skill);

    // ��������
    bool levelUpSkill(const std::string& name);

    // ��ȡ����
    Skill* getSkill(const std::string& name) const;

    // ��ȡ���м��ܵķ���
    const std::map<std::string, Skill*>& getAllSkills() const;

private:
    SkillTree() = default;  // ���캯��˽�л�����ֹ�ⲿֱ�Ӵ���

    static SkillTree* instance; // ��̬ʵ��

    std::map<std::string, Skill*> skills; // ���ܴ洢
};