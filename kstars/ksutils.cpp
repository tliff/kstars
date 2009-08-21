/***************************************************************************
                          ksutils.cpp  -  K Desktop Planetarium
                             -------------------
    begin                : Mon Jan  7 10:48:09 EST 2002
    copyright            : (C) 2002 by Mark Hollomon
    email                : mhh@mindspring.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "ksutils.h"

#include <QFile>

#include <kstandarddirs.h>

bool KSUtils::openDataFile( QFile &file, const QString &s ) {
    QString FileName = KStandardDirs::locate( "appdata", s );
    if ( !FileName.isNull() ) {
        file.setFileName( FileName );
        return file.open( QIODevice::ReadOnly );
    }
    return false;
}

long double KSUtils::lagrangeInterpolation( const long double x[], const long double v[], int n, long double xval) {
    long double value = 0;
    for (int i=1; i<n; ++i) {
        long double c = 1.0;
        for (int j = 1; j<n;++j)
            if (i != j)
                c *= (xval - x[j]) / (x[i] - x[j]);
        value += c *v[i];
    }

    return value;
}
