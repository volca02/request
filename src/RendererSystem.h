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

#ifndef __RENDERERSYSTEM_H
#define __RENDERERSYSTEM_H

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreCamera.h>
#include <OgreWindowEventUtilities.h>

#include "SubSystem.h"

namespace ReQuest {

/// Rendering subsystem. Handles rendering
class RendererSystem : public SubSystem {
public:
    RendererSystem(Game* owner);
    ~RendererSystem(void);

    virtual void update(float delta);

    // retrieves the created render window
    Ogre::RenderWindow *getRenderWindow(void) { return mRenderWindow; };
    Ogre::Camera *getDefaultCamera(void) { return mDefaultCamera; };
    Ogre::SceneManager *getSceneManager(void) { return mSceneManager; };

    int getScreenHeight(void) { return mRenderWindow->getHeight(); };
    int getScreenWidth(void) { return mRenderWindow->getWidth(); };

protected:
    Ogre::Root *mRoot;
    Ogre::RenderWindow *mRenderWindow;
    Ogre::SceneManager *mSceneManager;
    Ogre::Camera *mDefaultCamera;
};

}; // namespace ReQuest

#endif
