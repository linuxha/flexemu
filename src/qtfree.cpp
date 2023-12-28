/*                                                                              
    qtfree.cpp

 
    flexemu, an MC6809 emulator running FLEX
    Copyright (C) 2023  W. Schwotzer

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


#include <QLatin1Char>
#include <QFontDatabase>
#include <cmath>
#include "qtfree.h"

QFont GetFont(const QString &fontName)                       
{   
    auto list = fontName.split(QLatin1Char(','));
    
    if (list.size() >= 4)
    {
        bool ok = false;
        auto family = list[0];
        auto style = list[3];
        auto pointSizeString = list[1];
        auto pointSize =
            static_cast<int>(std::round(pointSizeString.toFloat(&ok)));

        if (ok && pointSize > 0)
        {
            auto fontDatabase = QFontDatabase();
            return fontDatabase.font(family, style, pointSize);
        }
    }

    return QFont();
}
