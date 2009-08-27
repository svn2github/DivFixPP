/***********************************(GPL)********************************
*   DivFix++ is AVI Video Repair & Preview Utility.                     *
*   Copyright (C) 2009  Erdem U. Altinyurt                              *
*                                                                       *
*   This program is free software; you can redistribute it and/or       *
*   modify it under the terms of the GNU General Public License         *
*   as published by the Free Software Foundation; either version 2      *
*   of the License, or any later version.                               *
*                                                                       *
*   This program is distributed in the hope that it will be useful,     *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       *
*   GNU General Public License for more details.                        *
*                                                                       *
*   You should have received a copy of the GNU General Public License   *
*   along with this program;                                            *
*   if not, write to the Free Software Foundation, Inc.,                *
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA        *
*                                                                       *
*               home  : divfixpp.sourceforge.net                        *
*               email : spamjunkeater at gmail.com                      *
*************************************************************************/

#ifndef _ENDIAN_H_
#define _ENDIAN_H_
///Endian change code for BigEndian systems MacOSX PPC and Linux PPC
template <class T>
inline T endian_swap( T x ){
	char *z = reinterpret_cast< char* >(&x);
	char val[sizeof( T )];
	for( unsigned short i = 0 ; i < sizeof( T ) ; i++ )
		val[i]=z[sizeof( T )-1-i];
	return *reinterpret_cast< T* >(val);
	}

inline bool is_bigendian( void ){
    unsigned char var[2] = {0,1};
    unsigned short test_endian = *reinterpret_cast<unsigned short*>(&var);
	return ( 0x00FF & test_endian );
    }

inline bool is_littleendian( void ){
    unsigned char var[2] = {0,1};
    unsigned short test_endian = *reinterpret_cast<unsigned short*>(&var);
	return ( 0x00FF & test_endian );
    }

template <class T>
inline T& to_littleendian( T x ){
	static T val;
	val = x;
	if( is_bigendian() )
		val = endian_swap( val );
	return val;
	}

template <class T>
inline T& to_bigendian( T x ){
	static T val;
	val = x;
	if( !is_bigendian() )
		val = endian_swap(val);
	return val;
	}
#endif //_ENDIAN_H_
