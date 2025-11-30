# FreeBSD-daemon-boilerplate
A minimal, rock-solid, FreeBSD-first daemon skeleton

Reference-quality example of how to write a proper FreeBSD daemon:
• Correct and race-free PID file handling with libutil  
• Proper syslog usage (LOG_ERR, LOG_NOTICE, LOG_NDELAY)  
• Safe signal handling with sig_atomic_t  
• Single exit point, fail-closed design  
• 100 % FreeBSD-idiomatic style (matches base system daemons)

Perfect starting point for any long-running service on FreeBSD.
