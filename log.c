/* log.c -- Logging routines, for a single, program-wide logging facility
 * Created: Mon Mar 10 09:37:21 1997 by faith@cs.unc.edu
 * Revised: Tue Mar 11 15:58:25 1997 by faith@cs.unc.edu
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
 * $Id: log.c,v 1.3 1997/03/12 01:11:30 faith Exp $
 * 
 */

#include "maaP.h"
#include <syslog.h>
#include <fcntl.h>

#if HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 64
#endif

static int        logFd = -1;
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

void log_file( const char *ident, const char *filename )
{
   if (logFd >= 0)
      err_internal( __FUNCTION__,
		    "Log file \"%s\" open when trying to open \"%s\"\n",
		    logFilename, filename );

   if ((logFd = open( filename, O_WRONLY|O_CREAT|O_APPEND, 0644 )) < 0)
      err_fatal_errno( __FUNCTION__,
		       "Cannot open \"%s\" for append\n", filename );

   logIdent    = str_find( ident );
   logFilename = str_find( filename );
   
   _log_set_hostname();
   ++logOpen;
}

void log_stream( const char *ident, FILE *stream )
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
   if (logFd >= 0)    close( logFd );
   if (logUserStream) fclose( logUserStream );
   if (logSyslog)     closelog();

   logOpen       = 0;
   logFd         = -1;
   logUserStream = NULL;
   logSyslog     = 0;
}

void log_error_va( const char *routine, const char *format, va_list ap )
{
   time_t t;
   char   buf[4096];
   char   *pt;
   
   if (!logOpen) return;
   
   time(&t);
   
   if (logFd >= 0 || logUserStream) {
      sprintf( buf,
	       "%24.24s %s %s[%d]: ",
	       ctime(&t),
	       logHostname,
	       logIdent,
	       getpid() );
      pt = buf + strlen( buf );
      if (routine) sprintf( pt, "(%s) ", routine );
      pt = buf + strlen( buf );
      vsprintf( pt, format, ap );
      
      if (logFd >= 0) write( logFd, buf, strlen(buf) );
      if (logUserStream) {
         fseek( logUserStream, 0L, SEEK_END ); /* might help if luser didn't
                                                  open stream with "a" */
         fprintf( logUserStream, "%s", buf );
         fflush( logUserStream );
      }
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
   char   buf[4096];
   char   *pt;
   
   if (!logOpen) return;
   
   time(&t);
   
   if (logFd >= 0 || logUserStream) {
      sprintf( buf,
	       "%24.24s %s %s[%d]: ",
	       ctime(&t),
	       logHostname,
	       logIdent,
	       getpid() );
      pt = buf + strlen( buf );
      vsprintf( pt, format, ap );
      
      if (logFd >= 0) write( logFd, buf, strlen(buf) );
      if (logUserStream) {
         fseek( logUserStream, 0L, SEEK_END ); /* might help if luser didn't
                                                  open stream with "a" */
         fprintf( logUserStream, "%s", buf );
         fflush( logUserStream );
      }
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