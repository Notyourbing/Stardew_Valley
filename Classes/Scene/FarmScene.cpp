#include "FarmScene.h"
#include "../Constant/Constant.h"
#include "../Bag/Bag.h"
#include "../MyButton/MyButton.h"
#include "../Control/Control.h"
#include "../DialogueBox/DialogueBox.h"

USING_NS_CC;

std::vector<Npc*> Farm::npcs;

Scene* Farm::createScene() {
	return Farm::create();
}

bool Farm::init() {
	if (!Scene::init()) {
		return false;
	}
	FarmMap* farmMap = FarmMap::getInstance();
	if (!farmMap->init("Maps/farmSpring11_28/farmMap.tmx")) {
		return false;
	}

	const auto farmMapSize = farmMap->getMapSize();
	farmMap->setPosition(WINSIZE.width / 2 -farmMapSize.width / 2, WINSIZE.height / 2 -farmMapSize.height / 2);
	this->addChild(farmMap, 0);

	// 加入两个NPC
	Npc* wizard = Npc::create(WIZARD_INFO);
	Npc* cleaner = Npc::create(CLEANER_INFO);
	npcs.push_back(cleaner);
	npcs.push_back(wizard);
	farmMap->npcInit(Vec2(WIZARD_X, WIZARD_Y), wizard);
	farmMap->npcInit(Vec2(CLEANER_X, CLEANER_Y), cleaner);
	DialogueBox::isDialogueVisible = false;

	// 玩家
	auto player = Player::getInstance();
	player->setPosition(WINSIZE.width / 2, WINSIZE.height / 2); // 玩家初始位置在屏幕中央
	this->addChild(player, 3);

	// 玩家名字
	auto nameLabel = Label::createWithTTF(player->getPlayerName() + "'s farm", "fonts/Marker Felt.ttf", 24);
	if (nameLabel) {
		nameLabel->setPosition(Vec2(WINSIZE.width / 2, WINSIZE.height - 50));
		this->addChild(nameLabel, 4);
	}

	// 背包
	Bag* bag = Bag::getInstance();
	if (bag) {
		this->addChild(bag, 4);
	}

	// 退出按钮
	auto closeButton = MyButton::create(ResPath::CLOSE_BUTTON_NORMAL, ResPath::CLOSE_BUTTON_HOVER);
	if (closeButton) {
		const auto closeButtonSize = closeButton->getContentSize();
		closeButton->setPosition(Vec2(closeButtonSize.width / 2, WINSIZE.height - closeButtonSize.height / 2)); // 放在左上角
		this->addChild(closeButton, 4);
		closeButton->addClickEventListener(CC_CALLBACK_1(Farm::closeButtonClicked, this));
	}

	Control* control = Control::create();
	this->addChild(control, 4);

	DateManage* dateManage = DateManage::getInstance();
	addChild(dateManage, 5);
	// 启动一个定时器，每秒调用一次 updateDate 方法
	schedule([this, dateManage, farmMap](float deltaTime) {
		dateManage->updateDate();
		farmMap->farmMapUpdateByTime();
		}, 1.0f, "update_date_key");

	return true;
}

// 关闭按钮的回调函数
void Farm::closeButtonClicked(Ref* pSender) {
	Director::getInstance()->popScene();
}