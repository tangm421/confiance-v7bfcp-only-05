#ifndef _BFCP_THREADS_
#define _BFCP_THREADS_

#ifndef _ASTERISK_LOCK_H
#include <pthread.h>
typedef pthread_mutex_t bfcp_mutex_t;
#define bfcp_mutex_init(a,b) pthread_mutex_init(a,b)
#define bfcp_mutex_destroy(a) pthread_mutex_destroy(a)
#define bfcp_mutex_lock(a) pthread_mutex_lock(a)
#define bfcp_mutex_unlock(a) pthread_mutex_unlock(a)
#else
#include "asterisk/lock.h"
typedef ast_mutex_t bfcp_mutex_t;
#define bfcp_mutex_init(a,b) ast_mutex_init(a)
#define bfcp_mutex_destroy(a) ast_mutex_destroy(a)
#define bfcp_mutex_lock(a) ast_mutex_lock(a)
#define bfcp_mutex_unlock(a) ast_mutex_unlock(a)
#endif

#endif
