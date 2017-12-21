/////////////////////////////////////////////////////////////////////////////////////
// BasicUtils.h: interface for the CBasicUtils class.
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


#if !defined(__BASICUTILS_HEADER_INCLUDED__)
#define __BASICUTILS_HEADER_INCLUDED__

#include <memory.h>
#include "EAQUALCore.h"

/////////////////////////////////////////////////////////////////////////////////////
//  @class:         CBasicUtils
//  @derived_from:  -
//  @variables:     -
//  @methods:       - zplAddVec (overloaded)
//                  - zplCopyVec (overloaded)
//                  - zplDiffVec (overloaded)
//                  - zplDivVec 
//                  - zplMultVec (overloaded)
//                  - zplMultVecConst (overloaded)
//                  - zplRealFFT
//                  - zplSetOne
//                  - zplSetZero
//                  - zplSquareVec (overloaded)
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         BasicUtils implements some basic methods
//  @creation_date: 28.10.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
class CBasicUtils  
{
public:
    CBasicUtils(){};
    virtual ~CBasicUtils(){};

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        zplRealFFT
    //  @parameter:     - zFLOAT            *x                      pointer on time data
    //                  - zINT32            m                       order of FFT
    //  @result:        -
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         computes FFT of order m, see file fft.cpp 
    //  @creation_date: 28.10.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    static  void    zplRealFFT(     zFLOAT          *x,
                                    zINT32          m);

    
    /////////////////////////////////////////////////////////////////////////////////////
    // inline functions
    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        zplSetZero
    //  @parameter:     - zFLOAT            *pA                     pointer on array A
    //                  - zUINT32       uiLength                length of arrays A and B
    //  @result:        -
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         flushes array A with zeros 
    //  @creation_date: 28.10.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    inline  void    zplSetZero(     zFLOAT      *pA, 
                                    zUINT32     uiLength)
    {
        memset(pA, 0, uiLength * sizeof(zFLOAT));
    }


    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        zplSetOne
    //  @parameter:     - zFLOAT            *pA                     pointer on array A
    //                  - zUINT32       uiLength                length of arrays A and B
    //  @result:        -
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         flushes array A with ones 
    //  @creation_date: 28.10.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    inline  void    zplSetOne(      zFLOAT      *pA, 
                                    zINT        iLength)
    {
        for (iLength-=1; iLength>=0; iLength--)
            pA[iLength] = 1;
    }
    
    
    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        zplCopyVec
    //  @parameter:     - zFLOAT            *pA                     pointer on array A
    //                  - zFLOAT            *pB                     pointer on array B
    //                  - zUINT32       uiLength                length of arrays A and B
    //  @result:        -
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         copies array B to array A
    //  @creation_date: 28.10.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    inline  void    zplCopyVec(     zFLOAT      *pA, 
                                    zFLOAT      *pB, 
                                    zUINT32     uiLength)
    {
        memcpy(pA, pB, uiLength * sizeof(zFLOAT));
    }
    
    
    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        zplCopyVec
    //  @parameter:     - zUINT32       *pA                     pointer on array A
    //                  - zUINT32       *pB                     pointer on array B
    //                  - zUINT32       uiLength                length of arrays A and B
    //  @result:        -
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         copies array B to array A
    //  @creation_date: 28.10.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    inline  void    zplCopyVec(     zUINT32     *pA, 
                                    zUINT32     *pB, 
                                    zUINT32     uiLength)
    {
        memcpy(pA, pB, uiLength * sizeof(zUINT32));
    }
    

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        zplSquareVec
    //  @parameter:     - zFLOAT            *pA                     pointer on array A
    //                  - zINT          iLength                 length of array A
    //  @result:        -
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         squares array A and writes result in array A (overloaded)
    //  @creation_date: 28.10.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    inline  void    zplSquareVec(   zFLOAT      *pA, 
                                    zINT        iLength)
    {
        for (iLength -= 1; iLength >= 0; iLength--)
            pA[iLength]        *= pA[iLength];
    }


    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        zplSquareVec 
    //  @parameter:     - zFLOAT            *pA                     pointer on array A
    //                  - zFLOAT            *pResult                pointer on resulting array Result
    //                  - zINT          iLength                 length of arrays A and Result
    //  @result:        -
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         squares array A into array Result (overloaded)
    //  @creation_date: 28.10.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    inline  void    zplSquareVec(   zFLOAT      *pA, 
                                    zFLOAT      *pResult, 
                                    zINT        iLength)
    {
        for (iLength -= 1; iLength >= 0; iLength--)
            pResult[iLength]        = pA[iLength] * pA[iLength];
    }


    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        zplMultVec 
    //  @parameter:     - zFLOAT            *pA                     pointer on array A
    //                  - zFLOAT            *pB                     pointer on array B
    //                  - zINT          iLength                 length of arrays A and B
    //  @result:        -
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         multiplies arrays A and B and writes the result to A (overloaded)
    //  @creation_date: 28.10.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    inline  void    zplMultVec(     zFLOAT      *pA, 
                                    zFLOAT      *pB, 
                                    zINT        iLength)
    {
        for (iLength -= 1; iLength >= 0; iLength--)
            pA[iLength]    *= pB[iLength];
    }


    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        zplMultVec 
    //  @parameter:     - zFLOAT            *pA                     pointer on array A
    //                  - zFLOAT            *pB                     pointer on array B
    //                  - zFLOAT            *pResult                pointer on array Result
    //                  - zINT          iLength                 length of arrays A, B and Result
    //  @result:        -
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         multiplies arrays A and B and writes the result to Result (overloaded)
    //  @creation_date: 28.10.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    inline  void    zplMultVec(     zFLOAT      *pA, 
                                    zFLOAT      *pB, 
                                    zFLOAT      *pResult, 
                                    zINT        iLength)
    {
        for (iLength -= 1; iLength >= 0; iLength--)
            pResult[iLength]    = pA[iLength] * pB[iLength];
    }


    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        zplDivVec 
    //  @parameter:     - zFLOAT            *pA                     pointer on array A
    //                  - zFLOAT            *pB                     pointer on array B
    //                  - zFLOAT            *pResult                pointer on array Result
    //                  - zINT          iLength                 length of arrays A, B and Result
    //  @result:        -
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         divides of A and B and writes the result to Result 
    //  @creation_date: 15.12.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    inline  void    zplDivVec(      zFLOAT      *pA, 
                                    zFLOAT      *pB, 
                                    zFLOAT      *pResult, 
                                    zINT        iLength)
    {
        for (iLength -= 1; iLength >= 0; iLength--)
            pResult[iLength]    = pA[iLength] / pB[iLength];
    }


    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        zplMultVecConst 
    //  @parameter:     - zFLOAT            *pA                     pointer on array A
    //                  - zFLOAT            fB                      constant fB
    //                  - zINT          iLength                 length of array A
    //  @result:        -
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         multiplies array A with constant fB and writes result into array A (overloaded)
    //  @creation_date: 28.10.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    inline  void    zplMultVecConst(    zFLOAT  *pA, 
                                        zFLOAT  fB, 
                                        zINT iLength)
    {
        for (iLength -= 1; iLength >= 0; iLength--)
            pA[iLength]    *= fB;
    }


    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        zplMultVecConst 
    //  @parameter:     - zFLOAT            *pA                     pointer on resulting array A
    //                  - zFLOAT            *pB                     pointer on array B
    //                  - zFLOAT            fC                      constant fC
    //                  - zUINT32       uiLength                length of array A
    //  @result:        -
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         multiplies array A with constant fB and writes result into array A (overloaded)
    //  @creation_date: 28.10.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    inline  void    zplMultVecConst(    zFLOAT  *pA, 
                                        zFLOAT  *pB,
                                        zFLOAT  fC, 
                                        zINT    iLength)
    {
        for (iLength -= 1; iLength >= 0; iLength--)
            pA[iLength]     = pB[iLength] * fC;
    }


    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        zplAddVec 
    //  @parameter:     - zFLOAT            *pA                     pointer on array A
    //                  - zFLOAT            *pB                     pointer on array B
    //                  - zINT          iLength                 length of arrays A and B
    //  @result:        -
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         adds array A with array B and writes result into array A (overloaded)
    //  @creation_date: 28.10.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    inline  void    zplAddVec(      zFLOAT  *pA, 
                                    zFLOAT  *pB, 
                                    zINT    iLength)
    {
        for (iLength -= 1; iLength >= 0; iLength--)
            pA[iLength]    += pB[iLength];
    }


    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        zplAddVec 
    //  @parameter:     - zFLOAT            *pA                     pointer on array A
    //                  - zFLOAT            *pB                     pointer on array B
    //                  - zFLOAT            *pResult                pointer on array Result
    //                  - zINT          iLength                 length of arrays A, B and Result
    //  @result:        -
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         adds array A with array B and writes result into array Result (overloaded)
    //  @creation_date: 28.10.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    inline  void    zplAddVec(      zFLOAT  *pA, 
                                    zFLOAT  *pB, 
                                    zFLOAT  *pResult, 
                                    zINT     iLength)
    {
        for (iLength -= 1; iLength >= 0; iLength--)
            pResult[iLength]    = pA[iLength] + pB[iLength];
    }


    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        zplDiffVec 
    //  @parameter:     - zFLOAT            *pA                     pointer on array A
    //                  - zFLOAT            *pB                     pointer on array B
    //                  - zUINT32       uiLength                length of arrays A and B
    //  @result:        -
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calc the difference of array A and array B and write result into array A (overloaded)
    //  @creation_date: 28.10.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    inline  void    zplDiffVec(     zFLOAT  *pA, 
                                    zFLOAT  *pB, 
                                    zINT    iLength)
    {
        for (iLength -= 1; iLength >= 0; iLength--)
            pA[iLength]    -= pB[iLength];
    }


    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        zplDiffVec 
    //  @parameter:     - zFLOAT            *pA                     pointer on array A
    //                  - zFLOAT            *pB                     pointer on array B
    //                  - zFLOAT            *pResult                pointer on array Result
    //                  - zINT          iLength                 length of arrays A, B and Result
    //  @result:        -
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calc the difference of array A and array B and write result into array Result (overloaded)
    //  @creation_date: 28.10.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    inline  void    zplDiffVec(     zFLOAT  *pA, 
                                    zFLOAT  *pB, 
                                    zFLOAT  *pResult, 
                                    zINT    iLength)
    {
        for (iLength -= 1; iLength >= 0; iLength--)
            pResult[iLength]    = pA[iLength] - pB[iLength];
    }

};

#endif // !defined(__BASICUTILS_HEADER_INCLUDED__)
