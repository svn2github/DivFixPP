//-----------------------------------------------------------------------
//
// Name:        DivFix++.cpp
// Author:      Death Knight
// Created:     26.05.2006 02:24:47
// Description: DivFix++ GUI Code
//
//-----------------------------------------------------------------------
/***********************************(GPL)********************************
*	DivFix++ is AVI index rebuilder/fixer for repair your avi! :)       *
*	Copyright (C) 2006  Erdem U. Altinyurt                              *
*                                                                       *
*	This program is free software; you can redistribute it and/or       *
*	modify it under the terms of the GNU General Public License         *
*	as published by the Free Software Foundation; either version 2      *
*	of the License, or any later version.                               *
*                                                                       *
*	This program is distributed in the hope that it will be useful,     *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of      *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       *
*	GNU General Public License for more details.                        *
*                                                                       *
*	You should have received a copy of the GNU General Public License   *
*	along with this program;                                            *
*   if not, write to the Free Software	Foundation, Inc.,               *
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA        *
*                                                                       *
*               home  : divfixpp.sourceforge.net                        *
*               email : death_knight at gamebox.net                     *
*************************************************************************/
#include "DivFix++.h"
#include "DivFix++Gui.h"

DECLARE_APP(DivFixppApp)

DivFixpp::DivFixpp(wxLocale& my_locale, wxWindow *parent, wxWindowID id)
:DivFixpp_Gui( parent, id ), DivFixppCore( WxGauge,TextCtrl_log), m_locale( my_locale )
	{
	const wxString name = wxString::Format(_T("DivFix++-%s"), wxGetUserId().c_str());
	single_inst_checker = new wxSingleInstanceChecker(name);
	state = fix;
	if ( single_inst_checker->IsAnotherRunning() ){
		wxLogError(_("Another program instance is already running, aborting."));
		Destroy();
		}
	else{
		CreateGUIControls();
		FileListBox->SetFocus();	//To remove Log window cursor
		DivFixppCore::WxGauge = m_gauge;
		DivFixppCore::WxMemoLog = TextCtrl_log;
		}
	}

DivFixpp::~DivFixpp(){
	if (single_inst_checker != NULL);{
		delete single_inst_checker;
		single_inst_checker = NULL;
		}
	wxConfigBase *pConfig = wxConfigBase::Get();
	if ( pConfig == NULL )
		return;

	// save the control's values to the config
	pConfig->Write( _T("PathOut"),textCtrl_savepath->GetValue() );
	pConfig->Write( _T("PathLog"),textCtrl_logpath->GetValue() );


	// save the frame position
	int x, y, w, h;
	GetClientSize(&w, &h);
	GetPosition(&x, &y);
	pConfig->Write(_T("x"), (long) x);
	pConfig->Write(_T("y"), (long) y);
	pConfig->Write(_T("w"), (long) w);
	pConfig->Write(_T("h"), (long) h);
	pConfig->Flush();
	}

void DivFixpp::CreateGUIControls(void){
	this->SetTitle(wxString(_T("DivFix++ ")) << _T(_VERSION_) << _T(_OS_) );
	this->Center();
	this->SetIcon(DivFixpp_xpm);
#ifdef __WXMSW__
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
#endif

	SetDropTarget( new DnDFile(FileListBox) );
	FileListBox->SetDropTarget( new DnDFile(FileListBox) );

	wxConfigBase *pConfig = wxConfigBase::Get();

	if ( ! pConfig->Read(_T("PathOut")).IsEmpty() )
		textCtrl_savepath->SetValue( wxConfigBase::Get()->Read(_T("PathOut")) );
	if ( ! pConfig->Read(_T("PathLog")).IsEmpty() )
		textCtrl_logpath->SetValue( wxConfigBase::Get()->Read(_T("PathLog")) );

	// restore frame position and size
	int x = pConfig->Read(_T("x"), 100),
		y = pConfig->Read(_T("y"), 100),
		w = pConfig->Read(_T("w"), 600),
		h = pConfig->Read(_T("h"), 320);
		Move(x, y);
		SetClientSize(w, h);
	}

void DivFixpp::DivFixppClose(wxCloseEvent& event){
    Destroy();
}

void DivFixpp::OnAddClick(wxCommandEvent& event){
	wxFileDialog OpenFile(this, _("Choose a file"), _T(""), _T(""), _T("*"), wxOPEN | wxFILE_MUST_EXIST | wxMULTIPLE | wxCHANGE_DIR);
	if(OpenFile.ShowModal() == wxID_OK){
		wxArrayString *wxas = new wxArrayString();
		OpenFile.GetPaths(*wxas);
		int nFiles = wxas->GetCount();
		for ( int n = 0; n < nFiles; n++ ) {
			if ( wxFile::Exists( wxas->Item(n) )){
				if  ( FileListBox->GetString( FileListBox->FindString( wxas->Item(n) ) ) != wxas->Item(n) )
					FileListBox->Append(wxas->Item(n));
				}
			}
		delete wxas;
		}
	}

void DivFixpp::OnRemoveClick(wxCommandEvent& event){
	wxArrayInt wxaint;
	FileListBox->GetSelections( wxaint );
	while( 0 < wxaint.GetCount() ){
        FileListBox->Delete( wxaint.Item(0) );
        FileListBox->GetSelections( wxaint );
        wxYield();
        }
    }

void DivFixpp::OnClearClick(wxCommandEvent& event){
    FileListBox->Clear();
    }

void DivFixpp::OnFixClick(wxCommandEvent& event){
	switch( state ){
		case fix:{
			ErrorCheckMode = false;
			if( wxchk_keeporiginal->GetValue() && (!wxFileName::DirExists( textCtrl_savepath->GetValue() ) ) ){
				wxMessageDialog *msg = new wxMessageDialog(this,
					   _("There is no or invalid output path selected!\nPlease select path now."),
					   _("Output Path Not Selected!"),
					   wxOK,
					   wxDefaultPosition);
				msg->ShowModal();
				delete msg;
				}
			else if( (wxchk_logerrors->GetValue()) ){
				wxFileName *logfile = new wxFileName(textCtrl_logpath->GetValue() );
				if(!logfile->IsOk() || logfile->IsDir() || !logfile->IsAbsolute() ){
					wxMessageDialog *msg = new wxMessageDialog(this,
						_("There is no or invalid Log out file selected!\nPlease select logout file now."),
						_("Log File Not Selected!"),
						wxOK,
						wxDefaultPosition);
					msg->ShowModal();
					delete msg;
					}
				else{
					state = pause; // press 4 pause
					wxbtn_fix->SetLabel(_("Pause"));
					TextCtrl_log->Clear();
					wxThreadHelper::Create();
					GetThread()->Run();
					}
				delete logfile;
				}
			else{
				wxConfigBase::Get()->Write( _T("PathOut"),textCtrl_savepath->GetValue() );
				if( wxchk_logerrors->GetValue() )
					wxConfigBase::Get()->Write( _T("PathLog"),textCtrl_logpath->GetValue() );
				wxConfigBase::Get()->Flush();
				TextCtrl_log->Clear();
				state = pause; // press 4 pause
				wxbtn_fix->SetLabel(_("Pause"));
				wxThreadHelper::Create();
				GetThread()->Run();
				}
			break;
			}
		case pause:{
			state = resume;
			wxbtn_fix->SetLabel(_("Resume"));
			GetThread()->Pause();
			break;
			}
		case resume:{
			state = pause;
			wxbtn_fix->SetLabel(_("Pause"));
			GetThread()->Resume();
			break;
			}
		}
	}

void DivFixpp::Disabler(){
 //   wxbtn_fix->Disable();
    wxbtn_strip->Disable();
    wxbtn_checkerrors->Disable();
    wxbtn_add->Disable();
    wxbtn_remove->Disable();
    wxbtn_clear->Disable();
    wxbtn_stop->Enable();
	wxbtn_preferences->Disable();
    //WxListBox->Disable();	//No effect
    wxchk_keeporiginal->Disable();
    wxchk_cutout->Disable();
    wxchk_logerrors->Disable();
    textCtrl_savepath->Disable();
    textCtrl_logpath->Disable();
    wxbitbtn_savepath->Disable();
    wxbitbtn_logpath->Disable();
    }

void DivFixpp::Enabler(){
//	wxbtn_fix->Enable();
	wxbtn_strip->Enable();
	wxbtn_checkerrors->Enable();
	wxbtn_add->Enable();
	wxbtn_remove->Enable();
	wxbtn_clear->Enable();
	wxbtn_stop->Disable();
	wxbtn_preferences->Enable();
	//WxListBox->Enable(); No effect
	wxchk_keeporiginal->Enable();
	wxchk_cutout->Enable();
	wxchk_logerrors->Enable();
	if( wxchk_keeporiginal->GetValue() ){
		textCtrl_savepath->Enable();
		wxbitbtn_savepath->Enable();
		}
	if(wxchk_logerrors->GetValue()){
		textCtrl_logpath->Enable();
		wxbitbtn_logpath->Enable();
		}
    }

void *DivFixpp::Entry(){
	wxMutexGuiEnter();
	Disabler();
	wxMutexGuiLeave();
	for( unsigned i =0 ; i < FileListBox->GetCount() ; i++){
		wxMutexGuiEnter();
		FileListBox->SetString(i, wxString::FromAscii("-> ") + FileListBox->GetString(i) );
		wxMutexGuiLeave();
		Fix( FileListBox->GetString(i).AfterFirst(' '),
		(textCtrl_savepath->GetValue() + wxFileName::GetPathSeparator() + wxString::FromAscii("DivFix++.") + FileListBox->GetString(i).AfterLast(wxFileName::GetPathSeparator())),
			wxchk_keeporiginal->GetValue(),
			wxchk_cutout->GetValue(),
			ErrorCheckMode,
			true, //keyframe scene startings enabled
			GetThread()
			);
		wxMutexGuiEnter();
		FileListBox->SetString(i, FileListBox->GetString(i).AfterFirst(' ') );
		TextCtrl_log->AppendText(_T("\n\n"));
		wxMutexGuiLeave();
		}
	if(wxchk_logerrors->GetValue())
		if( !TextCtrl_log->SaveFile( textCtrl_logpath->GetValue().c_str() ))
			TextCtrl_log->AppendText(_("Error at writing Error log!\n"));
	OnExit();
	return 0;
    }

void DivFixpp::OnExit(){
    // Enables Buttons and List Box if thread killed...
	wxMutexGuiEnter();
	Enabler();
	state = fix;
	wxbtn_fix->SetLabel(_("Fix"));
	WxGauge->SetValue(0);
	wxMutexGuiLeave();
	delete wxConfigBase::Set((wxConfigBase *) NULL);
	}

void DivFixpp::OnStripClick(wxCommandEvent& event){
    if( !wxchk_keeporiginal->GetValue() && !wxchk_cutout->GetValue()){
        TextCtrl_log->Clear();
        Disabler();
        for( unsigned i =0 ; i < FileListBox->GetCount() ; i++ ){
            FileListBox->SetString(i, _T("-> ") + FileListBox->GetString(i) );
            TextCtrl_log->AppendText( _("Striping index at file: ") + FileListBox->GetString(i).AfterLast(wxFileName::GetPathSeparator()) + _T("\n") );
            if(! Strip( FileListBox->GetString(i).AfterFirst(' ') ))
                TextCtrl_log->AppendText( _("Error occured at striping index\n" ));
            else
                TextCtrl_log->AppendText( _("Index striped successfuly\n" ));
            FileListBox->SetString(i, FileListBox->GetString(i).AfterFirst(' ') );
            }
        Enabler();
        }
    else{
        wxMessageDialog *msg = new wxMessageDialog(this,
                _("Strip Process is OVERWRITES file!\nPlease uncheck Keep Original and Cut Out Bad Parts options."),
                _("Unable To Strip!"),
                wxOK,
                wxDefaultPosition);
        msg->ShowModal();
        delete msg;
        }
    }

void DivFixpp::OnCheckClick(wxCommandEvent& event){
    ErrorCheckMode=true;
    TextCtrl_log->Clear();
    wxThreadHelper::Create();
    GetThread()->Run();
	}

void DivFixpp::OnStopClick(wxCommandEvent& event){
	 if(GetThread()!=NULL)
		if(GetThread()->IsRunning()){
			GetThread()->Delete();
			//WxGauge->SetValue(0);
			}
     }

void DivFixpp::OnPathClick( wxCommandEvent& event ){
	wxDirDialog OpenDir(this, _("Select a save directory path"), _T(""), wxDD_NEW_DIR_BUTTON);
    if( OpenDir.ShowModal() == wxID_OK ){
        textCtrl_savepath->SetValue(OpenDir.GetPath());
        wxConfigBase::Get()->Write( _T("PathOut"),OpenDir.GetPath() );
        wxConfigBase::Get()->Flush();
		}
    }

void DivFixpp::OnLogClick( wxCommandEvent& event ){
	wxFileDialog SaveLog(this, _("Choose a file"), _T(""), _T(""), _T("*.log"), wxSAVE | wxOVERWRITE_PROMPT);
    if( SaveLog.ShowModal()  == wxID_OK ){
		textCtrl_logpath->SetValue( SaveLog.GetPath());
		wxConfigBase::Get()->Write( _T("PathLog"),SaveLog.GetPath() );
        wxConfigBase::Get()->Flush();
		}
    }

void DivFixpp::OnKeepOriginalClick(wxCommandEvent& event){
	if( wxchk_keeporiginal->GetValue() ){
		textCtrl_savepath->Enable();
		wxbitbtn_savepath->Enable();
		}
	else{
		textCtrl_savepath->Disable();
		wxbitbtn_savepath->Disable();
		}
	}

void DivFixpp::OnLogErrorClick(wxCommandEvent& event){
	if(wxchk_logerrors->GetValue()){
		textCtrl_logpath->Enable();
		wxbitbtn_logpath->Enable();
		}
	else{
		textCtrl_logpath->Disable();
		wxbitbtn_logpath->Disable();
		}
	}

void DivFixpp::OnPreferencesClick(wxCommandEvent& event){
		PreferencesDialog *prefdlg = new PreferencesDialog( m_locale, this, -1 );
		prefdlg->ShowModal();
        prefdlg->Destroy();

        wxConfigBase* pConfig = wxConfigBase::Get();
		if ( ! pConfig->Read(_T("Language")).IsEmpty() ){
			int lang = wxConfigBase::Get()->Read(_T("Language"), -1) ;
			if ( lang != -1 )
				if ( m_locale.GetLanguage() != lang){
					if( lang == 0 && m_locale.GetSystemLanguage() == m_locale.GetLanguage() )	//prevents default redraw
						return;
					else
						{
						wxGetApp().ReCreateGui();
						Destroy();
						}
					}
			}
	}

void DivFixpp::OnAboutClick(wxCommandEvent& event){
	wxAboutDialogInfo myAbout;
	myAbout.AddDeveloper( _T("Erdem U. Altinyurt") );
	myAbout.SetLicense( _T("DivFix++ AVI Fix and Previewer utility.\n"
             "Copyright (C) 2006  Erdem U. Altinyurt\n"
             "\n"
             "This program is free software; you can redistribute it and/or\n"
             "modify it under the terms of the GNU General Public License\n"
             "as published by the Free Software Foundation; either version 2"
             "of the License, or any later version."
             "\n"
             "This program is distributed in the hope that it will be useful,\n"
             "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
             "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
             "GNU General Public License for more details.\n"
             "\n"
             "You should have received a copy of the GNU General Public License\n"
             "along with this program; if not, write to the Free Software\n"
             "Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.\n"
             "\n"
             "home:  divfixpp.sourceforge.net\n"
             "email: death_knight@gamebox.net\n")
             );

	myAbout.SetCopyright( _T( "Copyright (C) 2006  Erdem U. Altinyurt" ) );
	myAbout.SetDescription( _("DivFix++ is an AVI Fix and Previewer utility") );
	myAbout.SetName( _T("DivFix++") );
    myAbout.SetWebSite( _T("http://divfixpp.sourceforge.net"));
	myAbout.AddTranslator(_T("Turkish: by my-self :)") );
	myAbout.AddTranslator(_T("Hungarian: DirektX") );
	myAbout.AddTranslator(_T("Japanise: Norihito Waku") );
	myAbout.AddTranslator(_T("Czhech: SeC0nd.uNiT") );
    wxAboutBox( myAbout );
}

PreferencesDialog::PreferencesDialog(wxLocale& my_locale, wxWindow *parent, wxWindowID id)
:PreferencesDialog_Gui( parent, id ), m_locale(my_locale)
	{
	GetInstalledLanguages(LangNames,LangIds);
	wxchc_language->Clear();
	wxchc_language->Append( LangNames );

	wxConfigBase *pConfig = wxConfigBase::Get();

	if ( ! pConfig->Read(_T("Language")).IsEmpty() ){
		int lang = pConfig->Read(_T("Language"), -1) ;
		if ( lang != -1 )
			for( unsigned i = 0 ; i < LangIds.Count() ; i++)
				if( lang == LangIds.Item(i)){
					wxchc_language->SetSelection(i);
					break;
					}
		}
		textCtrl_playerpath->SetValue( pConfig->Read(_T("PathPlayer") ) );
	}

void PreferencesDialog::GetInstalledLanguages(wxArrayString & names, wxArrayLong & identifiers){
	names.Clear();
	identifiers.Clear();
	const wxLanguageInfo * langinfo;
	wxString name = wxLocale::GetLanguageName(wxLANGUAGE_DEFAULT);
	if(!name.IsEmpty()){
		names.Add(_T("Default"));
		identifiers.Add(wxLANGUAGE_DEFAULT);
		}

	names.Add(_T("English"));
	identifiers.Add(wxLANGUAGE_ENGLISH);

	wxString dirname;
	wxFileName flnm( wxGetApp().argv0 );
	//This part scans default linux directory for available catalogs

	dirname = (_T("/usr/local/share/locale"));
	if(wxDir::Exists( dirname )){
		wxDir dir(dirname);
		for(bool cont = dir.GetFirst(&dirname,_T(""),wxDIR_DEFAULT);
			cont;
			cont = dir.GetNext(&dirname)){
			langinfo = wxLocale::FindLanguageInfo(dirname);
			if(langinfo != NULL){
				if(wxFileExists(
					dir.GetName() + wxFileName::GetPathSeparator() +
					dirname + wxFileName::GetPathSeparator() +
					_T("LC_MESSAGES") + wxFileName::GetPathSeparator() +
					_T("DivFix++") + _T(".mo"))){

					bool duplicate = false;
					for( unsigned i = 0 ; i < identifiers.Count() ; i++ )	//Avoid duplicated locales
						if( identifiers.Item(i) == langinfo->Language ){
							duplicate = true;
							break;
							}
					if ( duplicate ) break;

					names.Add(langinfo->Description);
					identifiers.Add(langinfo->Language);
					}
				}
			}
		}

	//This part scans for for available catalogs on local directory, for Windows.
	dirname = (flnm.GetPath() + wxFileName::GetPathSeparator() + _T("locale"));
	if(wxDir::Exists( dirname )){
		wxDir dir(dirname);
		for(bool cont = dir.GetFirst(&dirname,_T(""),wxDIR_DEFAULT);
			cont;
			cont = dir.GetNext(&dirname)){
			langinfo = wxLocale::FindLanguageInfo(dirname);
			if(langinfo != NULL){
				if(wxFileExists(
					dir.GetName() + wxFileName::GetPathSeparator() +
					dirname + wxFileName::GetPathSeparator() +
					_T("DivFix++") + _T(".mo"))){

					bool duplicate = false;
					for( unsigned i = 0 ; i < identifiers.Count() ; i++ )	//Avoid duplicated locales
						if( identifiers.Item(i) == langinfo->Language ){
							duplicate = true;
							break;
							}
					if ( duplicate ) break;

					names.Add(langinfo->Description);
					identifiers.Add(langinfo->Language);
					}
				}
			}
		}

	//This part scans for for available catalogs on Bundle directory,for Mac.
	dirname = (flnm.GetPath() + wxFileName::GetPathSeparator() +
		_T("..") + wxFileName::GetPathSeparator() +
		_T("Resources") + wxFileName::GetPathSeparator() +
		_T("locale"));

	if(wxDir::Exists( dirname )){
		wxDir dir(dirname);
		for(bool cont = dir.GetFirst(&dirname,_T(""),wxDIR_DEFAULT);
			cont;
			cont = dir.GetNext(&dirname)){
			langinfo = wxLocale::FindLanguageInfo(dirname);
			if(langinfo != NULL){
				if(wxFileExists(
					dir.GetName() + wxFileName::GetPathSeparator() +
					dirname + wxFileName::GetPathSeparator() +
					_T("DivFix++") + _T(".mo"))){

					bool duplicate = false;
					for( unsigned i = 0 ; i < identifiers.Count() ; i++ )	//Avoid duplicated locales
						if( identifiers.Item(i) == langinfo->Language ){
							duplicate = true;
							break;
							}
					if ( duplicate ) break;

					names.Add(langinfo->Description);
					identifiers.Add(langinfo->Language);
					}
				}
			}
		}
	}

void PreferencesDialog::OnConfigChange( wxCommandEvent& event ){
	}

void PreferencesDialog::OnPlayerClick( wxCommandEvent& event ){
	wxFileDialog OpenPlayer(this, _("Choose a player"), _T(""), _T(""), _T("*"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if( OpenPlayer.ShowModal() == wxID_OK )
		textCtrl_playerpath->SetValue( OpenPlayer.GetPath());
	}

void PreferencesDialog::OnClose( wxCloseEvent& event ){
	wxConfigBase::Get()->Write( _T("Language"), LangIds.Item(wxchc_language->GetSelection()) );
	wxConfigBase::Get()->Write( _T("PathPlayer"),textCtrl_playerpath->GetValue() );
	wxConfigBase::Get()->Flush();
	PreferencesDialog_Gui::OnClose( event );
	}
