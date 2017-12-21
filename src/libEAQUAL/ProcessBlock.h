/////////////////////////////////////////////////////////////////////////////////////
// ProcessBlock.h: interface for the CProcessBlock class.
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


#if !defined(__PROCESSBLOCK_HEADER_INCLUDED__)
#define __PROCESSBLOCK_HEADER_INCLUDED__

#include "BasicUtils.h"

/////////////////////////////////////////////////////////////////////////////////////
//  @class:         CProcessBlock
//  @derived_from:  CBasicUtils
//  @variables:     -
//  @methods:       -Process
//  @author:        Alexander Lerch (AL) (mailto:lerch@zplane.de)
//  @company:       - Heinrich- Hertz-Institut fuer Nachrichtentechnik GmbH (http://www.hhi.de)
//                  - zplane.development (http://www.zplane.de)
//  @notes:         
//  @creation_date: 28.10.2001
//  @last_modified: 
/////////////////////////////////////////////////////////////////////////////////////
class CProcessBlock : public CBasicUtils  
{
public:
    CProcessBlock(){};
    virtual ~CProcessBlock(){};

    virtual zERROR Process(eaqstEAQUALInstance *pInstance) = 0;

};

#endif // !defined(__PROCESSBLOCK_HEADER_INCLUDED__)
