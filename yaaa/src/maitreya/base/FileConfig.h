/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/FileConfig.h
 *  Release    6.0
 *  Author     M. Pettau
 *  Copyright  2003-2010 by the author

 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
****************************************************************************/
#ifndef _FILECONFIG_H_
#define _FILECONFIG_H_

#ifdef __GNUG__
	#pragma interface "FileConfig.h"
#endif

#include <wx/string.h>

/*************************************************//**
*
* \brief Provides paths and file names
*
******************************************************/
class FileConfig
{
public:

	/**
	 * Access method for singleton
	 */
	static FileConfig *get();

	~FileConfig();

	/**
	 * Initialize paths based upon parameter datadir
	 */
	void init( const wxChar *datadir );

	wxString getLocationFile() { return locationfile; }

	wxString getLicenseFile() { return licensefile; }

	wxString getUranianTextFile( const int& );

	/**
	 * Gives file access to images. Used by IconProvider and AboutDialog
	 */
	wxString getPicDir() { return picdir; }

	/**
	 * Used by XmlWorker classes
	 */
	wxString getXmlDir() { return xmldir; }

	/**
	 * Used by XmlWorker classes
	 */
	wxString getYogaDir() { return yogadir; }

	/**
	 * Used by PdfWriter
	 */
	wxString getFontDir() { return fontdir; }

	/**
	 * Used by LanguageConfig
	 */
	wxString getBinDir() { return bindir; }

	/**
	 * Used by Config class
	 */
	wxString getConfigDir() { return configdir; }

	/**
	 * Used by PdfWriter TODO: obsolete?
	 */
	wxString getWorkingDir() { return workingdir; }

	/**
	 * make a file backup
	 */
	bool backupFile( wxString filename );

	/**
	 * Get Os depending path separator
	 */
	wxString getPathSeparator() { return separator; }

private:

	/**
	 *  Constructor is private, access is granted by get method
	 */
	FileConfig();

	/**
	 * Static self reference
	 */
	static FileConfig *ego;

	/**
	 * Handling of config directory ~./maitreya6 under Linux/UNIX
	 */
	void prepareConfigDir();

	wxString datadir, configdir, bindir, xmldir, yogadir, picdir, fontdir, locationdir, workingdir;
	wxString locationfile, licensefile, separator;	
};

#endif

