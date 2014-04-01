#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

enum  BORDER4
{
    BORDER_TOP,
    BORDER_LEFT,
    BORDER_BOTTOM,
    BORDER_RIGHT,
};

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
    
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
private:
    cocos2d::CCPoint m_BeginPoint;
    
    std::vector<cocos2d::CCSprite*> m_vDot;
    cocos2d::CCSpriteBatchNode* m_pBatchNode;
    cocos2d::CCRect m_Rect;
    
    float getAngle(cocos2d::CCPoint* des, cocos2d::CCPoint* tar);
    
    BORDER4 checkRect(cocos2d::CCPoint* pt);
};

#endif // __HELLOWORLD_SCENE_H__
