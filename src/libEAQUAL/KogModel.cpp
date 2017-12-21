/////////////////////////////////////////////////////////////////////////////////////
// KogModel.cpp: implementation of the CKogModel class.
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

#include "KogModel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKogModel::CKogModel(eaqsTablesNN *pNNTbl)
{
    m_pNNTables = pNNTbl;
	m_pfMOVs	= 0;
    m_pfMOVs	= new zFLOAT[m_pNNTables->piNNSize[0]];
}

CKogModel::~CKogModel()
{
    if (m_pfMOVs) delete [] m_pfMOVs;
	
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        kmWeightInput
//  @parameter:     - zFLOAT *pfAllMOV  pointer on all MOVs
//  @result:        zERROR      
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         input weighting for neural network (part of eq. (95))
//  @creation_date: 10.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zERROR  CKogModel::kmWeightInput(zFLOAT *pfAllMOV)
{
    zINT    iMOV;
    zINT    *piNNSize   = m_pNNTables->piNNSize;
    zFLOAT  *pfNormMin  = m_pNNTables->pfNNNormMin,
            *pfNormMax  = m_pNNTables->pfNNNormMax;

    for ( iMOV = 0; iMOV < piNNSize[0]; iMOV++)
    {
        pfAllMOV[iMOV]     -= pfNormMin[iMOV];
        pfAllMOV[iMOV]     /= (pfNormMax[iMOV] - pfNormMin[iMOV]);

        #if (TRUNC_MOV == ON)
            if (pfAllMOV[iMOV] < 0)
                pfAllMOV[iMOV]  = 0;
            else if (pfAllMOV[iMOV] > 1)
                pfAllMOV[iMOV]  = 1;
        #endif
    }
    return _NO_ERROR;
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        kmNN
//  @parameter:     - zFLOAT *pfAllMOV  pointer on all normalized MOVs
//  @result:        zFLOAT  DIX 
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculate DIX (eq. (95))
//  @creation_date: 10.07.2000
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
zFLOAT CKogModel::kmNN(zFLOAT *pfAllMOV)
{
    zFLOAT      fTmp[_NUM_NN_LAYER][_NUM_MOV_BASIC];

    zINT        iMOV,
                iNeuron,
                iLayer;
    zINT        *piNNSize   = m_pNNTables->piNNSize;
    zFLOAT      *pfWeights[_NUM_NN_LAYER-1][_NUM_HIDD_NEUR_ADVANCED];

    
    for ( iLayer = 0; iLayer < _NUM_NN_LAYER-1; iLayer++)
        for ( iNeuron = 0; iNeuron < _NUM_HIDD_NEUR_ADVANCED; iNeuron++)
            pfWeights[iLayer][iNeuron]  = m_pNNTables->pfNNWeights[iLayer][iNeuron];
    

    for (iMOV = 0; iMOV < piNNSize[0]; iMOV++)
        fTmp[0][iMOV]               = pfAllMOV[iMOV];

    for ( iLayer = 0; iLayer < _NUM_NN_LAYER-1; iLayer++)
    {
        for ( iNeuron = 0; iNeuron < piNNSize[iLayer+1]; iNeuron++)
        {
            fTmp[iLayer+1][iNeuron] = 0.0F;

            for ( iMOV = 0; iMOV < piNNSize[iLayer]; iMOV++)
                fTmp[iLayer+1][iNeuron]    += pfWeights[iLayer][iNeuron][iMOV] * fTmp[iLayer][iMOV];
            
            fTmp[iLayer+1][iNeuron]        += pfWeights[iLayer][iNeuron][iMOV];
            
            if (iLayer != 1)
                fTmp[iLayer+1][iNeuron]     = kmSigmoid(fTmp[iLayer+1][iNeuron]);
        }
    }
    return fTmp[2][0];
}


/////////////////////////////////////////////////////////////////////////////////////
//  @method:        Process
//  @parameter:     - zFLOAT *pfAllMOV  pointer on all MOVs, ODG and DIX (to be calculated) are at array pos 0,1
//  @result:        zERROR      
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         calculate DIX and ODG from the unscaled MOVs
//  @creation_date: 11.11.2001
//  @last_modified: 06.02.2002 AL: removed memory leak
/////////////////////////////////////////////////////////////////////////////////////
zERROR CKogModel::Process(zFLOAT *pfAllMOV)
{

    zplCopyVec(m_pfMOVs, &pfAllMOV[2], m_pNNTables->piNNSize[0]);

    // scale nn inputs
    kmWeightInput(m_pfMOVs);

    // calculate fDIX
    pfAllMOV[1] = kmNN(m_pfMOVs);

    // calculate fODG
    pfAllMOV[0] = kmDIX2ODG(pfAllMOV[1]);

    return _NO_ERROR;
}
    