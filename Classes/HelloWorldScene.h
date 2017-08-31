#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
	cocos2d::Sprite * sprite1;
	cocos2d::experimental::TMXTiledMap * background;
	cocos2d::Vector<cocos2d::SpriteFrame*> animFrames;
	cocos2d::Rect finishLineRectangle;
	enum ARROWS{LEFT, TOP, RIGHT, DOWN};
	bool arrow_states[4];

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
	void keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event);
	void keyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event);

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	void update(float delta) override;
private:
	cocos2d::PhysicsWorld* sceneWorld;
	void setPhysicsWorld(cocos2d::PhysicsWorld * world);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
