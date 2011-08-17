/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/YogaConfig.cpp
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
	#pragma implementation "YogaConfig.h"
#endif

#include "YogaConfig.h"

#include "constants.h"
#include "Expression.h"
#include "FileConfig.h"
#include "XmlWorker.h"

#include <wx/dir.h>
#include <wx/log.h>
#include <wx/textfile.h>
#include <wx/xml/xml.h>

/*****************************************************
**
**   YogaConfig   ---   Constructor
**
******************************************************/
YogaConfig::YogaConfig()
{
	rule = 0;
	description = wxT( "New Yoga" );
	allowHigherVargas = true;
}

/*****************************************************
**
**   YogaConfig   ---   Destructor
**
******************************************************/
YogaConfig::~YogaConfig()
{
	if ( rule ) delete rule;
}

/*****************************************************
**
**   YogaConfig   ---   dump
**
******************************************************/
wxString YogaConfig::dump()
{
	wxString s;
	s << wxT( " -----   Yoga -------" ) << Endl
		<< wxT( "Description: " ) << description << Endl
		<< wxT( "Group: " ) << group << Endl
		<< wxT( "Source: " ) << source << Endl
		<< wxT( "Effect: " ) << effect << Endl
		<< wxT( "AllowHigherVargas: " ) << allowHigherVargas << Endl
		<< wxT( "Rule: " ) << rulestr << Endl;
	return s;
}

/*****************************************************
**
**   YogaConfig   ---   dumpXML
**
******************************************************/
wxString YogaConfig::dumpXML()
{
	wxString s;
	s << wxT( "	<YogaConfig>" ) << Endl
		<< wxT( "		<Description>" ) << description << wxT( "</Description>" ) << Endl
		<< wxT( "		<Effect>" ) << effect << wxT( "</Effect>" ) << Endl
		<< wxT( "		<Group>" ) << group << wxT( "</Group>" ) << Endl
		<< wxT( "		<Source>" ) << source << wxT( "</Source>" ) << Endl
		<< wxT( "		<AllowHigherVargas>" ) << ( allowHigherVargas ? wxT( "true" ) : wxT( "false" )) << wxT( "</AllowHigherVargas>" ) << Endl
		<< wxT( "		<Rule>" ) << rulestr << wxT( "</Rule>" ) << Endl
		<< wxT( "	</YogaConfig>" ) << Endl;
	return s;
}

/*************************************************//**
*
* \brief worker for XML extraction of Yoga config
*
******************************************************/
class XmlWorkerYogaConfig : public XmlWorker
{
public:
	XmlWorkerYogaConfig() : XmlWorker( -1 ) {}

	/*****************************************************
	**
	**   XmlWorkerYogaConfig   ---   readConfig
	**
	******************************************************/
	bool readConfig( vector<YogaConfig*> &defs )
	{
		wxString fn;
		wxString yogadir = FileConfig::get()->getYogaDir();
		modtime.clear();

		wxDir dir( yogadir );
		if ( ! dir.IsOpened() )
		{
			printf( "Error: cannot open Yoga config directory %s", str2char( yogadir ));
		}
		else
		{
			bool cont = dir.GetFirst( &fn, wxT( "*.xml" ), wxDIR_FILES );
			while ( cont )
			{
				// Windows implementation doesn't filter extension
				if ( fn.Right( 4 ) == wxT( ".xml" ))
				{
					readConfigFile( fn, defs );
				}
				cont = dir.GetNext(&fn);
			}
		}

		//printf( "END READ YOGA %d\n\n\n-----------------\n\n", defs.size() );
		return true;
	}

	/*****************************************************
	**
	**   XmlWorkerYogaConfig   ---   readConfigFile
	**
	******************************************************/
	bool readConfigFile( wxString fn, vector<YogaConfig*> &v )
	{
		wxString yogadir = FileConfig::get()->getYogaDir();
		if ( parseFile( yogadir + fn ))
		{
			wxXmlNode *cur = doc->GetRoot()->GetChildren();
			while ( cur != 0 )
			{
				if ( cur->GetName() == XML_NODE_YOGA ) v.push_back( this->parseYogaConfig( cur, fn ));
				else reportWrongRootNode( cur->GetName() );
				cur = cur->GetNext();
			}
			endParse();
		}
		else
		{
			printf( "Parse Error in file %s", str2char( fn ));
		}
		return true;
	}

	/*****************************************************
	**
	**   XmlWorkerYogaConfig   ---   saveConfigFile
	**
	******************************************************/
	bool saveConfigFile( wxString fn, const vector<YogaConfig*> &cfg )
	{
		wxString yogadir = FileConfig::get()->getYogaDir();
		wxString s;
		wxString filename = yogadir  + fn;

		wxTextFile wfile( filename );

		if ( ! wfile.Exists() )
		{
			if ( ! wfile.Create( filename ) )
			{
				wxLogError( wxT( "Cannot create file %s" ), (const wxChar*)filename );
				return false;
			}
		}
		else
		{
			if ( ! wfile.Open( FILE_CONF_MODE ) )
			{
				wxLogError( wxT( "Cannot open file %s" ), (const wxChar*)filename );
				return false;
			}
			wfile.Clear();
		}

		s = wxT( "<?xml version=\"1.0\"?>" );
		wfile.AddLine( s );
		s = wxT( "<!DOCTYPE YogaConfigGroup SYSTEM \"YogasConfigGroup.dtd\">" );
		wfile.AddLine( s );
		s = wxT( "<YogaConfigGroup>" );
		wfile.AddLine( s );

		for( unsigned int i = 0; i < cfg.size(); i++ )
		{
			wfile.AddLine( cfg[i]->dumpXML() );
		}

		s = wxT( "</YogaConfigGroup>" );
		wfile.AddLine( s );

		wfile.Write( wxTextFileType_None, FILE_CONF_MODE );
		wfile.Close();
		return true;
	}

private:

	/*****************************************************
	**
	**   XmlWorkerYogaConfig   ---   parseYogaConfig
	**
	******************************************************/
	YogaConfig *parseYogaConfig( wxXmlNode *cur, wxString fn )
	{
		wxString key;

		YogaConfig *cfg = new YogaConfig;
		cfg->sourcefile = fn;

		cur = cur->GetChildren();
		while( cur )
		{
			key = cur->GetName();
			if( key == XML_NODE_GROUP ) cfg->group = cur->GetNodeContent();
			else if( key == XML_NODE_DESCRIPTION ) cfg->description = cur->GetNodeContent();
			else if( key == XML_NODE_EFFECT ) cfg->effect = cur->GetNodeContent();
			else if( key == XML_NODE_SOURCE ) cfg->source = cur->GetNodeContent();
			else if( key == XML_NODE_RULESTR ) cfg->rulestr = cur->GetNodeContent();
			else if ( key == XML_NODE_ALLOW_HIGHER_VARGAS ) XMLNODE2BOOLEAN( cur->GetNodeContent(), cfg->allowHigherVargas )
			else reportWrongKey( key );
			cur = cur->GetNext();
		}
		return cfg;
	}
};

IMPLEMENT_CONFIGLOADER( YogaConfigLoader, YogaConfig, XmlWorkerYogaConfig )

/*****************************************************
**
**   YogaConfigLoader   ---   readYogaDefinitionFile
**
******************************************************/
bool YogaConfigLoader::readYogaDefinitionFile( wxString filename, vector<YogaConfig*> &v )
{
	if ( ! worker ) worker = new XmlWorkerYogaConfig;
	return ((XmlWorkerYogaConfig*)worker)->readConfigFile( filename, v );
}

/*****************************************************
**
**   YogaConfigLoader   ---   saveYogaDefinitionFile
**
******************************************************/
bool YogaConfigLoader::saveYogaDefinitionFile( wxString filename, const vector<YogaConfig*> &cfg )
{
	if ( ! worker ) worker = new XmlWorkerYogaConfig;
	return ((XmlWorkerYogaConfig*)worker)->saveConfigFile( filename, cfg );
}



