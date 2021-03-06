/** *************************************************************************
                          supernovanode.h  -  K Desktop Planetarium
                             -------------------
    begin                : 26/06/2016
    copyright            : (C) 2016 by Artem Fedoskin
    email                : afedoskin3@gmail.com
 ***************************************************************************/
/** *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef SUPERNOVANODE_H_
#define SUPERNOVANODE_H_
#include "skynode.h"

class PolyNode;
class Supernova;
class PointNode;
class QSGFlatColorMaterial;

/** @class SupernovaNode
 *
 * @short A SkyNode derived class that represents supernova
 * @author Artem Fedoskin
 * @version 1.0
 */

class SupernovaNode : public SkyNode
{
    public:
        /**
         * @short Constructor.
         * @param snova - pointer to supernova that needs to be represented by this node
         */
        SupernovaNode(Supernova * snova);

        /**
         * @short Update position and visibility of supernova. Initialize m_lines if not already done
         */
        virtual void update() override;

        void changePos(QPointF pos);

        inline Supernova * snova()
        {
            return m_snova;
        }

    private:
        Supernova * m_snova;

        QSGGeometryNode * m_lines;

        QSGFlatColorMaterial * m_material;
        QSGGeometry * m_geometry;

        PointNode * m_point;
};

#endif


