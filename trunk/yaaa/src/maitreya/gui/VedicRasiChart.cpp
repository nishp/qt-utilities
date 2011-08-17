/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/VedicRasiChart.cpp
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
	#pragma implementation "VedicRasiChart.h"
#endif

#include "VedicRasiChart.h"

#include "Lang.h"
#include "ChartProperties.h"
#include "Lang.h"
#include "mathbase.h"
#include "Painter.h"
#include "VedicChartConfig.h"
#include "Writer.h"

// value for painting double frames of charts
#define RTOL_DOUBLE_FRAME .012

/*****************************************************
**
**   VedicRasiChart   ---   Constructor
**
******************************************************/
VedicRasiChart::VedicRasiChart( const ChartProperties *chartprops, const int chartcount )
 : BasicVedicChart( chartprops, 12, chartcount )
{
	centerstring_graphic = false;
}

/*****************************************************
**
**   VedicRasiChart   ---   paintChart
**
******************************************************/
void VedicRasiChart::paintChart()
{
	if ( chartprops->getVedicGraphicStyle() & VGRAPHIC_NORTH_INDIAN ) paintNorth();
	else if ( chartprops->getVedicGraphicStyle() & VGRAPHIC_EAST_INDIAN ) paintEast();
	else paintSouth();
}

/*****************************************************
**
**   VedicRasiChart   ---   paintSouth
**
******************************************************/
void VedicRasiChart::paintSouth()
{
	int i;

	const double xstep = (int)( xr / 2);
	const double ystep = (int)( yr / 2);

	// Rasi coordinates, single mode only
	const wxRect rasi_rect[12] = {
		wxRect( xcenter - xstep, ycenter - 2 * ystep, xstep, ystep ),
		wxRect( xcenter, ycenter - 2 * ystep, xstep, ystep ),
		wxRect( xcenter + xstep, ycenter - 2 * ystep, xstep, ystep ),
		wxRect( xcenter + xstep, ycenter - ystep, xstep, ystep ),
		wxRect( xcenter + xstep, ycenter, xstep, ystep ),
		wxRect( xcenter + xstep, ycenter + ystep, xstep, ystep ),

		wxRect( xcenter, ycenter + ystep, xstep, ystep ),
		wxRect( xcenter - xstep, ycenter + ystep, xstep, ystep ),
		wxRect( xcenter - 2 * xstep, ycenter + ystep, xstep, ystep  ),
		wxRect( xcenter - 2 * xstep, ycenter, xstep, ystep ),
		wxRect( xcenter - 2 * xstep, ycenter-ystep, xstep, ystep ),
		wxRect( xcenter - 2 * xstep, ycenter - 2 * ystep, xstep, ystep )
	};

	// Rasi coordinates for 1st chart (double mode only)
	const wxRect planet1_rect[12] = {
		wxRect( xcenter - xstep, ycenter - 1.5 * ystep, xstep, ystep / 2 ),
		wxRect( xcenter, ycenter - 1.5 * ystep, xstep, ystep / 2 ),
		wxRect( xcenter + xstep, ycenter - 1.5 * ystep, xstep / 2, ystep / 2 ),
		wxRect( xcenter + xstep, ycenter - ystep, xstep / 2, ystep ),
		wxRect( xcenter + xstep, ycenter, xstep / 2, ystep ),
		wxRect( xcenter + xstep, ycenter + ystep, xstep / 2, ystep / 2 ),

		wxRect( xcenter, ycenter + ystep, xstep, ystep / 2 ),
		wxRect( xcenter - xstep, ycenter + ystep, xstep, ystep / 2 ),
		wxRect( xcenter - 1.5 * xstep, ycenter + ystep, xstep / 2, ystep / 2 ),
		wxRect( xcenter - 1.5 * xstep, ycenter, xstep / 2, ystep ),
		wxRect( xcenter - 1.5 * xstep, ycenter-ystep, xstep / 2, ystep ),
		wxRect( xcenter - 1.5 * xstep, ycenter - 1.5 * ystep, xstep / 2, ystep / 2 )
	};
	
	// Rasi coordinates for 2nd chart (double mode only)
	const wxRect planet2_rect[12] = {
		wxRect( xcenter - xstep, ycenter - 2 * ystep, xstep, ystep / 2 ),
		wxRect( xcenter, ycenter - 2 * ystep, xstep, ystep / 2 ),
		wxRect( xcenter + xstep, ycenter - 2 * ystep, xstep, ystep / 2 ),
		wxRect( xcenter + 1.5 * xstep, ycenter - ystep, xstep / 2, ystep ),
		wxRect( xcenter + 1.5 * xstep, ycenter, xstep / 2, ystep ),
		wxRect( xcenter + xstep, ycenter + 1.5 * ystep, xstep, ystep / 2 ),

		wxRect( xcenter, ycenter + 1.5 * ystep, xstep, ystep / 2 ),
		wxRect( xcenter - xstep, ycenter + 1.5 * ystep, xstep, ystep / 2 ),
		wxRect( xcenter - 2 * xstep, ycenter + 1.5 * ystep, xstep, ystep / 2 ),
		wxRect( xcenter - 2 * xstep, ycenter, xstep / 2, ystep ),
		wxRect( xcenter - 2 * xstep, ycenter-ystep, xstep / 2, ystep ),
		wxRect( xcenter - 2 * xstep, ycenter - 2 * ystep, xstep, ystep / 2 )
	};

	// rectangle for separation of double charts
	const wxRect transit_rect( xcenter - 1.5 * xstep, ycenter - 1.5 * ystep, 3 * xstep, 3 * ystep );

	// fill signs if necessary
	if ( vconf->useSignColors )
	{
		for( i = ARIES; i <= PISCES; i++ )
		{
			painter->setPenColor( vconf->getSignBgColor( i ));
			painter->setBrushColor( vconf->getSignBgColor( i ));
			painter->drawRectangle( rasi_rect[i] );
		}
	}

	painter->setTransparentBrush();
	painter->setPenColor( vconf->fgColor );
	painter->setTextColor( vconf->textColor );

	//if ( ! vconf->bgImage.IsEmpty()) painter->setBackgroundImage( vconf->bgImage );
	
	// paint outer rectangle
	if( vconf->outerLineWidth > 0 )
	{
		painter->setPenWidth( vconf->outerLineWidth );
		painter->drawRectangle( xcenter - xr, ycenter - yr, 2 * xr, 2 * yr );
		if ( vconf->doubleOuterLine )
		{
			// TODO
			double xtol = Max( (int)(RTOL_DOUBLE_FRAME * xr + xr) - xr, 1 );
			double ytol = Max( (int)(RTOL_DOUBLE_FRAME * yr + yr) - yr, 1 );
			painter->drawRectangle( xcenter - xr - xtol, ycenter - yr - ytol, 2 * ( xr + xtol ), 2 * ( yr + ytol ));
		}
	}
	
	if ( vconf->lineWidth > 0 )
	{
		painter->setPenWidth( vconf->lineWidth );
		painter->setTransparentBrush();
		painter->drawLine( xcenter - xr + xstep, ycenter - yr, xcenter - xr + xstep, ycenter + yr  );
		painter->drawLine( xcenter - xr + 3 * xstep, ycenter - yr, xcenter - xr + 3 * xstep, ycenter + yr  );

		painter->drawLine( xcenter - xr + 2 * xstep, ycenter - yr, xcenter-xr + 2 * xstep, ycenter - yr + ystep  );
		painter->drawLine( xcenter - xr + 2 * xstep, ycenter + yr - ystep, xcenter - xr + 2 * xstep, ycenter + yr  );

		painter->drawLine( xcenter - xr, ycenter - yr + ystep, xcenter + xr, ycenter - yr + ystep  );
		painter->drawLine( xcenter - xr, ycenter - yr + 3 * ystep, xcenter + xr, ycenter - yr + 3 * ystep  );

		painter->drawLine( xcenter - xr, ycenter - yr + 2 * ystep, xcenter - xr + xstep, ycenter - yr + 2 * ystep  );
		painter->drawLine( xcenter + xr - xstep, ycenter - yr + 2 * ystep, xcenter + xr, ycenter - yr + 2 * ystep  );
	}
	if ( chart_count > 1 ) painter->drawRectangle( transit_rect );

	if ( ! chartprops->isBlank() )
	{
		for( i = ARIES; i <= PISCES; i++ )
		{
			if ( chart_count == 1 ) drawFieldText( rasi_rect[i], i, Align::Center, 0 );
			else
			{
				drawFieldText( planet1_rect[i], i, Align::Center, 0 );
				drawFieldText( planet2_rect[i], i, Align::Center, 1 );
			}
		}
		painter->setTextColor( vconf->fgColor );
		paintCenterString();
	}
	painter->setGraphicFont();
}

/*****************************************************
**
**   VedicRasiChart   ---   paintNorth
**
******************************************************/
void VedicRasiChart::paintNorth()
{
	Lang lang;
	wxString s;
	int current, i;

	// used for sign number/symbol display
	const int rtol = xmax / 15;

	// used for free space in the middle of the chart (leave free for center string if required)
	const int mtol = xmax / 100;

	// 1/4 of the chart
	const int xi = xr / 2;
	const int yi = yr / 2;

	// middle square for separation of 2 charts (transit or partner mode)
	const double ri_prop = .4;
  const int xm = (int)( ri_prop * xi + ( 1 - ri_prop ) * xr );
  const int ym = (int)( ri_prop * yi + ( 1 - ri_prop ) * yr );

	// text separation from chart border
	const int text_border = xr / 30;

	// Rasi coordinates in single mode
	const wxRect rasi_rect[12] = {
		wxRect( xcenter - xi, ycenter - yr, xr, yr ),
		wxRect( xcenter - xr, ycenter - yr, xr, yi ),
		wxRect( xcenter - xr, ycenter - yr, xr, yr ),
		wxRect( xcenter - xr, ycenter - yi, xr, yr ),
		wxRect( xcenter - xr, ycenter, xi, yr ),
		wxRect( xcenter - xr, ycenter + yi, xr, yi ),

		wxRect( xcenter - xi, ycenter, xr, yr ),
		wxRect( xcenter, ycenter + yi, xr, yi ),
		wxRect( xcenter, ycenter, xr, yr ),
		wxRect( xcenter, ycenter - yi, xr, yr ),
		wxRect( xcenter + xi, ycenter - yr, xi, yr ),
		wxRect( xcenter, ycenter - yr, xr, yi )
	};

	// Rasi coordinates for 2nd chart (only in double mode)
	const wxRect planet1_rect[12] = {
		wxRect( xcenter - xi, ycenter - ym, xr, xm ),
		wxRect( xcenter - xm, ycenter - ym, xi, ym ),
		wxRect( xcenter - xm, ycenter - yr, xr - xm, yr ),
		wxRect( xcenter - xm, ycenter - yi, xm, yr ),
		wxRect( xcenter - xm, ycenter, xr - xm, yr ),
		wxRect( xcenter - xr, ycenter + yi, xr, ym - yi ),

		wxRect( xcenter - xi, ycenter, xr, ym ),
		wxRect( xcenter + xi / 2, ycenter + yi, xi, ym - yi ),
		wxRect( xcenter + xi, ycenter, xm - xi, yr ),
		wxRect( xcenter, ycenter - yi, xm, yr ),
		wxRect( xcenter + xi, ycenter - ym, xm - xi, ym ),
		wxRect( xcenter + xi / 2, ycenter - ym, xi, ym - xi )
	};

	// Rasi coordinates for 2nd chart (only in double mode)
	const wxRect planet2_rect[12] = {
		wxRect( xcenter - xi, ycenter - yr, xr, yr - ym ),
		wxRect( xcenter - xr, ycenter - yr, xr, yr - ym ),
		wxRect( xcenter - xr, ycenter - yr, xr - xm, yr ),
		wxRect( xcenter - xr, ycenter - yi, xr - xm, yr ),
		wxRect( xcenter - xr, ycenter, xr - xm, yr ),
		wxRect( xcenter - xr, ycenter + ym, xr, yr - ym ),

		wxRect( xcenter - xi, ycenter + ym, xr, yr  - ym ),
		wxRect( xcenter, ycenter + ym, xr, yr - ym ),
		wxRect( xcenter + xm, ycenter, xr - xm, yr ),
		wxRect( xcenter + xm, ycenter - yi, xr - xm, yr ),
		wxRect( xcenter + xm, ycenter - yr, xr - xm, yr ),
		wxRect( xcenter, ycenter - yr, xr, yr - ym )
	};

	// Rectangles for sign names resp. numbers
	const wxRect sign_rect[12] = {
		wxRect( xcenter - rtol, ycenter - rtol, 2 * rtol, rtol ),
		wxRect( xcenter - xi - rtol / 2, ycenter - yi - rtol, rtol, rtol ),
		wxRect( xcenter - xi - rtol, ycenter - yi - rtol / 2, rtol, rtol ),
		wxRect( xcenter - 2 * rtol, ycenter - rtol, 2 * rtol, rtol ),
		wxRect( xcenter - xi - rtol, ycenter + yi - rtol / 2, rtol, rtol ),
		wxRect( xcenter - xi - rtol / 2, ycenter + yi, rtol, rtol ),

		wxRect( xcenter - rtol, ycenter, 2 * rtol, rtol ),
		wxRect( xcenter + xi - rtol / 2, ycenter + yi, rtol, rtol ),
		wxRect( xcenter + xi, ycenter + yi - rtol / 2, rtol, rtol ),
		wxRect( xcenter, ycenter - rtol, 2 * rtol, rtol ),
		wxRect( xcenter + xi, ycenter - yi - rtol / 2, rtol, rtol ),
		wxRect( xcenter + xi - rtol / 2, ycenter - yi - rtol, rtol, rtol )
	};

	// Text alignment for rasis
	const int rasi_alignment[12] = {
		Align::Center,
		Align::Top + Align::HCenter,
		Align::Left + Align::VCenter,
		Align::Center,
		Align::Left + Align::VCenter,
		Align::Bottom + Align::HCenter,

		Align::Center,
		Align::Bottom + Align::HCenter,
		Align::Right + Align::VCenter,
		Align::Center,
		Align::Right + Align::VCenter,
		Align::Top + Align::HCenter
	};

	// polygon coordinates for signs
	const wxPoint rasi_polygons[][12] = {
		{ wxPoint( xcenter-xi, ycenter - yi ), wxPoint( xcenter, ycenter ), wxPoint( xcenter+xi, ycenter - yi ), wxPoint( xcenter, ycenter - yr ) },
		{ wxPoint( xcenter-xr, ycenter - yr ), wxPoint( xcenter, ycenter-yr ), wxPoint( xcenter-xi, ycenter - yi ) },
		{ wxPoint( xcenter-xr, ycenter - yr ), wxPoint( xcenter-xr, ycenter ), wxPoint( xcenter-xi, ycenter - yi ) },
		{ wxPoint( xcenter-xr, ycenter ), wxPoint( xcenter-xi, ycenter+yi ), wxPoint( xcenter, ycenter ), wxPoint( xcenter-xi, ycenter - yi ) },
		{ wxPoint( xcenter-xr, ycenter + yr ), wxPoint( xcenter-xr, ycenter ), wxPoint( xcenter-xi, ycenter + yi ) },
		{ wxPoint( xcenter-xr, ycenter + yr ), wxPoint( xcenter, ycenter+yr ), wxPoint( xcenter-xi, ycenter + yi ) },

		{ wxPoint( xcenter-xi, ycenter+yi ), wxPoint( xcenter, ycenter+yr ), wxPoint( xcenter+xi, ycenter+yi ), wxPoint( xcenter, ycenter ) },
		{ wxPoint( xcenter, ycenter + yr ), wxPoint( xcenter+xr, ycenter+yr ), wxPoint( xcenter+xi, ycenter + yi ) },
		{ wxPoint( xcenter+xr, ycenter + yr ), wxPoint( xcenter+xr, ycenter ), wxPoint( xcenter+xi, ycenter + yi ) },
		{ wxPoint( xcenter, ycenter ), wxPoint( xcenter+xi, ycenter+yi ), wxPoint( xcenter+xr, ycenter ), wxPoint( xcenter+xi, ycenter-yi ) },
		{ wxPoint( xcenter+xr, ycenter - yr ), wxPoint( xcenter+xr, ycenter ), wxPoint( xcenter+xi, ycenter - yi ) },
		{ wxPoint( xcenter, ycenter - yr ), wxPoint( xcenter+xr, ycenter-yr ), wxPoint( xcenter+xi, ycenter - yi ) },
	};

	// rectangle for separation of double charts
	const wxRect transit_rect( xcenter - xm, ycenter - ym, 2 * xm, 2 * ym );

	// draw filled polygons
	if ( vconf->useSignColors || vconf->useHouseColors )
	{
		painter->setTransparentPen();
		for( i = ARIES; i <= PISCES; i++ )
		{
			painter->setBrushColor( vconf->getSignBgColor( redRasi( getAscendant() + i )));
			painter->drawPolygon( i % 3 ? 3 : 4, (wxPoint*)rasi_polygons[i] );
		}
	}
	painter->setPenColor( vconf->fgColor );
	painter->setTextColor( vconf->textColor );
	painter->setTransparentBrush();

	//if ( ! vconf->bgImage.IsEmpty()) painter->setBackgroundImage( vconf->bgImage );
	painter->setTransparentBrush();
	if ( vconf->outerLineWidth > 0 )
	{
		painter->setPenWidth( vconf->outerLineWidth );
		painter->drawRectangle( xcenter-xr, ycenter-yr, 2 * xr,  2 * yr );
		if ( vconf->doubleOuterLine )
		{
			double xtol = Max( (int)(RTOL_DOUBLE_FRAME * xr + xr) - xr, 1 );
			double ytol = Max( (int)(RTOL_DOUBLE_FRAME * yr + yr ) - yr, 1 );
			painter->drawRectangle( xcenter-xr-xtol, ycenter-yr-ytol, 1 + 2* ( xr + xtol ), 1 + 2 * ( yr + ytol ));
		}
	}

	if ( vconf->lineWidth > 0 )
	{
		painter->setPenWidth( vconf->lineWidth );

		// diagonal from egde to egde paint one line or two depending on chart center
		if ( ! ( chartprops->getVedicGraphicStyle() & VGRAPHIC_CHART_CENTER_NOTHING ))
		{
			painter->drawLine( xcenter - xr, ycenter - yr, xcenter - mtol, ycenter - mtol );
			painter->drawLine( xcenter + mtol, ycenter + mtol, xcenter + xr, ycenter + yr );

			painter->drawLine( xcenter - xr, ycenter + yr, xcenter - mtol, ycenter + mtol );
			painter->drawLine( xcenter + mtol , ycenter - mtol, xcenter + xr, ycenter - yr );
		}
		else
		{
			painter->drawLine( xcenter - xr, ycenter - yr, xcenter + xr, ycenter + yr );
			painter->drawLine( xcenter - xr, ycenter + yr, xcenter + xr, ycenter - yr );
		}

		painter->drawLine( xcenter, ycenter + yr, xcenter + xr, ycenter );
		painter->drawLine( xcenter, ycenter + yr, xcenter - xr, ycenter );
		painter->drawLine( xcenter, ycenter - yr, xcenter - xr, ycenter );
		painter->drawLine( xcenter, ycenter - yr, xcenter + xr, ycenter );
	}

	if ( chart_count == 2 ) painter->drawRectangle( transit_rect );

	/*********************************************************
	**  draw number of houses
	*********************************************************/
	if ( ! ( chartprops->getVedicGraphicStyle() & VGRAPHIC_NORTH_INDIAN_SYMBOL )) painter->setGraphicFont();
	else painter->setSymbolFont();

	for( i = ARIES; i <= PISCES; i++ )
	{
		current = redRasi( getAscendant() + i );
		//painter->drawRectangle( sign_rect[i] );
		if ( chartprops->getVedicGraphicStyle() & VGRAPHIC_NORTH_INDIAN_ASC ) s.Printf( wxT( "%d" ),  current + 1 );
		else if ( chartprops->getVedicGraphicStyle() & VGRAPHIC_NORTH_INDIAN_NUMBER ) s.Printf( wxT( "%d" ),  current + 1 );
		else if ( chartprops->getVedicGraphicStyle() & VGRAPHIC_NORTH_INDIAN_SHORT ) s = text_writer->getSignName( current, TSHORT );
		else s = lang.getSignSymbolCode( current );
		painter->drawSimpleText( sign_rect[i], s );

		// leave after ascendant for style == 0
		if ( chartprops->getVedicGraphicStyle() & VGRAPHIC_NORTH_INDIAN_ASC ) break;
	}
	painter->setGraphicFont();

	/*********************************************************
	**  draw planets
	*********************************************************/
	if ( ! chartprops->isBlank() )
	{

		for( i = ARIES; i <= PISCES; i++ )
		{
			if ( chart_count == 1 ) drawFieldText( rasi_rect[i], redRasi( i + getAscendant() ), rasi_alignment[i], 0, text_border );
			else
			{
				drawFieldText( planet1_rect[i], redRasi( i + getAscendant() ), rasi_alignment[i], 0, text_border );
				drawFieldText( planet2_rect[i], redRasi( i + getAscendant() ), rasi_alignment[i], 1, text_border );
			}
		}

		painter->setTextColor( vconf->fgColor );
		paintCenterString();
	}
	painter->setGraphicFont();
}

/*****************************************************
**
**   VedicRasiChart   ---   paintEast
**
******************************************************/
void VedicRasiChart::paintEast()
{
	int i;

	// inner square of chart
  const int xi = (int)(xr*0.28);
  const int yi = (int)(yr*0.28);

	// middle square for separation of 2 charts
	const double ri_prop = .4;
  const int xm = (int)( ri_prop * xi + ( 1 - ri_prop ) * xr );
  const int ym = (int)( ri_prop * yi + ( 1 - ri_prop ) * yr );

	const int text_border = xr / 30;

	// Rasi coordinates in single mode
	const wxRect rasi_rect[12] = {
		wxRect( xcenter - xi, ycenter - yr, 2 * xi, yr - yi ),
		wxRect( xcenter - xr, ycenter - yr, xr - xi, yr - yi ),
		wxRect( xcenter - xr, ycenter - yr, xr - xi, yr - yi ),
		wxRect( xcenter - xr, ycenter - yi, xr - xi, 2 * yi ),
		wxRect( xcenter - xr, ycenter + yi, xr - xi, yr - yi ),
		wxRect( xcenter - xr, ycenter + yi, xr - xi, yr - yi ),

		wxRect( xcenter - xi, ycenter + yi, 2 * xi, yr - yi ),
		wxRect( xcenter + xi, ycenter + yi, xr - xi, yr - yi ),
		wxRect( xcenter + xi, ycenter + yi, xr - xi, yr - yi ),
		wxRect( xcenter + xi, ycenter - yi, xr - xi, 2 * yi ),
		wxRect( xcenter + xi, ycenter - yr, xr - xi, yr - yi ),
		wxRect( xcenter + xi, ycenter - yr, xr - xi, yr - yi ),
	};

	// Rasi coordinates for 1st chart (only in double mode)
	const wxRect planet1_rect[12] = {
		wxRect( xcenter - xi, ycenter - ym, 2 * xi, yr - ym ),
		wxRect( xcenter - xm, ycenter - ym, xm - xi, ym - yi ),
		wxRect( xcenter - xm, ycenter - ym, xm - xi, ym - yi ),
		wxRect( xcenter - xm, ycenter - yi, xm - xi, 2 * yi ),
		wxRect( xcenter - xm, ycenter + yi, xm - xi, ym - yi ),
		wxRect( xcenter - xm, ycenter + yi, xm - xi, ym - yi ),

		wxRect( xcenter - xi, ycenter + yi, 2 * xi, ym - yi ),
		wxRect( xcenter + xi, ycenter + yi, xm - xi, ym - yi ),
		wxRect( xcenter + xi, ycenter + yi, xm - xi, ym - yi ),
		wxRect( xcenter + xi, ycenter - yi, xm - xi, 2 * yi ),
		wxRect( xcenter + xi, ycenter - ym, xm - xi, ym - yi ),
		wxRect( xcenter + xi, ycenter - ym, xm - xi, ym - yi )
	};

	// Rasi coordinates for 2nd chart (only in double mode)
	const wxRect planet2_rect[12] = {
		wxRect( xcenter - xi, ycenter - yr, 2 * xi, yr - ym ),
		wxRect( xcenter - xm, ycenter - yr, xm - xi, yr - ym ),
		wxRect( xcenter - xr, ycenter - yr, xr - xi, yr - yi ),
		wxRect( xcenter - xr, ycenter - yi, xr - xm, 2 * yi ),
		wxRect( xcenter - xr, ycenter + yi, xr - xm, yr - yi ),
		wxRect( xcenter - xm, ycenter + ym, xr - xm, yr - ym ),

		wxRect( xcenter - xi, ycenter + ym, 2 * xi, yr - ym ),
		wxRect( xcenter + xi, ycenter + ym, xr - xm, yr - ym ),
		wxRect( xcenter + xm, ycenter + yi, xr - xm, yr - yi ),
		wxRect( xcenter + xm, ycenter - yi, xr - xm, 2 * yi ),
		wxRect( xcenter + xm, ycenter - ym, xr - xm, yr - ym ),
		wxRect( xcenter + xi, ycenter - yr, xr - xi, yr - ym ),
	};

	// Text alignment for rasis
	const int rasi_alignment[12] = {
		Align::Center,
		Align::Right + Align::Top,
		Align::Left + Align::Bottom,
		Align::Center,
		Align::Left + Align::Top,
		Align::Right + Align::Bottom,

		Align::Center,
		Align::Left + Align::Bottom,
		Align::Right + Align::Top,
		Align::Center,
		Align::Right + Align::Bottom,
		Align::Left + Align::Top
	};

	// rectangle for separation of double charts
	const wxRect transit_rect( xcenter - xm, ycenter - ym, 2 * xm, 2 * ym );

	painter->setPenWidth( vconf->outerLineWidth );

	// draw filled polygons resp. rectangles if required
	if ( vconf->useSignColors )
	{
		painter->setTransparentPen();
		int a = 0;

		painter->setBrushColor( vconf->getSignBgColor( a  ));
		painter->drawRectangle( xcenter - xi, ycenter - yr, 2 * xi, yr - yi );

		painter->setBrushColor( vconf->getSignBgColor( redRasi( a + 1 )));
		wxPoint ptaurus[3] = { wxPoint( xcenter - xr, ycenter - yr ), wxPoint( xcenter - xi, ycenter - yr ), wxPoint( xcenter-xi, ycenter - yi ) };
		painter->drawPolygon( 3, ptaurus );

		painter->setBrushColor( vconf->getSignBgColor( redRasi( a + 2 )));
		wxPoint pgem[3] = { wxPoint( xcenter - xr, ycenter - yr ), wxPoint( xcenter-xr, ycenter - yi ), wxPoint( xcenter - xi, ycenter - yi ) };
		painter->drawPolygon( 3, pgem );

		painter->setBrushColor( vconf->getSignBgColor( redRasi( a + 3 )));
		painter->drawRectangle( xcenter - xr, ycenter - yi, xr - xi, 2 * yi );

		painter->setBrushColor( vconf->getSignBgColor( redRasi( a + 4 )));
		wxPoint pleo[3] = { wxPoint( xcenter - xr, ycenter + yi ), wxPoint( xcenter - xi, ycenter + yi ), wxPoint( xcenter - xr, ycenter + yr ) };
		painter->drawPolygon( 3, pleo );

		painter->setBrushColor( vconf->getSignBgColor( redRasi( a + 5 )));
		wxPoint pvirgo[3] = { wxPoint( xcenter - xr, ycenter + yr ), wxPoint( xcenter - xi, ycenter + yr ), wxPoint( xcenter - xi, ycenter + yi ) };
		painter->drawPolygon( 3, pvirgo );

		painter->setBrushColor( vconf->getSignBgColor( redRasi( a + 6 )));
		painter->drawRectangle( xcenter - xi, ycenter + yi, 2 * xi, yr - yi );

		painter->setBrushColor( vconf->getSignBgColor( redRasi( a + 7 )));
		wxPoint pscorp[3] = { wxPoint( xcenter + xi, ycenter + yr ), wxPoint( xcenter + xr, ycenter + yr ),
			wxPoint( xcenter + xi, ycenter + yi ) };
		painter->drawPolygon( 3, pscorp );

		painter->setBrushColor( vconf->getSignBgColor( redRasi( a + 8 )));
		wxPoint p2[3] = { wxPoint( xcenter + xi, ycenter + yi ), wxPoint( xcenter + xr, ycenter + yi ), wxPoint( xcenter + xr, ycenter + yr ) };
		painter->drawPolygon( 3, p2 );

		painter->setBrushColor( vconf->getSignBgColor( redRasi( a + 9 )));
		painter->drawRectangle( xcenter + xi, ycenter - yi, xr - xi, 2 * yi );

		painter->setBrushColor( vconf->getSignBgColor( redRasi( a + 10 )));
		wxPoint paqua[3] = { wxPoint( xcenter + xi, ycenter - yi ), wxPoint( xcenter + xr, ycenter - yi ), wxPoint( xcenter + xr, ycenter - yr ) };
		painter->drawPolygon( 3, paqua );

		painter->setBrushColor( vconf->getSignBgColor( redRasi( a + 11 )));
		wxPoint ppisc[3] = { wxPoint( xcenter + xi, ycenter - yr ), wxPoint( xcenter + xr, ycenter - yr ),
			wxPoint( xcenter + xi, ycenter - yi ) };
		painter->drawPolygon( 3, ppisc );
	}
	painter->setPenColor( vconf->fgColor );
	painter->setTextColor( vconf->textColor );
	painter->setTransparentBrush();
	//if ( ! vconf->bgImage.IsEmpty()) painter->setBackgroundImage( vconf->bgImage );
	if ( vconf->outerLineWidth > 0 )
	{
		painter->setPenWidth( vconf->outerLineWidth );
		painter->drawRectangle( xcenter - xr, ycenter - yr, 2 * xr, 2 * yr );
		if ( vconf->doubleOuterLine )
		{
			double xtol = Max( (int)(RTOL_DOUBLE_FRAME * xr + xr) - xr, 1 );
			double ytol = Max( (int)(RTOL_DOUBLE_FRAME * yr + yr) - yr, 1 );
			painter->drawRectangle( xcenter-xr-xtol, ycenter-yr-ytol, 2 * ( xr + xtol ), 2 * ( yr + ytol ));
		}
	}

	if( vconf->lineWidth > 0 )
	{
		painter->setPenWidth( vconf->lineWidth );
		painter->drawLine( xcenter - xi, ycenter - yr, xcenter - xi, ycenter + yr );

		painter->drawLine( xcenter + xi, ycenter-yr, xcenter+xi, ycenter+yr );

		painter->drawLine( xcenter - xr, ycenter + yi, xcenter + xr, ycenter + yi );
		painter->drawLine( xcenter - xr, ycenter - yi, xcenter + xr, ycenter - yi );

		painter->drawLine( xcenter - xr, ycenter + yr, xcenter - xi, ycenter + yi );
		painter->drawLine( xcenter - xr, ycenter - yr, xcenter - xi, ycenter - yi );

		painter->drawLine( xcenter + xr, ycenter + yr, xcenter + xi, ycenter + yi );
		painter->drawLine( xcenter + xr, ycenter - yr, xcenter + xi, ycenter - yi );

	}
	if ( chart_count == 2 ) painter->drawRectangle( transit_rect );

	painter->setBrushColor( vconf->bgColor );
	if ( ! chartprops->isBlank() )
	{
		for( i = ARIES; i <= PISCES; i++ )
		{
			if ( chart_count == 1 ) drawFieldText( rasi_rect[i], i, rasi_alignment[i], 0, text_border );
			else
			{
				drawFieldText( planet1_rect[i], i, rasi_alignment[i], 0, text_border );
				drawFieldText( planet2_rect[i], i, rasi_alignment[i], 1, text_border );
			}
		}
		painter->setTextColor( vconf->fgColor );
		paintCenterString();
	}
	painter->setGraphicFont();
}

/*****************************************************
**
**   VedicRasiChart   ---   paintCenterString
**
******************************************************/
void VedicRasiChart::paintCenterString()
{
	if ( chartprops->getVedicGraphicStyle() & VGRAPHIC_CHART_CENTER_NOTHING ) return;

	if ( centerstring_graphic ) painter->setSymbolFont();
	else painter->setGraphicFont();

	painter->setPenColor( vconf->fgColor );
	painter->setTextBackgroundColor( vconf->bgColor );
	painter->drawSimpleText( wxRect( (int)( xcenter - xr), (int)( ycenter - yr ), (int)( 2 * xr ), (int)( 2 * yr )), getCenterString() );
}


