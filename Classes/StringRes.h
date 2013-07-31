#ifndef __StringRes_H__
#define __StringRes_H__

#include "cocos2d.h"

USING_NS_CC;

#define ID_STRING_GAME_DESCRIPTION              0
#define ID_STRING_NEW_GAME 						1
#define ID_STRING_CONTINUE 						2
#define ID_STRING_SETTING 						3
#define ID_STRING_GAME_RULER 					4
#define ID_STRING_QUIT 							5
#define ID_STRING_SCORE 						6
#define ID_STRING_HIGH_SCORE 					7
#define ID_STRING_LUCK_CARD 					8
#define ID_STRING_WITHDRAW_CARD 				9
#define ID_STRING_AUDIO_ON 						10
#define ID_STRING_AUDIO_OFF						11
#define ID_STRING_5_BEAD_MODE 					12
#define ID_STRING_6_BEAD_MODE 					13
#define ID_STRING_7_BEAD_MODE 					14
#define ID_STRING_RESET_HIGH_SCORE 				15
#define ID_STRING_BACK 							16
#define ID_STRING_GOT_CARDS_HINT				17
#define ID_STRING_HIT_AD_HINT					18
#define ID_STRING_CANT_WITHDRAW_HINT			19
#define ID_STRING_OPERATOR_SUCC_HINT			20



class StringRes
{
public:
    static StringRes* sharedStringRes();
    
    ~StringRes();

    const char* getString(int key);

private:
    StringRes();

    void init();

    static StringRes *s_sharedRes;


};

#endif /* __Bead_H__ */
