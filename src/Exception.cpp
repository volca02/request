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

#include "Exception.h"

#include <sstream>

namespace ReQuest {

// ----------------------------------------------------------------------------
// ----- Exception Class ------------------------------------------------------
// ----------------------------------------------------------------------------

Exception::Exception(const std::string& reason, const std::string& location, const char* file , long line) :
    mReason(reason),
    mLocation(location),
    mFile(file),
    mLine(line) {

}

Exception::~Exception(void) throw() {

}

std::string Exception::getDetails(void) {
    std::stringstream out;
    out << mLine;

    return mReason + " (" + mLocation + ") in " + mFile + ":" + out.str();
}

}; // namespace ReQuest
