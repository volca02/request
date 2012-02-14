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

#ifndef __CONSTANTS_H
#define __CONSTANTS_H

#include <string>

namespace ReQuest {

// ----  All the extern consts here:  -----
/// Bullet mesh name
extern const std::string BULLET_MESH_NAME;

/// Mine mesh name
extern const std::string MINE_MESH_NAME;

/// Bulet lifetime in seconds
extern const float BULLET_LIFETIME;

/// Enemy mesh name
extern const std::string ENEMY_MESH_NAME;

/// Maximal distance enemy will do the zoom move from
extern const float ZOOM_MAX_DIST;

/// Corrects the RandomTimer probability (stretches the random generated time)
extern const float PROBABILITY_CORRECTION;

/// width of the playfield
extern int FIELD_WIDTH2;

/// height of the playfield
extern int FIELD_HEIGHT2;

/// Maximal count of player's bullets
extern const int MAX_BULLET_COUNT;

/// Name of the player's mesh
extern const std::string PLAYER_MESH_NAME;

/// Maximal Z value. After reaching there, star is initialized again
extern const float STAR_MAX_Z;

/// Star speed minimal
extern const float STAR_SPEED_MIN;

/// Star speed maximal
extern const float STAR_SPEED_MAX;

/// X,Y half of the range the star should be created in
extern const float STAR_RANGE;

/// Time in seconds to fade in
extern const float STAR_FADEIN_TIME;

/// The coefficient to use for distance -> radius comparisons in collision detection (>1 the objects will have to be closer to collide)
extern const float COLLISION_DIST_COEFF;

/// length of the explosion in seconds
extern const float EXPLOSION_LENGTH;

/// Speed compensation of player object (multiplies the mouse steps/second)
extern const float PLAYER_SPEED_COEFF;

}; // namespace ReQuest

#endif
