#include "BeadBoardLayer.h"
#include "SimpleAudioEngine.h"
#include "Bead.h"
#include <vector>
#include "Arithmetic.h"
#include "User.h"
#include "DialogLayer.h"
#include "MainMenuLayer.h"

static const char s_pPathClose[] = "Images/close.png";

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

#define  LEFT_SPACE     3.0f
#define  TOP_SPACE      4.0f

#define  TITLE_SPACE    40.0f
#define  BUTTON_SPACE    40.0f

#define  BEAD_GRID_DEFAULT_SIZE  35.0f

#define  INIT_BEAD_NUM  5
#define  EACH_STEP_BEAD_NUM  3

BeadBoardLayer::BeadBoardLayer(): m_selected(-1)
{
    m_gridWidth = BEAD_GRID_DEFAULT_SIZE;
    m_gridHeight = BEAD_GRID_DEFAULT_SIZE;
    m_scale = 1;

    m_boardStartX = LEFT_SPACE;
    m_boardStartY = 0;

}
BeadBoardLayer::~BeadBoardLayer()
{
    releaseAllBeads();
    releaseNextBeads();
}

BeadBoardLayer * BeadBoardLayer::create(bool is_new)
{
    BeadBoardLayer * pRet = new BeadBoardLayer();
    if (pRet && pRet->init(is_new))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

void BeadBoardLayer::initTitle()
{
    CCSize size = getContentSize();
    initNextBeads();
    m_scoreLabel = CCLabelTTF::create("Score","Artial", 20);
    m_scoreLabel->retain();
    m_scoreLabel->setColor( ccc3(0, 0, 0) );
    m_scoreLabel->setPosition( ccp(LEFT_SPACE+38, size.height-TOP_SPACE-16) );
    //m_scoreLabel->setPosition( ccp(0, 0) );
    addChild(m_scoreLabel);

    m_highscoreLabel = CCLabelTTF::create("H:10000","Artial", 20);
    m_highscoreLabel->retain();
    m_highscoreLabel->setColor( ccc3(50, 50, 50) );
    m_highscoreLabel->setPosition( ccp(size.width-LEFT_SPACE-40, size.height-TOP_SPACE-16) );
    addChild(m_highscoreLabel);

    setHighScore(m_keeper.getHighScore());
}

void BeadBoardLayer::initCardBar()
{
    CCScale9Sprite *backgroundButton = CCScale9Sprite::create("Images/button.png");
    CCScale9Sprite *backgroundHighlightedButton = CCScale9Sprite::create("Images/buttonHighlighted.png");

    m_luckyLabel = CCLabelTTF::create("Lucky:10", "Marker Felt", 20);

    CCControlButton *button = CCControlButton::create(m_luckyLabel, backgroundButton);
    button->setBackgroundSpriteForState(backgroundHighlightedButton, CCControlStateHighlighted);
    button->setTitleColorForState(ccWHITE, CCControlStateHighlighted);

    button->setPosition(ccp(button->getContentSize().width/2+40, 2));
    addChild(button);
    button->addTargetWithActionForControlEvents(this, cccontrol_selector(BeadBoardLayer::lucky), CCControlEventTouchDown);

    m_luckyBtn= button;
    setLuckyCardLabel(User::getLuckyCardNum());

    CCScale9Sprite *backgroundButton1 = CCScale9Sprite::create("Images/button.png");
    CCScale9Sprite *backgroundHighlightedButton1 = CCScale9Sprite::create("Images/buttonHighlighted.png");

    m_withdrawLabel = CCLabelTTF::create("WithDraw", "Marker Felt", 20);
    button=CCControlButton::create(m_withdrawLabel, backgroundButton1);
    button->setBackgroundSpriteForState(backgroundHighlightedButton1, CCControlStateHighlighted);
    button->setTitleColorForState(ccWHITE, CCControlStateHighlighted);
    button->setPosition(ccp(200, 2));
    addChild(button);
    button->addTargetWithActionForControlEvents(this, cccontrol_selector(BeadBoardLayer::withdraw), CCControlEventTouchDown);
    m_withdrawBtn = button;
    setWithdrawCardLabel(User::getWithdrawCardNum());
}

void BeadBoardLayer::setLuckyCardLabel(int num)
{
    static char card_num_str[30];
    memset(card_num_str,0,sizeof(card_num_str));
    sprintf(card_num_str,"Lucky:%d",num);
    //m_luckyLabel->setString(card_num_str);
    m_luckyBtn->setTitleForState(new CCString(card_num_str), CCControlStateNormal);

}
void BeadBoardLayer::lucky(CCObject *senderz, CCControlEvent controlEvent)
{
    int num = User::getLuckyCardNum();
    if(num<=0)
    {
        DialogLayer *d = DialogLayer::create("No card now!\nPlease hit the Ad to earn cards!");
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
    memset(card_num_str,0,sizeof(card_num_str));
    sprintf(card_num_str,"Withdraw:%d",num);
    //m_luckyLabel->setString(card_num_str);
    m_withdrawBtn->setTitleForState(new CCString(card_num_str), CCControlStateNormal);

}
void BeadBoardLayer::withdraw(CCObject *senderz, CCControlEvent controlEvent)
{
    int num = User::getWithdrawCardNum();
    if(num<=0)
    {
        DialogLayer *d = DialogLayer::create("No card now!\nPlease hit the Ad to earn cards!");
        addChild(d);
        return;
    }
    if(m_keeper.pop())
    {
        const BoardData data = m_keeper.getCurBoard();
        restoreBoard(data);
        User::deleteWithdrawCard(1);
        setWithdrawCardLabel(num);
    }
    else
    {
        DialogLayer *d = DialogLayer::create("Can't withdraw anymore!");
        addChild(d);
    }
}

void BeadBoardLayer::initBackKey()
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(s_pPathClose, s_pPathClose, this, menu_selector(BeadBoardLayer::backToMainMenu) );
        CCMenu* pMenu =CCMenu::create(pCloseItem, NULL);
        CCSize size = getContentSize();
        pMenu->setPosition( CCPointZero );
        pCloseItem->setPosition(ccp( size.width - 20,0));

        addChild(pMenu);

    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        setKeypadEnabled(true);
    #endif
}
bool BeadBoardLayer::init(bool is_new)
{

//init beads array
    memset(m_beads,0,sizeof(Bead*)*BEADBOARD_SIZE*BEADBOARD_SIZE);
    memset(m_nextBeads,0,sizeof(Bead*)*EACH_STEP_BEAD_NUM);

//init backgroud
    CCSprite* background = CCSprite::create("Images/board.png");

    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCSize bg_size = background->getContentSize();
    setContentSize(CCSize(bg_size.width,bg_size.height+TITLE_SPACE + BUTTON_SPACE) );
    if(size.width > bg_size.width)
    {
        m_scale = size.width/bg_size.width;
        //background->setScale(m_scale);
        m_gridWidth = BEAD_GRID_DEFAULT_SIZE*m_scale;
        m_gridHeight = BEAD_GRID_DEFAULT_SIZE*m_scale;
    }
    background->setPosition(ccp(bg_size.width/2,BUTTON_SPACE+bg_size.height/2));
    addChild(background);

    CCLog("BeadBoardLayer::init m_boardStartY = (%f)",m_boardStartY);

    m_keeper = BoardKeeper();
    initTitle();

    initCardBar();

    initBackKey();

    resetBoard();
    m_keeper.push(m_beads,0);

    setTouchEnabled(true);

    return true;

}

void BeadBoardLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}


void BeadBoardLayer::backToMainMenu(CCObject *pSender)
{
    MainMenuLayer::runInSceen();
}


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void BeadBoardLayer::keyBackClicked()
{
    CCLOG("keyBackClicked");
    backToMainMenu();
}

void BeadBoardLayer::keyMenuClicked()
{
    CCLOG("keyMenuClicked");
}
#endif

void BeadBoardLayer::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    // Choose one of the touches to work with
    CCTouch* touch = (CCTouch*)( touches->anyObject() );
    //CCPoint location = touch->getLocation();
    CCPoint location = convertTouchToNodeSpace(touch);
    CCLog("BeadBoardLayer::ccTouchesEnded touch x:%f, y:%f", location.x, location.y);

    processTouch(location);
    //CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("selected.wav");
}

bool BeadBoardLayer::isValidIndex(int idx)
{
    return (idx>=0 && idx<=BEADBOARD_SIZE*BEADBOARD_SIZE);
}

void BeadBoardLayer::setScore(int score)
{
    static char score_str[30];
    memset(score_str,0,sizeof(score_str));
    sprintf(score_str,"Score:%d",score);
    m_scoreLabel->setString(score_str);

}
void BeadBoardLayer::setHighScore(int highscore)
{
    static char score_str[30];
    memset(score_str,0,sizeof(score_str));
    sprintf(score_str,"H:%d",highscore);
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
    float p_x = point.x;
    float p_y = point.y - BUTTON_SPACE;
    CCSize bg_size = getContentSize();
    CCLog("getGridPosition m_gridWidth=%f,m_gridHeight=%f", m_gridWidth,m_gridHeight);
    CCLog("getGridPosition p_x=%f,p_y=%f", p_x,p_y);
    CCLog("getGridPosition bg_size.width=%f,bg_size.height=%f", bg_size.width,bg_size.height);
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

    float p_x = x*m_gridWidth + LEFT_SPACE + m_gridWidth/2;
    float p_y = y*m_gridHeight + TOP_SPACE + m_gridHeight/2 + BUTTON_SPACE;
    CCLog("getCoordinate p_x=%f,p_y=%f",p_x,p_y);
    point->setPoint(p_x,p_y);
    return true;
}
bool BeadBoardLayer::getCoordinate(int index,CCPoint* point)
{
    CCAssert(index < BEADBOARD_SIZE*BEADBOARD_SIZE,"getCoordinate wrong index");
    int x,y;
    CC_RETURN_VAL_IF_FAIL(getGridPosition(index,&x,&y),false);
    CCLog("getCoordinate idx=%d,x=%d,y=%d",index,x,y);
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
    CCLog("insertBead idx=%d",idx);
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
    CCLog("insertBead color=%d",(int)color);
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
        CCLog("addRandomBeads idx=%d,real_idx=%d",idx,real_idx);
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
    if(getEmptyNum() < EACH_STEP_BEAD_NUM)
    {
        gameOver();
        return;
    }
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
        return;
    }
    CCFiniteTimeAction*  action = CCSequence::create(
        CCDelayTime::create(0.1),
        CCCallFuncND::create( this, callfuncND_selector(BeadBoardLayer::startAddRandomBeadsWithActionCallback),num),
        NULL);
    array->addObject(action);
    if(array->count() > 0 )
    {
        CCSequence *seq = CCSequence::create(array);
        runAction(seq);
    }
}

void BeadBoardLayer::initNextBeads()
{
    CCSize size = getContentSize();
    float x_start=size.width/2;
    for(int i=0; i<EACH_STEP_BEAD_NUM;i++)
    {
        m_nextBeads[i]= Bead::create();
        //need check why retain is needed
        m_nextBeads[i]->retain();
        CCLog("prepareNextBeads idx=%d color=%d",i,m_nextBeads[i]->getBeadColor());

        CCSize itself = m_nextBeads[i]->getContentSize();

        m_nextBeads[i]->setPosition(ccp(x_start+(i-1)*itself.width+2, size.height-itself.height/2-TOP_SPACE));
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
        CCLog("prepareNextBeads idx=%d color=%d",i,m_nextBeads[i]->getBeadColor());
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
    m_selected = -1;
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
    CC_RETURN_VAL_IF_FAIL( getGridPosition( from_index, &x,&y), false);
    Point from(x,y);
    CC_RETURN_VAL_IF_FAIL( getGridPosition( to_index, &x,&y), false);
    Point target(x,y);
    if(!Arithmetic::calcPath(m_beads,BEADBOARD_SIZE,BEADBOARD_SIZE,from,target,path))
        return false;

    stopSelectedAnim();
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
        }
    }
    CCLog("moveBead array->count() =%d", array->count());
    CC_RETURN_VAL_IF_FAIL( array->count() > 0 ,false);

    m_beads[to_index] = m_beads[from_index];
    m_beads[from_index] = NULL;
    m_selected = -1;
    return true;

}


void BeadBoardLayer::selectBead(int index)
{
    CCAssert(index < BEADBOARD_SIZE*BEADBOARD_SIZE,"selectBead wrong index");
    if(index == -1 && isValidIndex(m_selected))
    {
        stopSelectedAnim();
        m_selected = -1;
        return;
    }
    if(m_selected==index) return;
    stopSelectedAnim();
    m_selected = index;
    CCActionInterval*  actionTo = CCScaleBy::create(0.5f, 0.7f);
    m_beads[index]->runAction(CCRepeatForever::create(CCSequence::create( actionTo, actionTo->reverse(), NULL)));

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
        score += Arithmetic::calcScore(h_path.size(),v_path.size(),ls_path.size(),rs_path.size());
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

void BeadBoardLayer::gameOver()
{

}

void BeadBoardLayer::pushBoard()
{
    //m_keeper.push(m_beads,score);
}
#if 1
void BeadBoardLayer::popBoard()
{
    //m_keeper.push(m_beads,score);
}
#endif