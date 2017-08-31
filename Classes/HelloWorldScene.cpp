#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <memory>

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
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
	auto sprite1Body = PhysicsBody::createBox(sprite1->getContentSize());
	sprite1->setPhysicsBody(sprite1Body);
	
	addChild(sprite1);

	auto sprite1Width = sprite1->getContentSize().width;
	auto sprite1Height = sprite1->getContentSize().height;
	auto sprite1Rect = Rect(0, 0, sprite1Width, sprite1Height);

	animFrames.pushBack(SpriteFrame::create("portrait001.png", sprite1Rect));
	animFrames.pushBack(SpriteFrame::create("portrait002.png", sprite1Rect));

	auto finishLineObject = background->getObjectGroup("Objects")->getObject("finishLine");
	finishLineRectangle = Rect{ 2 * finishLineObject["x"].asFloat(), 2 * finishLineObject["y"].asFloat(),
		2 * finishLineObject["width"].asFloat(),2 * finishLineObject["height"].asFloat() };

	auto leftBorder = background->getObjectGroup("Objects")->getObject("leftTerrain");
	auto leftSprite = Sprite::create();
	leftSprite->setPhysicsBody(PhysicsBody::createBox(Size(leftBorder["width"].asFloat(), leftBorder["height"].asFloat())));
	leftSprite->setPosition(Size(leftBorder["width"].asFloat()/2, leftBorder["height"].asFloat()/2));
	background->addChild(leftSprite);

	auto rightBorder = background->getObjectGroup("Objects")->getObject("rightTerrain");
	auto rightSprite = Sprite::create();
	rightSprite->setPhysicsBody(PhysicsBody::createBox(Size(rightBorder["width"].asFloat(), rightBorder["height"].asFloat())));
	rightSprite->setPosition(Size(rightBorder["x"].asFloat() + rightBorder["width"].asFloat()/2 , rightBorder["y"].asFloat() + rightBorder["height"].asFloat()/2));
	background->addChild(rightSprite);

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
	std::unique_ptr<bool[]> canMove{ new bool[4]{ true,true,true,true } };
	canMove = checkFinishLineReached(move(canMove));
	canMove = checkBorders(move(canMove));
	updateMovement(delta, move(canMove));
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

std::unique_ptr<bool[]> HelloWorld::checkBorders(std::unique_ptr<bool[]> canMove) const
{
	if (!background->getBoundingBox().containsPoint(sprite1->getPosition()))
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
	return move(canMove);
}

std::unique_ptr<bool[]> HelloWorld::checkFinishLineReached(std::unique_ptr<bool[]> pCanMove) const
{
	if (finishLineRectangle.containsPoint(sprite1->getPosition()))
	{
		if (sprite1->getChildren().size() < 1)
		{
			auto textField = ui::TextField::create("", "Arial", 300);
			textField->setString("YOU WON!");
			textField->setPosition(Vec2(100.0f, 550.0f));
			sprite1->Node::addChild(textField);
		}
		sprite1->getPhysicsBody()->setLinearDamping(10.0f);
		sprite1->getPhysicsBody()->resetForces();
		pCanMove[LEFT] = false;
		pCanMove[TOP] = false;
		pCanMove[RIGHT] = false;
		pCanMove[DOWN] = false;
	}
	return move(pCanMove);
}

std::unique_ptr<bool[]> HelloWorld::updateMovement(float delta, std::unique_ptr<bool[]> pCanMove) const
{
	auto multiplyFactorDelta = 1 + delta;
	if (arrow_states[LEFT] && pCanMove[LEFT])
	{
		sprite1->runAction(MoveBy::create(0, Vec2(multiplyFactorDelta * -10, 0)));
	}
	if (arrow_states[RIGHT] && pCanMove[RIGHT])
	{
		sprite1->runAction(MoveBy::create(0, Vec2(multiplyFactorDelta * 10, 0)));
	}
	if (arrow_states[TOP] && pCanMove[TOP])
	{
		/*sprite1->runAction(MoveBy::create(0, Vec2(0, multiplyFactorDelta * 10)));*/
		sprite1->getPhysicsBody()->applyForce(Vec2(0, multiplyFactorDelta * 200));
	}
	if (arrow_states[DOWN] && pCanMove[DOWN])
	{
		sprite1->runAction(MoveBy::create(0, Vec2(0, multiplyFactorDelta * -10)));
	}
	return move(pCanMove);
}

void HelloWorld::setPhysicsWorld(PhysicsWorld* world)
{
	this->sceneWorld = world;
}
