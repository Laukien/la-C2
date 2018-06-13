/*
 * =====================================================================================
 *
 *       Filename:  atomic.h
 *    Description:  synchronize code
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#ifndef LA_ATOMIC_H
#define LA_ATOMIC_H

typedef struct la_atomic atomic_t;

atomic_t *atomic_init();
void atomic_free(atomic_t *self);
void atomic_lock(atomic_t *self);
void atomic_unlock(atomic_t *self);

#endif
