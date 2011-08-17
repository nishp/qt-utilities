/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/YogaEditor.cpp
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
	#pragma implementation "YogaEditor.h"
#endif

#include "YogaEditor.h"

#include "ChildWindow.h"
#include "Conf.h"
#include "DragChart.h"
#include "FileConfig.h"
#include "guibase.h"
#include "MenuProvider.h"
#include "Statusbar.h"
#include "VargaHoroscope.h"
#include "Yoga.h"
#include "YogaConfig.h"

#include <wx/filename.h>
#include <wx/filedlg.h>
#include <wx/panel.h>
#include <wx/statusbr.h>
#include <wx/textdlg.h>

#include <vector>

using namespace std;

extern Config *config;

IMPLEMENT_CLASS( YogaEditor, BasicView )

#define STATUS_MESSAGE_TIMEOUT 3000

/*****************************************************
**
**   YogaEditor   ---   Constructor 
**
******************************************************/
YogaEditor::YogaEditor( wxWindow *parent, ChildWindow *frame )
 : BasicView( parent, frame, 0, VIEW_YOGA_EDITOR, true, true )
{
	evaluationChart = new MinimalVargaHoroscope();
	panel = new YogaEditorPanel( this, YE_YOGA_PANEL, evaluationChart );
	widget = panel;

	loader = YogaConfigLoader::get();
	if ( ! config->lastYogaConfigFile.IsEmpty() ) loadFile( config->lastYogaConfigFile );

	yogaexpert = YogaExpert::get();

	statusbar = new MyStatusbar( this );
	const int statwidths[3] = { -1, 200, 100 };
	statusbar->SetFieldsCount( 3, statwidths );

	initToolItems();

	Connect( TBB_NEW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( YogaEditor::OnNewFile ));
	Connect( TBB_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( YogaEditor::OnOpenFile ));
	Connect( TBB_SAVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( YogaEditor::OnSaveFile ));
	Connect( TBB_SAVEAS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( YogaEditor::OnSaveFileAs ));
	Connect( TBB_CLOSE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( YogaEditor::OnClose ));

	// new sources and groups
	Connect( YE_NEW_SOURCE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( YogaEditor::OnNewSource ));
	Connect( YE_NEW_GROUP, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( YogaEditor::OnNewGroup ));

	Connect( wxEVT_IDLE, wxIdleEventHandler( YogaEditor::OnIdle ));

	// rule buttons
	Connect( YE_PARSE_RULE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( YogaEditor::OnParseRule ));
	Connect( YE_CLEAR_CHART, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( YogaEditor::OnClearChart ));
	Connect( YE_EVALUATE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( YogaEditor::OnEvaluateRule ));
	Connect( YE_OBJECTS, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( YogaEditor::OnSelectObjects ));
}

/*****************************************************
**
**   YogaEditor   ---   Destructor 
**
******************************************************/
YogaEditor::~YogaEditor()
{
	config->lastYogaConfigFile = filename;
	delete evaluationChart;
}

/*****************************************************
**
**   YogaEditor   ---   queryClose
**
******************************************************/
bool YogaEditor::queryClose()
{
	if ( panel->isDirty() )
	{
		int ret = doMessageBox( this,
			wxString::Format( _( "Do you want to save the Yoga definition file %s ?" ), (const wxChar*)filename),
			wxYES_NO  | wxCANCEL | wxCENTRE | wxICON_QUESTION );

		switch( ret )
		{
			case wxID_YES:
				return saveFile();
			break;
			case wxID_CANCEL:
				return false;
			break;
			default:
				return true;
			break;
		}
	}
	return true;
}

/*****************************************************
**
**   YogaEditor   ---   loadFile
**
******************************************************/
void YogaEditor::loadFile( wxString fn )
{
	vector<YogaConfig*> yogas;

	panel->setDirty( false );

	if ( loader->readYogaDefinitionFile( fn, yogas ))
	{
		filename = fn;
		echo( wxString::Format( _( "File %s loaded, %d Yogas found" ), (const wxChar*)fn, (int)yogas.size()));
		panel->loadYogas( yogas );
	}
	else
	{
		filename.Clear();
	}
	updateUiItems();
}

/*****************************************************
**
**   YogaEditor   ---   closeFile
**
******************************************************/
bool YogaEditor::closeFile()
{
	if ( ! queryClose()) return false;

	panel->clearGui();
	filename.Clear();
	panel->setDirty( false );
	echo( _( "File closed" ));
	updateUiItems();
	return true;
}

/*****************************************************
**
**   YogaEditor   ---   OnNewFile
**
******************************************************/
void YogaEditor::OnNewFile( wxCommandEvent &event )
{
	closeFile();
	panel->newItem();
	panel->setDirty( false );
	echo( _( "New File created" ));
}

/*****************************************************
**
**   YogaEditor   ---   OnOpenFile
**
******************************************************/
void YogaEditor::OnOpenFile( wxCommandEvent &event )
{
	wxString filetypes = wxT( "Xml (*.xml)|*.xml| All files (*.*)|*.*" );

	if ( ! queryClose() ) return;

	wxFileDialog openFileDialog( this, _( "Open Yoga Definition File"), FileConfig::get()->getYogaDir(),
		wxEmptyString, filetypes, wxFD_OPEN, wxDefaultPosition );

	if (openFileDialog.ShowModal() == wxID_OK )
	{
		loadFile( openFileDialog.GetFilename() );
	}
	updateUiItems();
}

/*****************************************************
**
**   YogaEditor   ---   saveFile
**
******************************************************/
bool YogaEditor::saveFile()
{
	vector<YogaConfig*> yogas;

	if ( filename.IsEmpty() && ! saveFileAs()) return false;

	FileConfig::get()->backupFile( FileConfig::get()->getYogaDir() + filename );

	panel->saveYogas( yogas );
	if ( loader->saveYogaDefinitionFile( filename, yogas ))
	{
		panel->setDirty( false );
		echo( wxString::Format( _( "File %s sucessfully saved" ), (const wxChar*)filename));
		return true;
	}
	echo( wxString::Format( _( "Cannot save file %s" ), (const wxChar*)filename));
	updateUiItems();
	return false;
}

/*****************************************************
**
**   YogaEditor   ---   saveFileAs
**
******************************************************/
bool YogaEditor::saveFileAs()
{
	const wxString filetypes = wxT( "Xml (*.xml)|*.xml| All files (*.*)|*.*" );
	const wxString yogadir = FileConfig::get()->getYogaDir();

	wxFileDialog saveFileDialog( this, _("Save file as" ), yogadir, filename, filetypes, wxFD_SAVE, wxDefaultPosition );
	
	if ( saveFileDialog.ShowModal() == wxID_OK ) return saveFile();
	
	return false;
}

/*****************************************************
**
**   YogaEditor   ---   OnClose
**
******************************************************/
void YogaEditor::OnClose( wxCommandEvent &event )
{
	GetParent()->Close();
}

/*****************************************************
**
**   YogaEditor   ---   parseRule
**
******************************************************/
Rule *YogaEditor::parseRule()
{
	wxString message;
	Rule *rule = 0;
	wxString rulestr =  panel->getRuleText();

	if ( rulestr.IsEmpty())
	{
		message = _( "No rule specified" );
	}
	else
	{
		// YogaExperts writes message to string
		rule = yogaexpert->parseRule( rulestr, message );
	}
	echo( message );
	return rule;
}

/*****************************************************
**
**   YogaEditor   ---   OnEvaluateRule
**
******************************************************/
void YogaEditor::OnEvaluateRule( wxCommandEvent &event )
{
	wxString parseMessage, output;
	Rule *rule = yogaexpert->parseRule( panel->getRuleText(), parseMessage );
	if ( rule )
	{
		parseMessage.clear();
		bool b = yogaexpert->evaluateRule( evaluationChart, rule, parseMessage );
		if ( parseMessage.IsEmpty())
		{
			// evaluated without error
			output = b ? _( "true") : _( "false" );
			panel->echoOnDragChart( output );
		}
		else
		{
			// parse message ist set -> error
			output = parseMessage;
			panel->echoOnDragChart( _( "Error" ));
		}
		echo( output );
		delete rule;
		rule = 0;
	}
	else 
	{
		// empty rule -> error
		if ( parseMessage.IsEmpty() ) echo( _( "General error" ));
		else echo( parseMessage );
		panel->echoOnDragChart( _( "Error" ));
	}
}

/*****************************************************
**
**   YogaEditor   ---   OnSelectObjects
**
******************************************************/
void YogaEditor::OnSelectObjects( wxCommandEvent &event )
{
	doMessageBox( this, wxT( "This feature is not yet implemented" ));
}

/*****************************************************
**
**   YogaEditor   ---   OnClearChart
**
******************************************************/
void YogaEditor::OnClearChart( wxCommandEvent &event )
{
	panel->clearChart();
}

/*****************************************************
**
**   YogaEditor   ---   OnNewSource
**
******************************************************/
void YogaEditor::OnNewSource( wxCommandEvent &event )
{
	wxTextEntryDialog dialog( this, _( "New Source" ), _( "Yoga Editor"), _( "New Source" ));
	if ( dialog.ShowModal()  == wxID_OK )
	{
		panel->addSource( dialog.GetValue() );
	}
}

/*****************************************************
**
**   YogaEditor   ---   OnNewGroup
**
******************************************************/
void YogaEditor::OnNewGroup( wxCommandEvent &event )
{
	wxTextEntryDialog dialog( this, _( "New Group" ), _( "Yoga Editor"), _( "New Group" ));
	if ( dialog.ShowModal()  == wxID_OK )
	{
		panel->addGroup( dialog.GetValue() );
	}
}

/*****************************************************
**
**   YogaEditor   ---   OnIdle
**
******************************************************/
void YogaEditor::OnIdle( wxIdleEvent &event )
{
	updateUiItems();
	frame->setTitle();
}

/*****************************************************
**
**   YogaEditor   ---   updateUiItems
**
******************************************************/
void YogaEditor::updateUiItems()
{
	// status bar messages
	if ( filename.IsEmpty())
	{
		setStatusText( _( "No file opened" ), 1 );
		setStatusText( wxEmptyString, 2 );
	}
	else
	{
		setStatusText( filename, 1 );
		if ( panel->isDirty() ) setStatusText( _( "changed" ), 2 );
		else setStatusText( _( "unchanged" ), 2 );
	}
}

/*****************************************************
**
**   YogaEditor   ---   getWindowLabel
**
******************************************************/
wxString YogaEditor::getWindowLabel( const bool shortname )
{
	wxString s = _( "Yoga Editor" );
	if ( ! shortname && ! filename.IsEmpty()) s << wxT( ": " ) << filename;
	return s;
}

/*****************************************************
**
**   ViewFactory   ---   createYogaEditorView
**
******************************************************/
BasicView *ViewFactory::createYogaEditorView( wxWindow *parent, ChildWindow *frame )
{
	return new YogaEditor( parent, frame );
}

