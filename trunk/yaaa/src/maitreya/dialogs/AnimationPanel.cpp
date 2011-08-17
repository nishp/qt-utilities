/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/AnimationPanel.cpp
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
	#pragma implementation "AnimationPanel.h"
#endif

#include "AnimationPanel.h"

#include "Conf.h"
#include "guibase.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/colordlg.h>
#include <wx/filedlg.h>
#include <wx/font.h>
#include <wx/fontdlg.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

extern Config *config;
enum { CD_ANIMATIONMODE = wxID_HIGHEST + 1 };

IMPLEMENT_CLASS( AnimationPanel, ConfigPanel )

/*****************************************************
**
**   AnimationPanel   ---   Constructor 
**
******************************************************/
AnimationPanel::AnimationPanel( wxWindow* parent )
    : ConfigPanel( parent )
{
    // begin wxGlade: AnimationPanel::AnimationPanel
    sizer_animate_staticbox = new wxStaticBox(this, -1, _("Animation"));
    label_animation_freq = new wxStaticText(this, wxID_ANY, _("Frequency"));
    spin_animation_freq = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS|wxTE_AUTO_URL, 0, 1000);
    label_animation_freq2 = new wxStaticText(this, wxID_ANY, _("sec"));
    label_animation_mode = new wxStaticText(this, wxID_ANY, _("Mode"));
    const wxString choice_animation_mode_choices[] = {
        _("System Time"),
        _("Custom Steps")
    };
    choice_animation_mode = new wxChoice(this, CD_ANIMATIONMODE, wxDefaultPosition, wxDefaultSize, 2, choice_animation_mode_choices, 0);
    label_animation_step = new wxStaticText(this, wxID_ANY, _("Step Length"));
    spin_animation_steplength = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1000);
    const wxString choice_animate_dimension_choices[] = {
        _("seconds"),
        _("minutes"),
        _("hours"),
        _("days"),
        _("months"),
        _("years")
    };
    choice_animate_dimension = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 6, choice_animate_dimension_choices, 0);

    set_properties();
    do_layout();
    // end wxGlade

  Connect( CD_ANIMATIONMODE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AnimationPanel::OnAnimationModeChoice ));
}

/*****************************************************
**
**   AnimationPanel   ---   setData 
**
******************************************************/
void AnimationPanel::setData()
{
	spin_animation_freq->SetValue( config->animateInterval );
  choice_animation_mode->SetSelection( config->animateMode );
  spin_animation_steplength->SetValue( config->animateStepLength );
  spin_animation_steplength->Enable( config->animateMode );
  choice_animate_dimension->SetSelection( config->animateStepDimension );
  choice_animate_dimension->Enable( config->animateMode );
}

/*****************************************************
**
**   AnimationPanel   ---   saveData 
**
******************************************************/
bool AnimationPanel::saveData()
{
  config->animateInterval = spin_animation_freq->GetValue();

  config->animateMode = choice_animation_mode->GetSelection();
  config->animateStepLength = spin_animation_steplength->GetValue();
  config->animateStepDimension = choice_animate_dimension->GetSelection();

	return true;
}

/*****************************************************
**
**   AnimationPanel   ---   OnAnimationModeChoice 
**
******************************************************/
void AnimationPanel::OnAnimationModeChoice( wxCommandEvent &event )
{
  spin_animation_steplength->Enable( choice_animation_mode->GetSelection() > 0 );
  choice_animate_dimension->Enable( choice_animation_mode->GetSelection() > 0 );
}

/*****************************************************
**
**   AnimationPanel   ---   set_properties
**
******************************************************/
void AnimationPanel::set_properties()
{
    // begin wxGlade: AnimationPanel::set_properties
    spin_animation_freq->SetMinSize(wxSize(40,-1));
    choice_animation_mode->SetSelection(0);
    spin_animation_steplength->SetMinSize(wxSize(60,21));
    choice_animate_dimension->SetSelection(0);
    // end wxGlade
}

/*****************************************************
**
**   AnimationPanel   ---   do_layout
**
******************************************************/
void AnimationPanel::do_layout()
{
    // begin wxGlade: AnimationPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 1, 0, 0);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(2, 1, 0, 0);
    wxStaticBoxSizer* sizer_animate = new wxStaticBoxSizer(sizer_animate_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_ani2 = new wxFlexGridSizer(3, 2, 3, 3);
    wxFlexGridSizer* grid_steplen = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* grid_freq = new wxFlexGridSizer(1, 2, 0, 0);
    grid_ani2->Add(label_animation_freq, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_freq->Add(spin_animation_freq, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_freq->Add(label_animation_freq2, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 10);
    grid_freq->AddGrowableCol(0);
    grid_freq->AddGrowableCol(1);
    grid_ani2->Add(grid_freq, 1, wxEXPAND, 0);
    grid_ani2->Add(label_animation_mode, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_ani2->Add(choice_animation_mode, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_ani2->Add(label_animation_step, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_steplen->Add(spin_animation_steplength, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_steplen->Add(choice_animate_dimension, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_steplen->AddGrowableCol(0);
    grid_steplen->AddGrowableCol(1);
    grid_ani2->Add(grid_steplen, 1, wxEXPAND, 0);
    grid_ani2->AddGrowableCol(1);
    sizer_animate->Add(grid_ani2, 1, wxALL, 3);
    sizer_left->Add(sizer_animate, 1, wxALL, 3);
    sizer_left->AddGrowableCol(0);
    sizer_main->Add(sizer_left, 1, wxALL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    // end wxGlade
}

