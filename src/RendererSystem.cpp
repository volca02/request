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

#include "RendererSystem.h"
#include "Exception.h"

using namespace Ogre;


namespace ReQuest {

// ----------------------------------------------------------------------------
// ----- Renderer System Class ------------------------------------------------
// ----------------------------------------------------------------------------
RendererSystem::RendererSystem(Game* owner)
    : SubSystem(owner),
      mRenderWindow(0),
      mSceneManager(0),
      mDefaultCamera(0)
{
    mRoot = Ogre::Root::getSingletonPtr();

    // Temporary restoration code. Use the config storage for this
    if( !mRoot->restoreConfig() ) {
        // If there is no config file, show the configuration dialog
        if( !mRoot->showConfigDialog() ) {
            REQUEST_EXCEPT("Graphic configuration was canceled", "RenderSystem::RenderSystem");
        }
    }

    mRenderWindow = mRoot->initialise( true, "ReQuest" );

    mSceneManager = mRoot->createSceneManager(ST_GENERIC, "SceneManager");

    mDefaultCamera = mSceneManager->createCamera("DefaultCamera");

    mRenderWindow->addViewport(mDefaultCamera);
};

RendererSystem::~RendererSystem(void) {
    // TODO: deinitialize ogre
    if (mRenderWindow)
        mRenderWindow->removeAllViewports();

    if (mDefaultCamera)
        mSceneManager->destroyAllCameras();

    if (mSceneManager)
        mRoot->destroySceneManager(mSceneManager);
};

void RendererSystem::update(float delta) {
    mRoot->renderOneFrame();
    Ogre::WindowEventUtilities::messagePump();
};

}; // namespace ReQuest
