/***************************************************************************
                          kstars.h  -  K Desktop Planetarium
                             -------------------
    begin                : Mon Jan 7 2002
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

/**@short KStars utility functions
	*@author Mark Hollomon
	*@version 1.0
	*This class contains static member functions to perform various time
	*conversion functions, and coordinate-related functions.
	*The openDataFile() function is also here.  This function searches
	*the standard KDE directories for the data filename given as an argument.
	*All functions are static, so we have made the constructor private
	*(so it is impossible to instantiate a KSUtils object).
	*/

#ifndef KSTARS_KSUTILS_H_
#define KSTARS_KSUTILS_H_

#include <qfile.h>
#include <qdatetime.h>

#include "dms.h"

class KSUtils {
	public:
	/**Compute julian day from the UT time/date.
		*@param t the Universal Time/Date
		*@return long double representing the corresponding Julian Day
		*/
		static long double UTtoJD(const QDateTime &t);

	/**Compute UT time/date from the julian day.
		*@param jd the julian day
		*@return QDateTime representing date and time
		*/
		static QDateTime JDtoUT(long double jd);

	/**Compute Greenwich Sidereal Time from the UT DateTime.
		*@param t the Universal Time/Date
		*@return dms representing the Greenwich Sidereal Time.
		*/
		static dms UTtoGST( const QDateTime &UT );

	/**Compute the Universal Time from the Greenwich Sidereal Time.
		*This operation requires a date, which is why the UT
		*is needed as an argument (we only use the Date portion of the UT).
		*@param GST the Greenwich sidereal time
		*@param UT the Universal Time/Date
		*@return QTime representing the Universal Time corresponding to GST.
		*/
		static QTime GSTtoUT( const dms &GST, const QDateTime &UT );

	/**convert Greenwich sidereal time to local sidereal time
		*@param GST the Greenwich Sidereal Time
		*@param longitude the current location's longitude
		*@return dms representing local sidereal time
		*/
		static dms GSTtoLST( const dms &GST, const dms *longitude );

	/**convert local sidereal time to Greenwich sidereal time
		*@param LST the local`Sidereal Time
		*@param longitude the current location's longitude
		*@return dms representing Greenwich sidereal time
		*/
		static dms LSTtoGST( const dms &LST, const dms *longitude );

	/**convert universal time to local sidereal time.
		*This is a convenience function: it calls UTtoGST, followed by GSTtoLST.
		*@param UT the Universal Time
		*@param longitude the current location's longitude
		*@return dms representing local sidereal time
		*/
		static dms UTtoLST( const QDateTime &UT, const dms *longitude );

	/**convert universal time to local sidereal time.
		*This is a convenience function: it calls UTtoGST, followed by GSTtoLST.
		*@param UT the Universal Time
		*@param longitude the current location's longitude
		*@return dms representing local sidereal time
		*/
		static QTime LSTtoUT( const dms &LST, const QDateTime &UT, const dms *longitude );

	/**Convenience function to compute the Julian Day at 0h UT.
		*@param j The julian day
		*@returns julian day at 0h UT
		*/
		static long double JDat0hUT( long double j );

	/**Overloaded member function, provided for convenience.
		*It behaves essentially like the above function.
		*@param ut The Universal Time/Date
		*@returns julian day at 0h UT
		*/
		static long double JDat0hUT( const QDateTime &ut );

	/**Convenience function to compute the Greenwich sidereal
		*time at 0h of universal time.
		*
		*@param DT = date and time
		*@returns sidereal time in hours.
		*/
		static dms GSTat0hUT( const QDateTime &DT );

	/**
		*Attempt to open the data file named filename, using the QFile object "file".
		*First look in the standard KDE directories, then look in a local "data"
		*subdirectory.  If the data file cannot be found or opened, display a warning
		*messagebox.
		*@short Open a data file.
		*@param &file The QFile object to be opened
		*@param filename The name of the data file.
		*@returns bool Returns true if data file was opened successfully.
		*@returns a reference to the opened file.
		*/
		static bool openDataFile( QFile &file, const QString &filename );

	private:
		//
		// So you can't instantiate the class.
		//
		KSUtils();
};

#endif
