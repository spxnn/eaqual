/////////////////////////////////////////////////////////////////////////////////////
// KogModel.h: interface for the CKogModel class.
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


#if !defined(__KOGMODEL_HEADER_INCLUDED__)
#define __KOGMODEL_HEADER_INCLUDED__

#include "BasicUtils.h"

/////////////////////////////////////////////////////////////////////////////////////
//  @class:         CKogModel
//  @derived_from:  CBasicUtils
//  @variables:     - eaqsTablesNN  *m_pNNTables    pointer on tables for neural network
//					- zFLOAT		*m_pfMOVs		pointer on MOVs
//  @methods:       - kmDIX2ODG
//                  - kmNN
//                  - kmSigmoid
//                  - kmWeight
//                  -Process
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         transformation from MOVs to ODG
//  @creation_date: 01.12.2001
//  @last_modified: 06.02.2002 AL: removed memory leak
/////////////////////////////////////////////////////////////////////////////////////
class CKogModel : public CBasicUtils  
{
private:
    eaqsTablesNN    *m_pNNTables;
	zFLOAT			*m_pfMOVs;

    //methods

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
    zERROR          kmWeightInput(  zFLOAT  *pfAllMOV);

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        kmNN
    //  @parameter:     - zFLOAT *pfAllMOV  pointer on all normalized MOVs
    //  @result:        zFLOAT      
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calculate DIX (eq. (95))
    //  @creation_date: 10.07.2000
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    zFLOAT          kmNN(           zFLOAT  *pfAllMOV);

        /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        kmSigmoid
    //  @parameter:     - zFLOAT x  value for sigmoid transfer function
    //  @result:        zFLOAT      
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         nonlinear sigmoid transfer function, inline 
    //  @creation_date: 
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    inline zFLOAT   kmSigmoid(      zFLOAT  x)      { return (1.0F / (1.0F + zplEXP(-x)));};

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        kmDIX2ODG
    //  @parameter:     - zFLOAT fDIX   distortion index value
    //  @result:        zFLOAT      
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         maps fDIX to fODG, inline
    //  @creation_date: 
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    inline zFLOAT   kmDIX2ODG(      zFLOAT  fDIX)   { return ( -3.98F + 4.2F * kmSigmoid(fDIX));};
        
public:
    CKogModel(eaqsTablesNN *pNNTbl);
    virtual ~CKogModel();

    /////////////////////////////////////////////////////////////////////////////////////
    //  @method:        Process
    //  @parameter:     - zFLOAT *pfAllMOV  pointer on all MOVs, ODG and DIX (to be calculated) are at array pos 0,1
    //  @result:        zERROR      
    //  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
    //  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
    //                  - zplane.development (http://www.zplane.de)
    //  @notes:         calculate DIX and ODG from the unscaled MOVs
    //  @creation_date: 11.11.2001
    //  @last_modified: 
    /////////////////////////////////////////////////////////////////////////////////////
    zERROR          Process(        zFLOAT  *pfAllMOV);

};

#endif // !defined(__KOGMODEL_HEADER_INCLUDED__)
