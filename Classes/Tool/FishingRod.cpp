#include "FishingRod.h"
#include "../Player/Player.h"
#include "../Scene/FarmScene.h" 
#include "../Constant/Constant.h"
#include "../Bag/Bag.h"
#include "cocos2d.h"
#include "cocos/editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

bool FishingRod::isUsed = false;

FishingRod* FishingRod::create() {
	FishingRod* fishingRod = new (std::nothrow) FishingRod();
	if (fishingRod && fishingRod->init()) {
		fishingRod->autorelease();
		return fishingRod;
	}
	CC_SAFE_DELETE(fishingRod);
	return nullptr;
}

bool FishingRod::init()
{
    // ���û���ĳ�ʼ������
    return Tool::init("fishingRod");
}

void FishingRod::useTool()
{
    // ��ȡ���ʵ��
    Player* player = Player::getInstance();
    // ��������֡
    Vector<SpriteFrame*> frames;

    Vec2 direction = player->getLastDirection();
    // ÿ������֡����ͬ���ܸ���
    if (direction.x > 0) { // ��������Ҳ�
        // �����ӳ���͵Ķ���
        frames.pushBack(SpriteFrame::create(ResPath::RIGHT_FISHING_OUT_1, ToolRects::RIGHT_FISHING_OUT_1));
        frames.pushBack(SpriteFrame::create(ResPath::RIGHT_FISHING_OUT_2, ToolRects::RIGHT_FISHING_OUT_2));
        frames.pushBack(SpriteFrame::create(ResPath::RIGHT_FISHING_OUT_3, ToolRects::RIGHT_FISHING_OUT_3));
        frames.pushBack(SpriteFrame::create(ResPath::RIGHT_FISHING_OUT_4, ToolRects::RIGHT_FISHING_OUT_4));
    }
    else if (direction.x < 0) { // ����������
        frames.pushBack(SpriteFrame::create(ResPath::LEFT_FISHING_OUT_1, ToolRects::LEFT_FISHING_OUT_1));
        frames.pushBack(SpriteFrame::create(ResPath::LEFT_FISHING_OUT_2, ToolRects::LEFT_FISHING_OUT_2));
        frames.pushBack(SpriteFrame::create(ResPath::LEFT_FISHING_OUT_3, ToolRects::LEFT_FISHING_OUT_3));
        frames.pushBack(SpriteFrame::create(ResPath::LEFT_FISHING_OUT_4, ToolRects::LEFT_FISHING_OUT_4));
    }
    else if (direction.y > 0) { // ��������Ϸ�
        frames.pushBack(SpriteFrame::create(ResPath::UP_FISHING_OUT_1, ToolRects::UP_FISHING_OUT_1));
        frames.pushBack(SpriteFrame::create(ResPath::UP_FISHING_OUT_2, ToolRects::UP_FISHING_OUT_2));
        frames.pushBack(SpriteFrame::create(ResPath::UP_FISHING_OUT_3, ToolRects::UP_FISHING_OUT_3));
    }
    else if (direction.y < 0) { // ��������·�
        frames.pushBack(SpriteFrame::create(ResPath::DOWN_FISHING_OUT_1, ToolRects::DOWN_FISHING_OUT_1));
        frames.pushBack(SpriteFrame::create(ResPath::DOWN_FISHING_OUT_2, ToolRects::DOWN_FISHING_OUT_2));
        frames.pushBack(SpriteFrame::create(ResPath::DOWN_FISHING_OUT_3, ToolRects::DOWN_FISHING_OUT_3));
        frames.pushBack(SpriteFrame::create(ResPath::DOWN_FISHING_OUT_4, ToolRects::DOWN_FISHING_OUT_4));  
    }

    // ��������
    auto animation = Animation::createWithSpriteFrames(frames, 0.1f);  // ÿ֡���� 0.1 ��
    auto animate = Animate::create(animation);

    // �������У����Ŷ�����ִ�ж�����߼�
    auto sequence = Sequence::create(
        animate,
        CallFunc::create([=]() {
            // ��������֡
            Vector<SpriteFrame*> framesWaiting;

            Vec2 direction = player->getLastDirection();

            // �����ӳ���͵Ķ���
            if (direction.x > 0) { // ��������Ҳ�
                // �����ӳ���͵Ķ���
                framesWaiting.pushBack(SpriteFrame::create(ResPath::RIGHT_FISHING_WAITING_1, ToolRects::RIGHT_FISHING_WAITING_1));
                framesWaiting.pushBack(SpriteFrame::create(ResPath::RIGHT_FISHING_WAITING_2, ToolRects::RIGHT_FISHING_WAITING_2));
            }
            else if (direction.x < 0) { // ����������
                framesWaiting.pushBack(SpriteFrame::create(ResPath::LEFT_FISHING_WAITING_1, ToolRects::LEFT_FISHING_WAITING_1));
                framesWaiting.pushBack(SpriteFrame::create(ResPath::LEFT_FISHING_WAITING_2, ToolRects::LEFT_FISHING_WAITING_2));
            }
            else if (direction.y > 0) { // ��������Ϸ�
                framesWaiting.pushBack(SpriteFrame::create(ResPath::UP_FISHING_WAITING_1, ToolRects::UP_FISHING_WAITING_1));
                framesWaiting.pushBack(SpriteFrame::create(ResPath::UP_FISHING_WAITING_2, ToolRects::UP_FISHING_WAITING_2));
            }
            else if (direction.y < 0) { // ��������·�
                framesWaiting.pushBack(SpriteFrame::create(ResPath::DOWN_FISHING_WAITING_1, ToolRects::DOWN_FISHING_WAITING_1));
                framesWaiting.pushBack(SpriteFrame::create(ResPath::DOWN_FISHING_WAITING_2, ToolRects::DOWN_FISHING_WAITING_2));
            }


            auto animationWaiting = Animation::createWithSpriteFrames(framesWaiting, 0.5f);
            animationWaiting->setLoops(-1);
            auto animateWait = Animate::create(animationWaiting);

            player->runAction(animateWait);
            }),

        nullptr
    );
    if (isUsed == true) {
        player->stopAllActions(); // ֹͣ��������
        this->reelInRod();      // �����ջض���
        isUsed = false;
    }
    else {
        player->runAction(sequence);
        isUsed = true;
    }
}

void FishingRod::reelInRod()
{
    // ��ȡ���ʵ��
    Player* player = Player::getInstance();
    // ��������֡
    Vector<SpriteFrame*> frames;

    Vec2 direction = player->getLastDirection();
    // ÿ������֡����ͬ���ܸ���
    if (direction.x > 0) { // ��������Ҳ�
        // �����ӳ���͵Ķ���
        frames.pushBack(SpriteFrame::create(ResPath::RIGHT_FISHING_IN_1, ToolRects::RIGHT_FISHING_IN_1));
        frames.pushBack(SpriteFrame::create(ResPath::RIGHT_FISHING_IN_2, ToolRects::RIGHT_FISHING_IN_2));
        frames.pushBack(SpriteFrame::create(ResPath::RIGHT_FISHING_IN_3, ToolRects::RIGHT_FISHING_IN_3));
        frames.pushBack(SpriteFrame::create(ResPath::RIGHT_FISHING_IN_4, ToolRects::RIGHT_FISHING_IN_4));
        frames.pushBack(SpriteFrame::create(ResPath::RIGHT_FISHING_IN_5, ToolRects::RIGHT_FISHING_IN_5));
        frames.pushBack(SpriteFrame::create(ResPath::STAND_RIGHT, ToolRects::STAND_RIGHT));
    }
    else if (direction.x < 0) { // ����������
        frames.pushBack(SpriteFrame::create(ResPath::LEFT_FISHING_IN_1, ToolRects::LEFT_FISHING_IN_1));
        frames.pushBack(SpriteFrame::create(ResPath::LEFT_FISHING_IN_2, ToolRects::LEFT_FISHING_IN_2));
        frames.pushBack(SpriteFrame::create(ResPath::LEFT_FISHING_OUT_3, ToolRects::LEFT_FISHING_IN_3));
        frames.pushBack(SpriteFrame::create(ResPath::LEFT_FISHING_IN_4, ToolRects::LEFT_FISHING_IN_4));
        frames.pushBack(SpriteFrame::create(ResPath::LEFT_FISHING_IN_5, ToolRects::LEFT_FISHING_IN_5));
        frames.pushBack(SpriteFrame::create(ResPath::STAND_LEFT, ToolRects::STAND_LEFT));
    }
    else if (direction.y > 0) { // ��������Ϸ�
        frames.pushBack(SpriteFrame::create(ResPath::UP_FISHING_IN_1, ToolRects::UP_FISHING_IN_1));
        frames.pushBack(SpriteFrame::create(ResPath::UP_FISHING_IN_2, ToolRects::UP_FISHING_IN_2));
        frames.pushBack(SpriteFrame::create(ResPath::UP_FISHING_IN_3, ToolRects::UP_FISHING_IN_3));
        frames.pushBack(SpriteFrame::create(ResPath::STAND_UP, ToolRects::STAND_UP));
    }
    else if (direction.y < 0) { // ��������·�
        frames.pushBack(SpriteFrame::create(ResPath::DOWN_FISHING_IN_1, ToolRects::DOWN_FISHING_IN_1));
        frames.pushBack(SpriteFrame::create(ResPath::DOWN_FISHING_IN_2, ToolRects::DOWN_FISHING_IN_2));
        frames.pushBack(SpriteFrame::create(ResPath::STAND_DOWN, ToolRects::STAND_DOWN));
    }

    // ��������
    auto animation = Animation::createWithSpriteFrames(frames, 0.1f);  // ÿ֡���� 0.1 ��
    auto animate = Animate::create(animation);

    // �������У����Ŷ�����ִ�ж�����߼�
    auto sequence = Sequence::create(
        animate,
        CallFunc::create([=]() {
            // ���ѡ���������
            const Item pufferFish("pufferFish", "items/pufferFish.png");
            const Item tuna("tuna", "items/tuna.png");
            const Item anchovy("anchovy", "items/anchovy.png");

            const std::vector<Item> fishList = {
                pufferFish,
                tuna,
                anchovy
            };
            int randomIndex = rand() % fishList.size();  // �������
            Item fishCaught = fishList[randomIndex];  // ��ȡ�����������

            Bag* bag = Bag::getInstance();
            bag->addItem(&fishCaught);
            // ������Ϸ���ʾ�����������ʾ
            auto label = Label::createWithTTF(fishCaught.name + " Caught!", "fonts/Marker Felt.ttf", 24);
            label->setPosition(player->getPosition() + Vec2(0, 100));  // ��ʾ������Ϸ�
            label->setTextColor(Color4B::WHITE);
            player->getParent()->addChild(label);

            // ����Ч�������뵭��
            label->runAction(Sequence::create(
                FadeIn::create(0.5f),  // ����
                DelayTime::create(2.0f),  // ��ʾ 2 ��
                FadeOut::create(0.5f),  // ����
                RemoveSelf::create(),  // �Ƴ��Լ�
                nullptr
            ));
            }),
            nullptr
            );
    player->runAction(sequence);  // ���Ŷ���
}