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

#include "ObjectSystem.h"
#include "MineObject.h"

using namespace Ogre;

namespace ReQuest {

// ----------------------------------------------------------------------------
// ----- Mine Object Class --------------------------------------------------
// ----------------------------------------------------------------------------
MineObject::MineObject(ObjectSystem* owner, int id) : RenderedGameObject(owner, id, OT_MINE, MINE_MESH_NAME) {
};

void MineObject::onCollision(GameObject *b) {
    // Only collide with player
    if (b->getObjectType() == OT_PLAYER)  {
        fireEffect(EFF_EXPLOSION, getPosition());
        b->kill();
        kill();
    }
}


MineObjectFactory::MineObjectFactory() : GameObjectFactory(OT_MINE) {
}

GameObject * MineObjectFactory::create(ObjectSystem* owner, int id) {
    return new MineObject(owner, id);
}





}; // namespace ReQuest


