/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>     */
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License"); 	*/
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at 				*/
/*  									*/
/*  http://www.apache.org/licenses/LICENSE-2.0 				*/
/*  									*/
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 	*/
/* implied. 								*/
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License. 					*/
/* -------------------------------------------------------------------- */
#ifndef _restthread_c_
#define _restthread_c_

#include "restthread.h"

private	pthread_mutex_t restThreadLock=PTHREAD_MUTEX_INITIALIZER;
private	struct	rest_thread * firstThread=(struct rest_thread *) 0;
private	struct	rest_thread * lastThread=(struct rest_thread *) 0;
private	struct	rest_server * rootServer=(struct rest_server *) 0;

/*	-----------------------------------------------------------	*/
/*		   l o c k _ t h r e a d _ c o n t r o l		*/
/*	-----------------------------------------------------------	*/
private	void	lock_rest_thread(struct rest_thread * tptr)
{
	pthread_mutex_lock( &tptr->lock );
	return;
}

/*	-----------------------------------------------------------	*/
/*		u n l o c k _ t h r e a d _ c o n t r o l		*/
/*	-----------------------------------------------------------	*/
private	void	unlock_rest_thread(struct rest_thread * tptr)
{
	pthread_mutex_unlock( &tptr->lock );
	return;
}

/*	-----------------------------------------------------------	*/
/*		   i n s e r t _ r e s t _ t h r e a d			*/
/*	-----------------------------------------------------------	*/
private	struct rest_thread * insert_rest_thread( struct rest_thread * tptr )
{
	if (!( rootServer ))
		return( tptr );

	if (!( rootServer->method.thread ))
		return( tptr );

	lock_rest_thread(tptr);
	(void) (*rootServer->method.thread)(1,tptr);
	unlock_rest_thread(tptr);
	return( tptr );
}

/*	-----------------------------------------------------------	*/
/*		   u p d a t e _ r e s t _ t h r e a d			*/
/*	-----------------------------------------------------------	*/
private	struct rest_thread * update_rest_thread( struct rest_thread * tptr, int lock )
{
	if (!( rootServer ))
		return( tptr );

	if (!( rootServer->method.thread ))
		return( tptr );

	if ( lock )
		lock_rest_thread(tptr);

	(void) (*rootServer->method.thread)(3,tptr);

	if ( lock )
		unlock_rest_thread(tptr);

	return( tptr );
}

/*	-----------------------------------------------------------	*/
/*		   d e l e t e _ r e s t _ t h r e a d			*/
/*	-----------------------------------------------------------	*/
private	struct rest_thread * delete_rest_thread( struct rest_thread * tptr )
{
	if (!( rootServer ))
		return( tptr );

	if (!( rootServer->method.thread ))
		return( tptr );

	lock_rest_thread(tptr);
	(void) (*rootServer->method.thread)(4,tptr);
	unlock_rest_thread(tptr);
	return( tptr );
}

/*	-----------------------------------------------------------	*/
/*		s t a r t _ r e s t _ t h r e a d			*/
/*	-----------------------------------------------------------	*/
/*	called from the thread launcher to allow the target thread	*/
/*	to proceed with processing.					*/
/*	-----------------------------------------------------------	*/
private	void	start_rest_thread(struct rest_thread * tptr)
{
	int	item;
	lock_rest_thread(tptr);
	item = tptr->item;
	tptr->item = ((item + 1) & 1);
	unlock_rest_thread(tptr);
	if ( item )
	{
		pthread_mutex_lock( &tptr->controlZero );
		pthread_mutex_unlock( &tptr->controlOne );
	}
	else
	{
		pthread_mutex_lock( &tptr->controlOne );
		pthread_mutex_unlock( &tptr->controlZero );
	}
	return;
}

/*	-----------------------------------------------------------	*/
/*		w a i t _ r e s t _ t h r e a d 			*/
/*	-----------------------------------------------------------	*/
/*	called from the starting thread to await its order to start	*/
/*	the thread will try to lock the control mutex and when this	*/
/*	is not possible will sleep till it regains control.		*/
/*	-----------------------------------------------------------	*/
private	void	wait_rest_thread(struct rest_thread * tptr)
{
	int	item;
	lock_rest_thread(tptr);
	item = tptr->item;
	tptr->status = _THREAD_IDLE;
	tptr->completed = time((long *) 0);
	unlock_rest_thread(tptr);
	if ( item )
	{
		pthread_mutex_lock( &tptr->controlOne );
		pthread_mutex_unlock( &tptr->controlOne );
	}
	else
	{
		pthread_mutex_lock( &tptr->controlZero );
		pthread_mutex_unlock( &tptr->controlZero );
	}
	lock_rest_thread(tptr);
	tptr->started = time((long *) 0);
	tptr->activity++;
	tptr->status = _THREAD_WORKING;
	unlock_rest_thread(tptr);
	return;
}

/*	----------------------------------------	*/
/*	l i b e r a t e _ r e s t _ t h r e a d 	*/
/*	----------------------------------------	*/
public struct rest_thread * liberate_rest_thread(struct rest_thread * sptr)
{
	if ( sptr )
	{
		if ( sptr->reqid )
			sptr->reqid = liberate( sptr->reqid );
		sptr = liberate( sptr );
		pthread_mutex_unlock( &sptr->lock );
		pthread_mutex_unlock( &sptr->controlOne );
		pthread_mutex_unlock( &sptr->controlZero );
	}
	return((struct rest_thread *) 0);
}

/*	----------------------------------	*/
/*	r e s e t _ r e s t _ t h r e a d 	*/
/*	----------------------------------	*/
public struct rest_thread * reset_rest_thread(struct rest_thread * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct rest_thread*) 0;
		sptr->next = (struct rest_thread*) 0;
		sptr->id =  (pthread_t) 0;
		sptr->reqid = (char *) 0;
		sptr->response = (char *) 0;
		memset(&sptr->lock,0,sizeof( sptr->lock));
		sptr->client = (struct rest_client *) 0;
		sptr->request = (struct rest_request *) 0;
		sptr->item   = 1;
		sptr->status = _THREAD_IDLE;
		sptr->started = 0;
		sptr->completed = 0;
		sptr->activity= 0;
		sptr->pid = getpid();
		sptr->ppid = getppid();
		pthread_mutex_lock( &sptr->controlOne );
	}
	return(sptr);

}

/*	----------------------------------------	*/
/*	a l l o c a t e _ r e s t _ t h r e a d 	*/
/*	----------------------------------------	*/
public struct rest_thread * allocate_rest_thread()
{
	struct rest_thread * sptr;
	if (!( sptr = allocate( sizeof( struct rest_thread ) ) ))
		return( sptr );
	else if (!( sptr = reset_rest_thread( sptr ) ))
		return( sptr );
	else	return( insert_rest_thread(sptr) );
}

/*	---------------------------------------------------------	*/
/*	t e r m i n a t e _ r e s t _ t h r e a d _ m a n a g e r 	*/
/*	---------------------------------------------------------	*/
public	int	terminate_rest_thread_manager()
{
	struct	rest_thread * tptr;
	if ( rootServer )
	{
		pthread_mutex_lock( &restThreadLock );
		for (	tptr = firstThread;
			tptr != (struct rest_thread *) 0;
			tptr = tptr->next )
			delete_rest_thread( tptr );
		pthread_mutex_unlock( &restThreadLock );
	}
	rootServer = (struct rest_server *) 0;
	return(0);
}

/*	-----------------------------------------------------------	*/
/*	i n i t i a l i s e _ r e s t _ t h r e a d _ m a n a g e r 	*/
/*	-----------------------------------------------------------	*/
public	int	initialise_rest_thread_manager(struct rest_server * sptr)
{
	rootServer = sptr;	
	firstThread = lastThread = (struct rest_thread *) 0;
	return( 0 );
}

/*	-----------------------------------------------------------	*/
/*	    r e s t _ s t a r t _ r e s p o n s e _ t h r e a d		*/
/*	-----------------------------------------------------------	*/
public	struct rest_thread * rest_start_thread
		(
		struct rest_client * cptr, 
		struct rest_request * rptr
		)
{
	int	status;
	struct	rest_thread * tptr;
	struct	rest_client * wptr;
	/* ------------------------------- */
	/* attempt to locate a free thread */
	/* ------------------------------- */
	pthread_mutex_lock( &restThreadLock );
	for (	tptr=firstThread; 
		tptr != (struct rest_thread *) 0;
		tptr = tptr->next )
	{
		lock_rest_thread( tptr );
		wptr = tptr->client;
		unlock_rest_thread( tptr );

		if (!( wptr ))
			break;
		else	continue;
	}
	pthread_mutex_unlock( &restThreadLock );

	/* -------------------------------- */
	/* allocate and launch a new thread */
	/* -------------------------------- */
	if (!( tptr ))
	{
		pthread_mutex_lock( &restThreadLock );

		/* --------------------- */
		/* allocate a new thread */
		/* --------------------- */
		if (!( tptr = allocate_rest_thread() ))
			return( tptr );
		else	tptr->status = _THREAD_IDLE;

		pthread_attr_init( &tptr->attributes);

		/* --------------------- */
		/* launch the new thread */
		/* --------------------- */
		if ((status = pthread_create(
				&tptr->id,
				&tptr->attributes,
				rest_thread_message,
				(void *) tptr)) > 0 )
		{
			pthread_mutex_unlock( &restThreadLock );
			return( liberate_rest_thread( tptr ) );
		}
		else
		{
			/* --------------------- */
			/* manage the new thread */
			/* --------------------- */
			if (!( tptr->previous = lastThread ))
				firstThread = tptr;
			else 	tptr->previous->next = tptr;
			lastThread = tptr;
			pthread_mutex_unlock( &restThreadLock );
		}
	}

	/* ------------------------------ */
	/* prepare the job for the thread */
	/* ------------------------------ */
	rest_detach_client( cptr );

	lock_rest_thread( tptr );

	tptr->client = cptr;
	tptr->request = rptr;
	update_rest_thread( tptr,0 );

	unlock_rest_thread( tptr );

	start_rest_thread( tptr );

	return( tptr );

}

#endif	/* _restthread_c_ */
	/* -------------- */

