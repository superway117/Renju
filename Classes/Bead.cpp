#include "Bead.h"





Bead::Bead(BeadColor color): m_color(color)
{
    initWithFile(getIconPath());
}
Bead::~Bead()
{
    CCLOG("delete Bead");
}

Bead * Bead::create(BeadColor color)
{
    Bead * pRet = new Bead(color);
    if (pRet)
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}


Bead * Bead::create()
{
    BeadColor color = static_cast<BeadColor>(rand()%static_cast<int>(BeadColorNum));
    return Bead::create(color);
}

void Bead::setRandColor()
{
    BeadColor color = static_cast<BeadColor>(rand()%static_cast<int>(BeadColorNum));
    setBeadColor(color);
}

void Bead::setBeadColor(BeadColor color)
{
    m_color = color;
    CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(getIconPath());
    setTexture(pTexture);
}

const char* Bead::getIconPath()
{
    switch(m_color)
    {
        case BeadColor1:
            return "Images/bead_1.png";
        case BeadColor2:
            return "Images/bead_2.png";
        case BeadColor3:
            return "Images/bead_3.png";
        case BeadColor4:
            return "Images/bead_4.png";
        case BeadColor5:
            return "Images/bead_5.png";
        case BeadColorNum:
            CCAssert(false, "bead color is not right");
            break;

    }
    CCAssert(false, "bead color is not right");
    return NULL;
}

#if 0
bool Bead::isSamePosition(const Bead* other)
{
    CC_RETURN_VAL_IF_FAIL(other,false);
    CC_RETURN_VAL_IF(this == other,true);
    return other->m_x == m_x && other->m_y == m_y;
}
#endif