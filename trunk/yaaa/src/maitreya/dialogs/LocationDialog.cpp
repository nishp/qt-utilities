/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/LocationDialog.cpp
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
	#pragma implementation "LocationDialog.h"
#endif

#include "LocationDialog.h"

#include <math.h>
 
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/choicdlg.h>
#include <wx/choicdlg.h>
#include <wx/log.h>
#include <wx/progdlg.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/textfile.h>
#include <wx/tokenzr.h>

#include "LocationEditDialog.h"
#include "DialogElements.h"
#include "guibase.h"
#include "IconProvider.h"
#include "FontConfig.h"

#include "constants.h"
#include "Conf.h"
#include "Lang.h"
#include "mathbase.h"
#include "FileConfig.h"

extern Config *config;

#define MAX_GRID_ROWS 100


enum { LD_APPLYFILTER = wxID_HIGHEST + 1, LD_FILTER,  LD_RESETFILTER, LD_COUNTRY, LD_STATE, LD_GRID,
	LD_ADD, LD_DELETE, LD_EDIT, LD_SAVE, LD_ADDCOUNTRY, LD_DELCOUNTRY };

IMPLEMENT_CLASS( LocationDialog, wxDialog )

/*****************************************************
**
**   LocationDialog   ---   Constructor 
**
******************************************************/
LocationDialog::LocationDialog(wxWindow* parent, const int &callerId )
  :  wxDialog(parent, -1, wxT( "Location Database" ),  wxDefaultPosition, wxDefaultSize, DEFAULT_DIALOG_STYLE )
{
		progressdialog = new wxProgressDialog( _("Location Dialog"), _( "Loading Data ..." ));

		calledFromDialog = ( callerId != 0 );

    // begin wxGlade: LocationDialog::LocationDialog
    sizer_actions_staticbox = new wxStaticBox(this, -1, _("Actions"));
    sizer_country_edit_staticbox = new wxStaticBox(this, -1, _("Country"));
    sizer_datadetails_staticbox = new wxStaticBox(this, -1, _("Data"));
    sizer_entry_staticbox = new wxStaticBox(this, -1, _("Entry"));
    sizer_filter_staticbox = new wxStaticBox(this, -1, _("Filter"));
    label_filtermatch = new wxStaticText(this, wxID_ANY, _("Match"));
    filter_edit = new wxTextCtrl(this, LD_FILTER, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB);
    label_filtercountry = new wxStaticText(this, wxID_ANY, _("Country"));
    const wxString country_choice_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    country_choice = new wxChoice(this, LD_COUNTRY, wxDefaultPosition, wxDefaultSize, 4, country_choice_choices, 0);
    applyfilterbutton = new wxButton(this, LD_APPLYFILTER, _("Apply Filter"));
    resetbutton = new wxButton(this, LD_RESETFILTER, _("Reset Filter"));
    button_addcountry = new wxButton(this, LD_ADDCOUNTRY, _("Add to List"));
    button_delcountry = new wxButton(this, LD_DELCOUNTRY, _("Delete From List"));
    locgrid = new MyGrid(this, LD_GRID);
    match_label = new wxStaticText(this, wxID_ANY, _("0 Entries match"));
    static_line_match = new wxStaticLine(this, wxID_ANY);
    label_showname = new wxStaticText(this, wxID_ANY, _("Name"));
    label_name = new wxStaticText(this, wxID_ANY, _("name"));
    label_showcountry = new wxStaticText(this, wxID_ANY, _("Country"));
    label_country = new wxStaticText(this, wxID_ANY, _("country"));
    label_showstate = new wxStaticText(this, wxID_ANY, _("State"));
    label_state = new wxStaticText(this, wxID_ANY, _("state"));
    label_showlong = new wxStaticText(this, wxID_ANY, _("Longitude"));
    label_lon = new wxStaticText(this, wxID_ANY, _("longitude"));
    label_showlat = new wxStaticText(this, wxID_ANY, _("Latitude"));
    label_lat = new wxStaticText(this, wxID_ANY, _("latitude"));
    addbutton = new wxButton(this, LD_ADD, _("New"));
    button_mod = new wxButton(this, LD_EDIT, _("Edit"));
    delbutton = new wxButton(this, LD_DELETE, _("Delete"));
    static_line_button = new wxStaticLine(this, wxID_ANY);
    button_accept = new wxButton(this, wxID_OK, _("Accept"));
    button_close = new wxButton(this, wxID_CANCEL, _("Close"));
    button_save = new wxButton(this, LD_SAVE, _("Save DB"));

    set_properties();
    do_layout();
    // end wxGlade

		filename = FileConfig::get()->getLocationFile();
		SetIcon( IconProvider::getIcon( BITMAP_WORLD ));
		moveWindow2Center( this );
		setDirty( false );
		selectedEntry = 0;
		doSetup();
		updateLocationData();
		delete progressdialog;
		filter_edit->SetFocus();

	Connect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( LocationDialog::OnCellDClick ));
	Connect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( LocationDialog::OnSelectLabel ));
	Connect( wxEVT_GRID_SELECT_CELL, wxGridEventHandler( LocationDialog::OnCellChange ));
	Connect( LD_FILTER, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( LocationDialog::OnFilterChanged ));

	Connect( LD_APPLYFILTER, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationDialog::OnApplyFilter ));
	Connect( LD_RESETFILTER, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationDialog::OnResetFilter ));
	Connect( LD_ADD, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationDialog::OnAddEntry ));
	Connect( LD_DELETE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationDialog::OnDeleteEntry ));
	Connect( LD_EDIT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationDialog::OnEditEntry ));
	Connect( LD_SAVE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationDialog::OnSaveDB ));
	Connect( LD_ADDCOUNTRY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationDialog::OnAddCountry ));
	Connect( LD_DELCOUNTRY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationDialog::OnDelCountry ));
	Connect( wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationDialog::OnOK ));
	Connect( wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LocationDialog::OnCancel ));
	Connect( LD_COUNTRY, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( LocationDialog::OnCountryChoice ));
}

/**************************************************************
***
**   LocationDialog   ---   Destructor
***
***************************************************************/
LocationDialog::~LocationDialog()
{
	list<LocationEntry*>::iterator iter;
	LocationEntry* delentry;
	for( iter = entries.begin(); iter != entries.end(); iter++ )
	{
		delentry = (*iter);
		delete delentry;
	}
	entries.clear();
	countries.clear();
}

/**************************************************************
***
**
***
***************************************************************/
struct string_sort
{
  bool operator()( const wxString &s1, const wxString &s2 ) const
	  {
			return( wxStricmp( (const wxChar*)s1, (const wxChar*)s2 ) < 0 );
		}
};

/**************************************************************
***
**   Sort 
***
***************************************************************/
struct LocationEntry_sort
{
  bool operator()( const LocationEntry *s1, const LocationEntry *s2 ) const
	{
		return( s1->name.CmpNoCase( s2->name ) < 0 );
	}
};

/*****************************************************
**
**   LocationDialog   ---   doSetup 
**
******************************************************/
bool LocationDialog::doSetup()
{
	wxString buf, tmp, name, country, state;
	double longitude, latitude, timeZone;
	int count = 0;
	bool found;
	int nbrecords = 18000;
	wxString fc, s;
	wxTextFile theFile;
	list<LocationEntry*>::iterator iter;
	list<wxString>::iterator citer;


#ifndef __WXMSW__
	locgrid->SetDefaultCellBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BACKGROUND ));
	locgrid->SetDefaultCellTextColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ));
	locgrid->SetLabelBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ));
	locgrid->SetLabelTextColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNTEXT ));
#endif

	selectedEntry = 0;
	if ( ! wxFile::Exists( filename ))
	{ 
		wxLogError( wxT( "Cannot find file %s" ), (const wxChar*)filename );
		return false;
	}

	if ( ! theFile.Open( filename, wxConvUTF8 ))
	{ 
		wxLogInfo( wxT( "Cannot open file %s in UTF-8 mode" ), (const wxChar*)filename );
		if ( ! theFile.Open( filename, wxConvISO8859_1 ))
		{ 
			wxLogError( wxT( "Cannot open file %s in ISO mode" ), (const wxChar*)filename );
			return false;
		}
	}
	entries.clear();
	countries.clear();
	for ( buf = theFile.GetFirstLine(); ! theFile.Eof(); buf = theFile.GetNextLine() )
  {
		wxStringTokenizer t( buf, wxT( ";" ));
		if ( ! t.HasMoreTokens() ) continue;
		name = t.GetNextToken();
		if ( ! t.HasMoreTokens() ) continue;
		country = t.GetNextToken();
		if ( ! t.HasMoreTokens() ) continue;
		state = t.GetNextToken();

		if ( ! t.HasMoreTokens() ) continue;
		longitude = myatof( t.GetNextToken() );

		if ( ! t.HasMoreTokens() ) continue;
		latitude = myatof( t.GetNextToken() );

		if ( ! t.HasMoreTokens() ) continue;
		timeZone = myatof( t.GetNextToken() );

		if ( longitude == 0 || latitude == 0 || name.Len() == 0 ) continue;
		entries.push_back( new LocationEntry( name, country, state, longitude, latitude, timeZone ));
		count++;
		if ( ! ( count % 100 )) progressdialog->Update( Min((int)(count*100/nbrecords), 100 ));
		assert( count < 50000 );
  }
	wxLogMessage( wxT( "%d location entries found" ), entries.size());
  theFile.Close();

	// poor performace under Windows: leave it out
	//entries.sort( LocationEntry_sort() );

	// Generate country list
	for( iter = entries.begin(); iter != entries.end(); iter++ )
	{
		found = false;
		for( citer = countries.begin(); citer != countries.end(); citer++ )
		{
			if ( (*iter)->country == (*citer) )
			{
				found = true;
				break;
			}
		}
		if ( ! found ) countries.push_back( (*iter)->country );
	}
	countries.sort( string_sort() );
	wxLogMessage( wxT( "%d country entries found" ), countries.size());

	//const wxChar *dummy_countries[] = { _( "USA" ), _( "Germany" ), _( "Switzerland" ), _( "Austria" ), _( "India" ) };
	fav_countries.clear();
  fc = config->favouriteCountries;
	if ( fc.IsEmpty() ) fc = wxT( "USA;Germany;Switzerland;Austria;India" );
	wxStringTokenizer t( fc, wxT( ";" ));
	while( t.HasMoreTokens() )
	{
		//s = _( (const wxChar*)t.GetNextToken() );
		fav_countries.push_back( t.GetNextToken() );
		//fav_countries.push_back( s );
	}
	fav_countries.sort( string_sort() );
	updateCountryChoice();

	updateGrid();
	updateLocationData();
	return true;
}


/*****************************************************
**
**   LocationDialog   ---   updateCountryChoice 
**
******************************************************/
void LocationDialog::updateCountryChoice()
{
	list<wxString>::iterator citer;

	country_choice->Clear();
	country_choice->Append( _( "All" ) );
	for( citer = fav_countries.begin(); citer != fav_countries.end(); citer++ )
	{
		country_choice->Append( (*citer) );
	}
	country_choice->SetSelection( 0 );
}

/*****************************************************
**
**   LocationDialog   ---   updateGrid 
**
******************************************************/
void LocationDialog::updateGrid()
{
	wxString s;
	Formatter *formatter = Formatter::get();
	int row = 0;
	wxString locfilter = filter_edit->GetValue();
	int locfilter_len = locfilter.Len();
	list<LocationEntry*>::iterator iter;

	selectedEntry = 0;
	int sel_country = country_choice->GetSelection();
	wxString selected_country( wxT( "" ));
	if ( sel_country )
	{
		selected_country = country_choice->GetString( sel_country );
	}

	locgrid->ClearGrid();
	for( iter = entries.begin(); iter != entries.end(); iter++ )
	{
		if ( locfilter_len )
		{
			if ( wxStrnicmp( (const wxChar*)locfilter, (const wxChar*)((*iter)->name ), locfilter_len ))
			{
				(*iter)->row = -1;
				continue;
			}
		}
		if ( sel_country )
		{
			if ( wxStrcmp( (const wxChar*)selected_country, (const wxChar*)((*iter)->country) ))
			{
				(*iter)->row = -1;
				continue;
			}
		}
		(*iter)->row = row;
		if ( row < MAX_GRID_ROWS )
		{
			locgrid->SetCellValue( row, 0, (*iter)->name );
			locgrid->SetCellValue( row, 1, (*iter)->country );
			locgrid->SetCellValue( row, 2, (*iter)->state );

			s.Printf ( wxT( "%s %c" ), (const wxChar*)formatter->getDegreesFormatted( fabs( (*iter)->longitude ) ),
				( (*iter)->longitude >= 0 ? 'E' : 'W' ));
			locgrid->SetCellValue( row, 3, s );

			s.Printf ( wxT( "%s %c" ), (const wxChar*)formatter->getDegreesFormatted( fabs( (*iter)->latitude ) ),
				( (*iter)->latitude >= 0 ? 'N' : 'S' ));
			locgrid->SetCellValue( row, 4, s );

			s.Printf( wxT( "%02.1f" ), (*iter)->timeZone );
			locgrid->SetCellValue( row, 5, s );
		}

		row++;
	}
	s.Printf( _( "%d Entries match" ), row );
	match_label->SetLabel( s );
	locgrid->AutoSizeRows();
	locgrid->Scroll( 0, 0 );
	locgrid->ForceRefresh();
}

/*****************************************************
**
**   LocationDialog   ---   updateLocationData 
**
******************************************************/
void LocationDialog::updateLocationData()
{
	wxString s;
	Formatter *formatter = Formatter::get();
	if ( selectedEntry )
	{
		label_name->SetLabel( selectedEntry->name );
		label_country->SetLabel( selectedEntry->country );
		label_state->SetLabel( selectedEntry->state );

		s.Printf( wxT( "%s %c" ), (const wxChar*)formatter->getDegreesFormatted( fabs( selectedEntry->longitude ) ),
			( selectedEntry->longitude > 0 ? 'E' : 'W' ));
		label_lon->SetLabel( s );

		s.Printf( wxT( "%s %c" ), (const wxChar*)formatter->getDegreesFormatted( fabs( selectedEntry->latitude ) ),
			( selectedEntry->latitude > 0 ? 'N' : 'S' ));
		label_lat->SetLabel( s );
	}
	else
	{
		label_name->SetLabel( wxT( "" ));
		label_country->SetLabel( wxT( "" ));
		label_state->SetLabel( wxT( "" ));
		label_lon->SetLabel( wxT( "" ));
		label_lat->SetLabel( wxT( "" ));
	}
	button_mod->Enable( selectedEntry );
	delbutton->Enable( selectedEntry );
	button_accept->Enable( selectedEntry && calledFromDialog );
}

/*****************************************************
**
**   LocationDialog   ---   OnSelectCell 
**
******************************************************/
void LocationDialog::OnSelectCell( wxGridEvent& ev )
{
	list<LocationEntry*>::iterator iter;
	selectedEntry = 0;
	if ( ! locgrid->GetNumberRows() ) return;
	for( iter = entries.begin(); iter != entries.end(); iter++ )
	{
		if ( (*iter)->row == ev.GetRow() )
		{
			selectedEntry = (LocationEntry*)(*iter);
			break;
		}
	}
	updateLocationData();
	ev.Skip();
}

/*****************************************************
**
**   LocationDialog   ---   OnSelectLabel
**
******************************************************/
void LocationDialog::OnSelectLabel( wxGridEvent& event )
{
	list<LocationEntry*>::iterator iter;
	selectedEntry = 0;
	if ( ! locgrid->GetNumberRows() ) return;
	if ( event.GetCol() == -1 )
	{
		for( iter = entries.begin(); iter != entries.end(); iter++ )
		{
			if ( (*iter)->row == event.GetRow() )
			{
				selectedEntry = (LocationEntry*)(*iter);
				break;
			}
	}
	}
	locgrid->ClearSelection();
	updateLocationData();
	event.Skip();
}

/*****************************************************
**
**   LocationDialog   ---   OnCellChange
**
******************************************************/
void LocationDialog::OnCellChange( wxGridEvent& event )
{
	list<LocationEntry*>::iterator iter;
	selectedEntry = 0;
	if ( ! locgrid->GetNumberRows() ) return;
	for( iter = entries.begin(); iter != entries.end(); iter++ )
	{
		if ( (*iter)->row == event.GetRow() )
		{
			selectedEntry = (LocationEntry*)(*iter);
			break;
		}
	}
	updateLocationData();
	event.Skip();
}

/*****************************************************
**
**   LocationDialog   ---   OnCellDClick
**
******************************************************/
void LocationDialog::OnCellDClick( wxGridEvent& event )
{
	if ( selectedEntry )
	{
		if ( calledFromDialog ) EndModal( wxID_OK );
		else editEntry();
	}
}

/*****************************************************
**
**   LocationDialog   ---   OnFilterChanged 
**
******************************************************/
void LocationDialog::OnFilterChanged( wxCommandEvent &event )
{
	selectedEntry = 0;
	updateLocationData();
	updateGrid();
}

/*****************************************************
**
**   LocationDialog   ---   OnApplyFilter 
**
******************************************************/
void LocationDialog::OnApplyFilter( wxCommandEvent &event )
{
	selectedEntry = 0;
	updateLocationData();
	updateGrid();
}

/*****************************************************
**
**   LocationDialog   ---   OnResetFilter 
**
******************************************************/
void LocationDialog::OnResetFilter( wxCommandEvent &event )
{
	selectedEntry = 0;
	updateLocationData();
	filter_edit->SetValue( wxT( "" ));
	updateGrid();
}

/*****************************************************
**
**   LocationDialog   ---   OnCountryChoice 
**
******************************************************/
void LocationDialog::OnCountryChoice( wxCommandEvent &event )
{
	selectedEntry = 0;
	updateLocationData();
	updateGrid();
}

/*****************************************************
**
**   LocationDialog   ---   OnAddEntry 
**
******************************************************/
void LocationDialog::OnAddEntry( wxCommandEvent &event )
{
	LocationEditDialog dialog( this, -1, _( "New Location" ));
	dialog.setEntry( 0 );
	if ( dialog.ShowModal() == wxID_OK )
	{
		entries.push_back( new LocationEntry( dialog.getEntry()->name, dialog.getEntry()->country,
			dialog.getEntry()->state, dialog.getEntry()->longitude,
			dialog.getEntry()->latitude, dialog.getEntry()->timeZone )
		);
		selectedEntry = 0;
		entries.sort( LocationEntry_sort() );
		setDirty( true );
		updateLocationData();
		updateGrid();
	}
}

/*****************************************************
**
**   LocationDialog   ---   FindEntryByRow
**
******************************************************/
/*
list<LocationEntry>::iterator LocationDialog::FindEntryByRow( const int &row )
{
}
*/

/*****************************************************
**
**   LocationDialog   ---   OnDeleteEntry 
**
******************************************************/
void LocationDialog::OnDeleteEntry( wxCommandEvent &event )
{
	wxString s;
	list<LocationEntry*>::iterator iter;
	Formatter *formatter = Formatter::get();
	assert( selectedEntry );
	LocationEntry* delentry;

	s.Printf( wxT( "%s\n%s\n%s\n%s ?" ), _( "Do you want to delete the item" ),
		(const wxChar*)selectedEntry->name, 
		(const wxChar*)formatter->getLongitudeFormatted( selectedEntry->longitude ),
		(const wxChar*)formatter->getLatitudeFormatted( selectedEntry->latitude ));
	if ( doMessageBox( this, s, wxYES | wxNO | wxCENTRE | wxICON_QUESTION ) == wxID_YES )
	{
		for( iter = entries.begin(); iter != entries.end(); iter++ )
		{
			if ( (*iter)->row == selectedEntry->row )
			{
				delentry = (*iter);
				entries.erase( iter );
				delete delentry;
				//delete selectedEntry;
				selectedEntry = 0;
				setDirty( true );
				updateLocationData();
				updateGrid();
				return;
			}
		}
	}
}

/*****************************************************
**
**   LocationDialog   ---   OnEditEntry 
**
******************************************************/
void LocationDialog::OnEditEntry( wxCommandEvent &event )
{
	if( selectedEntry ) editEntry();
}

/*****************************************************
**
**   LocationDialog   ---   editEntry 
**
******************************************************/
bool LocationDialog::editEntry()
{
	assert( selectedEntry );
	LocationEditDialog dialog( this, -1, wxT( "Edit Location" ));
	dialog.setEntry( selectedEntry );
	if ( dialog.ShowModal() == wxID_OK )
	{
		*selectedEntry = *(dialog.getEntry());
		selectedEntry = 0;
		setDirty( true );
		entries.sort( LocationEntry_sort() );
		updateLocationData();
		updateGrid();
		return true;
	}
	return false;
}

/*****************************************************
**
**   LocationDialog   ---   OnSaveDB 
**
******************************************************/
void LocationDialog::OnSaveDB( wxCommandEvent &event )
{
	if ( saveDB() )
	{
		setDirty( false );
	}
}

/*****************************************************
**
**   LocationDialog   ---   setDirty 
**
******************************************************/
void LocationDialog::setDirty( const bool& d )
{
	dirty = d;
	button_save->Enable( dirty );
}

/*****************************************************
**
**   LocationDialog   ---   saveDB 
**
******************************************************/
bool LocationDialog::saveDB()
{
	wxString s;
	list<LocationEntry*>::iterator iter;

	FileConfig::get()->backupFile( filename );

  wxTextFile wfile( filename );
  if ( ! wfile.Exists() )
  {
    if ( ! wfile.Create( filename ) )
    {
      wxLogError( wxT( "Cannot create file %s" ), (const wxChar*)filename );
      return false;
    }
  }
  else
  {
    if ( ! wfile.Open( FILE_CONF_MODE ) )
    {
      wxLogError( wxT( "Cannot open file %s" ), (const wxChar*)filename );
      return false;
    }
    wfile.Clear();
  }
	int kk = 0;
	for( iter = entries.begin(); iter != entries.end(); iter++ )
	{
	
		s.Printf( wxT( "%s;%s;%s;%f;%f;%f" ), (const wxChar*)((*iter)->name), (const wxChar*)((*iter)->country),
			(const wxChar*)((*iter)->state), (*iter)->longitude, (*iter)->latitude, (*iter)->timeZone  );
		wfile.AddLine( s );
		kk++;
	}
	wfile.Write( wxTextFileType_None, FILE_CONF_MODE );
	wfile.Close();
	return true;
}

/*****************************************************
**
**   LocationDialog   ---   OnOK 
**
******************************************************/
void LocationDialog::OnOK( wxCommandEvent &event )
{ 
	saveFavouriteContries();
	if ( dirty ) { if ( ! saveDB()) return; }
	EndModal( wxID_OK );
}

/*****************************************************
**
**   LocationDialog   ---   OnCancel 
**
******************************************************/
void LocationDialog::OnCancel( wxCommandEvent &event )
{
	saveFavouriteContries();
	if ( dirty )
	{
		if ( doMessageBox( this, _( "The database has changed. Do you want to save it ?" ),
			wxYES | wxNO | wxCENTRE | wxICON_QUESTION ) == wxID_YES )
		{
			if ( ! saveDB()) return;
		}
	}
	EndModal( wxID_CANCEL );
}


/*****************************************************
**
**   LocationDialog   ---   saveFavouriteContries 
**
******************************************************/
void LocationDialog::saveFavouriteContries()
{
	wxString s = wxT( "" );
	list<wxString>::iterator citer;

	config->favouriteCountries = wxT( "" );
	for( citer = fav_countries.begin(); citer != fav_countries.end(); citer++ )
	{
		if ( config->favouriteCountries != wxT( "" )) config->favouriteCountries.Append( wxT( ";" ));
		config->favouriteCountries.Append( (*citer) );
	}
}

/**************************************************************
***
**
***
***************************************************************/
Location *LocationDialog::getSelectedLocation()
{
	if ( selectedEntry == 0 ) return 0;
	selectedLocation.setLocation( selectedEntry->name, selectedEntry->longitude, selectedEntry->latitude, selectedEntry->timeZone, 0 );
	return &selectedLocation;
}

/*****************************************************
**
**   LocationDialog   ---   OnAddCountry 
**
******************************************************/
void LocationDialog::OnAddCountry( wxCommandEvent &event )
{
	wxArrayString s;
	list<wxString>::iterator citer;
	list<wxString>::iterator fiter;
	int i = 0;
	bool found;

	for( citer = countries.begin(); citer != countries.end(); citer++ )
	{
		found = false;
		for( fiter = fav_countries.begin(); fiter != fav_countries.end(); fiter++ )
		{
			if ( (*citer) == (*fiter) )
			{
				found = true;
				break;
			}
		}
		if ( found == false )
		{
			//s[i] = (*citer);
			s.Add(*citer);
			i++;
		}
		if ( i >= 199 ) break;
	}
	wxSingleChoiceDialog dialog( this, _( "Select the country you want to add to the list." ), _( "Add Country" ), s );
	if ( dialog.ShowModal() == wxID_OK )
	{
		i = dialog.GetSelection();
		fav_countries.push_back( s[i] );
		fav_countries.sort( string_sort() );
		updateCountryChoice();
	}
}

/*****************************************************
**
**   LocationDialog   ---   OnDelCountry 
**
******************************************************/
void LocationDialog::OnDelCountry( wxCommandEvent &event )
{
	wxString s[200];
	int i = 0;
	list<wxString>::iterator citer;

	for( citer = fav_countries.begin(); citer != fav_countries.end(); citer++ )
	{
		s[i] = (*citer);
		i++;
		if ( i >= 199 ) break;
	}
	wxSingleChoiceDialog dialog( this, _( "Select the country you want to delete." ), _( "Delete Country" ), i, s );
	if ( dialog.ShowModal() == wxID_OK )
	{
		i = dialog.GetSelection();
		//for( j = 0; j < fav_countries.GetCount(); j++ )
		for( citer = fav_countries.begin(); citer != fav_countries.end(); citer++ )
		{
			if (( *citer) == s[i] )
			{
				fav_countries.erase( citer );
				updateCountryChoice();
				break;
			}
		}
	}
}

/*****************************************************
**
**   LocationDialog   ---   set_properties 
**
******************************************************/
void LocationDialog::set_properties()
{
    // begin wxGlade: LocationDialog::set_properties
    SetTitle(_("Location Database"));
    filter_edit->SetMinSize(wxSize(200, -1));
    country_choice->SetMinSize(wxSize(200, -1));
    country_choice->SetSelection(0);
    locgrid->SetMinSize(wxSize(640,300));
    match_label->SetMinSize(wxSize(200, 16));
    static_line_match->SetMinSize(wxSize(640, 2));
    label_showname->SetMinSize(wxSize(100, 16));
    label_name->SetMinSize(wxSize(150, 16));
    label_showcountry->SetMinSize(wxSize(100, 16));
    label_country->SetMinSize(wxSize(150, 16));
    label_showstate->SetMinSize(wxSize(100, 16));
    label_state->SetMinSize(wxSize(150, 16));
    label_showlong->SetMinSize(wxSize(100, 16));
    label_lon->SetMinSize(wxSize(150, 16));
    label_showlat->SetMinSize(wxSize(100, 16));
    label_lat->SetMinSize(wxSize(150, 16));
    // end wxGlade
	FontConfig *f = FontConfig::get();
	locgrid->CreateGrid(100, 6);
	locgrid->SetRowLabelSize(30);
	locgrid->SetColLabelSize(26);
	locgrid->EnableEditing(false);
	locgrid->SetColLabelValue(0, _("Name"));
	locgrid->SetColSize(0, 140);
	locgrid->SetColLabelValue(1, _("Country"));
	locgrid->SetColSize(1, 120);
	locgrid->SetColLabelValue(2, _("State"));
	locgrid->SetColSize(2, 120);
	locgrid->SetColLabelValue(3, _("Longitude"));
	locgrid->SetColSize(3, 80);
	locgrid->SetColLabelValue(4, _("Latitude"));
	locgrid->SetColSize(4, 80);
	locgrid->SetColLabelValue(5, _("TZ"));

	locgrid->SetColSize(5, 40);
	locgrid->SetMinSize(wxSize(400, 250));
	locgrid->SetDefaultCellFont( *f->getDefaultFont() );
	locgrid->SetLabelFont( *f->getHeaderFont() );
	//locgrid->SetExtraStyle( wxWS_EX_BLOCK_EVENTS );
}


/*****************************************************
**
**   LocationDialog   ---   do_layout 
**
******************************************************/
void LocationDialog::do_layout()
{
    // begin wxGlade: LocationDialog::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(7, 1, 0, 0);
    wxFlexGridSizer* sizer_button = new wxFlexGridSizer(1, 5, 0, 0);
    wxFlexGridSizer* sizer_data = new wxFlexGridSizer(1, 2, 0, 0);
    wxStaticBoxSizer* sizer_entry = new wxStaticBoxSizer(sizer_entry_staticbox, wxVERTICAL);
    wxStaticBoxSizer* sizer_datadetails = new wxStaticBoxSizer(sizer_datadetails_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_sizer_2 = new wxFlexGridSizer(5, 2, 0, 0);
    wxBoxSizer* sizer_entrymatch = new wxBoxSizer(wxHORIZONTAL);
    wxFlexGridSizer* sizer_top = new wxFlexGridSizer(1, 3, 0, 0);
    wxStaticBoxSizer* sizer_country_edit = new wxStaticBoxSizer(sizer_country_edit_staticbox, wxVERTICAL);
    wxStaticBoxSizer* sizer_actions = new wxStaticBoxSizer(sizer_actions_staticbox, wxVERTICAL);
    wxStaticBoxSizer* sizer_filter = new wxStaticBoxSizer(sizer_filter_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_filter = new wxFlexGridSizer(2, 2, 3, 3);
    grid_filter->Add(label_filtermatch, 0, wxALIGN_CENTER_VERTICAL, 0);
    grid_filter->Add(filter_edit, 0, wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);
    grid_filter->Add(label_filtercountry, 0, wxALIGN_CENTER_VERTICAL, 0);
    grid_filter->Add(country_choice, 0, 0, 0);
    sizer_filter->Add(grid_filter, 1, wxEXPAND, 0);
    sizer_top->Add(sizer_filter, 1, wxALL|wxEXPAND, 5);
    sizer_actions->Add(applyfilterbutton, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 3);
    sizer_actions->Add(resetbutton, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 3);
    sizer_top->Add(sizer_actions, 1, wxALL|wxEXPAND, 3);
    sizer_country_edit->Add(button_addcountry, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 3);
    sizer_country_edit->Add(button_delcountry, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 3);
    sizer_top->Add(sizer_country_edit, 1, wxALL|wxEXPAND, 3);
    sizer_top->AddGrowableCol(0);
    sizer_main->Add(sizer_top, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 6);
    sizer_main->Add(locgrid, 1, wxALL|wxEXPAND, 3);
    sizer_entrymatch->Add(match_label, 0, wxLEFT, 60);
    sizer_main->Add(sizer_entrymatch, 1, wxEXPAND, 5);
    sizer_main->Add(static_line_match, 0, wxTOP|wxBOTTOM, 5);
    grid_sizer_2->Add(label_showname, 0, wxALL, 3);
    grid_sizer_2->Add(label_name, 0, wxALL, 3);
    grid_sizer_2->Add(label_showcountry, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_2->Add(label_country, 0, wxALL, 3);
    grid_sizer_2->Add(label_showstate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_2->Add(label_state, 0, wxALL, 3);
    grid_sizer_2->Add(label_showlong, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_2->Add(label_lon, 0, wxALL, 3);
    grid_sizer_2->Add(label_showlat, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_2->Add(label_lat, 0, wxALL, 3);
    sizer_datadetails->Add(grid_sizer_2, 1, wxEXPAND, 0);
    sizer_data->Add(sizer_datadetails, 1, wxALL|wxEXPAND, 3);
    sizer_entry->Add(addbutton, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    sizer_entry->Add(button_mod, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    sizer_entry->Add(delbutton, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    sizer_data->Add(sizer_entry, 1, wxALL|wxEXPAND, 3);
    sizer_main->Add(sizer_data, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    sizer_main->Add(static_line_button, 0, wxALL|wxEXPAND, 5);
    sizer_button->Add(button_accept, 0, wxALL, 3);
    sizer_button->Add(button_close, 0, wxALL, 3);
    sizer_button->Add(button_save, 0, wxALL, 3);
    sizer_main->Add(sizer_button, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    sizer_main->AddGrowableRow(1);
    sizer_main->AddGrowableCol(0);
    Layout();
    // end wxGlade
}

