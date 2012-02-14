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

#ifndef __STARFIELD_H
#define __STARFIELD_H

#include <OgreManualObject.h>
#include <OgreMath.h>

namespace ReQuest {

/// Starfield effect used throughout the game
class Starfield {
public:
    Starfield(Ogre::SceneManager *mgr, unsigned int starcount, bool fly);
    ~Starfield(void);

    void update(float delta);

protected:
    typedef struct {
        float x,y,z;
        float speed;
        float brightness;
        float lifetime;

        void advance(float delta) {
            z += delta * speed;
            lifetime += delta;
        };

        void flicker(float delta) {
            lifetime += delta;
            float bv = Ogre::Math::RangeRandom(-0.5, 0.5);
            brightness += speed * bv * delta;
            brightness = std::min(1.0f, brightness);
            brightness = std::max(0.0f, brightness);
        };

    } StarInfo;

    void prepare(void);
    void clear(void);

    void reinitStar(StarInfo* si);

    int mNumStars;

    StarInfo *mStarInfos;

    Ogre::ManualObject *mManualObject;

    Ogre::SceneManager *mSceneMgr;

    Ogre::SceneNode *mSceneNode;

    bool mFlyThrough;
};

}; // namespace ReQuest

#endif
