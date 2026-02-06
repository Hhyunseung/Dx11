#pragma once
#include "Asset.h"

#include "ATexture.h"

class ASprite :
    public Asset
{
private:
    Ptr<ATexture> m_Atlas;
    Vec2 	      m_LeftTopUV;
    Vec2          m_SliceUV;

public:
    GET_SET(Ptr<ATexture>, Atlas);
    GET_SET(Vec2, LeftTopUV);
    GET_SET(Vec2, SliceUV);

public:
    ASprite();
    virtual ~ASprite();
};

