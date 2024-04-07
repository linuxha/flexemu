/*
    fattrib.h

    FLEXplorer, An explorer for any FLEX file or disk container
    Copyright (C) 2024  W. Schwotzer

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef FATTRIB_INCLUDED
#define FATTRIB_INCLUDED

#include "typedefs.h"
#include <array>
#include <utility>


// Supported file attributes.
// They are used as bit masks and can be combined.
// File attributes are used in struct s_dir_entry in field file_attr.
// All other bits of file_attr should remain 0.
// (WRITE_PROTECT also used for container attribute)
const Byte WRITE_PROTECT = 0x80;
const Byte DELETE_PROTECT = 0x40;
const Byte READ_PROTECT = 0x20;
const Byte CATALOG_PROTECT = 0x10;
const Byte ALL_PROTECT = WRITE_PROTECT | DELETE_PROTECT | READ_PROTECT |
                         CATALOG_PROTECT;

extern const std::array<std::pair<char, Byte>, 4> attributeCharToFlag;

#endif
