/*
 *  The Regina Rexx Interpreter
 *  Copyright (C) 1992-2016  Jan Lisowiec <jan.lisowiec@lzlabs.com>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * assert_custom.h
 *
 *  Created on: Jan 27, 2016
 */

#ifndef ASSERT_CUSTOM_H_
#define ASSERT_CUSTOM_H_

#ifdef REXX_8BIT_SUPPORT
void assert_custom(const char *file, int line, int cond);

#define ASSERT(a) assert_custom(__FILE__, __LINE__, (a))
#else
#define ASSERT(a) assert((a))
#endif


#endif /* ASSERT_CUSTOM_H_ */
