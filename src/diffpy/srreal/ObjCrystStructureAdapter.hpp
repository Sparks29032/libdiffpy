/*****************************************************************************
*
* diffpy.srreal     by DANSE Diffraction group
*                   Simon J. L. Billinge
*                   (c) 2009 Trustees of the Columbia University
*                   in the City of New York.  All rights reserved.
*
* File coded by:    Chris Farrow
*
* See AUTHORS.txt for a list of people who contributed.
* See LICENSE.txt for license information.
*
******************************************************************************
*
* class ObjCrystStructureAdapter   
*   -- adapter to the Crystal class from ObjCryst++.
* class ObjCrystAperiodicBondGenerator 
*   -- Generate bonds from aperiodic ObjCrystStructureAdapter.
* class ObjCrystPeriodicBondGenerator     
*   -- Generate bonds from periodic ObjCrystStructureAdapter.
*
* $Id$
*
*****************************************************************************/

#ifndef OBJCRYSTSTRUCTUREADAPTER_HPP_INCLUDED
#define OBJCRYSTSTRUCTUREADAPTER_HPP_INCLUDED

#include <memory>
#include <set>
#include <vector>

#include <diffpy/srreal/R3linalg.hpp>
#include <diffpy/srreal/StructureAdapter.hpp>
#include <diffpy/srreal/BaseBondGenerator.hpp>
#include <diffpy/srreal/Lattice.hpp>

#include <ObjCryst/ObjCryst/Crystal.h>
#include <ObjCryst/ObjCryst/Molecule.h>
#include <ObjCryst/ObjCryst/ScatteringPower.h>
#include <ObjCryst/ObjCryst/SpaceGroup.h>

namespace diffpy {
namespace srreal {


class PointsInSphere;

class ObjCrystStructureAdapter : public StructureAdapter
{
    friend class ObjCrystAperiodicBondGenerator;
    friend class ObjCrystPeriodicBondGenerator;

    public:

        // constructors
        ObjCrystStructureAdapter(const ObjCryst::Crystal&);

        // methods - overloaded
        virtual BaseBondGenerator* createBondGenerator() const;
        virtual int countSites() const;
        virtual double numberDensity() const;
        virtual const R3::Vector& siteCartesianPosition(int idx) const;
        virtual double siteOccupancy(int idx) const;
        virtual bool siteAnisotropy(int idx) const;
        virtual double siteMultiplicity(int idx) const;
        virtual const R3::Matrix& siteCartesianUij(int idx) const;
        virtual const std::string& siteAtomType(int idx) const;
        const Lattice& getLattice() const;
        bool isPeriodic() const;


    private:

        typedef std::vector<R3::Vector> SymPosVec;
        typedef std::vector<R3::Matrix> SymUijVec;

        // methods - own
        void getPeriodicUnitCell();
        void getAperiodicUnitCell();

        // Tolerance on distance measurements.  Two sites are the same if
        // their fractional coordinates are within this tolerance
        static const double mtoler;
        // The ObjCryst::Crystal
        const ObjCryst::Crystal* mpcryst;
        // The asymmetric unit cell of ScatteringComponent instances
        std::vector< ObjCryst::ScatteringComponent > mvsc;
        // The symmetry-related positions of the asymmetric unit cell
        std::vector<SymPosVec> mvsym;
        // The Uij for scatterers. Stored in same order as mvsym.
        std::vector<SymUijVec> mvuij;
        // The Lattice instance needed by the bond generator
        Lattice mlattice;

};


class ObjCrystAperiodicBondGenerator : public BaseBondGenerator
{
    public:

        // constructors
        ObjCrystAperiodicBondGenerator(const ObjCrystStructureAdapter*);

        // methods

        // data access
        virtual const R3::Vector& r1() const;
        virtual double msd0() const;
        virtual double msd1() const;

    protected:

        // methods
        virtual bool iterateSymmetry();
        virtual void rewindSymmetry();

        // data

        // The adapted structure
        const ObjCrystStructureAdapter* mpstructure;
        // Index over symmetry
        size_t msymidx;

    private:

        double msd(int siteidx, int symidx) const;

};


class ObjCrystPeriodicBondGenerator : public ObjCrystAperiodicBondGenerator
{

    public:

        // constructors
        ObjCrystPeriodicBondGenerator(const ObjCrystStructureAdapter*);

        // loop control
        virtual void rewind();

        // data access
        virtual const R3::Vector& r1() const;

        // configuration
        virtual void setRmin(double);
        virtual void setRmax(double);

    protected:

        // methods
        virtual bool iterateSymmetry();
        virtual void rewindSymmetry();

    private:

        std::auto_ptr<PointsInSphere> msphere;
};

// Adapter for ObjCryst::Molecule
//
// Molecules are always considered aperiodic. The anisotropic ADPs are treated
// as if in a cartesian cell. If this is not what is intended, pass the
// molecule as a scattering component within an ObjCryst::Crystal.

class ObjCrystMoleculeAdapter : public StructureAdapter
{

    friend class ObjCrystMoleculeBondGenerator;

    public:

        // constructors
        ObjCrystMoleculeAdapter(const ObjCryst::Molecule&);

        // methods - overloaded
        virtual BaseBondGenerator* createBondGenerator() const;
        virtual int countSites() const;
        virtual double numberDensity() const;
        virtual const R3::Vector& siteCartesianPosition(int idx) const;
        virtual double siteOccupancy(int idx) const;
        virtual bool siteAnisotropy(int idx) const;
        virtual const R3::Matrix& siteCartesianUij(int idx) const;
        virtual const std::string& siteAtomType(int idx) const;
        const Lattice& getLattice() const;


    private:

        // The ObjCryst::Molecule
        const ObjCryst::Molecule* mpmolecule;
        // The MolAtom instances
        std::vector< ObjCryst::MolAtom > mvatoms;
        // The positions of the scatterers. Same order as mvatoms.
        std::vector<R3::Vector> mvpos;
        // The Uij for scatterers. Same order as mvatoms.
        std::vector<R3::Matrix> mvuij;
        // The Lattice instance needed by the bond generator
        Lattice mlattice;

};


class ObjCrystMoleculeBondGenerator : public BaseBondGenerator
{
    public:

        // constructors
        ObjCrystMoleculeBondGenerator(const ObjCrystMoleculeAdapter*);

        // data access
        virtual double msd0() const;
        virtual double msd1() const;

    protected:

        // The adapted structure
        const ObjCrystMoleculeAdapter* mpstructure;

    private:

    double msd(int siteidx) const;

};


namespace objcrystutil
{
    R3::Matrix getUij(const ObjCryst::ScatteringPower* sp);
} // namespace objcrystutil


inline
StructureAdapter* 
createPQAdapter(const ObjCryst::Crystal& cryst)
{
    StructureAdapter* adapter = new ObjCrystStructureAdapter(cryst);
    return adapter;
}


inline
StructureAdapter* 
createPQAdapter(const ObjCryst::Molecule& molecule)
{
    StructureAdapter* adapter = new ObjCrystMoleculeAdapter(molecule);
    return adapter;
}


}   // namespace srreal
}   // namespace diffpy

#endif  // OBJCRYSTSTRUCTUREADAPTER_HPP_INCLUDED
