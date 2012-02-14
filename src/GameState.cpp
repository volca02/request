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

#include "GameState.h"
#include "Game.h"

namespace ReQuest {

//-----------------------------------------------------------------------------
//------- Game State base code ------------------------------------------------
//-----------------------------------------------------------------------------
GameState::GameState(Game* owner) : mOwner(owner), mActive(false) {
};

GameState::~GameState(void) {
    // ensure cleanup was done
    exit();
};

void GameState::enter(void) {
    if (!mActive)
        __enterImpl();

    mActive = true;
};

void GameState::exit(void) {
    if (mActive)
        __exitImpl();

    mActive = false;
};

bool GameState::isActive(void) {
    return mActive;
};

void GameState::update(float delta) {
    // Code in ancestor class
};


void GameState::__enterImpl(void) {
    // Code in ancestor class
};

void GameState::__exitImpl(void) {
    // Code in ancestor class
};

}; // namespace ReQuest
