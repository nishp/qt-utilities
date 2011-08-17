/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/PdfPrinter.cpp
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
  #pragma implementation "PdfPrinter.h"
#endif

#include "PdfPrinter.h"

#include "Ashtakavarga.h"
#include "ChartProperties.h"
#include "Conf.h"
//#include "config.h"
#include "Dasa.h"
#include "Horoscope.h"
#include "mathbase.h"
#include "PdfBase.h"
#include "PdfPainter.h"
#include "PdfWriter.h"
#include "PrintoutConfig.h"
#include "PrintoutHelper.h"
#include "SarvatobhadraChart.h"
#include "Uranian.h"
#include "Varga.h"
#include "VargaHoroscope.h"
#include "VedicChart.h"
#include "WesternChart.h"

#include <wx/defs.h>
#include <wx/filename.h>
#include <wx/pdfdoc.h>
#include <wx/string.h>
#ifdef __WXMSW__
#include <wx/mimetype.h>
#endif

extern Config *config;

/*****************************************************
**
**   PdfPrinter   ---   Constructor
**
******************************************************/
PdfPrinter::PdfPrinter( const Horoscope *horoscope )
:	h ( horoscope )
{
  pdfdoc = new BasePdfDocument;
  painter = new PdfPainter( pdfdoc );
  writer = new PdfWriter( pdfdoc, config );
  props = new	ChartProperties;
  props->setVedicSkin( config->printVGraphicSkin );
  props->setWesternSkin( config->printWGraphicSkin );
  // TODO colors
}

/*****************************************************
**
**   PdfPrinter   ---   Destructor
**
******************************************************/
PdfPrinter::~PdfPrinter()
{
  delete painter;
  delete writer;
  delete pdfdoc;
  delete props;
}

/*****************************************************
**
**   PdfPrinter   ---   preparePrint
**
******************************************************/
bool PdfPrinter::preparePrint( wxString f, const wxString basename, const bool askOverwrite )
{
  int i;
  wxString s, pathname;

  filename = f;
  pathname = config->defExportPath;
  if ( filename == wxEmptyString )
  {
    filename = pathname;
    // append a slash/backslash if necessary
    if ( pathname != wxEmptyString && pathname.Last() != wxFileName::GetPathSeparator())
      filename << wxFileName::GetPathSeparator();
    //filename << h->getHName() << wxT( ".pdf" );
    filename << basename << wxT( ".pdf" );
  }
  if ( wxFile::Exists( filename ) && ! wxFile::Access( filename, wxFile::write ))
  {
    s.Printf( _( "Permission denied (%s)" ), (const wxChar*)filename);
    doMessageBox( 0, s, wxOK | wxICON_ERROR );
    return false;
  }
  if ( config->printAskOverwrite && wxFile::Exists( filename ))
  {
    s.Printf( _( "File %s exists. Do you want to overwrite?" ), (const wxChar*)filename);
    i = doMessageBox( 0, s, wxYES_NO | wxCENTRE );
    if ( i == wxID_NO ) return false;
  }

  // try to create the file and look if it works; e.g. no write permission on directory will not work.
  wxFile file;
  bool b = file.Create( filename, true );
  if ( b == false )
  {
    s.Printf( _( "Could not create file %s" ), (const wxChar*)filename);
    doMessageBox( 0, s, wxOK | wxICON_ERROR );
    return false;
  }
  file.Close();
  config->defExportPath = pathname;

  return true;
}

/*****************************************************
**
**   PdfPrinter   ---   print
**
******************************************************/
bool PdfPrinter::print( const int &prt, wxString basename, const bool askOverwrite )
{
  wxString s;
  if ( preparePrint( basename, h->getHName(), askOverwrite ))
  {
    pdfdoc->AddPage();
    writer->setFilename( filename );

    PrintoutConfig *cfg = PrintoutConfigLoader::get()->getConfig( prt );
    writer->beginWriting();
    for( unsigned int i = 0; i < cfg->items.size(); i++ ) printItem( &cfg->items[i] );
    writer->endWriting();

    if ( config->printLaunchPdfViewer ) launchBrowser();
    else
    {
      s.Printf( _( "Chart printed to file %s." ), (const wxChar*)filename );
      doMessageBox( 0, s, wxOK | wxCENTRE );
    }
    return true;
  }
  return false;
}

/*****************************************************
**
**   PdfPrinter   ---   printItem
**
******************************************************/
void PdfPrinter::printItem( const PrintoutConfigItem *item )
{
  switch( item->type )
  {
    case PRINTOUT_TYPE_TITLE:
      printTitle();
    break;
    case PRINTOUT_TYPE_BASEDATA:
      PrintoutHelper( h, writer ).writePrintoutSummary( item->texttype );
      pdfdoc->Ln();
    break;
    case PRINTOUT_TYPE_VEDIC_CHART_PAIR:
      // TODO param3 raus
      printVedicPair( item->varga1, item->varga2, 0 );
    break;
    case PRINTOUT_TYPE_SBC:
      printSbc();
    break;
    case PRINTOUT_TYPE_URANIAN:
      printf(  "TODO URANIAN\n" );
      printUranian();
    break;
    case PRINTOUT_TYPE_WESTERN_CHART:
      // tod param
      printWesternChart( 0 );
    break;
    case PRINTOUT_TYPE_ASHTAKAVARGA:
    {
      VargaHoroscope vh( h );
      AshtakavargaExpert aexpert( &vh );
      aexpert.update();
      aexpert.write( writer );
      pdfdoc->Ln();
    }
    break;
    case PRINTOUT_TYPE_DASA:
    {
    // Workaround: symbols cannot be displayed in String produced by DasaExpert, so switch to text mode
      int tmpSignMode = config->useSignSymbols;
      int tmpPlanetMode = config->usePlanetSymbols;
      config->useSignSymbols = config->usePlanetSymbols = false;
      DasaExpertFactory::get()->getDasaExpert( item->dasatype )->write( writer, h );
      config->useSignSymbols = tmpSignMode;
      config->usePlanetSymbols = tmpPlanetMode;
    }
    break;
    default:
      printf( "ERROR, unknown type %d\n", item->type );
    break;
  }
}

/*****************************************************
**
**   PdfPrinter   ---   writeTitle
**
******************************************************/
void PdfPrinter::printTitle()
{
  writer->writeHeader1( h->getHName() );
}

/*****************************************************
**
**   PdfPrinter   ---   writeUranian
**
******************************************************/
void PdfPrinter::printUranian()
{
  UranianConfig cfg;
  UranianExpert expert( props, writer, cfg );
  int tmpSignMode = config->useSignSymbols;
  int tmpPlanetMode = config->usePlanetSymbols;
  config->useSignSymbols = config->usePlanetSymbols = false;

  expert.writeRadixAnalysis( h );
  pdfdoc->Ln();

  config->useSignSymbols = tmpSignMode;
  config->usePlanetSymbols = tmpPlanetMode;
}

/*****************************************************
**
**   PdfPrinter   ---   printSbc
**
******************************************************/
void PdfPrinter::printSbc()
{
  double oldy = pdfdoc->GetY();
  double totalwidth = pdfdoc->GetPageWidth() - pdfdoc->GetLeftMargin() - pdfdoc->GetRightMargin();
  double width = totalwidth;
  double height = width / 2;
  if ( pdfdoc->GetY() + height > pdfdoc->GetPageHeight() ) pdfdoc->AddPage();

  SarvatobhadraChart chart( h, props );
  chart.doPaint( painter, wxRect( (int)pdfdoc->GetLeftMargin(), (int)pdfdoc->GetY(), (int)width, (int)height ));
  pdfdoc->SetY( oldy + height );
  pdfdoc->Ln();
}

/*****************************************************
**
**   PdfPrinter   ---   printWesternChart
**
******************************************************/
void PdfPrinter::printWesternChart( int style )
{
  painter->setSymbolFont();
  double totalwidth = pdfdoc->GetPageWidth() - pdfdoc->GetLeftMargin() - pdfdoc->GetRightMargin();
  double width = totalwidth;
  double height = width;
  if ( pdfdoc->GetY() + height > pdfdoc->GetPageHeight() ) pdfdoc->AddPage();

  WesternChart chart( h, h, props );
  chart.setPtUnit( true );
  chart.OnDataChanged();
  chart.paint( painter, wxRect( (int)pdfdoc->GetLeftMargin(), (int)pdfdoc->GetY(), (int)width, (int)height ));
  pdfdoc->SetY( pdfdoc->GetY() + height );
}

/*****************************************************
**
**   PdfPrinter   ---   printVedicPair
**
******************************************************/
void PdfPrinter::printVedicPair( const int &varga1, const int &varga2, int style )
{
  double totalwidth = pdfdoc->GetPageWidth() - pdfdoc->GetLeftMargin() - pdfdoc->GetRightMargin();
  double width = totalwidth/2;
  double height = width;
  if ( props->getVedicGraphicStyle() & VGRAPHIC_SOUTH_INDIAN ) height *= .85;
  if ( pdfdoc->GetY() + height > pdfdoc->GetPageHeight() ) pdfdoc->AddPage();

  if ( style == -1 ) style = config->vGraphicStyle + config->vObjects;

  VargaHoroscope chart1( h, varga1 );
  VargaHoroscope chart2( h, varga2 );

  VedicVargaChart *graphic1 = new VedicVargaChart( &chart1, props );
  graphic1->setPtUnit( true );
  graphic1->OnDataChanged();
  graphic1->doPaint( painter, wxRect( (int)pdfdoc->GetLeftMargin(), (int)pdfdoc->GetY(), (int)width, (int)height ));

  VedicVargaChart *graphic2 = new VedicVargaChart( &chart2, props );
  graphic2->setPtUnit( true );
  graphic2->OnDataChanged();
  graphic2->doPaint( painter, wxRect( (int)(pdfdoc->GetLeftMargin() + width), (int)pdfdoc->GetY(), (int)width, (int)height ));
  pdfdoc->SetY( pdfdoc->GetY() + height+5 );
}

/*****************************************************
**
**   PdfPrinter   ---   launchBrowser
**
******************************************************/
void PdfPrinter::launchBrowser()
{
  wxString command;

#ifdef __WXMSW__
  wxFileType *filetype = wxMimeTypesManager().GetFileTypeFromExtension( wxT( "pdf" ));
  if ( filetype != 0 )
  {
    command = filetype->GetOpenCommand( filename );
  }
  else
  {
    doMessageBox( 0, _( "Cannot launch pdf viewer. Do you have a pdf viewer (e.g. Acrobat Reader) installed?" ),
      wxOK | wxICON_ERROR | wxCENTRE );
    return;
  }
#else
#define NUM_READERS 7

  // try and error for custom pdf viewers on Linux/UNIX
  const wxChar *pdfviewers[NUM_READERS] = { wxT( "evince" ), wxT( "kpdf" ), wxT( "xpdf" ), wxT( "okular" ), wxT( "kghostview" ), wxT( "acroread" ), wxT( "epdfview" ) };

  if ( config->printPdfViewerCommand.IsEmpty())
  {
    for( int i = 0; i < NUM_READERS; i++ )
    {
      config->printPdfViewerCommand = locateFile( pdfviewers[i] );
      if ( ! config->printPdfViewerCommand.IsEmpty())
      {
        break;
      }
    }
  }
  if ( config->printPdfViewerCommand.IsEmpty())
  {
    doMessageBox( 0, _( "Cannot launch pdf viewer. Please configure the pdf viewer in the configuration dialog." ),
      wxOK | wxICON_ERROR | wxCENTRE );
    return;
  }
  command << config->printPdfViewerCommand << wxT( " \"" ) << filename << wxT( "\"" );
#endif
  long r = wxExecute( command );
  if ( r == -1 )
  {
    doMessageBox( 0, wxString::Format( _( "Can't launch pdf viewer, command is \"%s\" (error code %ld)" ), (const wxChar*)command, r ), wxOK | wxICON_ERROR | wxCENTRE );
  }
}

