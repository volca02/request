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

#ifndef __BULLETOBJECT_H
#define __BULLETOBJECT_H

#include "Prerequisites.h"
#include "ObjectTypes.h"
#include "Constants.h"

#include <OgreSceneNode.h>
#include <OgreEntity.h>

namespace ReQuest {

/// Bullet object - object representing a bullet. Dies after a certain amount of time (constant defined as BULLET_LIFETIME)
class BulletObject : public RenderedGameObject {
public:
    BulletObject(ObjectSystem* owner, int id);

    void update(float delta);

    virtual void onCollision(GameObject *b);

protected:
    float mLifeTime;
};

class BulletObjectFactory : public GameObjectFactory {
public:
    BulletObjectFactory();
    virtual GameObject* create(ObjectSystem* owner, int id);
};

}; // namespace ReQuest

#endif
