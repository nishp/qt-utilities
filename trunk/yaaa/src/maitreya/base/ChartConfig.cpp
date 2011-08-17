/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/ChartConfig.cpp
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

#ifdef __GNUG__
	#pragma implementation "ChartConfig.h"
#endif

#include "ChartConfig.h"
#include "ColorConfig.h"
#include "Conf.h"

#include <wx/tokenzr.h>

extern Config *config;

/*****************************************************
**
**   ChartConfig   ---   Constructor
**
******************************************************/
ChartConfig::ChartConfig()
{
}

/*****************************************************
**
**   ChartConfig   ---   setDefaultConfig
**
******************************************************/
void ChartConfig::setDefaultConfig()
{
	fgColor = config->colors->fgColor;
	bgColor = config->colors->bgColor;
	doFloodFill = true;
	useTextColors = true;
}

/*****************************************************
**
**   ChartConfig   ---   initColor
**
******************************************************/
vector<wxColour> ChartConfig::initColor( const wxString &s )
{
	wxString t;
	vector<wxColour> v;

	wxStringTokenizer tk( s, wxT( ";" ));
	while( tk.HasMoreTokens())
	{
		t = tk.GetNextToken();
		v.push_back( wxColour( t ));
	}
	return v;
}

/*****************************************************
**
**   ChartConfig   ---   getSignFgColor
**
******************************************************/
wxColour &ChartConfig::getSignFgColor( const int &i )
{
	if ( signColor.size() == 0 ) return config->colors->getSignFgColor( i );
	else return signColor[i % signColor.size()];
}

/*****************************************************
**
**   ChartConfig   ---   getHouseFgColor
**
******************************************************/
wxColour &ChartConfig::getHouseFgColor( const int &i )
{
	if ( houseColor.size() == 0 ) return config->colors->getHouseFgColor( i );
	else return houseColor[i % houseColor.size()];
}

/*****************************************************
**
**   ChartConfig   ---   getSignBgColor
**
******************************************************/
wxColour &ChartConfig::getSignBgColor( const int &i )
{
	if ( signBgColor.size() == 0 ) return config->colors->getSignBgColor( i );
	else return signBgColor[i % signBgColor.size()];
}

/*****************************************************
**
**   ChartConfig   ---   getHouseBgColor
**
******************************************************/
wxColour &ChartConfig::getHouseBgColor( const int &i )
{
	if ( houseBgColor.size() == 0 ) return config->colors->getHouseBgColor( i );
	else return houseBgColor[i % houseBgColor.size()];
}

/*****************************************************
**
**   ChartConfig   ---   getPlanetColor
**
******************************************************/
wxColour &ChartConfig::getPlanetColor( const int &i )
{
	if ( planetColor.size() == 0 ) return config->colors->getPlanetColor( i );
	else return planetColor[i % planetColor.size()];
}
