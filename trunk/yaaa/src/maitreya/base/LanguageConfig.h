/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/LanguageConfig.h
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
#ifndef _LANGUAGECONFIG_H_
#define _LANGUAGECONFIG_H_

#ifdef __GNUG__
	#pragma interface "LanguageConfig.h"
#endif

#include <wx/string.h>
#include <list>

class wxLocale;

using namespace std;

/*************************************************//**
*
* 
*
******************************************************/
class LanguageEntry
{
public:
	LanguageEntry( const int &id, const wxString &langcode, const wxString &thename )
	{
		wxId = id;
		code = langcode;
		name = thename;
	}
	int wxId;
	wxString code;
	wxString name;
};

/*************************************************//**
*
*  applicatino wide language configuration
*
******************************************************/
class LanguageConfig
{
public:

	static LanguageConfig *get();

	list<LanguageEntry> getLanguages() { return langentries; }
	wxLocale *getLocale() { return locale; }
	void setLanguage( const int& language );

private:

	LanguageConfig();
	static LanguageConfig *ego;

	void init();
  wxLocale *locale;
	list<LanguageEntry> langentries;
};

#endif

