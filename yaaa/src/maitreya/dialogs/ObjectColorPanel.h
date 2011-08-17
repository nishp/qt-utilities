/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/ObjectColorPanel.h
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

#ifndef OBJECTCOLORPANEL_H
#define OBJECTCOLORPANEL_H

#ifdef __GNUG__
	#pragma interface "ObjectColorPanel.h"
#endif

#include "ConfigPanel.h"

#include "constants.h"

class wxButton;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;

/*************************************************//**
*
* \brief ConfigPanel for object color parameters
* 
******************************************************/
class ObjectColorPanel: public ConfigPanel
{
	DECLARE_CLASS( ObjectColorPanel )

public:

  ObjectColorPanel( wxWindow* parent );
	virtual void setData();   
	virtual bool saveData();   

private:
	void do_layout();

	wxButton *button_signs[12], *button_houses[12], *button_objects[MAX_EPHEM_OBJECTS], *button_default;
	void OnButton( wxCommandEvent& );
	void setDefaultColors();

protected:
	wxStaticBox* sizer_housecolors_staticbox;
	wxStaticBox* sizer_signcolors_staticbox;
	wxStaticBox* sizer_objectcolors_staticbox;
}; 


#endif // OBJECTCOLORPANEL_H
