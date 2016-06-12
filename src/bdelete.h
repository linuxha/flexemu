/*
    bdelete.h

    Basic class for automatic instance destruction
    Copyright (C) 1997-2005  W. Schwotzer

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

#ifndef _BDELETE_H_
#define _BDELETE_H_


template <class T>
class BDeleter
{
private:
   T *object;

   BDeleter();
   BDeleter(const BDeleter &);
   BDeleter &operator=(const BDeleter &);
public:
   BDeleter(T *anObject);
   ~BDeleter();
};

template <class T>
BDeleter<T>::BDeleter(T *anObject) : object(NULL)
{
   object = anObject;
}

template <class T>
BDeleter<T>::~BDeleter()
{
   delete object;
   object = NULL;
}
#endif

