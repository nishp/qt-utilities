/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/ArabicParts.h
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

#ifndef _ARABICPARTS_H_
#define _ARABICPARTS_H_

#ifdef __GNUG__
	#pragma interface "ArabicParts.h"
#endif

#include "constants.h"
#include "SimpleHoroscope.h"
#include <wx/string.h>

class Horoscope;
class Planet;
class VargaData;
class Writer;

/*************************************************//**
*
* \brief names and rules of arabis parts
*
******************************************************/
struct ArabicPartDefinition
{
	wxString name;
	int  part[6];
	bool flip;  // flip addition+subtraction for night birth
};

/*************************************************//**
*
* \brief encapsulates calculation of Arabic Parts
*
******************************************************/
class ArabicPartsExpert
{
public:

	ArabicPartsExpert( const Horoscope* );
	~ArabicPartsExpert();

	static wxString getObjectName( const int &p, const int &format );

	void update();
	ObjectPosition getObjectPosition( const int &p, const bool vedic );
	VargaData *getVargaData( const int &planet, const int &varga );
	Planet *getPlanet( const int &planet );
	void write( Writer*, const bool &vedic, const bool show_header = true );

private:

	void updateAll();
	void updatePart( const int &p );
	wxString getItemDescription( Writer *writer, const int &type, const int &obj );
	static ArabicPartDefinition getPart( const int &i );

	const Horoscope *h;
	bool dirty;
	double vayanamsa, wayanamsa;
	ObjectPosition vpos[MAX_ARABIC_OBJECTS], wpos[MAX_ARABIC_OBJECTS];
	Planet *planets[MAX_ARABIC_OBJECTS];
};

#endif


