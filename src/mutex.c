/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 11:06:10 by llefranc          #+#    #+#             */
/*   Updated: 2023/06/21 11:36:58 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mutex.h"

#include <pthread.h>

pthread_once_t once = PTHREAD_ONCE_INIT;
pthread_mutex_t mutex;
pthread_mutexattr_t attr;

/**
 * Init a mutex as a recursive mutex.
*/
static void mutex_init(void)
{
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&mutex, &attr);
}

/**
 * Lock a recursive mutex. Also perform the mutex initialization if this
 * function is called for the first time.
*/
void mutex_lock(void)
{
	pthread_once(&once, &mutex_init);
	pthread_mutex_lock(&mutex);
}

/**
 * Unlock a recursive mutex.
*/
void mutex_unlock(void)
{
	pthread_mutex_unlock(&mutex);
}