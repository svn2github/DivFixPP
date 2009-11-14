//-----------------------------------------------------------------------
//
// Name:        DivFix++Core2.cpp
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

#ifndef __DIVFIX_2CORE_H__
#define __DIVFIX_2CORE_H__

#include <wx/gauge.h>
#include <wx/ffile.h>
#include <wx/textctrl.h>
#include <wx/thread.h>
#include <wx/progdlg.h>
#include <wx/log.h>
#include <wx/filename.h>
#include <errno.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdint.h>

#include "endian.h"
#define MB 1024*1024
#define index_size 20*MB
#define buffer_size 1*MB
#define stream_limit 2
#ifndef wxUSE_UNICODE
	#define wxUSE_UNICODE 1
#endif

/*
typedef struct _AVIHeader{
    unsigned TimeBetweenFrames;     // Time delay between frames
    unsigned MaximumDataRate;       // Data rate of AVI data
    unsigned PaddingGranularity;    // Size of single unit of padding
    unsigned Flags;                 // Data parameters
    unsigned TotalNumberOfFrames;   // Number of video frame stored
    unsigned NumberOfInitialFrames; // Number of preview frames
    unsigned NumberOfStreams;       // Number of data streams in chunk
    unsigned SuggestedBufferSize;   // Minimum playback buffer size
    unsigned Width;                 // Width of video frame in pixels
    unsigned Height;                // Height of video frame in pixels
    unsigned TimeScale;             // Unit used to measure time
    unsigned DataRate;              // Data rate of playback
    unsigned StartTime;             // Starting time of AVI data
    unsigned DataLength;            // Size of AVI data chunk
} AVIHEADER;
typedef struct _StreamHeader{
    char  DataType[4];               // Chunk identifier ("strl")
    char  DataHandler[4];            // Device handler identifier
    unsigned Flags;                 // Data parameters
    unsigned Priority;              // Set to 0
    unsigned InitialFrames;         // Number of initial audio frames
    unsigned TimeScale;             // Unit used to measure time
    unsigned DataRate;              // Data rate of playback
    unsigned StartTime;             // Starting time of AVI data
    unsigned DataLength;            // Size of AVI data chunk
    unsigned SuggestedBufferSize;   // Minimum playback buffer size
    unsigned Quality;               // Sample quailty factor
    unsigned SampleSize;            // Size of the sample in bytes
} STREAMHEADER;
typedef struct tagBITMAPINFOHEADER {
  unsigned biSize;
  long  biWidth;
  long  biHeight;
  short  biPlanes;
  short  biBitCount;
  unsigned biCompression;
  unsigned biSizeImage;
  long  biXPelsPerMeter;
  long  biYPelsPerMeter;
  unsigned biClrUsed;
  unsigned biClrImportant;
}BITMAPINFOHEADER, *PBITMAPINFOHEADER;
*/

WX_DECLARE_STRING_HASH_MAP( int, ChunkID );

class binaryElement{
	public:
		binaryElement( ){};
		binaryElement( wxString _name );
		wxString name;
		wxMemoryBuffer data;
		void print( int level=0, bool endline=true );
		wxMemoryBuffer GetAsBuffer( void );
		unsigned size( void );
		std::vector<binaryElement*> get( wxString chunk_name );
		std::vector<binaryElement> binvector;
	};

struct FrameProp{
	uint8_t StreamNo;
	char StreamType[2];
	bool KeyFrame;
	uint32_t DataSize; //Dont covers 00dcSIZE, +8 for AVI frame size
	uint64_t Position;
	uint32_t FrameCount;
	wxString ChunkHead(){
		wxString ret = wxString::Format( wxT("%02X"), StreamNo );	//00
		ret.Append( StreamType[0] );	//00d
		ret.Append( StreamType[1] );	//00dc
		return ret;
		}
	};

class DivFixp2Core{
	protected:
		wxTextCtrl *WxMemoLog;
		wxGauge    *WxGauge;
		wxProgressDialog *WxProgress;
	private:
		void init();
		void MemoLogWriter( wxString, bool Error=false );
		wxString GetAVIFourCC( wxFFile* );
		bool IsAVI( wxFFile* );
		uint64_t Recover( wxFFile*,wxFFile*, unsigned, unsigned, unsigned, std::vector<FrameProp>&, wxString four_cc );
		bool IsFrame( const char *data );
		bool IsKeyFrameData( const char *data, unsigned size, const wxString& four_cc, unsigned FrameNumber );
		bool AddFrame( wxFFile *output, uint64_t& write_position, wxMemoryBuffer& buff, unsigned optype, std::vector<FrameProp>& frame_index, wxString four_cc);
		FrameProp FrameAnalyzer( wxMemoryBuffer& buff, uint64_t write_position, std::vector<FrameProp>& ,wxString four_cc );
		wxMemoryBuffer Create_OldIndex( std::vector<FrameProp>& , uint64_t movi_position );
		wxMemoryBuffer Create_StandartIndex( std::vector<FrameProp>& , unsigned streamno, uint64_t movi_position );
		bool UpdateService( unsigned short percent );
		bool AVIHeaderRepair( wxFFile* output, unsigned idx1_location, std::vector<FrameProp>& frame_index );
		bool Index1Repair( wxFFile* output, unsigned movi_position, unsigned idx1_location, std::vector<FrameProp>& frame_index );
		binaryElement LISTparser( wxMemoryBuffer buff );
		int SearchFrame( wxMemoryBuffer& buff );
		bool Truncate( wxString filename, unsigned cut_here );

	public:
		DivFixp2Core( );
		DivFixp2Core( wxProgressDialog *WxProgress);
		DivFixp2Core( wxGauge *WxGauge, wxTextCtrl* WxMemoLog );
		virtual ~DivFixp2Core();
		enum OperType { OverWrite = 0x01 /*not KeepOriginal*/, CutOut = 0x02, Error_Check=0x04, KeyFrameStart = 0x08,
						OldIndex = 0x10, StdIndex=0x20 };
		enum types_of_stream { UNKNOWN=0, AVI=1, MKV=2 };
		int IdentifyStreamType( wxString Source );
		bool Repair( wxString infile, wxString outfile, int optype = CutOut|KeyFrameStart|OldIndex);
		bool Strip( wxString name);
		bool HasAVIGotProperIndex( wxString input );
	};
#endif
