/* primetest.c -- 
 * Created: Thu Jul 20 22:15:54 1995 by r.faith@ieee.org
 * Revised: Mon Jan  1 15:18:19 1996 by r.faith@ieee.org
 * Copyright 1995 Rickard E. Faith (r.faith@ieee.org)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 * $Id: primetest.c,v 1.5 1996/01/02 04:09:45 faith Exp $
 * 
 */

#include "maaP.h"

int main( int argc, char **argv )
{
   if (argc != 2) {
      printf( "%lu\n", prm_next_prime( 10000 ) );
   } else {
      printf( "%lu\n", prm_next_prime( strtoul( argv[1], NULL, 0 ) ) );
   }
   return 0;
}
