/***************************************************************************
                          finddialog.cpp  -  K Desktop Planetarium
                             -------------------
    begin                : Wed Jul 4 2001
    copyright            : (C) 2001 by Jason Harris
    email                : jharris@30doradus.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QTimer>

#include <kmessagebox.h>

#include "finddialog.h"
#include "kstars.h"
#include "kstarsdata.h"
#include "Options.h"
#include "skyobject.h"

FindDialogUI::FindDialogUI( QWidget *parent ) : QFrame( parent ) {
	setupUi( parent );

	FilterType->insertItem( i18n ("Any") );
	FilterType->insertItem( i18n ("Stars") );
	FilterType->insertItem( i18n ("Solar System") );
	FilterType->insertItem( i18n ("Open Clusters") );
	FilterType->insertItem( i18n ("Glob. Clusters") );
	FilterType->insertItem( i18n ("Gas. Nebulae") );
	FilterType->insertItem( i18n ("Plan. Nebulae") );
	FilterType->insertItem( i18n ("Galaxies") );
	FilterType->insertItem( i18n ("Comets") );
	FilterType->insertItem( i18n ("Asteroids") );
	FilterType->insertItem( i18n ("Constellations") );

	SearchList->setMinimumWidth( 256 );
	SearchList->setMinimumHeight( 320 );
}

FindDialog::FindDialog( QWidget* parent ) :
		KDialogBase( KDialogBase::Plain, i18n( "Find Object" ), Ok|Cancel, Ok, parent ),
		currentitem(0), Filter(0)
{
	QFrame *page = plainPage();
	ui = new FindDialogUI( page );

//Connect signals to slots
//	connect( this, SIGNAL( okClicked() ), this, SLOT( accept() ) ) ;
	connect( this, SIGNAL( cancelClicked() ), this, SLOT( reject() ) );
	connect( ui->SearchBox, SIGNAL( textChanged( const QString & ) ), SLOT( filter() ) );
	connect( ui->SearchBox, SIGNAL( returnPressed() ), SLOT( slotOk() ) );
	connect( ui->FilterType, SIGNAL( activated( int ) ), this, SLOT( setFilter( int ) ) );
	connect( ui->SearchList, SIGNAL (itemActivated(QListWidgetItem *)), SLOT (updateSelection (QListWidgetItem *)));
	connect( ui->SearchList, SIGNAL( itemDoubleClicked ( QListWidgetItem *  ) ), SLOT( slotOk() ) );

	// first create and paint dialog and then load list
	QTimer::singleShot(0, this, SLOT( init() ));
}

FindDialog::~FindDialog() {
}

void FindDialog::init() {
	ui->SearchBox->clear();
	ui->FilterType->setCurrentItem(0);  // show all types of objects
	filter();
}

void FindDialog::filter() {  //Filter the list of names with the string in the SearchBox
	KStars *p = (KStars *)parent();

	ui->SearchList->clear();
	QStringList ObjNames;

	QString searchString = ui->SearchBox->text().lower();
		foreach ( QString name, p->data()->skyComposite()->objectNames() ) {
			if ( name.lower().startsWith( searchString ) ) {
				ObjNames.append( name );
/*				if ( i++ >= 5000 ) {              //Every 5000 name insertions,
					kapp->processEvents ( 50 );		//spend 50 msec processing KApplication events
					i = 0;
				}*/
			}
		}
	setListItemEnabled(); // Automatically highlight first item
	ui->SearchBox->setFocus();  // set cursor to QLineEdit
}

void FindDialog::filterByType() {
	KStars *p = (KStars *)parent();

	ui->SearchList->clear();	// QListBox
	QString searchFor = ui->SearchBox->text().lower();  // search string

	QStringList ObjNames;

	foreach ( QString name, p->data()->skyComposite()->objectNames() ) {
		//FIXME: We need pointers to the objects to filter by type
	}

	setListItemEnabled();    // Automatically highlight first item
	ui->SearchBox->setFocus();  // set cursor to QLineEdit
}

void FindDialog::setListItemEnabled() {
	ui->SearchList->setItemSelected( ui->SearchList->item(0), true );
	if ( ! ui->SearchList->isItemSelected( ui->SearchList->item(0) ) )
		updateSelection( ui->SearchList->item(0) );
}

void FindDialog::updateSelection (QListWidgetItem *it) {
	KStars *p = (KStars *)parent();
	currentitem = p->data()->skyComposite()->findByName( it->text() );
	ui->SearchBox->setFocus();  // set cursor to QLineEdit
}

void FindDialog::setFilter( int f ) {
        // Translate the Listbox index to the correct SkyObject Type ID 
        int f2( f ); // in most cases, they are the same number
	if ( f >= 7 ) f2 = f + 1; //need to skip unused "Supernova Remnant" Type at position 7
	
        // check if filter was changed or if filter is still the same
	if ( Filter != f2 ) {
		Filter = f2;
		if ( Filter == 0 ) {  // any type will shown
		// delete old connections and create new connections
			disconnect( ui->SearchBox, SIGNAL( textChanged( const QString & ) ), this, SLOT( filterByType() ) );
			connect( ui->SearchBox, SIGNAL( textChanged( const QString & ) ), SLOT( filter() ) );
			filter();
		}
		else {
		// delete old connections and create new connections
			disconnect( ui->SearchBox, SIGNAL( textChanged( const QString & ) ), this, SLOT( filter() ) );
			connect( ui->SearchBox, SIGNAL( textChanged( const QString & ) ), SLOT( filterByType() ) );
			filterByType();
		}
	}
}

void FindDialog::slotOk() {
	//If no valid object selected, show a sorry-box.  Otherwise, emit accept()
	if ( currentItem() == 0 ) {
		QString message = i18n( "No object named %1 found." ).arg( ui->SearchBox->text() );
		KMessageBox::sorry( 0, message, i18n( "Bad object name" ) );
	} else {
		accept();
	}
}

void FindDialog::keyPressEvent( QKeyEvent *e ) {
	switch( e->key() ) {
		case Qt::Key_Down :
			if ( ui->SearchList->currentRow() < ((int) ui->SearchList->count()) - 1 )
				ui->SearchList->setCurrentRow( ui->SearchList->currentRow() + 1 );
			break;
			
		case Qt::Key_Up :
			if ( ui->SearchList->currentRow() )
				ui->SearchList->setCurrentRow( ui->SearchList->currentRow() - 1 );
			break;
			
		case Qt::Key_Escape :
			reject();
			break;
			
	}
}

#include "finddialog.moc"
