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

	addChild(background);
	
	sprite1 = Sprite::create("zafer.jpg");
	sprite1->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
	sprite1->setScale(0.2, 0.2);

	addChild(sprite1);

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
			log("REACHED LEFT LIMIT");
			canMove[LEFT] = false;
		}
		if(background->getPositionX() <= -visibleSize.width)
		{
			log("REACHED RIGHT LIMIT");
			canMove[RIGHT] = false;
		}
		if(background->getPositionY() > visibleSize.height/2)
		{
			log("REACHED DOWN LIMIT");
			canMove[DOWN] = false;
		}
		if(background->getPositionY() <= -visibleSize.height)
		{
			log("REACHED TOP LIMIT");
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
}

void HelloWorld::keyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) arrow_states[RIGHT] = false;
	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) arrow_states[LEFT] = false;
	if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) arrow_states[TOP] = false;
	if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) arrow_states[DOWN] = false;
}

void HelloWorld::setPhysicsWorld(cocos2d::PhysicsWorld* world)
{
	this->sceneWorld = world;
}
