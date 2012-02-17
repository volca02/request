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

#include "MenuSystem.h"

#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>

#include "RendererSystem.h"

namespace ReQuest {

using namespace Ogre;

MenuItem::MenuItem(MenuSheet* owner, const std::string& caption, MenuCallback cb) :
    mOwner(owner),
    mCaption(caption),
    mCallback(cb),
    mFocused(false),
    mOverlay(0),
    mMatName(""),
    mAutoPosition(true)
{
    //
};

MenuItem::~MenuItem(void) {
    release();
};


void MenuItem::clicked(void) {
    if (mCallback)
        mCallback();
};

void MenuItem::build(int id, int x, int y, int w, int h) {
    release();

    if (mAutoPosition) {
        mX = x;
        mY = y;
        mW = w;
        mH = h;
    }

    mOverlay = mOwner->createOverlay(id, mCaption, mX, mY, mW, mH);

    if (!mMatName.empty())
        mOverlay->setMaterialName(mMatName);

    unFocus();
}

void MenuItem::setPosition(int x, int y, int w, int h) {
    mX = x;
    mY = y;
    mW = w;
    mH = h;
}

void MenuItem::release(void) {
    if (mOverlay) {
        // return the overlay
        mOwner->destroyOverlay(mOverlay);
        mOverlay = 0;
    }
}

void MenuItem::focus(void) {
    if (mCallback) {
        mOverlay->setColour(ColourValue(1,0.5,0.5));
        mFocused = true;
    }
}

void MenuItem::unFocus(void) {
    mOverlay->setColour(ColourValue(1,1,1));

    mFocused = true;
}

void MenuItem::setMaterialName(const std::string& name) {
    if (mOverlay)
        mOverlay->setMaterialName(name);

    mMatName = name;
}

MenuSheet::MenuSheet(MenuSystem *owner,
                     const std::string& name,
                     const std::string& fontName)
    : mMenuItems(),
      mFont(fontName),
      mName(name),
      mBuilt(false),
      mOwner(owner),
      mOverlay(0),
      mFocus(0),
      mFontSize(16)
{
    mOverlay = OverlayManager::getSingleton().create(mName);

    mX = 0;
    mY = 0;
    mW = mOwner->getScreenWidth();
    mH = mOwner->getScreenHeight();
};

MenuSheet::~MenuSheet(void) {
    MenuItems::iterator it = mMenuItems.begin();

    for (; it != mMenuItems.end(); ++it) {
        delete *it;
    }

    mMenuItems.clear();

    OverlayManager::getSingleton().destroy(mOverlay);
    mOverlay = 0;
};


MenuItem* MenuSheet::addItem(const std::string& caption, MenuCallback cb) {
    MenuItem *mi = new MenuItem(this, caption, cb);
    mMenuItems.push_back(mi);

    return mi;
};

void MenuSheet::build(void) {
    // Release will happen automatically. No need to do it manually
    mBuilt = false;

    // but just in case it was built before
    MenuItems::iterator it = mMenuItems.begin();

    int y = mY;

    int height = mH / mMenuItems.size();

    int id = 0;

    for (;it != mMenuItems.end(); ++it) {
        // TODO: Some nice calculation
        (*it)->build(id++, mX, y, mW, height);
        y += height;
    }

    mBuilt = true;
}

void MenuSheet::activate(void) {
    if (!mBuilt)
        build();

    mOverlay->show();
}

void MenuSheet::deactivate(void) {
    mOverlay->hide();
}

OverlayContainer* MenuSheet::createOverlay(int id, const std::string& text, int x, int y, int w, int h) {
    std::string ovname = mName + "/" + Ogre::StringConverter::toString(id);

    OverlayContainer* ov;

    if (text.empty())
        ov = static_cast<OverlayContainer*>(OverlayManager::getSingleton().createOverlayElement("Panel", ovname));
    else
        ov = static_cast<OverlayContainer*>(OverlayManager::getSingleton().createOverlayElement("TextArea", ovname));

    ov->setMetricsMode(GMM_PIXELS);

    // text area centered around X point, so we create it with w/2 surplus
    if (text.empty())
        ov->setPosition(x, y);
    else
        ov->setPosition(x + w/2, y);

    ov->setDimensions(w, h);

    ov->setCaption(text);
    ov->setParameter("font_name", mFont);
    ov->setParameter("char_height", StringConverter::toString(mFontSize));

    ov->setParameter("alignment", "center");

    ov->show();

    mOverlay->add2D(ov);

    return ov;
}

void MenuSheet::destroyOverlay(Ogre::OverlayContainer* ov) {
    mOverlay->remove2D(ov);
    OverlayManager::getSingleton().destroyOverlayElement(ov);
}

void MenuSheet::click(int x, int y) {
    // iterate over elements, find the one that fits, click it
    MenuItems::iterator it = mMenuItems.begin();

    for (;it != mMenuItems.end(); ++it) {
        if ((*it)->inside(x,y)) {
            (*it)->clicked();
            return;
        }
    }
}

void MenuSheet::mouseMoved(int x, int y) {
    MenuItem* newF = NULL;
    MenuItems::iterator it = mMenuItems.begin();

    for (;it != mMenuItems.end(); ++it) {
        if ((*it)->inside(x,y)) {
            newF = *it;
        }
    }

    if (mFocus != newF) {
        if (mFocus)
            mFocus->unFocus();

        mFocus = newF;

        if (mFocus)
            mFocus->focus();
    }
}

void MenuSheet::setRect(int x, int y, int w, int h) {
    mX = x;
    mY = y;
    mW = w;
    mH = h;
}

MenuSystem::MenuSystem(Game* owner, std::string materialName, RendererSystem* rs) :
    SubSystem(owner),
    mActiveSheet(0),
    mMaterialName(materialName),
    mRendererSystem(rs) {

    mCursor = static_cast<OverlayContainer*>(OverlayManager::getSingleton().createOverlayElement("Panel", "MouseCursor"));

    mCursor->setMaterialName("MouseCursor");

    mCursor->setMetricsMode(GMM_PIXELS);

    mCursor->setPosition(0, 0);
    mCursor->setDimensions(32, 32);

    mCursor->show();

    mCursorOverlay = OverlayManager::getSingleton().create("__CURSOR_OVERLAY__");

    mCursorOverlay->setZOrder(640);

    mCursorOverlay->add2D(mCursor);
};

MenuSystem::~MenuSystem(void) {
    SheetMap::iterator it = mSheetMap.begin();

    for (; it != mSheetMap.end(); ++it) {
        delete it->second;
    }

    mSheetMap.clear();

    mCursorOverlay->remove2D(mCursor);
    OverlayManager::getSingleton().destroyOverlayElement(mCursor);
    OverlayManager::getSingleton().destroy(mCursorOverlay);
};

MenuSheet *MenuSystem::createSheet(const std::string& name, const std::string& fontName) {
    MenuSheet* sht = getSheet(name);

    if (sht)
        return sht;

    sht = new MenuSheet(this, name, fontName);

    mSheetMap.insert(std::make_pair(name, sht));

    return sht;
}

MenuSheet * MenuSystem::getSheet(const std::string& name) {
    SheetMap::iterator it = mSheetMap.find(name);

    if (it != mSheetMap.end())
        return it->second;

    return NULL;
}

void MenuSystem::setActiveSheet(const std::string& name) {
    if (mActiveSheet)
        mActiveSheet->deactivate();

    mActiveSheet = getSheet(name);

    if (mActiveSheet)
        mActiveSheet->activate();
}

void MenuSystem::setActiveSheet(MenuSheet* sheet) {
    if (mActiveSheet)
        mActiveSheet->deactivate();

    mActiveSheet = sheet;

    if (mActiveSheet)
        mActiveSheet->activate();
}

void MenuSystem::show(void) {
    mActiveSheet->activate();
    mCursor->show();
    mCursorOverlay->show();
}

void MenuSystem::hide(void) {
    mActiveSheet->deactivate();
    mCursor->hide();
    mCursorOverlay->hide();
}

void MenuSystem::update(float delta) {
    // TODO: MenuSheet/MenuItem "mutators" could be used here
    // e.g.: update all active Menu effects
}

void MenuSystem::injectMouseMoved(int x, int y) {
    mMx = x; mMy = y;

    mCursor->setPosition(x, y);

    if (mActiveSheet)
        mActiveSheet->mouseMoved(mMx, mMy);
};

void MenuSystem::injectMouseClick(int x, int y) {
    mMx = x; mMy = y;

    if (mActiveSheet)
        mActiveSheet->click(mMx, mMy);
};

int MenuSystem::getScreenWidth() {
    return mRendererSystem->getScreenWidth();
};

int MenuSystem::getScreenHeight() {
    return mRendererSystem->getScreenHeight();
};

} // namespace ReQuest
