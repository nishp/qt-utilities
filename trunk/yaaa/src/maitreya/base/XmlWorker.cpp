/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/XmlWorker.cpp
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
	#pragma implementation "XmlWorker.h"
#endif

#include "XmlWorker.h"

#include "constants.h"
#include "mathbase.h"
#include "FileConfig.h"

#include <wx/filename.h>
#include <wx/xml/xml.h>

// Names of topics => file names (*.xml, *.dtd) encapsulated here
const wxChar *xml_core_name[MAX_XML_TOPICS] = { wxT( "MultipleViewConfigs" ), wxT( "WesternChartConfigs" ),
	wxT( "VedicChartConfigs" ), wxT( "PrintoutConfigs" ) 
};

/*****************************************************
**
**   XmlWorker   ---   Constructor
**
******************************************************/
XmlWorker::XmlWorker( const int &topic )
: topic ( topic )
{
	assert( topic >= -1 && topic < MAX_XML_TOPICS );
	doc = 0;
	if ( topic >= 0 ) filename = FileConfig::get()->getXmlDir() + xml_core_name[topic] + wxT( ".xml" );
}

/*****************************************************
**
**   XmlWorker   ---   needsReload
**
******************************************************/
bool XmlWorker::needsReload()
{
	for( FileModTimeHash::iterator it = modtime.begin(); it != modtime.end(); it++)
	{
		if ( modtime[it->first] != ::wxFileModificationTime( it->first ))
		{
			return true;
		}
	}
	return false;
}

/*****************************************************
**
**   XmlWorker   ---   dumpFile
**
******************************************************/
void XmlWorker::dumpFile()
{
	// TODO
}

/*****************************************************
**
**   XmlWorker   ---   saveFile
**
******************************************************/
bool XmlWorker::saveFile( wxString name )
{
	assert( doc );
	return doc->Save( name );
}

/*****************************************************
**
**   XmlWorker   ---   endParse
**
******************************************************/
void XmlWorker::endParse()
{
	delete doc;
	doc = 0;
}

/*****************************************************
**
**   XmlWorker   ---   parseFile
**
******************************************************/
bool XmlWorker::parseFile( wxString name )
{
	filename = name;
	return parseFile();
}

/*****************************************************
**
**   XmlWorker   ---   parseFile
**
******************************************************/
bool XmlWorker::parseFile()
{
	if ( doc ) delete doc;
	doc = new wxXmlDocument( filename );
	if ( ! doc->IsOk() )
	{
		delete doc;
		doc = 0;
		return false;
	}
	modtime[filename] = ::wxFileModificationTime( filename ) ;
	return true;
}

/*****************************************************
**
**   XmlWorker   ---   reportWrongKey
**
******************************************************/
void XmlWorker::reportWrongKey( wxString k )
{
	if ( k == wxT( "comment" ))
	{
		//err << wxT( "Found comment in XML document " ) << filename;
		//PrintLn( err );
	}
	else
	{
		printf( "Error: wrong key %s in XML document %s", str2char( k ), str2char ( filename ));
	}
}

/*****************************************************
**
**   XmlWorker   ---   reportWrongRootNode
**
******************************************************/
void XmlWorker::reportWrongRootNode( wxString k )
{
	if ( k == wxT( "comment" ))
	{
		// nothing
	}
	else
	{
		printf( "Error: wrong root node %s in XML document %s", str2char( k ), str2char ( filename ));
	}
}


