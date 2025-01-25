#pragma once

#include_next <pthread.h>

#ifdef __cplusplus
#include <e32cmn.h>

#ifdef __SERIES60_3X__
struct pthread_rwlockattr;
typedef struct pthread_rwlockattr *pthread_rwlockattr_t;

//struct pthread_rwlock {
//    RReadWriteLock lock;
//};
typedef RReadWriteLock *pthread_rwlock_t;

inline int pthread_rwlock_init(pthread_rwlock_t *rwlock, const pthread_rwlockattr_t *attr) {
    RReadWriteLock *lock = *rwlock = new RReadWriteLock();
    lock->CreateLocal();
    return 0;
}

inline int pthread_rwlock_destroy(pthread_rwlock_t *rwlock) {
    delete *rwlock;
    return 0;
}

inline int pthread_rwlock_unlock(pthread_rwlock_t *rwlock) {
    if (rwlock == NULL) {
        return 1;
    }

    (*rwlock)->Unlock();

    return 0;
}

inline int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock) {
    if (rwlock == NULL) {
        return 1;
    }

    (*rwlock)->WriteLock();

    return 0;
}

inline int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock) {
    if (rwlock == NULL) {
        return 1;
    }

    (*rwlock)->TryWriteLock();

    return 0;
}

inline int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock) {
    if (rwlock == NULL) {
        return 1;
    }

    (*rwlock)->ReadLock();

    return 0;
}

inline int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock) {
    if (rwlock == NULL) {
        return 1;
    }

    (*rwlock)->TryReadLock();

    return 0;
}
#endif

inline int pthread_setname_np(pthread_t thread, const char *name) {
    (void)thread;
    (void)name;
    return 1;
}
#endif
