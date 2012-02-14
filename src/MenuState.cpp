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

#include "MenuState.h"
#include "Game.h"
#include "PlayState.h"
#include "MenuSystem.h"
#include "RendererSystem.h"
#include "Starfield.h"

#include <OgreSceneNode.h>
#include <OgreMovableObject.h>
#include <OgreEntity.h>
#include <OgreLight.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreVector3.h>

using namespace Ogre;
using namespace std::tr1;

namespace ReQuest {

// ----------------------------------------------------------------------------
// ----- Menu State Class -----------------------------------------------------
// ----------------------------------------------------------------------------
MenuState::MenuState(Game *owner) :
    GameState(owner), mMenuSystem(0), mTime(0),
    mVelocity(), mAcceleration(), mStarfield(0)
{
    mMenuSystem = new MenuSystem(owner, "MenuMaterial", mOwner->getRendererSystem());

    int x = mOwner->getRendererSystem()->getScreenWidth();
    int y = mOwner->getRendererSystem()->getScreenHeight();

    // fill in the various menus.
    MenuSheet *main = mMenuSystem->createSheet("MainMenu", "MenuFont");

    main->setRect(x/2 - 100, y/3 + 20, 200, 360);

    main->setFontSize(30);

    main->addItem("Start Game", bind(&MenuState::newGameRequest, this));
    main->addItem("Help");
    main->addItem("Hall of Fame");
    main->addItem("Options");
    main->addItem("Demo");
    main->addItem("Exit", bind(&MenuState::exitRequest, this));

    MenuSheet *em = mMenuSystem->createSheet("ExitMenu", "MenuFont");

    em->setRect(x/2 - 100, y/2 - 100, 200, 150);

    em->addItem("Are you sure you want to exit?");
    em->addItem("Yes", bind(&MenuState::terminateRequest, this));
    em->addItem("No", bind(&MenuState::mainMenuRequest, this));

    mMenuSystem->setActiveSheet(main);
}

MenuState::~MenuState(void) {
    delete mMenuSystem;
}

bool MenuState::keyPressed(const OIS::KeyEvent &e) {
    if (e.key == OIS::KC_ESCAPE)
        mOwner->requestTermination();
}

bool MenuState::keyReleased(const OIS::KeyEvent &e) {

}

bool MenuState::mouseMoved(const OIS::MouseEvent &e) {
    mMenuSystem->injectMouseMoved(e.state.X.abs, e.state.Y.abs);
    mMouseX = e.state.X.abs;
    mMouseY = e.state.Y.abs;
}

bool MenuState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
    mMenuSystem->injectMouseClick(e.state.X.abs, e.state.Y.abs);
}

bool MenuState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
    // TODO: mMenuSystem->injectMouseReleased(e, id);
}

void MenuState::update(float delta) {
    mMenuSystem->update(delta);

    mStarfield->update(delta);

    mTime += delta;
    /*
      Real pos = -5 + 3 * mTime;

      // we place it up above the menu

      if (pos<0)
      mLogoNode->setPosition(0, 1.1, pos);
      else
      mLogoNode->setPosition(0, 1.1, 0);
    */
}


void MenuState::__enterImpl(void) {
    mMenuSystem->show();

    mSceneMgr = mOwner->getRendererSystem()->getSceneManager();

    mSceneMgr->setAmbientLight(ColourValue(0, 0, 0));

    mLogoNode = mSceneMgr->createSceneNode("LogoNode");
    //mLogoNode->setPosition(Ogre::Vector3(0,0,-100));
    mLogoNode->setPosition(Ogre::Vector3(0,1.1,0));

    mLogo = mSceneMgr->createEntity("Logo", "logo.mesh");

    mLogoNode->attachObject(mLogo);

    mSceneMgr->getRootSceneNode()->addChild(mLogoNode);

    mLogoNode->pitch(Radian(-0.3), Node::TS_LOCAL);

    mLight = mSceneMgr->createLight("OmniDefault");

    mLight->setType(Ogre::Light::LT_POINT);
    mLight->setPosition(-1, -1, 10);
    mLight->setAttenuation(10, 0, 0.1, 0);
    mLight->setDiffuseColour(0.7, 0.7, 0.7);
    mLight->setCastShadows(true);

    mCamera = mOwner->getRendererSystem()->getDefaultCamera();

    mCamera->setNearClipDistance(0.01);
    mCamera->setPosition(0,0,5);
    mCamera->lookAt(0,0,0);

    mStarfield = new Starfield(mSceneMgr, 10000, true);
}

void MenuState::__exitImpl(void) {
    mSceneMgr->destroySceneNode(mLogoNode->getName());
    mSceneMgr->destroyLight(mLight);

    mSceneMgr->destroyEntity(mLogo);

    mMenuSystem->hide();

    delete mStarfield;
    mStarfield = 0;
}

void MenuState::newGameRequest(void) {
    PlayState *playState = static_cast<PlayState*>(mOwner->getState(GS_PLAY));

    // so we'll get a newgame
    playState->reset();

    // and set the state
    mOwner->setState(playState);
}

void MenuState::exitRequest(void) {
    mMenuSystem->setActiveSheet("ExitMenu");
}

void MenuState::terminateRequest(void) {
    mOwner->requestTermination();
};

void MenuState::mainMenuRequest(void) {
    mMenuSystem->setActiveSheet("MainMenu");
}


}; // namespace ReQuest
