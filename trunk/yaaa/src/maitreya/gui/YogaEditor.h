/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/YogaEditor.h
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

#ifndef YOGAEDITOR_H
#define YOGAEDITOR_H

#ifdef __GNUG__
	#pragma interface "YogaEditor.h"
#endif

#include "BasicView.h"

#include "YogaConfig.h"
#include "YogaEditorPanel.h"

class wxBitmapButton;
class wxButton;
class wxCheckBox;
class wxChoice;
class wxListCtrl;
class wxListEvent;
class wxPanel;
class wxStaticBox;
class wxStaticText;
class wxTextCtrl;

class DragChart;
class	MinimalVargaHoroscope;
class Rule;
class YogaConfigLoader;
class YogaExpert;

/*************************************************//**
*
* \brief Editor tool for Yoga configurations
* 
******************************************************/
class YogaEditor: public BasicView
{
	DECLARE_CLASS( YogaEditor )
public:

	YogaEditor( wxWindow *parent, ChildWindow *frame );
	~YogaEditor();

  virtual wxString getWindowLabel( const bool shortname = false );

	virtual bool queryClose();

private:

	// File menu and toolbar
	void OnNewFile( wxCommandEvent& );
	void OnOpenFile( wxCommandEvent& );
	void OnSaveFile( wxCommandEvent& ) { saveFile(); }
	void OnSaveFileAs( wxCommandEvent& ) { saveFileAs(); }
	void OnCloseFile( wxCommandEvent& ) { closeFile(); }
	void OnClose( wxCommandEvent& );

	// command buttons in dialog
	void OnNewSource( wxCommandEvent& );
	void OnNewGroup( wxCommandEvent& );
	void OnParseRule( wxCommandEvent& ) { parseRule(); }
	void OnEvaluateRule( wxCommandEvent& );
	void OnSelectObjects( wxCommandEvent& );
	void OnClearChart( wxCommandEvent& );

	void loadFile( wxString filename );
	bool closeFile();
	bool saveFile();
	bool saveFileAs();
	Rule *parseRule();
	void OnIdle( wxIdleEvent& );

	void updateUiItems();

	wxString filename;
	YogaEditorPanel *panel;
	YogaConfigLoader *loader;
	YogaExpert *yogaexpert;
	MinimalVargaHoroscope *evaluationChart;
	
};

#endif

