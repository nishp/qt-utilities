/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/PdfBase.cpp
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
  #pragma implementation "PdfBase.h"
#endif

#include "PdfBase.h"

#include "Conf.h"
//#include "config.h"
#include "FontConfig.h"

extern Config *config;

/*****************************************************
**
**   BasePdfDocument   ---   Constructor
**
******************************************************/
BasePdfDocument::BasePdfDocument()
    : wxPdfDocument( wxPORTRAIT, wxString( _T( "mm" )), (wxPaperSize)config->printPaperFormat )
{
  FontConfig *fontconfig = FontConfig::get();
  defaultFont = *fontconfig->getDefaultFont();

  const int defaultSize = 10;
  defaultFont.SetPointSize( defaultSize );

  tinyFont = defaultFont;
  tinyFont.SetPointSize( .6 * defaultSize );

  headerFont = *fontconfig->getHeaderFont();
  headerFont.SetPointSize( defaultSize );

  symbolFont = *fontconfig->getSymbolFont( 70, config->symbolFontSize );

  setDefaultFont();
}

/*****************************************************
**
**   BasePdfDocument   ---   Footer
**
******************************************************/
void BasePdfDocument::Footer()
{
  wxString s;
  setTinyFont();
  if ( PageNo() == 1 )
  {
    if ( config->printCustomFooter )
    {
      wxString s = config->printCustomFooterText;
      s.Replace( wxT( "$date" ), wxDateTime().Now().FormatDate());
      s.Replace( wxT( "$version" ), wxConvertMB2WX( VERSION ));
      SetY( -20 );
      Cell( 0, 10, s, 0, 0, wxPDF_ALIGN_CENTER );
    }
  }
  else
  {
    SetY(-15);
    s << _( "Page" ) << wxT( " " ) << PageNo();
    //s << _( "Page" ) << wxT( " " ) << PageNo() << wxT( "/{nb}" );
    Cell( 0, 10, s, 0, 0, wxPDF_ALIGN_CENTER );

  }
  setDefaultFont();
}

/*****************************************************
**
**   BasePdfDocument   ---   setSymbolFont
**
******************************************************/
void BasePdfDocument::setSymbolFont( const int zoom )
{
  //const double size = zoom * PDF_SYMBOL_FONT_SIZE;
  SetFont( symbolFont );
}

/*****************************************************
**
**   BasePdfDocument   ---   setDefaultFont
**
******************************************************/
void BasePdfDocument::setDefaultFont()
{
  SetFont( defaultFont );
}

/*****************************************************
**
**   BasePdfDocument   ---   setDefaultBoldFont
**
******************************************************/
void BasePdfDocument::setDefaultBoldFont()
{
  SetFont( headerFont );
}

/*****************************************************
**
**   BasePdfDocument   ---   setHeader1Font
**
******************************************************/
void BasePdfDocument::setHeader1Font()
{
  SetFont( headerFont );
}

/*****************************************************
**
**   BasePdfDocument   ---   setHeader2Font
**
******************************************************/
void BasePdfDocument::setHeader2Font()
{
  SetFont( headerFont );
}

/*****************************************************
**
**   BasePdfDocument   ---   setHeader3Font
**
******************************************************/
void BasePdfDocument::setHeader3Font()
{
  SetFont( headerFont );
}

/*****************************************************
**
**   BasePdfDocument   ---   setTinyFont
**
******************************************************/
void BasePdfDocument::setTinyFont()
{
  SetFont( tinyFont );
}
