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

#ifndef __OBJECTTYPES_H
#define __OBJECTTYPES_H

namespace ReQuest {

/// Different object types as an enum
enum ObjectTypes {
    /// Player object
    OT_PLAYER = 1,
    /// Crystal object - the thing that should be collected
    OT_CRYSTAL,
    /// Bullet shot by player
    OT_BULLET,
    /// A mine,
    OT_MINE,
    /// Enemy base type (should not be used).
    /// Test if object type is >= to this to find out if the colliding object is enemy to player
    OT_ENEMY,
    OT_GRUNGER,
    OT_FASTMOVER,
    OT_SHOOTER,
    OT_HIBERNATOR,
    OT_MINER,
    OT_MEEBY,
    OT_RETALIATOR,
    OT_TERRIER,
    OT_DOING,
    OT_SNIPE,
    OT_TERRIER_MOTHER,
    OT_TRIBBLES,
    OT_BUCKSHOT,
    OT_CLUSTER,
    OT_STICKTIGHT,
    OT_REPUSLOR
};

enum EffectTypes {
    EFF_EXPLOSION = 1
};

/// A struct describing the enemy behavior
struct EnemyConstructionParams {
    /// type of the mesh, as enumerated in ObjectTypes enum
    int type;

    /// readable name of the enemy
    const char *name;

    /// The mesh the enemy uses
    const char *meshName;

    /// Stationary object. Does not move. Used for mines
    bool stationary;

    /// Movement speed (rational values are in 0 - 0.01 ranged)
    float speed;

    /// Movement curvature
    float curve;

    /// the type of the fired projectile
    int firetype;

    /// score one gets after killing this enemy
    int score;

    // TODO: int deathsound;

    // -- The capabilities as taken from the original xquest --
    bool
    /// can fire a bullet
    fires,
    /// tries to follow the player with some probability
        follows,
    /// flies with a curved path
        curves,
    /// explodes when destroyed (TODO: How/When?)
        explodes,
    /// lays mines
        laysmines,
    /// shoots back when shot
        shootback,
    /// zooms - gets closer to the player once in a certain range
        zoom,
    /// flies using the maximal possible speed - the one specified
        maxspeed,
    /// TODO: Dunno what these are yet
        rebounds, tribbles, repulses;

    /// The probabilities as in the original xquest (all tested against random 0.0f-1.0f. I guess this makes them framerate dependent)
    float
    /// controls the probability of the enemy to fire a projectile or lay a mine
    fireprob,
    /// controls the probability of the enemy to choose the next direction to travel
        changedir,
    /// controls the probability of the enemy to change the curve to travel on
        changecurve,
    /// controls the probability of the enemy to follow the player
        follow;
};

// --- TYPES OF ENEMIES ---
const EnemyConstructionParams GrungerEnemy = {
    OT_GRUNGER,
    "Grunger",
    "grunger.mesh",
    false,  // stationary
    0.5, // speed
    0.0, // curvature
    -1, // type of the fired projectile object
    200, // score points
    false, // fires
    false, // follows
    false, // curves
    false, // explodes
    false, // laysmines
    false, // shootback
    false, // zoom
    false, // maxspeed
    false, // rebounds
    false, // tribbles
    false, // repulses
    0.0, // fireprob
    0.001, // changedir
    0.0, // changecurve
    0.0 // follow
};

const EnemyConstructionParams FastMoverEnemy = {
    OT_FASTMOVER,
    "Fast Mover",
    "fastmover.mesh",
    false,  // stationary
    1.2, // speed
    6.0, // curvature
    -1, // type of the fired projectile object
    200, // score points
    false, // fires
    false, // follows
    true, // curves
    false, // explodes
    false, // laysmines
    false, // shootback
    false, // zoom
    false, // maxspeed
    false, // rebounds
    false, // tribbles
    false, // repulses
    0.0, // fireprob
    0.001, // changedir
    0.01, // changecurve
    0.0 // follow
};


}; // namespace ReQuest

#endif
