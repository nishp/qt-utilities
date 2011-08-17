/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/ToolbarPanel.cpp
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
	#pragma implementation "ToolbarPanel.h"
#endif

#include "ToolbarPanel.h"

#include "Conf.h"
#include "IconProvider.h"
#include "ToolPanel.h"

#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/imaglist.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>

extern Config *config;
enum { CD_DEFAULT = wxID_HIGHEST + 1, CD_LEFT, CD_RIGHT, CD_UP, CD_DOWN, CD_LIST1, CD_LIST2 };

IMPLEMENT_CLASS( ToolbarPanel, ConfigPanel )

/*****************************************************
**
**   ToolbarPanel   ---   Constructor 
**
******************************************************/
ToolbarPanel::ToolbarPanel( wxWindow* parent )
 : ConfigPanel( parent )
{
    // begin wxGlade: ToolbarPanel::ToolbarPanel
    sizer_selected_staticbox = new wxStaticBox(this, -1, _("Items in Toolbar"));
    sizer_available_staticbox = new wxStaticBox(this, -1, _("Unused Items"));
    list1 = new wxListCtrl(this, CD_LIST1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_NO_HEADER|wxLC_SINGLE_SEL|wxSUNKEN_BORDER);
    button_right = new wxBitmapButton(this, CD_RIGHT, wxNullBitmap);
    button_left = new wxBitmapButton(this, CD_LEFT, wxNullBitmap);
    button_up = new wxBitmapButton(this, CD_UP, wxNullBitmap);
    button_down = new wxBitmapButton(this, CD_DOWN, wxNullBitmap);
    list2 = new wxListCtrl(this, CD_LIST2, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_NO_HEADER|wxLC_SINGLE_SEL|wxSUNKEN_BORDER);
    button_default = new wxButton(this, CD_DEFAULT, _("Restore Defaults"));

    set_properties();
    do_layout();
    // end wxGlade

  Connect( CD_LEFT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ToolbarPanel::OnLeft ));
  Connect( CD_RIGHT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ToolbarPanel::OnRight ));
  Connect( CD_UP, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ToolbarPanel::OnUp ));
  Connect( CD_DOWN, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ToolbarPanel::OnDown ));
  Connect( CD_DEFAULT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ToolbarPanel::OnDefault ));

  Connect( CD_LIST1, wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( ToolbarPanel::OnRightSelected ));
  Connect( CD_LIST1, wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( ToolbarPanel::OnRightDeselected ));
  Connect( CD_LIST1, wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( ToolbarPanel::OnRightActivated ));

  Connect( CD_LIST2, wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( ToolbarPanel::OnLeftSelected ));
  Connect( CD_LIST2, wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( ToolbarPanel::OnLeftDeselected ));
  Connect( CD_LIST2, wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( ToolbarPanel::OnLeftActivated ));
}

/*****************************************************
**
**   ToolbarPanel   ---   Destructor 
**
******************************************************/
ToolbarPanel::~ToolbarPanel()
{
	delete imageList;
}

/*****************************************************
**
**   ToolbarPanel   ---   setData 
**
******************************************************/
void ToolbarPanel::setData()
{
	unsigned int i;
	//long l;
	ToolbarFactory *factory = ToolbarFactory::get();

	// Setup image list from full list of commands, so all is included
	const vector<ToolBarCommand> &fullcmdlist = factory->getFullCommandList();
	imageList = new wxImageList( 24, 24, true );
	imageList->Add( IconProvider::getBitmap( BITMAP_SEPARATOR ));

	for( i = 0; i < fullcmdlist.size(); i++ )
		if ( fullcmdlist[i].commandId != 0 ) imageList->Add( IconProvider::getBitmap( fullcmdlist[i].bitmapId ));
 	list1->SetImageList( imageList, wxIMAGE_LIST_SMALL );
	list2->SetImageList( imageList, wxIMAGE_LIST_SMALL );
	initListControls( true );
}

/*****************************************************
**
**   ToolbarPanel   ---   initListControls
**
******************************************************/
void ToolbarPanel::initListControls( const bool &useConfig )
{
	unsigned int i;
	long l;
	ToolbarFactory *factory = ToolbarFactory::get();

	const vector<ToolBarCommand> unusedCommandList = factory->getUnusedCommandList( useConfig );
	list1->DeleteAllItems();

	// Fixed Separator
	wxListItem item;
	item.SetId( 0 );
	item.SetImage( 0 );
	item.SetText( _( "Separator" ));
	list1->InsertItem( item );
	for( i = 0; i < unusedCommandList.size(); i++ )
	{
		wxListItem item;
		item.SetId( i+1 );
		item.SetData( unusedCommandList[i].commandId );
		item.SetImage( unusedCommandList[i].imageListId );
		item.SetText( unusedCommandList[i].description );
		l = list1->InsertItem( item );
	}
	list1->SetItemState( 0, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	list1->Arrange();

	list2->DeleteAllItems();
	// List of commands in current toolbar
	const vector<ToolBarCommand> cmdlist = factory->getUsedCommandList( useConfig );
	for( i = 0; i < cmdlist.size(); i++ )
	{
		wxListItem item;
		item.SetId( i );
		item.SetData( cmdlist[i].commandId );
		item.SetImage( cmdlist[i].imageListId );
		item.SetText( cmdlist[i].description );
		l = list2->InsertItem( item );
	}
	if ( cmdlist.size() > 0 ) list2->SetItemState( 0, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	list2->Arrange();
	dirty = false;
	left_sel = right_sel = 0;
}

/*****************************************************
**
**   ToolbarPanel   ---   saveData 
**
******************************************************/
bool ToolbarPanel::saveData()
{
	if ( dirty )
	{
		config->mainToolbarItems.Clear();
		for( long i = 0; i < list2->GetItemCount(); i++ )
		{
			if ( i > 0 ) config->mainToolbarItems << wxT( ";" );
			config->mainToolbarItems << list2->GetItemData( i );
		}
		dirty = false;
		return true;
	}
	return false;
}

/*****************************************************
**
**   ToolbarPanel   ---   OnLeft 
**
******************************************************/
void ToolbarPanel::OnLeft( wxCommandEvent &event )
{
	ToolBarCommand cmd;
	ToolbarFactory *factory = ToolbarFactory::get();
	if ( right_sel > -1 )
	{
		factory->getItemById( list2->GetItemData( right_sel ), cmd );
		list2->DeleteItem( right_sel );
		list2->SetItemState( right_sel, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
		list2->EnsureVisible( right_sel );
		if ( cmd.commandId > 0 )
		{
			wxListItem item;
			item.SetId( left_sel );
			item.SetData( cmd.commandId );
			item.SetImage( cmd.imageListId );
			item.SetText( cmd.description );
			list1->InsertItem( item );
			list1->SetItemState( left_sel, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
			list1->EnsureVisible( left_sel );
		}
		dirty = true;
	}
}

/*****************************************************
**
**   ToolbarPanel   ---   OnRight 
**
******************************************************/
void ToolbarPanel::OnRight( wxCommandEvent &event )
{
	ToolBarCommand cmd;
	ToolbarFactory *factory = ToolbarFactory::get();
	if ( right_sel > -1 )
	{
		factory->getItemById( list1->GetItemData( left_sel ), cmd );
		if ( cmd.commandId > 0 )
		{
			list1->DeleteItem( left_sel );
			list1->SetItemState( left_sel - 1, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
		}
		wxListItem item;
		item.SetId( right_sel + 1 );
		item.SetData( cmd.commandId );
		item.SetImage( cmd.imageListId );
		item.SetText( cmd.description );
		list2->InsertItem( item );
		list2->SetItemState( right_sel + 1, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
		list2->EnsureVisible( right_sel + 1 );
		dirty = true;
	}
}

/*****************************************************
**
**   ToolbarPanel   ---   OnUp
**
******************************************************/
void ToolbarPanel::OnUp( wxCommandEvent &event )
{
	ToolBarCommand cmd;
	ToolbarFactory *factory = ToolbarFactory::get();
	if ( right_sel > 0 )
	{
		factory->getItemById( list2->GetItemData( right_sel ), cmd );
	
		// DeleteItem sets the parameter right_sel under Windows
		int sel = right_sel;
		list2->DeleteItem( sel );
		wxListItem item;
		item.SetId( sel - 1 );
		item.SetData( cmd.commandId );
		item.SetImage( cmd.imageListId );
		item.SetText( cmd.description );
		list2->InsertItem( item );
		list2->SetItemState( sel - 1, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
		list2->EnsureVisible( sel - 1 );
		dirty = true;
	}
}

/*****************************************************
**
**   ToolbarPanel   ---   OnDown 
**
******************************************************/
void ToolbarPanel::OnDown( wxCommandEvent &event )
{
	ToolBarCommand cmd;
	ToolbarFactory *factory = ToolbarFactory::get();
	long l = list2->GetItemCount();
	if ( right_sel < l - 1 )
	{
		factory->getItemById( list2->GetItemData( right_sel ), cmd );
		int sel = right_sel;
		list2->DeleteItem( sel );
		wxListItem item;
		item.SetId( sel + 1 );
		item.SetData( cmd.commandId );
		item.SetImage( cmd.imageListId );
		item.SetText( cmd.description );
		list2->InsertItem( item );
		list2->SetItemState( sel + 1, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
		list2->EnsureVisible( sel + 1 );
		dirty = true;
	}
}

/*****************************************************
**
**   ToolbarPanel   ---   OnDefault 
**
******************************************************/
void ToolbarPanel::OnDefault( wxCommandEvent &event )
{
	initListControls( false );
	dirty = true;
}

/*****************************************************
**
**   ToolbarPanel   ---   OnRightSelected 
**
******************************************************/
void ToolbarPanel::OnRightSelected( wxListEvent& event )
{
	if ( ! button_right->IsEnabled() ) button_right->Enable( true );
	left_sel = event.GetIndex();
}

/*****************************************************
**
**   ToolbarPanel   ---   OnRightDeselected 
**
******************************************************/
void ToolbarPanel::OnRightDeselected( wxListEvent& event )
{
	if ( button_right->IsEnabled() ) button_right->Enable( false );
	left_sel = -1;
}

/*****************************************************
**
**   ToolbarPanel   ---   OnRightActivated 
**
******************************************************/
void ToolbarPanel::OnRightActivated( wxListEvent& event )
{
}

/*****************************************************
**
**   ToolbarPanel   ---   OnLeftSelected
**
******************************************************/
void ToolbarPanel::OnLeftSelected( wxListEvent& event )
{
	right_sel = event.GetIndex();
	if ( ! button_left->IsEnabled() ) button_left->Enable( true );
	if ( ! button_up->IsEnabled() ) button_up->Enable( right_sel > 0 );
	if ( ! button_down->IsEnabled() ) button_down->Enable( right_sel < list2->GetItemCount() - 1 );
}

/*****************************************************
**
**   ToolbarPanel   ---   OnLeftDeselected
**
******************************************************/
void ToolbarPanel::OnLeftDeselected( wxListEvent& event )
{
	if ( button_left->IsEnabled() ) button_left->Enable( false );
	if ( button_up->IsEnabled() ) button_up->Enable( false );
	if ( button_down->IsEnabled() ) button_down->Enable( false );
	right_sel = -1;
}

/*****************************************************
**
**   ToolbarPanel   ---   OnLeftActivated 
**
******************************************************/
void ToolbarPanel::OnLeftActivated( wxListEvent& event )
{
}

/*****************************************************
**
**   ToolbarPanel   ---   set_properties 
**
******************************************************/
void ToolbarPanel::set_properties()
{
	const int w = 150;
	wxListItem itemCol;
	itemCol.m_mask = wxLIST_MASK_TEXT | wxLIST_MASK_IMAGE;
	itemCol.m_text = _( "Type" );
	list1->InsertColumn(0, itemCol);
	list1->SetColumnWidth( 0, w );

	list2->InsertColumn(0, itemCol);
	list2->SetColumnWidth( 0, w );

	button_right->SetBitmapLabel( IconProvider::getBitmap( BITMAP_RIGHT ));
	button_left->SetBitmapLabel( IconProvider::getBitmap( BITMAP_LEFT ));
	button_up->SetBitmapLabel( IconProvider::getBitmap( BITMAP_UP ));
	button_down->SetBitmapLabel( IconProvider::getBitmap( BITMAP_DOWN ));

    // begin wxGlade: ToolbarPanel::set_properties
    list1->SetMinSize(wxSize(200,250));
    button_right->SetMinSize(wxSize(32,32));
    button_right->SetToolTip(_("Move selected item to toolbar"));
    button_left->SetMinSize(wxSize(32,32));
    button_left->SetToolTip(_("Remove selected item from toolbar"));
    button_up->SetMinSize(wxSize(32,32));
    button_up->SetToolTip(_("Move selected item up"));
    button_down->SetMinSize(wxSize(32,32));
    button_down->SetToolTip(_("Move selected item down"));
    list2->SetMinSize(wxSize(200,250));
    button_default->SetToolTip(_("Restore default settings"));
    // end wxGlade
}

/*****************************************************
**
**   ToolbarPanel   ---   do_layout 
**
******************************************************/
void ToolbarPanel::do_layout()
{
    // begin wxGlade: ToolbarPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(2, 1, 3, 3);
    wxFlexGridSizer* sizer_listbox = new wxFlexGridSizer(1, 3, 3, 3);
    wxStaticBoxSizer* sizer_selected = new wxStaticBoxSizer(sizer_selected_staticbox, wxVERTICAL);
    wxFlexGridSizer* sizer_selected2 = new wxFlexGridSizer(2, 1, 0, 0);
    wxFlexGridSizer* sizer_buttons = new wxFlexGridSizer(4, 1, 3, 3);
    wxStaticBoxSizer* sizer_available = new wxStaticBoxSizer(sizer_available_staticbox, wxVERTICAL);
    wxFlexGridSizer* sizer_available2 = new wxFlexGridSizer(1, 1, 0, 0);
    sizer_available2->Add(list1, 1, wxALL|wxEXPAND, 3);
    sizer_available2->AddGrowableRow(0);
    sizer_available2->AddGrowableCol(0);
    sizer_available->Add(sizer_available2, 1, wxALL|wxEXPAND, 3);
    sizer_listbox->Add(sizer_available, 1, wxALL|wxEXPAND, 3);
    sizer_buttons->Add(button_right, 0, wxALL|wxEXPAND, 3);
    sizer_buttons->Add(button_left, 0, wxALL|wxEXPAND, 3);
    sizer_buttons->Add(button_up, 0, wxALL|wxEXPAND, 3);
    sizer_buttons->Add(button_down, 0, wxALL|wxEXPAND, 3);
    sizer_listbox->Add(sizer_buttons, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL|wxSHAPED, 3);
    sizer_selected2->Add(list2, 1, wxALL|wxEXPAND, 3);
    sizer_selected2->Add(button_default, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    sizer_selected2->AddGrowableRow(0);
    sizer_selected2->AddGrowableCol(0);
    sizer_selected->Add(sizer_selected2, 1, wxALL|wxEXPAND, 3);
    sizer_listbox->Add(sizer_selected, 1, wxALL|wxEXPAND, 3);
    sizer_listbox->AddGrowableRow(0);
    sizer_listbox->AddGrowableCol(0);
    sizer_listbox->AddGrowableCol(2);
    sizer_main->Add(sizer_listbox, 1, wxALL|wxEXPAND, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    sizer_main->AddGrowableRow(0);
    sizer_main->AddGrowableCol(0);
    // end wxGlade
}

