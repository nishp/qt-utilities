/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/ConfigDialog.h
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

#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#ifdef __GNUG__
	#pragma interface "ConfigDialog.h"
#endif

#include <wx/dialog.h>

class ConfigPanel;
class wxButton;
class wxPanel;
class wxStaticLine;

class wxTreebook;

BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EVENT_TYPE( CONFIG_CHANGED, wxID_HIGHEST + 1300 )
	DECLARE_EVENT_TYPE( CONFIG_TOOLBAR_CHANGED, wxID_HIGHEST + 1301 )
END_DECLARE_EVENT_TYPES()

#define NB_PANELS 21

/*************************************************//**
*
* \brief main class for config dialog
* 
******************************************************/
class ConfigDialog: public wxDialog
{
	DECLARE_CLASS( ConfigDialog )

public:

	ConfigDialog( wxWindow* parent );
	~ConfigDialog();

protected:

	void OnApply( wxCommandEvent& );
	void OnOK( wxCommandEvent& );
	void OnCancel( wxCommandEvent& );
	void OnSize( wxSizeEvent& );
	void saveData();
	void showPanel( const int &selected );

	ConfigPanel *configpanel[NB_PANELS];
	wxPanel *panel[NB_PANELS];
	wxStaticLine* static_line_bottom;
	wxButton *okbutton, *applybutton, *cancelbutton;

	wxTreebook* notebook;

#ifdef _WX29
	void OnTreebook( class wxBookCtrlEvent& );
#else
	void OnTreebook( class wxTreebookEvent& );
#endif
};

#endif // CONFIGDIALOG_H

