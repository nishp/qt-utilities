/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/AnimationPanel.h
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

#ifndef ANIMATIONPANEL_H
#define ANIMATIONPANEL_H

#ifdef __GNUG__
	#pragma interface "AnimationPanel.h"
#endif

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class wxButton;
class wxCheckBox;
class wxChoice;
class wxSpinCtrl;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;

/*************************************************//**
*
* \brief ConfigPanel for animation parameters
* 
******************************************************/
class AnimationPanel: public ConfigPanel
{
	DECLARE_CLASS( AnimationPanel )

public:
    // begin wxGlade: AnimationPanel::ids
    // end wxGlade

	AnimationPanel( wxWindow* parent );
  virtual void setData();
  virtual bool saveData();

private:
    // begin wxGlade: AnimationPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

		void OnSwitchViewerMode( wxCommandEvent& );
	void OnAnimationModeChoice( wxCommandEvent& );

protected:
    // begin wxGlade: AnimationPanel::attributes
    wxStaticBox* sizer_animate_staticbox;
    wxStaticText* label_animation_freq;
    wxSpinCtrl* spin_animation_freq;
    wxStaticText* label_animation_freq2;
    wxStaticText* label_animation_mode;
    wxChoice* choice_animation_mode;
    wxStaticText* label_animation_step;
    wxSpinCtrl* spin_animation_steplength;
    wxChoice* choice_animate_dimension;
    // end wxGlade
}; // wxGlade: end class


#endif // ANIMATIONPANEL_H
