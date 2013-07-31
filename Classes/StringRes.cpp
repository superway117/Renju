#include "StringRes.h"


const char* en_strings[] ={ 
    "\
    Q.  How to play the game?\n \
    A.  Every step 3 beads are put on the board\n          at random. Move bead in share of line or\n          slash with same colors.\n \
    \n \
    Q.  How to win the game?\n \
    A.  You can't win it!\n           but you can try to get higher score!\n \
    \n",
    "New Game",
    "Continue",
    "Setting",
    "Game Rulers",
    "Quit",
    "Score",
    "High",
    "Lucky",
    "Withdraw",
    "Audo On",
    "Audo Off",
    "5 Beads Mode",
    "6 Beads Mode",
    "7 Beads Mode",
    "Reset High Score",
    "Back",
    "Got one lucky card and \none withdraw card!",
    "No card available!\nPlease hit Ad to earn cards!",
    "Can't withdraw any more!",
    "Reset Successful!",
    "No card now!\nPlease hit the Ads!"

};

const char* cn_strings[] = {
    "\
    Q.  怎么玩这个游戏?\n \
    A.  在不消行的情况下,每走一步,\n          会有三个随机的珠子落在随机的位置.\n          五个或五个以上相同颜色的珠子能\n          连成一线, 就可以消掉.\n \
    \n \
    Q.  怎么赢得这个游戏?\n \
    A.  这个游戏永远没有赢的那天,\n          你只能试图获得更高的分数!\n \
    \n",
    "新游戏",
    "继续",
    "设置",
    "游戏说明",
    "退出",   //ID_STRING_QUIT   5
    "分数",
    "最高分",
    "幸运卡",  //ID_STRING_LUCK_CARD   8
    "悔棋卡",   //ID_STRING_WITHDRAW_CARD   9
    "打开声音",  //ID_STRING_AUDIO_ON 10
    "关闭声音",
    "五子模式",  //ID_STRING_5_BEAD_MODE  12
    "六子模式",
    "七子模式",
    "清除最高分",
    "Back",
    "已获得一张幸运卡和一张悔棋卡!",
    "卡已经用完!\n请点击广告获得卡片!",
    "不能再悔棋了!",
    "操作成功!",


};

StringRes* StringRes::s_sharedRes = NULL;


StringRes* StringRes::sharedStringRes()
{
    if(s_sharedRes == NULL)
    {
        s_sharedRes = new StringRes();
        s_sharedRes->init();
    }
    return s_sharedRes;
}

StringRes::StringRes()
{
    
}

StringRes::~StringRes()
{

}

void StringRes::init()
{

}

const char* StringRes::getString(int key)
{
    ccLanguageType currentLanguageType = CCApplication::sharedApplication()->getCurrentLanguage();
    if(currentLanguageType == kLanguageChinese)
        return cn_strings[key];
    else
        return en_strings[key];
}