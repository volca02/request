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

#ifndef __GAME_H
#define __GAME_H

#include "OgreRoot.h"
#include "Prerequisites.h"
#include "InputSystem.h"
#include "GameState.h"

namespace ReQuest {


/// The root class. Manages the whole application's main flow
class Game {
public:
    /// Constructor. Initializes all the subsystems
    Game(void);

    /// Destructor. Deletes all the subsystems
    ~Game(void);

    /// initializes the game object
    void init(void);

    /// Main loop invoker
    void run(void);

    /// getter for config storage
    inline ConfigStorage* getConfigStorage(void) { return mConfigStorage; };

    /// invokes a request for termination
    inline void requestTermination(void) { mTerminationRequested = true; };

    void setState(GameState* state);

    GameState *getState(GameStateType type);

    RendererSystem *getRendererSystem(void) { return mRendererSys; };
    EffectSystem *getEffectSystem(void) { return mEffectSys; };

protected:
    // --- member vars
    Ogre::Root mOgreRoot;

    // --- state mgmt
    GameState *mPlayState;
    GameState *mMenuState;

    GameState *mCurrentState;
    GameState *mTargetState;
    bool mStateChangeRequested;

    /// handles the state change request. Returns true if there is no state left
    bool handleStateChange(void);


    ConfigStorage* mConfigStorage;

    InputSystem *mInputSys;
    RendererSystem *mRendererSys;
    EffectSystem *mEffectSys;
    // ObjectSystem* mObjectSys;
    // MenuSystem* mMenuSys;

    /// Will get true upon request termination
    bool mTerminationRequested;

    /// set to true upon succesful initialization
    bool mInitialized;

    /// Time of the last frame
    unsigned long mLastFrameTime;
};

};

#endif
