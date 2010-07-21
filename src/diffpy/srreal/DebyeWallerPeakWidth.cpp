/*****************************************************************************
*
* diffpy.srreal     by DANSE Diffraction group
*                   Simon J. L. Billinge
*                   (c) 2009 Trustees of the Columbia University
*                   in the City of New York.  All rights reserved.
*
* File coded by:    Christopher Farrow, Pavol Juhas
*
* See AUTHORS.txt for a list of people who contributed.
* See LICENSE.txt for license information.
*
******************************************************************************
*
* class DebyeWallerPeakWidth -- peak width calculated assuming independent
*     thermal vibrations of atoms forming a pair.
*
* $Id$
*
*****************************************************************************/

#include <cmath>

#include <diffpy/srreal/DebyeWallerPeakWidth.hpp>

using namespace std;
using namespace diffpy::srreal;

// Constructors --------------------------------------------------------------

PeakWidthModelPtr DebyeWallerPeakWidth::create() const
{
    PeakWidthModelPtr rv(new DebyeWallerPeakWidth());
    return rv;
}


PeakWidthModelPtr DebyeWallerPeakWidth::clone() const
{
    PeakWidthModelPtr rv(new DebyeWallerPeakWidth(*this));
    return rv;
}

// Public Methods ------------------------------------------------------------

const string& DebyeWallerPeakWidth::type() const
{
    static const string rv = "debye-waller";
    return rv;
}


double DebyeWallerPeakWidth::calculate(const BaseBondGenerator& bnds) const
{
    double msdval = bnds.msd();
    return this->DebyeWallerPeakWidth::calculateFromMSD(msdval);
}


double DebyeWallerPeakWidth::calculateFromMSD(double msdval) const
{
    const double tofwhm = 2 * sqrt(2 * M_LN2);
    double fwhm = tofwhm * sqrt(msdval);
    return fwhm;
}

// Registration --------------------------------------------------------------

bool reg_DebyeWallerPeakWidth = DebyeWallerPeakWidth().registerThisType();

// End of file
