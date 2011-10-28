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
#ifndef	_rest_thread_h
#define	_rest_thread_h

struct	rest_thread
{
	struct	rest_thread * 	previous;
	struct	rest_thread * 	next;
	struct 	rest_client *	client;
	struct 	rest_request *	request;
	int			status;
	pthread_mutex_t 	lock;
	pthread_t		id;
	pthread_attr_t		attributes;
};

#endif	/* _rest_thread_h */
	/* -------------- */




