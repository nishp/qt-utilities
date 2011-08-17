/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/WesternChartConfig.cpp
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
	#pragma implementation "WesternChartConfig.h"
#endif

#include "WesternChartConfig.h"
#include "XmlWorker.h"

#include "Conf.h"
#include "mathbase.h"

#include <wx/xml/xml.h>

extern Config *config;

/*****************************************************
**
**   WesternChartConfig   ---   Constructor
**
******************************************************/
WesternChartConfig::WesternChartConfig()
	: ChartConfig() 
{
	ChartConfig::setDefaultConfig();
	setDefaultConfig();
}

/*****************************************************
**
**   WesternChartConfig   ---   setDefaultConfig
**
******************************************************/
void WesternChartConfig::setDefaultConfig()
{
	ChartConfig::setDefaultConfig();

	innerZodiac = 79;
	zodiacDegree1 = 81;
	zodiacDegree5 = 83;
	outerZodiac = 92;
	planetRadius = 73;
	aspectRadius = 55;
	outerAngleMarker = 97;
	planetMarkerLength = 2;
	innerHouse = 55;
	outerHouse = 79;
	symbolZoomFactor = 100;
	rotateSymbols = false;
	symbolType = 0;

	circle2InnerRadius = 96;
	circle2Degree1 = 98;
	circle2OuterRadius = 100;
	circle2PlanetRadius = 106;

	paint1DegCircle = false;
	paint5DegCircle = false;
	paintOuterHouseCircle = paintInnerHouseCircle = paintHouseNumbers = true;
	innerAngleMarker = 0;
	circleLineWidth = 1;
	lineWidth = 1;
}

/*************************************************//**
*
* \brief worker for XML extraction of western chart config
*
******************************************************/
class XmlWorkerWesternChartConfig : public XmlWorker
{
public:
	XmlWorkerWesternChartConfig() : XmlWorker( XML_TOPIC_WESTERN_CHART ) {}

	/*****************************************************
	**
	**   XmlWorkerWesternChartConfig   ---   readConfig
	**
	******************************************************/
	bool readConfig( vector<WesternChartConfig*> &defs )
	{
		if ( ! parseFile()) return false;
		wxXmlNode *cur = doc->GetRoot()->GetChildren();

		while ( cur != 0 )
		{
			if ( cur->GetName() == XML_NODE_WESTERN_CHART) defs.push_back( this->parseWesternChartConfig( cur ));
			else reportWrongRootNode( cur->GetName() );
			cur = cur->GetNext();
		}
		endParse();
		return true;
	}

private:

	/*****************************************************
	**
	**   XmlWorkerWesternChartConfig   ---   parseWesternChartConfig
	**
	******************************************************/
	WesternChartConfig *parseWesternChartConfig( wxXmlNode *cur )
	{
		WesternChartConfig *cfg = new WesternChartConfig;	
		wxString key;

		cur = cur->GetChildren();
		while( cur )
		{
			key = cur->GetName();

			if( key == XML_NODE_NAME ) cfg->name = cur->GetNodeContent();
			else if( key == XML_NODE_DESCRIPTION ) cfg->description = cur->GetNodeContent();
			else if ( key == XML_NODE_DO_FLOOD_FILL ) XMLNODE2BOOLEAN( cur->GetNodeContent(), cfg->doFloodFill )
			else if ( key == XML_NODE_USE_TEXT_COLORS ) XMLNODE2BOOLEAN( cur->GetNodeContent(), cfg->useTextColors )
			else if( key == XML_NODE_BG_IMAGE ) cfg->bgImage= cur->GetNodeContent();

#define NODE2WXCOLOUR_V( n, c )  else if ( key == n ) c = cfg->initColor( cur->GetNodeContent() );
			NODE2WXCOLOUR_V( XML_NODE_SIGN_COLOR, cfg->signColor )
			NODE2WXCOLOUR_V( XML_NODE_SIGN_BGCOLOR, cfg->signBgColor )
			NODE2WXCOLOUR_V( XML_NODE_HOUSE_COLOR, cfg->houseColor )
			NODE2WXCOLOUR_V( XML_NODE_HOUSE_BGCOLOR, cfg->houseBgColor )
			NODE2WXCOLOUR_V( XML_NODE_ASPECT_COLOR, cfg->aspectColor )
			NODE2WXCOLOUR_V( XML_NODE_PLANET_COLOR, cfg->planetColor )
#undef NODE2WXCOLOUR_V

#define NODE2WXCOLOUR( n, c ) else if ( key == n ) c = wxString( cur->GetNodeContent());
			NODE2WXCOLOUR( XML_NODE_BG_COLOR, cfg->bgColor )
			NODE2WXCOLOUR( XML_NODE_FG_COLOR, cfg->fgColor )
			NODE2WXCOLOUR( XML_NODE_TEXT_COLOR, cfg->textColor )
#undef NODE2WXCOLOUR

			else if( key == XML_NODE_INNER_ZODIAC ) cfg->innerZodiac = myatof( cur->GetNodeContent());
			else if( key == XML_NODE_ZODIAC_DEGREE_1 ) cfg->zodiacDegree1 = myatof( cur->GetNodeContent());
			else if( key == XML_NODE_ZODIAC_DEGREE_5 ) cfg->zodiacDegree5 = myatof( cur->GetNodeContent());
			else if( key == XML_NODE_OUTER_ZODIAC ) cfg->outerZodiac = myatof( cur->GetNodeContent());
			else if( key == XML_NODE_PLANET_RADIUS ) cfg->planetRadius = myatof( cur->GetNodeContent());
			else if( key == XML_NODE_ASPECT_RADIUS ) cfg->aspectRadius = myatof( cur->GetNodeContent());
			else if( key == XML_NODE_INNER_HOUSE ) cfg->innerHouse = myatof( cur->GetNodeContent());
			else if( key == XML_NODE_OUTER_HOUSE ) cfg->outerHouse = myatof( cur->GetNodeContent());
			else if( key == XML_NODE_OUTER_ANGLE_MARKER ) cfg->outerAngleMarker = myatof( cur->GetNodeContent());
			else if( key == XML_NODE_INNER_ANGLE_MARKER ) cfg->innerAngleMarker = myatof( cur->GetNodeContent());
			else if( key == XML_NODE_PLANET_MARKER_LENGTH ) cfg->planetMarkerLength = myatof( cur->GetNodeContent());
			
			else if( key == XML_NODE_CIRCLE2_INNER_RADIUS ) cfg->circle2InnerRadius = myatof( cur->GetNodeContent());
			else if( key == XML_NODE_CIRCLE2_DEGREE_1 ) cfg->circle2Degree1 = myatof( cur->GetNodeContent());
			else if( key == XML_NODE_CIRCLE2_OUTER_RADIUS ) cfg->circle2OuterRadius = myatof( cur->GetNodeContent());
			else if( key == XML_NODE_CIRCLE2_PLANET_RADIUS ) cfg->circle2PlanetRadius = myatof( cur->GetNodeContent());

			else if ( key == XML_NODE_CIRCLE_LINE_WIDTH ) XMLNODE2INT( cur->GetNodeContent(), cfg->circleLineWidth )
			else if ( key == XML_NODE_LINE_WIDTH ) XMLNODE2INT( cur->GetNodeContent(), cfg->lineWidth )
			else if ( key == XML_NODE_SYMBOL_ZOOM_FACTOR ) XMLNODE2INT( cur->GetNodeContent(), cfg->symbolZoomFactor )
			else if ( key == XML_NODE_SYMBOL_TYPE ) XMLNODE2INT( cur->GetNodeContent(), cfg->symbolType )

			else if ( key == XML_NODE_PAINT1_DEG_CIRCLE ) XMLNODE2BOOLEAN( cur->GetNodeContent(), cfg->paint1DegCircle )
			else if ( key == XML_NODE_PAINT5_DEG_CIRCLE ) XMLNODE2BOOLEAN( cur->GetNodeContent(), cfg->paint5DegCircle )
			else if ( key == XML_NODE_PAINT_OUTER_HOUSE_CIRCLE ) XMLNODE2BOOLEAN( cur->GetNodeContent(), cfg->paintOuterHouseCircle )
			else if ( key == XML_NODE_PAINT_INNER_HOUSE_CIRCLE ) XMLNODE2BOOLEAN( cur->GetNodeContent(), cfg->paintInnerHouseCircle )
			else if ( key == XML_NODE_PAINT_HOUSE_NUMBERS ) XMLNODE2BOOLEAN( cur->GetNodeContent(), cfg->paintHouseNumbers )
			else if ( key == XML_NODE_ROTATE_SYMBOLS ) XMLNODE2BOOLEAN( cur->GetNodeContent(), cfg->rotateSymbols )

			else reportWrongKey( key );
			cur = cur->GetNext();
		}
		return cfg;
	}
};

IMPLEMENT_CONFIGLOADER( WesternChartConfigLoader, WesternChartConfig, XmlWorkerWesternChartConfig )

