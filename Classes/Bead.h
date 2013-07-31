#ifndef __Bead_H__
#define __Bead_H__

#include "cocos2d.h"
USING_NS_CC;

typedef enum
{
    BeadColor1,
    BeadColor2,
    BeadColor3,
    BeadColor4,
    BeadColor5,
    BeadColor6,
    BeadColor7,
    BeadColorNum
}BeadColor;

class Bead : public CCSprite
{
public:
    Bead(BeadColor color=BeadColor1);
    ~Bead();
    static Bead * create();
    static Bead * create(BeadColor color);

    void setRandColor();

    void setBeadColor(BeadColor color);// {m_color = color;}
    BeadColor getBeadColor() {return m_color;}

    const char* getIconPath();

private:
    BeadColor m_color;

};

#endif /* __Bead_H__ */
