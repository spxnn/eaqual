/////////////////////////////////////////////////////////////////////////////////////
// PsychoModel.cpp: implementation of the CPsychoModel class.
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


#include "EAQUALCore.h"

#include "PsychoModel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPsychoModel::CPsychoModel(eaqstEAQUALInstance *pInst)
{
    zINT    iCh,
            iNumOfBands = ( pInst->bModelType == _BASIC ) ? _NUM_BANDS_BASIC : _NUM_BANDS_ADVANCED;

    m_pInstance     = pInst;
    m_pTables       = &(pInst->Tables);

    for ( iCh = 0; iCh < m_pInstance->WaveFormat.iNumOfChannel; iCh++)
    {
        m_pfEnLastBlockRef[iCh]                             = new zFLOAT[iNumOfBands];
        m_pfEnLastBlockTest[iCh]                            = new zFLOAT[iNumOfBands];
        m_pInstance->ChannelData[iCh].pfEnLastBlockRef      = m_pfEnLastBlockRef[iCh];
        m_pInstance->ChannelData[iCh].pfEnLastBlockTest     = m_pfEnLastBlockTest[iCh];
        
        // initialize these vectors
        zplSetZero ( m_pfEnLastBlockRef[iCh], iNumOfBands );
        zplSetZero ( m_pfEnLastBlockTest[iCh], iNumOfBands );
    }
    for ( iCh = m_pInstance->WaveFormat.iNumOfChannel; iCh < _NUM_CHANNELS_MAX; iCh++)
    {
        m_pfEnLastBlockRef[iCh]                             = 0;
        m_pfEnLastBlockTest[iCh]                            = 0;
    }
}


CPsychoModel::~CPsychoModel()
{
    for (zINT iCh = 0; iCh < _NUM_CHANNELS_MAX; iCh++)
    {
        if ( m_pfEnLastBlockRef[iCh]        != 0) delete [] m_pfEnLastBlockRef[iCh];
        if ( m_pfEnLastBlockTest[iCh]       != 0) delete [] m_pfEnLastBlockTest[iCh];
//      if ( m_pfEnLastBlockRefFB[iCh]      != 0) delete [] m_pfEnLastBlockRefFB[iCh];
//      if ( m_pfEnLastBlockTestFB[iCh]     != 0) delete [] m_pfEnLastBlockTestFB[iCh];
    }
    
}


