/** *************************************************************************
                          skypointlite.cpp  -  K Desktop Planetarium
                             -------------------
    begin                : 20/07/2016
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
#include "skypointlite.h"
#include "skyobject.h"

SkyPointLite::SkyPointLite()
    :point(NULL)
{

}

void SkyPointLite::setPoint(SkyPoint * p)
{
    point = p;
}
