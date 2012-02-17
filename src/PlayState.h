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

#ifndef __PLAYSTATE_H
#define __PLAYSTATE_H

#include "Prerequisites.h"
#include "GameState.h"

namespace ReQuest {

/// Play game state. The most entertaining state
class PlayState : public GameState {
public:
    /// Constructor
    PlayState(Game *owner);

    /// Destructor
    virtual ~PlayState(void);

    /// resets the game to the new gameplay prepared state
    void reset(void);

    // ---- Input listener related
    bool keyPressed( const OIS::KeyEvent &e );
    bool keyReleased( const OIS::KeyEvent &e );

    bool mouseMoved( const OIS::MouseEvent &e );
    bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );

    virtual void update(float delta);

protected:
    /// takes care of all the previously created objects, freeing them
    void clear(void);

    virtual void __enterImpl(void);
    virtual void __exitImpl(void);

    void processActivePowerUps(float delta);

    void activatePowerUp(PowerUp* powerup, float time);

    typedef std::set<PowerUp*> PowerUpSet;

    PowerUpSet mActivePowerUps;

    RendererSystem *mRendererSystem;
    ObjectSystem *mObjectSys;

    PlayerObjectFactory *mPlayerFactory;
    RenderedGameObjectFactory *mCrystalFactory;

    BulletObjectFactory *mBulletFactory;
    MineObjectFactory *mMineFactory;

    EnemyObjectFactory *mGrungerFactory;
    EnemyObjectFactory *mFastMoverFactory;

    PlayerObject *mPlayer;

    Ogre::SceneManager *mSceneMgr;

    Ogre::Vector3 mRelative;

    Ogre::Light* mLight;

    Ogre::Camera* mCamera;

    float mTime, mTimeSinceLastMonster, mMonsterEmitTime;

    Starfield *mStarfield;

    Ogre::SceneNode *mBoundsNode;
    Ogre::ManualObject *mLevelBounds;

    float mZoom;

    EffectSystem *mEffectSystem;
    EffectFactory *mExplosionFactory;

};

}; // namespace ReQuest

#endif
