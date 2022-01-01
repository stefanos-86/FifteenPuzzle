#include "FifteenPuzzleBoardScene.h"

#include "BlockNode.h"
#include "ClockNode.h"
#include "EndgameBannerNode.h"
#include "GridNode.h"
#include "UIConstants.h"

USING_NS_CC;

Scene* FifteenPuzzleBoardScene::createScene()
{
    return FifteenPuzzleBoardScene::create();
}

bool FifteenPuzzleBoardScene::init()
{
    if ( !Scene::init() )
        return false;

    setupInput();

    // Comment out to have the board in a winning state (useful for testing).
    board.shuffle();
    
    drawBoard();
    prepareFinalMessage();
    startClock();
    
    return true;
}

void FifteenPuzzleBoardScene::drawBoard()
{
    using namespace fifteenPuzzle;

    // Use the width as a reference, but do not go over the height if it is too small.
    // The grid is square.
    const float gridSize = std::min(getContentSize().width * 0.70f, getContentSize().height);

    grid = GridNode::create(gridSize, Board::SIZE);
    grid->setAnchorPoint(Vec2(0, 0));
    grid->setPosition(gridSize * 0.15, 0);
    addChild(grid);

    for (uint8_t i = 0; i < Board::SIZE; ++i)
        for (uint8_t j = 0; j < Board::SIZE; ++j) {
            const fifteenPuzzle::Cell position{i, j};
            const fifteenPuzzle::BoardValue number = board.get(position);
            if (number != fifteenPuzzle::Board::HOLE) {
                auto cell = BlockNode::create("Square.png", number);
                grid->addChild(cell, i, j);
            }
        }
}

void FifteenPuzzleBoardScene::setupInput()
{
    inputListener = EventListenerMouse::create();
    inputListener->onMouseUp = CC_CALLBACK_1(FifteenPuzzleBoardScene::onMouseUp, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(inputListener, this);
    
    movementListener = EventListenerCustom::create(BlockNode::BLOCK_MOVED_EVENT, CC_CALLBACK_1(FifteenPuzzleBoardScene::onEndMove, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(movementListener, this);
}

void FifteenPuzzleBoardScene::prepareFinalMessage()
{
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto screenHeight = visibleSize.height;
    const auto fallingHeight = visibleSize.height / 2;  // TODO: + half widget, or the vertical centering won't be perfect.

    endgameBanner = EndgameBannerNode::create(UI::BLOCK_BACKGROUND, UI::ENDGAME_MESSAGE, fallingHeight);

    const auto widgetHeight = endgameBanner->getContentSize().height;
    const Vec2 aboveScreen(
        visibleSize.width / 2,  // Center vertically.
        screenHeight + widgetHeight
    );

    endgameBanner->setPosition(aboveScreen);
    endgameBanner->setZOrder(UI::TOP);
    addChild(endgameBanner);
}

void FifteenPuzzleBoardScene::startClock()
{
    clock = ClockNode::CreateWithTTF(UI::FONT, UI::CLOCK_TEXT_SIZE);
    clock->setAnchorPoint({ 0, 1 });
    clock->setPosition(0, getContentSize().height);

    addChild(clock);

    clock->start();
}

void FifteenPuzzleBoardScene::onMouseUp(Event* event)
{
    EventMouse* click = dynamic_cast<EventMouse*>(event);
    assert(click); // But I doubt it can ever be anything else. The cast will be ok.

    const Vec2& locationInView = click->getLocationInView();

    BlockNode* clickHit= dynamic_cast<BlockNode*>(grid->blockAt(locationInView));

    // Don't react to click if the block is still moving. While it is possible to mix
    // running actions, the results are hard to get right. Crashes are also possible.
    if (clickHit && 
        clickHit->getNumberOfRunningActions() == 0) {

        const fifteenPuzzle::Cell originalPosition = grid->cellAt(locationInView);
        if (board.canBeMoved(originalPosition)) {
            const fifteenPuzzle::Cell newPosition = board.moveIntoHole(originalPosition);
            const Vec2 newClickHitPosition = grid->positionOf(newPosition);
            clickHit->moveAnimation(newClickHitPosition);
        }
        else {
            clickHit->popOutAnimation(UI::IMMEDIATELY_S);
        }
    }
}

void FifteenPuzzleBoardScene::onEndMove(cocos2d::Event* event)
{
    // The condition is insufficient. You can make an event "race ahead". Example:
    // move a block, before it stops move another and complete the puzzle. The 1st block
    // end move event happens before the last, but finds the board solved and stops the 
    // game before the last event is in place.
    // Can be solved by checking how many move actions are "in flight", probably.
    if (board.isSolved()) {

        // Clean up before doing the other actions. This callback may be called by an 
        // event at the end of a movement action, as its name says. At that point the origin
        // action is not completed, there will be a crash when the origin node tries to start
        // another action.
        _actionManager->removeAllActions();
        // ...this can't be the right way. I can't find it on the internet and I am not going to
        // mess up with the update callback to implement an action queue.
        // It also does not wait for all actions to complete: anything still running freezes mid-way.

        waveAllBlocks();
        endgameBanner->fallDownAnimation();

        // Freeze everything to block input at the end of the game.
        pause();
        clock->stop();
    }
}

void FifteenPuzzleBoardScene::waveAllBlocks() {
    // Do not construct a parallel action, just reuse the block animations.

    using fifteenPuzzle::Board;

    for (uint8_t diagonalSlice = 0; diagonalSlice < 2 * Board::SIZE - 1; ++diagonalSlice) {
        for (uint8_t row = 0; row <= diagonalSlice; ++row)
        {
            const uint8_t column = diagonalSlice - row;
            if (row >= Board::SIZE || column >= Board::SIZE)
                continue;

            auto pos = grid->positionOf({ column , row });
            auto block = grid->blockAtLocal(pos);

            // Skip the hole.
            if (!block)
                continue;

            dynamic_cast<BlockNode*>(block)->popOutAnimation(diagonalSlice * UI::WAVE_SEPARATION_S);
        }
    }
}


