#ifndef _LOGGING_H_
#define _LOGGING_H_

#include <syslog.h>

#include "utils.h"

#define INIT_LOG openlog("metarb", 0, 0);
#define FINI_LOG closelog();

/* FIXME to allow more parameters from user */

/* To trace in very detailed level. Usually will appear many times in
   every function */

#define LOG_T       syslog(LOG_DEBUG,				\
			   "[T] %s:%s:%d\n",			\
			   __FILE__, __FUNCTION__, __LINE__);


/* Debug log. Put in Ad hoc way */

#define LOG_D(msg)  syslog(LOG_DEBUG,					\
			   "[D] %s  %s:%s:%d\n",			\
			   msg, __FILE__, __FUNCTION__, __LINE__)


/* informative log. Show to user */

#define LOG_I(msg)				   \
	do {					   \
		syslog(LOG_INFO, "[I] %s\n", msg); \
		printf("[I] %s\n", msg);	   \
	} while(0)


/* Error */

#define LOG_E(msg)							\
	do {								\
		syslog(LOG_ERR, "[E] %s %s\n", msg, __FUNCTION__);	\
		printf(         "[E] %s %s\n", msg, __FUNCTION__);	\
	} while(0)


/* Critical Error */

#define LOG_C(msg)							\
	do {								\
		syslog(LOG_CRIT,   "[!] %s %s\n", msg, __FUNCTION__);	\
		printf(            "[!] %s %s\n", msg, __FUNCTION__);	\
	} while(0)



/* Well, sounds like this function below should not be here. But
   before we have a more formal error handling system let's just put
   it here. */

#define DIE_IF(cond, msg)				\
	do{						\
		if(unlikely(cond)){			\
			LOG_C(msg);			\
			exit(1);			\
		}					\
	}while(0)



#endif	/* _LOGGING_H_ */
