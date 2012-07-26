/* ------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                   */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>    */
/* --------------------------------------------------------------------*/
/*  This is free software; you can redistribute it and/or modify it    */
/*  under the terms of the GNU Lesser General Public License as        */
/*  published by the Free Software Foundation; either version 2.1 of   */
/*  the License, or (at your option) any later version.                */
/*                                                                     */
/*  This software is distributed in the hope that it will be useful,   */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of     */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   */
/*  Lesser General Public License for more details.                    */
/*                                                                     */
/*  You should have received a copy of the GNU Lesser General Public   */
/*  License along with this software; if not, write to the Free        */
/*  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA */
/*  02110-1301 USA, or see the FSF site: http://www.fsf.org.           */
/* --------------------------------------------------------------------*/
#ifndef	_cosacsctrl_h
#define	_cosacsctrl_h

#define	_COSACS_TIMEOUT	15
#define	_COSACS_RETRY   60

public	void	cosacs_release_interface( char * host );
public	int	cosacs_create_metadata( char * cosacs, char * prefix, char * symbol, char * value );
public	int	cosacs_create_script( char * cosacs, char * action, char * parameters, char * type );
public	int	cosacs_create_file( char * cosacs, char * remotename, char * localname, char * type );
public	int	cosacs_create_probe( char * cosacs, char * prefix, char * symbol, char * stream, char * metric);
public	int	cosacs_metadata_instructions( char * cosacs, char * nature, char * contract, char * publisher );
public	int	cosacs_test_interface( char * cosacs, int timeout, int retry );

	/* ------------- */
#endif	/* _cosacsctrl_h */
	/* ------------- */


