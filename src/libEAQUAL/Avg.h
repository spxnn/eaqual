/////////////////////////////////////////////////////////////////////////////////////
// Avg.h: interface for the CAvg class.
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


#if !defined(__AVG_HEADER_INCLUDED__)
#define __AVG_HEADER_INCLUDED__

#include "BasicUtils.h"

/////////////////////////////////////////////////////////////////////////////////////
//  @class:         CAvg
//  @derived_from:  CBasicUtils
//  @variables:     -
//  @methods:       - avLinAvg 
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         implements some basic methods for averaging
//  @creation_date: 01.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
class CAvg  : public CBasicUtils
{
public:
    CAvg(){};
    virtual ~CAvg(){};

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        avLinAvg
    //  @parameter:     - zFLOAT *pfVector  pointer on vector on which the average is calulated
    //                  - zINT  iLength     length of vector    
    //  @result:        - zFLOAT    fAvg        result of averaging
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calculates average according BS.1387, eq. (88) 
    //  @creation_date: 13.07.2000
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    inline zFLOAT   avLinAvg(   zFLOAT  *pfVector, 
                                zINT    iLength)
                    {
                        zFLOAT      fAvg    = 0.0F;

                        for (zINT iIdx = 0; iIdx < iLength; iIdx++)
                            fAvg   += pfVector[iIdx];

                        return ( fAvg / iLength );
                    };

};

#endif // !defined(__AVG_HEADER_INCLUDED__)
