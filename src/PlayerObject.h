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

#ifndef __PLAYEROBJECT_H
#define __PLAYEROBJECT_H

#include "Prerequisites.h"
#include "ObjectTypes.h"
#include "Constants.h"

#include <OgreSceneNode.h>
#include <OgreEntity.h>

namespace ReQuest {

/// Powerup holder class
class PowerUp {
public:
    PowerUp() : mRemainingTime(0) {};

    /// updates the powerup. returns true if the powerup stayed active
    bool update(float delta) {
        mRemainingTime -= delta;
        if (mRemainingTime < 0) {
            return false;
        };
        return true;
    };

    /// activates the powerup
    void activate(float time) { mRemainingTime = time; };

    /// gets the remaining time
    float getRemainingTime(void) { return mRemainingTime; };

    /// gets if the powerup is active or not
    bool isActive(void) { return mRemainingTime > 0; };

protected:
    float mRemainingTime;
};

/// Player object - object representing a player. Supports movement injections
class PlayerObject : public RenderedGameObject {
public:
    PlayerObject(ObjectSystem* owner, int id);

    void update(float delta);

    virtual void outOfBounds(const Ogre::Vector3& bounds, unsigned int sides);

    /// overriden kill to avoid player object destruction
    virtual void kill(void);

    virtual bool isKilled(void) { return mKilled; };

    virtual void onCollision(GameObject *b);

    virtual bool isAlive(void) const;

    void fire();

    PowerUp *bulletBounce() { return &mBulletBounce; };

protected:
    bool mKilled;
    int mCollectedCrystals;

    PowerUp mBulletBounce;    //!< Bullet Bounce powerupw
};

class PlayerObjectFactory : public GameObjectFactory {
public:
    PlayerObjectFactory();
    virtual GameObject* create(ObjectSystem* owner, int id);
};


}; // namespace ReQuest

#endif
