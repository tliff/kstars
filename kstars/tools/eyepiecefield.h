/***************************************************************************
                 eyepiecefield.h  -  K Desktop Planetarium
                             -------------------
    begin                : Fri 30 May 2014 14:39:37 CDT
    copyright            : (c) 2014 by Akarsh Simha
    email                : akarsh.simha@kdemail.net
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/



#ifndef EYEPIECEFIELD_H
#define EYEPIECEFIELD_H

#include "dms.h"

#include <QDialog>
#include <QTemporaryFile>
#include <QPixmap>

class SkyPoint;
class FOV;
class QString;
class QLabel;
class QImage;
class QSlider;
class QComboBox;
class QCheckBox;
class QPushButton;
class FOV;
class KSDssDownloader;
class KStarsDateTime;

/**
 * @class EyepieceField
 * @short Renders the view through the eyepiece of various telescope types
 * @author Akarsh Simha <akarsh.simha@kdemail.net>
 */

class EyepieceField : public QDialog   // FIXME: Rename to EyepieceView
{

        Q_OBJECT

    public:

        /**
         * @short Constructor
         */
        EyepieceField( QWidget * parent = 0 );

        /**
         * @short Destructor
         */
        ~EyepieceField();

        /**
         * @short Show the eyepiece field dialog
         * @param sp Sky point to draw the eyepiece field around.
         * @param fov Pointer to the FOV object describing the field of view. If no pointer is provided, tries to get from image. If no image is provided, assumes 1 degree.
         * @param imagePath Optional path to DSS or other image. North should be on the top of the image.
         * @note The SkyPoint must have correct Alt/Az coordinates, maybe by calling update() already before calling this method.
         */
        void showEyepieceField( SkyPoint * sp, FOV const * const fov = 0, const QString &imagePath = QString() );

        /**
         * @short Show the eyepiece field dialog
         * @param sp Sky point to draw the eyepiece field around.
         * @param fovWidth width of field-of-view in arcminutes
         * @param fovHeight height of field-of-view in arcminutes (if not supplied, is set to fovWidth)
         * @param imagePath Optional path to DSS or other image. North should be on the top of the image.
         * @note The SkyPoint must have correct Alt/Az coordinates, maybe by calling update() already before calling this method.
         */
        void showEyepieceField( SkyPoint * sp, const double fovWidth, double fovHeight = -1.0, const QString &imagePath = QString() );

        /**
         * @short Generate the eyepiece field view and corresponding image view
         * @param sp Sky point to draw the render the eyepiece field around
         * @param skyChart A non-null pointer to replace with the eyepiece field image
         * @param skyImage An optionally non-null pointer to replace with the re-oriented sky image
         * @param fovWidth width of the field-of-view in arcminutes
         * @param fovHeight height of field-of-view in arcminutes (if not supplied, is set to fovWidth)
         * @param imagePath Optional path to DSS or other image. North
         * should be on the top of the image, and the size should be in
         * the metadata; otherwise 1.01 arcsec/pixel is assumed.
         * @note fovWidth can be zero/negative if imagePath is non-empty. If it is, the image size is used for the FOV.
         * @note fovHeight can be zero/negative. If it is, fovWidth will be used. If fovWidth is also zero, image size is used.
         */

        static void generateEyepieceView( SkyPoint * sp, QImage * skyChart, QImage * skyImage = 0, double fovWidth = -1.0,
                                          double fovHeight = -1.0, const QString &imagePath = QString() );

        /**
         * @short Overloaded method provided for convenience. Obtains fovWidth/fovHeight from fov if non-null, else uses image
         */
        static void generateEyepieceView( SkyPoint * sp, QImage * skyChart, QImage * skyImage = 0, const FOV * fov = 0, const QString &imagePath = QString() );

        /**
         * @short Orients the eyepiece view as needed, performs overlaying etc.
         * @param skyChart image which contains the sky chart, possibly generated using generateEyepieceView
         * @param skyImage optional image which contains the sky image, possibly generated using generateEyepieceView
         * @param renderChart pixmap onto which the sky chart is to be rendered
         * @param renderImage optional pixmap onto which the sky image is to be rendered
         * @param rotation optional, number of degrees by which to rotate the image(s)
         * @param scale optional, factor by which to scale the image(s)
         * @param flip optional, if true, the image is mirrored horizontally
         * @param invert optional, if true, the image is inverted, i.e. rotated by 180 degrees
         * @param overlay optional, if true, the sky image is overlaid on the sky map
         * @param invertColors optional, if true, the sky image is color-inverted
         */
        static void renderEyepieceView( const QImage * skyChart, QPixmap * renderChart, const double rotation = 0, const double scale = 1.0, const bool flip = false, const bool invert = false,
                                        const QImage * skyImage = 0, QPixmap * renderImage = 0, const bool overlay = false, const bool invertColors = false );


        /**
         * @short Convenience method that generates and the renders the eyepiece view
         * @note calls generateEyepieceView() followed by the raw form of renderEyepieceView() to render an eyepiece view
         */
        static void renderEyepieceView( SkyPoint * sp, QPixmap * renderChart, double fovWidth = -1.0, double fovHeight = -1.0, const double rotation = 0, const double scale = 1.0,
                                        const bool flip = false, const bool invert = false, const QString &imagePath = QString(), QPixmap * renderImage = 0,
                                        const bool overlay = false, const bool invertColors = false );

        /**
         * @short Finds the angle between "up" (i.e. direction of increasing altitude) and "north" (i.e. direction of increasing declination) at a given point in the sky
         * @fixme Procedure does not account for precession and nutation at the moment
         * @note SkyPoint must already have Equatorial and Horizontal coordinate synced
         */
        static dms findNorthAngle( const SkyPoint * sp, const dms * lat );

    public slots:

        /**
         * @short Re-renders the view
         * Takes care of things like inverting colors, inverting orientation, flipping, rotation
         * @note Calls the static method renderEyepieceView to set things up
         */
        void render();

        /**
         * @short Enforces a preset setting
         */
        void slotEnforcePreset( int index = -1 );

        /**
         * @short save image
         */
        void slotExport();

    private slots:
        /**
         * @short downloads a DSS image
         */
        void slotDownloadDss();

        /**
         * @short loads a downloaded DSS image
         */
        void slotDssDownloaded( bool success );

    private:
        QLabel * m_skyChartDisplay;
        QLabel * m_skyImageDisplay;
        QImage * m_skyChart;
        QImage * m_skyImage;
        QSlider * m_rotationSlider;
        QCheckBox * m_invertColors;
        QCheckBox * m_overlay;
        QCheckBox * m_invertView;
        QCheckBox * m_flipView;
        QComboBox * m_presetCombo;
        QPushButton * m_getDSS;
        const FOV * m_currentFOV;
        double m_fovWidth, m_fovHeight;
        KSDssDownloader * m_dler;
        KStarsDateTime * m_dt;
        SkyPoint * m_sp;
        double m_lat; // latitude
        QTemporaryFile m_tempFile;
        QPixmap m_renderImage, m_renderChart;
        bool m_usedAltAz;
};

#endif
