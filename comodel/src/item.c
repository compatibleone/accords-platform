#ifndef _item_c_
#define _item_c_

#include "item.h"

/*	--------------------------	*/
/*	l i b e r a t e _ i t e m 	*/
/*	--------------------------	*/
public struct item * liberate_item(struct item * sptr)
{
	if ( sptr )
	{
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->basic )
			 sptr->basic = liberate(sptr->basic);
		if ( sptr->type )
			 sptr->type = liberate(sptr->type);
		sptr = liberate( sptr );
	}
	return((struct item *) 0);

}

/*	--------------------	*/
/*	r e s e t _ i t e m 	*/
/*	--------------------	*/
public struct item * reset_item(struct item * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct item*) 0;
		sptr->next = (struct item*) 0;
		sptr->name = (char*) 0;
		sptr->basic = (char*) 0;
		sptr->type = (char*) 0;
		sptr->dimension =  0;
		sptr->indirection =  0;
	}
	return(sptr);

}

/*	--------------------------	*/
/*	a l l o c a t e _ i t e m 	*/
/*	--------------------------	*/
public struct item * allocate_item()
{
	struct item * sptr;
	if (!( sptr = allocate( sizeof( struct item ) ) ))
		return( sptr );
	else	return( reset_item(sptr) );
}

#endif	/* _item_c_ */
