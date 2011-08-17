/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/YogaEditorPanel.cpp
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
	#pragma implementation "YogaEditorPanel.h"
#endif

#include "YogaEditorPanel.h"

#include <wx/bitmap.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/image.h>
#include <wx/listctrl.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>

#include "constants.h"
#include "DragChart.h"
#include "guibase.h"
#include "IconProvider.h"
#include "YogaConfig.h"

IMPLEMENT_CLASS( YogaEditorPanel, wxPanel )

// begin wxGlade: ::extracode

// end wxGlade

/*****************************************************
**
**   YogaEditorPanel   ---   Constructor 
**
******************************************************/
YogaEditorPanel::YogaEditorPanel( wxWindow* parent, wxWindowID id, MinimalVargaHoroscope *chart )
 : wxPanel( parent, id, wxPoint( 0, 0 ), wxDefaultSize, wxTAB_TRAVERSAL | wxWANTS_CHARS | wxSUNKEN_BORDER ),
//:	BasicWidget( parent, 0 ),
		evaluationChart( chart )
{
    // begin wxGlade: YogaEditorPanel::YogaEditorPanel
    sizer_effect_staticbox = new wxStaticBox(this, -1, _("Effect"));
    sizer_rule_staticbox = new wxStaticBox(this, -1, _("Rule"));
    sizer_dragchart_staticbox = new wxStaticBox(this, -1, _("Drag and Drop Chart"));
    sizer_right_staticbox = new wxStaticBox(this, -1, _("Properties of Selected Yoga"));
    sizer_yogalist_staticbox = new wxStaticBox(this, -1, _("Yogas"));
    list_yogas = new wxListCtrl(this, YE_YOGA_LIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_AUTOARRANGE|wxLC_NO_HEADER|wxLC_SINGLE_SEL|wxSUNKEN_BORDER|wxFULL_REPAINT_ON_RESIZE);
    button_up = new wxBitmapButton(this, YE_UP, wxNullBitmap);
    button_down = new wxBitmapButton(this, YE_DOWN, wxNullBitmap);
    button_new_yoga = new wxBitmapButton(this, YE_NEW_YOGA, wxNullBitmap);
    button_delete_yoga = new wxBitmapButton(this, YE_DELETE_YOGA, wxNullBitmap);
    button_copy_yoga = new wxBitmapButton(this, YE_COPY_YOGA, wxNullBitmap);
    label_description = new wxStaticText(this, YE_DESCRIPTION, _("Description"));
    text_description = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_group = new wxStaticText(this, wxID_ANY, _("Group"));
    const wxString *choice_group_choices = NULL;
    choice_group = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, choice_group_choices, 0);
    button_new_group = new wxBitmapButton(this, YE_NEW_GROUP, wxNullBitmap);
    label_source = new wxStaticText(this, wxID_ANY, _("Source"));
    const wxString *choice_source_choices = NULL;
    choice_source = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, choice_source_choices, 0);
    button_new_source = new wxBitmapButton(this, YE_NEW_SOURCE, wxNullBitmap);
    check_higher_vargas = new wxCheckBox(this, wxID_ANY, _("Allow Higher Vargas"));
    text_effect = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    text_rule = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    drag_chart = new DragChart(this, wxID_ANY, evaluationChart);
    button_parse_rule = new wxButton(this, YE_PARSE_RULE, _("Parse Rule"));
    button_evaluate = new wxButton(this, YE_EVALUATE, _("Evaluate"));
    button_clear_chart = new wxButton(this, YE_CLEAR_CHART, _("Clear Chart"));

    set_properties();
    do_layout();
    // end wxGlade


	propagateUserInput = true;
	dirty = false;

	Connect( YE_DOWN, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( YogaEditorPanel::OnYogaListItemDown ));
	Connect( YE_UP, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( YogaEditorPanel::OnYogaListItemUp ));
	Connect( YE_NEW_YOGA, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( YogaEditorPanel::OnYogaListNewItem ));
	Connect( YE_DELETE_YOGA, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( YogaEditorPanel::OnYogaListDeleteItem ));
	Connect( YE_COPY_YOGA, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( YogaEditorPanel::OnYogaListCopyItem ));

	// changes events of input items
	Connect( wxID_ANY, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( YogaEditorPanel::OnFieldChanged ));
	Connect( wxID_ANY, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( YogaEditorPanel::OnFieldChanged ));
	Connect( wxID_ANY, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( YogaEditorPanel::OnFieldChanged ));

	// events in Yoga list box
	Connect( YE_YOGA_LIST, wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( YogaEditorPanel::OnYogaListLeftSelected ));
	Connect( YE_YOGA_LIST, wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( YogaEditorPanel::OnYogaListLeftDeSelected ));

	// TODO delegate key events
	//Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( YogaEditorPanel::OnKeyDown ));

	updateFieldStatus();
}

/*****************************************************
**
**   YogaEditorPanel   ---   addSource
**
******************************************************/
void YogaEditorPanel::addSource( wxString s )
{
	choice_source->Append( s );
}

/*****************************************************
**
**   YogaEditorPanel   ---   addGroup
**
******************************************************/
void YogaEditorPanel::addGroup( wxString s )
{
	choice_group->Append( s );
}

/*****************************************************
**
**   YogaEditorPanel   ---   OnCharHook
**
******************************************************/
void YogaEditorPanel::OnCharHook(wxKeyEvent& event)
{
	//if ( event.GetKeyCode() == WXK_ESCAPE )
	//int i = event.GetKeyCode();
	//int g = 0;
}

/*****************************************************
**
**   YogaEditorPanel   ---   loadYogas
**
******************************************************/
void YogaEditorPanel::loadYogas( const vector<YogaConfig*> yogas )
{
	bool found;
	unsigned i, j;
	vector<wxString> sources, groups;
	wxListItem item;

	propagateUserInput = false;

	choice_group->Clear();
	choice_source->Clear();
	list_yogas->DeleteAllItems();

	for( i = 0; i < yogas.size(); i++ )
	{
		wxListItem item;
		item.SetId( i );
		item.SetText( yogas[i]->description );
		item.SetData( yogas[i] );
		list_yogas->InsertItem( item );

		// sources
		found = false;
		for( j = 0; j < sources.size(); j++ )
		{
			if ( sources[j] == yogas[i]->source )
			{
				found = true;
				break;
			}
		}
		if ( ! found ) sources.push_back( yogas[i]->source );

		// groups
		found = false;
		for( j = 0; j < groups.size(); j++ )
		{
			if ( groups[j] == yogas[i]->group )
			{
				found = true;
				break;
			}
		}
		if ( ! found ) groups.push_back( yogas[i]->group );
	}
	list_yogas->SetItemState( 0, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	list_yogas->Arrange();

	for( j = 0; j < sources.size(); j++ )
	{
		choice_source->Append( sources[j] );
	}
	choice_source->SetSelection( 0 );

	for( j = 0; j < groups.size(); j++ )
	{
		choice_group->Append( groups[j] );
	}
	choice_group->SetSelection( 0 );

	propagateUserInput = true;
}

/*****************************************************
**
**   YogaEditorPanel   ---   saveYogas
**
******************************************************/
void YogaEditorPanel::saveYogas( vector<YogaConfig*> &yogas )
{
	long item = -1;
	int count = 0;

	// save selected item
	item = list_yogas->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
	if ( item >= 0 ) saveCurrentInput( item );

	// iterate 
	item = list_yogas->GetNextItem( -1, wxLIST_NEXT_ALL, wxLIST_STATE_DONTCARE );
	while ( item != -1 )
	{
		count++;
		YogaConfig *cfg = (YogaConfig*)list_yogas->GetItemData( item );
		yogas.push_back( cfg );
		item = list_yogas->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_DONTCARE );
	}
	dirty = false;
}

/*****************************************************
**
**   YogaEditorPanel   ---   OnYogaListLeftDeSelected
**
******************************************************/
void YogaEditorPanel::OnYogaListLeftDeSelected( wxListEvent &event )
{
	saveCurrentInput( event.GetIndex() );
	text_description->Clear();
	text_effect->Clear();
	text_rule->Clear();
	choice_group->SetSelection( -1 );
	choice_source->SetSelection( -1 );
}

/*****************************************************
**
**   YogaEditorPanel   ---   saveCurrentInput
**
******************************************************/
void YogaEditorPanel::saveCurrentInput( const long &item )
{
	if ( item < 0 ) return;

	propagateUserInput = false;
	YogaConfig *yoga = (YogaConfig*)list_yogas->GetItemData( item );
	yoga->description = text_description->GetValue();
	yoga->group = choice_group->GetStringSelection();
	yoga->source = choice_source->GetStringSelection();
	yoga->allowHigherVargas = check_higher_vargas->GetValue();
	yoga->effect = text_effect->GetValue();
	yoga->rulestr = text_rule->GetValue();

	propagateUserInput = true;
}

/*****************************************************
**
**   YogaEditorPanel   ---   OnYogaListLeftSelected
**
******************************************************/
void YogaEditorPanel::OnYogaListLeftSelected( wxListEvent& event )
{
	long item = -1;
	item = list_yogas->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
	if ( item < 0 ) return;
	
	propagateUserInput = false;
	YogaConfig *yoga = (YogaConfig*)list_yogas->GetItemData( item );
	text_description->SetValue( yoga->description );
	
	choice_group->SetStringSelection( yoga->group );
	choice_source->SetStringSelection( yoga->source );

	check_higher_vargas->SetValue( yoga->allowHigherVargas );
	text_effect->SetValue( yoga->effect );
	text_rule->SetValue( yoga->rulestr );
	propagateUserInput = true;
}

/*****************************************************
**
**   YogaEditorPanel   ---   OnFieldChanged
**
******************************************************/
void YogaEditorPanel::OnFieldChanged( wxCommandEvent &event )
{
	long item = -1;
	item = list_yogas->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
	YogaConfig *cfg = 0;
	if ( item != -1 )
	{
		cfg = (YogaConfig*)list_yogas->GetItemData( item );
	}

	if ( event.GetEventObject() == (wxObject*)text_description )
	{
		if ( cfg && cfg->description != text_description->GetValue() )
		{
			cfg->description = text_description->GetValue();
			list_yogas->SetItemText( item, cfg->description );
		}
	}
	updateFieldStatus();
	propagateDirtyEvent();
}

/*****************************************************
**
**   YogaEditorPanel   ---   updateFieldStatus
**
******************************************************/
void YogaEditorPanel::updateFieldStatus()
{
	int size = list_yogas->GetItemCount();

	long item = -1;
	item = list_yogas->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
	YogaConfig *cfg = 0;
	if ( item != -1 )
	{
		cfg = (YogaConfig*)list_yogas->GetItemData( item );
	}

	// only if selected item is not first
	button_up->Enable( size > 0 && item > 0 );

	// only if selected item is not last
	button_down->Enable( size > 0 && item < size -1 );

	// only if there is a selected item
	button_delete_yoga->Enable( size > 0 && item >= 0 );
	button_copy_yoga->Enable( size > 0 && item >= 0 );

	choice_group->Enable( size > 0 );
	choice_source->Enable( size > 0 );
	check_higher_vargas->Enable( size > 0 );
	text_description->Enable( size > 0 );
	text_effect->Enable( size > 0 );
	text_rule->Enable( size > 0 );

	// Rule evaluation buttons, only if rule text filed is not empty
	button_parse_rule->Enable( text_rule->IsEnabled() && ! text_rule->IsEmpty() );
	button_evaluate->Enable( text_rule->IsEnabled() && ! text_rule->IsEmpty() );
}

/*****************************************************
**
**   YogaEditorPanel   ---   OnYogaListItemUp
**
******************************************************/
void YogaEditorPanel::OnYogaListItemUp( wxCommandEvent &event )
{
	long item = -1;
	item = list_yogas->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );

	if ( item < 0  ) return;

	YogaConfig *cfg = (YogaConfig*)list_yogas->GetItemData( item );

	list_yogas->DeleteItem( item );
	wxListItem listItem;
	listItem.SetId( item - 1 );
	listItem.SetData( cfg );
	listItem.SetText( cfg->description );
	list_yogas->InsertItem( listItem );
	list_yogas->SetItemState( item - 1, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
	list_yogas->EnsureVisible( item - 1 );
	propagateDirtyEvent();
}

/*****************************************************
**
**   YogaEditorPanel   ---   OnYogaListItemDown
**
******************************************************/
void YogaEditorPanel::OnYogaListItemDown( wxCommandEvent &event )
{
	long item = -1;
	item = list_yogas->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );

	if ( item < 0  || item == list_yogas->GetItemCount() ) return;

	YogaConfig *cfg = (YogaConfig*)list_yogas->GetItemData( item );

	list_yogas->DeleteItem( item );
	wxListItem listItem;
	listItem.SetId( item + 1 );
	listItem.SetData( cfg );
	listItem.SetText( cfg->description );
	list_yogas->InsertItem( listItem );
	list_yogas->SetItemState( item + 1, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
	list_yogas->EnsureVisible( item + 1 );
	propagateDirtyEvent();
}

/*****************************************************
**
**   YogaEditorPanel   ---   newItem
**
******************************************************/
void YogaEditorPanel::newItem()
{
	long item = -1;
	item = list_yogas->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );

	YogaConfig *cfg = new YogaConfig();
	wxListItem listItem;
	listItem.SetId( item + 1 );
	listItem.SetData( cfg );
	listItem.SetText( cfg->description );
	list_yogas->InsertItem( listItem );
	list_yogas->SetItemState( item + 1, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
	list_yogas->EnsureVisible( item + 1 );
	propagateDirtyEvent();
}

/*****************************************************
**
**   YogaEditorPanel   ---   OnYogaListDeleteItem
**
******************************************************/
void YogaEditorPanel::OnYogaListDeleteItem( wxCommandEvent &event )
{
	long item = -1;
	item = list_yogas->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
	if ( item < 0 ) return;

	YogaConfig *cfg = (YogaConfig*)list_yogas->GetItemData( item );
	int ret = doMessageBox( this, wxString::Format( _( "Do you want to delete the Yoga %s?" ),
		(const wxChar*)cfg->description ), wxYES_NO  | wxCENTRE );

	if ( ret == wxID_YES )
	{
		list_yogas->DeleteItem( item );
		list_yogas->SetItemState( item - 1, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
		list_yogas->EnsureVisible( item - 1 );
		delete cfg;
		propagateDirtyEvent();
	}
}

/*****************************************************
**
**   YogaEditorPanel   ---   OnYogaListCopyItem
**
******************************************************/
void YogaEditorPanel::OnYogaListCopyItem( wxCommandEvent &event )
{
	long item = -1;
	item = list_yogas->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
	if ( item < 0 ) return;

	YogaConfig *origcfg = (YogaConfig*)list_yogas->GetItemData( item );
	YogaConfig *cfg = new YogaConfig( *origcfg );

	wxListItem listItem;
	listItem.SetId( item + 1 );
	listItem.SetData( cfg );
	listItem.SetText( cfg->description );
	
	list_yogas->InsertItem( listItem );
	list_yogas->SetItemState( item + 1, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
	list_yogas->EnsureVisible( item + 1 );
	propagateDirtyEvent();
}

/*****************************************************
**
**   YogaEditorPanel   ---   propagateDirtyEvent
**
******************************************************/
void YogaEditorPanel::propagateDirtyEvent()
{
	if ( ! propagateUserInput ) return;
	dirty = true;
}

/*****************************************************
**
**   YogaEditorPanel   ---   clearGui
**
******************************************************/
void YogaEditorPanel::clearGui()
{
	list_yogas->DeleteAllItems();

	text_description->SetValue( wxEmptyString );
	choice_group->Clear();
	choice_source->Clear();
	check_higher_vargas->SetValue( false );
	text_effect->SetValue( wxEmptyString );
	text_rule->SetValue( wxEmptyString );

	clearChart();
}

/*****************************************************
**
**   YogaEditorPanel   ---   clearChart
**
******************************************************/
void YogaEditorPanel::clearChart()
{
	drag_chart->clearChart();
	drag_chart->Refresh();
}

/*****************************************************
**
**   YogaEditorPanel   ---   getRuleText
**
******************************************************/
wxString YogaEditorPanel::getRuleText() const
{
	return text_rule->GetValue();
}

/*****************************************************
**
**   YogaEditorPanel   ---   OnKeyDown
**
******************************************************/
void YogaEditorPanel::OnKeyDown( wxKeyEvent &event )
{
	event.ResumePropagation( wxEVENT_PROPAGATE_MAX );
	event.Skip();
}

/*****************************************************
**
**   YogaEditorPanel   ---   OnChar
**
******************************************************/
void YogaEditorPanel::OnChar( wxKeyEvent &event )
{
	event.ResumePropagation( wxEVENT_PROPAGATE_MAX );
	event.Skip();
}

/*****************************************************
**
**   YogaEditorPanel   ---   echoOnDragChart
**
******************************************************/
void YogaEditorPanel::echoOnDragChart( wxString s )
{
	drag_chart->echo( s );
}

/*****************************************************
**
**   YogaEditorPanel   ---   set_properties
**
******************************************************/
void YogaEditorPanel::set_properties()
{
	// setup list box
	const int w = 200;
	const int imagesize = 20;
	
	wxListItem itemCol;
	itemCol.m_mask = wxLIST_MASK_TEXT | wxLIST_MASK_IMAGE;
	itemCol.m_text = wxT( "dummy" );
	list_yogas->InsertColumn( 0, itemCol );
	list_yogas->SetColumnWidth( 0, w );

	// setup bitmap buttons
	button_new_group->SetBitmapLabel( wxBitmap(
		IconProvider::getBitmap( BITMAP_NEW ).ConvertToImage().Scale( imagesize, imagesize )));
	button_new_source->SetBitmapLabel( wxBitmap(
		IconProvider::getBitmap( BITMAP_NEW ).ConvertToImage().Scale( imagesize, imagesize )));

	//button_new_source->SetBitmapLabel( IconProvider::getBitmap( BITMAP_NEW ));
	button_up->SetBitmapLabel( IconProvider::getBitmap( BITMAP_UP ));
	button_down->SetBitmapLabel( IconProvider::getBitmap( BITMAP_DOWN ));
	button_new_yoga->SetBitmapLabel( IconProvider::getBitmap( BITMAP_NEW ));
	button_delete_yoga->SetBitmapLabel( IconProvider::getBitmap( BITMAP_CLOSE ));
	button_copy_yoga->SetBitmapLabel( IconProvider::getBitmap( BITMAP_COPY ));

    // begin wxGlade: YogaEditorPanel::set_properties
    list_yogas->SetMinSize(wxSize(200, 300));
    button_up->SetMinSize(wxSize(32, 32));
    button_up->SetToolTip(_("Move current Yoga up"));
    button_down->SetMinSize(wxSize(32, 32));
    button_down->SetToolTip(_("Move current Yoga down"));
    button_new_yoga->SetMinSize(wxSize(32, 32));
    button_new_yoga->SetToolTip(_("Insert new Yoga at current position"));
    button_delete_yoga->SetMinSize(wxSize(32, 32));
    button_delete_yoga->SetToolTip(_("Delete current Yoga"));
    button_copy_yoga->SetMinSize(wxSize(32, 32));
    button_copy_yoga->SetToolTip(_("Copy current Yoga"));
    label_description->SetMinSize(wxSize(100, -1));
    text_description->SetMinSize(wxSize(300, -1));
    label_group->SetMinSize(wxSize(100, -1));
    choice_group->SetMinSize(wxSize(150, -1));
    button_new_group->SetMinSize(wxSize(24, 24));
    button_new_group->SetToolTip(_("Create a new group"));
    label_source->SetMinSize(wxSize(100, -1));
    choice_source->SetMinSize(wxSize(150, -1));
    button_new_source->SetMinSize(wxSize(24, 24));
    button_new_source->SetToolTip(_("Create a new source"));
    check_higher_vargas->SetToolTip(_("Check this to include the Yoga only in Rasi chart"));
    text_effect->SetMinSize(wxSize(300, 80));
    text_rule->SetMinSize(wxSize(300, 80));
    drag_chart->SetMinSize(wxSize(340, 100));
    button_parse_rule->SetToolTip(_("Parse the current rule for syntactical correctness"));
    button_evaluate->SetToolTip(_("Evaluate the rule in the chart"));
    button_clear_chart->SetToolTip(_("Remove all objects from the chart"));
    // end wxGlade
}


/*****************************************************
**
**   YogaEditorPanel   ---   do_layout
**
******************************************************/
void YogaEditorPanel::do_layout()
{
    // begin wxGlade: YogaEditorPanel::do_layout
    wxBoxSizer* sizer_main = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* sizer_right = new wxStaticBoxSizer(sizer_right_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_right = new wxFlexGridSizer(6, 1, 0, 0);
    wxStaticBoxSizer* sizer_dragchart = new wxStaticBoxSizer(sizer_dragchart_staticbox, wxHORIZONTAL);
    wxBoxSizer* sizer_evaluate = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* sizer_rule = new wxStaticBoxSizer(sizer_rule_staticbox, wxVERTICAL);
    wxStaticBoxSizer* sizer_effect = new wxStaticBoxSizer(sizer_effect_staticbox, wxVERTICAL);
    wxGridSizer* grid_sizer_source_group = new wxGridSizer(2, 3, 0, 0);
    wxBoxSizer* sizer_description = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* sizer_yogalist = new wxStaticBoxSizer(sizer_yogalist_staticbox, wxHORIZONTAL);
    wxBoxSizer* sizer_yoga_buttons = new wxBoxSizer(wxVERTICAL);
    sizer_yogalist->Add(list_yogas, 1, wxALL|wxEXPAND, 3);
    sizer_yoga_buttons->Add(button_up, 0, wxALL, 3);
    sizer_yoga_buttons->Add(button_down, 0, wxALL, 3);
    sizer_yoga_buttons->Add(button_new_yoga, 0, wxALL, 3);
    sizer_yoga_buttons->Add(button_delete_yoga, 0, wxALL, 3);
    sizer_yoga_buttons->Add(button_copy_yoga, 0, wxALL, 3);
    sizer_yogalist->Add(sizer_yoga_buttons, 0, wxALIGN_CENTER_VERTICAL, 0);
    sizer_main->Add(sizer_yogalist, 0, wxEXPAND, 0);
    sizer_description->Add(label_description, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_description->Add(text_description, 1, wxALL|wxEXPAND, 3);
    grid_right->Add(sizer_description, 0, wxALL|wxEXPAND, 3);
    grid_sizer_source_group->Add(label_group, 1, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_source_group->Add(choice_group, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_source_group->Add(button_new_group, 0, wxALL, 3);
    grid_sizer_source_group->Add(label_source, 1, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_source_group->Add(choice_source, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_source_group->Add(button_new_source, 0, wxALL, 3);
    grid_right->Add(grid_sizer_source_group, 0, wxALL, 3);
    grid_right->Add(check_higher_vargas, 0, wxALL|wxEXPAND, 3);
    sizer_effect->Add(text_effect, 1, wxALL|wxEXPAND, 3);
    grid_right->Add(sizer_effect, 0, wxALL|wxEXPAND, 3);
    sizer_rule->Add(text_rule, 1, wxALL|wxEXPAND, 3);
    grid_right->Add(sizer_rule, 0, wxALL|wxEXPAND, 3);
    sizer_dragchart->Add(drag_chart, 2, wxALL|wxEXPAND, 3);
    sizer_evaluate->Add(button_parse_rule, 0, wxALL, 3);
    sizer_evaluate->Add(button_evaluate, 0, wxALL, 3);
    sizer_evaluate->Add(button_clear_chart, 0, wxALL, 3);
    sizer_dragchart->Add(sizer_evaluate, 0, wxALL, 3);
    grid_right->Add(sizer_dragchart, 2, wxALL|wxEXPAND, 3);
    grid_right->AddGrowableRow(3);
    grid_right->AddGrowableRow(4);
    grid_right->AddGrowableRow(5);
    grid_right->AddGrowableCol(0);
    sizer_right->Add(grid_right, 1, wxALL|wxEXPAND, 3);
    sizer_main->Add(sizer_right, 1, wxALL|wxEXPAND, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    // end wxGlade
}

