/* base64.c -- Encode/decode integers in base64 format
 * Created: Mon Sep 23 16:55:12 1996 by faith@cs.unc.edu
 * Revised: Mon Sep 23 21:03:41 1996 by faith@cs.unc.edu
 * Copyright 1996 Rickard E. Faith (faith@cs.unc.edu)
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
 * $Id: base64.c,v 1.1 1996/09/24 01:06:11 faith Exp $
 *

 * These routines use the 64-character subset of International Alphabet IA5
 * discussed in RFC 1421 (printeable encoding) and RFC 1522 (base64 MIME):

   Value Encoding  Value Encoding  Value Encoding  Value Encoding
       0 A            17 R            34 i            51 z
       1 B            18 S            35 j            52 0
       2 C            19 T            36 k            53 1
       3 D            20 U            37 l            54 2
       4 E            21 V            38 m            55 3
       5 F            22 W            39 n            56 4
       6 G            23 X            40 o            57 5
       7 H            24 Y            41 p            58 6
       8 I            25 Z            42 q            59 7
       9 J            26 a            43 r            60 8
      10 K            27 b            44 s            61 9
      11 L            28 c            45 t            62 +
      12 M            29 d            46 u            63 /
      13 N            30 e            47 v
      14 O            31 f            48 w         (pad) =
      15 P            32 g            49 x
      16 Q            33 h            50 y
 *
 */

#include "maaP.h"

#include <netinet/in.h>

static unsigned char b64_list[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

#define XX 100

static int b64_index[256] = {
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,62, XX,XX,XX,63,
    52,53,54,55, 56,57,58,59, 60,61,XX,XX, XX,XX,XX,XX,
    XX, 0, 1, 2,  3, 4, 5, 6,  7, 8, 9,10, 11,12,13,14,
    15,16,17,18, 19,20,21,22, 23,24,25,XX, XX,XX,XX,XX,
    XX,26,27,28, 29,30,31,32, 33,34,35,36, 37,38,39,40,
    41,42,43,44, 45,46,47,48, 49,50,51,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
};

/* |b64_encode| encodes |val| in a printable base 64 format.  |val| is
   first converted to network byte order.  Then a MSB-first encoding is
   generated. */

const char *b64_encode( unsigned long val )
{
   static char   result[7];
   unsigned long v = htonl( val ); /* Convert to network byte order first! */

   result[0] = b64_list[ (v & 0xc0000000) >> 30 ];
   result[1] = b64_list[ (v & 0x3f000000) >> 24 ];
   result[2] = b64_list[ (v & 0x00fc0000) >> 18 ];
   result[3] = b64_list[ (v & 0x0003f000) >> 12 ];
   result[4] = b64_list[ (v & 0x00000fc0) >>  6 ];
   result[5] = b64_list[ (v & 0x0000003f)       ];
   result[6] = 0;

   return result;
}

unsigned long b64_decode( const char *val )
{
   unsigned long v = 0;
   int           i;
   int           offset = 30;

   for (i = 0; i < 6; i++) {
      int tmp = b64_index[ (unsigned char)val[i] ];

      if (tmp == XX)
	 err_internal( __FUNCTION__,
		       "Illegal character in base64 value: `%c'\n", val[i] );
      
      v |= tmp << offset;
      offset -= 6;
   }

   return ntohl( v );
}
