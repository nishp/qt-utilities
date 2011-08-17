/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/XmlWorker.h
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

#ifndef _XMLWORKER_H_
#define _XMLWORKER_H_

#ifdef __GNUG__
	#pragma interface "XmlWorker.h"
#endif

#include <wx/hashmap.h>
#include <wx/string.h>

#define RELOAD_ON_FILE_CHANGE

// Xml topics
#define MAX_XML_TOPICS 5

enum { 
	XML_TOPIC_MULTIPLE_VIEWS = 0,
	XML_TOPIC_WESTERN_CHART,
	XML_TOPIC_VEDIC_CHART,
	XML_TOPIC_PRINTOUT
};

// Root names for view containers (multiple views)
#define XML_ROOT_MULTIPLE_VIEWS wxT( "MultipleViewConfigs" )
#define XML_NODE_MULTIPLEVIEW wxT( "MultipleViewConfig" )

// Root names for western chart
#define XML_ROOT_WESTERN_CHART wxT( "WesternChartConfigs" )
#define XML_NODE_WESTERN_CHART wxT( "WesternChartConfig" )

// Root names for vedic chart
#define XML_ROOT_VEDIC_CHART wxT( "VedicChartConfigs" )
#define XML_NODE_VEDIC_CHART wxT( "VedicChartConfig" )

// Root names for Yogas
#define XML_ROOT_YOGA wxT( "YogaConfigs" )
#define XML_NODE_YOGA wxT( "YogaConfig" )
#define XML_NODE_YOGA_GROUP wxT( "YogaConfigGroup" )

// Root names for color configuration
#define XML_ROOT_COLOR wxT( "ColorConfigs" )
#define XML_NODE_COLOR wxT( "ColorConfig" )

// Root names for vedic chart
#define XML_ROOT_PRINTOUT wxT( "PrintoutConfigs" )
#define XML_NODE_PRINTOUT wxT( "PrintoutConfig" )

// Various element node names
#define XML_NODE_DESCRIPTION wxT( "Description" )
#define XML_NODE_GROUP wxT( "Group" )
#define XML_NODE_EXTENDS wxT( "Extends" )
#define XML_NODE_SOURCE wxT( "Source" )
#define XML_NODE_EFFECT wxT( "Effect" )
#define XML_NODE_NAME wxT( "Name" )
#define XML_NODE_SHORTNAME wxT( "ShortName" )
#define XML_NODE_RULESTR wxT( "Rule" )
#define XML_NODE_ALLOW_HIGHER_VARGAS wxT( "AllowHigherVargas" )

#define XML_NODE_PLANET_COLOR wxT( "PlanetColor" )
#define XML_NODE_SIGN_COLOR wxT( "SignColor" )
#define XML_NODE_SIGN_BGCOLOR wxT( "SignBgColor" )
#define XML_NODE_HOUSE_COLORS wxT( "HouseColors" )
#define XML_NODE_HOUSE_COLOR wxT( "HouseColor" )
#define XML_NODE_HOUSE_BGCOLOR wxT( "HouseBgColor" )
#define XML_NODE_MARKED_FIELD_COLOR wxT( "MarkedFieldColor" )
#define XML_NODE_MARKED_FIELD_BRUSH wxT( "MarkedFieldBrush" )

#define XML_NODE_FG_COLOR wxT( "FgColor" )
#define XML_NODE_BG_COLOR wxT( "BgColor" )
#define XML_NODE_TEXT_COLOR wxT( "TextColor" )
#define XML_NODE_USE_TEXT_COLORS wxT( "UseTextColors" )
#define XML_NODE_ASPECT_COLOR wxT( "AspectColor" )

#define XML_NODE_DO_FLOOD_FILL wxT( "DoFloodFill" )
#define XML_NODE_BG_IMAGE wxT( "BgImage" )

#define XML_NODE_INNER_ZODIAC wxT( "InnerZodiac" )
#define XML_NODE_ZODIAC_DEGREE_1 wxT( "ZodiacDegree1" )
#define XML_NODE_ZODIAC_DEGREE_5 wxT( "ZodiacDegree5" )
#define XML_NODE_OUTER_ZODIAC wxT( "OuterZodiac" )
#define XML_NODE_PLANET_RADIUS wxT( "PlanetRadius" )
#define XML_NODE_ASPECT_RADIUS wxT( "AspectRadius" )
#define XML_NODE_INNER_HOUSE wxT( "InnerHouse" )
#define XML_NODE_OUTER_HOUSE wxT( "OuterHouse" )
#define XML_NODE_OUTER_ANGLE_MARKER wxT( "OuterAngleMarker" )
#define XML_NODE_INNER_ANGLE_MARKER wxT( "InnerAngleMarker" )
#define XML_NODE_PLANET_MARKER_LENGTH wxT( "PlanetMarkerLength" )
#define XML_NODE_CIRCLE_LINE_WIDTH wxT( "CircleLineWidth" )
#define XML_NODE_LINE_WIDTH wxT( "LineWidth" )
#define XML_NODE_OUTER_LINE_WIDTH wxT( "OuterLineWidth" )
#define XML_NODE_SYMBOL_ZOOM_FACTOR wxT( "SymbolZoomFactor" )
#define XML_NODE_ROTATE_SYMBOLS wxT( "RotateSymbols" )
#define XML_NODE_SYMBOL_TYPE wxT( "SymbolType" )

#define XML_NODE_CIRCLE2_INNER_RADIUS wxT( "Circle2InnerRadius" )
#define XML_NODE_CIRCLE2_DEGREE_1 wxT( "Circle2Degree1" )
#define XML_NODE_CIRCLE2_OUTER_RADIUS wxT( "Circle2OuterRadius" )
#define XML_NODE_CIRCLE2_PLANET_RADIUS wxT( "Circle2PlanetRadius" )
  
#define XML_NODE_PAINT1_DEG_CIRCLE wxT( "Paint1DegCircle" )
#define XML_NODE_PAINT5_DEG_CIRCLE wxT( "Paint5DegCircle" )
#define XML_NODE_PAINT_OUTER_HOUSE_CIRCLE wxT( "PaintOuterHouseCircle" )
#define XML_NODE_PAINT_INNER_HOUSE_CIRCLE wxT( "PaintInnerHouseCircle" )
#define XML_NODE_PAINT_HOUSE_NUMBERS wxT( "PaintHouseNumbers" )

#define XML_NODE_USE_SIGN_COLORS wxT( "UseSignColors" )
#define XML_NODE_USE_HOUSE_COLORS wxT( "UseHouseColors" )
#define XML_NODE_USE_DOUBLE_OUTER_LINES wxT( "DoubleOuterLine" )
#define XML_NODE_USE_SYMBOLS wxT( "UseSymbols" )

#define XML_NODE_WINDOW_NODE wxT( "Window" )
#define XML_NODE_WINDOW1 wxT( "Window1" )
#define XML_NODE_WINDOW2 wxT( "Window2" )
#define XML_NODE_WIDGET_NODE wxT( "Widget" )
#define XML_NODE_SPLITTER_NODE wxT( "Splitter" )
#define XML_NODE_PAGE_NODE wxT( "Page" )
#define XML_NODE_NOTEBOOK_NODE wxT( "Notebook" )
#define XML_NODE_VIEW_TYPE wxT( "ViewType" )
#define XML_NODE_TEXTMODE wxT( "TextMode" )
#define XML_NODE_VARGA wxT( "Varga" )
#define XML_NODE_SUB_SHOWTOOLBAR wxT( "ShowToolbar" )
#define XML_NODE_VEDIC wxT( "Vedic" )
#define XML_NODE_FALSE wxT( "false" )
#define XML_NODE_TRUE wxT( "true" )
#define XML_NODE_ROW_COUNT wxT( "RowCount" )
#define XML_NODE_COL_COUNT wxT( "ColCount" )
#define XML_NODE_VARGA_LIST wxT( "VargaList" )
#define XML_NODE_VARGA_LIST_ITEM wxT( "VargaItem" )

#define XML_NODE_SPLIT_DIRECTION wxT( "SplitDirection" )
#define XML_NODE_SPLIT_HORIZONTAL wxT( "Horizontal" )
#define XML_NODE_SPLIT_VERTICAL wxT( "Vertical" )
#define XML_NODE_SASH_POS wxT( "SashPos" )
#define XML_NODE_SASH_POS_PERCENT wxT( "SashPosPercent" )

// Printout
#define XML_NODE_PAGES wxT( "Pages" )
#define XML_NODE_ITEM wxT( "Item" )
#define XML_NODE_STYLE wxT( "Style" )
#define XML_NODE_DASA_TYPE wxT( "DasaType" )
#define XML_PRINTOUT_TYPE_TITLE wxT( "Title" )
#define XML_PRINTOUT_TYPE_BASE_DATA wxT( "ChartData" )
#define XML_PRINTOUT_TYPE_VEDIC_CHART_PAIR wxT( "VedicChartPair" )
#define XML_PRINTOUT_TYPE_WESTERN_CHART    wxT( "WesternChart" )
#define XML_PRINTOUT_TYPE_DASA wxT( "Dasa" )
#define XML_PRINTOUT_TYPE_ASHTAKAVARGA wxT( "Ashtakavarga" )
#define XML_PRINTOUT_TYPE_SBC wxT( "Sarvatobhadra" )
#define XML_PRINTOUT_TYPE_URANIAN wxT( "Uranian" ) 
#define XML_PRINTOUT_TYPE_EPHEM wxT( "Ephemeris" )
#define XML_NODE_VARGA1 wxT( "Varga1" )
#define XML_NODE_VARGA2 wxT( "Varga2" )

#define XMLNODE2INT( k, v ) { long l; v = ( k.ToLong( &l ) ? (int)l : 0 ); }
#define XMLNODE2BOOLEAN( k, v ) { v = ( k.Lower() == wxT( "true" )); }
#define XMLNODE2DOUBLE( k, v ) { v = myatof( k ); }

WX_DECLARE_STRING_HASH_MAP( int, FileModTimeHash );

/*************************************************//**
*
* \brief base class for reading XML config files
*
******************************************************/
class XmlWorker
{
public:

	bool needsReload();

	void dumpFile();

	bool saveFile( wxString name );

protected:

	XmlWorker( const int &topic );

	bool parseFile();

	bool parseFile( wxString name );

	void endParse();

	void reportWrongKey( wxString );

	void reportWrongRootNode( wxString );

	class wxXmlDocument *doc;

	FileModTimeHash modtime;

private:

	wxString filename;

	const int topic;
};

#endif

