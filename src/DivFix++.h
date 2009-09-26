//-----------------------------------------------------------------------
//
// Name:        DivFix++.h
// Author:      Death Knight
// Created:     26.05.2006 02:24:47
// Description: DivFix++ GUI Header
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
*               email : spamjunkeater at gmail.com                      *
*************************************************************************/

#ifndef __DIVFIXPP_H__
#define __DIVFIXPP_H__

#include "DivFix++App.h" // For include _VERSION_ definition

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/frame.h>
#else
	#include <wx/wxprec.h>
#endif

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
#include <wx/dirdlg.h>
#include <wx/filedlg.h>
////Header Include End
#include <wx/file.h>
#include <wx/dir.h>
#include <wx/filedlg.h>
#include <wx/filefn.h>
#include <wx/filename.h>
#include <wx/dnd.h>
#include <wx/config.h>
#include <wx/aboutdlg.h>
#include <wx/url.h>
#include <wx/artprov.h>
#include <wx/datetime.h>

#include "DivFix++Core.h"
#include "DivFix++Gui.h"
#include "../resources/DivFixpp.xpm"
#ifndef wxUSE_UNICODE
#define wxUSE_UNICODE 1
#endif
class DivFixpp : public DivFixpp_Gui, wxThreadHelper, DivFixppCore
{
	public:
		DivFixpp(wxLocale& my_locale, wxWindow *parent = NULL, wxWindowID id = 1);
		virtual ~DivFixpp();
		virtual void *Entry();
		virtual void OnExit();
//		virtual bool OnDropFiles(wxCoord x, wxCoord y,  const wxArrayString& filenames);
		class DnDFile *DnD;
		void Disabler( void );
		void Enabler( void );
		friend class PreferencesDialog;

	private:
		enum states { fix, pause, resume } state;

		wxSingleInstanceChecker *single_inst_checker;
		wxLocale& m_locale;
		wxArrayString LangNames;
		wxArrayLong LangIds;

		bool ErrorCheckMode;

		void DivFixppClose(wxCloseEvent& event);
		void CreateGUIControls(void);
		void OnAddClick(wxCommandEvent& event);
		void OnRemoveClick(wxCommandEvent& event);
		void OnClearClick(wxCommandEvent& event);
		void OnFixClick(wxCommandEvent& event);
		void OnStripClick(wxCommandEvent& event);
		void OnCheckClick(wxCommandEvent& event);
		void OnStopClick(wxCommandEvent& event);
		void OnPathClick(wxCommandEvent& event);
		void OnCheck_RelativeOutputFile(wxCommandEvent& event);
		void OnCheck_KeepOriginal(wxCommandEvent& event);
		void OnCheck_SaveLog(wxCommandEvent& event);
		void OnAboutClick(wxCommandEvent& event);
		void OnPreferencesClick(wxCommandEvent& event);
};

class PreferencesDialog : public PreferencesDialog_Gui{
		PreferencesDialog(wxLocale& my_locale, wxWindow *parent = NULL, wxWindowID id = 1);
		friend class DivFixpp;
	private:
		wxLocale& m_locale;
		wxArrayString LangNames;
		wxArrayLong LangIds;
		void GetInstalledLanguages( wxArrayString & names, wxArrayLong & identifiers );
		void OnPlayerClick( wxCommandEvent& event );
		void OnClose( wxCloseEvent& event );
		void OnCheckNewVersion( wxCommandEvent& event );
	};

class DnDFile : public wxFileDropTarget{
	public:
		DnDFile(wxListBox *listbox):m_listbox( listbox ){}

	virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames){
		int nFiles = filenames.GetCount();
		for ( int n = 0; n < nFiles; n++ ) {
//			wxLogError( wxMBConv::cWX2WC( filenames.Item(n).c_str() ) ) ;
			if ( wxFile::Exists( filenames.Item(n).c_str() )){
				if  ( m_listbox->GetString( m_listbox->FindString( filenames.Item(n) ) ) != filenames.Item(n) )
                m_listbox->Append(filenames.Item(n));
				}
           }
		return TRUE;
		}
	private:
		wxListBox *m_listbox;
	};

class VersionChecker : public UpdateDialog_Gui {
	public:
		VersionChecker( wxString _url, wxString current_version, wxWindow *parent = NULL, wxWindowID id = 1  );
		void OnChkDisplay( wxCommandEvent& event );
	};
#endif
