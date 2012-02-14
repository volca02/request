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

#include "Starfield.h"
#include "Constants.h"

#include <OgreMath.h>
#include <OgreSceneManager.h>

using namespace Ogre;

namespace ReQuest {


Starfield::Starfield(Ogre::SceneManager *mgr, unsigned int starcount, bool fly) :
    mSceneMgr(mgr),
    mStarInfos(0),
    mNumStars(starcount),
    mFlyThrough(fly),
    mManualObject(0)
{
    mSceneNode = static_cast<SceneNode*>(mSceneMgr->getRootSceneNode()->createChild("STARFIELD_SCENE_NODE"));

    prepare();
}

Starfield::~Starfield(void) {
    clear();

    mSceneMgr->getRootSceneNode()->removeChild("STARFIELD_SCENE_NODE");
    mSceneMgr->destroySceneNode("STARFIELD_SCENE_NODE");
}

void Starfield::update(float delta) {
    if (!mStarInfos)
        return;

    // advance the stars, reinitialize the stars that cross a certain boundary
    for (unsigned int n = 0; n < mNumStars; ++n) {
        StarInfo* si = &mStarInfos[n];

        if (mFlyThrough)
            si->advance(delta);
        else
            si->flicker(delta);

        if (si->z > STAR_MAX_Z) {
            reinitStar(si);
        }
    }

    mManualObject->beginUpdate(0);
    // now update the manual object to reflect the changes
    for (unsigned int n = 0; n < mNumStars; ++n) {
        StarInfo* si = &mStarInfos[n];

        float fb = si->brightness;
        if (si->lifetime < STAR_FADEIN_TIME) {
            fb *= si->lifetime / STAR_FADEIN_TIME;
        }

        mManualObject->position(si->x, si->y, si->z);
        mManualObject->colour(fb, fb, fb);
    }
    mManualObject->end();
}

void Starfield::prepare(void) {
    clear();

    // now init
    mStarInfos = new StarInfo[mNumStars];

    for (unsigned int n = 0; n < mNumStars; ++n) {
        reinitStar(&mStarInfos[n]);
    }

    mManualObject = mSceneMgr->createManualObject("STARFIELD_MANUAL_OBJECT");
    mManualObject->begin("BaseWhiteNoLighting", RenderOperation::OT_POINT_LIST);

    // now update the manual object to reflect the changes
    for (unsigned int n = 0; n < mNumStars; ++n) {
        StarInfo* si = &mStarInfos[n];

        mManualObject->position(si->x, si->y, si->z);
        mManualObject->colour(0, 0, 0);
    }

    mManualObject->end();

    mSceneNode->attachObject(mManualObject);
}

void Starfield::clear(void) {
    delete[] mStarInfos;
    mStarInfos = 0;

    // delete the manual object?
    // yep
    if (mManualObject) {
        mSceneNode->detachObject(mManualObject);
        mSceneMgr->destroyManualObject(mManualObject);
        mManualObject = 0;
    }
}

void Starfield::reinitStar(StarInfo* si) {
    si->lifetime = 0;
    si->brightness = Math::RangeRandom(0.1, 1);

    // RLY?
    si->speed = Math::RangeRandom(STAR_SPEED_MIN, STAR_SPEED_MAX);

    si->x = Math::RangeRandom(-STAR_RANGE, STAR_RANGE);
    si->y = Math::RangeRandom(-STAR_RANGE, STAR_RANGE);
    si->z = Math::RangeRandom(-STAR_RANGE, 0);
}


}; // namespace ReQuest
