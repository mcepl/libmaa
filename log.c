/* log.c -- Logging routines, for a single, program-wide logging facility
 * Created: Mon Mar 10 09:37:21 1997 by faith@cs.unc.edu
 * Revised: Mon Mar 10 21:21:34 1997 by faith@cs.unc.edu
 * Copyright 1997 Rickard E. Faith (faith@cs.unc.edu)
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
 * 
 * $Id: log.c,v 1.2 1997/03/11 04:27:33 faith Exp $
 * 
 */

#include "maaP.h"
#include <syslog.h>

#if HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 64
#endif

static FILE       *logStream;
static FILE       *logUserStream;
static int        logSyslog;

static int        logOpen;

static const char *logIdent;
static const char *logFilename;
static char       logHostname[MAXHOSTNAMELEN];

static void _log_set_hostname( void )
{
   static int hostnameSet = 0;
   char       *pt;

   if (!hostnameSet) {
      memset( logHostname, 0, sizeof(logHostname) );
      gethostname( logHostname, sizeof(logHostname)-1 );
      if ((pt = strchr(logHostname, '.'))) *pt = '\0';
      ++hostnameSet;
   }
}

void log_syslog( const char *ident, int daemon )
{
   if (logSyslog)
      err_internal( __FUNCTION__, "Syslog facility already open\n" );
   
   openlog( ident, LOG_PID|LOG_NOWAIT, daemon ? LOG_DAEMON : LOG_USER );
   ++logOpen;
   ++logSyslog;
}

void log_file( const char *filename, const char *ident )
{
   if (logStream)
      err_internal( __FUNCTION__,
		    "Log file \"%s\" open when trying to open \"%s\"\n",
		    logFilename, filename );

   if (!(logStream = fopen( filename, "a" )))
      err_fatal_errno( __FUNCTION__,
		       "Cannot open \"%s\" for append\n", filename );

   logIdent    = str_find( ident );
   logFilename = str_find( filename );
   
   _log_set_hostname();
   ++logOpen;
}

void log_stream( FILE *stream, const char *ident )
{
   if (logUserStream)
      err_internal( __FUNCTION__, "User stream already open\n" );

   logUserStream = stream;
   logIdent      = str_find( ident );

   _log_set_hostname();
   ++logOpen;
}

void log_close( void )
{
   if (logStream)     fclose( logStream );
   if (logUserStream) fclose( logUserStream );
   if (logSyslog)     closelog();

   logOpen       = 0;
   logStream     = NULL;
   logUserStream = NULL;
   logSyslog     = 0;
}

void log_error_va( const char *routine, const char *format, va_list ap )
{
   time_t t;
   
   if (!logOpen) return;
   
   time(&t);
   
   if (logStream) {
      fprintf( logStream,
	       "%24.24s %s %s[%d]: ",
	       ctime(&t),
	       logHostname,
	       logIdent,
	       getpid() );
      if (routine) fprintf( logStream, "(%s) ", routine );
      vfprintf( logStream, format, ap );
   }

   if (logUserStream) {
      fseek( logUserStream, 0L, SEEK_END ); /* might help if luser didn't
                                               open stream with "a" */
      fprintf( logUserStream,
	       "%24.24s %s %s[%d]: ",
	       ctime(&t),
	       logHostname,
	       logIdent,
	       getpid() );
      if (routine) fprintf( logUserStream, "(%s) ", routine );
      vfprintf( logUserStream, format, ap );
      fflush( logUserStream );
   }
   
   if (logSyslog) {
      vsyslog( LOG_ERR, format, ap );
   }
}

void log_error( const char *routine, const char *format, ... )
{
   va_list ap;

   va_start( ap, format );
   log_error_va( routine, format, ap );
   va_end( ap );
}

void log_info_va( const char *format, va_list ap )
{
   time_t t;
   
   if (!logOpen) return;
   
   time(&t);
   
   if (logStream) {
      fprintf( logStream,
	       "%24.24s %s %s[%d]: ",
	       ctime(&t),
	       logHostname,
	       logIdent,
	       getpid() );
      vfprintf( logStream, format, ap );
   }

   if (logUserStream) {
      fseek( logUserStream, 0L, SEEK_END ); /* might help if luser didn't
                                               open stream with "a" */
      fprintf( logUserStream,
	       "%24.24s %s %s[%d]: ",
	       ctime(&t),
	       logHostname,
	       logIdent,
	       getpid() );
      vfprintf( logUserStream, format, ap );
      fflush( logUserStream );
   }
   
   if (logSyslog) {
      vsyslog( LOG_INFO, format, ap );
   }
}

void log_info( const char *format, ... )
{
   va_list ap;

   va_start( ap, format );
   log_info_va( format, ap );
   va_end( ap );
}
