#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
	cocos2d::PhysicsWorld* sceneWorld = nullptr;
	cocos2d::Sprite * sprite1 = nullptr;
	cocos2d::experimental::TMXTiledMap * background = nullptr;
	cocos2d::Vector<cocos2d::SpriteFrame*> animFrames;
	cocos2d::Rect finishLineRectangle;
	enum ARROWS { LEFT, TOP, RIGHT, DOWN };
	bool arrow_states[4];
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	bool init() override;
	void update(float delta) override;
private:	
	void keyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event);
	void keyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event);
	std::unique_ptr<bool[]> checkBorders(std::unique_ptr<bool[]> pCanMove) const;
	std::unique_ptr<bool[]> checkFinishLineReached(std::unique_ptr<bool[]> pCanMove) const;
	std::unique_ptr<bool[]> updateMovement(float pDelta, std::unique_ptr<bool[]> pCanMove) const;
	void setPhysicsWorld(cocos2d::PhysicsWorld * world);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
