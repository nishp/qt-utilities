/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/YogaConfig.h
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

#ifndef _YOGACONFIG_H_
#define _YOGACONFIG_H_

#ifdef __GNUG__
	#pragma interface "YogaConfig.h"
#endif

#include <wx/string.h>
#include <vector>

#include "ConfigLoader.h"

using namespace std;

/*************************************************//**
*
* \brief config and MDL descrition for a single Yoga configuration
*
******************************************************/
class YogaConfig
{
public:
	YogaConfig();
	~YogaConfig();

	wxString dump();
	wxString dumpXML();

	wxString group, description, effect, source, rulestr, sourcefile;
	bool allowHigherVargas;
	class Rule *rule;
};

/*************************************************//**
*
* \brief reads and contains the system wide Yoga configs
*
******************************************************/
class YogaConfigLoader
{
	DECLARE_CONFIGLOADER( YogaConfigLoader, YogaConfig )

public:
	bool readYogaDefinitionFile( wxString filename, vector<YogaConfig*>&  );
	bool saveYogaDefinitionFile( wxString filename, const vector<YogaConfig*>& );

};

#endif


