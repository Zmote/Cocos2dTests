#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
	Node * rootNodePtr;
	Node * sprite1;
	Node * sprite2;
	enum ARROWS{LEFT, TOP, RIGHT, DOWN};
	bool arrow_states[4];
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
	bool onContactBegin(cocos2d::PhysicsContact& pContact);
	void keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event);
	void keyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event);

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	virtual void update(float delta) override;
private:
	cocos2d::PhysicsWorld* sceneWorld;
	void setPhysicsWorld(cocos2d::PhysicsWorld * world);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
