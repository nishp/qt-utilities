/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Horoscope.h
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

#ifndef _HOROSCOPE_H_
#define _HOROSCOPE_H_

#ifdef __GNUG__
	#pragma interface "Horoscope.h"
#endif

#include "constants.h"
#include "DataSet.h"
#include "SimpleHoroscope.h"

#include <list>

class ChartProperties;
class Planet;
class VargaData;
class ArabicPartsExpert;

using namespace std;

/*************************************************//**
*
*  \brief main astrological class for charts holding most features of Vedic astrology
*
******************************************************/
class Horoscope : public SimpleHoroscope
{
	friend class CompositHoroscope;
public:

	Horoscope();
	virtual ~Horoscope();

	// calculation of planetary positions
	void update();

	virtual ObjectPosition getObjectPosition( const int &i, const bool &v ) const;
	Planet *getPlanet( const int &i ) const;
	VargaData *getVargaData( const int &planet, const int &varga ) const;

	double getSunrise()  const { return sunrise; }
	double getSunset()   const { return sunset; }
	bool getDayBirth()   const { return isDayBirth; }

	bool isBenefic( const int& ) const;

	// not included in update() because Dasa index may change
	void updateKP( const int &dasaindex ) const;
	void getKPLords( const int &planet, int *lord, int *sublord ) const;
	void getHouseKPLords( const int &house, int *lord, int *sublord ) const;

protected:
	mutable int kp_lord[MAX_EPHEM_OBJECTS], kp_sublord[MAX_EPHEM_OBJECTS];
	mutable int ekp_lord[MAX_EXTENDED_OBJECTS], ekp_sublord[MAX_EXTENDED_OBJECTS];
	mutable int akp_lord[MAX_ARABIC_OBJECTS], akp_sublord[MAX_ARABIC_OBJECTS];
	mutable int hkp_lord[12], hkp_sublord[12];

	Planet *ephem_planets[MAX_EPHEM_OBJECTS];

	ObjectPosition *evpos[MAX_EXTENDED_OBJECTS];
	ObjectPosition *ewpos[MAX_EXTENDED_OBJECTS];
	Planet *extended_planets[MAX_EXTENDED_OBJECTS];

	double sunrise, sunset, arieslen;
	bool isDayBirth;
	bool moonBenefic, mercuryBenefic;

	ArabicPartsExpert *arabic_expert;
	double lagna_len[NUM_LAGNA];
	double upagraha_len[NUM_UPA];
	double arabic_len[MAX_ARABIC_OBJECTS];

	void updateObjectPositions();

private:
	DECLARE_CLASS( Horoscope )
};

/*************************************************//**
*
*  \brief derived from Horoscope class for Solar (Tajaka) charts
*
******************************************************/
class TajakaHoroscope : public Horoscope
{
public:
	TajakaHoroscope( DataSet* );
	void calcTajaka( const int &year, const bool &vedic );

private:
	DataSet *birth_ds;

	DECLARE_CLASS( TajakaHoroscope )
};

#endif



