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

#ifndef __OBJECTSYSTEM_H
#define __OBJECTSYSTEM_H

#include "Prerequisites.h"
#include "SubSystem.h"
#include "Constants.h"

#include <map>
#include <vector>
#include "OgreEntity.h"

namespace ReQuest {
// forward decl. to be sure
class ObjectSystem;

/// The bounds of the field (to be used with outOfBounds)
enum FieldBounds {
    BOUND_TOP = 1,
    BOUND_BOTTOM = 2,
    BOUND_LEFT = 4,
    BOUND_RIGHT = 8
};

/// A base class for game objects. Overriden to represent various object types
class GameObject {
public:
    GameObject(ObjectSystem* owner, int id, int type);

    virtual ~GameObject(void);

    virtual void update(float delta);

    int getID() const {return mID; };

    virtual void setPosition(const Ogre::Vector3& pos);

    virtual const Ogre::Vector3& getPosition(void) const { return mPosition; };

    virtual void setVelocity(const Ogre::Vector3& vel);

    virtual const Ogre::Vector3& getVelocity(void) const { return mVelocity; };

    const int getObjectType(void) const { return mObjectType; };

    /** Called when object goes out of the defined boundaries
     * @param bounds the corrected position that does not overlap the boundaries
     */
    virtual void outOfBounds(const Ogre::Vector3& bounds, unsigned int sides);

    virtual void kill(void) { mDying = true; };

    virtual bool isDying(void) const { return mDying; };

    virtual bool isDead(void) const { return mDead; };

    virtual bool isAlive(void) const { return !(mDead || mDying); };

    /// was killed, now dies
    virtual void die(void) { if (!mDying) return; mDying = false; mDead = true; };

    virtual void setBounce(bool bounce) { mBounce = bounce; mDieOnBounds = !bounce; };

    virtual float getRadius(void) { return mRadius; };

    virtual void setRadius(float rnew) { assert(rnew > 0.0f); mRadius = rnew; };

    virtual bool isStationary(void) { return false; };

    /** places the object randomly, so it does not collide with anything
     * @param tries The number of tries before the placement is given up (-1 try == forever)
     * @return true if the placement went ok, false if given up
     */
    bool placeRandomly(int tries = 20);

    /// called by object system when a collision is detected
    virtual void onCollision(GameObject *b);

protected:
    virtual Effect* fireEffect(int effectType, const Ogre::Vector3& pos) const;

    ObjectSystem *mOwner;

    bool mDead, mDying;

    bool mBounce, mDieOnBounds;

    int mID;

    const int mObjectType;

    Ogre::Vector3 mPosition;

    Ogre::Vector3 mVelocity;

    float mRadius;
};


/// Game object that is on-screen, visible
class RenderedGameObject : public GameObject {
public:
    RenderedGameObject(ObjectSystem* owner, int id, int type, const std::string meshName);

    virtual ~RenderedGameObject(void);

    virtual void setPosition(const Ogre::Vector3& pos);

    virtual float getRadius(void);

protected:
    RendererSystem *mRendererSys;

    Ogre::Entity *mEntity;

    Ogre::SceneNode *mSceneNode;
};

/// Factory abstract base. Used as a base for factories for different object types
class GameObjectFactory {
public:
    GameObjectFactory(const int type) : mType(type) {};

    virtual ~GameObjectFactory() {};

    virtual GameObject* create(ObjectSystem* owner, int id) = 0;

    int getType(void) { return mType; };

protected:
    int mType;
};

/// Factory for basic rendered objects
class RenderedGameObjectFactory : public GameObjectFactory {
public:
    RenderedGameObjectFactory(int type, const std::string meshName);

    virtual GameObject* create(ObjectSystem* owner, int id);

protected:
    int mObjectType;
    std::string mMeshName;
};

/// Class managing in-game objects
class ObjectSystem : public SubSystem {
public:
    ObjectSystem(Game* game);
    virtual ~ObjectSystem();

    /// performs per-frame update for all objects, purges the objects queues for deletion
    virtual void update(float delta);

    /// registers a new factory for object types
    void addObjectFactory(GameObjectFactory* factory);

    /// unregisters a factory for object types
    void removeObjectFactory(GameObjectFactory* factory);

    /// creates an object given it's type ID
    GameObject *createObject(int type);

    /// creates an object given it's type ID, then set's it's position to pos
    GameObject *createObject(int type, const Ogre::Vector3& pos);

    /// destroys an object (queued destroy)
    void destroyObject(GameObject *obj);

    /// clears the whole objectsystem
    void clear(void);

    /// Checks if the given object is in the bounds defined by the constants FIELD_WIDTH2 and FIELD_HEIGHT2
    /// if not, it will call outOfBounds method of the object with the corrected position as a parameter
    void checkBounds(GameObject* ob);

    /// Shortcut to the renderer system
    RendererSystem *getRendererSystem(void);

    /// Detects collision on demand for the two given objects
    bool objectsCollide(GameObject* a, GameObject* b);

    /// Detects if the given object collides with anything. colObj gets filled with the colliding object
    bool objectCollides(GameObject* a, GameObject** colObj);

    /// counts the objects of the specified type
    size_t getObjectCount(int type);

    /// @return a random object of the specified type type or 0 if the object does not exist in simulation
    GameObject* getRandomObjectOfType(int type);

    /// Fires a effect on the specified position
    Effect* fireEffect(int effectType, const Ogre::Vector3& pos);


protected:
    void detectCollisions(void);

    GameObjectFactory *getFactoryForType(int type);

    typedef std::map<int, GameObjectFactory*> ObjectFactoryMap;

    ObjectFactoryMap mObjectFactoryMap;

    typedef std::vector<GameObject*> GameObjects;

    GameObjects mGameObjects;

    int mObjectID;

    EffectSystem *mEffectSystem;
};

}; // namespace ReQuest

#endif
