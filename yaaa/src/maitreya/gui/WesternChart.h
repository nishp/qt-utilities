/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/WesternChart.h
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

#ifndef WESTERNCHART_H_
#define WESTERNCHART_H_

#ifdef __GNUG__
	#pragma interface "WesternChart.h"
#endif

class wxRect;

class AspectExpert;
class ChartProperties;
class Horoscope;
class Painter;
class WesternChartConfig;
class Writer;

/*************************************************//**
*
*  western representation of graphical chart
*
******************************************************/
class WesternChart
{
public:

	WesternChart( const Horoscope*, const Horoscope*, const ChartProperties* );
	~WesternChart();
	void paint( Painter*, const wxRect& );
	virtual void OnDataChanged();
	void setPtUnit( const bool b = true )  { ptunit = b; }

	WesternChartConfig *getChartConfig() const { return wconf; }


private:

  const Horoscope *horoscope, *htransit;
	const ChartProperties *chartprops;
	
	AspectExpert *aexpert;
	WesternChartConfig *wconf;
	double symbolzoom;
	bool ptunit;

	int lastaspect;
	bool transitmode;
	Painter *painter;
 	Writer *writer;
	double psize, aries, xcenter, ycenter;

	double rInnerZodiac, rZodiacDegree1, rZodiacDegree5, rOuterZodiac, rPlanetRadius, rAspectRadius, rInnerHouse, rOuterHouse,
		rInnerAngleMarker, rOuterAngleMarker;
	double rCircle2InnerRadius, rCircle2Degree1, rCircle2OuterRadius, rCircle2PlanetRadius;

	void paintZodiac();
	void paintAllPlanets( const Horoscope *h, const double &markerbase );
	void getXyCoordinates( double &x, double &y, const double &len, const double &r );

	// paint a symmetric line
	void paintSymLine( const double &a, const double &rmin, const double &rmax );

	// get coordinates for a planet
	void getXYValues( double &x, double &y, const int &planet, const Horoscope *h );

	// get radius for planet. Can vary if other planets occupy the same place
	double getPRadius( const int &planet, const Horoscope *h );

	void paintAspects();
	void paintSingleAspect( const double &len1, const double &len2, const int &type );

	// TODO
	void paintArrow( const double &a, const double &rmin, const double &rmax );

	void paintDummies( const Horoscope *h );
	void paintHouses();
};

#endif

