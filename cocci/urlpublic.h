
#ifndef	_url_public_h
#define	_url_public_h

public struct url * liberate_url(struct url * sptr);
public struct url * allocate_url();
public struct url * reset_url(struct url * sptr);
public struct url * analyse_url( char * sptr );
public struct url * validate_url( struct url * uptr );
public char 	  * serialise_url_host( struct url * uptr );
public char 	  * serialise_url_host_no_port( struct url * uptr );
public char 	  * serialise_url( struct url * uptr, char * xptr );
public char 	  * append_url( char * uptr, char * xptr );
public	char 	  * replace_url_object( struct url * uptr, char * xptr );

#endif	/* _url_public_h */

