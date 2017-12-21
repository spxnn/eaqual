/////////////////////////////////////////////////////////////////////////////////////
// PsychoModel.h: interface for the CPsychoModel class.
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


#if !defined(__PSYCHOMODEL_HEADER_INCLUDED__)
#define __PSYCHOMODEL_HEADER_INCLUDED__


#include "ProcessBlock.h"

/////////////////////////////////////////////////////////////////////////////////////
//  @class:         CPsychoModel
//  @derived_from:  CProcessBlock
//  @variables:     - zFLOAT    *m_pfLastBlockRef       pointer on low passed excitation patterns reference
//                  - zFLOAT    *m_pfLastBlockTest      pointer on low passed excitation patterns test
//  @methods:       - pmAddNoise
//                  - pmOuterEar
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         base class for psychomodels
//  @creation_date: 01.12.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
class CPsychoModel : public CProcessBlock  
{
public:

    eaqstEAQUALInstance *m_pInstance;
    eaqsTables          *m_pTables;
    
    zFLOAT              *m_pfEnLastBlockRef[_NUM_CHANNELS_MAX],
                        *m_pfEnLastBlockTest[_NUM_CHANNELS_MAX];
//                      *m_pfEnLastBlockRefInt[_NUM_CHANNELS_MAX],
//                      *m_pfEnLastBlockTestInt[_NUM_CHANNELS_MAX],
//                      *m_pfEnLastBlockRefIntFB[_NUM_CHANNELS_MAX],
//                      *m_pfEnLastBlockTestIntFB[_NUM_CHANNELS_MAX],
//                      *m_pfEnLastBlockRefFB[_NUM_CHANNELS_MAX],
//                      *m_pfEnLastBlockTestFB[_NUM_CHANNELS_MAX];
    
    
    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        pmOuterEar
    //  @parameter:     - zFLOAT *pfSpecData        _energy_ vector to be weighted
    //                  - zFLOAT    *pfIntNoise     table with internal noise
    //                  - zINT  iLength         length of vectors
    //  @result:        -       
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         input weighting for neural network (part of eq. (95))
    //  @creation_date: 26.06.2000
    //  @last_modified: 12.11.2001 AL: changed input params
    /////////////////////////////////////////////////////////////////////////////////////
    inline  void    pmOuterEar(     zFLOAT      *pfSpecData, 
                                    zFLOAT      *pfOuterEar,
                                    zINT        iLength)
                                    {   zplMultVec(pfSpecData, pfOuterEar, iLength);};
    
    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        pmAddNoise
    //  @parameter:     - zFLOAT *pfSpecData        _energy_ vector to be weighted
    //                  - zFLOAT    *pfIntNoise     table with internal noise
    //                  - zINT  iNumOfBannds    length of both vectors
    //  @result:        -       
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         iadding internal noise to power spectrum (BS.1387, p.212)
    //  @creation_date: 04.07.2000
    //  @last_modified: 12.11.2001 AL: changed input params
    /////////////////////////////////////////////////////////////////////////////////////
    inline  void    pmAddNoise(     zFLOAT      *pfSpecData, 
                                    zFLOAT      *pfIntNoise, 
                                    zINT        iNumOfBands)
                                    {   zplAddVec(pfSpecData, pfIntNoise, iNumOfBands);};
    
    CPsychoModel(           eaqstEAQUALInstance *pInstance);

    virtual ~CPsychoModel();

};

#endif // !defined(__PSYCHOMODEL_HEADER_INCLUDED__)
