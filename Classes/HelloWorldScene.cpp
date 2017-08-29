#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	//scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
    // 'layer' is an autorelease object
	auto layer = HelloWorld::create();
	layer->setPhysicsWorld(scene->getPhysicsWorld());

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
	auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);

	auto edgeNode = Node::create();
	edgeNode->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
	edgeNode->setPhysicsBody(edgeBody);
	addChild(edgeNode);

	sprite1 = Sprite::create("zafer.jpg");
	sprite1->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
	sprite1->setScale(0.5, 0.5);
	auto sprite1Body = PhysicsBody::createBox(sprite1->getContentSize(), PhysicsMaterial(0, 1, 0));
	sprite1->setPhysicsBody(sprite1Body);
	sprite1->getPhysicsBody()->setCollisionBitmask(1);
	sprite1->getPhysicsBody()->setContactTestBitmask(true);

	sprite2 = Sprite::create("zafer.jpg");
	sprite2->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 100);
	sprite2->setScale(0.5, 0.5);
	auto sprite2Body = PhysicsBody::createBox(sprite2->getContentSize(), PhysicsMaterial(0, 1, 0));
	sprite2->setPhysicsBody(sprite2Body);
	sprite2->getPhysicsBody()->setCollisionBitmask(2);
	sprite2->getPhysicsBody()->setContactTestBitmask(true);

	addChild(sprite1);
	addChild(sprite2);

	auto eventListener = EventListenerKeyboard::create();
	auto eventReleaseListener = EventListenerKeyboard::create();
	eventReleaseListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::keyReleased, this);
	eventListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::keyPressed, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventReleaseListener, this);
	this->scheduleUpdate();
    return true;
}

void HelloWorld::update(float delta)
{
	if (arrow_states[LEFT]) sprite1->getPhysicsBody()->applyForce(Vec2(-10,0));
	if (arrow_states[TOP]) sprite1->getPhysicsBody()->applyForce(Vec2(0, 10));
	if (arrow_states[RIGHT]) sprite1->getPhysicsBody()->applyForce(Vec2(10, 0));
	if (arrow_states[DOWN]) sprite1->getPhysicsBody()->applyForce(Vec2(0, -10));
}

bool HelloWorld::onContactBegin(PhysicsContact& pContact)
{
	PhysicsBody * a = pContact.getShapeA()->getBody();
	PhysicsBody * b = pContact.getShapeB()->getBody();
	if ((1 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask()) || (2 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask())) {
		log("COLLISION DETECTED");
	}
	return true;
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
