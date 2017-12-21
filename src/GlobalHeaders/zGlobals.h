/////////////////////////////////////////////////////////////////////////////////////
// zGlobals.h: global constants and macros
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


#if !defined(__ZGLOBALS_HEADER_INCLUDED__)
#define __ZGLOBALS_HEADER_INCLUDED__


// include the global files
#include "zTypes.h"
#include "zErrorCodes.h"



#if defined(_TRUE)
#undef _TRUE
#endif
#if defined(_FALSE)
#undef _FALSE
#endif
#define _TRUE   1
#define _FALSE  0

/////////////////////////////////////////////////////////////////////////////////////
// math constants
#if !defined(_EULER)
#define _EULER                  (zFLOAT)(2.7182818284590452354)
#endif
#if !defined(_PI)
#define _PI                     (zFLOAT)(3.14159265358979323846)
#endif
#if !defined(_2PI)
#define _2PI                    (zFLOAT)(6.28318530717958647692)     // 2*pi
#endif
#if !defined(_INVLN10)
#define _INVLN10                (zFLOAT)(4.342944819032518e-001)     // 1/ln(10)
#endif
#if !defined(_10INVLN10)
#define _10INVLN10              (zFLOAT)(4.342944819032518)          // 10/ln(10)
#endif
#if !defined(_INVLN2)
#define _INVLN2                 (zFLOAT)(1.44269504088896)           // 1/ln(2)
#endif


/////////////////////////////////////////////////////////////////////////////////////
// programming constants
#if !defined(_FLT_MIN)
#define _FLT_MIN                1.175494351e-38F
#endif
#if !defined(_FLT_MAX)
#define _FLT_MAX                3.402823466e+38F
#endif

#endif // #if !defined(__ZGLOBALS_HEADER_INCLUDED__)