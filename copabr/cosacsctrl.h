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
#ifndef	_cosacsctrl_h
#define	_cosacsctrl_h

#define	_COSACS_START_TIMEOUT	15
#define	_COSACS_START_RETRY   60

#define	_COSACS_STOP_TIMEOUT	10
#define	_COSACS_STOP_RETRY   2

#define	_NO_COSACS	0
#define	_USE_COSACS	1
#define	_INSTALL_COSACS	2

public	void	cosacs_release_interface( char * host );
public	int	cosacs_create_metadata( char * cosacs, char * prefix, char * symbol, char * value );
public	int	cosacs_create_script( char * cosacs, char * action, char * parameters, char * type );
public	int	cosacs_create_file( char * cosacs, char * remotename, char * localname, char * type );
public	char *	cosacs_create_probe( char * cosacs, char * prefix, char * symbol, char * stream, char * metric, char * period);
public	int	cosacs_metadata_instructions( char * cosacs, char * nature, char * contract, char * publisher, char * account );
public	int	cosacs_test_interface( char * cosacs, int timeout, int retry );

	/* ------------- */
#endif	/* _cosacsctrl_h */
	/* ------------- */


