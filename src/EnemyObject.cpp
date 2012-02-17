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
#include "EnemyObject.h"
#include "Random.h"

using namespace Ogre;

namespace ReQuest {

// ----------------------------------------------------------------------------
// ----- Enemy Object Class --------------------------------------------------
// ----------------------------------------------------------------------------
EnemyObject::EnemyObject(ObjectSystem* owner, int id, const EnemyConstructionParams& ec)
    : RenderedGameObject(owner, id, ec.type, ec.meshName),
      mParams(ec),
      mFireTimer(mParams.fireprob),
      mLayMinesTimer(mParams.fireprob),
      mFollowTimer(mParams.follow),
      mChangeDirTimer(mParams.changedir),
      mChangeCurveTimer(mParams.changecurve),
      mCurveAngle(0)
{
    setBounce(true);

    Vector3 v = randomDirection();

    if (mParams.maxspeed) {
        mVelocity = v * mParams.speed * rand1();
    } else {
        mVelocity = v * mParams.speed;
    }

    mLifeTime = 0;
};

void EnemyObject::update(float delta) {
    // let's go through all the capabilities
    if (mParams.fires && mFireTimer.tick(delta))
        // the projectile will know if it wants to be direct shot or random
        mOwner->createObject(mParams.firetype, mPosition);

    // should we lay a mine on this position?
    if (mParams.laysmines && mLayMinesTimer.tick(delta))
        mOwner->createObject(OT_MINE, mPosition);

    // move the enemy
    if (mParams.follows && mFollowTimer.tick(delta)) {
        // get the player
        GameObject* player = mOwner->getRandomObjectOfType(OT_PLAYER);

        if (player && player->isAlive()) {
            Vector3 dist = player->getPosition() - mPosition;

            mVelocity = dist.normalise() * mParams.speed;
        }
    } else if (mChangeDirTimer.tick(delta)) {
        GameObject* player = mOwner->getRandomObjectOfType(OT_PLAYER);

        Vector3 dist(0,0,0);
        bool zoom = false;
        if (player) {
            zoom = mParams.zoom;
            dist = player->getPosition() - mPosition;
        }

        if (zoom && Math::Abs(dist.length()) < ZOOM_MAX_DIST) {
            // new velocity in the vector of the player
            dist.normalise();
            mVelocity = dist * mParams.speed;
        } else if (mParams.maxspeed) {
            mVelocity = randomDirection() * mParams.speed;
        } else {
            mVelocity = (mParams.speed * rand1()) * randomDirection();
        }
    }

    if (mParams.curves) {
        if (mChangeCurveTimer.tick(delta)) {
            // new curvature
            mCurveAngle = 2 * mParams.curve * (-0.5 + rand1());
        }

        // curve the velocity
        Vector2 c(Math::Cos(mCurveAngle * delta), Math::Sin(mCurveAngle * delta));

        Vector3 ov(mVelocity);
        mVelocity.x = ov.x * c.x - ov.y * c.y;
        mVelocity.y = ov.y * c.x + ov.x * c.y;
    }

    mLifeTime += delta;
    RenderedGameObject::update(delta);
    mPosition.z = 0; // just to be sure
};

/*
  float EnemyObject::getRadius(void) {
  return mParams.radius;
  }
*/

bool EnemyObject::isStationary(void) {
    return mParams.stationary;
}

EnemyObjectFactory::EnemyObjectFactory(const EnemyConstructionParams& cpar) :
    GameObjectFactory(cpar.type),
    mConstructionParams(cpar) {
}

GameObject * EnemyObjectFactory::create(ObjectSystem* owner, int id) {
    return new EnemyObject(owner, id, mConstructionParams);
}





}; // namespace ReQuest


