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

#include "Prerequisites.h"
#include "GameEffects.h"
#include "EffectSystem.h"
#include "Constants.h"

#include <OgreSceneManager.h>
#include <OgreStringConverter.h>

using namespace Ogre;

namespace ReQuest {

ExplosionEffect::ExplosionEffect(EffectSystem* owner,
                                 int id,
                                 const Ogre::Vector3& pos)
    : Effect(owner, id, EFF_EXPLOSION, pos), mTime(0)
{
    mPosition += Vector3(0,0,1);

    mLight = mOwner->getSceneManager()->createLight("EffectLight" + StringConverter::toString(id));
    mLight->setType(Light::LT_POINT);
    mLight->setDiffuseColour(1.0, 0.5, 0.25);
    mLight->setSpecularColour(1.0, 0, 0);
    mLight->setPosition(mPosition);
    mLight->setAttenuation(1.5, 0, 0.1, 0);

    // todo: particle effect
};

ExplosionEffect::~ExplosionEffect(void) {
    mOwner->getSceneManager()->destroyLight(mLight);
}

void ExplosionEffect::update(float delta) {
    Effect::update(delta);

    mLight->setPosition(mPosition);

    mTime += delta;

    // set the lights intensity to lineary decrease
    float intens = EXPLOSION_LENGTH - mTime;


    if (mTime > EXPLOSION_LENGTH)  {
        mFinished = true;
    } else {
        mLight->setDiffuseColour(intens, intens/2, intens/4);
        mLight->setSpecularColour(intens, 0, 0);
    }
};


ExplosionEffectFactory::ExplosionEffectFactory(EffectSystem *owner) : EffectFactory(owner, EFF_EXPLOSION) {
}

Effect* ExplosionEffectFactory::create(int id, const Ogre::Vector3& pos) {
    return new ExplosionEffect(mOwner, id, pos);
}

}; // namespace ReQuest
