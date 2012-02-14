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

#include "Game.h"
#include "PlayState.h"

#include "ObjectSystem.h"

#include "Starfield.h"

// various object factories
#include "PlayerObject.h"
#include "BulletObject.h"
#include "EnemyObject.h"
#include "MineObject.h"

// effects
#include "GameEffects.h"

#include "RendererSystem.h"

#include "Constants.h"

#include <OgreColourValue.h>
#include <OgreVector3.h>

using namespace Ogre;

namespace ReQuest {

// ----------------------------------------------------------------------------
// ----- Play State Class -----------------------------------------------------
// ----------------------------------------------------------------------------
PlayState::PlayState(Game *owner) :
    GameState(owner),
    mLight(0),
    mTime(0),
    mMonsterEmitTime(5.0f),
    mStarfield(0),
    mZoom(0) {

    // we need the scene manager to work with objects
    mRendererSystem = mOwner->getRendererSystem();

    mObjectSys = new ObjectSystem(mOwner);

    // add factories for various creatures
    mPlayerFactory = new PlayerObjectFactory();
    mCrystalFactory = new RenderedGameObjectFactory(OT_CRYSTAL, "crystal.mesh");
    mBulletFactory = new BulletObjectFactory();
    mMineFactory = new MineObjectFactory();

    // TODO: This will be repeated times the enemy types, each one with custom parameter set. Constructor parameters needed
    mGrungerFactory = new EnemyObjectFactory(GrungerEnemy);
    mFastMoverFactory = new EnemyObjectFactory(FastMoverEnemy);

    mObjectSys->addObjectFactory(mPlayerFactory);
    mObjectSys->addObjectFactory(mCrystalFactory);
    mObjectSys->addObjectFactory(mBulletFactory);
    mObjectSys->addObjectFactory(mMineFactory);

    // enemies
    mObjectSys->addObjectFactory(mGrungerFactory);
    mObjectSys->addObjectFactory(mFastMoverFactory);

    mSceneMgr = mRendererSystem->getSceneManager();
    mCamera = mRendererSystem->getDefaultCamera();

    mLevelBounds = 0;
    mBoundsNode = 0;

    // effects
    mEffectSystem = mOwner->getEffectSystem();
    mExplosionFactory = new ExplosionEffectFactory(mEffectSystem);
    mEffectSystem->addEffectFactory(mExplosionFactory);
}

PlayState::~PlayState(void) {
    delete mObjectSys;

    mEffectSystem->removeEffectFactory(mExplosionFactory);
    delete mExplosionFactory;

    delete mPlayerFactory;
    delete mCrystalFactory;
    delete mBulletFactory;
    delete mMineFactory;

    // enemies
    delete mGrungerFactory;
    delete mFastMoverFactory;

    mActivePowerUps.clear();
}

bool PlayState::keyPressed(const OIS::KeyEvent &e) {
    if (e.key == OIS::KC_ESCAPE) // switch back to menu
        mOwner->setState(mOwner->getState(GS_MENU));
    else if (e.key == OIS::KC_B) // test bounce powerup
        activatePowerUp(mPlayer->bulletBounce(), 5.0f);
}

bool PlayState::keyReleased(const OIS::KeyEvent &e) {

}

bool PlayState::mouseMoved(const OIS::MouseEvent &e) {
    mRelative += Vector3(e.state.X.rel, -e.state.Y.rel, 0);
    mZoom += e.state.Z.rel;
}

bool PlayState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
    // shoot a bullet
    //    if (id && OIS::MB_Left)
    mPlayer->fire();
}

bool PlayState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
}

void PlayState::update(float delta) {
    mTime += delta;
    mTimeSinceLastMonster += delta;

    mStarfield->update(delta);

    mPlayer->setVelocity(mRelative * PLAYER_SPEED_COEFF);

    mObjectSys->update(delta);

    // control the camera to look at the player, if in the bounds of the playfield
    Vector3 pos = mPlayer->getPosition();

    if (pos.x < -FIELD_WIDTH2)
        pos.x = -FIELD_WIDTH2;
    if (pos.x > FIELD_WIDTH2)
        pos.x = FIELD_WIDTH2;

    if (pos.y < -FIELD_HEIGHT2)
        pos.y = -FIELD_HEIGHT2;
    if (pos.y > FIELD_HEIGHT2)
        pos.y = FIELD_HEIGHT2;

    mCamera->lookAt(pos);
    pos.z = 10 - 0.001 * mZoom;
    mCamera->setPosition(pos);

    if (mPlayer->isKilled()) {
        clear();
        mOwner->setState(mOwner->getState(GS_MENU));
    };

    if (mTimeSinceLastMonster > mMonsterEmitTime) {
        mTimeSinceLastMonster = 0;
        // TODO: Random type (level spec based)
        GameObject *o = mObjectSys->createObject(OT_GRUNGER);
        o->placeRandomly(20);
        o = mObjectSys->createObject(OT_FASTMOVER);
        o->placeRandomly(20);
    }

    processActivePowerUps(delta);
}

void PlayState::reset(void) {
    clear();

    mSceneMgr->setAmbientLight(ColourValue(0.1, 0.1, 0.1));

    mPlayer = static_cast<PlayerObject*>(mObjectSys->createObject(OT_PLAYER));

    mPlayer->setPosition(Vector3(0,0,0));
    mPlayer->setVelocity(Vector3(0,0,0));

    mLight = mSceneMgr->createLight("MainLight");
    mLight->setType(Light::LT_DIRECTIONAL);
    mLight->setPosition(-1, -1, 10);
    mLight->setDirection(-0.5, -0.5, -1);
    mLight->setDiffuseColour(0.7, 0.7, 0.7);
    mLight->setSpecularColour(0.7, 0.7, 0.7);
    mLight->setCastShadows(false);

    // TODO: This is a task for Level class...
    for (int i = 0; i < 30; ++i) {
        GameObject* o = mObjectSys->createObject(OT_CRYSTAL);
        o->placeRandomly(20);
    }

    for (int i = 0; i < 40; ++i) {
        GameObject* o = mObjectSys->createObject(OT_MINE);
        o->placeRandomly(20);
    }

    mTime = 0;
    mTimeSinceLastMonster = 0;
    mMonsterEmitTime = 5.0f;
    mRelative = Vector3::ZERO;
}

void PlayState::clear(void) {
    mObjectSys->clear();

    if (mLight) {
        mSceneMgr->destroyLight(mLight);
        mLight = 0;
    }

    mPlayer = 0;
}

void PlayState::__enterImpl(void) {
    mStarfield = new Starfield(mSceneMgr, 1000, false);

    // create the level boundaries
    mLevelBounds = mSceneMgr->createManualObject("LEVEL_BOUNDS");

    ColourValue bcol(0.8,0.5,0.5,0.5);

    mLevelBounds->begin("BaseWhiteNoLightning", RenderOperation::OT_LINE_STRIP);
    mLevelBounds->position(-FIELD_WIDTH2, -FIELD_HEIGHT2, 0);
    mLevelBounds->colour(bcol);
    mLevelBounds->position( FIELD_WIDTH2, -FIELD_HEIGHT2, 0);
    mLevelBounds->colour(bcol);
    mLevelBounds->position( FIELD_WIDTH2,  FIELD_HEIGHT2, 0);
    mLevelBounds->colour(bcol);
    mLevelBounds->position(-FIELD_WIDTH2,  FIELD_HEIGHT2, 0);
    mLevelBounds->colour(bcol);
    mLevelBounds->position(-FIELD_WIDTH2, -FIELD_HEIGHT2, 0);
    mLevelBounds->colour(bcol);
    mLevelBounds->end();

    mBoundsNode = static_cast<SceneNode*>(mSceneMgr->getRootSceneNode()->createChild("LevelBoundsNode"));
    mBoundsNode->attachObject(mLevelBounds);

    reset();
}

void PlayState::__exitImpl(void) {
    // clear all the gameplay date
    clear();

    delete mStarfield;
    mStarfield = 0;

    if (mLevelBounds) {
        mBoundsNode->detachObject(mLevelBounds);
        mSceneMgr->destroyManualObject(mLevelBounds);
        mSceneMgr->getRootSceneNode()->removeChild(mBoundsNode);
        mSceneMgr->destroySceneNode(mBoundsNode->getName());
        mLevelBounds = 0;
        mBoundsNode = 0;
    }
}

void PlayState::processActivePowerUps(float delta) {
    // update all powerups
    PowerUpSet::iterator it = mActivePowerUps.begin();

    while (it != mActivePowerUps.end()) {
        PowerUpSet::iterator cur = it++;

        if (!((*cur)->update(delta))) {
            // remove from set
            mActivePowerUps.erase(cur);
        };
    }
}

void PlayState::activatePowerUp(PowerUp* powerup, float time) {
    powerup->activate(time);
    mActivePowerUps.insert(powerup);
}

}; // namespace ReQuest

