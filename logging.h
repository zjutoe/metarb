#ifndef _LOGGING_H_
#define _LOGGING_H_

#include <syslog.h>

#include "utils.h"

#define INIT_LOG openlog("metarb", LOG_CONS, 0);
#define FINI_LOG closelog();

/* FIXME to allow more parameters from user */

/* To trace in very detailed level. Usually will appear many times in
   every function */


#define WHERESTR  " %s:%s:%d"
#define WHEREARG  __FILE__, __FUNCTION__, __LINE__


#define LOG_T							\
	syslog(LOG_DEBUG,					\
	       "[T] " WHERESTR "\n",				\
	       WHEREARG);


/* Debug log. Put in Ad hoc way */
#define LOG_D(_fmt, ...)						\
	syslog(LOG_DEBUG,						\
	       "[D] " _fmt WHERESTR "\n",				\
	       __VA_ARGS__, WHEREARG)



/* informative log. Show to user */
#define LOG_I(_fmt, ...)					 \
	do {							 \
		syslog(LOG_INFO, "[I] " _fmt "\n", __VA_ARGS__); \
		printf("[I] " _fmt "\n", __VA_ARGS__);		 \
	} while(0)


/* Error */
#define LOG_E(_fmt, ...)						\
	do {								\
		syslog(3, "[E] " _fmt WHERESTR "\n", __VA_ARGS__, WHEREARG); \
		printf( "[E] " _fmt WHERESTR "\n", __VA_ARGS__, WHEREARG); \
	} while(0)



/* Critical Error */
#define LOG_C(_fmt, ...)						\
	do {								\
		syslog(2,"[!] " _fmt WHERESTR "\n", __VA_ARGS__, WHEREARG); \
		printf(  "[!] " _fmt WHERESTR "\n", __VA_ARGS__, WHEREARG); \
	} while(0)



/* Well, sounds like this function below should not be here. But
   before we have a more formal error handling system let's just put
   it here. */

// #define DIE_IF(cond, _fmg, ...)				\
// 	do{						\
// 		if(unlikely(cond)){			\
// 			LOG_C(msg);			\
// 			exit(1);			\
// 		}					\
// 	}while(0)



#endif	/* _LOGGING_H_ */
