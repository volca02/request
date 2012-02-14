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

#ifndef __GAMESTATE_H
#define __GAMESTATE_H

#include "Prerequisites.h"
#include "InputSystem.h"

namespace ReQuest {
/// game state type definition
typedef enum { GS_MENU, GS_PLAY } GameStateType;

/// Class representing a game state (menu, intro, gameplay)
class GameState : public InputListener {
public:
    /// constructor
    GameState(Game *owner);

    /// destructor. Calls exit to ensure cleanup was done
    virtual ~GameState(void);

    /// Called upon state entrance
    virtual void enter(void);

    /// called upon state exit
    virtual void exit(void);

    /// returns true if this state is active
    virtual bool isActive(void);

    /// handles frame updates
    virtual void update(float delta);

protected:
    /// the real implementation of the state entrance. Guaranteed to be called only once per state change
    virtual void __enterImpl(void);

    /// the real implementation of the state exit. Guaranteed to be called only once per state change
    virtual void __exitImpl(void);

    /// owner of this state
    Game *mOwner;

    /// activity flag
    bool mActive;
};

}; // namespace ReQuest

#endif
