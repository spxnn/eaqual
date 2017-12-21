/////////////////////////////////////////////////////////////////////////////////////
// zErrorCodes.h: global error codes
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


////////////////////////////////////////////////////////////////////
// Headerfile zErrorCodes.h defines zError-Codes
// 0000000: No Error
//
// 1000000: Memory Errors
// 2000000: Math Errors
// 3000000: File Errors
// 4000000: Hardware Errors
// 5000000: Function Errors (wrong parameters etc.)
// 6000000: OS Errors
// 7000000: User Errors
//
// 9999999: Unknown Error
////////////////////////////////////////////////////////////////////
#if !defined(__ERRORCODES_HEADER_INCLUDED__)
#define __ERRORCODES_HEADER_INCLUDED__


////////////////////////////////////////////////////////////////////
// No Errors
#define     _NO_ERROR               0

// Memory Errors
#define     _MEMORY_ERROR_BASE      1000000
#define     _MEM_ALLOC_FAILED       (_MEMORY_ERROR_BASE+1)

// Math Errors
#define     _MATH_ERROR_BASE        2000000
#define     _DIV_BY_ZERO            (_MATH_ERROR_BASE+1)
#define     _PRECISION_ERROR        (_MATH_ERROR_BASE+2)

// File Errors
#define     _FILE_ERROR_BASE        3000000
#define     _FILE_OPEN_ERROR        (_FILE_ERROR_BASE+1)
#define     _FILE_CLOSE_ERROR       (_FILE_ERROR_BASE+2)
#define     _FILE_READ_ERROR        (_FILE_ERROR_BASE+3)
#define     _FILE_WRITE_ERROR       (_FILE_ERROR_BASE+4)
#define     _END_OF_FILE_REACHED    (_FILE_ERROR_BASE+5)
#define     _UNKNOWN_FILE_FORMAT    (_FILE_ERROR_BASE+6)
#define     _ILLEGAL_FILE_FORMAT    (_FILE_ERROR_BASE+7)

// Hardware Errors
#define     _HARDWARE_ERROR_BASE    4000000
#define     _NO_SOUND               (_HARDWARE_ERROR_BASE+1)
#define     _NO_MMX                 (_HARDWARE_ERROR_BASE+2)
#define     _NO_ISSE                (_HARDWARE_ERROR_BASE+3)
#define     _NO_3DNOW               (_HARDWARE_ERROR_BASE+4)

// Function Errors
#define     _FUNCTION_ERROR_BASE    5000000
#define     _FUNCTION_NOT_READY     (_FUNCTION_ERROR_BASE+1)
#define     _ILLEGAL_FUNCTION_CALL  (_FUNCTION_ERROR_BASE+2)
#define     _INVALID_FUNCTION_ARGS  (_FUNCTION_ERROR_BASE+3)
#define     _INVALID_SAMPLE_FREQ    (_FUNCTION_ERROR_BASE+4)
#define     _INVALID_NUM_OF_CHANNEL (_FUNCTION_ERROR_BASE+5)
#define     _INVALID_BITRESOLUTION  (_FUNCTION_ERROR_BASE+6)
#define     _INVALID_TYPE           (_FUNCTION_ERROR_BASE+7)
#define     _NO_BUFFER_GENERATED    (_FUNCTION_ERROR_BASE+8)
#define     _NO_BUFFER_AVAILABLE    (_FUNCTION_ERROR_BASE+9)
#define     _NO_CONNECTION          (_FUNCTION_ERROR_BASE+10)
#define     _FEEDBACK               (_FUNCTION_ERROR_BASE+11)
#define     _ALREADY_CONNECTED      (_FUNCTION_ERROR_BASE+12)
#define     _WIRING_CONFLICT        (_FUNCTION_ERROR_BASE+13)   

// OS Errors
#define     _OS_ERROR_BASE          6000000
#define     _THREAD_CREATION_FAILED (_OS_ERROR_BASE+1)
#define     _SYSTEM_TO_SLOW         (_OS_ERROR_BASE+2)

// User Errors
#define     _USER_ERROR_BASE        7000000
#define     _MISSING_CL_ARG         (_USER_ERROR_BASE+1)
#define     _WRONG_CL_ARG           (_USER_ERROR_BASE+2)
#define     _ONLY_HELP_ARG          (_USER_ERROR_BASE+3)
#define     _ONLY_VERSION_ARG       (_USER_ERROR_BASE+4)

// Unknown Errors
#define     _UNKNOWN_ERROR          9999999

#endif // !defined(__ERRORCODES_HEADER_INCLUDED__)