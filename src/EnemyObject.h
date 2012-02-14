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

#ifndef __ENEMYOBJECT_H
#define __ENEMYOBJECT_H

#include "Prerequisites.h"
#include "ObjectTypes.h"
#include "Constants.h"

#include <OgreSceneNode.h>
#include <OgreEntity.h>

namespace ReQuest {

/// A repetitive, random-length timer (a base for Frame independent random events)
class RandomTimer {
public:
    RandomTimer(float prob) : mProb(prob), mTime(0) { reinit(); };

    /// Ticks the timer. Returns true if the timer ran over the precomputed limit (which also reinitializes the timer)
    bool tick(float delta) {
        if (mProb <= 0)
            return false;

        mTime += delta;

        if (mTime > mMaxTime) {
            reinit();
            return true;

        }

        return false;
    };

protected:
    void reinit(void) {
        if (mProb <= 0)
            return;

        mTime = 0;
        mMaxTime = Ogre::Math::RangeRandom(0, PROBABILITY_CORRECTION / mProb);
    };

    /// Fireing probability
    float mProb, mTime, mMaxTime;
};

/// Enemy object - base object for all enemy types
class EnemyObject : public RenderedGameObject {
public:
    EnemyObject(ObjectSystem* owner, int id, const EnemyConstructionParams& ec);

    void update(float delta);

    // commented out because I hope to get this one from the mesh
    // float getRadius(void);

    virtual bool isStationary(void);

protected:
    float mLifeTime;

    const EnemyConstructionParams& mParams;

    RandomTimer mFireTimer,
        mLayMinesTimer,
        mFollowTimer,
        mChangeDirTimer,
        mChangeCurveTimer;

    Ogre::Real mCurveAngle;
};

class EnemyObjectFactory : public GameObjectFactory {
public:
    EnemyObjectFactory(const EnemyConstructionParams& cpar);

    virtual GameObject* create(ObjectSystem* owner, int id);

protected:
    const EnemyConstructionParams& mConstructionParams;
};


}; // namespace ReQuest

#endif
