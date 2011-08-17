/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/ChartConfig.h
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

#ifndef _CHARTCONFIG_H_
#define _CHARTCONFIG_H_

#ifdef __GNUG__
	#pragma interface "ChartConfig.h"
#endif

#include <wx/colour.h>

#include <vector>

using namespace std;

/*************************************************//**
*
* \brief common class for chart configuration (western and Vedic)
*
******************************************************/
class ChartConfig
{
	friend class XmlWorkerVedicChartConfig;
	friend class XmlWorkerWesternChartConfig;

public:
	ChartConfig();
	virtual ~ChartConfig() {}
	virtual void setDefaultConfig();
	vector<wxColour> initColor( const wxString& );

	wxString name;
	wxString description;
	bool doFloodFill;
	wxString bgImage;
	bool useTextColors;
	wxColour bgColor, fgColor, textColor;

	wxColour &getSignFgColor( const int &i );
	wxColour &getHouseFgColor( const int &i );
	wxColour &getSignBgColor( const int &i );
	wxColour &getHouseBgColor( const int &i );
	wxColour &getPlanetColor( const int &i );

protected:

	vector<wxColour> signColor, houseColor, aspectColor, planetColor, houseBgColor, signBgColor;
};

#endif

