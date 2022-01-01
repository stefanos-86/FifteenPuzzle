#pragma once

#include "cocos2d.h"

#include "Board.h"

class ClockNode;
class GridNode;
class EndgameBannerNode;


class FifteenPuzzleBoardScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(FifteenPuzzleBoardScene);

private:
    void drawBoard();
    void setupInput();
    void prepareFinalMessage();
    void startClock();

    /** Trigger on input. Since the input is not linked to a node, I can have one listener
    in the board rather than one listener per sprite. */
    void onMouseUp(cocos2d::Event* event);

    /** To be called after every movement of a block, to check if the game is completed. */
    void onEndMove(cocos2d::Event* event);

    /** Uses the block idle animations with delay to pulse them to do "the wave". */
    void waveAllBlocks();


    cocos2d::EventListenerMouse* inputListener;
    cocos2d::EventListenerCustom* movementListener;
    ClockNode* clock;
    GridNode* grid;
    EndgameBannerNode* endgameBanner;
    fifteenPuzzle::Board board;

};

