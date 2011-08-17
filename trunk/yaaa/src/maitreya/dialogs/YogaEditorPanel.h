/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/YogaEditorPanel.h
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

#ifndef YOGAEDITORPANEL_H
#define YOGAEDITORPANEL_H

#ifdef __GNUG__
	#pragma interface "YogaEditorPanel.h"
#endif

#include <wx/panel.h>

#include "YogaConfig.h"
//#include "BasicWidget.h"

#include <vector>

using namespace std;

// begin wxGlade: ::dependencies
// end wxGlade

// begin wxGlade: ::extracode

// end wxGlade

class wxBitmapButton;
class wxButton;
class wxCheckBox;
class wxChoice;
class wxListCtrl;
class wxListEvent;
class wxStaticBox;
class wxStaticText;
class wxTextCtrl;

class DragChart;
class MinimalVargaHoroscope;
class YogaConfig;

enum { YE_YOGA_PANEL = wxID_HIGHEST + 10000, YE_YOGA_LIST, YE_OPENFILE,
	YE_PARSE_RULE, YE_FORMAT_RULE, YE_FORMAT_ALL_RULES, YE_NEW_SOURCE,
	YE_NEW_GROUP, YE_DESCRIPTION, YE_EVALUATE, YE_UP, YE_DOWN, YE_DELETE_YOGA,
	YE_NEW_YOGA, YE_COPY_YOGA, YE_CLEAR_CHART, YE_OBJECTS
};

/*************************************************//**
*
* \brief Main panel for YogaEditor
* 
******************************************************/
//class YogaEditorPanel: public BasicWidget
class YogaEditorPanel : public wxPanel
{
	DECLARE_CLASS( YogaEditorPanel )

public:
    // begin wxGlade: YogaEditorPanel::ids
    // end wxGlade

	YogaEditorPanel( wxWindow*, wxWindowID id, MinimalVargaHoroscope* );

	void addSource( wxString );

	void addGroup( wxString );

	void loadYogas( const vector<YogaConfig*> );

	void saveYogas( vector<YogaConfig*>& );

	void clearGui();

	void clearChart();

	wxString getRuleText() const;

	bool isDirty() const { return dirty; }

	void setDirty( const bool b = true ) { dirty = b; }

	void echoOnDragChart( wxString );

	void newItem();

private:
    // begin wxGlade: YogaEditorPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnFieldChanged( wxCommandEvent& );
	void updateFieldStatus();

	void OnYogaListLeftSelected( wxListEvent& );
	void OnYogaListLeftDeSelected( wxListEvent& );

	void OnYogaListItemUp( wxCommandEvent& );
	void OnYogaListItemDown( wxCommandEvent& );
	void OnYogaListNewItem( wxCommandEvent& ) { newItem(); }
	void OnYogaListDeleteItem( wxCommandEvent& );
	void OnYogaListCopyItem( wxCommandEvent& );
	void OnKeyDown( wxKeyEvent& );
	void OnChar( wxKeyEvent& );
	void OnCharHook(wxKeyEvent& );

	void saveCurrentInput( const long& );
	void propagateDirtyEvent();

	MinimalVargaHoroscope *evaluationChart;

	bool propagateUserInput, dirty;

protected:

  virtual void doPaint( bool eraseBackground = true, const wxRect* rect = 0 ) {}

    // begin wxGlade: YogaEditorPanel::attributes
    wxStaticBox* sizer_right_staticbox;
    wxStaticBox* sizer_dragchart_staticbox;
    wxStaticBox* sizer_rule_staticbox;
    wxStaticBox* sizer_effect_staticbox;
    wxStaticBox* sizer_yogalist_staticbox;
    wxListCtrl* list_yogas;
    wxBitmapButton* button_up;
    wxBitmapButton* button_down;
    wxBitmapButton* button_new_yoga;
    wxBitmapButton* button_delete_yoga;
    wxBitmapButton* button_copy_yoga;
    wxStaticText* label_description;
    wxTextCtrl* text_description;
    wxStaticText* label_group;
    wxChoice* choice_group;
    wxBitmapButton* button_new_group;
    wxStaticText* label_source;
    wxChoice* choice_source;
    wxBitmapButton* button_new_source;
    wxCheckBox* check_higher_vargas;
    wxTextCtrl* text_effect;
    wxTextCtrl* text_rule;
    DragChart* drag_chart;
    wxButton* button_parse_rule;
    wxButton* button_evaluate;
    wxButton* button_clear_chart;
    // end wxGlade
}; // wxGlade: end class


#endif // YOGAEDITORPANEL_H
