#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

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
    
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	background = experimental::TMXTiledMap::create("racetrack_map.tmx");
	background->setScale(2, 2);
	background->setPosition(-650.00, 500.00);

	addChild(background);

	sprite1 = Sprite::create("portrait001.png");
	sprite1->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
	sprite1->setScale(0.2f, 0.2f);
	
	addChild(sprite1);


	auto sprite1Width = sprite1->getContentSize().width;
	auto sprite1Height = sprite1->getContentSize().height;
	auto sprite1Rect = Rect(0, 0, sprite1Width, sprite1Height);

	animFrames.pushBack(SpriteFrame::create("portrait001.png", sprite1Rect));
	animFrames.pushBack(SpriteFrame::create("portrait002.png", sprite1Rect));

	auto eventListener = EventListenerKeyboard::create();
	auto eventReleaseListener = EventListenerKeyboard::create();
	eventReleaseListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::keyReleased, this);
	eventListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::keyPressed, this);

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventReleaseListener, this);

	this->scheduleUpdate();
    return true;
}

void HelloWorld::update(float delta)
{
	//BOUNDARIES without using physics, map movement as character movement
	Size visibleSize = Director::getInstance()->getVisibleSize();
	bool canMove[4] = {true,true,true,true};

	if(!background->getBoundingBox().containsPoint(sprite1->getPosition()))
	{
		if(background->getPositionX() > visibleSize.width/2)
		{
			canMove[LEFT] = false;
		}
		if(background->getPositionX() <= -visibleSize.width)
		{
			canMove[RIGHT] = false;
		}
		if(background->getPositionY() > visibleSize.height/2)
		{
			canMove[DOWN] = false;
		}
		if(background->getPositionY() <= -visibleSize.height)
		{
			canMove[TOP] = false;
		}
	}

	if (arrow_states[LEFT] && canMove[LEFT])
	{
		background->runAction(MoveBy::create(0, Vec2(10, 0)));
	}
	if (arrow_states[RIGHT] && canMove[RIGHT])
	{
		background->runAction(MoveBy::create(0, Vec2(-10, 0)));
	}
	if (arrow_states[TOP] && canMove[TOP])
	{
		background->runAction(MoveBy::create(0, Vec2(0, -10)));
	}
	if (arrow_states[DOWN] && canMove[DOWN])
	{
		background->runAction(MoveBy::create(0, Vec2(0, 10)));
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

void HelloWorld::setPhysicsWorld(cocos2d::PhysicsWorld* world)
{
	this->sceneWorld = world;
}
