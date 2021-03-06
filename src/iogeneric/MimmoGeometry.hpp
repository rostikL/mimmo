/*---------------------------------------------------------------------------*\
 *
 *  mimmo
 *
 *  Copyright (C) 2015-2017 OPTIMAD engineering Srl
 *
 *  -------------------------------------------------------------------------
 *  License
 *  This file is part of mimmo.
 *
 *  mimmo is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License v3 (LGPL)
 *  as published by the Free Software Foundation.
 *
 *  mimmo is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 *  License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with mimmo. If not, see <http://www.gnu.org/licenses/>.
 *
\*---------------------------------------------------------------------------*/
#ifndef __MIMMOGEOMETRY_HPP__
#define __MIMMOGEOMETRY_HPP__

#include "BaseManipulation.hpp"
#include "MimmoNamespace.hpp"

BETTER_ENUM(FileType, int, STL = 0, STVTU = 1, SQVTU = 2, VTVTU = 3, VHVTU = 4, NAS = 5, OFP = 6, PCVTU = 7, CURVEVTU = 8);

namespace mimmo{

class NastranInterface;

/*!
 * \enum WFORMAT
 * \ingroup iogeneric
 * Format of data to read/write the Nastran geometries.
 */
enum WFORMAT{    /*!Single precision data.*/        Short,
    /*!Double precision data.*/        Long
};


/*!
 * \class MimmoGeometry
 * \ingroup iogeneric
 * \brief MimmoGeometry is an executable block class wrapping(linking or internally instantiating) a Mimmo Object, handling geometry.
 *
 *  MimmoGeometry is the object to manage the import/export/substantial modifications of geometries. It returns a
 *  standard MimmoObject class as product of its execution;
 *  The mesh to import/export has to be a mesh with constant type elements.
 *  The valid format are: binary .stl, ascii .vtu (triangle/quadrilateral elements) and
 *  ascii .nas (triangle elements) for surface mesh; ascii .vtu (tetra/hexa elements)
 *  for volume mesh.
 * 
 * \n
 * It uses smart enums FileType list of available geometry formats, which are:
 * 
 * - <B>STL     = 0</B>    Ascii/Binary triangulation stl.
 * - <B>STVTU     = 1</B> Surface triangulation vtu.
 * - <B>SQVTU     = 2</B> Surface quadrilateral vtu.
 * - <B>VTVTU     = 3</B> Volume tetrahedral VTU.
 * - <B>VHVTU     = 4</B> Volume hexahedral VTU.
 * - <B>NAS     = 5</B> Nastran triangulation nas.
 * - <B>OFP     = 6</B> Ascii OpenFoam point cloud.
 * - <B>PCVTU   = 7</B> Point Cloud VTU
 * - <B>CURVEVTU= 8</B> 3D Curve in VTU
 *
 * Outside this list of options, the class cannot hold any other type of formats for now.
 * The smart enum can be recalled in every moment in your code, just using <tt>mimmo::FileType</tt>
 * and including MimmoGeometry header.
 *
 * \n
 * Ports available in MimmoGeometry Class :
 *
 *    =========================================================

     |                 Port Input  |||                                 |
     |-------|----------|-------------------|-----------------------|
    |<B>PortID</B> | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | 99    | M_GEOM   | m_geometry        | (SCALAR, MIMMO_)      |


     |            Port Output ||               |                       |
     |-------|-----------|-------------------|-----------------------|
    |<B>PortID</B> | <B>PortType</B>   | <B>variable/function</B>  |<B>DataType</B> |
     | 99    | M_GEOM    | getGeometry       | (SCALAR, MIMMO_)      |

 *    =========================================================
 * \n
 *
 * The xml available parameters, sections and subsections are the following :
 *
 * - <B>ClassName</B>: name of the class as <tt>mimmo.Geometry</tt>;
 * - <B>Priority</B>: uint marking priority in multi-chain execution;
 * - <B>ReadFlag</B>: activate reading mode boolean;
 * - <B>ReadDir</B>: reading directory path;
 * - <B>ReadFileType</B>: file type identifier;
 * - <B>ReadFilename</B>: name of file for reading;
 * - <B>WriteFlag</B>: activate writing mode boolean;
 * - <B>WriteDir</B>: writing directory path;
 * - <B>WriteFileType</B>: file type identifier;
 * - <B>WriteFilename</B>: name of file for writing;
 * - <B>Codex</B>: boolean to write ascii/binary;
 * - <B>BvTree</B>: evaluate bvTree true 1/false 0;
 * - <B>KdTree</B>: evaluate kdTree true 1/false 0.
 *
 * In case of writing mode Geometry has to be mandatorily passed through port.
 *
 */
class MimmoGeometry: public BaseManipulation{

private:
    bool         m_read;         /**<If true it reads the geometry from file during the execution.*/
    FileDataInfo m_rinfo;       /**< Info on the external file to read */

    bool         m_write;         /**<If true it writes the geometry on file during the execution.*/
    FileDataInfo m_winfo;       /**< Info on the external file to write */

    bool        m_isInternal;                /**< flag for internal instantiated MimmoObject */
    std::unique_ptr<MimmoObject> m_intgeo;    /**< pointer to internal allocated geometry, if any */
    bool        m_codex;                    /**< Set codex format for writing true binary, false ascii */
    WFORMAT        m_wformat;                    /**<Format for .nas import/export. (Short/Long).*/

    bool        m_buildBvTree;                /**<If true the simplex ordered BvTree of the geometry is built in execution, whenever geometry support simplicies. */
    bool        m_buildKdTree;                /**<If true the vertex ordered KdTree of the geometry is built in execution*/

public:
    MimmoGeometry();
    MimmoGeometry(const bitpit::Config::Section & rootXML);
    virtual ~MimmoGeometry();

    MimmoGeometry(const MimmoGeometry & other);
    MimmoGeometry & operator=(const MimmoGeometry & other);

    void buildPorts();

    const MimmoGeometry *    getCopy();
    MimmoObject *     getGeometry();
    const MimmoObject *     getGeometry()const ;

    void        setReadDir(std::string dir);
    void        setReadFileType(FileType type);
    void        setReadFileType(int type);
    void        setWriteFileType(FileType type);
    void        setWriteFileType(int type);
    void        setRead(bool read);
    void        setWriteDir(std::string dir);
    void        setReadFilename(std::string filename);
    void        setWrite(bool write);
    void        setWriteFilename(std::string filename);
    void        setCodex(bool binary = true);

    void        setHARDCopy( const MimmoGeometry * other);
    void        setSOFTCopy( const MimmoGeometry * other);

    void        setGeometry( MimmoObject * external);
    void        setGeometry(int type=1);

    bitpit::PiercedVector<bitpit::Vertex> *     getVertices();
    bitpit::PiercedVector<bitpit::Cell> *         getCells();

    void        setVertices(bitpit::PiercedVector<bitpit::Vertex> * vertices);
    void        setCells(bitpit::PiercedVector<bitpit::Cell> * cells);

    void        setPID(shivector1D pids);
    void        setPID(std::unordered_map<long,short> pidsMap);

    void        setFormatNAS(WFORMAT wform);

    void        setBuildBvTree(bool build);
    void        setBuildKdTree(bool build);

    bool         isEmpty();
    bool        isInternal();
    void         clear();
    bool        write();
    bool        read();

    void         execute();

    void         readOFP(std::string& inputDir, std::string& surfaceName, dvecarr3E& points);
    void         writeOFP(std::string& outputDir, std::string& surfaceName, dvecarr3E& points);

    virtual void absorbSectionXML(const bitpit::Config::Section & slotXML, std::string name="");
    virtual void flushSectionXML(bitpit::Config::Section & slotXML, std::string name="");

private:
    void     setDefaults();

};

/*!
 * \class NastranInterface
 * \ingroup iogeneric
 * \brief NastranInterface is an interface class for I/O handling of BDF bulk nastran format *.nas.
 */
class NastranInterface{
    static const char nl = '\n'; /**< static const declaration of new line command. */
public:

    WFORMAT    wformat; /**< member storing the file type format Short/Long */

    void setWFormat(WFORMAT);
    void writeKeyword(std::string key, std::ofstream& os);
    void writeCoord(darray3E & p, int& pointI, std::ofstream& os);
    void writeFace(std::string faceType, ivector1D& facePts, int& nFace, std::ofstream& os, int PID);
    void writeGeometry(dvecarr3E& points, ivector2D& faces, std::ofstream& os, shivector1D* PIDS = NULL);
    void writeFooter(std::ofstream& os, std::unordered_set<short>* PIDSSET = NULL);
    void write(std::string& outputDir, std::string& surfaceName, dvecarr3E& points, ivector2D& faces, shivector1D* PIDS = NULL, std::unordered_set<short>* PIDSSET = NULL);
    void read(std::string& inputDir, std::string& surfaceName, dvecarr3E& points, ivector2D& faces, shivector1D& PIDS);

    std::string trim(std::string in);
    std::string convertVertex(std::string in);

    /*!
     * Write a template value according to WFORMAT chosen by the User
     * \param[in]     value    value of class Type
     * \param[in,out] os    ofstream where the value is written
     */
    template<class Type>
    void writeValue (Type& value, std::ofstream& os){

        int offset = 5;
        if (wformat == Long) offset = 13;

        std::stringstream manip;
        manip << value;
        std::string manips = manip.str();
        std::string mantissa, expon;
        int pos = manips.find("E");
        if (pos < manips.size()){
            mantissa = manips.substr(0,std::min(offset,pos));
            expon = manips.substr(pos+1,manips.size());
            manips = mantissa + expon;
        }
        pos = manips.find("e");
        if (pos < manips.size()){
            mantissa = manips.substr(0,std::min(offset,pos));
            expon = manips.substr(pos+1,manips.size());
            manips = mantissa + expon;
        }
        manips = manips.substr(0, offset+3);

        switch (wformat)
        {
        case Short:
        {
            os << std::setw(8) << manips;
            break;
        }
        case Long:
        {
            os << std::setw(16) << manips;
            break;
        }
        }
    }
};

REGISTER(BaseManipulation, MimmoGeometry, "mimmo.Geometry")

};

#endif /* __MIMMOGEOMETRY_HPP__ */
