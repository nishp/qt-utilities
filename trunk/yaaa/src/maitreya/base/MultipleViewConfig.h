/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/MultipleViewConfig.h
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
#ifndef _MULTIPLEVIEWCONFIG_H_
#define _MULTIPLEVIEWCONFIG_H_

#ifdef __GNUG__
	#pragma interface "MultipleViewConfig.h"
#endif

#include "ConfigLoader.h"
#include "ViewNode.h"

using namespace std;

/*************************************************//**
*
* \brief configuration for a single mulitple view
*
******************************************************/
class MultipleViewConfig : public ViewNode
{
public:

	MultipleViewConfig() : ViewNode( 0 ) { sizex = sizey = 500; window = 0; }
	virtual ~MultipleViewConfig();
	virtual void dump( wxString& );

	wxString name, description;
	ViewNode *window;
	int sizex, sizey;

private:
	DECLARE_CLASS( MultipleViewConfig )
};

/*************************************************//**
*
* \brief contains all multiple view definitions
*
******************************************************/
class MultipleViewConfigLoader
{
	DECLARE_CONFIGLOADER( MultipleViewConfigLoader, MultipleViewConfig )
};

#endif


