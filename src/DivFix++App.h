//-----------------------------------------------------------------------
//
// Name:        DivFix++App.h
// Author:      Death Knight
// Created:     26.05.2006 02:24:47
// Description: DivFix++ Main Application Header
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

#ifndef __DIVFIXPPAPP_H__
#define __DIVFIXPPAPP_H__

//#define _project_Meteorite_

#define _VERSION_STRING_ "Development Version"
#define _VERSION_ "0.32-svn"

//#if defined(__LINUX__)
#if defined(__WXGTK__)
	#define _OS_ " for Linux"
//#elif defined (__UNIX__)
//	#define _OS_ " for Unix"
#elif defined(__WXMSW__)
	#define _OS_ " for Windows"
#elif defined (__WXMAC__)
	#define _OS_ " for Mac"
#else
	#define _OS_ ""
#endif

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#else
	#include <wx/wxprec.h>
#endif
#ifndef wxUSE_UNICODE
#define wxUSE_UNICODE 1
#endif
#include "DivFix++Core.h"
#include <wx/cmdline.h>   // for Command Line parsing tools
#include <wx/snglinst.h>  // for Single Instance
#include <wx/intl.h>

class DivFixppApp : public wxApp{
	public:
		bool SelectLanguage();
		void RecreateFrame();
		bool OnInit();
		void OnInitCmdLine( wxCmdLineParser& parser );
		bool OnCmdLineError(wxCmdLineParser& parser);
		wxString argv0;
		bool ReCreateGui();
		bool SetLanguage();

protected:
		bool OnCmdLineParsed(wxCmdLineParser& parser);
		wxLocale my_locale;
};

#endif
