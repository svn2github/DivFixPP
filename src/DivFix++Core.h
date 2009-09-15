//-----------------------------------------------------------------------
//
// Name:        DivFix++Core.cpp
// Author:      Death Knight
// Description: DivFix++ CORE Library Header
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

#ifndef __DIVFIX_CORE_H__
#define __DIVFIX_CORE_H__

#include <wx/gauge.h>
#include <wx/ffile.h>
#include <wx/textctrl.h>
#include <wx/thread.h>
#include <wx/progdlg.h>
#include <wx/log.h>
#include <wx/filename.h>
#include <errno.h>
#include <iostream>

#include "endian.h"
#define MB 1024*1024
#define index_size 20*MB
#define buffer_size 1*MB
#define stream_limit 2
#ifndef wxUSE_UNICODE
	#define wxUSE_UNICODE 1
#endif

class DivFixppCore{
	protected:
		wxTextCtrl *WxMemoLog;
		wxGauge    *WxGauge;
		wxProgressDialog *WxProgress;

	private:
		void DivFix_initialize( void );
		void MemoLogWriter( wxString, bool Error=false );		// Code cleaning with MutexLock
		bool frame_copy( unsigned position,
						bool KeepOrginal, bool CutOut, bool Error_Check = false);
		bool is_frame( const char *buff, bool keyframe = false );
		bool is_keyframe( const char *buff );
		bool is_keyflag( const char *buff );
		int search_frame( char *bfr, int bfrsize, bool keyframe );
		bool junk_padding( unsigned int pad_to, bool force = false );
		bool avi_header_fix( void );
		bool update_gauge( int percent );
		bool LIST_parser( char* bfr, int lenght, int base );
		void JUNK_parser( const char* bfr, int lenght);
		void INFO_parser( const char* bfr, int lenght);
		void close_files( bool delete_output=false );
		bool Truncate(wxString, unsigned int);
		wxFFile *input,*output,*tempout;
		struct _avih{
			char *avi_header;
			int position;
			} avih;
		struct _strh{
			char *stream_header;
			int position;
			} strh[stream_limit];
		struct _odmlh{
			char *odml_header;
			int position;
			int size;
			} odmlh;
		char *buffer,*index_list_ptr,*index_list;
		unsigned long long read_position, write_position;
		unsigned stream_start;
		int end;
		int frame_counter[2];
		char frame_type[2][2];
		char four_cc[5];
		bool KeepOrg;
		wxString target_file;
	public:
		DivFixppCore( );
		DivFixppCore( wxProgressDialog *WxProgress);
		DivFixppCore( wxGauge *WxGauge, wxTextCtrl* WxMemoLog );
		virtual ~DivFixppCore();
		bool Fix( wxString Source, wxString Target,
					bool KeepOrginal=true,
					bool CutOut=true,
					bool Error_Check=false,
					bool KeyFrameStart = true );
		bool Strip( wxString File );
		bool HasAVIGotProperIndex( wxString Source );
		enum types_of_stream { UNKNOWN=0, AVI=1, MKV=2 };
		int IdentifyStreamType( wxString Source );
	};
#endif
