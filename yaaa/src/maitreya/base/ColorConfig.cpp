/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/ColorConfig.cpp
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
	#pragma implementation "ColorConfig.h"
#endif

#include "ColorConfig.h"

#include "Conf.h"
#include "mathbase.h"

#include <wx/colour.h>
#include <wx/file.h>
#include <wx/gdicmn.h>
#include <wx/tokenzr.h>
#include <vector>

using namespace std;

extern Config *config;

#define SIGN_RED wxT( "#E39696" )
#define SIGN_YELLOW wxT( "#ece5a6" )
#define SIGN_GREEN wxT( "#b0eca6" )
#define SIGN_BLUE wxT( "#a6cdec" )

/*****************************************************
**
**   ColorConfig   ---   Constructor
**
******************************************************/
ColorConfig::ColorConfig()
{
	// Base Colors
	fgColor = wxT( "black" );
	sbcPlanetFgColor = wxT( "navy" );
	sbcTransitFgColor = wxT( "blue" );
	bgColor = wxT( "white" );
	tableHeaderBgColor = wxT( "#92BCD5" );
	sbcMarkedFieldsBgColor = wxT( "#FDFE98" );
	sbcBeneficBgColor = wxT( "green" );
	sbcMaleficBgColor = wxT( "maroon" );

	static const struct DasaListDescription
	{
		wxString name;
		unsigned char fgcolor[3];
		unsigned char bgcolor[3];
		int bgstyle;
		wxString bmpname;
	}
	dasalist[] =
	{ 
			// Signs: Jataka Parijata I, 23; planets: II 19
			{  _( "Aries" ), { 255, 255, 255 }, { 255, 0, 0 }, wxSOLID, wxEmptyString },
			{  _( "Taurus" ), { 0, 0, 0 }, { 255, 255, 255 }, wxSOLID, wxEmptyString },
			{  _( "Gemini" ), { 0, 0, 0 }, { 0, 204, 51 }, wxSOLID, wxEmptyString },
			{  _( "Cancer" ), { 0, 0, 0 }, { 255, 160, 255 }, wxSOLID, wxEmptyString },
			{  _( "Leo" ), { 0, 0, 0 }, { 226, 255, 211 }, wxSOLID, wxEmptyString },
			{  _( "Virgo" ), { 0, 0, 0 }, { 255, 255, 255 }, wxSTIPPLE, _( "piebald.png" ) },
			{  _( "Libra" ), { 255, 255, 255 }, { 0, 0, 0 }, wxSOLID, wxEmptyString },
			{  _( "Scorpio" ), { 0, 0, 0 }, { 229, 169, 104 }, wxSOLID, wxEmptyString },
			{  _( "Sagittarius" ), { 255, 255, 255 }, { 167, 123, 76 }, wxSOLID, wxEmptyString },
			{  _( "Capricorn" ), { 0, 0, 0 }, { 255, 255, 255 }, wxSTIPPLE, _( "rainbow.png" ) },
			{  _( "Aquarius" ), { 255, 255, 255 }, { 104, 77, 47 }, wxSOLID, wxEmptyString },
			{  _( "Pisces" ), { 0, 0, 0 }, { 255, 255, 255 }, wxSOLID, wxEmptyString },
			{  _( "Sun" ), { 0, 0, 0 }, { 255, 94, 7 }, wxSOLID, wxEmptyString },
			{  _( "Moon" ), { 0, 0, 0 }, { 255, 255, 255 }, wxSOLID, wxEmptyString },
			{  _( "Mercury" ), { 0, 0, 0 }, { 102, 204, 102 }, wxSOLID, wxEmptyString },
			{  _( "Venus" ), { 0, 0, 0 }, { 255, 102, 255 }, wxSTIPPLE, _( "rainbow.png" ) },
			{  _( "Mars" ), { 255, 255, 255 }, { 204, 0, 0 }, wxSOLID, wxEmptyString },
			{  _( "Jupiter" ), { 0, 0, 0 }, { 255, 255, 102 }, wxSOLID, wxEmptyString },
			{  _( "Saturn" ), { 255, 255, 255 }, { 0, 0, 0 }, wxSOLID, wxEmptyString },
			{  _( "Rahu" ), { 255, 255, 255 }, { 82, 109, 198 }, wxSOLID, wxEmptyString },
			{  _( "Ketu" ), { 0, 0, 0 }, { 132, 132, 132 }, wxSOLID, wxEmptyString },
			{  _( "Ascendant" ), { 0, 0, 0 }, { 132, 132, 132 }, wxSOLID, wxEmptyString }
	};
	for( int i = 0; i < MAX_DASA_COLORS; i++ )
	{
		dasa[i].name = dasalist[i].name;
		dasa[i].fgColor.Set( dasalist[i].fgcolor[0], dasalist[i].fgcolor[1], dasalist[i].fgcolor[2] );
		dasa[i].bgColor.Set( dasalist[i].bgcolor[0], dasalist[i].bgcolor[1], dasalist[i].bgcolor[2] );
		dasa[i].fgWidth = 1;
		dasa[i].fgStyle = wxSOLID;
		dasa[i].bgStyle = dasalist[i].bgstyle;
		dasa[i].bgImage = dasalist[i].bmpname;
	}
}

/*****************************************************
**
**   ColorConfig   ---   readConfigParams
**
******************************************************/
void ColorConfig::readConfigParams()
{
	int i;
	wxString s;

	if ( ! config->fgColor.IsEmpty() ) fgColor = wxColour( config->fgColor );
	if ( ! config->sbcPlanetFgColor.IsEmpty() ) sbcPlanetFgColor = wxColour( config->sbcPlanetFgColor );
	if ( ! config->sbcTransitFgColor.IsEmpty() ) sbcTransitFgColor = wxColour( config->sbcTransitFgColor );
	if ( ! config->bgColor.IsEmpty() ) bgColor = wxColour( config->bgColor );
	if ( ! config->tableHeaderBgColor.IsEmpty() ) tableHeaderBgColor = wxColour( config->tableHeaderBgColor );
	if ( ! config->sbcMarkedFieldsBgColor.IsEmpty() ) sbcMarkedFieldsBgColor = wxColour( config->sbcMarkedFieldsBgColor );
	if ( ! config->sbcBeneficBgColor.IsEmpty() ) sbcBeneficBgColor = wxColour( config->sbcBeneficBgColor );
	if ( ! config->sbcMaleficBgColor.IsEmpty() ) sbcMaleficBgColor = wxColour( config->sbcMaleficBgColor );

	for( i = 0; i < 12; i++ )
	{
		if ( ! config->signFgColor[i].IsEmpty() ) chart.signFgColor[i] = wxColour( config->signFgColor[i] );
		if ( ! config->signBgColor[i].IsEmpty() ) chart.signBgColor[i] = wxColour( config->signBgColor[i] );
		if ( ! config->houseFgColor[i].IsEmpty() ) chart.houseFgColor[i] = wxColour( config->houseFgColor[i] );
		if ( ! config->houseBgColor[i].IsEmpty() ) chart.houseBgColor[i] = wxColour( config->houseBgColor[i] );
	}
	for( i = 0; i < MAX_EPHEM_OBJECTS; i++ )
	{
		if ( ! config->planetFgColor[i].IsEmpty() ) chart.planetFgColor[i] = wxColour( config->planetFgColor[i] );
	}
	for( i = 0; i < MAX_DASA_COLORS; i++ )
	{
		if ( config->dasaColor[i].IsEmpty() ) continue;
		wxStringTokenizer tk( config->dasaColor[i], wxT( ";" ));

		// Must have 6 or 7 tokens
		if ( tk.CountTokens() != 6 && tk.CountTokens() != 7 )
		{
			printf( "Error: wrong number of tokens (%d) in dasa config %d\n", (int)tk.CountTokens(), i );
		}
		else
		{
			if( tk.HasMoreTokens() ) dasa[i].name = tk.GetNextToken();
			if( tk.HasMoreTokens() ) dasa[i].fgColor = wxColour( tk.GetNextToken() );
			if( tk.HasMoreTokens() ) dasa[i].bgColor = wxColour( tk.GetNextToken() );
			if( tk.HasMoreTokens() ) dasa[i].fgWidth = (int)myatof( tk.GetNextToken() );
			if( tk.HasMoreTokens() ) dasa[i].fgStyle = (int)myatof( tk.GetNextToken() );
			if( tk.HasMoreTokens() ) dasa[i].bgStyle = (int)myatof( tk.GetNextToken() );
			if( tk.HasMoreTokens() ) dasa[i].bgImage = tk.GetNextToken();
		}
	}
}

/*****************************************************
**
**   ColorConfig   ---   writeConfigParams
**
******************************************************/
void ColorConfig::writeConfigParams()
{
	int i;

	config->fgColor = fgColor.GetAsString( wxC2S_HTML_SYNTAX );
	config->sbcPlanetFgColor = sbcPlanetFgColor.GetAsString( wxC2S_HTML_SYNTAX );
	config->sbcTransitFgColor = sbcTransitFgColor.GetAsString( wxC2S_HTML_SYNTAX );
	config->bgColor = bgColor.GetAsString( wxC2S_HTML_SYNTAX );
	config->tableHeaderBgColor = tableHeaderBgColor.GetAsString( wxC2S_HTML_SYNTAX );
	config->sbcMarkedFieldsBgColor = sbcMarkedFieldsBgColor.GetAsString( wxC2S_HTML_SYNTAX );
	config->sbcBeneficBgColor = sbcBeneficBgColor.GetAsString( wxC2S_HTML_SYNTAX );
	config->sbcMaleficBgColor = sbcMaleficBgColor.GetAsString( wxC2S_HTML_SYNTAX );
	for( i = 0; i < 12; i++ )
	{
		config->signFgColor[i] =	chart.signFgColor[i].GetAsString( wxC2S_HTML_SYNTAX );
		config->signBgColor[i] =	chart.signBgColor[i].GetAsString( wxC2S_HTML_SYNTAX );
		config->houseFgColor[i] =	chart.houseFgColor[i].GetAsString( wxC2S_HTML_SYNTAX );
		config->houseBgColor[i] =	chart.houseBgColor[i].GetAsString( wxC2S_HTML_SYNTAX );
	}
	for( i = 0; i < MAX_EPHEM_OBJECTS; i++ ) config->planetFgColor[i] =	chart.planetFgColor[i].GetAsString( wxC2S_HTML_SYNTAX );;
	for( i = 0; i < MAX_DASA_COLORS; i++ )
	{
		config->dasaColor[i] = dasa[i].toString();
	}
}

/*****************************************************
**
**   ColorConfig   ---   getDasa
**
******************************************************/
DasaColorConfig *ColorConfig::getDasa( const int &i )
{
	assert( i >= 0 && i < MAX_DASA_COLORS );
	return &dasa[i];
}

/*****************************************************
**
**   ColorConfig   ---   getDasaColorConfigId
**
******************************************************/
int ColorConfig::getDasaColorConfigId( const int &i, const bool &rasitype )
{
	int p;
	if ( rasitype )
	{
		assert( i >= 0 && i < 12 );
		return i;
	}
	else
	{
		if ( i >= OSUN && i <= OSATURN ) p = i;
		else if ( i == OMEANNODE || i == OTRUENODE || i == 8 ) p = 7;
		else if ( i == OMEANDESCNODE || i == OTRUEDESCNODE ) p = 8;
		else if ( i == OASCENDANT || i == 7 ) p = 9; // caution: Ascendant on 7 (AV) TODO
		else assert( false );
		return p + 12;
	}
}

/*****************************************************
**
**   ColorConfig   ---   getPlanetColor 
**
******************************************************/
wxColour &ColorConfig::getPlanetColor( const int &i )
{
	assert( i >= 0 && i < MAX_EPHEM_OBJECTS );
	return chart.planetFgColor[i];
}

/*****************************************************
**
**   ColorConfig   ---   getSignFgColor
**
******************************************************/
wxColour &ColorConfig::getSignFgColor( const int &i )
{
	//assert( i >= 0 && i < 12 );
	return chart.signFgColor[i%12];
}

/*****************************************************
**
**   ColorConfig   ---   getSignBgColor
**
******************************************************/
wxColour &ColorConfig::getSignBgColor( const int &i )
{
	//assert( i >= 0 && i < 12 );
	return chart.signBgColor[i%12];
}

/*****************************************************
**
**   ColorConfig   ---   getHouseFgColor
**
******************************************************/
wxColour &ColorConfig::getHouseFgColor( const int &i )
{
	assert( i >= 0 && i < 12 );
	return chart.houseFgColor[i];
}

/*****************************************************
**
**   ColorConfig   ---   getHouseBgColor
**
******************************************************/
wxColour &ColorConfig::getHouseBgColor( const int &i )
{
	assert( i >= 0 && i < 12 );
	return chart.houseBgColor[i];
}

/*****************************************************
**
**   DasaColorConfig   ---   Constructor
**
******************************************************/
DasaColorConfig::DasaColorConfig()
{
	bgColor = *wxWHITE;
	fgColor = *wxBLACK;
	fgWidth = 1;
	fgStyle = bgStyle = wxSOLID;
}	

/*****************************************************
**
**   DasaColorConfig   ---   Constructor
**
******************************************************/
DasaColorConfig::DasaColorConfig( const DasaColorConfig &d )
{
	fgColor = d.fgColor;
	bgColor = d.bgColor;
	fgWidth = d.fgWidth;
	fgStyle = d.fgStyle;
	bgStyle = d.bgStyle;
	name = d.name;
	bgImage = d.bgImage;
}

/*****************************************************
**
**   DasaColorConfig   ---   operator=
**
******************************************************/
void DasaColorConfig::operator=( const DasaColorConfig &d )
{
	fgColor = d.fgColor;
	bgColor = d.bgColor;
	fgWidth = d.fgWidth;
	fgStyle = d.fgStyle;
	bgStyle = d.bgStyle;
	name = d.name;
	bgImage = d.bgImage;
}

/*****************************************************
**
**   DasaColorConfig   ---   toString
**
******************************************************/
wxString DasaColorConfig::toString()
{
	wxString s;
	s << name
		<< wxT( ";" )
		<< fgColor.GetAsString( wxC2S_HTML_SYNTAX )
		<< wxT( ";" )
		<< bgColor.GetAsString( wxC2S_HTML_SYNTAX )
		<< wxT( ";" )
		<< fgWidth
		<< wxT( ";" )
		<< fgStyle
		<< wxT( ";" )
		<< bgStyle
		<< wxT( ";" )
		<< bgImage;
	return s;
}	

/*****************************************************
**
**   AspectColorConfig   ---   Constructor
**
******************************************************/
AspectColorConfig::AspectColorConfig()
{
	fgWidth = 1;
	fgStyle = wxSOLID;
}

/*****************************************************
**
**   ChartColorConfig   ---   Constructor
**
******************************************************/
ChartColorConfig::ChartColorConfig()
{
	int i;
	const static wxChar *k_sign_house_bg_color[12]  = {
		SIGN_RED, SIGN_YELLOW, SIGN_GREEN, SIGN_BLUE,
		SIGN_RED, SIGN_YELLOW, SIGN_GREEN, SIGN_BLUE,
		SIGN_RED, SIGN_YELLOW, SIGN_GREEN, SIGN_BLUE
	};
	const static wxChar *k_sign_house_fg_color[12]  = {
		wxT( "red" ), wxT( "yellow" ), wxT( "green" ), wxT( "blue" ),
		wxT( "red" ), wxT( "yellow" ), wxT( "green" ), wxT( "blue" ),
		wxT( "red" ), wxT( "yellow" ), wxT( "green" ), wxT( "blue" )
	};
	const static wxChar *k_planet_color[MAX_EPHEM_OBJECTS]  = {
		wxT( "red" ),    // Sun
		wxT( "blue" ),   // Moon
		wxT( "green" ),  // Merc
		wxT( "yellow" ), // Ven
		wxT( "red" ),    // Mars
		wxT( "red" ),    // Jup
		wxT( "yellow" ), // Sat
		wxT( "green" ),  // Uran
		wxT( "blue" ),   // Nep
		wxT( "blue" ),   // Plu
		wxT( "gray" ),   // Mean Node
		wxT( "gray" ),   // True Node
		wxT( "gray" ),   // Desc Mean Node
		wxT( "gray" ),   // Desc True Node
		wxT( "red" ),    // Ascendant
		wxT( "yellow" ), // MC
		wxT( "yellow" ), // Cupido
		wxT( "navy" ),   // Hades
		wxT( "green" ),  // Zeus
		wxT( "navy" ),   // Kronos
		wxT( "red" ),    // Apollon
		wxT( "yellow" ), // Admetos
		wxT( "red" ),    // Vulkanus
		wxT( "blue" ),   // Poseidon
		wxT( "red" ),   // Chiron
		wxT( "red" ),   // Pholus
		wxT( "purple" ),   // Plan1
		wxT( "purple" ),   // 
		wxT( "purple" ),   // 
		wxT( "purple" ),   // Plan4
		wxT( "gray" )   // Lilith
	};
	for( i = 0; i < 12; i++ )
	{
		signFgColor[i] = k_sign_house_fg_color[i];
		houseFgColor[i] = k_sign_house_fg_color[i];
		signBgColor[i] = k_sign_house_bg_color[i];
		houseBgColor[i] = k_sign_house_bg_color[i];
	}
	for( i = 0; i < MAX_EPHEM_OBJECTS; i++ ) planetFgColor[i] = k_planet_color[i];
}
