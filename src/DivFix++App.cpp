//-----------------------------------------------------------------------
//
// Name:        DivFix++App.cpp
// Author:      Death Knight
// Created:     26.05.2006 02:24:47
// Description: DivFix++ Main Application Code
//
//-----------------------------------------------------------------------
/***********************************(GPL)********************************
*   DivFix++ is AVI Video Repair & Preview Utility.                     *
*   Copyright (C) 2006  Erdem U. Altinyurt                              *
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
*               email : death_knight at gamebox.net                     *
*************************************************************************/

#include "DivFix++App.h"
#include "DivFix++.h"

IMPLEMENT_APP(DivFixppApp)

void DivFixppApp::OnInitCmdLine(wxCmdLineParser& parser){
	static const wxCmdLineEntryDesc cmdLineDesc[] ={
			{ wxCMD_LINE_OPTION, _T("i"),		_T("input"),		_("input file"),				wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
			{ wxCMD_LINE_OPTION, _T("o"),		_T("output"),		_("output file or directory"),	wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
			{ wxCMD_LINE_SWITCH, _T("p"),		_T("preview"),		_("delete output file after player is closed"),	wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },
			{ wxCMD_LINE_SWITCH, _T("f"),		_T("fix_index_only"),_("doesn't cuts out bad parts of file"),		wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },
			{ wxCMD_LINE_SWITCH, _T("w"),		_T("overwrite"),	_("overwrite original file"),	wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },
			{ wxCMD_LINE_SWITCH, _T("a"),		_T("all_frames"),	_("doesn't skip unwanted frames"),	wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },
			{ wxCMD_LINE_SWITCH, _T("s"),		_T("skip_fix"),		_("skips fix process if index is available "),	wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },
			{ wxCMD_LINE_OPTION, _T("m"),		_T("media_player"),	_("path of player"),        wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
			{ wxCMD_LINE_SWITCH, _T("h"),		_T("help"),			_("shows help"),			wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
			{ wxCMD_LINE_PARAM,		NULL,			NULL,			_T(""),	wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_MULTIPLE|wxCMD_LINE_PARAM_OPTIONAL },
			{ wxCMD_LINE_NONE }
			};
	parser.SetDesc (cmdLineDesc);
	parser.SetSwitchChars (_T("-")); // must refuse '/' as parameter starter or cannot use "/path" style paths
	}

bool DivFixppApp::OnCmdLineParsed(wxCmdLineParser& parser){
	bool keeporiginal = !parser.Found(_T("w"));	// keep original != overwrite
	bool cutout = !parser.Found(_T("f"));		// cutout 	 != fix_index_only
	bool skip = !parser.Found(_T("a"));			// skip		 != all_frames
	bool preview = parser.Found(_T("p"));
	wxString input,output,m_player;
	if(parser.Found(_T("i"),&input))
		if(! wxFileName::FileExists( input ) ){
			wxLogError(wxString(_("Error: "))+_("Input file cannot be opened!\n"));
			return false;
			}

	if( parser.GetParamCount() == 1 ){ 							//if only one parameter feeded
		if( wxFileName::FileExists( parser.GetParam( 0 ) ) ){ 	//if it's a file
			input = parser.GetParam( 0 );						//make it input
			preview = true;										//and assume preview mode
			}
		else
			wxLogError(wxString(_("Error: "))+_("Input file cannot be opened!\n"));
		}

	if( wxFileName::FileExists( input ) ){
		if(parser.Found(_T("o"), &output)){
			if( input == output && keeporiginal ){
				wxLogError(wxString(_("Error: "))+_("Input and Output file is same while overwrite option is off!"));
				return false;
				}
			if( wxFileName::DirExists(output)){
				output = output + wxFileName::GetPathSeparator()+_T("DivFix++.")+input.AfterLast(wxFileName::GetPathSeparator());
				}
			}
		if( output == _T("") ){	//if there is no output then output file defaulting to DivFix++.filename
			output = input.BeforeLast(wxFileName::GetPathSeparator())+wxFileName::GetPathSeparator()+_T("DivFix++.")+input.AfterLast(wxFileName::GetPathSeparator());
			}

		if( preview ){														// Preview mode here
			if(! parser.Found(_T("m"), &m_player) )							// read player location from CLI
				m_player = wxConfigBase::Get()->Read(_T("PathPlayer") );	// if not given, default to player at preferences
			if( m_player.IsEmpty() ){
				wxLogError(wxString(_("Error: "))+_("No player defined! Define by \"-m\" option or select from preferences at GUI."));
				return false;
				}
			DivFixppCore *dfx = new DivFixppCore;
			if( !dfx->IsAvi( input ) ||	// if file is not AVI,
				(dfx->IsAvi( input ) && dfx->HasProperIndex( input ) && parser.Found(_T("s"))) ){ // or AVI file with index + Skip flag
					wxExecute( m_player+wxChar(' ')+input ); 	//execute directly on m_player
					return false; // disables gui
					}
			delete dfx;
			}

		wxString prgstitle;
		prgstitle << _T("DivFix++ ") << _T(_VERSION_) << _T(_OS_) << _(" on Progress");
		wxProgressDialog *prgrs = new wxProgressDialog(prgstitle , _("Fixing file: ")+input, 100,  NULL, wxPD_SMOOTH|wxPD_REMAINING_TIME|wxPD_CAN_ABORT|wxPD_AUTO_HIDE );
		prgrs->SetWindowStyleFlag( prgrs->GetWindowStyleFlag()|wxSTAY_ON_TOP|wxMINIMIZE_BOX );
		wxIcon DivFixpp_ICON (DivFixpp_xpm);
		prgrs->SetIcon(DivFixpp_ICON);
		DivFixppCore *dx = new DivFixppCore( prgrs );

		if(dx->Fix( input, output, keeporiginal, cutout, false, skip )){
			delete dx;
			delete prgrs;
			if(! parser.Found(_T("m"), &m_player) )						// read player location from CLI
				m_player = wxConfigBase::Get()->Read(_T("PathPlayer") );// or read from registry
			if( preview ){		// keep/delete output file after playback at preview mode!{
				wxExecute( m_player+_T(" \"")+output+_T("\""), wxEXEC_SYNC );
				wxRemoveFile( output );
				}
			}
		else{
			if(! prgrs->Update(100))	//if Cancel pressed, don't show error.
				wxLogError(_("Error Occured while fixing file!"));
			delete dx;
			delete prgrs;
			return false;
			}
		}
	return false; // disables gui
	}

bool DivFixppApp::OnCmdLineError(wxCmdLineParser& parser){
	OnCmdLineHelp(parser);
	return false;
	}

bool DivFixppApp::OnInit(){
	argv0.Clear();
	argv0.Printf(_T("%s"), argv[0]);
	SetLanguage();
	if(argc!=1){		//If there is an argument
		wxCmdLineParser parser(argc, argv);
		OnInitCmdLine( parser );
		parser.Parse();
		return OnCmdLineParsed( parser );
		}
	SetVendorName(_T("Death Knight"));
	SetAppName(_T("DivFix++"));
	return ReCreateGui();
	}

bool DivFixppApp::SetLanguage(){
	if ( ! wxConfigBase::Get()->Read(_T("Language")).IsEmpty() ){
		int langid = wxConfigBase::Get()->Read( _T("Language"), -1 );
		if ( !my_locale.Init( langid, wxLOCALE_CONV_ENCODING) ){
				wxLogError(_T("This language is not supported by the system."));
				return false;
			}
			wxFileName flnm(argv0);
			my_locale.AddCatalogLookupPathPrefix( flnm.GetPath(wxPATH_GET_SEPARATOR) + _T("locale") );
//#ifdef _UNIX_
//			my_locale.AddCatalogLookupPathPrefix( _T("/usr/local/share/locale/") );
//#endif
#ifdef __WXMAC__
			my_locale.AddCatalogLookupPathPrefix( flnm.GetPath(wxPATH_GET_SEPARATOR) +
			_T("..") + wxFileName::GetPathSeparator() + _T("Resources") + wxFileName::GetPathSeparator() + _T("locale") );
#endif
			my_locale.AddCatalog(_T("DivFix++"));
		return true;
		}
	return false;
	}

bool DivFixppApp::ReCreateGui(){
	SetLanguage();
	DivFixpp* frame = new DivFixpp( my_locale );
	SetTopWindow(frame);
	frame->Show(true);
	return true;
	}
