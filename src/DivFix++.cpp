//-----------------------------------------------------------------------
//
// Name:        DivFix++.cpp
// Author:      Death Knight
// Created:     26.05.2006 02:24:47
// Description: DivFix++ GUI Code
//
//-----------------------------------------------------------------------
/***********************************(GPL)********************************
*   DivFix++ is AVI Video Repair & Preview Utility.                     *
*   Copyright (C) 2011 Erdem U. Altinyurt                              *
*                                                                       *
*   This program is free software; you can redistribute it and/or       *
*   modify it under the terms of the GNU General Public License         *
*   as published by the Free Software Foundation; either version 2      *
*   of the License, or any later version.                               *
*                                                                       *
*   This program is distributed in the hope that it will be useful,     *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       *
*   GNU General Public License for more details.                        *
*                                                                       *
*   You should have received a copy of the GNU General Public License   *
*   along with this program;                                            *
*   if not, write to the Free Software Foundation, Inc.,                *
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA        *
*                                                                       *
*               home  : divfixpp.sourceforge.net                        *
*               email : spamjunkeater at gmail.com                      *
*************************************************************************/
#include "DivFix++.h"
#include "DivFix++Gui.h"

DECLARE_APP(DivFixppApp)

DivFixpp::DivFixpp(wxLocale& my_locale, wxWindow *parent, wxWindowID id)
:DivFixpp_Gui( parent, id ), DivFixp2Core( WxGauge,TextCtrl_log), m_locale( my_locale )
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
		DivFixp2Core::WxGauge = m_gauge;
		DivFixp2Core::WxMemoLog = TextCtrl_log;
		}
	}

DivFixpp::~DivFixpp(){
	if (single_inst_checker != NULL){
		delete single_inst_checker;
		single_inst_checker = NULL;
		}
	wxConfigBase *pConfig = wxConfigBase::Get();
	if ( pConfig == NULL )
		return;

	// save the control's values to the config
	pConfig->Write( _T("PathOutRelativeEnable"),wxchk_relativeoutputfile->GetValue() );
	pConfig->Write( _T("PathOut"),textCtrl_savepath->GetValue() );

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
	this->SetTitle(wxString(_T("DivFix++ ")) << _T("v") << _T(_VERSION_) << _T(" ") << _T(_VERSION_STRING_) << _T(_OS_) );
	this->Center();
	this->SetIcon(DivFixpp_xpm);
#ifdef __WXMSW__
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
#endif

	SetDropTarget( new DnDFile(FileListBox) );
	FileListBox->SetDropTarget( new DnDFile(FileListBox) );

	wxConfigBase *pConfig = wxConfigBase::Get();
	bool tmp;
	if ( ! pConfig->Read(_T("PathOut")).IsEmpty() )
		textCtrl_savepath->SetValue( pConfig->Read(_T("PathOut")) );
	if ( pConfig->Read(_T("PathOutRelativeEnable"), &tmp) )
		wxchk_relativeoutputfile->SetValue( tmp );

	bool update_enable = true;
	if ( ! pConfig->Read(_T("UpdateCheck"), &update_enable )){
		update_enable = true;
		wxConfigBase::Get()->Write( _T("UpdateCheck"), update_enable );
		}
	if( update_enable ){
		//time_t last_chk=0;
		double last_chk=0;
		pConfig->Read(_T("LastUpdateCheckTime"), (&last_chk));
		if( wxDateTime::Now() - wxDateSpan::Day() > wxDateTime( last_chk ) ){
			pConfig->Write(_T("LastUpdateCheckTime"), static_cast< double >( wxDateTime::Now().GetTicks()) );
			VersionChecker vc( wxT("http://divfixpp.sourceforge.net/version.php"), wxT(_VERSION_) );
			}
		}
	Enabler();	//for adjust PathOut and Pathlog Buttons & TextCtrls

	// read old location and size from registry
	int x = pConfig->Read(_T("x"), 100),
		y = pConfig->Read(_T("y"), 100),
		w = pConfig->Read(_T("w"), 600),
		h = pConfig->Read(_T("h"), 320);

	// Normalizing for avoid screen disapperaring
	wxSize dsz = wxGetDisplaySize();
	x = x < 0 ? 0 : x < dsz.x ? x : dsz.x - w;
	y = y < 0 ? 0 : y < dsz.y ? y : dsz.y - h;

	// restore frame position and size
	Move(x, y);
	SetClientSize(w, h);

	}

void DivFixpp::DivFixppClose(wxCloseEvent& event){
    Destroy();
}

void DivFixpp::OnAddClick(wxCommandEvent& event){
	wxFileDialog OpenFile(this, _("Choose a file"), _T(""), _T(""), _T("*"), wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE | wxFD_CHANGE_DIR);
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
			if( wxchk_keeporiginal->GetValue()				// Keeping original file
				&& !wxchk_relativeoutputfile->GetValue()	// while relative output option is not enabled
				&& (!wxFileName::DirExists( textCtrl_savepath->GetValue() ) ) ){	// and there is no correct path selected
				wxMessageDialog *msg = new wxMessageDialog(this,
					   _("There is no or invalid output path selected!\nPlease select path now."),
					   _("Output Path Not Selected!"),
					   wxOK,
					   wxDefaultPosition);
				msg->ShowModal();
				delete msg;
				break;
				}

			if( wxFileName::DirExists( textCtrl_savepath->GetValue() ) )
				wxConfigBase::Get()->Write( _T("PathOut"),textCtrl_savepath->GetValue() );

			wxConfigBase::Get()->Flush();
			TextCtrl_log->Clear();
			state = pause; // press 4 pause
			wxbtn_fix->SetLabel(_("Pause"));
			wxThreadHelper::Create();
			GetThread()->Run();
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
	wxbtn_strip->Disable();
    wxbtn_checkerrors->Disable();
    wxbtn_add->Disable();
    wxbtn_remove->Disable();
    wxbtn_clear->Disable();
    wxbtn_stop->Enable();
	wxbtn_preferences->Disable();
    //WxListBox->Disable();	//No effect
    wxchk_relativeoutputfile->Disable();
    wxchk_keeporiginal->Disable();
    wxchk_cutout->Disable();
    wxchk_keyframe->Disable();
    textCtrl_savepath->Disable();
    wxbitbtn_savepath->Disable();
    }

void DivFixpp::Enabler(){
	wxbtn_strip->Enable();
	wxbtn_checkerrors->Enable();
	wxbtn_add->Enable();
	wxbtn_remove->Enable();
	wxbtn_clear->Enable();
	wxbtn_stop->Disable();
	wxbtn_preferences->Enable();
	//WxListBox->Enable(); No effect
	wxchk_relativeoutputfile->Enable();
	wxchk_keeporiginal->Enable();
	wxchk_cutout->Enable();
	wxchk_keyframe->Enable();
	if( wxchk_keeporiginal->GetValue() && !wxchk_relativeoutputfile->GetValue() ){
		textCtrl_savepath->Enable();
		wxbitbtn_savepath->Enable();
		}
    }

void *DivFixpp::Entry(){
	wxMutexGuiEnter();
	Disabler();
	int flag =  ( wxchk_keeporiginal->GetValue() ? 0 : DivFixp2Core::OverWrite ) |
			( wxchk_cutout->GetValue() ? DivFixp2Core::CutOut : 0 ) |
			( ErrorCheckMode ? DivFixp2Core::Error_Check : 0 ) |
			( wxchk_keyframe->GetValue() ? DivFixp2Core::KeyFrameStart : 0 );
	wxMutexGuiLeave();
	for( unsigned i =0 ; i < FileListBox->GetCount() ; i++){
		wxMutexGuiEnter();
		FileListBox->SetString(i, wxString::FromAscii("-> ") + FileListBox->GetString(i) );
		wxMutexGuiLeave();

		wxString wxAppendOutput = wxFileName::GetPathSeparator()			// + "/"
			 + wxString::FromAscii("DivFix++.")		// + "DivFix++."
			 + FileListBox->GetString(i).AfterLast(wxFileName::GetPathSeparator());	// + "broken.avi"

		Repair( FileListBox->GetString(i).AfterFirst(' '),	//input
			wxchk_relativeoutputfile->GetValue()		//output selection		//for example /home/video/broken.avi

			?( wxString(FileListBox->GetString(i).AfterFirst(' ').BeforeLast(wxFileName::GetPathSeparator())	// "/home/video"
			 + wxAppendOutput ))

			:(textCtrl_savepath->GetValue()				// "/<output directory>"

			 + wxAppendOutput) ,

			flag
			);

		wxMutexGuiEnter();
		FileListBox->SetString(i, FileListBox->GetString(i).AfterFirst(' ') );
		TextCtrl_log->AppendText(_T("\n\n"));
		wxMutexGuiLeave();
		}
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
            if(not Strip( FileListBox->GetString(i).AfterFirst(' ') ))
                TextCtrl_log->AppendText( _("Error occured at striping index\n" ));
            else
                TextCtrl_log->AppendText( _("Index striped successfully\n" ));
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
		if( GetThread()->IsRunning() || GetThread()->IsPaused() ){
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

void DivFixpp::OnCheck_RelativeOutputFile( wxCommandEvent& event ){
	if( wxchk_relativeoutputfile->GetValue() ){
		textCtrl_savepath->Disable();
		wxbitbtn_savepath->Disable();
		wxchk_keeporiginal->SetValue( true );
		}
	else{
		textCtrl_savepath->Enable();
		wxbitbtn_savepath->Enable();
		}
	}

void DivFixpp::OnCheck_KeepOriginal(wxCommandEvent& event){
	if( wxchk_keeporiginal->GetValue() ){
		textCtrl_savepath->Enable();
		wxbitbtn_savepath->Enable();
		}
	else{
		textCtrl_savepath->Disable();
		wxbitbtn_savepath->Disable();
		wxchk_relativeoutputfile->SetValue( false );
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
						delete single_inst_checker;
						single_inst_checker=NULL;
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
             "email: spamjunkeater at gmail.com\n")
             );

	myAbout.SetCopyright( _T( "Copyright (C) 2011  Erdem U. Altinyurt" ) );
	myAbout.SetDescription( _("DivFix++ is an AVI Video Repair and Preview utility") );
	myAbout.SetName( _T("DivFix++") );
	myAbout.SetVersion( wxString( _T("v")) << _T( _VERSION_ ) << _T(" ") << _T(_VERSION_STRING_) );
    myAbout.SetWebSite( _T("http://divfixpp.sourceforge.net"));
	myAbout.AddTranslator(_T("Czech :\tSeC0nd.uNiT") );
	myAbout.AddTranslator(_T("Farsi :\tMXAmin") );
	myAbout.AddTranslator(_T("French:\tDidier Bourre & Oggiwan") );
	myAbout.AddTranslator(_T("Gallician:\tMarcos Lansgarza") );
	myAbout.AddTranslator(_T("German:\tBastian Wiegmann") );
	myAbout.AddTranslator(_T("Hebrew:\tAriel Nemtzov") );
	myAbout.AddTranslator(_T("Hungarian:DirektX") );
	myAbout.AddTranslator(_T("Italian:\tGiovanni Fiocco") );
	myAbout.AddTranslator(_T("Japanese:\tNorihito Waku") );
	myAbout.AddTranslator(_T("Korean:\tStarCodec") );
	myAbout.AddTranslator(_T("Spanish:\tOCReactive") );
	myAbout.AddTranslator(_T("Russian:\tKonstantin Krasnov") );
	myAbout.AddTranslator(_T("Turkish:\tby my-self :)") );
	myAbout.AddTranslator(_T("Ukranian:\tarestarh") );
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
		bool update = false;
		pConfig->Read(_T("UpdateCheck"), &update);
		wxchk_update->SetValue( update );
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
	//This part scans default installation on linux directory for available catalogs - both on main and on local share
	for( int i = 0 ; i < 2 ; i++ ){
		if( i == 0 )
			dirname = (_T("/usr/local/share/locale"));
		else dirname = (_T("/usr/share/locale"));
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
		}

	//This part scans for for available catalogs on local directory, for Windows and Linux.
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

#ifdef __WXMAC__
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
#endif  //__WXMAC__
	}

void PreferencesDialog::OnPlayerClick( wxCommandEvent& event ){
	wxFileDialog OpenPlayer(this, _("Choose a player"), _T(""), _T(""), _T("*"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if( OpenPlayer.ShowModal() == wxID_OK )
		textCtrl_playerpath->SetValue( OpenPlayer.GetPath());
	}

void PreferencesDialog::OnClose( wxCloseEvent& event ){
	wxConfigBase::Get()->Write( _T("Language"), LangIds.Item(wxchc_language->GetSelection()) );
	wxConfigBase::Get()->Write( _T("PathPlayer"), textCtrl_playerpath->GetValue() );
	wxConfigBase::Get()->Write( _T("UpdateCheck"), wxchk_update->GetValue() );
	wxConfigBase::Get()->Flush();
	event.Skip();
	}

void PreferencesDialog::OnCheckNewVersion( wxCommandEvent& event ){
	if( wxchk_update->GetValue() ){
		VersionChecker vc( wxT("http://divfixpp.sourceforge.net/version.php"), wxT(_VERSION_) );
		wxConfigBase::Get()->Write(_T("LastUpdateCheckTime"), static_cast< double >( wxDateTime::Now().GetTicks() )) ;
		}
	}

VersionChecker::VersionChecker( wxString _url, wxString _version, wxWindow *parent, wxWindowID id )
:UpdateDialog_Gui( parent, id ){
	wxURL url( _url );
	if (url.IsOk()){
		url.GetProtocol().SetTimeout(3);
		wxInputStream *in_stream = url.GetInputStream();
		if( in_stream == NULL || in_stream->GetSize() > 10 ){
			return;	//need for keep safe
			}
		char *bfr = new char[in_stream->GetSize()+1];
		for(unsigned i = 0 ; i < in_stream->GetSize()+1 ; i++ )
			bfr[i]=0;
		in_stream->Read(bfr, in_stream->GetSize());
		if( strcmp( bfr, _version.To8BitData() ) > 0 ){
			wxString newver = wxString::FromAscii( bfr );
			version_text->SetLabel(wxString::Format( _("New DivFix++ version %s is available!"), newver.c_str() ));
			wxbtmp_icon->SetBitmap(  wxArtProvider::GetBitmap( wxART_TIP, wxART_MESSAGE_BOX ) );
			Centre();
			Fit();
			wxBell();
			ShowModal();
			}
		}
	}
void VersionChecker::OnChkDisplay( wxCommandEvent& event ){
	wxConfigBase::Get()->Write( _T("UpdateCheck"), !wxchk_display->GetValue());
	}
