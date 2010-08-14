//-----------------------------------------------------------------------
//
// Name:        DivFix++Core2.cpp
// Author:      Death Knight
// Description: DivFix++ CORE Library Code
//
//-----------------------------------------------------------------------
/***********************************(GPL)********************************
*   DivFix++ is AVI Video Repair & Preview Utility.                     *
*   Copyright (C) 2009  Erdem U. Altinyurt                              *
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

#include "DivFix+2Core.h"
#include "DivFix++App.h"
#ifdef _project_Meteorite_
	#include "meteorite.h"
#endif
#include <omp.h>

int atoh(const char hex){
	return ( hex >= '0' and hex <= '9' ) ? hex -'0' :
			( hex >= 'a' and hex <= 'f' ) ? hex -'a' + 10:
			( hex >= 'A' and hex <= 'F' ) ? hex -'A' + 10:
			-1;
	}
int atoh2( const char a[2] ){
	unsigned H = atoh( a[0] );
	unsigned L = atoh( a[1] );
	if( H >= 0 and L >= 0)
		return H*16 + L;
	else
		return -1;
	}

using namespace std;

ChunkID acl;//int array wx

void init_acl(void){
	acl[ wxT("hdrl") ] = 1;	//type 1 means chunk has no 4 byte size after token
	acl[ wxT("strl") ] = 1;
	acl[ wxT("odml") ] = 1;
	acl[ wxT("avih") ] = 2;	//type 2 means chunk has 4 byte size after token
	acl[ wxT("strh") ] = 2;
	acl[ wxT("strn") ] = 2;
	acl[ wxT("dmlh") ] = 2;
	acl[ wxT("strf") ] = 2;
	acl[ wxT("segm") ] = 2;
	acl[ wxT("vedt") ] = 2;
	acl[ wxT("vprp") ] = 2;
	acl[ wxT("RIFF") ] = 2;
	acl[ wxT("AVIX") ] = 1;
	acl[ wxT("AVI ") ] = 1;
	acl[ wxT("JUNK") ] = 2;
	acl[ wxT("LIST") ] = 2;
	acl[ wxT("movi") ] = 1;
	acl[ wxT("idx1") ] = 2;
	acl[ wxT("indx") ] = 2;
	acl[ wxT("INFO") ] = 1;
	///Standard RIFF INFO tags:
	acl[ wxT("IARL") ] = 3;// Archival Location /// type 3 means same as 2 but flan identify that is INFO flag
	acl[ wxT("IART") ] = 3;// Artist, Director
	acl[ wxT("ICMS") ] = 3;// Commissioned by
	acl[ wxT("ICMT") ] = 3;// Comments
	acl[ wxT("ICOP") ] = 3;// Copyright
	acl[ wxT("ICRD") ] = 3;// Creation Date
	acl[ wxT("IDIM") ] = 3;// Dimensions
	acl[ wxT("IDPI") ] = 3;// Dots Per Inch
	acl[ wxT("IENG") ] = 3;// Engineer, Digitized by
	acl[ wxT("IGNR") ] = 3;// Genre
	acl[ wxT("IKEY") ] = 3;// Keywords
	acl[ wxT("ILGT") ] = 3;// Lightness
	acl[ wxT("IMED") ] = 3;// Medium
	acl[ wxT("INAM") ] = 3;// Name, title
	acl[ wxT("IPLT") ] = 3;// Palette Setting
	acl[ wxT("IPRD") ] = 3;// Product
	acl[ wxT("ISBJ") ] = 3;// Subject
	acl[ wxT("ISFT") ] = 3;// Software
	acl[ wxT("ISHP") ] = 3;// Sharpness
	acl[ wxT("ISRC") ] = 3;// Source
	acl[ wxT("ISRF") ] = 3;// Source Form
	acl[ wxT("ITCH") ] = 3;// Technician, Encoded by
	acl[ wxT("IDIT") ] = 3;// Digitizing Date
	acl[ wxT("ISMP") ] = 3;// SMPTE time code
	acl[ wxT("ICRP") ] = 3;// Cropped

///Extended RIFF INFO tags:
	acl[ wxT("ISGN") ] = 3;// Secondary Genre
	acl[ wxT("IWRI") ] = 3;// Written by
	acl[ wxT("IPRO") ] = 3;// Produced by
	acl[ wxT("ICNM") ] = 3;// Cinematographer
	acl[ wxT("IPDS") ] = 3;// Production Designer
	acl[ wxT("IEDT") ] = 3;// Edited by
	acl[ wxT("ICDS") ] = 3;// Costume Designer
	acl[ wxT("IMUS") ] = 3;// Music by
	acl[ wxT("ISTD") ] = 3;// Production Studio
	acl[ wxT("IDST") ] = 3;// Distributed by
	acl[ wxT("ICNT") ] = 3;// Country
	acl[ wxT("ILNG") ] = 3;// Language
	acl[ wxT("IRTD") ] = 3;// Rating
	acl[ wxT("ISTR") ] = 3;// Starring
	acl[ wxT("IWEB") ] = 3;// Internet Address
	acl[ wxT("IPRT") ] = 3;// Part
	acl[ wxT("IFRM") ] = 3;// Total Number of Parts
	acl[ wxT("IAS1") ] = 3;// First language
	acl[ wxT("IAS2") ] = 3;// Second language
	acl[ wxT("IAS3") ] = 3;// Third language
	acl[ wxT("IAS4") ] = 3;// Fourth language
	acl[ wxT("IAS5") ] = 3;// Fifth language
	acl[ wxT("IAS6") ] = 3;// Sixth language
	acl[ wxT("IAS7") ] = 3;// Seventh language
	acl[ wxT("IAS8") ] = 3;// Eighth language
	acl[ wxT("IAS9") ] = 3;// Ninth language
	acl[ wxT("ICAS") ] = 3;// Default audio stream
	acl[ wxT("IBSU") ] = 3;// Base URL
	acl[ wxT("ILGU") ] = 3;// Logo URL
	acl[ wxT("ILIU") ] = 3;// Logo Icon URL
	acl[ wxT("IWMU") ] = 3;// Watermark URL
	acl[ wxT("IMIU") ] = 3;// More Info URL
	acl[ wxT("IMBI") ] = 3;// More Info Banner Image
	acl[ wxT("IMBU") ] = 3;// More Info Banner URL
	acl[ wxT("IMIT") ] = 3;// More Info Text
	///Other supported RIFF INFO tags (only reading, saved as standard tags):
	acl[ wxT("IENC") ] = 3;// Encoded by (=ITCH), used by GSpot
	acl[ wxT("IRIP") ] = 3;// Ripped by (=IENG), used by GSpot
//	acl[ wxT("") ] = 3;//
/*
IDivX tag (reading and updating):
Movie Name - 32 bytes
Author (director) - 28 bytes
Year - 4 bytes
Comment - 48 bytes;
Genre - 3 bytes;
Rating - 1 byte;
*/
	}

void DivFixp2Core::init(){
	WxGauge = NULL;
	WxProgress = NULL;
	WxMemoLog = NULL;// new wxTextCtrl;
	init_acl();	//initializes AVI Chunk List for first use
	}
DivFixp2Core::DivFixp2Core(){
	init();
	}
DivFixp2Core::DivFixp2Core( wxProgressDialog *prgs ){
	init();
	WxProgress = prgs;
	}
DivFixp2Core::DivFixp2Core( wxGauge *wxgg, wxTextCtrl *wxml ){
	init();
	delete WxMemoLog;
	WxGauge = wxgg;
	WxMemoLog = wxml;
	}
DivFixp2Core::~DivFixp2Core(){
	}
bool DivFixp2Core::UpdateService( unsigned short percent ){
	if(WxGauge){
		if( WxGauge->GetValue() != percent ){		//Checks value is changed or not
			wxMutexGuiEnter();
			WxGauge->SetValue( percent );
			wxYield();
			wxMutexGuiLeave();
			}
		}
	else if( WxProgress ){
		if(! WxProgress->Update( percent ) )	//Progress Dialog update
			return false;
		}
	else{
//		wxString value;
//		wxGetEnv( wxT("COLUMNS"), &value);
//		std::cout << "\r" << value.ToAscii() << " "  << target_file.ToAscii() << "\t\%" << percent;
		}

	if( !wxThread::This()->IsMain() )				//Checks if functions is running on thread
		if( wxThread::This()->TestDestroy() )		//Checks if thread has termination request
			return false;
	return true;
	}

bool DivFixp2Core::Repair( wxString Source, wxString Destination, int optype ){
	MemoLogWriter( _("Processing file : "));
	MemoLogWriter( Source.AfterLast(wxFileName::GetPathSeparator())+ _T("\n"));

	wxFFile *input, *output;
	input = new wxFFile();
	output= new wxFFile();

	if( Error_Check & optype ){
		if(not input->Open( Source, _T("rb") )){
			MemoLogWriter( wxString(_("Error: "))+_("Input file cannot be opened!\n"),true );
			return false;
			}
		}
	else if( OverWrite & optype ){
		if(not input->Open( Source, _T("ab+"))){
			MemoLogWriter( wxString(_("Error: "))+_("Input file cannot be opened!\n"),true );
			return false;
			}
		}
	else{
		if(not input->Open( Source, _T("rb") )){
			MemoLogWriter( wxString(_("Error: "))+_("Input file cannot be opened!\n"),true );
			return false;
			}

		if(not IsAVI( input ) ){
			MemoLogWriter( wxString(_("Error: "))+_("Input is not an AVI file\n"),true );
			input->Close();
			return false;
			}

		if( not output->Open( Destination, _T("wb+") )){
			MemoLogWriter( wxString(_("Error: "))+_("Output file cannot opened!\n"),true );
			input->Close();
			return false;
			}
		}

	wxString four_cc = GetAVIFourCC( input );
	MemoLogWriter(  _("Four CC is : ") + four_cc + _T("\n") );

	//Detecting first movi chunk start
	wxMemoryBuffer buff;
	for(int movi_position = 0,jump = 16 ; strncmp(buff,"movi",4) ;movi_position += 8 ){
		movi_position += jump;
		input->Seek( movi_position, wxFromStart );
		input->Read( reinterpret_cast<char*>(&jump), 4 );
		jump = make_littleendian(jump);
		buff.UngetWriteBuf( input->Read( buff.GetWriteBuf(4) , 4 ) );
		if( input->Eof() ){
			MemoLogWriter(_("Input's movi section not found!\n"),true);
			input->Close();
			output->Close();
			wxRemove( Destination );
			return false;
			}
		}
	int movi_position = input->Tell()-4;

	//Copying old Header to our new fixed file.
	input->Seek(0, wxFromStart);
	buff.UngetWriteBuf( input->Read( buff.GetWriteBuf(movi_position+4) , movi_position+4 ) );
// TODO (death#1#): try/catch
	if( not( OverWrite & optype ) and not( Error_Check & optype )){
		output->Seek( 0, wxFromStart);
		output->Write( buff, movi_position+4);
		}

	//init frame copy engine variables
	unsigned int movi_size;
	memcpy(reinterpret_cast<char*>(&movi_size), buff+movi_position-4, 4);	//LIST****movi
	movi_size = make_littleendian(movi_size);

	vector< FrameProp > frame_index;
	uint64_t write_position = Recover( input, output, movi_position, movi_size, optype, frame_index, four_cc );
	if( write_position == 0 ){
		if( not (OverWrite & optype )){
			input->Close();
			output->Close();
			wxRemoveFile( Destination );
			return false;
			}
		}

	// TODO (death#1#): Multi RIFF AVIX and inter-movi chunk like idx1 detection code here
	uint64_t filesize = input->Length();
	uint64_t read_position = movi_size + movi_position;
	char buffer[32];
	while( read_position < filesize and not input->Eof()){//Maın loop...
		if(read_position%2)
			read_position++;	// To frame start at even byte

		input->Seek( read_position, wxFromStart );
		input->Read( buffer, 32);
		char nm[5]={0,0,0,0,0};
		strncpy( nm, buffer, 4);
		wxString name = wxString::FromAscii( nm );	//readed potantial chunk name
		int query = acl[ name ];
		if( query== 0 ){	//not known chunk name
			read_position++;
			}

		else if( query == 1 ){
			unsigned chunk_size=0;
			if( name == wxT("AVIX"))
				MemoLogWriter(wxString(_("Info: ")) << _("New AVIX stream located at " ) << read_position << wxT("\n"));
			else if( name == wxT("movi")){
				MemoLogWriter(wxString(_("Info: ")) << _("New movi chunk located at " ) << read_position << wxT("\n"));
				input->Seek( read_position-4, wxFromStart );
				input->Read( buffer, 4);

				memcpy(reinterpret_cast<char*>(&chunk_size), buffer, 4);	//read index size
				chunk_size = make_littleendian(chunk_size);
				// TODO (death#1#): Need to identify headering type
				//vector< FrameProp > frame_index1;
				// TODO (death#1#): Need to Inject all streams last header for proper frame number indication.
				write_position = Recover( input, output, read_position, chunk_size, optype, frame_index, four_cc );
				if( write_position == 0 ){
					if( not (OverWrite & optype )){
						input->Close();
						output->Close();
						wxRemoveFile( Destination );
						return false;
						}
					}
				}
			else{
				MemoLogWriter(wxString(_("Error: ")) << _("Unexpected chunk : " ) << name  << wxT("\n") << _(" Report this bug to Author from http://divfixpp.sourceforge.net") << wxT("\n"));
				return false;
				}
			read_position += chunk_size + 4;//chunk_name
			}

		else if( query >= 2 ){
			unsigned chunk_size = 0;
			if( name == wxT("idx1") ){
				MemoLogWriter(wxString(_("Info: ")) << _("Original index chunk found at " ) << read_position << wxT("\n"));
				memcpy(reinterpret_cast<char*>(&chunk_size), buffer+4, 4);	//read index size
				chunk_size = make_littleendian(chunk_size);
				}
			else if( name == wxT("RIFF") )
				MemoLogWriter(wxString(_("Info: ")) << _("Another RIFF part located at " ) << read_position << wxT("\n"));
			else if( name == wxT("LIST"))
				MemoLogWriter(wxString(_("Info: ")) << _("New LIST stream located at " ) << read_position << wxT("\n"));
			else{
				memcpy(reinterpret_cast<char*>(&chunk_size), buffer+4, 4);	//read index size
				chunk_size = make_littleendian(chunk_size);
				}
			read_position += chunk_size + 8; // 8 for chunk_name + size
			}

		if(read_position%2)
			read_position++;	// To frame start at even byte
		}



	if( Error_Check & optype ){
		// TODO (death#1#): index check algorithm		}
	else{
		Index1Repair( output, movi_position, write_position, frame_index );
		AVIHeaderRepair( output, write_position, frame_index );
		}

	unsigned RIFFSize = output->Length() - 8;	//-8 for RIFFSIZE
	output->Seek( 4, wxFromStart );
	output->Write( reinterpret_cast<char*>(&make_littleendian( RIFFSize )), 4 );

	input->Close();
	output->Close();
	return true;
	}

uint64_t DivFixp2Core::Recover( wxFFile *input, wxFFile *output, unsigned movi_position, unsigned movi_size, unsigned optype, vector<FrameProp>& frame_index, wxString four_cc ){
	uint64_t read_position = movi_position+4;
	uint64_t write_position = movi_position+4;
	wxMemoryBuffer buff;
	wxString tmp;

	//Until movi_size, proces each frame 1-1
	uint64_t read_upto = movi_size + movi_position;
	uint64_t filesize = input->Length();
	while( read_position < read_upto and not input->Eof() ){
		if( not UpdateService( read_position*100/filesize ) )
			return 0;

		if(read_position%2){
			read_position++;	// To frame start at even byte
			}
		input->Seek( read_position, wxFromStart );
		buff.UngetWriteBuf( input->Read( buff.GetWriteBuf(32), 32 ));
		if( IsFrame(buff) ){
			int frame_size;
			memcpy( reinterpret_cast<void*>( &frame_size ), buff+4, 4 );
			frame_size = make_littleendian( frame_size );
			if( frame_size < 0 ){
				MemoLogWriter(_("Warning: Negative frame size detected!\n"));
				read_position++;
				if(read_position%2)
					read_position++;	// To frame start at even byte
				continue;
				}
			else if( frame_size == 0 ){
				MemoLogWriter(wxString(_("Info: ")) << _("Zero length frame detected at " ) << read_position << wxT("\n"));
				//read_position++;
				//continue;
				}

			if( IsFrame( buff+16 ) and
				(frame_size not_eq 8) and
				(frame_size not_eq 7) and	//+1 for padding to even byte
				(frame_size not_eq 0) ){	//0 check for sequal 00dc0x00 stream
//				input->Seek( read_position, wxFromStart );
//				buff.UngetWriteBuf( input->Read( buff.GetWriteBuf(16*5), 16*5 ));	//check 5 frame for identify index
//				bool idx = true;
//				for( unsigned i = 0 ; i+16 < buff.GetDataLen() ; i+=16 )
//					idx and_eq IsFrame( buff+i );
//				if( idx and (four_cc not_eq wxT("MJPG")) ){
					MemoLogWriter(wxString(_("Info: ")) << _("Broken Index frames detected at " ) << read_position << wxT("\n"));
					read_position += 16;
					continue;
//					}
				}

			if( frame_size > 1*MB ){
				MemoLogWriter(wxString(_("Warning: ")) << _("Frame size bigger than MB! (too big) Dropped.") << wxT("\n"));
				read_position++;
				continue;
				}
			input->Seek( read_position, wxFromStart );
			buff.UngetWriteBuf( input->Read( buff.GetWriteBuf(frame_size+8), frame_size+8 ));	//+8 for frame type + size
			read_position = input->Tell();
			//buffer has frame now. Process it.
			AddFrame( output, write_position, buff, optype, frame_index, four_cc );
			}
		else if( !strncmp( buff, "LIST", 4)){
			//MemoLogWriter(wxString(_("Info: "))+_("LIST chunk in movi section at " ) << read_position << wxT("\n"));
			read_position += 8;
			}
		else if( !strncmp( buff, "rec ", 4) ){
			read_position += 4;
			}
		else if( !strncmp( buff, "JUNK", 4)
				){
			int chunk_size;
			memcpy(reinterpret_cast<char*>(&chunk_size), buff+4, 4);
			chunk_size= make_littleendian( chunk_size );
			read_position += 8 + chunk_size;
			}
		else if( !strncmp(buff, "idx1", 4) ){
			MemoLogWriter(wxString(_("Info: "))+_("Original index chunk found at " ) << read_position << wxT("\n"));
			int idx_size = 0;
			memcpy(reinterpret_cast<char*>(&idx_size), buff+4, 4);	//read index size
			idx_size = make_littleendian(idx_size);
			read_position += idx_size + 8; // 8 for idx1 + size
//AVIXXX

//			input->Seek( read_position, wxFromStart );
//			input->Read( buff, 24 );
//			if( !strncmp(buff, "RIFF", 4) && !strncmp(buff+8, "AVIX", 4) &&
//				!strncmp(buff+12, "LIST", 4) && !strncmp(buff+20, "movi", 4) ){
//					read_position += 24;
//					return frame_copy( read_position, KeepOriginalFile, CutOutBadParts, Error_Check_Mode);
//					}
//			return true;
			}
		else if( not strncmp(buff,"ix", 2) or not strncmp(buff+2,"ix", 2)){
			uint16_t streamnumber;
			if( not strncmp(buff,"ix", 2) )
				streamnumber = atoh2(buff+2);
			else
				streamnumber = atoh2(buff);

			MemoLogWriter( _("Info: ") );

			uint16_t ix_type;
			memcpy(reinterpret_cast<char*>(&ix_type), buff+10, 2);
			ix_type = make_littleendian(ix_type);
			if( ix_type == 0x0100)
				MemoLogWriter(_("Standard index chunk"));
			else if( ix_type == 0x0101)
				MemoLogWriter(_("AVI Field index chunk"));
			else if( ix_type == 0x0000 or ix_type == 0x0001 )
				MemoLogWriter(_("Super index chunk"));
			else
				MemoLogWriter(_("Unknown type index chunk"));

			MemoLogWriter( wxString::Format(_(" for stream %d found at %d"), streamnumber, read_position) << wxT("\n"));
			int ix_size = 0;
			memcpy(reinterpret_cast<char*>(&ix_size), buff+4, 4);
			ix_size = make_littleendian(ix_size);
			//Read ix and store to file
			input->Seek( read_position, wxFromStart );
			buff.UngetWriteBuf( input->Read( buff.GetWriteBuf(ix_size+8), ix_size+8 ) );
			static int f = 0;
	#ifdef __DEBUG__
			wxString flnm = wxT("ixir-");
			wxFFile ixir( (flnm << ++f), wxT("wb") );
			ixir.Write( buff, buff.GetDataLen() );
			ixir.Close();
	#endif
			//
			read_position += ix_size + 8; // 8 for ix00/ix01 + size
	//		return frame_copy( read_position, KeepOriginalFile, CutOutBadParts, Error_Check_Mode); //Not necessary and req eof() check here
	//		return true; //remove error warning and make eof() check

			}
		else{
			MemoLogWriter(wxString(_("Error: ")) << _(" File broken on byte : " ) << read_position << wxT("\n"));
			while(read_position < read_upto and not input->Eof()){	//Skip bytes untill frame detection.
				input->Seek( read_position, wxFromStart );
				buff.UngetWriteBuf( input->Read( buff.GetWriteBuf(1*MB), 1*MB ));
				int jmp = SearchFrame( buff );
				if( jmp == -1 ){
					read_position += 1*MB-4;	//-4 for last 4 byte because this bytes are not scanned
					}
				else{	//means found some frames here
					read_position += jmp;
					if( KeyFrameStart & optype ){	//Recover from KeyFrames
						input->Seek( read_position, wxFromStart );
						buff.UngetWriteBuf( input->Read( buff.GetWriteBuf(1*MB), 1*MB ));
						uint32_t chunk_size = 0;
						memcpy(reinterpret_cast<char*>(&chunk_size), buff+4, 4);
						chunk_size = make_littleendian(chunk_size);
						if(buff[2]=='d')	//dc or db frame!
							if( IsKeyFrameData( buff+8, chunk_size, four_cc, 5 ) )//5 not important, different than 1
								break;
						read_position++;//= chunk_size;	//or read_position++ is better here because of false matchs?
						}
					else
						break;
					}
				if( not UpdateService( read_position*100/filesize ) )
					return 0;
				}
			}
		if(read_position%2)
			read_position++;	// To frame start at even byte
		}//End of Movi Size
	return write_position;
	}

int DivFixp2Core::SearchFrame( wxMemoryBuffer& buff ){	// Frame search algorithm.
// TODO (death#1#): Insert Multi-Thread OpenMP code and/or OpenCL binary search :)
	if(buff.GetDataLen() < 4)
		return -1;

	///OpenMP
//	unsigned ret = 0;
//	unsigned bfr_ptr = 0;
//	#pragma omp parallel for private(bfr_ptr)
//		for( bfr_ptr = 0; bfr_ptr < buff.GetDataLen() - 4 ; bfr_ptr +=1 ){
//			if( IsFrame( buff+bfr_ptr ) )			//If found frame
//				if( ret == 0 or ret > bfr_ptr )		//and there is no frame found untill than
//					ret = bfr_ptr;					//update location.
//		}
//	return ( ret ? ret : -1)

	///No OpenMP
	for(unsigned bfr_ptr = 0; bfr_ptr < buff.GetDataLen() - 4 ; bfr_ptr +=1 ){
		if( IsFrame( buff+bfr_ptr ) )
			return bfr_ptr;
		}

	return -1;
	}

bool DivFixp2Core::Index1Repair( wxFFile* output, unsigned movi_position, unsigned idx1_location, vector<FrameProp>& frame_index ){
	if(idx1_location%2){					// Even Byte Padding
		output->Seek( idx1_location , wxFromStart);	// this fix that situations.
		output->Write( "\0", 1);
		idx1_location++;					// necessery to put frames to even positions
		}
	//Writing idx1
	wxMemoryBuffer idx1 = Create_OldIndex( frame_index, movi_position );
	output->Seek( idx1_location, wxFromStart );
	output->Write( idx1, idx1.GetDataLen() );
	//Repairing movi size for pointing idx1 location
	output->Seek( movi_position-4, wxFromStart );
	output->Write( reinterpret_cast< char*>( &make_littleendian( idx1_location - movi_position) ),4 );
	return true;
	}

bool DivFixp2Core::AVIHeaderRepair( wxFFile* output, unsigned idx1_location, vector<FrameProp>& frame_index ){		 // Updates/Fixes headers frame counts
	wxMemoryBuffer buff;
	for(int movi_position = 0,jump = 16 ; strncmp(buff,"movi",4) ;movi_position += 8 ){
		movi_position += jump;
		output->Seek( movi_position, wxFromStart );
		output->Read( reinterpret_cast<char*>(&jump), 4 );
		jump = make_littleendian(jump);
		buff.UngetWriteBuf( output->Read( buff.GetWriteBuf(4) , 4 ) );
		if( output->Eof() ){
			MemoLogWriter(_("Input's movi section not found!\n"),true);
			output->Close();
			return false;
			}
		}
	int movi_position = output->Tell()-4;

	//Extracting AVI header from input AVI
	unsigned chunk_size = movi_position - 8 - 12;// Read chunk from first LIST to LAST List Before Movi excluded
	output->Seek( 12, wxFromStart );

// TODO (death#1#): Add other lists till movi section!

	wxMemoryBuffer ListBuffer;
	ListBuffer.UngetWriteBuf( output->Read( ListBuffer.GetWriteBuf( chunk_size ), chunk_size ) );
	//Creating main Root and leafs via LISTparser
	binaryElement avi_header = LISTparser( ListBuffer );
	avi_header.name = wxT("ROOT");

	cout << endl;

	avi_header.print();
	cout << "Origin size : " << chunk_size << endl;
	cout << "Parsed size : " << avi_header.size() << endl;
	cout << "Buffer size : " << avi_header.GetAsBuffer().GetDataLen() << endl;

	//Calculating stream 0 frame count
	unsigned frame_count = 0;
	for ( vector<FrameProp>::reverse_iterator rit=frame_index.rbegin() ; rit < frame_index.rend() ; ++rit )
		if( rit->StreamNo == 0 ){
			frame_count = rit->FrameCount;
			break;
			}
	//Fixing Frame count for AVI Header
	vector<binaryElement*> element_ptr_vec = avi_header.get( wxT("avih") );	//Get avih header information
	if( element_ptr_vec.size() ){//if has items
		wxMemoryBuffer& avihdata = element_ptr_vec.at(0)->data;
		char *GWBuff = reinterpret_cast< char* >( avihdata.GetWriteBuf( 56 ) );
		memcpy( GWBuff+16,  &make_littleendian( frame_count ) , 4 );	//update avih 16->20 with frame count
		avihdata.UngetWriteBuf( 56 );
		}
	else
		return false;

	//Fixing Frame count for stream headers
	element_ptr_vec = avi_header.get( wxT("strh") );	//Get tracks information
	cout << "strh in vector : " << element_ptr_vec.size() << endl;
	if( element_ptr_vec.size() ){//if has items
		int trackno=0;
		for ( vector<binaryElement*>::iterator it=element_ptr_vec.begin() ; it < element_ptr_vec.end() ; ++it ){
			frame_count = 0;
			for ( vector<FrameProp>::reverse_iterator rit=frame_index.rbegin() ; rit < frame_index.rend() ; ++rit )
				if( rit->StreamNo == trackno ){
					frame_count = rit->FrameCount;
					break;
					}

			wxMemoryBuffer& strhdata = (*it)->data;
			char *GWBuff = reinterpret_cast< char* >( strhdata.GetWriteBuf( 56 ) );
			memcpy( GWBuff+32,  &make_littleendian( frame_count ) , 4 );	//update strh 32->36 with frame count
			strhdata.UngetWriteBuf( 56 );
			}
		}

	//Removing standart indexes from header
	element_ptr_vec = avi_header.get( wxT("strl") );
	if( element_ptr_vec.size() )	//if has items
		for ( vector<binaryElement*>::iterator it=element_ptr_vec.begin() ; it < element_ptr_vec.end() ; ++it )	//vector of strl
			for ( vector<binaryElement>::iterator jit= (*it)->binvector.begin() ; jit < (*it)->binvector.end() ; ++jit )
				if(jit->name == wxT("indx") )
					(*it)->binvector.erase( jit );


	//Removing standart indexes from header
	element_ptr_vec = avi_header.get( wxT("hdrl") );
	if( element_ptr_vec.size() )	//if has items
		for ( vector<binaryElement*>::iterator it=element_ptr_vec.begin() ; it < element_ptr_vec.end() ; ++it )
			for ( vector<binaryElement>::iterator jit= (*it)->binvector.begin() ; jit < (*it)->binvector.end() ; ++jit )
				if(jit->name == wxT("LIST") and jit->binvector.begin()->name == wxT("odml") )
					(*it)->binvector.erase( jit );

	// Needed to adjust total header size as its original!
	if( chunk_size != avi_header.GetAsBuffer().GetDataLen() ){
		int diff = (chunk_size - avi_header.GetAsBuffer().GetDataLen());
		element_ptr_vec = avi_header.get( wxT("JUNK") );	//Get tracks information
		if( element_ptr_vec.size() ){//if has items
			for ( vector<binaryElement*>::iterator it=element_ptr_vec.begin() ; it < element_ptr_vec.end() ; ++it ){
				wxMemoryBuffer& junkdata = (*it)->data;
				if( diff == 0)
					break;
				else if( diff > 0){	//need to get weight
					char *buf = new char[diff];
					for(int i = 0 ; i < diff ; i ++)
						buf[i]=0;
					junkdata.AppendData( &buf, diff );
					break;
					}
				else{	//need diet
					if( junkdata.GetDataLen() > abs( diff ))
						junkdata.SetDataLen( junkdata.GetDataLen()+diff );
					else
						continue;
					}
				}
			}
		}


	cout << "Fixed  size : " << avi_header.size() << endl;
	cout << "Buffer size : " << avi_header.GetAsBuffer().GetDataLen() << endl;

	//wxMemoryBuffer z = avi_header.GetAsBuffer();
	//wxFFile zw(wxT("./header.data"), wxT("wr"));
	//zw.Write( z, z.GetDataLen() );
	//zw.Close();

	output->Seek( 12, wxFromStart );
	output->Write( avi_header.GetAsBuffer(), chunk_size );
	binaryElement new_header = LISTparser( avi_header.GetAsBuffer() );
	new_header.print();

	return true;
	}

//Recursive!
binaryElement DivFixp2Core::LISTparser( wxMemoryBuffer bfr ){// Header LIST Parser
	binaryElement BE;
	unsigned chunk_size;
	unsigned bfr_ptr=0;

	///const char *buffer = bfr; //for debuging
	while( bfr_ptr < bfr.GetDataLen() ){
		char nm[5]={0,0,0,0,0};
		strncpy( nm, bfr+bfr_ptr, 4);
		wxString name = wxString::FromAscii( nm );
		bfr_ptr += bfr_ptr%2;	//Add 1 for make it even
		//Unified parsing code
		if( acl[ name ]==1 ){	//for tokens type 1
			bfr_ptr += 4; //chunk name
			wxMemoryBuffer tmp;
			tmp.AppendData( bfr+bfr_ptr, bfr.GetDataLen()-bfr_ptr );
			binaryElement chunk = LISTparser( tmp );
			chunk.name = name;
			BE.binvector.push_back( chunk );
			bfr_ptr = bfr.GetDataLen();
			}
		else if( acl[ name ] >= 2 ){	// for tokens type 2
// TODO (death#1#): inspection on vprp - vedt - segm
			bfr_ptr += 4; //chunk name
			memcpy(reinterpret_cast<char*>(&chunk_size), bfr+bfr_ptr, 4);
			chunk_size= make_littleendian( chunk_size );
			bfr_ptr += 4; //size

			if( chunk_size + bfr_ptr > bfr.GetDataLen() ){
				MemoLogWriter(wxString(_("Error: "))+ name + _(" Chunk bigger than buffer.\n"));
				return BE;
				}

			binaryElement chunk( name );
			if( name == wxT("LIST") ){
				wxMemoryBuffer tmp;
				tmp.AppendData( bfr+bfr_ptr, chunk_size );
				chunk = LISTparser( tmp );
				chunk.name =name;
				}
			else
				chunk.data.AppendData( bfr+bfr_ptr, chunk_size );

			BE.binvector.push_back( chunk );
			bfr_ptr += chunk_size;
			bfr_ptr += bfr_ptr%2;	//if bfr_ptr is odd, add 1 to make it even. Chunk modifiers only start at even bytes.
			}
		else if( !strncmp(bfr+bfr_ptr,"rec ",4)){
			bfr_ptr+=4;
			}
		else{
			MemoLogWriter(wxString(_("Error: "))+ _("Unknown chunk name: ") + name + wxT(". Stopped processing.\n"));
			break;
			}
		}
	return BE;
	}

//wxMemoryBuffer DivFixp2Core::Create_SuperIndex( vector<FrameProp>& frame_index, unsigned streamno, uint64_t movi_position ){
//	vector<FrameProp>::iterator it;
//	wxMemoryBuffer idx;
//	idx.AppendData( "ix" , 2);
//	idx.AppendData( wxString::Format(wxT("%02X"), streamno).ToAscii(), 2 );
//	StreamFrame
//	uint32_t tmp32 = frame_index.size_of_streamno( streamno )*8 + 00000;
//	idx.AppendData( reinterpret_cast< void*>(&make_littleendian( tmp32 )), 4 );
//
//	uint16_t tmp16 = 4;
//	idx.AppendData( reinterpret_cast< void*>(&make_littleendian( tmp16 )), 2 );
//
//	idx.AppendByte( '\0' );	//must be 0 or AVI_INDEX_2FIELD
//	idx.AppendByte( '\0' ); // must be AVI_INDEX_OF_INDEXES
//
//	tmp32 = frame_index.size_of_streamno( streamno );
//	idx.AppendData( reinterpret_cast< void*>(&make_littleendian( tmp32 )), 4 );
//
//	idx.AppendData( stream_name , 4 );
//
//	tmp32 = 0;
//	idx.AppendData( reinterpret_cast< void*>(&make_littleendian( tmp32 )), 4 );
//
//
//	int KeyFrameFlag = 0x0F; // AVIIF_KEYFRAME
//	for ( it=frame_index.begin() ; it < frame_index.end() ; ++it ){
//		}
//	return idx;
//	}
//
wxMemoryBuffer DivFixp2Core::Create_StandartIndex( vector<FrameProp>& frame_index, unsigned StreamNo, uint64_t movi_position ){
	//Format is : ix00 size(ix00)
	vector<FrameProp>::iterator it;
	wxMemoryBuffer idx;
	idx.AppendData( "ix" , 2);
	idx.AppendData( wxString::Format(wxT("%02X"), StreamNo).ToAscii(), 2 );			// ’ix##’

	int FrameCount=0;
	vector<FrameProp>::reverse_iterator rit;
	for ( rit=frame_index.rbegin() ; rit < frame_index.rend() ; ++rit )
		if( rit->StreamNo == StreamNo ){
			FrameCount = rit->FrameCount+1;
			break;
			}

	uint32_t tmp32 = FrameCount;
	idx.AppendData( reinterpret_cast< void*>(&make_littleendian( tmp32 )), 4 );

	uint16_t tmp16 = 2;
	idx.AppendData( reinterpret_cast< void*>(&make_littleendian( tmp16 )), 2 );	// Longs Per Entry

	idx.AppendByte( '\0' ); //must be 0
	idx.AppendByte( '\1' ); //must be AVI_INDEX_OF_CHUNKS

	tmp32 = FrameCount;
	idx.AppendData( reinterpret_cast< void*>(&make_littleendian( tmp32 )), 4 );	// Entries In Use;

	wxString StreamName;
	for ( rit=frame_index.rbegin() ; rit < frame_index.rend() ; ++rit )
		if( rit->StreamNo == StreamNo ){
			StreamName = rit->ChunkHead();
			break;
			}

	idx.AppendData( StreamName.ToAscii() , 4 );	// ChunkId

	tmp32 = movi_position;
	idx.AppendData( reinterpret_cast< void*>(&make_littleendian( tmp32 )), 4 );	//BaseOffset;

	tmp32 = 0;
	idx.AppendData( reinterpret_cast< void*>(&make_littleendian( tmp32 )), 4 );	//Reserved;

	for ( it=frame_index.begin() ; it < frame_index.end() ; ++it ){
		tmp32 = 0;
		idx.AppendData( reinterpret_cast< void*>(&make_littleendian( tmp32 )), 4 );	//Offset;

		tmp32 = 0;
		idx.AppendData( reinterpret_cast< void*>(&make_littleendian( tmp32 )), 4 );	//Size - // bit 31 is set if this is NOT a keyframe;
		}
	return idx;
	}
//Creates old type idx1 for AVI < 2GB or 4GB
wxMemoryBuffer DivFixp2Core::Create_OldIndex( vector<FrameProp>& frame_index, uint64_t movi_position ){
	vector<FrameProp>::iterator it;
	wxMemoryBuffer idx;
	idx.AppendData( "idx1" , 4);
	int tmp = frame_index.size()*16;
	idx.AppendData( reinterpret_cast< void*>(&make_littleendian( tmp )), 4 );
	int KeyFrameFlag = 0x10; // AVIIF_KEYFRAME
	for ( it=frame_index.begin() ; it < frame_index.end() ; ++it ){
		idx.AppendData( it->ChunkHead().ToAscii(), 4 );
		tmp = (it->KeyFrame ? KeyFrameFlag : 0);
		idx.AppendData( reinterpret_cast< void*>(&make_littleendian( tmp )), 4 );
		tmp = it->Position - movi_position;
		idx.AppendData( reinterpret_cast< void*>(&make_littleendian( tmp )), 4 );
		tmp = it->DataSize - 8;	//-8 for AVI header like 00dcSIZE
		idx.AppendData( reinterpret_cast< void*>(&make_littleendian( tmp )), 4 );
		}
	return idx;
	}


//Add frame that located in buff to output file and hangle keyFrame indexing.
bool DivFixp2Core::AddFrame( wxFFile *output, uint64_t& write_position, wxMemoryBuffer& buff, unsigned optype, vector<FrameProp>& frame_index, wxString four_cc ){
	if( Error_Check & optype ){
		write_position += buff.GetDataLen();
		}
	else{
		//uint64_t write_position = output->Tell();
		if( OverWrite & optype ){
			if( CutOut & optype ){
				if(write_position%2){	// Even Byte Padding
					output->Seek( write_position , wxFromStart);
					output->Write( "\0", 1);
					write_position++;
					}
				output->Write( buff, buff.GetDataLen() );
				write_position += buff.GetDataLen();
				}
			else{
//do nothing, only make index proper, because we not cut frames...
//				if(write_position%2){	// Even Byte Padding
//					output->Seek( write_position , wxFromStart);
//				write_position += buff.GetDataLen();
				}
			}
		else{	//Not Overwrite
			output->Seek( write_position , wxFromStart);
			if(write_position%2){		// Even Byte Padding
				output->Write( "\0", 1);
				write_position++;
				}
			output->Write( buff, buff.GetDataLen() );
			write_position += buff.GetDataLen();
			}
		}
	frame_index.push_back( FrameAnalyzer( buff, write_position - buff.GetDataLen(), frame_index, four_cc ) );
	return true;
	}

FrameProp DivFixp2Core::FrameAnalyzer( wxMemoryBuffer& buff, uint64_t write_position, vector<FrameProp>& frame_index, wxString four_cc ){
	FrameProp tmp;
	tmp.StreamNo = atoh2( buff );	// atohex
	strncpy( tmp.StreamType, buff+2, 2 );
	tmp.DataSize = buff.GetDataLen();
	tmp.Position = write_position;	//denotes exact position in file, not movi relative.
	tmp.FrameCount = 0;

	vector<FrameProp>::reverse_iterator rit;
	for ( rit=frame_index.rbegin() ; rit < frame_index.rend() ; ++rit ){
		if( rit->StreamNo == tmp.StreamNo ){
			tmp.FrameCount = rit->FrameCount+1;
			break;
			}
		}

	if( tmp.FrameCount == 0 ){
		tmp.FrameCount++;
		}
	if( tmp.StreamType[1] == 'b' )	//wb or db
		tmp.KeyFrame = true;
	else
		tmp.KeyFrame = IsKeyFrameData( buff+8, buff.GetDataLen()-8, four_cc, tmp.FrameCount );

	return tmp;
	}

bool DivFixp2Core::IsKeyFrameData( const char* data, unsigned size, const wxString& four_cc, unsigned FrameNumber ){
	unsigned flag;
	memcpy(reinterpret_cast<char*>(&flag), data, 4);
	flag = make_littleendian( flag );//Because definitions uses little indian.

	if( four_cc == wxT("MJPG") )
		if( size > 0 )
			return true;
		else
			return false;

	else if( four_cc == wxT("DIV3") or
		four_cc == wxT("MP43") or
		four_cc == wxT("MP42") ){
		if( (flag & 0x00007001)==0x00007001 and ( (0xFFFF7F3F | flag) == 0xFFFF7F3F) )
			return true;
		else
			return false;
		}
	else if( four_cc == wxT("DIVX") or
			  four_cc == wxT("DX50") or
			  four_cc == wxT("XVID"))
		//return ( (flag & 0xB6000000)==0xB0000000 || (flag & 0xB6000000)==0 );
		return (flag & 0x06000000)==0;

	else if( four_cc == wxT("SVQ1")) //Sorenson Video 1
		return (flag & 0x01000000)==0 ;	  //For last 2 bit : 0 = I frame | 1 = P frame | 2 = B frame - Accept for I & B frames

	else if( four_cc == wxT("SVQ3")){//Sorenson Video 3 (Apple Quicktime 5) & FLV1?
		///Reverse engineered SVQ3 key frame detection algorithm.
		///Not detect all frames but I think its enough for now.
		///Might be later, implement of Golomb code for make it proper.
		static unsigned char svq3_byte;
		static int svq3_type;
		if( FrameNumber == 1 ){	//First frame has to be keyframe
			svq3_byte=0x00;							//Clear svq3_byte for detect rare type of SVQ.
			svq3_type=0;							//Clear svq3_type detect variant.
			if( flag & 0x10000000 )					//Common Type P Frame flag set while KeyFrame,
				svq3_type = 2;						// So stream has to be RareType
			if( flag & 0x08000000 )					//Rare Type P Frame flag set while KeyFrame,
				svq3_type = 1;						// So stream has to be RareType
			return true;
			}
		if((( flag & 0x41 ) == 0x41 )				// There is actualy 2 version of SVQ3.
			& (( data[4] & 0x41 ) == 0x41) ){		// All keyframes 1st byte [0x61-0x41] & 5th byte is [0xC1-0x41]
			if(( flag & 0xFF ) == 0x61 ){			// If flag (fist 4 byte) starting with 0x61, that is key frame.
				if( svq3_type == 0){
					///FrameType prediction as above
					if( flag & 0x10000000 )
						svq3_type = 2;
					else if( flag & 0x08000000 )
						svq3_type = 1;
					}
				return true;
				}

			if( svq3_type == 1 )					// Common Frame Type - This one is 4th byte of flag '00010000' = 0x10 indicates P frame.
				return (flag & 0x10000000)==0;
			else if( svq3_type == 2 )				// Rare Frame Type   - Rare one is 4th byte of flag '00001000' = 0x80 indicates P frame.
				return (flag & 0x08000000)==0;
			// So ( svq3_type == 0 )
			return (flag & 0x10000000)==0; //Defaulting to Common Frame Type
			}

		else{	//Non-Keyframe
			svq3_byte = data[4] | svq3_byte;
			if( FrameNumber == 16 ){// Collect first 16 Frames data on 5th byte
				if( svq3_byte & 0xF8 )	// Rare Frame Type frames have a format that 5 MSBs clear on 5.th byte. '00000xxx' on P frames but I and B frames.
					svq3_type = 1;	//Common Frame Type if there are any set at 5 most significant bits
				else
					svq3_type = 2;	//Rare Frame Type  if there are clear all 5 most significant bits
				}
			return false;
			}
		}

	else if( four_cc == wxT("VP30") or // Theora &
			  four_cc == wxT("VP31") or // TrueMotion VP codecs
			  four_cc == wxT("VP32") or
			  four_cc == wxT("VP40") or
			  four_cc == wxT("VP50") or
			  four_cc == wxT("VP60") or // FLV4?
			  four_cc == wxT("VP61") or
			  four_cc == wxT("VP62"))
			  //!strncmp( four_cc, "VP6F", 4 )) // ffmpeg (open source) - FFmpeg VP6 / Flash') ?
		return (flag & 0x00000080)==0;

	//	else if( !strncmp( four_cc, "VP70", 4 ) || // TrueMotion VP7 codecs - PROBLEM!
	//			  !strncmp( four_cc, "VP71", 4 ) ||
	//			  !strncmp( four_cc, "VP72", 4 ) )
	//		return ( flag & 0x00000001)==0;

	else if( four_cc == wxT("H264") or
			four_cc == wxT("AVC1")){
			if( FrameNumber == 1 )	//Frame numbers start from 1, not zero.
				return true;		//Fist frame of AVC is always keyframe
			unsigned nalu_len = 0;
			const char* data_nextnalu = data;
			unsigned pos = 0;
			unsigned avcflag = 0;
			while( pos+4 < size ){
				data_nextnalu = data+pos;
				avcflag |= 1 << ((data_nextnalu[4] & 0x0F)-1);
//nalu_len = to_bigendian( *reinterpret_cast< uint32_t*>( data_nextnalu ));
				memcpy(reinterpret_cast<char*>(&nalu_len), data_nextnalu, 4);
				nalu_len = make_bigendian( nalu_len );
				pos += nalu_len+4;
				}
			return ((avcflag>>(5-1)) & 0x01);

//			int pos = 0;
//			bool z = false;
//			while( pos < size ){
//				if( data[++pos]!=0x00 ){
//					z = (data[pos+1] & 0x1F)==0x07;
//					break;
//					}
//				}
//			return z;
#define NALU_START_CODE 0x00000001
#define NALU_TYPE_NON_IDR_SLICE  0x01
#define NALU_TYPE_DP_A_SLICE     0x02
#define NALU_TYPE_DP_B_SLICE     0x03
#define NALU_TYPE_DP_C_SLICE     0x04
#define NALU_TYPE_IDR_SLICE      0x05
#define NALU_TYPE_SEI            0x06
#define NALU_TYPE_SEQ_PARAM      0x07
#define NALU_TYPE_PIC_PARAM      0x08
#define NALU_TYPE_ACCESS_UNIT    0x09
#define NALU_TYPE_END_OF_SEQ     0x0a
#define NALU_TYPE_END_OF_STREAM  0x0b
#define NALU_TYPE_FILLER_DATA    0x0c
		}

	else if( four_cc == wxT("WMV1"))
		return (flag & 0x00000040)==0;
	else if( four_cc == wxT("WMV2") )
		return (flag & 0x00000080)==0;
	else if( four_cc == wxT("WMV3"))
		return (flag & 0x00000020)==0;
	/**VC1 code will need to be tested.
	//	else if( four_cc == wxT("WVC1")){
	//		int ret = 0;
	//		static int interlace, tfcntr, pulldown,profile,level,AnchorPictureSeen;
	////		for( int a = 0 ; a < 10 ; a++ ){
	////			uint8_t s = *(data+a);
	////			printf( "%02X ", s );
	////			}
	////		cout << endl;
	//		char frame_mode_str[10];
	//		unsigned char c;
	//		int frame_mode;
	//
	//		switch (data[3])
	//		{
	//		case 0x0f:
	//			// Sequence header.
	//			profile = data[4] >> 6;
	//			level = (data[4] >> 3) & 7;
	//			tfcntr = (data[9] >> 5) & 1;
	//			interlace = (data[9] >> 6) & 1;
	//			pulldown = (data[9] >> 7) & 1;
	//			break;
	//		case 0x0e:
	//			// Entry point header.
	//			// Don't accept entry points until we've seen a sequence header.
	//			if (profile == -1)
	//				break;
	//			break;
	//		case 0x0d:
	//			// Picture header.
	//			// Don't accept pictures until we've seen a sequence header.
	//			if (profile == -1)
	//				break;
	//			{
	////				coded++;
	////				playback++;
	//				c = data[4];
	//				frame_mode = 1;
	//				if (interlace)
	//				{
	//					if ((c & 0x80) == 0x00)
	//					{
	//						// Progressive.
	//						strcpy(frame_mode_str, "PROG");
	//						c <<= 1;
	//					}
	//					else if ((c & 0xc0) == 0x80)
	//					{
	//						// Frame interlace.
	//						strcpy(frame_mode_str, "FRAME");
	//						c <<= 2;
	//					}
	//					else if ((c & 0xc0) == 0xc0)
	//					{
	//						// Field interlace.
	//						strcpy(frame_mode_str, "FIELD");
	//						c <<= 2;
	//						c >>= 5;
	//						frame_mode = 0;
	//					}
	//				}
	//				else
	//					strcpy(frame_mode_str, "PROG");
	//				if (frame_mode)
	//				{
	//					if ((c & 0xe0) == 0xc0)
	//					{
	//						cout << " I frame" << endl;
	//						AnchorPictureSeen = 1;
	//						return true;
	//					}
	//					else if ((c & 0x80) == 0x00){
	//						cout << " P frame" << endl;
	//						return false;
	//						}
	//					else if ((c & 0xc0) == 0x80){
	//						cout << " B frame" << endl;
	//						return false;
	//						}
	//					else if ((c & 0xf0) == 0xe0){
	//						cout << " BI frame" << endl;
	//						return true;
	//						}
	//					else if ((c & 0xf0) == 0xf0){
	//						cout << " P Skip" << endl;
	//						return false;
	//						}
	//				}
	//				else
	//				{
	//					if (c < 3)
	//					{
	//						cout << " I frame" << endl;
	//						AnchorPictureSeen = 1;
	//						return true;
	//					}
	//					else if (c == 3){
	//						cout << " P frame" << endl;
	//						return false;
	//						}
	//					else if (c == 4){
	//						cout << " B frame" << endl;
	//						return false;
	//						}
	//					else{
	//						cout << " BI frame" << endl;
	//						return true;
	//						}
	//				}
	//			}
	//			break;
	//		}
	//		return ret;
	//
	//		}
	/*/
	else
		return (flag & 0x06000000)==0;	// Defaulting XVID codec flag.
	}
inline bool DivFixp2Core::IsFrame( const char *data ){
	// accepted keywords are XX[dw][bc] XX is hex ASCII
	if((( ( data[0]<= '9' and data[0]>='0' ) and ( data[1]<= '9' and data[1]>='0' ) ) or
		( ( data[0]<= 'a' and data[0]>='f' ) and ( data[1]<= 'a' and data[1]>='f' ) ) or
		( ( data[0]<= 'A' and data[0]>='F' ) and ( data[1]<= 'A' and data[1]>='F' ) ) )
		and
 		( (( data[2] == 'd' or data[2] == 'w') and ( data[3] == 'b' or data[3] == 'c')) or
			(data[2] == 't' and data[3] == 'x' )) 	//for txt data like subtitle
		)
		return true;
	else
		return false;
		}
bool DivFixp2Core::IsAVI( wxFFile* input ){
	wxMemoryBuffer buff;
	input->Seek( 0 , wxFromStart);
	buff.UngetWriteBuf( input->Read( buff.GetWriteBuf(16), 16) );
// TODO (death#1#): try/catch
	if( !strncmp(buff+8,"AVI LIST",8 ))
		return true;
	else
		return false;
	}
wxString DivFixp2Core::GetAVIFourCC( wxFFile* input ){
	char four_cc[5];
	input->Seek( 188 , wxFromStart );
	input->Read( &four_cc , 4);
	four_cc[4]='\0';
// TODO (death#1#): try/catch
	return wxString::FromAscii(four_cc).Upper();
	}

bool DivFixp2Core::Strip( wxString strip_file ){
	wxFFile input;
	if(! input.Open( strip_file.c_str(), _T("rb+") ) ){
		MemoLogWriter( wxString(_("Error: "))+_("Input file cannot be opened!\n"),true );
		input.Close();
		return false;
		}
	char buffer[] = {0,0,0,0,0};
	unsigned read_position = 16;
	for(int jump = 0 ; strncmp(buffer,"movi",4) ;read_position += jump+8 ){
		input.Seek( read_position, wxFromStart );
		if( input.Error() ){
			input.Close();
			MemoLogWriter(wxString(_("Error: "))+_("Input file seek error.\n"),true);
			return false;
			}
		input.Read( reinterpret_cast<char*>(&jump), 4 );
		jump = make_littleendian( jump );
		input.Read( buffer, 4 );
		if( input.Eof() ){
			MemoLogWriter(wxString(_("Error: "))+_("Input's idx1 section not found!\n"));
			input.Close();
			return false;
			}
		}
	input.Close();							//File closing for truncation
// TODO (death#1#): Is it's multiple RIFF section? Than fill idx1 with junk chunk!
// TODO (death#1#): Need cross check for idx1 chunk on movi end
	return Truncate(strip_file, read_position-4);
// TODO (death#1#): Also needed to remove standart indexes and make them fill with junk	}

bool DivFixp2Core::Truncate( wxString filename, unsigned cut_here ){
	//Hand made low level truncate function
	int fdes;									// File descriptor
	fdes = open( filename.ToAscii(), O_RDWR );	// Opens file descriptor
	std::cout << strerror(errno) << std::endl;

	int success;
	#ifdef  __WXMSW__
		success = _chsize( fdes,cut_here );					// Resizes file at windows
	#elif defined __UNIX__
		success = ftruncate(fdes,cut_here );					// Resizes file at Unix/Linux/Mac
	#elif defined __WXGTK__
		success = ftruncate(fdes,cut_here );					// Resizes file at Linux
	#elif defined __WXMAC__
		success = ftruncate(fdes,cut_here );					// Resizes file at Mac
	#else
		wxLogError("Error: "))+("No truncation function on this system!\nFile is not truncated!"),true);
		success = -1;
	#endif
	close(fdes);								// Closes file descriptor
	if( success == -1 )
		MemoLogWriter(wxString(_("Error: "))+wxString::Format(_("File cannot truncate at %d"),cut_here)+_T("\n"));
	return success == 0 ;
	}


bool DivFixp2Core::HasAVIGotProperIndex( wxString input ){
	// TODO (death#1#): HasAVIGotProperIndex Import
	}
void DivFixp2Core::MemoLogWriter( wxString message, bool Error ){
	if(WxMemoLog != NULL){
		if( ! wxThread::This()->IsMain() )//We need check if its main thread or not for locking Gui...
			wxMutexGuiEnter();
		WxMemoLog->AppendText( message );
		if( ! wxThread::This()->IsMain() )
			wxMutexGuiLeave();
		}
	else if( Error )
		wxLogError(message);
	}
int DivFixp2Core::IdentifyStreamType( wxString Source){
	wxFFile input;
	wxMemoryBuffer buffer;
	if(not input.Open( Source, _T("rb"))){
		MemoLogWriter( wxString(_("Error: "))+_("Input file cannot be opened! ")+_T("at IsAvi()\n") ,true);
		input.Close();
		return -1;
		}
	input.Seek( 0 , wxFromStart);
	input.Read( buffer.GetWriteBuf(16), 16);
	buffer.UngetWriteBuf( 16 );

	input.Close();

	if( not strncmp(buffer+8,"AVI LIST",8 ))
		return 1;
	else if( 0xA3DF451A == make_littleendian(*reinterpret_cast<unsigned int*>(buffer.GetData())) )	// EBML / Matroska header
		return 2;
	else
		return 0;
	}

//////////////////binaryElement Section////////////////////////////////////////

binaryElement::binaryElement( wxString _name ):name(_name){
	}

void binaryElement::print( int level, bool endline ){
	for( int i = 0 ; i < level ; i ++ )
		cout << '|';
	cout << name.ToAscii() << " size :" << size();
	if( endline ) cout << endl;
	for( vector<binaryElement>::iterator it = binvector.begin() ; it != binvector.end() ; it++ )
		it->print( level+1 );
	}
wxMemoryBuffer binaryElement::GetAsBuffer(){
	wxMemoryBuffer ret;
	if( name != wxT("ROOT"))
		ret.AppendData( name.ToAscii(), name.Len() );
	if( acl[name] >= 2)
// TODO (death#1#): //Add this tokens to undependent query db in binaryElement structure		ret.AppendData( reinterpret_cast<void*>( &make_littleendian( size() -4 - name.Len() ) ) ,4 );
	ret.AppendData( data, data.GetDataLen());

	if( ret.GetDataLen()%2 )	//All chunks needed to start from even positions
		ret.AppendByte('\0');

	for( vector<binaryElement>::iterator it = binvector.begin() ; it != binvector.end()  ; it++ ){
		wxMemoryBuffer tmp = it->GetAsBuffer();
		ret.AppendData( tmp, tmp.GetDataLen() );
		}

	return ret;
	}
unsigned binaryElement::size( void ){
	unsigned size = 4;// all chunk names is  4 byte
	if( acl[name] >= 2 )
		size +=4;
	else if( name == wxT("ROOT") )
		size -= 4;	// Root element not ocunt on sizing

	size += data.GetDataLen();

	for( vector<binaryElement>::iterator it = binvector.begin() ; it != binvector.end()  ; it++ )
		size += it->size();
	return size+size%2;//round to even
	}
vector<binaryElement*> binaryElement::get(wxString chunk_name){
	vector<binaryElement*> ret;
	if( chunk_name == name )
		ret.push_back( this );
	vector<binaryElement*> tmp;
	for( vector<binaryElement>::iterator it = binvector.begin() ; it != binvector.end()  ; it++ ){
		tmp = it->get( chunk_name );
		if( tmp.size() != 0 )	//if it has content
			for( vector<binaryElement*>::iterator itx = tmp.begin() ; itx != tmp.end()  ; itx++ )
				ret.push_back( *itx );
		}
	return ret;
	}
/*
xxdb now accepts 255 stream.
aware of wc frames
full frame inspection
token avi chunks
modular code
*/
