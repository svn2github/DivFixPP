///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 17 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "DivFix++Gui.h"

#include "../resources/wxbitbtn_path.xpm"

///////////////////////////////////////////////////////////////////////////

DivFixpp_Gui::DivFixpp_Gui( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer_main;
	bSizer_main = new wxBoxSizer( wxVERTICAL );
	
	wxGridSizer* gSizer_checkboxes;
	gSizer_checkboxes = new wxGridSizer( 2, 2, 0, 0 );
	
	wxchk_relativeoutputfile = new wxCheckBox( this, wxID_ANY, _("Relative Output Folder and File"), wxDefaultPosition, wxDefaultSize, 0 );
	wxchk_relativeoutputfile->SetValue(true);
	
	wxchk_relativeoutputfile->SetToolTip( _("This option creates output file near original file for each file on list.") );
	
	gSizer_checkboxes->Add( wxchk_relativeoutputfile, 0, wxLEFT|wxTOP, 5 );
	
	bSizer_main->Add( gSizer_checkboxes, 0, wxEXPAND, 0 );
	
	wxBoxSizer* bSizer_save;
	bSizer_save = new wxBoxSizer( wxHORIZONTAL );
	
	wxbitbtn_savepath = new wxBitmapButton( this, ID_SAVE, wxBitmap( wxbitbtn_path_xpm ), wxDefaultPosition, wxSize( 32,32 ), 0 );
	wxbitbtn_savepath->Enable( false );
	wxbitbtn_savepath->SetToolTip( _("Output file folder") );
	
	wxbitbtn_savepath->Enable( false );
	wxbitbtn_savepath->SetToolTip( _("Output file folder") );
	
	bSizer_save->Add( wxbitbtn_savepath, 0, wxALIGN_CENTER|wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 2 );
	
	textCtrl_savepath = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	textCtrl_savepath->Enable( false );
	textCtrl_savepath->SetToolTip( _("Output file folder") );
	
	bSizer_save->Add( textCtrl_savepath, 1, wxALIGN_CENTER|wxALL, 5 );
	
	wxbtn_about = new wxButton( this, ID_ABOUT, _("About"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer_save->Add( wxbtn_about, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	bSizer_main->Add( bSizer_save, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer_titles;
	bSizer_titles = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, _("Source Files"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer_titles->Add( m_staticText1, 1, wxLEFT, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, _("Log Window"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer_titles->Add( m_staticText2, 1, 0, 5 );
	
	bSizer_main->Add( bSizer_titles, 0, wxEXPAND, 5 );
	
	wxBoxSizer* ErrorHandler;
	ErrorHandler = new wxBoxSizer( wxHORIZONTAL );
	
	FileListBox = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_HSCROLL|wxLB_MULTIPLE|wxLB_NEEDED_SB ); 
	ErrorHandler->Add( FileListBox, 1, wxEXPAND|wxLEFT, 5 );
	
	TextCtrl_log = new wxTextCtrl( this, wxID_ANY, _("DivFix++ AVI Fix and Previewer utility.\nCopyright (C) 2006 Erdem U. Altinyurt\n\nDivFix++ comes with \nABSOLUTELY NO WARRANTY;\nfor details, press the \"About\" button.\n\nThis is free software and you are welcome\n to redistribute it under GPL."), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	ErrorHandler->Add( TextCtrl_log, 1, wxEXPAND|wxRIGHT, 5 );
	
	bSizer_main->Add( ErrorHandler, 1, wxEXPAND, 5 );
	
	m_gauge = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxSize( -1,35 ), wxGA_HORIZONTAL );
	bSizer_main->Add( m_gauge, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer_bottom;
	bSizer_bottom = new wxBoxSizer( wxHORIZONTAL );
	
	wxGridSizer* gSizer_buttons;
	gSizer_buttons = new wxGridSizer( 2, 4, 0, 0 );
	
	wxbtn_add = new wxButton( this, ID_ADD, _("Add Files"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer_buttons->Add( wxbtn_add, 1, wxALL|wxEXPAND, 1 );
	
	wxbtn_remove = new wxButton( this, ID_REMOVE, _("Remove Files"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer_buttons->Add( wxbtn_remove, 1, wxALL|wxEXPAND, 1 );
	
	wxbtn_clear = new wxButton( this, ID_CLEAR, _("Clear List"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer_buttons->Add( wxbtn_clear, 1, wxALL|wxEXPAND, 1 );
	
	wxbtn_preferences = new wxButton( this, ID_ABOUT, _("Preferences"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer_buttons->Add( wxbtn_preferences, 1, wxALL|wxEXPAND, 1 );
	
	wxbtn_fix = new wxButton( this, ID_REBUID, _("Fix"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer_buttons->Add( wxbtn_fix, 1, wxALL|wxEXPAND, 1 );
	
	wxbtn_strip = new wxButton( this, ID_STRIP, _("Strip Index"), wxDefaultPosition, wxDefaultSize, 0 );
	wxbtn_strip->SetToolTip( _("This button removes index part from file, makes your video file unseekable in most of video players.") );
	
	gSizer_buttons->Add( wxbtn_strip, 1, wxALL|wxEXPAND, 1 );
	
	wxbtn_checkerrors = new wxButton( this, ID_CHECKERRORS, _("Check Errors"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer_buttons->Add( wxbtn_checkerrors, 1, wxALL|wxEXPAND, 1 );
	
	wxbtn_stop = new wxButton( this, ID_STOP, _("Stop"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer_buttons->Add( wxbtn_stop, 1, wxALL|wxEXPAND, 1 );
	
	bSizer_bottom->Add( gSizer_buttons, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer_checkboxes;
	bSizer_checkboxes = new wxBoxSizer( wxVERTICAL );
	
	wxchk_cutout = new wxCheckBox( this, wxID_ANY, _("Cut Out Bad Parts"), wxDefaultPosition, wxDefaultSize, 0 );
	wxchk_cutout->SetValue(true);
	
	wxchk_cutout->SetToolTip( _("This option removes corrupt frames from file instead leaving inside of video.") );
	
	bSizer_checkboxes->Add( wxchk_cutout, 0, wxBOTTOM|wxTOP, 2 );
	
	wxchk_keeporiginal = new wxCheckBox( this, wxID_ANY, _("Keep Original File"), wxDefaultPosition, wxDefaultSize, 0 );
	wxchk_keeporiginal->SetValue(true);
	
	wxchk_keeporiginal->SetToolTip( _("This option overwrites the orginal file. Not recomended for fixing downloading files, it corrupt your download.") );
	
	bSizer_checkboxes->Add( wxchk_keeporiginal, 0, wxBOTTOM|wxTOP, 1 );
	
	wxchk_keyframe = new wxCheckBox( this, wxID_ANY, _("Recover From Key Frames"), wxDefaultPosition, wxDefaultSize, 0 );
	
	wxchk_keyframe->SetToolTip( _("This option removes potentially corrupted frames those after broken section until the next key-frame. It's good for previewing but not recommended for repair operation.") );
	
	bSizer_checkboxes->Add( wxchk_keyframe, 0, wxTOP, 2 );
	
	bSizer_bottom->Add( bSizer_checkboxes, 0, wxEXPAND|wxLEFT, 5 );
	
	bSizer_main->Add( bSizer_bottom, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	this->SetSizer( bSizer_main );
	this->Layout();
	bSizer_main->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DivFixpp_Gui::DivFixppClose ) );
	wxchk_relativeoutputfile->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DivFixpp_Gui::OnCheck_RelativeOutputFile ), NULL, this );
	wxbitbtn_savepath->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DivFixpp_Gui::OnPathClick ), NULL, this );
	wxbtn_about->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DivFixpp_Gui::OnAboutClick ), NULL, this );
	wxbtn_add->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DivFixpp_Gui::OnAddClick ), NULL, this );
	wxbtn_remove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DivFixpp_Gui::OnRemoveClick ), NULL, this );
	wxbtn_clear->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DivFixpp_Gui::OnClearClick ), NULL, this );
	wxbtn_preferences->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DivFixpp_Gui::OnPreferencesClick ), NULL, this );
	wxbtn_fix->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DivFixpp_Gui::OnFixClick ), NULL, this );
	wxbtn_strip->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DivFixpp_Gui::OnStripClick ), NULL, this );
	wxbtn_checkerrors->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DivFixpp_Gui::OnCheckClick ), NULL, this );
	wxbtn_stop->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DivFixpp_Gui::OnStopClick ), NULL, this );
	wxchk_keeporiginal->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DivFixpp_Gui::OnCheck_KeepOriginal ), NULL, this );
}

DivFixpp_Gui::~DivFixpp_Gui()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DivFixpp_Gui::DivFixppClose ) );
	wxchk_relativeoutputfile->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DivFixpp_Gui::OnCheck_RelativeOutputFile ), NULL, this );
	wxbitbtn_savepath->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DivFixpp_Gui::OnPathClick ), NULL, this );
	wxbtn_about->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DivFixpp_Gui::OnAboutClick ), NULL, this );
	wxbtn_add->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DivFixpp_Gui::OnAddClick ), NULL, this );
	wxbtn_remove->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DivFixpp_Gui::OnRemoveClick ), NULL, this );
	wxbtn_clear->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DivFixpp_Gui::OnClearClick ), NULL, this );
	wxbtn_preferences->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DivFixpp_Gui::OnPreferencesClick ), NULL, this );
	wxbtn_fix->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DivFixpp_Gui::OnFixClick ), NULL, this );
	wxbtn_strip->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DivFixpp_Gui::OnStripClick ), NULL, this );
	wxbtn_checkerrors->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DivFixpp_Gui::OnCheckClick ), NULL, this );
	wxbtn_stop->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DivFixpp_Gui::OnStopClick ), NULL, this );
	wxchk_keeporiginal->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DivFixpp_Gui::OnCheck_KeepOriginal ), NULL, this );
}

PreferencesDialog_Gui::PreferencesDialog_Gui( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 250,110 ), wxDefaultSize );
	
	wxBoxSizer* bSizer_top;
	bSizer_top = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer_language;
	sbSizer_language = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Language") ), wxVERTICAL );
	
	wxArrayString wxchc_languageChoices;
	wxchc_language = new wxChoice( this, ID_LANG, wxDefaultPosition, wxDefaultSize, wxchc_languageChoices, 0 );
	wxchc_language->SetSelection( 0 );
	sbSizer_language->Add( wxchc_language, 1, wxALIGN_BOTTOM|wxEXPAND, 3 );
	
	bSizer_top->Add( sbSizer_language, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer_mediaplayer;
	sbSizer_mediaplayer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Default Media Player Path") ), wxHORIZONTAL );
	
	wxbitbtn_player = new wxBitmapButton( this, wxID_ANY, wxBitmap( wxbitbtn_path_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	sbSizer_mediaplayer->Add( wxbitbtn_player, 0, wxALL, 5 );
	
	textCtrl_playerpath = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer_mediaplayer->Add( textCtrl_playerpath, 1, wxALL, 5 );
	
	bSizer_top->Add( sbSizer_mediaplayer, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer_update;
	sbSizer_update = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Updates") ), wxHORIZONTAL );
	
	wxchk_update = new wxCheckBox( this, wxID_ANY, _("Check for New Versions"), wxDefaultPosition, wxDefaultSize, 0 );
	wxchk_update->SetValue(true);
	
	sbSizer_update->Add( wxchk_update, 1, wxALL, 5 );
	
	bSizer_top->Add( sbSizer_update, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer_top );
	this->Layout();
	bSizer_top->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( PreferencesDialog_Gui::OnClose ) );
	wxbitbtn_player->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PreferencesDialog_Gui::OnPlayerClick ), NULL, this );
	wxchk_update->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PreferencesDialog_Gui::OnCheckNewVersion ), NULL, this );
}

PreferencesDialog_Gui::~PreferencesDialog_Gui()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( PreferencesDialog_Gui::OnClose ) );
	wxbitbtn_player->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PreferencesDialog_Gui::OnPlayerClick ), NULL, this );
	wxchk_update->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PreferencesDialog_Gui::OnCheckNewVersion ), NULL, this );
}

UpdateDialog_Gui::UpdateDialog_Gui( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* MainBoxSizer;
	MainBoxSizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxbtmp_icon = new wxStaticBitmap( this, wxID_OPEN, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	MainBoxSizer->Add( wxbtmp_icon, 0, wxALIGN_CENTER|wxALL, 10 );
	
	wxBoxSizer* VerticalBoxSizer;
	VerticalBoxSizer = new wxBoxSizer( wxVERTICAL );
	
	version_text = new wxStaticText( this, wxID_ANY, _("New DivFix++ version %s is available!"), wxDefaultPosition, wxDefaultSize, 0 );
	version_text->Wrap( -1 );
	VerticalBoxSizer->Add( version_text, 0, wxALIGN_CENTER|wxALL, 10 );
	
	m_hyperlink = new wxHyperlinkCtrl( this, wxID_ANY, _("Download new DivFix++"), wxT("http://divfixpp.sourceforge.net"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	VerticalBoxSizer->Add( m_hyperlink, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxchk_display = new wxCheckBox( this, wxID_ANY, _("Do not show this again."), wxDefaultPosition, wxDefaultSize, 0 );
	
	VerticalBoxSizer->Add( wxchk_display, 0, wxALIGN_CENTER|wxALL, 10 );
	
	MainBoxSizer->Add( VerticalBoxSizer, 1, wxEXPAND, 5 );
	
	this->SetSizer( MainBoxSizer );
	this->Layout();
	MainBoxSizer->Fit( this );
	
	// Connect Events
	wxchk_display->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( UpdateDialog_Gui::OnChkDisplay ), NULL, this );
}

UpdateDialog_Gui::~UpdateDialog_Gui()
{
	// Disconnect Events
	wxchk_display->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( UpdateDialog_Gui::OnChkDisplay ), NULL, this );
}
