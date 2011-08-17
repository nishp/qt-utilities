/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/ConfigPanel.h
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

#ifndef CONFIGPANEL_H
#define CONFIGPANEL_H

#ifdef __GNUG__
	#pragma interface "ConfigPanel.h"
#endif

#include <wx/panel.h>

/*************************************************//**
*
* \brief Base class for all panels in ConfigurationDialog
* 
******************************************************/
class ConfigPanel: public wxPanel
{
	DECLARE_CLASS( ConfigPanel )

public:

	ConfigPanel( wxWindow* parentC );

	/**
	 * \brief Abstact method for reading data from config and setup of UI items. Must be overwritten.
	 */
	virtual void setData() = 0;

	/**
	 * \brief Abstact method for reading UI item states and saving data to config. Must be overwritten.
	 *
	 * May return false on error.
	 */
	virtual bool saveData() = 0;

	/**
	 * \brief Called on panel activation. May be overwritten.
	 */
	virtual void onActivate() {}

	/**
	 * \brief Called on panel deactivation. May be overwritten.
	 */
	virtual void onPassivate() {}

	bool isDirty() { return dirty; }

protected:
	void OnSize( wxSizeEvent& );

	void OnChoice( wxCommandEvent& );
	bool dirty;
};

#endif // CONFIGPANEL_H
