#ifndef __BeadBoardLayer_H__
#define __BeadBoardLayer_H__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "Bead.h"
#include "BoardKeeper.h"
#include "BoardConfig.h"
#include "BaseLayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "CCAdView.h"
#endif

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class BeadBoardLayer : public BaseLayer//cocos2d::CCLayer
{
public:
    
    ~BeadBoardLayer();

    //static BeadBoardLayer* getSingleBoard();

    static void runInSceen(bool is_new);
    
    static BeadBoardLayer * create(bool is_new);

    void onEnter();
    void onExit();

    void registerWithTouchDispatcher();

    void ccTouchesEnded(CCSet* touches, CCEvent* event);

    void insertRandBead(int idx);
    void insertBead(int idx,BeadColor color);
    void insertBead(int idx,Bead* bead);

    void releaseBead(int idx);
    void releaseBead(int x,int y);
    void releaseAllBeads();

    void selectBead(int index);

    bool moveBeadWithAction(int from_index, int to_index,CCArray* array);

    bool scanWithAction(int index,CCArray* array,bool addScore);

    void restoreBoard(const BoardData& data);

    void setScore(int score);
    void setHighScore(int highscore);

    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_scoreLabel, ScoreLabel);
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, m_highscoreLabel, HighScoreLabel);

    void setLuckyCardLabel(int score);
    void setWithdrawCardLabel(int num);

private:

    BeadBoardLayer();

    bool getGridPosition(int index, int* x,int* y);

    bool getGridPosition(const CCPoint& point,int* x, int* y);

    bool getGridIndex(const CCPoint& point,int* index);

    bool getGridIndex(int x, int y ,int* index);

    bool getCoordinate(int index,CCPoint* point);

    bool getCoordinate(int x,int y,CCPoint* point);

    bool isValidIndex(int idx);

    virtual bool init(bool is_new);

    void initTitle();

    void initCardBar();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    void initAdMob();
    void deinitAdMob();
    void checkAd();
    #endif
    

    vector<int>* getEmpytGridList();

    void resetBoard();

    void prepareNextBeads();

    void initNextBeads();

    void releaseNextBeads();

    int getEmptyNum();

    bool addRandomBeads(int num);

    void addRandomBeadsWithAction(CCArray* array);

    bool addRandBeadWithAction(CCArray* array,int idx);

    void startAddRandomBeadsWithAction(CCArray* array,int num);

    void startAddRandomBeadsWithActionCallback(CCNode* pSender,void* num);

    void delayDeleteBeadsCallback(CCNode* pSender,void* data);

    void stopSelectedAnim();

    void processTouch(const CCPoint& location);

    bool checkIsGameOver();

    void gameOver();

    //void lucky(CCObject *pSender);
    void lucky(CCObject *sender, CCControlEvent controlEvent);

    void withdraw(CCObject *senderz, CCControlEvent controlEvent);

    


    void congratulate(CCNode* pSender);


private:

    CCSprite*       m_background;
    float           m_boardStartY;

    float           m_gridWidth;
    float           m_gridHeight;
    float           m_scale;

    

    Bead*           m_nextBeads[EACH_STEP_BEAD_NUM];

    Bead*           m_beads[BEADBOARD_SIZE*BEADBOARD_SIZE];

    int             m_selected;

    BoardKeeper     m_keeper;

    CCLabelTTF*     m_luckyLabel;
    CCLabelTTF*     m_withdrawLabel;
    CCControlButton*     m_luckyBtn;
    CCControlButton*     m_withdrawBtn;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCAdView*       m_adLayer;
#endif

    //static BeadBoardLayer*  m_singleBoard;


};

#endif /* __BeadBoardLayer_H__ */
