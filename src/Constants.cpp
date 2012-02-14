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

#include "Constants.h"

namespace ReQuest {

const std::string BULLET_MESH_NAME = "bullet.mesh";
const std::string MINE_MESH_NAME = "mine.mesh";
const std::string ENEMY_MESH_NAME = "enemy.mesh";
const std::string PLAYER_MESH_NAME = "ship.mesh";

const float BULLET_LIFETIME = 5.0f;

// Maximal zoom distance
const float ZOOM_MAX_DIST = 2.0;

// Has to be greater than zero
const float PROBABILITY_CORRECTION = 0.01;

int FIELD_WIDTH2 = 5;

int FIELD_HEIGHT2 = 5;

const int MAX_BULLET_COUNT = 30;

const float STAR_MAX_Z = 4.0f;

const float STAR_SPEED_MIN = 0.5f;

const float STAR_SPEED_MAX = 5.5f;

const float STAR_RANGE = 10.0f;

const float STAR_FADEIN_TIME = 5.0f;

const float COLLISION_DIST_COEFF = 1.25f;

const float EXPLOSION_LENGTH = 0.6f;

const float PLAYER_SPEED_COEFF = 0.01;

}; // namespace ReQuest

