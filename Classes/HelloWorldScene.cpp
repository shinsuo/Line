#include "HelloWorldScene.h"

USING_NS_CC;

#define DOT_SEGMENT 20.0f
#define DOT_LENGTH 100
#define Pi  3.1415926f
#define PiAngle 180.0f
#define AREA_LIMIT 20.0f

#define BALL_LENGTH 5
#define BALL_SCALE 0.1f
const char* ball[] = {"Blue-Candy.png","Green-Candy.png","Pink-Candy.png","Red-Candy.png","Yellow-Candy.png"};
#define MONSTER_LENGTH 5
const char* monster[] = {"BigFootcube.png","Frankencube.png","Mummycube.png","Ogrecube.png","Vampcube.png"};

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);

    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
    
    m_BeginPoint = ccp(visibleSize.width/2, AREA_LIMIT);
    
    m_Rect.setRect(-(m_BeginPoint.x - AREA_LIMIT), -(m_BeginPoint.y - AREA_LIMIT), visibleSize.width - 2*AREA_LIMIT, visibleSize.height - 2*AREA_LIMIT);
    
    m_pBatchNode = CCSpriteBatchNode::create("test.png");
    this->addChild(m_pBatchNode);
    m_pBatchNode->setPosition(m_BeginPoint);
    m_pBatchNode->setVisible(false);
    
    for (int i = 0; i < DOT_LENGTH; i++)
    {
        CCSprite* sprite = CCSprite::create("test.png");
        m_pBatchNode->addChild(sprite);
        sprite->setPosition(ccp(0,i*DOT_SEGMENT));
        m_vDot.push_back(sprite);
    }
    
    m_pBall = CCSprite::create(ball[0]);
    this->addChild(m_pBall);
    m_pBall->setPosition(m_BeginPoint);
    
    for (int i = 0; i < MONSTER_LENGTH; i++)
    {
        CCSprite* monsterSp = CCSprite::create(monster[i]);
        this->addChild(monsterSp);
        monsterSp->setPosition(ccp(200*i+200, 300));
        CCLog("%f==%f",monsterSp->getContentSize().width,monsterSp->getContentSize().height);
        
        
        m_vMonster.push_back(monsterSp);
    }
    
    return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

float HelloWorld::getAngle(CCPoint* des, CCPoint* tar)
{
    float angle;
    
    float _x = tar->x - des->x;
    float _y = tar->y - des->y;
    
    angle = atan2f(_y, _x);
    
    for (int i = 0; i < m_vDot.size(); i++)
    {
        float height = i*DOT_SEGMENT;
        
        float y = sinf(angle)*height;
        float x = cosf(angle)*height;
        
        CCPoint pt = ccp(x,y);
        checkRect(&pt);
        
        m_vDot[i]->setPosition(pt);
    }
    
    return angle;
}

BORDER4 HelloWorld::checkRect(cocos2d::CCPoint* pt)
{

    //Top
    float top = m_Rect.origin.y + m_Rect.size.height;
    if (pt->y > top)
    {
        pt->y =  2*top - pt->y;
    }
    
    //Left
    float left = m_Rect.origin.x;
    if (pt->x < left)
    {
        pt->x = 2*left - pt->x;
    }
    
    //Buttom
    float buttom = m_Rect.origin.y;
    if (pt->y < buttom)
    {
        pt->y = 2*buttom - pt->y;
    }
    
    //Right
    float right = m_Rect.origin.x + m_Rect.size.width;
    if (pt->x > right)
    {
        pt->x = 2*right - pt->x;
    }
    
    
    if (!m_Rect.containsPoint(*pt))
    {
        checkRect(pt);
    }
    
    return BORDER_BOTTOM;
}

bool HelloWorld::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_pBatchNode->setVisible(true);
    CCPoint tar = pTouch->getLocation();
    
    CCPoint new_tar = tar;//ccpSub(tar, m_BeginPoint);
    
    float angle = getAngle(&m_BeginPoint, &new_tar);
    
    CCLOG("ccTouchBegan :%f %f == %f %f %f",tar.x,tar.y,angle/Pi*PiAngle,atan2f(1,1)/Pi*PiAngle,tan(45.0f/PiAngle*Pi));
    
    
    
    
    
    return true;
}


void HelloWorld::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{

    CCPoint tar = pTouch->getLocation();
    
    CCPoint new_tar = tar;//ccpSub(tar, m_BeginPoint);
    
    float angle = getAngle(&m_BeginPoint, &new_tar);
    CCLOG("ccTouchMoved :%f %f",tar.x,tar.y);
}

void HelloWorld::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_pBatchNode->setVisible(false);
    CCPoint tar = pTouch->getLocation();
    CCLOG("ccTouchEnded :%f %f",tar.x,tar.y);
}

void HelloWorld::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_pBatchNode->setVisible(false);
    CCPoint tar = pTouch->getLocation();
    CCLOG("ccTouchCancelled :%f %f",tar.x,tar.y);
}

