#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <winuser.h>
#include <winuser.h>

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
	auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	background = experimental::TMXTiledMap::create("racetrack_map.tmx");
	background->setScale(2, 2);
	
	addChild(background);

	auto spawnPoint = background->getObjectGroup("Objects")->getObject("SpawnPoint");
	
	sprite1 = Sprite::create("portrait001.png");
	auto xPos = 2 * spawnPoint["x"].asFloat();
	auto yPos = 2 * spawnPoint["y"].asFloat();
	auto height = spawnPoint["height"].asFloat();
	auto width = spawnPoint["width"].asFloat();
	sprite1->setPosition(Vec2(xPos + width, yPos + height));
	sprite1->setScale(0.2f, 0.2f);
	
	addChild(sprite1);

	auto sprite1Width = sprite1->getContentSize().width;
	auto sprite1Height = sprite1->getContentSize().height;
	auto sprite1Rect = Rect(0, 0, sprite1Width, sprite1Height);

	animFrames.pushBack(SpriteFrame::create("portrait001.png", sprite1Rect));
	animFrames.pushBack(SpriteFrame::create("portrait002.png", sprite1Rect));

	auto finishLineObject = background->getObjectGroup("Objects")->getObject("finishLine");
	finishLineRectangle = Rect{ 2 * finishLineObject["x"].asFloat(), 2 * finishLineObject["y"].asFloat(),
		2 * finishLineObject["width"].asFloat(),2 * finishLineObject["height"].asFloat() };

	auto eventListener = EventListenerKeyboard::create();
	auto eventReleaseListener = EventListenerKeyboard::create();
	eventReleaseListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::keyReleased, this);
	eventListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::keyPressed, this);

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventReleaseListener, this);

	this->scheduleUpdate();
	this->runAction(Follow::create(sprite1));
    return true;
}

void HelloWorld::update(float delta)
{
	//BOUNDARIES without using physics
	bool canMove[4] = {true,true,true,true};
	
	if(!background->getBoundingBox().containsPoint(sprite1->getPosition()))
	{
		if (sprite1->getPositionX() <= background->getPositionX())
		{
			canMove[LEFT] = false;
		}
		if (sprite1->getPositionX() >= background->getPositionX() + 2 * background->getContentSize().width)
		{
			canMove[RIGHT] = false;
		}
		if (sprite1->getPositionY() <= background->getPositionY())
		{
			canMove[DOWN] = false;
		}
		if (sprite1->getPositionY() >= background->getPositionY() + 2 * background->getContentSize().height)
		{
			canMove[TOP] = false;
		}
	}

	auto multiplyFactorDelta = 1 + delta;
	if (arrow_states[LEFT] && canMove[LEFT])
	{
		sprite1->runAction(MoveBy::create(0, Vec2(multiplyFactorDelta * -10, 0)));
	}
	if (arrow_states[RIGHT] && canMove[RIGHT])
	{
		sprite1->runAction(MoveBy::create(0, Vec2(multiplyFactorDelta * 10, 0)));
	}
	if (arrow_states[TOP] && canMove[TOP])
	{
		sprite1->runAction(MoveBy::create(0, Vec2(0, multiplyFactorDelta * 10)));
	}
	if (arrow_states[DOWN] && canMove[DOWN])
	{
		sprite1->runAction(MoveBy::create(0, Vec2(0, multiplyFactorDelta * -10)));
	}
	
	if(finishLineRectangle.containsPoint(sprite1->getPosition()))
	{
		MessageBox("Yay, you reached the finish line", "You win!");
	}
}

void HelloWorld::keyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) arrow_states[RIGHT] = true;
	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) arrow_states[LEFT] = true;
	if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) arrow_states[TOP] = true;
	if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) arrow_states[DOWN] = true;

	if(arrow_states[LEFT] || arrow_states[TOP] || arrow_states[RIGHT] || arrow_states[DOWN])
	{
		auto actionCount = sprite1->getActionManager()->getNumberOfRunningActionsInTarget(sprite1);
		if(actionCount < 1)
		{
			auto animation = Animation::createWithSpriteFrames(animFrames, 0.25f);
			auto animate = Animate::create(animation);
			auto repeat = RepeatForever::create(animate);
			sprite1->runAction(repeat);
		}
	}
}

void HelloWorld::keyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) arrow_states[RIGHT] = false;
	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) arrow_states[LEFT] = false;
	if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) arrow_states[TOP] = false;
	if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) arrow_states[DOWN] = false;

	if(!(arrow_states[LEFT] || arrow_states[TOP] || arrow_states[RIGHT] || arrow_states[DOWN]))
	{
		sprite1->stopAllActions();
		sprite1->setSpriteFrame(animFrames.at(0));
	}
}

void HelloWorld::setPhysicsWorld(PhysicsWorld* world)
{
	this->sceneWorld = world;
}
