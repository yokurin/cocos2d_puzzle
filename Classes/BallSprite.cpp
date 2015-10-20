#include "BallSprite.h"

USING_NS_CC;

//コンストラクタ
BallSprite::BallSprite()
: _removedNo(0)
, _checkedX(false)
, _checkedY(false)
, _fallCount(0)
, _positionIndex(0, 0)
{
}

//インスタンス生成
BallSprite* BallSprite::create(BallType type, bool visible)
{
    BallSprite *pRet = new BallSprite();
    if (pRet && pRet->init(type, visible))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

//初期化
bool BallSprite::init(BallType type, bool visible)
{
    if (!Sprite::initWithFile(getBallImageFilePath(type)))
        return false;
    
    // ボールのサイズにあわせて拡大縮小
    setScale(BALL_SIZE/106.0f);
    
    _ballType = type;
    
    setVisible(visible);
    
    return true;
}

//パラメータのリセット
void BallSprite::resetParams()
{
    _removedNo = 0;
    _checkedX = false;
    _checkedY = false;
    _fallCount = 0;
}

//初期位置へ移動
void BallSprite::resetPosition()
{
    //位置を変更する
    setPosition(getPositionForPositionIndex(_positionIndex));
}

//位置インデックスを返す
BallSprite::PositionIndex BallSprite::getPositionIndex()
{
    //位置インデックスを返す
    return _positionIndex;
}

//位置インデックスとタグを変更する
void BallSprite::setPositionIndex(PositionIndex positionIndex)
{
    //位置インデックスを保持する
    _positionIndex = positionIndex;
    
    //タグをセットする
    setTag(generateTag(_positionIndex));
}

//インデックスとタグと位置を変更する
void BallSprite::setPositionIndexAndChangePosition(PositionIndex positionIndex)
{
    //インデックスとタグを変更する
    setPositionIndex(positionIndex);
    
    //位置を変更する
    resetPosition();
}

//消去アニメーションと落下アニメーション
void BallSprite::removingAndFallingAnimation(int maxRemovedNo)
{
    //ボールを消すアニメーション
    removingAnimation(maxRemovedNo);
    
    //ボールを落とすアニメーション
    fallingAnimation(maxRemovedNo);
}

//ボール削除アニメーション
void BallSprite::removingAnimation(int maxRemovedNo)
{
    if (_removedNo > 0)
    {
        //ボールを消すアニメーション
        auto delay1 = DelayTime::create(ONE_ACTION_TIME * (_removedNo - 1));
        auto fade = FadeTo::create(ONE_ACTION_TIME, 0);
        auto delay2 = DelayTime::create(ONE_ACTION_TIME * (maxRemovedNo - _removedNo));
        auto removeSelf = RemoveSelf::create(false);
        
        //アニメーション実行
        runAction(Sequence::create(delay1, fade, delay2, removeSelf, nullptr));
    }
}

//ボール落下アニメーション
void BallSprite::fallingAnimation(int maxRemovedNo)
{
    if (_fallCount > 0)
    {
        //ボールを落とすアニメーション
        setPositionIndex(PositionIndex(_positionIndex.x, _positionIndex.y - _fallCount));
        
        auto delay = DelayTime::create(ONE_ACTION_TIME * maxRemovedNo);
        auto show = Show::create();
        auto move = MoveTo::create(ONE_ACTION_TIME, getPositionForPositionIndex(getPositionIndex()));
        runAction(Sequence::create(delay, show, move, nullptr));
    }
}

//ボール画像取得
std::string BallSprite::getBallImageFilePath(BallType type)
{
    //ボールタイプに適した画像を返す
    switch (type)
    {
        case BallType::Red: return "red.png";
        case BallType::Blue: return "blue.png";
        case BallType::Yellow: return "yellow.png";
        case BallType::Green: return "green.png";
        case BallType::Purple: return "purple.png";
        default: return "pink.png";
    }
}

//位置インデックスからPointを取得
Point BallSprite::getPositionForPositionIndex(PositionIndex positionIndex)
{
    return Point(BALL_SIZE * (positionIndex.x - 0.5) + 1,
                 BALL_SIZE * (positionIndex.y - 0.5) + 1);
}

#define BALL_X_MAX (100)     // パズル横幅MAX値

//位置インデックスからタグを取得
int BallSprite::generateTag(PositionIndex positionIndex)
{
    return positionIndex.x * BALL_X_MAX + positionIndex.y;
}
