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

#ifndef __INPUTSYSTEM_H
#define __INPUTSYSTEM_H

#include "Prerequisites.h"
#include "SubSystem.h"

#include <OISMouse.h>
#include <OISKeyboard.h>
#include <OISJoyStick.h>
#include <OISInputManager.h>


namespace ReQuest {

/// Input listener abstract class
class InputListener : public OIS::MouseListener, public OIS::KeyListener {
public:
    bool keyPressed( const OIS::KeyEvent &e ) = 0;
    bool keyReleased( const OIS::KeyEvent &e ) = 0;

    bool mouseMoved( const OIS::MouseEvent &e ) = 0;
    bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id ) = 0;
    bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ) = 0;
};

/// Input subsystem. Handles input
class InputSystem : public SubSystem, public OIS::MouseListener, public OIS::KeyListener {
public:
    InputSystem(Game* owner, RendererSystem* rs);
    ~InputSystem(void);

    virtual void update(float delta);

    void setInputListener(InputListener* list);

protected:
    // --- OIS Listener handlers
    bool keyPressed( const OIS::KeyEvent &e );
    bool keyReleased( const OIS::KeyEvent &e );

    bool mouseMoved( const OIS::MouseEvent &e );
    bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );

    // --- Our member vars.
    RendererSystem *mRendererSystem;

    OIS::InputManager *mInputManager;

    OIS::Mouse        *mMouse;
    OIS::Keyboard     *mKeyboard;

    InputListener     *mListener;
};

}; // namespace ReQuest

#endif
