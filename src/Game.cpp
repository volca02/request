/******************************************************************************
 *
 *    This file is part of ReQuest
 *    Copyright (C) 2008-2009 Filip Volejnik
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *        $Id$
 *
 *****************************************************************************/

#include <Ogre.h>
#include <OgreResourceGroupManager.h>
#include <OgreResourceManager.h>
#include <OgreFontManager.h>

#include "Game.h"

#include "ConfigStorage.h"

#include "InputSystem.h"
#include "RendererSystem.h"
#include "EffectSystem.h"
#include "Exception.h"

#include "MenuState.h"
#include "PlayState.h"

namespace ReQuest {

// ----------------------------------------------------------------------------
// ----- Game Class -----------------------------------------------------------
// ----------------------------------------------------------------------------
Game::Game(void) :
    mOgreRoot(),
    mPlayState(0),
    mMenuState(0),
    mCurrentState(0),
    mTargetState(0),
    mStateChangeRequested(0),
    mConfigStorage(0),
    mInputSys(0),
    mRendererSys(0),
    mEffectSys(0),
    mTerminationRequested(false),
    mInitialized(false),
    mLastFrameTime(0)
{
}

Game::~Game(void) {
    delete mMenuState;
    delete mPlayState;

    delete mInputSys;
    delete mEffectSys;
    delete mRendererSys;

    if (mConfigStorage)
        mConfigStorage->storeConfig();

    delete mConfigStorage;
    mConfigStorage = 0;
}

void Game::init(void) {
    // already initialized. Silently exit
    if (mInitialized)
        return;

    // first step: initialize the configuration
    mConfigStorage = new ConfigStorage();

    // now initialize all the sub-systems
    mRendererSys = new RendererSystem(this);
    mInputSys = new InputSystem(this, mRendererSys);
    mEffectSys = new EffectSystem(this, mRendererSys);

    // initialize the resources
    // TODO: For now... (Add configure time variables)
    Ogre::ResourceGroupManager::getSingleton().
        addResourceLocation("./fonts.zip", "Zip", "Fonts");
    Ogre::ResourceGroupManager::getSingleton().
        addResourceLocation("./images.zip", "Zip", "General");
    Ogre::ResourceGroupManager::getSingleton().
        addResourceLocation("./materials.zip", "Zip", "General");
    Ogre::ResourceGroupManager::getSingleton().
        addResourceLocation("./meshes.zip", "Zip", "General");

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Preload the font
    Ogre::FontManager::getSingleton().getByName("MenuFont")->load();

    // Construct the states (depends on the resources, etc. last step)
    mMenuState = new MenuState(this);
    mPlayState = new PlayState(this);

    // set the default state
    setState(mMenuState);

    mInitialized = true;
}

void Game::run(void) {
    if (!mInitialized)
        REQUEST_EXCEPT("No initialization done prior to run() call!", "Game::run");

    // no state prepared and no state to be switched to?
    if (mCurrentState == 0 && ((mTargetState == 0) || !mStateChangeRequested) )
        REQUEST_EXCEPT("No state to start the game with!", "Game::run");

    // loop while there is something to be done
    while (!mTerminationRequested) {
        // if a request to change state happened, do it now
        if (handleStateChange())
            break;

        unsigned long frameStart = mOgreRoot.getTimer()->getMicroseconds();
        unsigned long deltaTime = frameStart - mLastFrameTime;
        mLastFrameTime = frameStart;

        float deltaSec = deltaTime / 1000000.0f;

        // TODO: Accumulate the deltaSec if it is too small

        // handle the frame...
        mInputSys->update(deltaSec);
        mCurrentState->update(deltaSec);
        mEffectSys->update(deltaSec);
        mRendererSys->update(deltaSec);
    }
}

GameState* Game::getState(GameStateType type) {
    switch (type) {
    case GS_MENU : return mMenuState;
    case GS_PLAY : return mPlayState;
    default: return NULL;
    }
}

bool Game::handleStateChange(void) {
    if (mStateChangeRequested) {
        mStateChangeRequested = false;

        if (mTargetState == 0) { // the same as request for termination
            if (mCurrentState)
                mCurrentState->exit();

            return true;
        }

        if (mTargetState != mCurrentState) {
            if (mCurrentState)
                mCurrentState->exit();

            mTargetState->enter();
            mCurrentState = mTargetState;

            // inputs to the focused state
            mInputSys->setInputListener(mCurrentState);
        }
    }

    return false;
}

void Game::setState(GameState* target) {
    mStateChangeRequested = true;
    mTargetState = target;
};

}; // namespace ReQuest
