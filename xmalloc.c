/* xmalloc.c -- Error-checking malloc
 * Created: Sun Nov  6 18:14:10 1994 by faith@dict.org
 * Copyright 1994-1996, 2002 Rickard E. Faith (faith@dict.org)
 * Copyright 2002-2008 Aleksey Cheusov (vle@gmx.net)
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Library General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 */

#include "maaP.h"

#if STDC_HEADERS
#include <stdlib.h>
#else
void *malloc();
void *calloc();
void *realloc();
void free();
#endif

#ifndef DMALLOC_FUNC_CHECK

void *xmalloc( size_t size )
{
   void *pt = malloc( size );

   if (!pt) err_fatal( __func__,
		       "Out of memory while allocating %lu bytes\n",
		       (unsigned long)size );
   return pt;
}

void *xcalloc( size_t num, size_t size )
{
   void *pt = calloc( num, size );

   if (!pt) err_fatal( __func__,
		       "Out of memory while allocating %lu X %lu byte block\n",
		       (unsigned long)num, (unsigned long)size );
   return pt;
}

void *xrealloc( void *pt, size_t size )
{
   void *new = realloc( pt, size );

   if (!new) err_fatal( __func__,
			"Out of memory while reallocating block at %p to"
			" %lu bytes\n",
			pt,
			(unsigned long)size );
   return new;
}

void xfree( void *pt )
{
   if (pt) free( pt );
   else err_fatal( __func__, "Attempt to free null pointer\n" );
}

char *xstrdup( const char *s )
{
   size_t len = strlen (s);
   char *pt = xmalloc (len + 1);

   if (!pt) err_fatal( __func__,
		       "Out of memory while duplicating string\n" );

   memcpy (pt, s, len + 1);
   return pt;
}

#endif
