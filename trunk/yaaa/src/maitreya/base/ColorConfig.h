/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/ColorConfig.h
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

#ifndef _COLORCONFIG_H_
#define _COLORCONFIG_H_

#ifdef __GNUG__
	#pragma interface "ColorConfig.h"
#endif

#include "constants.h"

#include <vector>
#include <wx/colour.h>
#include <wx/string.h>

using namespace std;

/*************************************************//**
*
* \brief configuration for Dasa colors
*
******************************************************/
struct DasaColorConfig
{
	DasaColorConfig();
	DasaColorConfig( const DasaColorConfig& );
	void operator=( const DasaColorConfig& );

	wxColour fgColor, bgColor;
	int fgWidth, fgStyle, bgStyle;
	wxString name, bgImage;

	wxString toString();
};

/*************************************************//**
*
* \brief configuration for aspect colors
*
******************************************************/
struct AspectColorConfig
{
	AspectColorConfig();
	wxColour fgColor, bgColor;
	int fgWidth, fgStyle;
};

/*************************************************//**
*
* \brief configuration for chart colors
*
******************************************************/
struct ChartColorConfig
{
	ChartColorConfig();
	wxColour signFgColor[12], signBgColor[12], houseFgColor[12], houseBgColor[12], planetFgColor[MAX_EPHEM_OBJECTS];
};

/*************************************************//**
*
* \brief general configuration for colors
*
******************************************************/
struct ColorConfig
{
	ColorConfig();
	void readConfigParams();
	void writeConfigParams();

	wxColour &getPlanetColor( const int &i );
	wxColour &getSignFgColor( const int &i );
	wxColour &getSignBgColor( const int &i );
	wxColour &getHouseFgColor( const int &i );
	wxColour &getHouseBgColor( const int &i );

	int getDasaColorConfigId( const int &i, const bool &rasitype );

	wxColour fgColor, sbcPlanetFgColor, sbcTransitFgColor;
	wxColour bgColor, tableHeaderBgColor, sbcMarkedFieldsBgColor, sbcBeneficBgColor, sbcMaleficBgColor;

	// chart colors (Vedic chart, western chart, graphical ephemeris)
	ChartColorConfig chart;

	// Dasa
	DasaColorConfig dasa[MAX_DASA_COLORS];
	DasaColorConfig *getDasa( const int& );
};

#endif

