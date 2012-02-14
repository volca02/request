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

#ifndef __SUBSYSTEM_H
#define __SUBSYSTEM_H

#include "Prerequisites.h"

namespace ReQuest {

/// Abstract parent of all game's subsystems
class SubSystem {
public:
    /// Constructor
    SubSystem(Game* owner) : mOwner(owner) {  };

    /// Destructor
    virtual ~SubSystem() { };

    /// per-frame handling routine. delta is the last frame's time in seconds
    virtual void update(float delta) = 0;

protected:
    Game *mOwner;
};

}; // namespace ReQuest

#endif
