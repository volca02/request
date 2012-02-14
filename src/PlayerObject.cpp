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
#include "PlayerObject.h"

using namespace Ogre;

namespace ReQuest {

// ----------------------------------------------------------------------------
// ----- Player Object Class --------------------------------------------------
// ----------------------------------------------------------------------------
PlayerObject::PlayerObject(ObjectSystem* owner, int id)
    : RenderedGameObject(owner, id, OT_PLAYER, PLAYER_MESH_NAME),
      mKilled(false),
      mCollectedCrystals(0),
      mBulletBounce()
{}

void PlayerObject::update(float delta) {
    // update the SN to face the velocity if nonzero
    Vector3 lpos = mPosition + mVelocity * delta;
    mSceneNode->lookAt(lpos, Node::TS_WORLD, Vector3(0,1,0));

    RenderedGameObject::update(delta);
};

void PlayerObject::outOfBounds(const Ogre::Vector3& bounds, unsigned int sides) {
    // mVelocity = Vector3::ZERO;
    mPosition = bounds;
    // TODO: Crash
}

void PlayerObject::kill(void) {
    mKilled = true;
}

bool PlayerObject::isAlive(void) const {
    return !mKilled;
}

void PlayerObject::onCollision(GameObject *b) {
    // don't collide with bullet at all
    if (b->getObjectType() == OT_BULLET)
        return;

    if (b->getObjectType() == OT_CRYSTAL) {
        // crystal found
        if (!b->isDead())
            mCollectedCrystals++;

        fireEffect(EFF_EXPLOSION, getPosition());
        b->kill();

        return;
    }

    if (b->getObjectType() >= OT_ENEMY) {
        // kill enemy, and us
        b->kill();
        kill();
    }
}

void PlayerObject::fire() {
    if (mOwner->getObjectCount(OT_BULLET) < MAX_BULLET_COUNT) {
        GameObject* bull = mOwner->createObject(OT_BULLET);
        bull->setPosition(getPosition() +
                          getRadius() * getVelocity().normalisedCopy());
        bull->setVelocity(1.2 * getVelocity());
        bull->setBounce(mBulletBounce.isActive());
    }
}

PlayerObjectFactory::PlayerObjectFactory() : GameObjectFactory(OT_PLAYER) {
}

GameObject * PlayerObjectFactory::create(ObjectSystem* owner, int id) {
    return new PlayerObject(owner, id);
}

}; // namespace ReQuest


