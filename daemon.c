#include <stdio.h>
#include <stdbool.h>
#include <libutil.h>
#include <syslog.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <paths.h>
#include <unistd.h>


static struct pidfh *pfh = NULL;
static volatile sig_atomic_t running = true;


static void signalHandler( int sig __unused )
{
   running = false;
}


int main( void )
{
pid_t otherpid;
int rv = EXIT_FAILURE;

   openlog( getprogname(), LOG_PID | LOG_NDELAY, LOG_DAEMON );

   if( signal( SIGTERM, signalHandler ) == SIG_ERR )
   {
      syslog( LOG_ERR, "Failed to install the signal handler : %s", strerror( errno ) );
      goto cleanup;
   }

   if( ( pfh = pidfile_open( NULL, 0600, &otherpid ) ) == NULL )
   {
      if( errno == EEXIST )
      {
         syslog( LOG_ERR, "Daemon already running (pid %d)", otherpid );
      }
      else
      {
         syslog( LOG_ERR, "Failed to create PID file (%s%s.pid): %s", _PATH_VARRUN, getprogname(), strerror( errno ) );
      }
      goto cleanup;
   }

   if( daemon( 0, 0 ) == -1 )
   {
      syslog( LOG_ERR, "Failed to daemonize: %s", strerror( errno ) );
      goto cleanup;
   }

   pidfile_write( pfh );
   syslog( LOG_NOTICE, "Daemon started successfully" );

   rv = EXIT_SUCCESS;

   while( running )
   {
      // real work here
   }

   syslog( LOG_NOTICE, "Shutting down gracefully" );


cleanup:
   if( pfh != NULL )
   {
      pidfile_remove( pfh );
   }
   closelog();
   return rv;
}
