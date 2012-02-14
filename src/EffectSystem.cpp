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
#include "EffectSystem.h"
#include "RendererSystem.h"

namespace ReQuest {

void Effect::update(float delta) {
    if (mFollowObject)
        setPosition(mFollowObject->getPosition());
}


EffectSystem::EffectSystem(Game* owner, RendererSystem* rendererSys) : SubSystem(owner), mRendererSys(rendererSys), mID(0) {
}

EffectSystem::~EffectSystem(void) {
    EffectList::iterator it = mActiveEffects.begin();

    while (it != mActiveEffects.end()) {
        Effect* e = *(it++);

        delete e;
    }
}

void EffectSystem::update(float delta) {
    // update all the effects
    EffectList::iterator it = mActiveEffects.begin();

    while (it != mActiveEffects.end()) {
        Effect* e = *it;

        if (e->finished()) {
            // delete the effect, remove
            it = mActiveEffects.erase(it);
            delete e;
        } else {
            e->update(delta);
            ++it;
        }
    }
}


void EffectSystem::addEffectFactory(EffectFactory* fac) {
    mEffectFactories.insert(std::make_pair(fac->getEffectType(), fac));
}

void EffectSystem::removeEffectFactory(EffectFactory* fac) {
    EffectFactoryMap::iterator it = mEffectFactories.find(fac->getEffectType());

    if (it != mEffectFactories.end()) {
        mEffectFactories.erase(it);
    }
}

Effect* EffectSystem::fireEffect(int type, const Ogre::Vector3& pos) {
    EffectFactoryMap::iterator it = mEffectFactories.find(type);

    if (it != mEffectFactories.end()) {
        Effect* ef = it->second->create(++mID, pos);

        mActiveEffects.push_back(ef);

        return ef;
    }

    return NULL;
}

Ogre::SceneManager* EffectSystem::getSceneManager(void) {
    return mRendererSys->getSceneManager();
}

}; // namespace ReQuest

