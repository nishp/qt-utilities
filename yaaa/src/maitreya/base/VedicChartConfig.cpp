/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/VedicChartConfig.cpp
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
	#pragma implementation "VedicChartConfig.h"
#endif

#include "VedicChartConfig.h"

#include "ColorConfig.h"
#include "Conf.h"
#include "FileConfig.h"
#include "mathbase.h"
#include "XmlWorker.h"

#include <wx/xml/xml.h>

extern Config *config;

/*****************************************************
**
**   VedicChartConfig   ---   Constructor
**
******************************************************/
VedicChartConfig::VedicChartConfig()
	: ChartConfig() 
{
	ChartConfig::setDefaultConfig();
	setDefaultConfig();
}

/*****************************************************
**
**   VedicChartConfig   ---   setDefaultConfig
**
******************************************************/
void VedicChartConfig::setDefaultConfig()
{
	ChartConfig::setDefaultConfig();

	useSignColors = false;
	useHouseColors = false;
	useSymbols = true;
	lineWidth = 1;
	doubleOuterLine = false;
	outerLineWidth = 2;
	outerZodiac = 100;
	markedFieldBrush = 0; // solid
	markedFieldColor = config->colors->sbcMarkedFieldsBgColor;
}

/*****************************************************
**
**   VedicChartConfig   ---   toString
**
******************************************************/
wxString VedicChartConfig::toString()
{
	wxString s;
	s << wxT( "VedicChart: " ) << name << Endl
		<< wxT( "  Description: " ) << description << Endl
		<< wxT( "  doFloodFill: " ) << doFloodFill << Endl
		<< wxT( "  bgImage: " ) << bgImage << Endl
		<< wxT( "  useTextColors: " ) << useTextColors << Endl
		<< wxT( "  bgColor: " ) << bgColor.GetAsString() << Endl
		<< wxT( "  fgColor: " ) << fgColor.GetAsString() << Endl
		<< wxT( "  textColor: " ) << textColor.GetAsString() << Endl;

	s << wxT( "  useSignColors: " ) << useSignColors << Endl
		<< wxT( "  useHouseColors: " ) << useHouseColors << Endl
		<< wxT( "  lineWidth: " ) << lineWidth << Endl
		<< wxT( "  outerLineWidth: " ) << outerLineWidth << Endl
		<< wxT( "  doubleOuterLine: " ) << doubleOuterLine << Endl
		<< wxT( "  useSymbols: " ) << useSymbols << Endl
		<< wxT( "  outerZodiac: " ) << outerZodiac << Endl;
	return s;
}

/*************************************************//**
*
* \brief worker for XML extraction of Vedic chart config
*
******************************************************/
class XmlWorkerVedicChartConfig : public XmlWorker
{
public:
	XmlWorkerVedicChartConfig() : XmlWorker( XML_TOPIC_VEDIC_CHART ) {}

	/*****************************************************
	**
	**   XmlWorkerVedicChartConfig   ---   readVedicChartConfig
	**
	******************************************************/
	bool readConfig( vector<VedicChartConfig*> &defs )
	{
		if ( ! parseFile()) return false;
		wxXmlNode *cur = doc->GetRoot()->GetChildren();
		defs.clear();

		while ( cur != 0 )
		{
			if ( cur->GetName() == XML_NODE_VEDIC_CHART) mydefs.push_back( this->parseVedicChartConfig( cur ));
			else reportWrongRootNode( cur->GetName() );
			cur = cur->GetNext();
		}
		endParse();
		for( unsigned int i = 0; i < mydefs.size(); i++ ) defs.push_back( mydefs[i] );
		mydefs.clear();
		return true;
	}

private:
	vector<VedicChartConfig*> mydefs;

	/*****************************************************
	**
	**   XmlWorkerVedicChartConfig   ---   initExtendedConfig
	**
	******************************************************/
	void initExtendedConfig( VedicChartConfig *cfg, wxString name )
	{
		wxString tname = cfg->name;
		wxString tdescription = cfg->description;

		for( unsigned int i = 0; i < mydefs.size(); i++ )
		{
			if ( mydefs[i]->name == name )
			{
				*cfg = *mydefs[i];
				break;
			}
		}
		cfg->name = tname;
		cfg->description = tdescription;
	}

	/*****************************************************
	**
	**   XmlWorkerVedicChartConfig   ---   parseVedicChartConfig
	**
	******************************************************/
	VedicChartConfig *parseVedicChartConfig( wxXmlNode *cur )
	{
		VedicChartConfig *cfg = new VedicChartConfig;	
		wxString key;

		cur = cur->GetChildren();
		while( cur )
		{
			key = cur->GetName();

			if ( key == XML_NODE_EXTENDS ) this->initExtendedConfig( cfg, cur->GetNodeContent() );

			else if( key == XML_NODE_NAME ) cfg->name = cur->GetNodeContent();
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
			NODE2WXCOLOUR( XML_NODE_MARKED_FIELD_COLOR, cfg->markedFieldColor )
#undef NODE2WXCOLOUR

			else if( key == XML_NODE_OUTER_ZODIAC ) cfg->outerZodiac = myatof( cur->GetNodeContent());

			else if ( key == XML_NODE_LINE_WIDTH ) XMLNODE2INT( cur->GetNodeContent(), cfg->lineWidth )
			else if ( key == XML_NODE_OUTER_LINE_WIDTH ) XMLNODE2INT( cur->GetNodeContent(), cfg->outerLineWidth )
			else if ( key == XML_NODE_MARKED_FIELD_BRUSH ) XMLNODE2INT( cur->GetNodeContent(), cfg->markedFieldBrush )

			else if ( key == XML_NODE_USE_SIGN_COLORS ) XMLNODE2BOOLEAN( cur->GetNodeContent(), cfg->useSignColors )
			else if ( key == XML_NODE_USE_HOUSE_COLORS ) XMLNODE2BOOLEAN( cur->GetNodeContent(), cfg->useHouseColors )
			else if ( key == XML_NODE_USE_DOUBLE_OUTER_LINES ) XMLNODE2BOOLEAN( cur->GetNodeContent(), cfg->doubleOuterLine )
			else if ( key == XML_NODE_USE_SYMBOLS ) XMLNODE2BOOLEAN( cur->GetNodeContent(), cfg->useSymbols )

			else reportWrongKey( key );
			cur = cur->GetNext();
		}
		return cfg;
	}
};

IMPLEMENT_CONFIGLOADER( VedicChartConfigLoader, VedicChartConfig, XmlWorkerVedicChartConfig )

/*****************************************************
**
**   VedicChartConfigLoader   ---   getDefaultConfig
**
******************************************************/
VedicChartConfig *VedicChartConfigLoader::getDefaultConfig()
{
	if ( defs.size() > 0 ) return defs[0];
	else
	{
		VedicChartConfig *cfg = new VedicChartConfig;
		cfg->setDefaultConfig();
		defs.push_back( cfg );
		return cfg;
	}
}

