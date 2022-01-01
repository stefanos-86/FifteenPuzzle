#pragma once

#include "cocos2d.h"

/**
@brief    The cocos2d Application.
*/
class  AppDelegate : private cocos2d::Application
{
public:
    virtual ~AppDelegate();

    /** Setup OpenGL. Left as auto-generated. */
    void initGLContextAttrs() final;

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    bool applicationDidFinishLaunching()final;

    /**
    @brief  Called when the application moves to the background
    @param  the pointer of the application
    */
    void applicationDidEnterBackground() final;

    /**
    @brief  Called when the application reenters the foreground
    @param  the pointer of the application
    */
    void applicationWillEnterForeground() final;
};


