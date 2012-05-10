#ifndef _LOGGING_H_
#define _LOGGING_H_

#include <syslog.h>

#include "utils.h"

#define INIT_LOG openlog("metarb", LOG_CONS, 0);
#define FINI_LOG closelog();

/* TODO to define the macro's as inline functions? */


#define WHERESTR  "[%s:%s:%d]"
#define WHEREARG  __FILE__, __FUNCTION__, __LINE__


// To trace in very detailed level. Usually will appear many times in
//    every function
#define LOG_T							\
	syslog(LOG_DEBUG,					\
	       "[T] " WHERESTR "\n",				\
	       WHEREARG)


// we use the tricks of Variadic Macros below, see
// http://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html

// Debug log. Put in Ad hoc way
#define LOG_D(_fmt, args...)					\
	syslog(LOG_DEBUG,					\
	       "[D] " _fmt WHERESTR "\n" , ##args, WHEREARG)


// informative log. Show to user
#define LOG_I(_fmt, args...)					 \
	do {							 \
		syslog(LOG_INFO,				 \
		       "[I] " _fmt "\n" , ##args);		 \
		fprintf(stderr,					 \
			"[I] " _fmt "\n" , ##args);		 \
	} while(0)



// Error
#define LOG_E(_fmt, args...)						\
	do {								\
		syslog(LOG_ERR,						\
		       "[E] " _fmt WHERESTR "\n" , ##args, WHEREARG);	\
		fprintf(stderr,						\
			"[E] " _fmt WHERESTR "\n" , ##args, WHEREARG);	\
	} while(0)



/* Critical Error */
#define LOG_C(_fmt, args...)						\
	do {								\
		syslog(LOG_CRIT,					\
		       "[!] " _fmt WHERESTR "\n" , ##args, WHEREARG);	\
		fprintf(stderr,						\
			"[!] " _fmt WHERESTR "\n" , ##args, WHEREARG);	\
	} while(0)



#endif	/* _LOGGING_H_ */
