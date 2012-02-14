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

#ifndef __PREREQUISITES_H
#define __PREREQUISITES_H

namespace ReQuest {

// Prerequisites are handled here to break the .h inclusion nightmares
class Game;
class ConfigStorage;
class InputSystem;
class InputListener;
class RendererSystem;
class GameState;
class MenuItem;
class MenuSheet;
class MenuSystem;
class ObjectSystem;
class EffectSystem;

class Starfield;

class Effect;
class EffectFactory;

class GameObject;
class GameObjectFactory;

// Various factories and object types
class PlayerObject;
class PlayerObjectFactory;

class RenderedGameObject;
class RenderedGameObjectFactory;

class BulletObject;
class BulletObjectFactory;

class MineObject;
class MineObjectFactory;

class EnemyObject;
class EnemyObjectFactory;


// various
class PowerUp;

}; // namespace ReQuest

#endif
