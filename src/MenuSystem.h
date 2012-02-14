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

#ifndef __MENUSYSTEM_H
#define __MENUSYSTEM_H

#include "Prerequisites.h"
#include "SubSystem.h"

#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>

#include <string>
#include <vector>
#include <map>
#include <tr1/functional>

namespace ReQuest {

// forward decl
class MenuSheet;

typedef std::tr1::function<void()> MenuCallback;

class MenuRect {
public:
    MenuRect() : mX(0), mY(0), mW(0), mH(0) {};

    bool inside(int x, int y) {
        return ( (x > mX) && (y > mY) && (x <= mX + mW) && (y <= mY + mH));
    };

protected:
    int mX, mY, mW, mH;
};

/// Item of the menu
class MenuItem : public MenuRect {
public:
    /// Constructor. Takes ownership of the callback! (so no deletion is needed, as destructor of menuitem will take care of it)
    MenuItem(MenuSheet* owner, const std::string& caption, MenuCallback cb);
    ~MenuItem(void);

    void clicked(void);

    void build(int id, int x, int y, int w, int h);

    /// sets position, and turns off autopositioning
    void setPosition(int x, int y, int w, int h);

    void focus(void);

    void unFocus(void);

    void setMaterialName(const std::string& name);

protected:
    void release(void);

    std::string mCaption;
    std::string mMatName;
    MenuCallback mCallback;
    MenuSheet *mOwner;
    Ogre::OverlayContainer *mOverlay;
    bool mAutoPosition;
    bool mFocused;
};

class MenuSystem;

/// one sheet of the menu
class MenuSheet : MenuRect {
public:
    MenuSheet(MenuSystem *owner, const std::string& name, const std::string& fontName);

    ~MenuSheet(void);

    MenuItem* addItem(const std::string& caption, MenuCallback cb = NULL);

    void build(void);

    void activate(void);

    void deactivate(void);

    bool isBuilt(void) { return mBuilt; };

    /// internal. Used by MenuItem to create overlay
    Ogre::OverlayContainer* createOverlay(int id, const std::string& name, int x, int y, int w, int h);

    /// internal. Used by MenuItem to destroy overlay
    void destroyOverlay(Ogre::OverlayContainer* ov);

    /// call this to inject mouse click
    void click(int x, int y);

    /// call this to inject mouse movement
    void mouseMoved(int x, int y);

    /// sets the sheet's desired allocated space on screen
    void setRect(int x, int y, int w, int h);

    void setFontSize(int size) { mFontSize = size; };

protected:
    typedef std::vector<MenuItem*> MenuItems;

    MenuItems mMenuItems;

    bool mBuilt;

    std::string mName;

    MenuSystem *mOwner;

    Ogre::Overlay *mOverlay;

    std::string mFont;

    MenuItem *mFocus;

    int mFontSize;
};


/// menu system. Contains all the menu sheets. Can be shown/hidden. Handles sheet switching
class MenuSystem : public SubSystem {
public:
    MenuSystem(Game* owner, std::string materialName, RendererSystem* rs);

    ~MenuSystem(void);

    MenuSheet* createSheet(const std::string& name, const std::string& fontName);

    MenuSheet* getSheet(const std::string& name);

    void setActiveSheet(const std::string& name);

    void setActiveSheet(MenuSheet* sheet);

    void show(void);

    void hide(void);

    void update(float delta);

    std::string& getMaterialName(void) { return mMaterialName; };

    Ogre::OverlayContainer *getCursor(void) { return mCursor; };

    void injectMouseMoved(int x, int y);

    void injectMouseClick(int x, int y);

    int getScreenWidth();
    int getScreenHeight();

protected:
    typedef std::map<std::string, MenuSheet*> SheetMap;

    SheetMap mSheetMap;
    MenuSheet *mActiveSheet;
    std::string mMaterialName;
    Ogre::OverlayContainer *mCursor;
    Ogre::Overlay *mCursorOverlay;


    int mMx, mMy;

    RendererSystem *mRendererSystem;
};

}; // namespace ReQuest

#endif
