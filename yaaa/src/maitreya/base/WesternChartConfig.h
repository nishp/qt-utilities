/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/WesternChartConfig.h
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

#ifndef _WESTERNCHARTCONFIG_H_
#define _WESTERNCHARTCONFIG_H_

#ifdef __GNUG__
	#pragma interface "WesternChartConfig.h"
#endif

#include "ChartConfig.h"
#include "ConfigLoader.h"

using namespace std;

/*************************************************//**
*
* \brief configuration for a single western chart
*
******************************************************/
class WesternChartConfig : public ChartConfig
{
	friend class XmlWorkerWesternChartConfig;
public:

	WesternChartConfig();
	virtual void setDefaultConfig();

	double innerZodiac;
	double zodiacDegree1;
	double zodiacDegree5;
	double outerZodiac;
	double planetRadius;
	double aspectRadius;
	double innerHouse;
	double outerHouse;
	double outerAngleMarker;
	double innerAngleMarker;
	double planetMarkerLength;
	bool paint1DegCircle;
	bool paint5DegCircle;
	bool paintOuterHouseCircle;
	bool paintInnerHouseCircle;
	bool paintHouseNumbers;
	bool rotateSymbols;
	int circleLineWidth;
	int lineWidth;
	int symbolZoomFactor;
	int symbolType;

	double circle2InnerRadius;
	double circle2Degree1;
	double circle2OuterRadius;
	double circle2PlanetRadius;
};

/*************************************************//**
*
* \brief contains configs for western charts
*
******************************************************/
class WesternChartConfigLoader
{
	DECLARE_CONFIGLOADER( WesternChartConfigLoader, WesternChartConfig )

public:
	WesternChartConfig *getDefaultConfig();
};

#endif

