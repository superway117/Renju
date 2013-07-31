#include "BeadBoardLayer.h"
#include "SimpleAudioEngine.h"
#include "Bead.h"
#include <vector>
#include "Arithmetic.h"
#include "User.h"
#include "DialogLayer.h"
#include "MainMenuLayer.h"
#include "SmartRes.h"
#include "BoardConfig.h"
#include "StringRes.h"

//BeadBoardLayer*  BeadBoardLayer::m_singleBoard = NULL;

static const char s_pPathClose[] = "Images/close.png";
static const char s_stars1[]     = "Images/stars.png";

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;

using namespace std;

#define  LEFT_PADDING     3.0f
#define  TOP_PADDING      4.0f

#define  TITLE_SPACE    80.0f
#define  BOTTOM_SPACE    40.0f

#define  BOARD_X_GAP  4.f//35.0f
#define  BOARD_Y_GAP  6.f//35.0f
#define  BEAD_GRID_DEFAULT_SIZE  79.f//35.0f
#define  BEAD_GRID_Y_GAP         4.f//35.0f

#define  INIT_BEAD_NUM  5
#define  EACH_STEP_BEAD_NUM  3


void BeadBoardLayer::runInSceen(bool is_new)
{
    CCScene * pScene = CCScene::create();
    CCLayer * pLayer = BeadBoardLayer::create(is_new);
    pScene->addChild(pLayer);
    CCDirector::sharedDirector()->replaceScene(pScene);
}
BeadBoardLayer::BeadBoardLayer(): m_selected(-1)
{
    m_gridWidth = BEAD_GRID_DEFAULT_SIZE;
    m_gridHeight = BEAD_GRID_DEFAULT_SIZE;
    m_scale = 1;

    m_boardStartY = 0;
    m_closeMenu = false;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    m_adLayer = NULL ;
#endif
    //m_singleBoard = NULL;
}
BeadBoardLayer::~BeadBoardLayer()
{
    releaseAllBeads();
    releaseNextBeads();
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    deinitAdMob();
    
    #endif

}

BeadBoardLayer * BeadBoardLayer::create(bool is_new)
{


    BeadBoardLayer* board= new BeadBoardLayer();
    if (board && board->init(is_new))
    {
        board->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(board);

    }
    return board;
}

void BeadBoardLayer::initTitle()
{

    initNextBeads();
    m_scoreLabel = CCLabelTTF::create("Score:10000","Marker Felt", NORMAL_FONT_SIZE);
    m_scoreLabel->retain();
    m_scoreLabel->setColor( ccWHITE );
    CCSprite* s = CCSprite::create("Images/button2.png");
    m_scoreLabel->setPosition(ccp(s->getContentSize().width/2,s->getContentSize().height/2));
    s->addChild(m_scoreLabel);
    s->setAnchorPoint(ccp(0,0.5));
    s->setPosition(ccp(SMART_LEFT+LEFT_PADDING,SMART_TOP-TITLE_SPACE/2));
    addChild(s);

    s = CCSprite::create("Images/button2.png");

    m_highscoreLabel = CCLabelTTF::create("High:10000","Marker Felt", NORMAL_FONT_SIZE);
    m_highscoreLabel->retain();
    m_highscoreLabel->setPosition(ccp(s->getContentSize().width/2,s->getContentSize().height/2));
    m_highscoreLabel->setColor(ccWHITE );
    s->addChild(m_highscoreLabel);
    s->setAnchorPoint(ccp(1,0.5));
    s->setPosition( ccp(SMART_RIGHT - 1, SMART_TOP-TITLE_SPACE/2) );
    addChild(s);

    setHighScore(m_keeper.getHighScore());
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

void BeadBoardLayer::checkAd()
{

    extern int s_adTouchNum;
    extern bool s_adLastTouchChecked;
    if(!s_adLastTouchChecked)
    {
        CCLog("BeadBoardLayer::checkAd s_adTouchNum %d", s_adTouchNum);
        //deinitAdMob();
        //initAdMob();
        m_adLayer->init();


        s_adLastTouchChecked = true;
        
        setLuckyCardLabel(User::addLuckyCard(1));
        setWithdrawCardLabel(User::addWithdrawCard(1));
        const char* str = StringRes::sharedStringRes()->getString(ID_STRING_GOT_CARDS_HINT);
        DialogLayer *d = DialogLayer::create(str);
        addChild(d);
    }


}
void BeadBoardLayer::deinitAdMob()
{

    if(m_adLayer)
    {
        CCLOG("BeadBoardLayer::deinitAdMob");
        //removeChild(m_adLayer,true);
        //m_adLayer->setParent(0);
        CC_SAFE_RELEASE_NULL(m_adLayer);

        //m_adLayer = NULL;
    }
}

void BeadBoardLayer::initAdMob()
{
    if(m_adLayer)return;
    CCAdSize adsize = kCCAdSizeSmartBanner;
    //CCSize size = CCDirector::sharedDirector()->getWinSize();
    //CCLOG("BeadBoardLayer::initAdMob height=%f",size.height);
    //if(size.height >= 1000)
    //    adsize = kCCAdSizeMediumRectangle;

    m_adLayer = CCAdView::create(adsize, "a151f7d321e46ec");
#if 0
    m_adLayer->setAlignment(kCCHorizontalAlignmentCenter, kCCVerticalAlignmentBottom);
    m_adLayer->setVisible(true);

    m_adLayer->setParent(this);
    m_adLayer->loadAd();
#endif
    CCLOG("BeadBoardLayer::initAdMob end");

    
}
#endif

void BeadBoardLayer::initCardBar()
{
    //CCRect rect = VisibleRect::getVisibleRect();
    CCScale9Sprite *backgroundButton = CCScale9Sprite::create("Images/button2.png");
    CCScale9Sprite *backgroundHighlightedButton = CCScale9Sprite::create("Images/button2.png");

    m_luckyLabel = CCLabelTTF::create("Lucky:10", "Marker Felt", LARGE_FONT_SIZE);

    CCControlButton *button = CCControlButton::create(m_luckyLabel, backgroundButton);
    button->setBackgroundSpriteForState(backgroundHighlightedButton, CCControlStateHighlighted);
    button->setTitleColorForState(ccWHITE, CCControlStateHighlighted);

    //button->setPosition(ccp(button->getContentSize().width/2+40, 2));
    //button->setAnchorPoint(ccp(0, 0));
    float x = SMART_CENTER.x/2 ;
    button->setPosition(ccp(x, m_boardStartY-40));
    addChild(button);
    button->addTargetWithActionForControlEvents(this, cccontrol_selector(BeadBoardLayer::lucky), CCControlEventTouchDown);

    m_luckyBtn= button;
    setLuckyCardLabel(User::getLuckyCardNum());

    CCScale9Sprite *backgroundButton1 = CCScale9Sprite::create("Images/button2.png");
    CCScale9Sprite *backgroundHighlightedButton1 = CCScale9Sprite::create("Images/button2.png");

    m_withdrawLabel = CCLabelTTF::create("WithDraw", "Marker Felt", LARGE_FONT_SIZE);
    button=CCControlButton::create(m_withdrawLabel, backgroundButton1);
    button->setBackgroundSpriteForState(backgroundHighlightedButton1, CCControlStateHighlighted);
    button->setTitleColorForState(ccWHITE, CCControlStateHighlighted);
    //button->setAnchorPoint(ccp(1, 0));
    x = SMART_RIGHT/2 + SMART_CENTER.x/2 ;
    button->setPosition(x, m_boardStartY-40);
    addChild(button);
    button->addTargetWithActionForControlEvents(this, cccontrol_selector(BeadBoardLayer::withdraw), CCControlEventTouchDown);
    m_withdrawBtn = button;
    setWithdrawCardLabel(User::getWithdrawCardNum());
}

void BeadBoardLayer::setLuckyCardLabel(int num)
{
    static char card_num_str[30];
    const char* str = StringRes::sharedStringRes()->getString(ID_STRING_LUCK_CARD);
    memset(card_num_str,0,sizeof(card_num_str));
    sprintf(card_num_str,"%s:%d",str,num);
    //m_luckyLabel->setString(card_num_str);
    m_luckyBtn->setTitleForState(new CCString(card_num_str), CCControlStateNormal);

}
void BeadBoardLayer::lucky(CCObject *senderz, CCControlEvent controlEvent)
{
    int num = User::getLuckyCardNum();
    if(num<=0)
    {
        const char* str = StringRes::sharedStringRes()->getString(ID_STRING_HIT_AD_HINT);
        DialogLayer *d = DialogLayer::create(str);
        addChild(d);
        return;
    }
    num=User::deleteLuckyCard(1);
    setLuckyCardLabel(num);
    prepareNextBeads();
}

void BeadBoardLayer::setWithdrawCardLabel(int num)
{
    static char card_num_str[30];
    const char* str = StringRes::sharedStringRes()->getString(ID_STRING_WITHDRAW_CARD);
    memset(card_num_str,0,sizeof(card_num_str));
    sprintf(card_num_str,"%s:%d",str,num);
    //m_luckyLabel->setString(card_num_str);
    m_withdrawBtn->setTitleForState(new CCString(card_num_str), CCControlStateNormal);

}
void BeadBoardLayer::withdraw(CCObject *senderz, CCControlEvent controlEvent)
{
    int num = User::getWithdrawCardNum();
    if(num<=0)
    {
        const char* str = StringRes::sharedStringRes()->getString(ID_STRING_HIT_AD_HINT);
        DialogLayer *d = DialogLayer::create(str);
        addChild(d);
        return;
    }
    if(m_keeper.pop())
    {
        const BoardData data = m_keeper.getCurBoard();
        restoreBoard(data);
        num=User::deleteWithdrawCard(1);
        setWithdrawCardLabel(num);
    }
    else
    {
        const char* str = StringRes::sharedStringRes()->getString(ID_STRING_CANT_WITHDRAW_HINT);
        DialogLayer *d = DialogLayer::create(str);
        addChild(d);
    }
}


bool BeadBoardLayer::init(bool is_new)
{

    BaseLayer::init();
//init beads array
    memset(m_beads,0,sizeof(Bead*)*BEADBOARD_SIZE*BEADBOARD_SIZE);
    memset(m_nextBeads,0,sizeof(Bead*)*EACH_STEP_BEAD_NUM);

    m_background = CCSprite::create("Images/board.png");

    m_background->setAnchorPoint(ccp(0,1));
    m_background->setPosition(ccp(SMART_LEFT,SMART_TOP - TITLE_SPACE));
    m_boardStartY = SMART_TOP - m_background->getContentSize().height - TITLE_SPACE;
    addChild(m_background);

    CCLog("BeadBoardLayer::init m_boardStartY = (%f),SMART_TOP=%f",m_boardStartY,SMART_TOP);

    initTitle();

    initCardBar();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    initAdMob();
    this->schedule( schedule_selector(BeadBoardLayer::checkAd), 1.0 );
    #endif

    m_keeper.init(is_new);
    if(is_new || !m_keeper.hasValidStore())
    {
        resetBoard();
        m_keeper.push(m_beads,0);
    }
    else
    {
        restoreBoard(m_keeper.getCurBoard());
    }

    setTouchEnabled(true);


    return true;

}
void BeadBoardLayer::onEnter()
{
    if(User::isAudioOn())
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("game.mp3", true);

    CCLayer::onEnter();
}

void BeadBoardLayer::onExit()
{
    if(User::isAudioOn())
        SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(false);

    CCLayer::onExit();
}

void BeadBoardLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}



void BeadBoardLayer::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    // Choose one of the touches to work with
    CCTouch* touch = (CCTouch*)( touches->anyObject() );
    //CCPoint location = touch->getLocation();
    CCPoint location = convertTouchToNodeSpace(touch);
    CCLOG("BeadBoardLayer::ccTouchesEnded touch x:%f, y:%f", location.x, location.y);

    processTouch(location);
}

bool BeadBoardLayer::isValidIndex(int idx)
{
    return (idx>=0 && idx<=BEADBOARD_SIZE*BEADBOARD_SIZE);
}

void BeadBoardLayer::setScore(int score)
{

    static char score_str[30];
    const char* str = StringRes::sharedStringRes()->getString(ID_STRING_SCORE);
    memset(score_str,0,sizeof(score_str));
    sprintf(score_str,"%s:%d",str,score);
    m_scoreLabel->setString(score_str);

}
void BeadBoardLayer::setHighScore(int highscore)
{

    static char score_str[30];
    const char* str = StringRes::sharedStringRes()->getString(ID_STRING_HIGH_SCORE);
    memset(score_str,0,sizeof(score_str));
    sprintf(score_str,"%s:%d",str,highscore);
    m_highscoreLabel->setString(score_str);
}

bool BeadBoardLayer::getGridPosition(int index, int* x,int* y)
{
    CC_RETURN_VAL_IF_FAIL(index >= 0 && index<BEADBOARD_SIZE*BEADBOARD_SIZE,false);
    *y = index/BEADBOARD_SIZE;
    *x = index%BEADBOARD_SIZE;
    return true;
}

bool BeadBoardLayer::getGridPosition(const CCPoint& point,int* x, int* y)
{
    float p_x = point.x - BOARD_X_GAP;
    CCSize bg_size = m_background->getContentSize();
    float p_y = point.y - (m_boardStartY) - BOARD_Y_GAP;
    
    CCLog("getGridPosition m_gridWidth=%f,m_gridHeight=%f", m_gridWidth,m_gridHeight);
    CCLog("getGridPosition p_x=%f,p_y=%f", p_x,p_y);
    //CCLog("getGridPosition bg_size.width=%f,bg_size.height=%f", bg_size.width,bg_size.height);
    CC_RETURN_VAL_IF_FAIL((p_x>=0 && p_x <= bg_size.width),false);
    CC_RETURN_VAL_IF_FAIL((p_y>=0 && p_y<=bg_size.height),false);
    *x = p_x / m_gridWidth;
    *y = p_y / m_gridHeight;
    if(*y>=BEADBOARD_SIZE)  *y= BEADBOARD_SIZE-1;
    if(*x>=BEADBOARD_SIZE)  *x= BEADBOARD_SIZE-1;
    return true;

}

bool BeadBoardLayer::getGridIndex(const CCPoint& point,int* index)
{
    int x=0;
    int y=0;
    CC_RETURN_VAL_IF_FAIL(getGridPosition(point,&x,&y),false);
    *index = y*BEADBOARD_SIZE+x;
    return true;
}

bool BeadBoardLayer::getCoordinate(int x,int y,CCPoint* point)
{
    CCAssert(x < BEADBOARD_SIZE && y < BEADBOARD_SIZE,"getCoordinate wrong xy");
    float p_x = x*m_gridWidth  + m_gridWidth/2 + BOARD_X_GAP;
    float p_y = y*m_gridHeight + m_boardStartY + BOARD_Y_GAP + m_gridHeight/2 ;
    //CCLog("getCoordinate p_x=%f,p_y=%f",p_x,p_y);
    point->setPoint(p_x,p_y);
    return true;
}
bool BeadBoardLayer::getCoordinate(int index,CCPoint* point)
{
    CCAssert(index < BEADBOARD_SIZE*BEADBOARD_SIZE,"getCoordinate wrong index");
    int x,y;
    CC_RETURN_VAL_IF_FAIL(getGridPosition(index,&x,&y),false);
    //CCLog("getCoordinate idx=%d,x=%d,y=%d",index,x,y);
    return getCoordinate(x,y,point);
}

bool BeadBoardLayer::getGridIndex(int x, int y ,int* index)
{
    CC_RETURN_VAL_IF_FAIL(x<BEADBOARD_SIZE && y<BEADBOARD_SIZE,false);
    *index = (y*BEADBOARD_SIZE+x);
    return true;
}


int BeadBoardLayer::getEmptyNum()
{

    int num = 0;
    for(int i=0; i<BEADBOARD_SIZE*BEADBOARD_SIZE;i++)
    {
        if(!m_beads[i]) num++;
    }
    return num;
}

std::vector<int>* BeadBoardLayer::getEmpytGridList()
{
    std::vector<int>* list = new vector<int>();
    for(int i=0; i<BEADBOARD_SIZE*BEADBOARD_SIZE;i++)
    {
        if(!m_beads[i]) list->push_back(i);
    }
    return list;
}

void BeadBoardLayer::releaseBead(int idx)
{
    if(m_beads[idx])
    {
        removeChild(m_beads[idx],true);
        m_beads[idx] = NULL;

    }
}
void BeadBoardLayer::releaseBead(int x,int y)
{
    int index = 0;
    CC_RETURN_IF_FAIL( getGridIndex( x, y,&index));
    releaseBead(index);
}
void BeadBoardLayer::releaseAllBeads()
{
    for(int i=0; i<BEADBOARD_SIZE*BEADBOARD_SIZE;i++)
    {
        releaseBead(i);
    }
}

void BeadBoardLayer::insertBead(int idx,Bead* bead)
{
    CCAssert(bead,"insertBead NULL");
    //CCLog("insertBead idx=%d",idx);
    CCPoint point;
    if(!getCoordinate(idx,&point))
        CCAssert(false,"the coordinate is wrong");
    releaseBead(idx);
    bead->setPosition(point);
    m_beads[idx] = bead;
    addChild(m_beads[idx]);
}

void BeadBoardLayer::insertBead(int idx,BeadColor color)
{
    //CCLog("insertBead color=%d",(int)color);
    Bead* bead= Bead::create(color);
    insertBead(idx,bead);
}

void BeadBoardLayer::insertRandBead(int idx)
{
    Bead* bead= Bead::create();
    insertBead(idx,bead);
}
//TODO: need check if the added bead could cause scan true
//only init use this function ,do not need check scan
bool BeadBoardLayer::addRandomBeads(int num)
{
    std::vector<int>* list  = getEmpytGridList();
    int size = list->size();
    if(!list || size<num)
    {
        if(list) delete list;
        return false;
    }
    while(num>0)
    {
        int idx = rand() % size;
        int real_idx = list->at(idx);
        //CCLog("addRandomBeads idx=%d,real_idx=%d",idx,real_idx);
        if(m_beads[real_idx] != NULL)
            continue;
        //CCAssert(m_beads->data[list[idx]]==NULL,"should be empyt slot");
        insertRandBead(real_idx);
        num--;
        list->erase(list->begin()+idx);
        size--;
    }
    delete list;
    return true;
}

bool BeadBoardLayer::addRandBeadWithAction(CCArray* array,int prepare_index)
{
    std::vector<int>* list  = getEmpytGridList();
    int size = list->size();
    if(!list)
    {
        return false;
    }
    int idx = rand() % size;
    int real_idx = list->at(idx);

    CCLog("addRandBeadWithAction idx=%d,real_idx=%d prepare_index=%d",idx,real_idx,prepare_index);
    CCAssert(m_nextBeads[prepare_index],"prepare bead should not be null");
    Bead* bead = m_nextBeads[prepare_index];

    insertBead(real_idx,bead->getBeadColor());

    delete list;
    scanWithAction(real_idx,array,false);

    return true;
}

void BeadBoardLayer::startAddRandomBeadsWithAction(CCArray* array,int num)
{
    //check if lose the game
    #if 0
    if(checkIsGameOver())
    {
        gameOver();
        return;
    }
    #endif
    int *p_num = (int*)malloc(sizeof(int));
    *p_num = num;
    CCFiniteTimeAction*  action = CCSequence::create(
        CCDelayTime::create(0.1),
        CCCallFuncND::create( this, callfuncND_selector(BeadBoardLayer::startAddRandomBeadsWithActionCallback),p_num ),
        NULL);

    //runAction(action);
    array->addObject(action);
}

void BeadBoardLayer::startAddRandomBeadsWithActionCallback(CCNode* pSender,void* num)
{
    CCArray* array = CCArray::createWithCapacity(20);
    int* p_num = (int*)num;
    CCAssert(*p_num>0," num should be larger than 0");
    --(*p_num);
    CCLog("startAddRandomBeadsWithActionCallback num=%d",*p_num);
    addRandBeadWithAction(array,*p_num);

    if((*p_num) == 0)
    {
        free(p_num);
        m_keeper.push(m_beads,m_keeper.getCurScore());

        prepareNextBeads();
        if(checkIsGameOver())
        {
            gameOver();
            array->removeAllObjects();
            CC_SAFE_RELEASE(array);
            return;
        }
    }
    else
    {
        CCFiniteTimeAction*  action = CCSequence::create(
            CCDelayTime::create(0.1),
            CCCallFuncND::create( this, callfuncND_selector(BeadBoardLayer::startAddRandomBeadsWithActionCallback),num),
            NULL);
        array->addObject(action);
    }
    if(array->count() > 0 )
    {
        CCSequence *seq = CCSequence::create(array);
        runAction(seq);
    }
    else
    {
        CC_SAFE_RELEASE(array);
    }
}

void BeadBoardLayer::initNextBeads()
{
    //CCPoint top = VisibleRect::leftTop();
    //CCSize size = getContentSize();
    float x_start=SMART_CENTER.x;//size.width/2;
    CCLOG("initNextBeads x=%f",x_start);
    for(int i=0; i<EACH_STEP_BEAD_NUM;i++)
    {
        m_nextBeads[i]= Bead::create();
        //need check why retain is needed
        m_nextBeads[i]->retain();
        CCLog("prepareNextBeads idx=%d color=%d",i,m_nextBeads[i]->getBeadColor());

        CCSize itself = m_nextBeads[i]->getContentSize();

        m_nextBeads[i]->setPosition(ccp(x_start+(i-1)*itself.width+2, SMART_TOP  - TITLE_SPACE/2));
        addChild(m_nextBeads[i]);

    }
}

void BeadBoardLayer::releaseNextBeads()
{
    for(int i=0; i<EACH_STEP_BEAD_NUM;i++)
    {
       if(m_nextBeads[i])
       {
            removeChild(m_nextBeads[i],true);
            m_nextBeads[i] = NULL;
      }
    }
}

void BeadBoardLayer::prepareNextBeads()
{
    for(int i=0; i<EACH_STEP_BEAD_NUM;i++)
    {
        m_nextBeads[i]->setRandColor();
        //CCLog("prepareNextBeads idx=%d color=%d",i,m_nextBeads[i]->getBeadColor());
    }
}

void BeadBoardLayer::resetBoard()
{
    releaseAllBeads();
    m_selected = -1;
    addRandomBeads(INIT_BEAD_NUM);
    setScore(0);
}

void BeadBoardLayer::restoreBoard(const BoardData& data)
{
    stopSelectedAnim();
    //m_selected = -1;
    for(int i=0; i<BEADBOARD_SIZE*BEADBOARD_SIZE;i++)
    {
        releaseBead(i);
        if(data.m_beads[i]>=0)
        {
            insertBead(i,(BeadColor)data.m_beads[i]);
        }
    }
    setScore(data.m_score);
}

void BeadBoardLayer::stopSelectedAnim()
{
    if(isValidIndex(m_selected))
    {
        CCLog("stopSelectedAnim  m_selected=%d",m_selected);
        CCAssert(m_beads[m_selected],"stopSelectedAnim selected null");
        m_beads[m_selected]->stopAllActions();
        m_beads[m_selected]->setScale(1.0f);
        m_selected = -1;
    }
}

void BeadBoardLayer::processTouch(const CCPoint& location)
{
    

    int index= 0;

    CC_RETURN_IF_FAIL(getGridIndex(location,&index));
    CCLog("BeadBoardLayer::touch index %d", index);

    if(m_beads[index])
        selectBead(index);
    else
    {
        if( isValidIndex(m_selected) )
        {


            CCArray *array = CCArray::createWithCapacity(100);

            if(moveBeadWithAction(m_selected,index,array))
            {

                if(!scanWithAction(index,array,true))
                {
                    //m_keeper.push(m_beads,m_keeper.getCurScore());
                    startAddRandomBeadsWithAction(array,EACH_STEP_BEAD_NUM);
                }
            }
            else
            {
                if(User::isAudioOn())
                    SimpleAudioEngine::sharedEngine()->playEffect("error.wav",false);
            }


            if(array->count() > 0 )
            {
                CCSequence *seq = CCSequence::create(array);
                m_beads[index]->runAction(seq);
            }

        }
    }

}

bool BeadBoardLayer::moveBeadWithAction(int from_index, int to_index,CCArray *array)
{
    vector<class Point> path;
    int x;
    int y;
    int count= 0;
    CC_RETURN_VAL_IF_FAIL( getGridPosition( from_index, &x,&y), false);
    Point from(x,y);
    CC_RETURN_VAL_IF_FAIL( getGridPosition( to_index, &x,&y), false);
    Point target(x,y);
    if(!Arithmetic::calcPath(m_beads,BEADBOARD_SIZE,BEADBOARD_SIZE,from,target,path))
        return false;

    
    for (std::vector<class Point>::iterator it = path.begin() ; it != path.end(); ++it)
    {
        Point p = *it;
        CCPoint point;
        CCLog("moveBead Point x=%d,%d", p.x,p.y);
        if(getCoordinate(p.x,p.y,&point))
        {
            CCLog("moveBead CCPoint x=%f,%f", point.x,point.y);
            CCActionInterval*  actionTo = CCMoveTo::create(0.1, point);
            array->addObject(actionTo);
            count++;
        }
    }
    CCLog("moveBead array->count() =%d", array->count());
    CC_RETURN_VAL_IF_FAIL( count > 0 ,false);
    stopSelectedAnim();
    m_beads[to_index] = m_beads[from_index];
    m_beads[from_index] = NULL;
    //m_selected = -1;
    return true;

}


void BeadBoardLayer::selectBead(int index)
{
    CCAssert(index < BEADBOARD_SIZE*BEADBOARD_SIZE,"selectBead wrong index");
    if(index == -1 && isValidIndex(m_selected))
    {
        stopSelectedAnim();
        //m_selected = -1;
        return;
    }
    if(m_selected==index) return;
    stopSelectedAnim();
    m_selected = index;
    CCActionInterval*  actionTo = CCScaleBy::create(0.5f, 0.7f);
    m_beads[index]->runAction(CCRepeatForever::create(CCSequence::create( actionTo, actionTo->reverse(), NULL)));
    //test
    //m_adLayer->autoHit();
}


bool BeadBoardLayer::scanWithAction(int index,CCArray* array,bool addScore)
{
    int x,y;
    CC_RETURN_VAL_IF_FAIL(getGridPosition(index,&x,&y),false);
    Point p(x,y);


    int num = 0;
    CCLog("scan x=%d,y=%d",x,y);
    vector<class Point> * delete_array = new vector<class Point>;
    vector<class Point> rs_path;
    if(Arithmetic::rightSlantingScan(m_beads,BEADBOARD_SIZE,BEADBOARD_SIZE,p,rs_path))
    {
        num++;
        CCLog("scan rightSlantingScan true rs_path.size=%d",rs_path.size());
        for (std::vector<class Point>::iterator it = rs_path.begin() ; it != rs_path.end(); ++it)
        {
            Point t = *it;
            CCLog("scan rightSlantingScan x=%d,y=%d",t.x,t.y);
            //releaseBead(t.x,t.y);
            delete_array->push_back(t);
        }
    }
    vector<class Point> ls_path;
    if(Arithmetic::leftSlantingScan(m_beads,BEADBOARD_SIZE,BEADBOARD_SIZE,p,ls_path))
    {
        num++;
        CCLog("scan leftSlantingScan true ls_path.size=%d",ls_path.size());
        for (std::vector<class Point>::iterator it = ls_path.begin() ; it != ls_path.end(); ++it)
        {
            Point t = *it;
            CCLog("scan leftSlantingScan x=%d,y=%d",t.x,t.y);
            //releaseBead(t.x,t.y);
            delete_array->push_back(t);
        }
    }
    vector<class Point> v_path;
    if(Arithmetic::verticalScan(m_beads,BEADBOARD_SIZE,BEADBOARD_SIZE,p,v_path))
    {
        num++;
        CCLog("scan verticalScan true v_path.size=%d",v_path.size());
        for (std::vector<class Point>::iterator it = v_path.begin() ; it != v_path.end(); ++it)
        {
            Point t = *it;
            CCLog("scan verticalScan x=%d,y=%d",t.x,t.y);
            //releaseBead(t.x,t.y);
            delete_array->push_back(t);
        }
    }
    vector<class Point> h_path;
    if(Arithmetic::horizontalScan(m_beads,BEADBOARD_SIZE,BEADBOARD_SIZE,p,h_path))
    {
        num++;

        CCLOG("scan horizontalScan true h_path.size=%d",h_path.size());
        for (std::vector<class Point>::iterator it = h_path.begin() ; it != h_path.end(); ++it)
        {
            Point t = *it;
            CCLOG("scan horizontalScan x=%d,y=%d",t.x,t.y);
            //releaseBead(t.x,t.y);
            delete_array->push_back(t);
        }
        if(!addScore)
        {
            CCLOG("scan horizontalScan without add score");
        }
    }

    if(num==0)
    {
        delete delete_array;
        return false;
    }
    delete_array->push_back(p);
    CCFiniteTimeAction*  action = CCSequence::create(
        CCDelayTime::create(0.5),
        CCCallFuncND::create( this, callfuncND_selector(BeadBoardLayer::delayDeleteBeadsCallback),delete_array),
        NULL);
    array->addObject(action);
    int score = m_keeper.getCurScore();
    if(score == -1) score = 0;
    if(addScore)
    {
        int newScore = Arithmetic::calcScore(h_path.size(),v_path.size(),ls_path.size(),rs_path.size());
        if((score/500)<((score+newScore)/500))
        {
            CCFiniteTimeAction*  action = CCSequence::create(
                CCCallFuncN::create( this, callfuncN_selector(BeadBoardLayer::congratulate)),
                NULL);
            array->addObject(action);
        }
        score += newScore;
    }
    m_keeper.push(score);
    return true;

}


void BeadBoardLayer::delayDeleteBeadsCallback(CCNode* pSender,void* data)
{
    vector<class Point>* array = reinterpret_cast<vector<class Point>*>(data);
    for (std::vector<class Point>::iterator it = array->begin() ; it != array->end(); ++it)
    {
        Point t = *it;
        CCLog("delayDeleteBeadsCallback x=%d,y=%d",t.x,t.y);
        releaseBead(t.x,t.y);
    }
    delete(array);
    int score = m_keeper.getCurScore();
    setScore(score);
    if(score == m_keeper.getHighScore())
        setHighScore(score);
    m_keeper.setBoardData(m_beads);
}

bool BeadBoardLayer::checkIsGameOver()
{
    return (getEmptyNum() < EACH_STEP_BEAD_NUM);
}

void BeadBoardLayer::gameOver()
{
    CCParticleSystem*    emitter  = CCParticleExplosion::create();
    m_background->addChild(emitter, 10);

    emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage(s_stars1) );
    emitter->setPosition( SMART_CENTER );
    emitter->setAutoRemoveOnFinish(true);
    emitter->setDuration(4);

    DialogLayer *d = DialogLayer::create("Game Over!");
    addChild(d);
}

void BeadBoardLayer::congratulate(CCNode* pSender)
{
    CCParticleSystem*    emitter = CCParticleFireworks::create();
    m_background->addChild(emitter, 10);

    emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage(s_stars1) );
    //CCSize s = CCDirector::sharedDirector()->getWinSize();
    emitter->setPosition( SMART_CENTER );
    emitter->setAutoRemoveOnFinish(true);
    emitter->setDuration(4);

}
