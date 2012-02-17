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

#include "InputSystem.h"
#include "RendererSystem.h"

#include <cassert>

namespace ReQuest {

// ----------------------------------------------------------------------------
// ----- Input System Class ---------------------------------------------------
// ----------------------------------------------------------------------------
InputSystem::InputSystem(Game* owner, RendererSystem* rs)
    : SubSystem(owner),
      mRendererSystem(rs),
      mInputManager(0),
      mMouse(0),
      mKeyboard(0),
      mListener(0)
{
    // We count with the fact that the renderer system is created
    assert(rs);

    // construct the needed OIS objects
    OIS::ParamList paramList;

    size_t windowHnd = 0;

    mRendererSystem->getRenderWindow()->getCustomAttribute("WINDOW", &windowHnd);

    std::ostringstream windowHndStr;

    windowHndStr << (unsigned int) windowHnd;
    paramList.insert( std::make_pair( std::string( "WINDOW" ), windowHndStr.str() ) );

    /*
    // Debug nonexclusive mode
    #if defined OIS_WIN32_PLATFORM
    paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
    paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
    paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
    paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
    #elif defined OIS_LINUX_PLATFORM
    paramList.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
    paramList.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
    paramList.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
    paramList.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
    #endif
    */

    mInputManager = OIS::InputManager::createInputSystem( paramList );

    // Partially taken from Ogre wiki article about OIS
    // create inputs
    if (mInputManager->getNumberOfDevices(OIS::OISKeyboard) > 0) {
        mKeyboard = static_cast<OIS::Keyboard*>( mInputManager->createInputObject( OIS::OISKeyboard, true ) );
        mKeyboard->setEventCallback( this );
    }

    if (mInputManager->getNumberOfDevices(OIS::OISMouse) > 0) {

        mMouse = static_cast<OIS::Mouse*>( mInputManager->createInputObject( OIS::OISMouse, true ) );
        mMouse->setEventCallback( this );

        // Get window size
        unsigned int width, height, depth;
        int left, top;

        mRendererSystem->getRenderWindow()->getMetrics( width, height, depth, left, top );

        // Set mouse region
        const OIS::MouseState &mouseState = mMouse->getMouseState();
        mouseState.width  = width;
        mouseState.height = height;
    }
};

InputSystem::~InputSystem(void) {
    if (mMouse)
        mInputManager->destroyInputObject(mMouse);

    if (mKeyboard)
        mInputManager->destroyInputObject(mKeyboard);

    // TODO: Deinit ois.
    if (mInputManager)
        mInputManager->destroyInputSystem( mInputManager );
}

void InputSystem::update(float delta) {
    // handle inputs
    if( mMouse ) {
        mMouse->capture();
    }

    if( mKeyboard ) {
        mKeyboard->capture();
    }
}

void InputSystem::setInputListener(InputListener* list) {
    mListener = list;
}

bool InputSystem::keyPressed( const OIS::KeyEvent &e ) {
    if (!mListener)
        return false;

    return mListener->keyPressed(e);
}

bool InputSystem::keyReleased( const OIS::KeyEvent &e ) {
    if (!mListener)
        return false;


    return mListener->keyReleased(e);
}

bool InputSystem::mouseMoved( const OIS::MouseEvent &e ) {
    if (!mListener)
        return false;

    return mListener->mouseMoved(e);
}

bool InputSystem::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
    if (!mListener)
        return false;

    return mListener->mousePressed(e, id);
}

bool InputSystem::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
    if (!mListener)
        return false;

    return mListener->mouseReleased(e, id);
}

}; // namespace ReQuest
