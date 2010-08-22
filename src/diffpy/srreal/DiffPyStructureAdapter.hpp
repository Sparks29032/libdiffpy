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
* class DiffPyStructureAdapter -- adapter to the Structure class from the
*     Python diffpy.Structure package.
* class DiffPyStructureBondGenerator -- bond generator
*
*
* $Id$
*
*****************************************************************************/

#ifndef DIFFPYSTRUCTUREADAPTER_HPP_INCLUDED
#define DIFFPYSTRUCTUREADAPTER_HPP_INCLUDED

#include <memory>
#include <vector>
#include <boost/python.hpp>
#include <boost/unordered_map.hpp>

#include <diffpy/srreal/StructureAdapter.hpp>
#include <diffpy/srreal/Lattice.hpp>

namespace diffpy {
namespace srreal {

class PointsInSphere;

class DiffPyStructureAdapter : public StructureAdapter
{
    public:

        // constructors
        DiffPyStructureAdapter(boost::python::object);

        // methods - overloaded
        virtual BaseBondGenerator* createBondGenerator() const;
        virtual int countSites() const;
        virtual double numberDensity() const;
        virtual const R3::Vector& siteCartesianPosition(int idx) const;
        virtual double siteOccupancy(int idx) const;
        virtual bool siteAnisotropy(int idx) const;
        virtual const R3::Matrix& siteCartesianUij(int idx) const;
        virtual const std::string& siteAtomType(int idx) const;
        virtual void customPQConfig(PairQuantity* pq) const;

        // methods - own
        const Lattice& getLattice() const;

    private:

        // methods
        void fetchPythonData(boost::python::object dpstru);
        bool isPeriodic() const;

        // data
        Lattice mlattice;
        std::vector<R3::Vector> mcartesian_positions;
        std::vector<double> moccupancies;
        std::vector<bool> manisotropies;
        std::vector<R3::Matrix> mcartesian_uijs;
        std::vector<std::string> matomtypes;
        boost::unordered_map<std::string, double> mpdffit;

};


class DiffPyStructureBaseBondGenerator : public BaseBondGenerator
{
    public:

        // constructors
        DiffPyStructureBaseBondGenerator(const DiffPyStructureAdapter*);

    protected:

        // data
        const DiffPyStructureAdapter* mdpstructure;
};


class DiffPyStructurePeriodicBondGenerator : public DiffPyStructureBaseBondGenerator
{
    public:

        // constructors
        DiffPyStructurePeriodicBondGenerator(const DiffPyStructureAdapter*);

        // methods
        // loop control
        virtual void rewind();

        // configuration
        virtual void setRmin(double);
        virtual void setRmax(double);

        // data access
        virtual const R3::Vector& r0() const;
        virtual const R3::Vector& r1() const;

    protected:

        // methods
        virtual bool iterateSymmetry();
        virtual void rewindSymmetry();
        virtual void getNextBond();

    private:

        // data
        std::auto_ptr<PointsInSphere> msphere;
        std::vector<R3::Vector> mcartesian_positions_uc;
        R3::Vector mrcsphere;
};


}   // namespace srreal
}   // namespace diffpy

#endif  // DIFFPYSTRUCTUREADAPTER_HPP_INCLUDED
