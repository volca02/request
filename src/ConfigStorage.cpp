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

#include "ConfigStorage.h"

namespace ReQuest {

#ifdef __REQUEST_WINDOWS

void WindowsConfigHandler::getConfigDefault(const std::string& section, const std::string& key, std::string& target, const std::string& dflt) {
    // TODO: Code!
    target = dflt;
}

bool WindowsConfigHandler::getConfig(const std::string& section, const std::string& key, std::string& target) {
    // TODO: Code!
    target = "";
    return false;
}

void WindowsConfigHandler::storeConfig(void) {
    // TODO: Code!
}

#else

void LinuxConfigHandler::getConfigDefault(const std::string& section, const std::string& key, std::string& target, const std::string& dflt) {
    // TODO: Code!
    target = dflt;
}

bool LinuxConfigHandler::getConfig(const std::string& section, const std::string& key, std::string& target) {
    // TODO: Code!
    target = "";
    return false;
}

void LinuxConfigHandler::storeConfig(void) {
    // TODO: Code!
}

#endif

// ----------------------------------------------------------------------------
// ----- Configuration Storage Class ------------------------------------------
// ----------------------------------------------------------------------------
ConfigStorage::ConfigStorage(void) : mConfigHandler(0) {
#ifdef __REQUEST_WINDOWS
    mConfigHandler = new WindowsConfigHandler();
#else
    // TODO: Could also be other platforms?
    mConfigHandler = new LinuxConfigHandler();
#endif
}

ConfigStorage::~ConfigStorage(void) {
}

void ConfigStorage::getConfig(const std::string& section, const std::string& key, std::string& target, const std::string& dflt) {
    mConfigHandler->getConfigDefault(section, key, target, dflt);
}

bool ConfigStorage::getConfig(const std::string& section, const std::string& key, std::string& target) {
    return mConfigHandler->getConfig(section, key, target);
}


void ConfigStorage::storeConfig(void) {
    mConfigHandler->storeConfig();
}

}; // namespace ReQuest
