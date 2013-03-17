#ifndef	_cotimer_h
#define	_cotimer_h

#include "restthread.h"

struct	job_timer
{
	struct	job_timer * previous;	/* previous in managed list	*/
	struct	job_timer * next;	/* next in managed list		*/
	pthread_mutex_t     lock;	/* access semaphore		*/
	int	number;			/* unique job number		*/
	int	nature;			/* 0 / 1 : as a thread  	*/
	int	oneshot;		/* only once			*/
 	int	hitcount;		/* number of times launched 	*/
	int	period;			/* every period seconds		*/
	int	duration;		/* calculated duration seconds	*/
	int	timestamp;		/* time of last launch		*/
	int	status;			/* status of job 0/1 active	*/
	int	result;			/* last result 0 or ! 0		*/
	int	working;		/* signals timer has fired load	*/
	int	deleting;		/* delete has been requested	*/
	char *	id;
	char *	source;
	char *	target;
	char *	account;
};

public	int			start_timer_manager();
public	struct job_timer * 	allocate_job_timer();
public	struct job_timer * 	liberate_job_timer(struct job_timer * );
public	int			add_timer_operation(struct job_timer * tptr);
public	int			drop_timer_operation(struct job_timer * tptr);


	/* ------------ */
#endif	/* _cotimer_h */
	/* ------------ */


