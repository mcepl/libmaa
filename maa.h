/* maa.h -- Header file for visible libmaa functions
 * Created: Sun Nov 19 13:21:21 1995 by faith@cs.unc.edu
 * Revised: Sat Feb 24 18:57:06 1996 by faith@cs.unc.edu
 * Copyright 1994, 1995, 1996 Rickard E. Faith (faith@cs.unc.edu)
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
 * $Id: maa.h,v 1.10 1996/02/25 00:08:58 faith Exp $
 */

#ifndef _MAA_H_
#define _MAA_H_

#include <stdio.h>
#include <stdarg.h>

#ifndef __GNUC__
#define __inline__
#define __attribute__(x)
#endif

				/* If MAA_MAGIC is non-zero, important
                                   structures will be tagged with a magic
                                   number which will be checked for
                                   integrity at each access.  This uses up
                                   more memory, and is slightly slower, but
                                   helps debugging quite a bit. */
#ifndef MAA_MAGIC
#define MAA_MAGIC 1
#endif

#if MAA_MAGIC
#define HSH_MAGIC               0x01020304
#define HSH_MAGIC_FREED         0x10203040
#define SET_MAGIC               0x02030405
#define SET_MAGIC_FREED         0x20304050
#define LST_MAGIC               0x03040506
#define LST_MAGIC_FREED         0x30405060
#define MEM_STRINGS_MAGIC       0x23232323
#define MEM_STRINGS_MAGIC_FREED 0x32323232
#define MEM_OBJECTS_MAGIC       0x42424242
#define MEM_OBJECTS_MAGIC_FREED 0x24242424
#define ARG_MAGIC               0xfeedbead
#define ARG_MAGIC_FREED         0xefdeebda
#define PR_MAGIC                0x0bad7734
#define PR_MAGIC_FREED          0xb0da7743
#define SL_LIST_MAGIC           0xabcdef01
#define SL_LIST_MAGIC_FREED     0xbadcfe10
#define SL_ENTRY_MAGIC          0xacadfeed
#define SL_ENTRY_MAGIC_FREED    0xcadaefde
#endif

/* version.c */

extern const char *maa_revision_string;


/* maa.c */

#define MAA_MEMORY   0xc1000000	/* Print memory usage statistics at exit */
#define MAA_TIME     0xc2000000	/* Print timer information at exit       */
#define MAA_PR       0xc4000000	/* Debug process routines                */
#define MAA_SL       0xc8000000	/* Debug skip list routines              */

extern void       maa_init( const char *programName );
extern void       maa_shutdown( void );
extern int        maa_version_major( void );
extern int        maa_version_minor( void );
extern const char *maa_version( void );

/* xmalloc.c */

#ifndef DMALLOC_FUNC_CHECK
extern __inline__ void *xmalloc( unsigned int size );
extern __inline__ void *xcalloc( unsigned int num, unsigned int size );
extern __inline__ void *xrealloc( void *pt, unsigned int size );
extern __inline__ void xfree( void *pt );
extern __inline__ char *xstrdup( const char *s );
#endif


/* bit.c */
extern __inline__ void bit_set( unsigned long *flags, int bit );
extern __inline__ void bit_clr( unsigned long *flags, int bit );
extern __inline__ int  bit_tst( unsigned long *flags, int bit );
extern __inline__ int  bit_cnt( unsigned long *flags );

/* prime.c */

extern int           prm_is_prime( unsigned int value );
extern unsigned long prm_next_prime( unsigned int start );

/* hash.c */

typedef void *hsh_HashTable;

typedef struct hsh_Stats {
   unsigned long size;		 /* Size of table */
   unsigned long resizings;	 /* Number of resizings */
   unsigned long entries;	 /* Total entries in table */
   unsigned long buckets_used;	 /* Number of hash buckets in use */
   unsigned long singletons;	 /* Number of length one lists */
   unsigned long maximum_length; /* Maximum list length */

   unsigned long retrievals;	 /* Total number of retrievals */
   unsigned long hits;		 /* Number of retrievals from top of a list */
   unsigned long misses;	 /* Number of unsuccessful retrievals */
} *hsh_Stats;

extern hsh_HashTable hsh_create( unsigned long (*hash)( const void * ),
				 int (*compare)( const void *,
						 const void * ) );
extern void          hsh_destroy( hsh_HashTable table );
extern int           hsh_insert( hsh_HashTable table,
				 const void *key,
				 const void *datum );
extern int           hsh_delete( hsh_HashTable table, const void *key );
extern const void    *hsh_retrieve( hsh_HashTable table, const void *key );
extern int           hsh_iterate( hsh_HashTable table,
				  int (*iterator)( const void *key,
						   const void *datum ) );
extern hsh_Stats     hsh_get_stats( hsh_HashTable table );
extern void          hsh_print_stats( hsh_HashTable table, FILE *stream );
extern unsigned long hsh_string_hash( const void *key );
extern unsigned long hsh_pointer_hash( const void *key );
extern int           hsh_string_compare( const void *key1, const void *key2 );
extern int           hsh_pointer_compare( const void *key1, const void *key2 );
extern void          hsh_key_strings(hsh_HashTable);

/* set.c */

typedef void *set_Set;
typedef void *set_Position;

typedef struct set_Stats{
   unsigned long size;		 /* Size of table */
   unsigned long resizings;	 /* Number of resizings */
   unsigned long entries;	 /* Total entries in table */
   unsigned long buckets_used;	 /* Number of hash buckets in use */
   unsigned long singletons;	 /* Number of length one lists */
   unsigned long maximum_length; /* Maximum list length */

   unsigned long retrievals;	 /* Total number of retrievals */
   unsigned long hits;		 /* Number of retrievals from top of a list */
   unsigned long misses;	 /* Number of unsuccessful retrievals */
} *set_Stats;

typedef unsigned long (*set_HashFunction)( const void * );
typedef int           (*set_CompareFunction)( const void *, const void * );

extern set_Set             set_create( set_HashFunction hash,
				       set_CompareFunction compare );
extern set_HashFunction    set_get_hash( set_Set set );
extern set_CompareFunction set_get_compare( set_Set set );
extern void                set_destroy( set_Set set );
extern int                 set_insert( set_Set set, const void *elem );
extern int                 set_delete( set_Set set, const void *elem );
extern int                 set_member( set_Set set, const void *elem );
extern void                set_iterate( set_Set set,
					int (*iterator)( const void *key ) );
extern set_Set             set_union( set_Set set1, set_Set set2 );
extern set_Set             set_inter( set_Set set1, set_Set set2 );
extern set_Set             set_diff( set_Set set1, set_Set set2 );
extern int                 set_equal( set_Set set1, set_Set set2 );
extern set_Stats           set_get_stats( set_Set set );
extern void                set_print_stats( set_Set set, FILE *stream );
extern int                 set_count( set_Set set );
extern set_Position        set_init_position( set_Set set );
extern set_Position        set_next_position( set_Set set,
					      set_Position position );
extern void                *set_get_position( set_Position position );
extern int                 set_readonly( set_Set set, int flag );

#define SET_POSITION_INIT(P,S) ((P)=set_init_position(S))
#define SET_POSITION_NEXT(P,S) ((P)=set_next_position(S,P))
#define SET_POSITION_OK(P)     (P)
#define SET_POSITION_GET(P,E)  ((E)=set_get_position(P))

/* iterate over all entries E in set S */
#define SET_ITERATE(S,P,E)                                                   \
   for (SET_POSITION_INIT((P),(S));                                          \
	SET_POSITION_OK(P) && (SET_POSITION_GET((P),(E)),1);                 \
	SET_POSITION_NEXT((P),(S)))

/* if the SET_ITERATE loop is exited before all element in the set are
   seen, then SET_ITERATE_END should be called.  Calling this function
   after complete loops does no harm.*/
#define SET_ITERATE_END(S) set_readonly(S,0)

/* stack.c */

typedef void *stk_Stack;

extern stk_Stack stk_create( void );
extern void      stk_destroy( stk_Stack stack );
extern void      stk_push( stk_Stack stack, void *datum );
extern void      *stk_pop( stk_Stack stack );
extern void      *stk_top( stk_Stack stack );

/* list.c */

typedef void *lst_List;
typedef void *lst_Position;

extern lst_List     lst_create( void );
extern void         lst_destroy( lst_List list );
extern void         lst_append( lst_List list, const void *datum );
extern void         lst_push( lst_List list, const void *datum );
extern void         *lst_pop( lst_List list );
extern void         *lst_top( lst_List list );
extern void         *lst_nth_get( lst_List list, unsigned int n );
extern void         lst_nth_set( lst_List list, unsigned int n,
				 const void *datum );
extern int          lst_member( lst_List list, const void *datum );
extern unsigned int lst_length( lst_List list );
extern void         lst_iterate( lst_List list,
				 int (*iterator)( const void *datum ) );
extern void         lst_truncate( lst_List list, unsigned int length );
extern void         lst_truncate_position( lst_List list,
					   lst_Position position );
extern lst_Position lst_init_position( lst_List list );
extern lst_Position lst_last_position( lst_List list );
extern lst_Position lst_next_position( lst_Position position );
extern lst_Position lst_nth_position( lst_List list, unsigned int n );
extern void         *lst_get_position( lst_Position position );
extern void         lst_set_position( lst_Position position,
				      const void *datum );
extern void         _lst_shutdown( void );

#define LST_POSITION_INIT(P,L) ((P)=lst_init_position(L))
#define LST_POSITION_NEXT(P)   ((P)=lst_next_position(P))
#define LST_POSITION_OK(P)     (P)
#define LST_POSITION_GET(P,E)  ((E)=lst_get_position(P))

/* iterate over all entries E in list L */
#define LST_ITERATE(L,P,E)                                                  \
   for (LST_POSITION_INIT((P),(L));                                         \
        LST_POSITION_OK(P) && (LST_POSITION_GET((P),(E)),1);                \
        LST_POSITION_NEXT(P))

/* iterate over all entries in lists L1 and L2 */
#define LST_ITERATE2(L1,L2,P1,P2,E1,E2)                                      \
   for (LST_POSITION_INIT((P1),(L1)), LST_POSITION_INIT((P2),(L2));          \
	LST_POSITION_OK(P1) && LST_POSITION_OK(P2)                           \
	   && (LST_POSITION_GET((P1),(E1)),LST_POSITION_GET((P2),(E2)),1);   \
	LST_POSITION_NEXT(P1), LST_POSITION_NEXT(P2))

/* error.c */

extern void       err_set_program_name( const char *programName );
extern const char *err_program_name( void );
extern void       err_fatal( const char *routine, const char *format, ... )
   __attribute__((noreturn,format(printf, 2, 3)));
extern void       err_fatal_errno( const char *routine,
				   const char *format, ... )
   __attribute__((noreturn,format(printf, 2, 3)));

extern void       err_warning( const char *routine, const char *format, ... )
   __attribute__((format(printf, 2, 3)));

extern void       err_internal( const char *routine, const char *format, ... )
   __attribute__((noreturn,format(printf, 2, 3)));

/* memory.c */

typedef void *mem_String;
typedef void *mem_Object;

typedef struct mem_StringStats {
   int count;			/* Number of strings or objects */
   int bytes;			/* Number of bytes allocated */
} *mem_StringStats;

typedef struct mem_ObjectStats {
   int total;			/* Total objects requested */
   int used;			/* Total currently in use */
   int reused;			/* Total reused  */
   int size;			/* Size of each object */
} *mem_ObjectStats;

extern mem_String      mem_create_strings( void );
extern void            mem_destroy_strings( mem_String info );
extern const char      *mem_strcpy( mem_String info, const char *string );
extern const char      *mem_strncpy( mem_String info,
				     const char *string,
				     int len );
extern void            mem_grow( mem_String info,
				 const char *string,
				 int len );
extern const char      *mem_finish( mem_String info );
extern mem_StringStats mem_get_string_stats( mem_String info );
extern void            mem_print_string_stats( mem_String info, FILE *stream );

extern mem_Object      mem_create_objects( int size );
extern void            mem_destroy_objects( mem_Object info );
extern void            *mem_get_object( mem_Object info );
extern void            mem_free_object( mem_Object info, void *obj );
extern mem_ObjectStats mem_get_object_stats( mem_Object info );
extern void            mem_print_object_stats( mem_Object info, FILE *stream );

/* string.c */

typedef void *str_Pool;

typedef struct str_Stats {
   int count;			/* Number of strings created */
   int bytes;			/* Number of bytes allocated */
   int retrievals;		/* Total number of retrievals */
   int hits;			/* Number of retrievals from top of a list */
   int misses;			/* Number of unsuccessful retrievals */
} *str_Stats;

extern str_Pool   str_pool_create( void );
extern void       str_pool_destroy( str_Pool pool );
extern const char *str_pool_find( str_Pool pool, const char *s );
extern void       str_pool_grow( str_Pool pool, const char *s, int length );
extern const char *str_pool_finish( str_Pool pool );
extern str_Stats  str_pool_get_stats( str_Pool pool );
extern void       str_pool_print_stats( str_Pool pool, FILE *stream );

extern const char *str_find( const char *s );
extern const char *str_findn( const char *s, int length );
extern void       str_grow( const char *s, int length );
extern const char *str_finish( void );
extern const char *str_unique( const char *prefix );
extern void       str_destroy( void );
extern str_Stats  str_get_stats( void );
extern void       str_print_stats( FILE *stream );

/* debug.c */

typedef unsigned long int dbg_Type;

extern void           dbg_register( dbg_Type flag, const char *name );
extern void           _dbg_register( dbg_Type flag, const char *name );
extern void           dbg_destroy( void );
extern void           dbg_set( const char *name );
extern __inline__ int dbg_test( dbg_Type flag );
extern void           dbg_list( FILE *stream );

#define PRINTF(flag,arg) if (dbg_test(flag)) printf arg

/* timer.c */

extern void   tim_start( const char *name );
extern void   tim_stop( const char *name );
extern void   tim_reset( const char *name );
extern double tim_get_real( const char *name );
extern double tim_get_user( const char *name );
extern double tim_get_system( const char *name );
extern void   tim_print_timer( FILE *str, const char *name );
extern void   tim_print_timers( FILE *str );
extern void   _tim_shutdown( void );

/* arg.c */

typedef void *arg_List;

extern arg_List   arg_create( void );
extern void       arg_destroy( arg_List arg );
extern arg_List   arg_add( arg_List arg, const char *string );
extern arg_List   arg_addn( arg_List arg, const char *string, int length );
extern void       arg_grow( arg_List arg, const char *string, int length );
extern arg_List   arg_finish( arg_List arg );
extern const char *arg_get( arg_List arg, int item );
extern void       arg_get_vector( arg_List arg, int *argc, char ***argv );
extern arg_List   arg_argify( const char *string );

/* pr.c */

#define PR_USE_STDIN        0x00000001
#define PR_USE_STDOUT       0x00000002
#define PR_USE_STDERR       0x00000004
#define PR_CREATE_STDIN     0x00000010
#define PR_CREATE_STDOUT    0x00000020
#define PR_CREATE_STDERR    0x00000040
#define PR_STDERR_TO_STDOUT 0x00000100

extern int  pr_open( const char *command, int flags,
		     FILE **instr, FILE **outstr, FILE **errstr );
extern int  pr_close( FILE *str );
extern void _pr_shutdown( void );

/* sl.c */

typedef void *sl_List;
typedef int (*sl_Iterator)( const void *datum );

extern sl_List    sl_create( int (*compare)( const void *key1,
					     const void *key2 ),
			     const void *(*key)( const void *datum ),
			     const char *(*print)( const void *datum ) );
extern void       sl_destroy( sl_List list );
extern void       _sl_shutdown( void );
extern void       sl_insert( sl_List list, const void *datum );
extern void       sl_delete( sl_List list, const void *datum );
extern const void *sl_find( sl_List list, const void *key );
extern int         sl_iterate( sl_List list, sl_Iterator );
extern void       _sl_dump( sl_List list );

#endif
