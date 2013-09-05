#ifndef	_cotimer_c
#define	_cotimer_c

#include "cotimer.h"
#include "cb.h"
#include "cspi.h"

/*	----------------------------------	*/
/*	timer events and operation manager	*/
/*	----------------------------------	*/
private	struct	job_timer_control
{
	int	status;
	int	count;
	int	hitcount;
	int	duration;
	int	newduration;
	int	timers;
	struct	job_timer * first;
	struct	job_timer * last;
	struct	job_timer * current;
	struct	rest_thread * thread;
	pthread_mutex_t     lock;
} JobControl = {
	0,0,0,0,0,0,
	(struct job_timer *) 0,
	(struct job_timer *) 0,
	(struct job_timer *) 0,
	(struct rest_thread *) 0
	};

#define	job_debug check_debug()

/*	-----------------------------------------------		*/
/*	i n i t i a l i s e _ t i m e r _ c o n t r o l		*/
/*	-----------------------------------------------		*/
public	void	initialise_timer_control()
{
	memset(&JobControl,0,sizeof( struct job_timer_control ));
	return;
}

/*	-----------------------------------------------------------	*/
/*		   l o c k _ l o b _ t i m e r				*/
/*	-----------------------------------------------------------	*/
private	void	lock_job_timer(struct job_timer * tptr)
{
	pthread_mutex_lock( &tptr->lock );
	return;
}

/*	-----------------------------------------------------------	*/
/*		u n l o c k _ l o b _ t i m e r				*/
/*	-----------------------------------------------------------	*/
private	void	unlock_job_timer(struct job_timer * tptr)
{
	pthread_mutex_unlock( &tptr->lock );
	return;
}

/*	-----------------------------------------------------------	*/
/*		   l o c k _ j o b _ c o n t r o l 			*/
/*	-----------------------------------------------------------	*/
private	void	lock_job_control()
{
	pthread_mutex_lock( &JobControl.lock );
	return;
}

/*	-----------------------------------------------------------	*/
/*		u n l o c k _ j o b _ c o n t r o l			*/
/*	-----------------------------------------------------------	*/
private	void	unlock_job_control()
{
	pthread_mutex_unlock( &JobControl.lock );
	return;
}

/*	-------------------------------------	*/
/*	a l l o c a t e _ o c c i _ t i m e r 	*/
/*	-------------------------------------	*/
public	struct job_timer * allocate_job_timer()
{
	struct	job_timer * tptr=(struct job_timer *) 0;
	if (!( tptr = (struct job_timer *) allocate( sizeof( struct job_timer ) ) ))
		return( tptr );
	else
	{
		memset(tptr,0,sizeof( struct job_timer ));
		tptr->number = ++JobControl.count;
		return( tptr );
	}
}

/*	-------------------------------------	*/
/*	l i b e r a t e _ o c c i _ t i m e r 	*/
/*	-------------------------------------	*/
public	struct job_timer * liberate_job_timer(struct job_timer *  tptr)
{
	if ( tptr )
	{
		liberate( tptr );
	}
	return((struct job_timer *) 0);
}

/*	-------------------------------------	*/
/*	a d d _ t i m e r _ o p e r a t i o n 	*/
/*	-------------------------------------	*/
public	int	add_timer_operation(struct job_timer * tptr)
{
	lock_job_control();
	if (!( tptr->previous = JobControl.last ))
		JobControl.first = tptr;
	else	tptr->previous->next = tptr;
	JobControl.last = tptr;
	JobControl.timers++;
	unlock_job_control();
	return( 0 );
}

/*	---------------------------------------------	*/
/*	r e s o l v e _ t i m e r _ o p e r a t i o n 	*/
/*	---------------------------------------------	*/
public	struct job_timer * resolve_timer_operation( int reference )
{
	struct	job_timer * jptr;
	lock_job_control();
	for (	jptr=JobControl.first;
		jptr != (struct job_timer *) 0;
		jptr = jptr->next )
		if ( jptr->number == reference )
			break;
	unlock_job_control();
	return( jptr );
}

/*	-------------------------------------------	*/
/*	f o r g e t _ t i m e r _ o p e r a t i o n 	*/
/*	-------------------------------------------	*/
private	int	forget_timer_operation(struct job_timer * tptr)
{
	lock_job_control();
	if (!( tptr->previous ))
		JobControl.first = tptr->next;
	else	tptr->previous->next = tptr->next;
	if (!( tptr->next ))
		JobControl.last = tptr->previous;
	else	tptr->next->previous = tptr->previous;
	if ( JobControl.timers )
		JobControl.timers--;
	unlock_job_control();
	return(0);
}

/*	----------------------------------------	*/
/*	d r o p _ t i m e r _ o p e r a t i o n 	*/
/*	----------------------------------------	*/
public	int	drop_timer_operation(struct job_timer * tptr)
{
	forget_timer_operation(tptr);
	tptr = liberate_job_timer( tptr );
	return( 0 );
}

/*	-----------------------------------	*/
/*	 j o b _ a l a r m _ c a t c h e r	*/
/*	-----------------------------------	*/
private	int	job_alarm=0;
private	void	job_alarm_catcher(int sig)
{
	job_alarm=1;
	if ( job_debug ) { printf("job timer manager alarm catcher\n"); }
	return;
}

/*	-----------------------------------	*/
/*	j o b _ s i g n a l _ c a t c h e r	*/
/*	-----------------------------------	*/
private	int	job_signal=0;
private	void	job_signal_catcher(int sig)
{
	job_signal=1;
	if ( job_debug ) { 	printf("job timer manager signal catcher\n"); }
	return;
}

/*	-------------------------------------	*/
/*	c a l c u l a t e _ d u r a t i o n s	*/
/*	-------------------------------------	*/
private	void	calculate_durations(int elapsed)
{
	struct	job_timer * jptr;
	struct	job_timer * tptr;
	struct	job_timer * smallest;
	lock_job_control();
	if ( job_debug ) { printf("calculate durations\n"); }
	if (!(tptr = JobControl.current ))
	{
		elapsed=0;
		JobControl.duration=0;
		JobControl.current=(struct job_timer *) 0;
		/* ---------------------------- */
		/* select the shortest duration */
		/* ---------------------------- */
		for (	jptr=JobControl.first;
			jptr != (struct job_timer *) 0;
			jptr = jptr->next )
		{
			if (!( JobControl.duration ))
			{
				JobControl.duration = jptr->duration;
				JobControl.current  = jptr;
			}
			else if ( jptr->duration < JobControl.duration )
			{
				JobControl.duration = jptr->duration;
				JobControl.current  = jptr;
			}
		}
	}	
	/* ---------------------- */
	/* adjust other durations */
	/* ---------------------- */
	if ((tptr = JobControl.current) != (struct job_timer *) 0)
	{
		if ( job_debug ) { 	printf("adjust durations\n"); }
		for (	jptr=JobControl.first;
			jptr != (struct job_timer *) 0;
			jptr = jptr->next )
		{
			if ( jptr->number != tptr->number )
			{
				if ( jptr->duration >= tptr->duration )
					jptr->duration -= (tptr->duration+elapsed);
				else	jptr->duration = 0;
			}
		}
		if ( tptr->duration >= elapsed )
			tptr->duration -= elapsed;
		else	tptr->duration = 1;
		JobControl.duration = tptr->duration;
		if ( job_debug ) { printf("JobControl current=%u, duration=%u \n",tptr->number,tptr->duration); }
	}
	unlock_job_control();
}

/*	-----------------------------------	*/
/*	p r o c e s s _ j o b _ s i g n a l	*/
/*	-----------------------------------	*/
private	void	process_job_signal(int before, int after)
{
	/* --------------------------- */
	/* here we need to recalculate */
	/* the job mapping to reflect  */
	/* any OCCI interface changes  */
	/* --------------------------- */
	if ( job_debug ) { printf("job timer manager process signal( timers=%u)\n",JobControl.timers); }
	calculate_durations(after-before);
	return;
}

/*	----------------------------------------------------------	*/
/*	    s c r i p t e d _ t i m e r _ p r o c e s s i n g		*/
/*	----------------------------------------------------------	*/
private	int	scripted_timer_processing( 
		char * expression,
		char * account,
		char * timer,
		char * subject )

{
	char *	argv[10];
	int	argc=0;
	char *	result;
	char 	buffer[2048];
	if (!( rest_valid_string( expression ) ))
		return( 0 );
	else
	{
		/* ----------------------------------------------------------------------- */
		/* CORDSCRIPT: CALLER, CONTROLID, NATURE, PACKET, PUBLISHER, OPERATOR, TLS */
		/* ----------------------------------------------------------------------- */
		sprintf(buffer,"%s/%s/%s",get_identity(), _CORDS_TIMER, timer );
		argv[argc++] = "timer.event";
		argv[argc++] = buffer;
		argv[argc++] = ( account ? account : "none");
		argv[argc++] = ( subject ? subject : "none");
		argv[argc++] = default_publisher();
		argv[argc++] = default_operator();
		argv[argc++] = default_tls();
		argv[argc] = (char *) 0;
		if (!( result = evaluate_cordscript( expression, argc, argv )))
			return( 0 );
		else	return( atoi( result ) );
	}
}




/*	---------------------------------	*/
/*	   j o b _ i n v o c a t i o n		*/
/*	---------------------------------	*/
private	int	job_invocation( struct job_timer * jptr )
{
	char *	xptr;
	char *	aptr;
	struct	occi_response * zptr;
	if (!( rest_valid_string( xptr = jptr->target ) ))
		return(0);
	else if (!( strncmp( xptr, "cordscript:", strlen( "cordscript:" ) ) ))
	{
		xptr += strlen( "cordscript:" );
		jptr->result = scripted_timer_processing( xptr, jptr->account, jptr->id, jptr->source );
		return(0);
	}
	else if (!( rest_valid_string( aptr = jptr->source ) ))
		return(0);
	else if (!( zptr = cords_invoke_action( xptr, aptr, _CORDS_CONTRACT_AGENT, default_tls() ) ))
	{
		jptr->result = 500;
		return(0);
	}
	else
	{
		jptr->result = 200;
		zptr = occi_remove_response (zptr );
		return(0);
	}
}

/*	---------------------------------	*/
/*	p r o c e s s _ j o b _ a l a r m	*/
/*	---------------------------------	*/
private	void	process_job_alarm(int before, int after)
{
	int	consumed = (after-before);
	struct	job_timer * tptr;
	struct	job_timer * jptr;
	/* --------------------------------- */
	/* here we must launch the timed job */
	/* --------------------------------- */
	if ( job_debug ) { printf("job timer manager process alarm\n"); }
	if (( tptr = JobControl.current ) != (struct job_timer *) 0)
	{
		tptr->duration = 0;
		while (tptr)
		{
			if (!( tptr->duration ))
			{
				/* ----------------------- */
				/* adjust timer statistics */
				/* ----------------------- */
				lock_job_timer(tptr);
				tptr->working=1;
				tptr->hitcount++;
				tptr->timestamp = time((long *) 0);
				tptr->duration  = tptr->period;
				if ( job_debug) { printf("job timer launched job : (id=%u, hit=%u, time=%u)\n",tptr->number,tptr->hitcount,tptr->timestamp); }
				unlock_job_timer(tptr);
				/* ------------------------- */
				/* launch the timed workload */
				/* ------------------------- */
				job_invocation( tptr );

				/* ---------------------- */
				/* post workload clean up */
				/* ---------------------- */
				lock_job_timer(tptr);
				tptr->working = 0;
				if ( tptr->deleting )
				{
					/* ----------------- */
					/* delete is pending */
					/* ----------------- */
					jptr = tptr->next;
					forget_timer_operation(tptr);
					unlock_job_timer(tptr);
					tptr = liberate_job_timer( tptr );
					tptr = jptr;
					continue;
				}
				else
				{
					unlock_job_timer(tptr);
					tptr = tptr->next;
				}
			}
			else	tptr = tptr->next;
		}
		lock_job_control();
		JobControl.duration=0;
		JobControl.current=(struct job_timer *) 0;
		unlock_job_control();
		calculate_durations(0);
	}
	else	calculate_durations(0);
	JobControl.hitcount++;
	return;
}

/*	-----------------------------------	*/
/*	 j o b _ t i m e r _ m a n a g e r	*/
/*	-----------------------------------	*/
private	void * 	job_timer_manager( void * vptr )
{
	struct rest_thread * tptr=vptr;
	void *	previous_alarm;
	void *	previous_signal;
	int	before;
	int	after;
	sigset_t	signals;
	

	rest_log_message("thread: occi timer manager detach\n");
	pthread_detach( tptr->id );

	if ( job_debug ) { 	printf("job timer manager entry\n");	}
	sigemptyset(&signals);
	sigaddset(&signals,SIGUSR1);
	pthread_sigmask(SIG_BLOCK,0,&signals);
	calculate_durations(0);
	job_signal = 0;
	job_alarm  = 0;
	while (1)
	{
		signal(SIGUSR1,job_signal_catcher);
		job_signal = 0;
		before = time((long*) 0);
		/* -------------------- */
		/* set a job controller */
		/* -------------------- */
		if ( JobControl.duration )
		{
			if ( job_debug ) { printf("job timer manager sleep(%u)\n",JobControl.duration); }

			if ( sleep( JobControl.duration ) != -1 )
				job_alarm = 1;
			else	job_signal= 1;
		}
		else if ( JobControl.current )
		{
			usleep(100);
			job_alarm = 1;
		}
		else
		{
			if ( job_debug ) { printf("job timer manager pause\n");	}
			pause();
		}
		if ( job_debug ) { printf("job timer manager wakeup\n");	}

		after = time((long*) 0);

		/* ---------------------------- */
		/* process conditions on wakeup */
		/* ---------------------------- */
		if ( job_signal ) { job_signal=0; process_job_signal(before,after); }
		if ( job_alarm ) { job_alarm=0; process_job_alarm(before,after); }
		
	}

	if ( job_debug ) { printf("job timer manager exit\n"); }
	tptr->status = 0;
	rest_log_message("thread: occi timer manager exit");
	pthread_exit((void *) 0);
}

/*	-----------------------------------	*/
/*	i n f o r m _ j o b _ c o n t r o l	*/
/*	-----------------------------------	*/
public	void	inform_job_control()
{
	struct	rest_thread * tptr;
	lock_job_control();
	if (( tptr = JobControl.thread ))
		pthread_kill( tptr->id, SIGUSR1 );
	unlock_job_control();
	return;
}

/*	-----------------------------------	*/
/*	 s t a r t _ j o b _ c o n t r o l	*/
/*	-----------------------------------	*/
public	int	start_job_control()
{
	struct	rest_thread * tptr;
	int	status;

	/* ---------------------- */
	/* Launch the OCCI Thread */
	/* ---------------------- */
	if ( JobControl.status )
		return( 0 );

	else if (!( tptr = allocate_rest_thread() ))
		return( 37 );
	else
	{
		JobControl.thread = tptr;
		tptr->status = 1;
		pthread_attr_init( &tptr->attributes);

		/* --------------------- */
		/* launch the new thread */
		/* --------------------- */
		if ((status = pthread_create(
				&tptr->id,
				&tptr->attributes,
				job_timer_manager,
				(void *) tptr)) > 0 )
			return( 38 );
		else
		{
			JobControl.status = 1;
			return( 0 );
		}
	}
}

/*	-------------------------------------------	*/
/* 	       a c t i v a t e _ t i m e r		*/
/*	-------------------------------------------	*/
private	int	activate_timer( struct cords_timer * tptr )
{
	struct	job_timer * jptr;
	if (!( tptr ))
		return(0);
	else if (!( rest_valid_string( tptr->period ) ))
		return(0);
	else if (!( jptr = allocate_job_timer() ))
		return( 0 );
	else
	{
		/* ----------------------------- */
		/* duplicate the workload if any */
		/* ----------------------------- */
		if (!( rest_valid_string( tptr->id ) ))
			jptr->id = (char *) 0;
		else if (!( jptr->id = allocate_string( tptr->id ) ))
		{
			jptr = liberate_job_timer( jptr );
			return( 0 );
		}

		if (!( rest_valid_string( tptr->source ) ))
			jptr->source = (char *) 0;
		else if (!( jptr->source = allocate_string( tptr->source ) ))
		{
			jptr = liberate_job_timer( jptr );
			return( 0 );
		}

		if (!( rest_valid_string( tptr->target ) ))
			jptr->target = (char *) 0;
		else if (!( jptr->target = allocate_string( tptr->target ) ))
		{
			jptr = liberate_job_timer( jptr );
			return( 0 );
		}

		if (!( rest_valid_string( tptr->account ) ))
			jptr->account = (char *) 0;
		else if (!( jptr->account = allocate_string( tptr->account ) ))
		{
			jptr = liberate_job_timer( jptr );
			return( 0 );
		}

		if ( add_timer_operation( jptr ) )
		{
			jptr = liberate_job_timer(jptr);
			return( 0 );
		}
		else
		{
			/* ---------------------------------- */
			/* calculate the required periodicity */
			/* ---------------------------------- */
			if (!( strcasecmp( tptr->period, "second" ) ))
				jptr->period = 1;
			else if (!( strcasecmp( tptr->period, "minute" ) ))
				jptr->period = 60;
			else if (!( strcasecmp( tptr->period, "hour" ) ))
				jptr->period = 60*60;
			else if (!( strcasecmp( tptr->period, "day" ) ))
				jptr->period = 60*60*24;
			else if (!( strcasecmp( tptr->period, "week" ) ))
				jptr->period = 60*60*24*7;
			else if (!( strcasecmp( tptr->period, "month" ) ))
				jptr->period = 60*60*24*30;
			else if (!( strcasecmp( tptr->period, "year" ) ))
				jptr->period = 60*60*24*365;
			else if (!( jptr->period = atoi( tptr->period ) ))
			{
				jptr = liberate_job_timer(jptr);
				return( 0 );
			}
			jptr->duration = jptr->period;
			/* ----------------------- */
			/* start the job timer now */
			/* ----------------------- */
			if (!( JobControl.status ))
				start_job_control();
			else	inform_job_control();
			return( jptr->number );	
		}
	}
}

/*	-------------------------------------------	*/
/* 	      r e l e a s e _ t i m e r			*/
/*	-------------------------------------------	*/
private	int	release_timer( int reference )
{
	struct	job_timer * jptr;
	if (!( jptr = resolve_timer_operation( reference ) ))
		return( 0 );
	else
	{
		lock_job_control();
		if (( JobControl.current )
		&&  ( JobControl.current->number == jptr->number ))
		{
			lock_job_timer( jptr );
			if ( jptr->working )
				jptr->deleting = 1;
			JobControl.current = (struct job_timer *) 0;
			JobControl.duration= 0;
			unlock_job_timer( jptr );
		}	
		unlock_job_control();
		if (!( jptr->deleting ))
		{
			drop_timer_operation( jptr );
			inform_job_control();
		}
		return( 0 );
	}
}

/*	-------------------------------------------	*/
/* 	           h i t _ c o u n t e r 		*/
/*	-------------------------------------------	*/
private	struct	rest_response * hit_counter(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_counter * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		pptr->value = pptr->value + 1;
		autosave_cords_counter_nodes();
	 	return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	        s t a r t _ t i m e r			*/
/*	-------------------------------------------	*/
private	struct	rest_response * start_timer(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	char 	buffer[256];
	int	delay;
	struct	cords_timer * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->state )
	 	return( rest_html_response( aptr, 200, "OK" ) );
	else if (!( pptr->reference = activate_timer( pptr ) ))
	 	return( rest_html_response( aptr, 400, "Failure to start timer" ) );
	else
	{
		/* ----------------------------------- */
		/* perform timer category book keeping */
		/* ----------------------------------- */
		sprintf(buffer,"%lu",time((long *) 0));
		pptr->activated = allocate_string( buffer );
		pptr->state=1;
		autosave_cords_timer_nodes();
	 	return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	        s t o p _ j o b _ t i m e r		*/
/*	-------------------------------------------	*/
private	int	stop_job_timer( struct cords_timer * pptr )
{
	if ( pptr->state )
	{
		if ( pptr->reference ) 
			pptr->reference = release_timer( pptr->reference ); 
		pptr->state=0;
		autosave_cords_timer_nodes();
	}
	return(0);
}

/*	-------------------------------------------	*/
/* 	        s t o p _ t i m e r			*/
/*	-------------------------------------------	*/
private	struct	rest_response * stop_timer(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_timer * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if (!( pptr->state ))
	 	return( rest_html_response( aptr, 200, "OK" ) );
	else 
	{
		stop_job_timer( pptr );
	 	return( rest_html_response( aptr, 200, "OK" ) );
	}

}

/*	-------------------------------------------	*/
/* 	      c r e a t e _ t i m e r  			*/
/*	-------------------------------------------	*/
private	int	create_timer(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	char 	buffer[256];
	struct	cords_timer * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else
	{
		sprintf(buffer,"%lu",time((long *) 0));
		pptr->created = allocate_string( buffer );
		if ( pptr->state )
			
		return(0);
	}
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ t i m e r		  	*/
/*	-------------------------------------------	*/
private	int	delete_timer(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_timer * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(stop_job_timer(pptr));
}

private	struct	occi_interface	timer_interface = {
	create_timer,
	NULL,
	NULL,
	delete_timer
	};


	/* ---------- */
#endif	/* _cotimer_c */
	/* ---------- */


