/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/VedicChartConfig.h
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

#ifndef _VEDICCHARTCONFIG_H_
#define _VEDICCHARTCONFIG_H_

#ifdef __GNUG__
	#pragma interface "VedicChartConfig.h"
#endif

#include "ChartConfig.h"
#include "ConfigLoader.h"

/*************************************************//**
*
* \brief config for a single Vedic chart
*
******************************************************/
class VedicChartConfig : public ChartConfig
{
public:
	VedicChartConfig();
	wxString toString();
	virtual void setDefaultConfig();

	bool useSignColors;
	bool useHouseColors;
	int lineWidth;
	int outerLineWidth;
	bool doubleOuterLine;
	bool useSymbols;
	double outerZodiac;
	int markedFieldBrush;
	wxColour markedFieldColor;
};

/*************************************************//**
*
* \brief contains the configs for Vedic charts
*
******************************************************/
class VedicChartConfigLoader
{
	DECLARE_CONFIGLOADER( VedicChartConfigLoader, VedicChartConfig )

public:
	VedicChartConfig *getDefaultConfig();
};

#endif

