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

#ifndef __EFFECTSYSTEM_H
#define __EFFECTSYSTEM_H

#include "Prerequisites.h"
#include "SubSystem.h"

#include <OgreVector3.h>

namespace ReQuest {
// forward decl of effect system
class EffectSystem;

/// Effect class definition
class Effect {
public:
    Effect(EffectSystem* owner, int id, int type, const Ogre::Vector3& pos) :
        mOwner(owner), mPosition(pos),
        mID(id), mType(type),
        mFinished(false), mFollowObject(0)
    {}

    virtual ~Effect(void) {}

    virtual void update(float delta);

    virtual void follow(GameObject* obj) { mFollowObject = obj; };

    /// @return true if the effect finished and should be disposed
    virtual bool finished(void) { return mFinished; };

    /// To be used by caller to update position if the effect should move in some way
    virtual void setPosition(const Ogre::Vector3& pos) { mPosition = pos; };

protected:
    EffectSystem *mOwner;
    Ogre::Vector3 mPosition;
    int mID;
    int mType;
    bool mFinished;
    GameObject *mFollowObject;
};

/// Abstract parent for affect factories
class EffectFactory {
public:
    EffectFactory(EffectSystem *owner, int effType) :
        mOwner(owner),
        mEffectType(effType)
    { }

    virtual Effect* create(int id, const Ogre::Vector3& pos) = 0;
    virtual int getEffectType(void) { return mEffectType; };

protected:
    EffectSystem *mOwner;
    int mEffectType;
};

/// Effect subsystem. Handles various effects. Effect is a on-event, short time-span generated object that has a type and a coordinates to exist at.
class EffectSystem : public SubSystem { // TODO: public GameObjectEventListener
public:
    EffectSystem(Game* owner, RendererSystem* rendererSys);
    ~EffectSystem(void);

    virtual void update(float delta);

    void addEffectFactory(EffectFactory* fac);
    void removeEffectFactory(EffectFactory* fac);

    Effect* fireEffect(int type, const Ogre::Vector3& pos);

    Ogre::SceneManager* getSceneManager(void);
protected:
    typedef std::map<int, EffectFactory*> EffectFactoryMap;
    typedef std::list<Effect*> EffectList;

    EffectFactoryMap mEffectFactories;
    EffectList mActiveEffects;

    RendererSystem* mRendererSys;
    int mID;
};

}; // namespace ReQuest

#endif
