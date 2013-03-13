/***************************************************************************
                          wiequipsettings.cpp  -  K Desktop Planetarium
                             -------------------
    begin                : 2013/09/01
    copyright            : (C) 2013 by Samikshan Bairagya
    email                : samikshan@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QListWidget>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QComboBox>
#include "wiequipsettings.h"
#include "kstars.h"
#include "kstarsdata.h"
#include "Options.h"

WIEquipSettings::WIEquipSettings(KStars* ks): QFrame(ks), m_Ks(ks)
{
    setupUi(this);
    populateScopeListWidget();
    scopeListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    binoDetailsFrame->setEnabled(kcfg_BinocularsCheck->isChecked());
    scopeFrame->setEnabled(kcfg_TelescopeCheck->isChecked());

    connect(kcfg_TelescopeCheck, SIGNAL(toggled(bool)), this, SLOT(slotTelescopeCheck(bool)));
    connect(kcfg_BinocularsCheck, SIGNAL(toggled(bool)), this, SLOT(slotBinocularsCheck(bool)));
    connect(kcfg_NoEquipCheck, SIGNAL(toggled(bool)), this, SLOT(slotNoEquipCheck(bool)));
    connect(scopeListWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this,
            SLOT(slotScopeSelected(QListWidgetItem *)));
    connect(saveNewScopeButton, SIGNAL(clicked()), this, SLOT(slotSaveNewScope()));
}

void WIEquipSettings::populateScopeListWidget()
{
    scopeListWidget->clear();
    ///Get telescope list from KStars user database.
    KStars::Instance()->data()->userdb()->GetAllScopes(m_ScopeList);
    foreach(OAL::Scope *scope, m_ScopeList)
    {
        QListWidgetItem *scopeItem = new QListWidgetItem;
        scopeItem->setText(scope->vendor());
        scopeItem->setData(Vendor, scope->vendor());
        scopeItem->setData(Model, scope->model());
        scopeItem->setData(Aperture, scope->aperture());
        scopeItem->setData(FocalLength, scope->focalLength());
        scopeItem->setData(Type, scope->type());

        scopeListWidget->addItem(scopeItem);
    }
    if (scopeListWidget->count() == 0) return;

    vendorText->setText(scopeListWidget->item(0)->data(Vendor).toString());
    modelText->setText(scopeListWidget->item(0)->data(Model).toString());
    apertureText->setText(scopeListWidget->item(0)->data(Aperture).toString().append(" mm"));
    scopeListWidget->setCurrentItem(scopeListWidget->item(0));
}

void WIEquipSettings::slotTelescopeCheck(bool on)
{
    if (on)
        kcfg_NoEquipCheck->setEnabled(false);
    else
    {
        if (!kcfg_BinocularsCheck->isChecked())
            kcfg_NoEquipCheck->setEnabled(true);
    }
    scopeFrame->setEnabled(on);
}

void WIEquipSettings::slotBinocularsCheck(bool on)
{
    if (on)
        kcfg_NoEquipCheck->setEnabled(false);
    else
    {
        if (!kcfg_TelescopeCheck->isChecked())
            kcfg_NoEquipCheck->setEnabled(true);
    }
    binoDetailsFrame->setEnabled(on);
}

void WIEquipSettings::slotNoEquipCheck(bool on)
{
    if (on)
    {
        kcfg_TelescopeCheck->setEnabled(false);
        kcfg_BinocularsCheck->setEnabled(false);
    }
    else
    {
        kcfg_TelescopeCheck->setEnabled(true);
        kcfg_BinocularsCheck->setEnabled(true);
    }
}

void WIEquipSettings::slotScopeSelected(QListWidgetItem* scopeItem)
{
    if (!scopeItem) return;
    vendorText->setText(scopeItem->data(Vendor).toString());
    modelText->setText(scopeItem->data(Model).toString());
    apertureText->setText(scopeItem->data(Aperture).toString().append(" mm"));

    if (scopeItem->data(Type).toString() == "Reflector")
        m_TelType = ObsConditions::Reflector;
    else if (scopeItem->data(Type).toString() == "Reflector")
        m_TelType = ObsConditions::Refractor;
}

void WIEquipSettings::slotSaveNewScope()
{
    KStars::Instance()->data()->userdb()->AddScope(modelLineEdit->text(), vendorLineEdit->text(), driverComboBox->currentText(),
                                                   typeComboBox->currentText(), focalLenSpinBox->value(), apertureSpinBox->value());
    populateScopeListWidget();       //Reload scope list widget
}

void WIEquipSettings::setAperture()
{
    if (scopeListWidget->count() == 0)
    {
        if (Options::binocularsCheck())
        {
            m_Aperture = kcfg_BinocularsAperture->value();
            return;
        }
        else
        {
            m_Aperture = INVALID_APERTURE;
            return;
        }
    }

    if (!Options::telescopeCheck() && !Options::binocularsCheck())
    {
        m_Aperture = INVALID_APERTURE;
    }
    else if (!Options::telescopeCheck())    //No telescope available, but binoculars available
    {
        m_Aperture = kcfg_BinocularsAperture->value();
    }
    else if (!Options::binocularsCheck())   //No binoculars available, but telescope available
    {
        if (scopeListWidget->count() == 0)
        {
                m_Aperture = INVALID_APERTURE;
                return;
        }
        else
            m_Aperture = scopeListWidget->currentItem()->data(Aperture).toDouble();
    }
    else                                    //Both Telescope and Binoculars available
    {
        if (scopeListWidget->count() == 0)
        {
            m_Aperture = kcfg_BinocularsAperture->value();
            return;
        }
        //If both Binoculars and Telescope available then select bigger aperture
        double telAperture = scopeListWidget->currentItem()->data(Aperture).toDouble();
        double binoAperture = kcfg_BinocularsAperture->value();
        m_Aperture = telAperture > binoAperture ? telAperture : binoAperture;
    }
}