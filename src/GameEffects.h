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

#ifndef __GAMEEFFECTS_H
#define __GAMEEFFECTS_H

#include "Prerequisites.h"
#include "ObjectTypes.h"
#include "EffectSystem.h"

namespace ReQuest {

class ExplosionEffect : public Effect {
public:
    ExplosionEffect(EffectSystem* owner, int id, const Ogre::Vector3& pos);
    virtual ~ExplosionEffect(void);

    virtual void update(float delta);

protected:
    float mTime;
    Ogre::Light* mLight;
};

class ExplosionEffectFactory : public EffectFactory {
public:
    ExplosionEffectFactory(EffectSystem *owner);
    virtual Effect* create(int id, const Ogre::Vector3& pos);
};
}; // namespace ReQuest

#endif
