/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/PrintoutHelper.h
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
#ifndef _PRINTOUTHELPER_H_
#define _PRINTOUTHELPER_H_

#ifdef __GNUG__
	#pragma interface "PrintoutHelper.h"
#endif

#include <wx/string.h>
#include <vector>

using namespace std;

#include "constants.h"

class Horoscope;
class Table;
class Writer;

enum { PSUM_WDEFAULT = 0, PSUM_WURANIAN, PSUM_VDEFAULT, PSUM_VKP };

/*************************************************//**
*
*  creation of text output for printouts
*
******************************************************/
class PrintoutHelper
{
public:

	PrintoutHelper( const Horoscope*, Writer* );
	~PrintoutHelper();

	void writePrintoutSummary( const int& );

private:

	void writeBaseData();
	void writeWesternDefault();
	void writeWesternUranian();
	void writeVedicDefault();
	void writeVedicKp();
	void writePlanetaryObject( const int& row, const int &line, const int& index, const bool&, const bool &vedic, const bool cutname = false );
	void writeHouse( const int &row, const int &line, const int& house, const bool &vedic );

	int style;
	const Horoscope *h;
	Writer *writer;
	bool preferVedic;
	bool umode;
	Table *table;

};


#endif


