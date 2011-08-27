/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/FontConfig.cpp
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
  #pragma implementation "FontConfig.h"
#endif

#include "FontConfig.h"
#include "Conf.h"

#include <wx/font.h>
#include <wx/gdicmn.h>
#include <wx/tokenzr.h>

extern Config *config;

/**************************************************************
***
**   FontDescription   ---   Constructor
***
***************************************************************/
FontDescription::FontDescription()
{
  pointSize = 10;
  family = wxFONTFAMILY_DEFAULT;
  style = wxFONTSTYLE_NORMAL;
  weight = wxFONTWEIGHT_NORMAL;
  underline = false;
}

/**************************************************************
***
**   FontDescription   ---   isEmpty
***
***************************************************************/
bool FontDescription::isEmpty()
{
  return( pointSize <= 0 );
}

/**************************************************************
***
**   FontDescription   ---   toString
***
***************************************************************/
wxString FontDescription::toString()
{
  return wxString::Format( wxT( "%d:%d:%d:%d:%s:%s" ), pointSize, family, style, weight, underline ? wxT( "true" ) : wxT( "false" ), (const wxChar*)facename );
}

FontConfig *FontConfig::ego = 0;
/**************************************************************
***
**   FontConfig   ---   get
***
***************************************************************/
FontConfig *FontConfig::get()
{
  if ( ego == 0 ) ego = new FontConfig();
  return ego;
}

/**************************************************************
***
**   FontConfig   ---   Constructor
***
***************************************************************/
FontConfig::FontConfig()
{
  const int default_size = 12;

  defaultfd[FONT_DEFAULT].pointSize = default_size;

  defaultfd[FONT_HEADER].pointSize = default_size;
  defaultfd[FONT_HEADER].weight = wxFONTWEIGHT_BOLD;

  defaultfd[FONT_GRAPHIC].pointSize = default_size;
  defaultfd[FONT_GRAPHIC].family = wxFONTFAMILY_SWISS;

  defaultfd[FONT_TEXT].pointSize = default_size;
  defaultfd[FONT_TEXT].family = wxFONTFAMILY_MODERN;

  defaultfd[FONT_SYMBOL].facename = DEFAULT_SYMBOL_FONT_NAME;
  defaultfd[FONT_SYMBOL].pointSize = getSymbolFontSize( config->symbolFontSize );
  defaultfd[FONT_SYMBOL].family = wxFONTFAMILY_DECORATIVE;

  initFontDescription( FONT_DEFAULT, config->fontDefault );
  initFontDescription( FONT_HEADER, config->fontHeader );
  initFontDescription( FONT_GRAPHIC, config->fontGraphic );
  initFontDescription( FONT_TEXT, config->fontText );
  initFontDescription( FONT_SYMBOL, config->fontSymbol );
  fd[FONT_SYMBOL].pointSize = defaultfd[FONT_SYMBOL].pointSize;

  //for( int i = 0; i < MAX_FONTS; i++ ) fbFont[i] = *(wxTheFontList->FindOrCreateFont( defaultfd[i].pointSize, defaultfd[i].family, defaultfd[i].style, defaultfd[i].weight ));
}

/**************************************************************
***
**   FontConfig   ---   initFontDescription
***
***************************************************************/
void FontConfig::initFontDescription( const int &i, const wxString &cf  )
{
  wxString s;
  long l;
  wxStringTokenizer t( cf, wxT( ":" ));

  if ( t.CountTokens() < 5 )
  {
    printf( "WARN: FontConfig::initFont config %d entry has only %ld tokens, setting default\n", i, t.CountTokens());
    fd[i] = defaultfd[i];
    return;
  }

  s = t.GetNextToken();
  s.ToLong( &l );
  fd[i].pointSize = l;

  s = t.GetNextToken();
  s.ToLong( &l );
  fd[i].family = l;

  s = t.GetNextToken();
  s.ToLong( &l );
  fd[i].style = l;

  s = t.GetNextToken();
  s.ToLong( &l );
  fd[i].weight  = l;

  s = t.GetNextToken();
  fd[i].underline = ( s.CmpNoCase( wxT( "true" )) == 0 );

  fd[i].facename = t.GetNextToken();
}

/**************************************************************
***
**   FontConfig   ---   getFallbackFont
***
***************************************************************/
wxFont *FontConfig::getFallbackFont( const int &i, const int zoom )
{
  assert( i >= 0 && i < MAX_FONTS );
  return &fbFont[i];
}

/**************************************************************
***
**   FontConfig   ---   writeConfigEntries
***
***************************************************************/
void FontConfig::writeConfigEntries()
{
  config->fontDefault = fd[FONT_DEFAULT].toString();
  config->fontHeader = fd[FONT_HEADER].toString();
  config->fontGraphic = fd[FONT_GRAPHIC].toString();
  config->fontText = fd[FONT_TEXT].toString();
  config->fontSymbol = fd[FONT_SYMBOL].toString();
}

/**************************************************************
***
**   FontConfig   ---   setFont
***
***************************************************************/
void FontConfig::setFont( const int &i, const wxFont &f )
{
  assert( i >= 0 && i < MAX_FONTS );
  fd[i].pointSize = f.GetPointSize();
  fd[i].family = f.GetFamily();
  fd[i].style = f.GetStyle();
  fd[i].weight = f.GetWeight();
  fd[i].facename = f.GetFaceName();
}

/**************************************************************
***
**   FontConfig   ---   getSymbolFontName
***
***************************************************************/
wxString FontConfig::getSymbolFontName()
{
  return DEFAULT_SYMBOL_FONT_NAME;
}

/**************************************************************
***
**   FontConfig   ---   getSymbolFontSize
***
***************************************************************/
int FontConfig::getSymbolFontSize( const int &scale )
{
  int size = 16;
  switch( scale )
  {
    case 0:
      size = 12;
    break;
    case 1:
      size = 16;
    break;
    case 2:
      size = 18;
    break;
    case 3:
      size = 20;
    break;
    case 4:
      size = 24;
    break;
  }
  return size;
}

/**************************************************************
***
**   FontConfig   ---   getFont
***
***************************************************************/
wxFont *FontConfig::getFont( const int &i, const int zoom, const int tempFontSize )
{
  assert( i >= 0 && i < MAX_FONTS );
  int size;

  if ( fd[i].isEmpty() )
  {
    printf( "FontConfig::getFont setting default font description for font #%d\n", i );
    fd[i] = defaultfd[i];
  }

  if ( i == FONT_SYMBOL ) size = zoom * getSymbolFontSize( tempFontSize == -1 ? config->symbolFontSize : tempFontSize ) / 100;
  else size = zoom * fd[i].pointSize / 100;

  return wxTheFontList->FindOrCreateFont( size, fd[i].family, fd[i].style, fd[i].weight, fd[i].underline, fd[i].facename );
}

