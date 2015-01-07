/*  Ekos Mount Module
    Copyright (C) 2015 Jasem Mutlaq <mutlaqja@ikarustech.com>

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
 */

#include "mount.h"
#include "Options.h"

#include "indi/driverinfo.h"
#include "indi/indicommon.h"
#include "indi/clientmanager.h"
#include "indi/indifilter.h"

#include "ekosmanager.h"

#include "kstars.h"
#include "kstarsdata.h"
#include "ksutils.h"

#include <basedevice.h>

namespace Ekos
{

Mount::Mount()
{
    setupUi(this);

    stopB->setIcon(QIcon::fromTheme("process-stop"));
    northB->setIcon(QIcon::fromTheme("go-up"));
    westB->setIcon(QIcon::fromTheme("go-previous"));
    eastB->setIcon(QIcon::fromTheme("go-next"));
    southB->setIcon(QIcon::fromTheme("go-down"));


    QFile tempFile;

    if (KSUtils::openDataFile( tempFile, "go-nw.png" ) )
    {
        northwestB->setIcon(QIcon(tempFile.fileName()));
        tempFile.close();
    }

    if (KSUtils::openDataFile( tempFile, "go-ne.png" ) )
    {
        northeastB->setIcon(QIcon(tempFile.fileName()));
        tempFile.close();
    }

    if (KSUtils::openDataFile( tempFile, "go-sw.png" ) )
    {
        southwestB->setIcon(QIcon(tempFile.fileName()));
        tempFile.close();
    }

    if (KSUtils::openDataFile( tempFile, "go-se.png" ) )
    {
        southeastB->setIcon(QIcon(tempFile.fileName()));
        tempFile.close();
    }

    connect(northB, SIGNAL(pressed()), this, SLOT(move()));
    connect(northB, SIGNAL(released()), this, SLOT(stop()));
    connect(westB, SIGNAL(pressed()), this, SLOT(move()));
    connect(westB, SIGNAL(released()), this, SLOT(stop()));
    connect(southB, SIGNAL(pressed()), this, SLOT(move()));
    connect(southB, SIGNAL(released()), this, SLOT(stop()));
    connect(eastB, SIGNAL(pressed()), this, SLOT(move()));
    connect(eastB, SIGNAL(released()), this, SLOT(stop()));
    connect(northeastB, SIGNAL(pressed()), this, SLOT(move()));
    connect(northeastB, SIGNAL(released()), this, SLOT(stop()));
    connect(northwestB, SIGNAL(pressed()), this, SLOT(move()));
    connect(northwestB, SIGNAL(released()), this, SLOT(stop()));
    connect(southeastB, SIGNAL(pressed()), this, SLOT(move()));
    connect(southeastB, SIGNAL(released()), this, SLOT(stop()));
    connect(southwestB, SIGNAL(pressed()), this, SLOT(move()));
    connect(southwestB, SIGNAL(released()), this, SLOT(stop()));


}

Mount::~Mount()
{

}

void Mount::setTelescope(ISD::GDInterface *newTelescope)
{
    currentTelescope = static_cast<ISD::Telescope*> (newTelescope);

    connect(currentTelescope, SIGNAL(numberUpdated(INumberVectorProperty*)), this, SLOT(updateScopeCoords(INumberVectorProperty*)), Qt::UniqueConnection);

    syncTelescopeInfo();
}

void Mount::syncTelescopeInfo()
{
    INumberVectorProperty * nvp = currentTelescope->getBaseDevice()->getNumber("TELESCOPE_INFO");

    if (nvp)
    {

        primaryScopeGroup->setTitle(currentTelescope->getDeviceName());
        guideScopeGroup->setTitle(xi18n("%1 guide scope", currentTelescope->getDeviceName()));

        INumber *np = NULL;

        np = IUFindNumber(nvp, "TELESCOPE_APERTURE");
        if (np && np->value > 0)
            primaryScopeApertureIN->setValue(np->value);

        np = IUFindNumber(nvp, "TELESCOPE_FOCAL_LENGTH");
        if (np && np->value > 0)
            primaryScopeFocalIN->setValue(np->value);

        np = IUFindNumber(nvp, "GUIDER_APERTURE");
        if (np && np->value > 0)
            guideScopeApertureIN->setValue(np->value);

        np = IUFindNumber(nvp, "GUIDER_FOCAL_LENGTH");
        if (np && np->value > 0)
            guideScopeFocalIN->setValue(np->value);

    }

}

void Mount::updateScopeCoords(INumberVectorProperty *coord)
{
    if (!strcmp(coord->name, "EQUATORIAL_EOD_COORD"))
    {
        telescopeCoord.setRA(coord->np[0].value);
        telescopeCoord.setDec(coord->np[1].value);
        telescopeCoord.EquatorialToHorizontal(KStarsData::Instance()->lst(), KStarsData::Instance()->geo()->lat());

        raOUT->setText(telescopeCoord.ra().toHMSString());
        decOUT->setText(telescopeCoord.dec().toDMSString());
        azOUT->setText(telescopeCoord.az().toDMSString());
        altOUT->setText(telescopeCoord.alt().toDMSString());
    }
}

void Mount::appendLogText(const QString &text)
{
    logText.insert(0, xi18nc("log entry; %1 is the date, %2 is the text", "%1 %2", QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss"), text));

    emit newLog();
}


void Mount::clearLog()
{
    logText.clear();
    emit newLog();
}

void Mount::move()
{
    QObject* obj = sender();

    if (obj == northB)
    {
        currentTelescope->MoveNS(ISD::Telescope::MOTION_NORTH);
    }
    else if (obj == westB)
    {
        currentTelescope->MoveWE(ISD::Telescope::MOTION_WEST);
    }
    else if (obj == southB)
    {
        currentTelescope->MoveNS(ISD::Telescope::MOTION_SOUTH);
    }
    else if (obj == eastB)
    {
        currentTelescope->MoveWE(ISD::Telescope::MOTION_EAST);
    }
    else if (obj == northwestB)
    {
        currentTelescope->MoveNS(ISD::Telescope::MOTION_NORTH);
        currentTelescope->MoveWE(ISD::Telescope::MOTION_WEST);
    }
    else if (obj == northeastB)
    {
        currentTelescope->MoveNS(ISD::Telescope::MOTION_NORTH);
        currentTelescope->MoveWE(ISD::Telescope::MOTION_EAST);
    }
    else if (obj == southwestB)
    {
        currentTelescope->MoveNS(ISD::Telescope::MOTION_SOUTH);
        currentTelescope->MoveWE(ISD::Telescope::MOTION_WEST);
    }
    else if (obj == southeastB)
    {
        currentTelescope->MoveNS(ISD::Telescope::MOTION_SOUTH);
        currentTelescope->MoveWE(ISD::Telescope::MOTION_EAST);
    }
}

void Mount::stop()
{
    currentTelescope->Abort();
}

}
