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

#ifndef __EXCEPTION_H
#define __EXCEPTION_H

#include <exception>
#include <string>

namespace ReQuest {
/// ReQuest's own exception type.
class Exception : public std::exception {
public:
    /// Constructor
    Exception(const std::string& reason, const std::string& location, const char* file = NULL, long line = -1);

    /// destructor
    ~Exception(void) throw();

    /// returns the full detailed description of the exception (incl. source and line number of the throw)
    std::string getDetails(void);

    /// returns the reason of the exception (without code location, etc.)
    std::string getReason(void) { return mReason; };

protected:
    // --- member vars
    std::string mReason, mLocation, mFile;
    long mLine;
};

#define REQUEST_EXCEPT(reason, location) throw( ReQuest::Exception(reason, location, __FILE__, __LINE__) )

}; // namespace ReQuest

#endif
