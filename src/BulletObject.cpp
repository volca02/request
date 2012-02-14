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
#include "BulletObject.h"

using namespace Ogre;

namespace ReQuest {

// ----------------------------------------------------------------------------
// ----- Bullet Object Class --------------------------------------------------
// ----------------------------------------------------------------------------
BulletObject::BulletObject(ObjectSystem* owner, int id)
    : RenderedGameObject(owner, id, OT_BULLET, BULLET_MESH_NAME),
      mLifeTime(0)
{
}

void BulletObject::update(float delta) {
    mLifeTime += delta;

    if (mLifeTime >= BULLET_LIFETIME)
        kill();

    RenderedGameObject::update(delta);
}

void BulletObject::onCollision(GameObject *b) {
    // don't collide with any bullet at all
    if (b->getObjectType() == OT_BULLET)
        return;

    if (b->getObjectType() >= OT_ENEMY)  {
        // kill enemy, and us
        fireEffect(EFF_EXPLOSION, mPosition);
        b->kill();
        kill();
    }
}

BulletObjectFactory::BulletObjectFactory() : GameObjectFactory(OT_BULLET) {
}

GameObject * BulletObjectFactory::create(ObjectSystem* owner, int id) {
    return new BulletObject(owner, id);
}

}; // namespace ReQuest


