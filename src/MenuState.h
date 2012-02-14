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

#ifndef __MENUSTATE_H
#define __MENUSTATE_H

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreCamera.h>

#include "GameState.h"
#include "MenuSystem.h"

namespace ReQuest {

/// menu game state. Manages different menus
class MenuState : public GameState {
public:
    /// Constructor
    MenuState(Game *owner);

    /// Destructor
    virtual ~MenuState(void);

    // ---- Input listener related
    bool keyPressed( const OIS::KeyEvent &e );
    bool keyReleased( const OIS::KeyEvent &e );

    bool mouseMoved( const OIS::MouseEvent &e );
    bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );

    virtual void update(float delta);

protected:
    virtual void __enterImpl(void);
    virtual void __exitImpl(void);

    // ---- Menu callbacks
    void exitRequest(void);
    void newGameRequest(void);
    void terminateRequest(void);
    void mainMenuRequest(void);

    MenuSystem *mMenuSystem;
    Ogre::SceneNode *mLogoNode;
    Ogre::Camera *mCamera;
    Ogre::SceneManager *mSceneMgr;
    Ogre::Real mTime;
    Ogre::Real mMouseX, mMouseY;
    Ogre::Vector3 mVelocity;
    Ogre::Vector3 mAcceleration;
    Ogre::Light *mLight;
    Ogre::Entity *mLogo;

    Starfield *mStarfield;
};

}; // namespace ReQuest

#endif
