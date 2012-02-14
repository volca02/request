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

#ifndef __MINEOBJECT_H
#define __MINEOBJECT_H

#include "Prerequisites.h"
#include "ObjectTypes.h"
#include "Constants.h"

#include <OgreSceneNode.h>
#include <OgreEntity.h>

namespace ReQuest {

/// Mine object - object representing a mine player collides with. Dies after a collision with player
class MineObject : public RenderedGameObject {
public:
    MineObject(ObjectSystem* owner, int id);

    virtual void onCollision(GameObject *b);
};

class MineObjectFactory : public GameObjectFactory {
public:
    MineObjectFactory();
    virtual GameObject* create(ObjectSystem* owner, int id);
};


}; // namespace ReQuest

#endif
