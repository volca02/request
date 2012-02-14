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

#ifndef __CONFIGSTORAGE_H
#define __CONFIGSTORAGE_H

#include "config.h"
#include <string>

namespace ReQuest {

// config handler interface. Handles config storage based on the platform (ancestor)
class ConfigHandler {
public:
    /// gets a stored config value. if the config value is not found, default is used (and stored into config)
    virtual void getConfigDefault(const std::string& section, const std::string& key, std::string& target, const std::string& dflt) = 0;

    /// gets the config key associated value, or returns false if it does not exist
    virtual bool getConfig(const std::string& section, const std::string& key, std::string& target) = 0;

    /// stores the configuration for further use
    virtual void storeConfig(void)= 0;
};


#ifdef __REQUEST_WINDOWS
/// Windows config handler. Only available for windows
class WindowsConfigHandler : public ConfigHandler {
public:
    WindowsConfigHandler(void) {};
    ~WindowsConfigHandler(void) {};

    /// @see ConfigHandler::getConfigDefault
    virtual void getConfigDefault(const std::string& section, const std::string& key, std::string& target, const std::string& dflt);

    /// @see ConfigHandler::getConfig
    virtual bool getConfig(const std::string& section, const std::string& key, std::string& target);

    /// @see ConfigHandler::storeConfig
    virtual void storeConfig(void);
};

#else
/// Linux config handler. Handles the config storage on unix machines
class LinuxConfigHandler : public ConfigHandler {
public:
    LinuxConfigHandler(void) {};
    ~LinuxConfigHandler(void) {};

    /// @see ConfigHandler::getConfigDefault
    virtual void getConfigDefault(const std::string& section, const std::string& key, std::string& target, const std::string& dflt);

    /// @see ConfigHandler::getConfig
    virtual bool getConfig(const std::string& section, const std::string& key, std::string& target);

    /// @see ConfigHandler::storeConfig
    virtual void storeConfig(void);

};
#endif

/// Storage for configuration. Platform dependant handler for config variables
class ConfigStorage {
public:
    /// Constructor
    ConfigStorage(void);

    /// Destructor
    ~ConfigStorage(void);

    /// gets a stored config value. if the config value is not found, default is used (and stored into config)
    void getConfig(const std::string& section, const std::string& key, std::string& target, const std::string& dflt);

    /// gets the config key associated value, or returns false if it does not exist
    bool getConfig(const std::string& section, const std::string& key, std::string& target);

    /// stores the configuration for further use
    void storeConfig(void);

protected:
    ConfigHandler *mConfigHandler;
};

}; // namespace ReQuest

#endif
