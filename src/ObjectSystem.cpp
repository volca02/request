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
#include "Exception.h"
#include "Game.h"
#include "RendererSystem.h"
#include "EffectSystem.h"

#include <OgreStringConverter.h>

#include <cstdlib>


// Thanks Doli@abclinuxu for this snippet
int genrand(int min, int max)
{
    return min+(rand()%(++max-min));
}



using namespace Ogre;

namespace ReQuest {


// ----------------------------------------------------------------------------
// ----- Object System Class --------------------------------------------------
// ----------------------------------------------------------------------------
ObjectSystem::ObjectSystem(Game* game) : SubSystem(game), mObjectID(1) {
    mEffectSystem = mOwner->getEffectSystem();
}

ObjectSystem::~ObjectSystem() {
    // It is not our duty to erase all the factories
    mObjectFactoryMap.clear();
}

void ObjectSystem::addObjectFactory(GameObjectFactory* factory) {
    mObjectFactoryMap[factory->getType()] = factory;
}


void ObjectSystem::removeObjectFactory(GameObjectFactory* factory) {
    ObjectFactoryMap::iterator it = mObjectFactoryMap.begin();

    while (it != mObjectFactoryMap.end()) {
        ObjectFactoryMap::iterator current = it++;

        if (current->second == factory)
            mObjectFactoryMap.erase(current);
    }
}

GameObjectFactory *ObjectSystem::getFactoryForType(int type) {
    ObjectFactoryMap::iterator it = mObjectFactoryMap.find(type);

    if (it != mObjectFactoryMap.end())
        return it->second;

    REQUEST_EXCEPT("Invalid factory type requested", "ObjectSystem::getFactoryForType");
}

void ObjectSystem::update(float delta) {
    GameObjects::iterator it = mGameObjects.begin();

    while (it != mGameObjects.end()) {
        GameObject* ob = *it;

        if (ob->isDying())
            ob->die();

        if (ob->isDead()) {
            // erase, remove
            it = mGameObjects.erase(it);
            delete ob;
        } else {
            ob->update(delta);
            checkBounds(ob);
            ++it;
        }
    }

    detectCollisions();
}

void ObjectSystem::checkBounds(GameObject* ob) {
    Vector3 pos = ob->getPosition();
    Real rad = ob->getRadius();

    unsigned int bnds = 0;
    // look if we're in the bounds
    bool mOut = false;

    if (pos.x - rad < -FIELD_WIDTH2) {
        mOut = true;
        bnds |= BOUND_LEFT;
        pos.x = - FIELD_WIDTH2 + rad;
    }

    if (pos.x + rad > FIELD_WIDTH2) {
        mOut = true;
        bnds |= BOUND_RIGHT;
        pos.x = FIELD_WIDTH2 - rad;
    }

    if (pos.y - rad < -FIELD_HEIGHT2) {
        mOut = true;
        bnds |= BOUND_BOTTOM;
        pos.y = -FIELD_HEIGHT2 + rad;
    }

    if (pos.y + rad > FIELD_HEIGHT2) {
        mOut = true;
        bnds |= BOUND_TOP;
        pos.y = FIELD_HEIGHT2 - rad;
    }

    if (mOut)
        ob->outOfBounds(pos, bnds);
}

GameObject *ObjectSystem::createObject(int type) {
    GameObject *o = getFactoryForType(type)->create(this, mObjectID++);

    mGameObjects.push_back(o);

    return o;
}

GameObject *ObjectSystem::createObject(int type, const Vector3& pos) {
    GameObject* o = createObject(type);

    o->setPosition(pos);

    return o;
}

void ObjectSystem::destroyObject(GameObject *obj) {
    obj->kill();
}

void ObjectSystem::clear(void) {
    GameObjects::iterator it = mGameObjects.begin();

    while (it != mGameObjects.end()) {
        delete *(it++);
    }

    mGameObjects.clear();
}

RendererSystem *ObjectSystem::getRendererSystem(void) {
    return mOwner->getRendererSystem();
}

bool ObjectSystem::objectsCollide(GameObject* a, GameObject* b) {
    // Naive collision impl.

    Vector3 dist = b->getPosition() - a->getPosition();

    return ((dist.length() * COLLISION_DIST_COEFF) < (b->getRadius() + a->getRadius()));
}

bool ObjectSystem::objectCollides(GameObject* a, GameObject** colObj) {
    *colObj = 0;

    GameObjects::iterator it = mGameObjects.begin();

    for (; it != mGameObjects.end(); ++it) {
        if (*it == a)
            continue;

        if (objectsCollide(a, *it)) {
            *colObj = *it;
            return true;
        }
    }

    return false;
}

size_t ObjectSystem::getObjectCount(int type) {
    size_t count = 0;

    GameObjects::iterator it = mGameObjects.begin();

    for (; it != mGameObjects.end(); ++it) {
        if ((*it)->getObjectType() == type)
            count++;
    }

    return count;
}

GameObject* ObjectSystem::getRandomObjectOfType(int type) {
    // not entirelly an optimal implementation, but well...
    size_t maxcnt = getObjectCount(type);

    size_t count = 0;
    size_t target = genrand(0, maxcnt);

    GameObjects::iterator it = mGameObjects.begin();

    for (; it != mGameObjects.end(); ++it) {
        if ((*it)->getObjectType() == type) {
            if (count == target)
                return *it;

            count++;
        }

    }

    return 0;
}

void ObjectSystem::detectCollisions(void) {
    // try to detect on all objects
    GameObjects::iterator it1 = mGameObjects.begin();

    for (; it1 != mGameObjects.end(); ++it1) {
        GameObjects::iterator it2 = it1 + 1;

        for (; it2 != mGameObjects.end(); ++it2) {
            if (objectsCollide(*it1, *it2)) {
                (*it1)->onCollision(*it2);
                (*it2)->onCollision(*it1);
            }
        }
    }
}

Effect* ObjectSystem::fireEffect(int effectType, const Ogre::Vector3& pos) {
    return mEffectSystem->fireEffect(effectType, pos);
}

// ----------------------------------------------------------------------------
// ----- Game Object ----------------------------------------------------------
// ----------------------------------------------------------------------------
GameObject::GameObject(ObjectSystem* owner, int id, int type) :
    mOwner(owner),
    mID(id),
    mObjectType(type),
    mPosition(0,0,0),
    mVelocity(0,0,0),
    mDead(false),
    mDying(false),
    mDieOnBounds(true),
    mBounce(false),
    mRadius(0.1f) {

};

GameObject::~GameObject(void) {
}

void GameObject::update(float delta) {
    if (!isStationary())
        setPosition(mPosition + mVelocity * delta);
}

void GameObject::setPosition(const Ogre::Vector3& pos) {
    mPosition = pos;
}

void GameObject::setVelocity(const Ogre::Vector3& vel) {
    mVelocity = vel;
}

void GameObject::outOfBounds(const Ogre::Vector3& bounds, unsigned int sides) {
    if (mBounce) {
        if (sides & (BOUND_LEFT | BOUND_RIGHT))
            mVelocity.x = -mVelocity.x;

        if (sides & (BOUND_TOP | BOUND_BOTTOM))
            mVelocity.y = -mVelocity.y;
    }

    mPosition = bounds;


    if (mDieOnBounds)
        kill();
};

bool GameObject::placeRandomly(int tries) {
    bool forever = (tries == -1);

    while((tries--) || forever) {
        Vector3 placement = Vector3(
                                Math::RangeRandom(-FIELD_WIDTH2, FIELD_WIDTH2),
                                Math::RangeRandom(-FIELD_HEIGHT2, FIELD_HEIGHT2),
                                0
        );

        setPosition(placement);

        GameObject* other;

        if (!mOwner->objectCollides(this, &other))
            return true;
    }

    return false;
}

void GameObject::onCollision(GameObject *b) {
    // nothing here!
}

Effect* GameObject::fireEffect(int effectType, const Ogre::Vector3& pos) const {
    // use owner to do this
    return mOwner->fireEffect(effectType, pos);
}

// ----------------------------------------------------------------------------
// ----- Rendered Game Object -------------------------------------------------
// ----------------------------------------------------------------------------
RenderedGameObject::RenderedGameObject(ObjectSystem* owner,
                                       int id, int type,
                                       const std::string meshName)
    : GameObject(owner, id, type),
      mSceneNode(0),
      mEntity(0)
{
    RendererSystem *rendererSys = mOwner->getRendererSystem();

    SceneNode* root = rendererSys->getSceneManager()->getRootSceneNode();

    std::string name = "ObjectNode" + StringConverter::toString(mID);

    mEntity = rendererSys->getSceneManager()->createEntity(name, meshName);
    mSceneNode = static_cast<SceneNode*>(root->createChild(name));

    mSceneNode->attachObject(mEntity);

    mSceneNode->setPosition(mPosition);

    mSceneNode->setVisible(true);
}

RenderedGameObject::~RenderedGameObject(void) {
    RendererSystem *rendererSys = mOwner->getRendererSystem();

    Ogre::SceneNode* root = rendererSys->getSceneManager()->getRootSceneNode();

    if (mSceneNode)
        root->removeChild(mSceneNode);

    if (mEntity)
        rendererSys->getSceneManager()->destroySceneNode(mSceneNode->getName());

    if (mSceneNode)
        rendererSys->getSceneManager()->destroyEntity(mEntity);
}

void RenderedGameObject::setPosition(const Ogre::Vector3& pos) {
    GameObject::setPosition(pos);

    mSceneNode->setPosition(mPosition);
}

float RenderedGameObject::getRadius(void) {
    return mEntity->getBoundingRadius();
}

// ----------------------------------------------------------------------------
// ----- Rendered Game Object -------------------------------------------------
// ----------------------------------------------------------------------------
RenderedGameObjectFactory::RenderedGameObjectFactory(int type, const std::string meshName) : GameObjectFactory(type), mObjectType(type), mMeshName(meshName) {

}

GameObject* RenderedGameObjectFactory::create(ObjectSystem* owner, int id) {
    return new RenderedGameObject(owner, id, mObjectType, mMeshName);
}

}; // namespace ReQuest


