/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/TransitView.h
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

#ifndef _TRANSITVIEW_H_
#define _TRANSITVIEW_H_

#ifdef __GNUG__
	#pragma interface "TransitView.h"
#endif

#include "BasicWidget.h"
#include "NotebookWidget.h"

class BasicView;
class ChartGridWidget;
class ChartProperties;
class ChildWindow;
class Document;
class Horoscope;
class NotebookWidget;
class SbcWidget;
class SplitterWidget;
class TransitExpert;
class TransitTextWidget;
class WesternChart;

class wxCalendarEvent;
class wxChoice;
class wxDateEvent;
class wxIdleEvent;
class wxNotebookEvent;
class wxSpinCtrl;
class wxSpinEvent;

/*************************************************//**
*
* \brief transit chart display
*
******************************************************/
class TransitChartWidget : public BasicWidget
{
public:
	TransitChartWidget( wxWindow *parent, ChartProperties*, const Horoscope *h1, const Horoscope *h2, const bool &vedic,
		const wxPoint& pos = wxDefaultPosition, const wxSize& = wxDefaultSize );

	~TransitChartWidget();

	virtual void OnDataChanged();

protected:
	bool preferVedic;
	const Horoscope *doc, *htransit;
	WesternChart *wchart;
	virtual void doPaint( bool eraseBackground = true, const wxRect* rect = 0 );
	virtual bool prepareGraphic( const int& ) { return true; }

private:
	DECLARE_CLASS( TransitChart )
};

/*************************************************//**
*
* \brief view container for transits
*
******************************************************/
class TransitView : public BasicNotebookView
{
	DECLARE_CLASS( TransitView )
public:
	TransitView( wxWindow *parent, ChildWindow *frame, Document *doc, const bool &vedic );
	~TransitView();
	virtual wxString getWindowLabel( const bool shortname = false );
	virtual void postCreate();
	virtual bool dispatchCommand( const int &command );

	virtual bool supportsEWToggle() const;
	virtual bool supportsFullGraphicStyleToggle() const;
	virtual bool supportsSkinToggle() const;
	virtual bool supportsSbcStyleToggle() const;
	virtual bool supportsGraphicExport() const;
	virtual bool supportsTextExport() const;
	virtual bool isVedic() const;

protected:

	virtual void initToolItems();
	void OnDateProgress( wxScrollEvent& );
	void OnIdle( wxIdleEvent& );
	void OnDateCtrlChanged( wxCommandEvent& );
	void OnTimeCtrlWrap( wxCommandEvent& );
		
	void OnSetToCurrent( wxCommandEvent& );
	virtual void OnNotebook( wxNotebookEvent& );
	virtual void OnDataChanged();
	virtual void OnToolbarCommand();

	double getDeltaValue( const int &v );
	void resetSlider();

	void writeAspectText();
	void writeUranianText();

	TransitExpert *expert;
	TransitTextWidget *aspectText, *uranianText, *activetext;
	BasicWidget *swidget;
	ChartGridWidget *gwidget;
	SplitterWidget *tsplitter;
	BasicWidget *activewidget;

	class ShadVargaChartGridWidget *shadvargawidget;
	class VargaTab2ChartGridWidget *varga2widget;
	class VargaTab3ChartGridWidget *varga3widget;

};

#endif

