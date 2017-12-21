/////////////////////////////////////////////////////////////////////////////////////
// zTypes.h: global data typedefs
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
// This file is part of EAQUAL - Evaluation of Audio QUALity (objective measurement of audio quality)
// Copyright (C) 2000-2002  Alexander Lerch
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// contact information:     Alexander Lerch (lerch@zplane.de)
//                          zplane.development
//                          Holsteinische Str. 39-42
//                          Aufgang 8
//                          D-12161 Berlin
//                          Germany
/////////////////////////////////////////////////////////////////////////////////////


#if !defined(__ZTYPES_HEADER_INCLUDED__)
#define __ZTYPES_HEADER_INCLUDED__

// def for floating point single precision
//#define SINGLE_PRECISION



#if defined(__BORLANDC__) || defined (__WATCOMC__) || defined(_MSC_VER) || defined(__ZTC__) || defined(__HIGHC__)

    #ifdef FLOAT 
    #undef FLOAT 
    #endif

    //zplane.typedefs

    // integer 8bit
    typedef char                zINT8;
    // unsigned integer 8bit
    typedef unsigned char       zUINT8;
    // integer 16bit
    typedef short               zINT16;
    // unsigned integer 16bit
    typedef unsigned short      zUINT16;
    // integer 32bit
    typedef int                 zINT32;
    // integer 64bit
    typedef long                zINT64;
    // unsigned integer 32bit
    typedef unsigned int        zUINT32;    
    // unsigned integer 64bit
    typedef unsigned long       zUINT64;
    // 32 bit float
    typedef float               zFLOAT32;
    // 64 bit float
    typedef double              zFLOAT64;
    // error
    typedef zINT32              zERROR;
    // bool
    typedef unsigned int        zBOOL;
    // handle
    typedef zUINT32             zHANDLE;


#elif defined( __sun)
    typedef short               zINT16;
    typedef long                zINT32;
    typedef zINT32              zERROR;
    #error  PLEASE CHECK "zTypes.h" for correct typedefs!

#elif defined(__unix__) || defined(__unix)

    //zplane.typedefs

    // integer 8bit
    typedef char                zINT8;
    // unsigned integer 8bit
    typedef unsigned char       zUINT8;
    // integer 16bit
    typedef short               zINT16;
    // unsigned integer 16bit
    typedef unsigned short      zUINT16;
    // integer 32bit
    typedef int                 zINT32;
    // integer 64bit
    typedef long                zINT64;
    // unsigned integer 32bit
    typedef unsigned int        zUINT32;    
    // unsigned integer 64bit
    typedef unsigned long       zUINT64;
    // 32 bit float
    typedef float               zFLOAT32;
    // 64 bit float
    typedef double              zFLOAT64;
    // error
    typedef zINT32              zERROR;
    // bool
    typedef unsigned int        zBOOL;
    // handle
    typedef zUINT32             zHANDLE;

#elif defined(VMS) || defined(__VMS)
    typedef short               zINT16;
    typedef long                zINT32;
    typedef zINT32              zERROR;
    #error  PLEASE CHECK "zTypes.h" for correct typedefs!

#else
    #error  COMPILER NOT TESTED. "zTypes.h" needs to be updated
#endif

    #if defined (SINGLE_PRECISION)
        typedef zFLOAT32        zFLOAT;
        typedef zINT32          zINT;
    #else
        typedef zFLOAT64        zFLOAT;
        typedef zINT32          zINT;
#endif


#endif  // __ZTYPES_HEADER_INCLUDED__