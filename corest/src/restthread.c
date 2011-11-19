/* ------------------------------------------------------------------------------------	*/
/*				 CompatibleOne Cloudware				*/
/* ------------------------------------------------------------------------------------ */
/*											*/
/* Ce fichier fait partie de ce(tte) oeuvre de Iain James Marshall et est mise a 	*/
/* disposition selon les termes de la licence Creative Commons Paternit‚ : 		*/
/*											*/
/*			 	Pas d'Utilisation Commerciale 				*/
/*				Pas de Modification 					*/
/*				3.0 non transcrit.					*/
/*											*/
/* ------------------------------------------------------------------------------------ */
/* 			Copyright (c) 2011 Iain James Marshall for Prologue 		*/
/*				   All rights reserved					*/
/* ------------------------------------------------------------------------------------ */
#ifndef _restthread_c_
#define _restthread_c_

#include "restthread.h"

private	struct	rest_thread * firstThread=(struct rest_thread *) 0;
private	struct	rest_thread * lastThread=(struct rest_thread *) 0;

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
/*		w a i t _ r e s t _ t h r e a d 			*/
/*	-----------------------------------------------------------	*/
/*	called from the starting thread to await its order to start	*/
/*	the thread will try to lock the control mutex and when this	*/
/*	is not possible will sleep till it regains control.		*/
/*	-----------------------------------------------------------	*/

private	void	wait_rest_thread(struct rest_thread * tptr)
{
	pthread_mutex_lock( &tptr->control );
	lock_rest_thread(tptr);
	tptr->started=1;
	unlock_rest_thread(tptr);
	pthread_mutex_unlock( &tptr->control );
	return;
}

/*	-----------------------------------------------------------	*/
/*		s t a r t _ r e s t _ t h r e a d			*/
/*	-----------------------------------------------------------	*/
/*	called from the thread launcher to allow the target thread	*/
/*	to proceed with processing.					*/
/*	-----------------------------------------------------------	*/
private	void	start_rest_thread(struct rest_thread * tptr)
{
	int	started;
	lock_rest_thread(tptr);
	tptr->started=0;
	unlock_rest_thread(tptr);
	pthread_mutex_unlock( &tptr->control );
	do	{
		lock_rest_thread(tptr);
		started = tptr->started;
		unlock_rest_thread(tptr);
		}
	while (!( started ));
	pthread_mutex_lock( &tptr->control );
	return;
}

/*	----------------------------------------	*/
/*	l i b e r a t e _ r e s t _ t h r e a d 	*/
/*	----------------------------------------	*/
public struct rest_thread * liberate_rest_thread(struct rest_thread * sptr)
{
	if ( sptr )
	{
		sptr = liberate( sptr );
		pthread_mutex_unlock( &sptr->lock );
		pthread_mutex_unlock( &sptr->control );
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
		memset(&sptr->lock,0,sizeof( sptr->lock));
		sptr->client = (struct rest_client *) 0;
		sptr->request = (struct rest_request *) 0;
		sptr->status = 1;
		sptr->started = 0;
		pthread_mutex_lock( &sptr->control );
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
	else	return( reset_rest_thread(sptr) );
}

/*	---------------------------------------------------------	*/
/*	t e r m i n a t e _ r e s t _ t h r e a d _ m a n a g e r 	*/
/*	---------------------------------------------------------	*/
public	int	terminate_rest_thread_manager()
{
	return(0);
}

/*	-----------------------------------------------------------	*/
/*	i n i t i a l i s e _ r e s t _ t h r e a d _ m a n a g e r 	*/
/*	-----------------------------------------------------------	*/
public	int	initialise_rest_thread_manager()
{
	
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

	/* -------------------------------- */
	/* allocate and launch a new thread */
	/* -------------------------------- */
	if (!( tptr ))
	{
		/* --------------------- */
		/* allocate a new thread */
		/* --------------------- */
		if (!( tptr = allocate_rest_thread() ))
			return( tptr );
		else	tptr->status = 1;

		pthread_attr_init( &tptr->attributes);

		/* --------------------- */
		/* launch the new thread */
		/* --------------------- */
		if ((status = pthread_create(
				&tptr->id,
				&tptr->attributes,
				rest_thread_message,
				(void *) tptr)) > 0 )
			return( liberate_rest_thread( tptr ) );
		else
		{
			/* --------------------- */
			/* manage the new thread */
			/* --------------------- */
			if (!( tptr->previous = lastThread ))
				firstThread = tptr;
			else 	tptr->previous->next = tptr;
			lastThread = tptr;
		}
	}

	/* ------------------------------ */
	/* prepare the job for the thread */
	/* ------------------------------ */
	rest_detach_client( cptr );

	lock_rest_thread( tptr );

	tptr->client = cptr;
	tptr->request = rptr;

	unlock_rest_thread( tptr );

	start_rest_thread( tptr );

	return( tptr );

}

#endif	/* _restthread_c_ */
	/* -------------- */

