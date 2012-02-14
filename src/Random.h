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

#ifndef __RANDOM_H
#define __RANDOM_H

#include <OgreMath.h>

namespace ReQuest {

/// @returns a float 0.0-1.0f
inline float rand1(void) {
    // return a float 0-1
    return Ogre::Math::RangeRandom(0.0f, 1.0f);
}

inline Ogre::Vector3 randomDirection(void) {
    Ogre::Vector3 v(Ogre::Math::RangeRandom(-1, 1),
                    Ogre::Math::RangeRandom(-1, 1),
                    0
    );

    v.normalise();

    return v;
}

}; // namespace ReQuest

#endif
