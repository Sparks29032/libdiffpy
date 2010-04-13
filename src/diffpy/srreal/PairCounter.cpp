/*****************************************************************************
*
* diffpy.srreal     by DANSE Diffraction group
*                   Simon J. L. Billinge
*                   (c) 2009 Trustees of the Columbia University
*                   in the City of New York.  All rights reserved.
*
* File coded by:    Pavol Juhas
*
* See AUTHORS.txt for a list of people who contributed.
* See LICENSE.txt for license information.
*
******************************************************************************
*
* class PairCounter -- concrete counter of pairs in a structure.
*
* $Id$
*
*****************************************************************************/

#include <diffpy/srreal/PairCounter.hpp>

using namespace diffpy::srreal;

// Protected Methods ---------------------------------------------------------

void PairCounter::addPairContribution(const BaseBondGenerator& bnds,
        int summationscale)
{
    mvalue.front() += summationscale / 2.0;
}

// End of file
