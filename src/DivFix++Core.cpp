//-----------------------------------------------------------------------
//
// Name:        DivFix++Core.cpp
// Author:      Death Knight
// Description: DivFix++ CORE Library Code
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

#include "DivFix++Core.h"
#include "DivFix++App.h"

#ifdef _project_Meteorite_
	#include "meteorite.h"
#endif
void DivFixppCore::DivFix_initialize(){
	WxGauge = NULL;
	WxProgress = NULL;
	WxMemoLog = NULL;// new wxTextCtrl;
	input = new wxFFile();
	tempout = output = new wxFFile();
	read_position = write_position  = end = 0;
	buffer = new char[buffer_size];
	avih.avi_header =  odmlh.odml_header = NULL;
	avih.position = odmlh.position = odmlh.size = 0;

	if( !buffer ){
		wxLogError(wxString( _("Error: "))+wxString::Format(_("No Free Memory Available. %d %s"),((buffer_size+index_size)/(1024*1024)), _(" MB required.")), true);

		}
	index_list_ptr = index_list = new char[index_size];
	if( !index_list ){
		wxLogError(wxString( _("Error: "))+wxString::Format(_("No Free Memory Available. %d %s"),((buffer_size+index_size)/(1024*1024)), _(" MB required.")), true);
		}
	}

DivFixppCore::DivFixppCore(){
	DivFix_initialize();
	}

DivFixppCore::DivFixppCore( wxProgressDialog *prgs ){
	DivFix_initialize();
	WxProgress = prgs;
	}

DivFixppCore::DivFixppCore( wxGauge *wxgg, wxTextCtrl *wxml ){
	DivFix_initialize();
	delete WxMemoLog;
	WxGauge = wxgg;
	WxMemoLog = wxml;
	}

DivFixppCore::~DivFixppCore(){
	delete buffer;
	delete index_list;
	}

inline bool DivFixppCore::is_frame( const char *data, bool keyframe ){
	if( keyframe )
		return is_keyframe( data );

	if(( ( data[0]<= '9' && data[0]>='0' ) && ( data[1]<= '9' && data[1]>='0' )	) &&
 		   			( ( data[2] == 'd' && data[3] == 'c') || (( data[2] == 'd' || data[2] == 'w') && data[3] == 'b') )	)
		return true;
	else
		return false;
		}

inline bool DivFixppCore::is_keyframe( const char *data ){
	if(( ( data[0]<= '9' && data[0]>='0' )	&& ( data[1]<= '9' && data[1]>='0' ) )
		&&
		(
		( (data[2] == 'd' && data[3] == 'c') && ( is_keyflag( data ) ))
		||
		((data[2] == 'd' /*|| data[2] == 'w'*/) && data[3] == 'b'))
// TODO (death#1#): 00wb is count as keyframe? No for searching but indexing in divxvid		)
		return true;
	 else
		return false;
		}

inline bool DivFixppCore::is_keyflag( const char *data ){
	unsigned int flag;
	memcpy(reinterpret_cast<char*>(&flag), data+8, 4);
	flag = make_littleendian( flag );

	if( !strncmp( four_cc, "MJPG", 4 ) )
		return true;

	if( !strncmp( four_cc, "DIV3", 4 ) ||
		!strncmp( four_cc, "MP43", 4 ) ||
		!strncmp( four_cc, "MP42", 4 )){
		if( (flag & 0x00007001)==0x00007001 && ( (0xFFFF7F3F | flag) == 0xFFFF7F3F) )
			return true;
		else
			return false;
		}
	else if( !strncmp( four_cc, "DIVX", 4 ) ||
			  !strncmp( four_cc, "DX50", 4 ) ||
			  !strncmp( four_cc, "XVID", 4 ))
		//return ( (flag & 0xB6000000)==0xB0000000 || (flag & 0xB6000000)==0 );
		return (flag & 0x06000000)==0;

	else if( !strncmp( four_cc, "SVQ1", 4 )) //Sorenson Video 1
		return (flag & 0x01000000)==0 ;	  //For last 2 bit : 0 = I frame | 1 = P frame | 2 = B frame - Accept for I & B frames

	else if( !strncmp( four_cc, "SVQ3", 4 )){//Sorenson Video 3 (Apple Quicktime 5) & FLV1?
		///Reverse engineered SVQ3 key frame detection algorithm.
		///Not detect all frames but I think its enough for now.
		///Might be later, implement of Golomb code for make it proper.
		static unsigned char svq3_byte;
		static int svq3_type;
		if( frame_counter[ atoi( buffer ) ] == 1 ){	//First frame has to be keyframe
			svq3_byte=0x00;							//Clear svq3_byte for detect rare type of SVQ.
			svq3_type=0;							//Clear svq3_type detect variant.
			if( flag & 0x10000000 )					//Common Type P Frame flag set while KeyFrame,
				svq3_type = 2;						// So stream has to be RareType
			if( flag & 0x08000000 )					//Rare Type P Frame flag set while KeyFrame,
				svq3_type = 1;						// So stream has to be RareType
			return true;
			}
		if((( flag & 0x41 ) == 0x41 )				// There is actualy 2 version of SVQ3.
			& (( data[8+4] & 0x41 ) == 0x41) ){		// All keyframes 1st byte [0x61-0x41] & 5th byte is [0xC1-0x41]
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
			svq3_byte = data[8+4] | svq3_byte;
			if( frame_counter[ atoi( buffer ) ] == 16 ){// Collect first 16 Frames data on 5th byte
				if( svq3_byte & 0xF8 )	// Rare Frame Type frames have a format that 5 MSBs clear on 5.th byte. '00000xxx' on P frames but I and B frames.
					svq3_type = 1;	//Common Frame Type if there are any set at 5 most significant bits
				else
					svq3_type = 2;	//Rare Frame Type  if there are clear all 5 most significant bits
				}
			return false;
			}
		}

	else if( !strncmp( four_cc, "VP30", 4 ) || // Theora &
			  !strncmp( four_cc, "VP31", 4 ) || // TrueMotion VP codecs
			  !strncmp( four_cc, "VP32", 4 ) ||
			  !strncmp( four_cc, "VP40", 4 ) ||
			  !strncmp( four_cc, "VP50", 4 ) ||
			  !strncmp( four_cc, "VP60", 4 ) || // FLV4?
			  !strncmp( four_cc, "VP61", 4 ) ||
			  !strncmp( four_cc, "VP62", 4 ))
			  //!strncmp( four_cc, "VP6F", 4 )) // ffmpeg (open source) - FFmpeg VP6 / Flash') ?
		return (flag & 0x00000080)==0;

//	else if( !strncmp( four_cc, "VP70", 4 ) || // TrueMotion VP7 codecs - PROBLEM!
//			  !strncmp( four_cc, "VP71", 4 ) ||
//			  !strncmp( four_cc, "VP72", 4 ) )
//		return ( flag & 0x00000001)==0;

	else if( !strncmp( four_cc, "H264", 4 ) || !strncmp( four_cc, "AVC1", 4 )){
		if( flag & 0x01000000 )					// Some NAL has start_code_prefix_one_4bytes others start_code_prefix_one_3bytes
			return (data[8+4] & 0x1F)==0x07;	// if nal_unit_type 7 - Sequence parameter set.
		else if( flag & 0x00010000)			// 0x1F filters last 5 bit which equals nal_unit_type
			return (data[8+3] & 0x1F)==0x07;	// IDR frame needed to be Type 5 (NAL_IDR_SLICE).
		else									// but 7 (NAL_SPS) looks working. I might add 5 later.
			return false;						// 6 (NAL_SEI) could lucky number too..
		}
	else if( !strncmp( four_cc, "WMV1", 4 ))	// WMVx flag values (Reverse engineered)
		return (flag & 0x00000040)==0;
	else if( !strncmp( four_cc, "WMV2", 4 ))
		return (flag & 0x00000080)==0;
	else if( !strncmp( four_cc, "WMV3", 4 ))	// I am not sure if this is true but works.
		return (flag & 0x00000020)==0;
	else
		return (flag & 0x06000000)==0;	// Defaulting XVID codec flag.
	}

inline int DivFixppCore::search_frame( char *bfr, int bfrsize, bool keyframe ){	// Frame search algorithm.
		int bfr_ptr;												// Checks every 4th char
		for(bfr_ptr = 0; bfr_ptr < bfrsize - 16 ; bfr_ptr +=4 ){	// Does not check last 4 chars...
			switch ( *(bfr+bfr_ptr) ){
				case ('0'):{
						if( is_frame( bfr+bfr_ptr, keyframe ) )
							return bfr_ptr-0;
						else if( bfr_ptr != 0){ //case('1')
							if( is_frame( bfr+bfr_ptr-1, keyframe ) )
								return bfr_ptr-1;
							else
								break;
							}
						else
							break;
						}
				case ('1'):{
						if( bfr_ptr < 1 ) break;	//for avoid negative buffer overflow
							if( is_frame( bfr+bfr_ptr-1, keyframe ) )
								return bfr_ptr-1;
							else
								break;
						}
				case ('d'):{
						if( bfr_ptr < 2 ) break;	//for avoid negative buffer overflow
							if( is_frame( bfr+bfr_ptr-2, keyframe ) )
								return bfr_ptr-2;
							else
								break;
						}
				case ('c'):{
						if( bfr_ptr <= 3 ) break;	//for avoid negative buffer overflow
							if( is_frame( bfr+bfr_ptr-3, keyframe ) )
								return bfr_ptr-3;
							else
								break;
						}
				case ('w'):{
						if( bfr_ptr < 2 ) break;	//for avoid negative buffer overflow
							if( is_frame( bfr+bfr_ptr-2, keyframe ) )
								return bfr_ptr-2;
							else
								break;
						}
				case ('b'):{
						if( bfr_ptr < 3 ) break;	//for avoid negative buffer overflow
							if( is_frame( bfr+bfr_ptr-3, keyframe ) )
								return bfr_ptr-3;
							else
								break;
						}
					}
				} // for's end
		return -1;
		}

bool DivFixppCore::junk_padding( unsigned int pad_to, bool force ){   // Creates Junk chunk for match file size 2kb (or given bytes)
	unsigned int temp;
	if(force){
		temp = write_position % pad_to;
		output->Seek(write_position,wxFromStart);
		}
	else{
		temp = output->Length() % pad_to;
		output->SeekEnd();
		}
	if(output->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Output file seek error.\n"),true); close_files(); return false; }

	if( temp == 0){
		return true;
		}
	temp = pad_to - temp;
	output->Write( "JUNK", 4 );                   // padding to 2048
	if(output->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Output file write error.\n"),true); close_files(); return false; }
	temp-=4;                                      // -4 for JUNK writed
	temp-=4;									  // -4 for JUNK size will be writen
	output->Write( reinterpret_cast<char*>(& make_littleendian(temp)), 4 ); //Writing Junk Size
	if(output->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Output file write error.\n"),true); close_files(); return false; }
	memset(buffer,0,buffer_size);                 // fill buffer zero
	output->Write( buffer, temp);                 // Fill JUNK with zero
	if(output->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Output file write error.\n"),true); close_files(); return false; }
	return true;
	}

bool DivFixppCore::avi_header_fix( void ){		 // Updates/Fixes headers frame counts
	int pos=0;
	int chunk_size=0;

	if(output->Eof()){
		MemoLogWriter(_("EndOfFile at output\n"));
		}
	if(!output->IsOpened()){
		MemoLogWriter(wxString(_("Error: "))+_("Input file cannot be opened!\n"));
		return false;
		}

	output->Seek( 16, wxFromStart );
	if(output->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Output file seek error.\n"),true); close_files(); return false; }
	output->Read( reinterpret_cast<char*>(&chunk_size), 4 );
	chunk_size= make_littleendian( chunk_size );
	if(output->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Output file read error.\n"),true); close_files(); return false; }
	pos = output->Tell();
	output->Read( buffer, chunk_size );
	if(output->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Output file read error.\n"),true); close_files(); return false; }
	LIST_parser( buffer, chunk_size, pos );
	output->Read( buffer, 12 );					//read next 'ChunkID Size Header'
	if(output->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Output file read error.\n"),true); close_files(); return false; }
	while( strncmp(buffer, "LIST", 4) && strncmp(buffer+8, "movi", 4)){
		memcpy( reinterpret_cast<char*>(&chunk_size), buffer+4, 4 );
		chunk_size = make_littleendian( chunk_size );
		output->Seek( -4, wxFromCurrent );
		if(output->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Output file seek error.\n"),true); close_files(); return false; }
		if( !strncmp( buffer, "LIST", 4 )){
			pos = output->Tell();
			output->Read( buffer, chunk_size );
			LIST_parser( buffer, chunk_size, pos );
			}
		else if( !strncmp( buffer, "JUNK", 4 )){
			output->Read( buffer, chunk_size );
			JUNK_parser( buffer, chunk_size );
			}
		else{
			buffer[4]=0;
			MemoLogWriter(wxString(_("Error: "))+wxString::Format(_("Unknown identifier at List Parsing: %s\n"),buffer));
			return false;
			}
		output->Read( buffer, 12 );
		if(output->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Output file read error.\n"),true); close_files(); return false; }
		}
//Warning! 00DC Predicted!
//	unsigned int total_frame_count= frame_counter[0];
//	if( !strncmp(four_cc, "VP7",3))	//VP7 codec requires total frame count as needed to be (Video+Audio).
//		total_frame_count += frame_counter[1];
	memcpy( avih.avi_header+(4*4), &make_littleendian(frame_counter[0]), 4);  //16->20  is TotalNumberOfFrames
	output->Seek( avih.position, wxFromStart );
	if(output->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Output file seek error.\n"),true); close_files(); return false; }
	output->Write( reinterpret_cast<char*>(avih.avi_header), 56 );
	if(output->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Output file write error.\n"),true); close_files(); return false; }
//Warning! 00DC Predicted!
	memcpy( strh[0].stream_header+(4*8), &make_littleendian(frame_counter[0]), 4);        // 32 -> 36 is Data lenght
	output->Seek( strh[0].position, wxFromStart );
	if(output->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Output file seek error.\n"),true); close_files(); return false; }
	output->Write( reinterpret_cast<char*>(strh[0].stream_header), 56 );
	if(output->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Output file write error.\n"),true); close_files(); return false; }

//Warning! 01WB Predicted!
	if( strh[1].stream_header != NULL ){
		memcpy( strh[1].stream_header+(4*8),&make_littleendian(frame_counter[1]),4);           // 32 -> 36 is Data lenght
		output->Seek( strh[1].position, wxFromStart );
		if(output->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Output file seek error.\n"),true); close_files(); return false; }
		output->Write( reinterpret_cast<char*>(strh[1].stream_header), 56 );
		if(output->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Output file write error.\n"),true); close_files(); return false; }
		}

	//00DC Predicted!
	if( odmlh.odml_header != NULL ){							// Some avi has no ODML Header
		memcpy( odmlh.odml_header, &make_littleendian(frame_counter[0]), 4);
		output->Seek( odmlh.position, wxFromStart );
		if(output->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Output file seek error.\n"),true); close_files(); return false; }
		output->Write( reinterpret_cast<char*>( odmlh.odml_header), odmlh.size );
		if(output->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Output file write error.\n"),true); close_files(); return false; }

		int temp;
		pos=0;
		do{
			memcpy( reinterpret_cast<char*>(&temp), odmlh.odml_header+pos, 4);
			temp = make_littleendian( temp );
			pos += 4; // ODML lenght
			if(temp)
					MemoLogWriter(wxString::Format(_("DMLH found: %d\n"),temp));
			}while(temp);
		}
	return true;
	}

bool DivFixppCore::LIST_parser( char* bfr, int lenght, int base ){// Header LIST Parser
		bool correct = true;										// base means buffer start location at file
		int chunk_size;
		int bfr_ptr=0;
		if( !strncmp( bfr, "hdrl",4 )){
			bfr_ptr+=4;	// hdrl
			if( strncmp( bfr+4, "avih",4 )){
				bfr[8]=0;
				MemoLogWriter(wxString(_("Error: "))+wxString::Format(_("AVI header not found? Found: %s\n"),bfr+4));
				return false;
				}
			bfr_ptr+=4;	//avih
			if( static_cast<int>(*(bfr+8)) != 56 ){ // is little endian req?
				MemoLogWriter(wxString( _("Error: "))+wxString::Format(_("AVI header size is false?  Found: %d\n"),make_littleendian(static_cast<int>(*(bfr+8)) )));;
				return false;
				}
			bfr_ptr+=4; //size
			avih.avi_header = new char[56];
			memcpy( reinterpret_cast<char*>(avih.avi_header), bfr+bfr_ptr, 56);
			avih.position = base+bfr_ptr;
			bfr_ptr += 56;	//avih chunk
			}
		if( !strncmp( bfr+bfr_ptr, "LIST", 4 )){
			bfr_ptr += 4; //LIST
			memcpy(reinterpret_cast<char*>(&chunk_size), bfr+bfr_ptr, 4);
			chunk_size= make_littleendian( chunk_size );
			bfr_ptr += 4; //size
			correct &= LIST_parser( bfr+bfr_ptr, chunk_size, base+bfr_ptr );
			bfr_ptr += chunk_size;
			bfr_ptr+=bfr_ptr%2;	//if bfr_ptr is odd, add 1 to make it even. Chunk modifiers only start at even bytes.
			}
		if( !strncmp( bfr+bfr_ptr, "strl",4 )){
			bfr_ptr += 4; //strl
			if( strncmp( bfr+bfr_ptr, "strh",4 )){
				(bfr+bfr_ptr)[4]=0;
				MemoLogWriter(wxString( _("Error: "))+wxString::Format(_("STREAMLIST location not found? Found: %d\n"), bfr+bfr_ptr));
				return false;
				}
			bfr_ptr += 4; //strh
			if( static_cast<int>(*(bfr+bfr_ptr)) != 56 ){
				MemoLogWriter(wxString( _("Error: "))+wxString::Format(_("AVI header size is false?  Found: %d\n"),make_littleendian(static_cast<int>(*(bfr+4)))));
				return false;
				}
			bfr_ptr += 4; //size

			int strh_ptr;
			for(strh_ptr=0 ; strh[strh_ptr].stream_header != NULL ; strh_ptr++);
			if(strh_ptr >= stream_limit){
				MemoLogWriter(wxString( _("Error: "))+wxString::Format(_("Bigger than %d stream? Breaked.\n"), stream_limit));
				return false;
				}
			strh[strh_ptr].stream_header = new char[56];
			memcpy(reinterpret_cast<char*>(strh[strh_ptr].stream_header) , bfr+bfr_ptr, 56);
			strh[strh_ptr].position = base+bfr_ptr;
			bfr_ptr += 56;

			if( !strncmp( bfr+bfr_ptr, "strf",4 )){
				bfr_ptr+=4;	//strf
				memcpy( reinterpret_cast<char*>(&chunk_size), bfr+bfr_ptr, 4);
				chunk_size= make_littleendian( chunk_size );
				bfr_ptr+=4;	//size
				bfr_ptr+=chunk_size;
				bfr_ptr+=bfr_ptr%2;	//if bfr_ptr is odd, add 1 to make it even. Chunk modifiers only start at even bytes.
				// STREAMBUFFER PARSER( bfr+bfr_ptr, frame_size )
				}
			if( !strncmp( bfr+bfr_ptr, "JUNK",4 )){
				bfr_ptr+=4;	//JUNK
				memcpy( reinterpret_cast<char*>(&chunk_size), bfr+bfr_ptr, 4);
				chunk_size= make_littleendian( chunk_size );
				bfr_ptr+=4;	//size
				JUNK_parser( bfr+bfr_ptr, chunk_size );
				bfr_ptr+=chunk_size;
				bfr_ptr+=bfr_ptr%2;	//if bfr_ptr is odd, add 1 to make it even. Chunk modifiers only start at even bytes.
				}
			if( bfr_ptr < lenght-1){
				return false;
				}
			}	// STREAMLIST end
		if( !strncmp( bfr, "odml", 4 )){
			bfr_ptr+=4; // odml
			if( !strncmp( bfr+bfr_ptr, "dmlh", 4 )){
				bfr_ptr+=4; // dmlh
				memcpy( reinterpret_cast<char*>(&chunk_size), bfr+bfr_ptr, 4);
				chunk_size= make_littleendian( chunk_size );
				bfr_ptr+=4; // size
				odmlh.odml_header = new char[chunk_size];
				odmlh.size = chunk_size;
				memcpy( odmlh.odml_header, bfr+bfr_ptr, chunk_size);
				odmlh.position = base + bfr_ptr;
					int temp2=0;
				do{
					memcpy( reinterpret_cast<char*>(&temp2), bfr+bfr_ptr, 4);
					temp2 = make_littleendian( temp2 );
					bfr_ptr += 4; // ODML lenght
					chunk_size -= 4;
					}while(temp2);
					//DML_parser( bfr+bfr_ptr, temp );
				bfr_ptr+=chunk_size;
				}
		}
		if( !strncmp(bfr+bfr_ptr,"vedt",4)){
			bfr_ptr+=4;	//JUNK
			memcpy( reinterpret_cast<char*>(&chunk_size), bfr+bfr_ptr, 4);
			chunk_size= make_littleendian( chunk_size );
			bfr_ptr+=4;	//size
			bfr_ptr+=chunk_size;
			bfr_ptr+=bfr_ptr%2;	//if bfr_ptr is odd, add 1 to make it even. Chunk modifiers only start at even bytes.
			}
		if( !strncmp(bfr+bfr_ptr,"segm",4)){
			bfr_ptr+=4;	//JUNK
			memcpy( reinterpret_cast<char*>(&chunk_size), bfr+bfr_ptr, 4);
			chunk_size= make_littleendian( chunk_size );
			bfr_ptr+=4;	//size
			bfr_ptr+=chunk_size;
			bfr_ptr+=bfr_ptr%2;	//if bfr_ptr is odd, add 1 to make it even. Chunk modifiers only start at even bytes.
			}
		if( !strncmp(bfr+bfr_ptr,"rec ",4)){
			bfr_ptr+=4;
			}
		if( !strncmp(bfr+bfr_ptr,"INFOISFT",4)){
			bfr_ptr+=8;	//INFOISFT
			memcpy( reinterpret_cast<char*>(&chunk_size), bfr+bfr_ptr, 4);
			chunk_size= make_littleendian( chunk_size );
			bfr_ptr+=4;	//size
			INFO_parser( bfr+bfr_ptr, chunk_size );
			bfr_ptr+=chunk_size;
			bfr_ptr+=bfr_ptr%2;	//if bfr_ptr is odd, add 1 to make it even. Chunk modifiers only start at even bytes.
			}
		if( !strncmp(bfr+bfr_ptr,"INFO",4)){
			bfr_ptr+=4;	//INFO
			memcpy( reinterpret_cast<char*>(&chunk_size), bfr+bfr_ptr, 4);
			chunk_size= make_littleendian( chunk_size );
			bfr_ptr+=4;	//size
			INFO_parser( bfr+bfr_ptr, chunk_size );
			bfr_ptr+=chunk_size;
			bfr_ptr+=bfr_ptr%2;	//if bfr_ptr is odd, add 1 to make it even. Chunk modifiers only start at even bytes.
			}
		if( !strncmp(bfr+bfr_ptr,"JUNK",4)){
			bfr_ptr+=4;	//JUNK
			memcpy( reinterpret_cast<char*>(&chunk_size), bfr+bfr_ptr, 4);
			chunk_size= make_littleendian( chunk_size );
			bfr_ptr+=4;	//size
			JUNK_parser( bfr+bfr_ptr, chunk_size );
			bfr_ptr+=chunk_size;
			bfr_ptr+=bfr_ptr%2;	//if bfr_ptr is odd, add 1 to make it even. Chunk modifiers only start at even bytes.
			}
		if( bfr_ptr < lenght-1 )	//if there is things on buffer, branch recursively
			correct &= LIST_parser( bfr+bfr_ptr, lenght-bfr_ptr, base+bfr_ptr );
		return correct;
		}

void DivFixppCore::JUNK_parser( const char* bfr, int lenght){
        //MemoLogWriter( wxString::FromAscii("JUNK: ")<< wxString::FromAscii( bfr ));
		}

void DivFixppCore::INFO_parser( const char* bfr, int lenght){
        //MemoLogWriter( wxString::FromAscii("INFO: \n") << wxString::FromAscii( bfr ));
		}

//Returns true if frame is not broken and copied successfully.
inline bool DivFixppCore::frame_copy( unsigned pos, bool KeepOriginalFile, bool CutOutBadParts, bool Error_Check_Mode){
	int temp;
	int frame_size;
	input->Seek( pos , wxFromStart);
	if( input->Error() ){
			MemoLogWriter(wxString(_("Error: "))+_("Input file seek error.\n"),true);
			end++;
			//close_files();
			return false;
			}
	input->Read( buffer, 16 );
	if( input->Error() ){
			MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);
			end++;
			//close_files();
			return false;
			}

	if( is_frame(buffer) ){
		memcpy(reinterpret_cast<char*>(&frame_size),buffer+4, 4);
		frame_size= make_littleendian( frame_size );
		//input.read( reinterpret_cast<char*>(&frame_size), 4);
		read_position += 8;					// FrameID + Size
		if( frame_size < 0 ){
			MemoLogWriter(_("Warning: Negative frame size detected!\n"));
			return false;
			}
		if( frame_size == 0 || frame_size == 16 ){
			if( is_frame(buffer+16) ){
				MemoLogWriter(_("Index frames detected.\n"));
				end++;
				return false;
				}
	//		else
	//			MemoLogWriter(wxString::Format(_("Weird frame detected. (%d Bytes frame at non-index section)\n"),frame_size));
			}
		if( frame_size >= buffer_size ){
			MemoLogWriter(_("Warning: Frame size is too big! Dropped\n"));
			return false;
			}
		// read_position -= 16;
		// input.seekg( read_position, ios::cur);	// Returns frame start point
		// input.read( buffer, frame_size+8-16);	// Read frameID and size
		if( frame_size > 7)
			input->Read( buffer+16, frame_size+8-16 );	// New Code to avoid rereading 8 bytes
			if(input->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);
			 //close_files();
			 return false; }

		read_position += frame_size;				// increase position within frame_size
		// input.seekg(-4, ios::cur);               // Point's next frame check for next frame is avail able or not
		// && is_frame(buffer+frame_size+8) ){      // this might be useful to describe this frame is corrupt or not

		if( KeepOriginalFile || CutOutBadParts ){ // don't write at only header/index fix
			if(write_position%2){					// Even Byte Padding
				write_position++;					// necessery to put frames to even positions
				if( !KeepOriginalFile && !Error_Check_Mode ){		// At Overwriting mode padding leaves some char t padded area.
					output->Seek( write_position-1 , wxFromStart);	// this fix that situations.
					if(output->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Output file seek error.\n"),true);	return false; }
					output->Write( "\0", 1);
					if(output->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Output file write error.\n"),true);	return false; }
					}
				}
			if(!Error_Check_Mode){
				output->Seek( write_position , wxFromStart);
				if(output->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Output file seek error.\n"),true);	return false; }
				output->Write( buffer, frame_size+8);	// +8 for FrameID+Size
				if(output->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Output file write error.\n"),true);	return false; }
				}
			}
	/*-------------------------------index writer part--------------------------*/
		if( index_list_ptr >= index_list+index_size-16){	// Checks if there are available memory at index buffer
			MemoLogWriter(wxString(_("Error: "))+wxString::Format(_("Index buffer size (%d) is not enought.\n"),index_size),true);
			return false;
			}
		memcpy(index_list_ptr, buffer, 4);	// Writes FrameID to Index
		index_list_ptr+=4;
		short stream_no;
		if(!strncmp(buffer+2,"wb",2)){		// Audio Frame (wb stand for Wave Binary?)
			stream_no = atoi( buffer );		// Taking Stream Number		decreasing performance?
			frame_counter[stream_no]++;		// Stream's Frame count ++
			temp = 16;				// Audio Frames are key frame
			}
		else if(!strncmp(buffer+2,"dc",2)){// Video Frame (dc stand for Data Compressed?)
			stream_no = atoi( buffer );		// Taking Stream Number		decreasing performance?
			frame_counter[stream_no]++;		// Stream's Frame count ++
			if ( is_keyframe( buffer ) )
				temp = 16;
			else if( !strncmp( four_cc, "H264", 4 ) || !strncmp( four_cc, "AVC1", 4 )){
				unsigned flag;
				memcpy( reinterpret_cast< char* >(&flag), buffer+8, 4 );
				flag = make_littleendian( flag );
				if( frame_counter[stream_no] == 1 )		// First frame of h264 is always keyframe.
					temp = 16;
				else if(( (flag & 0x01000000) && !(buffer[8+4] & 0xF0))	// if nal_ref_idc zero
						||
						( (flag & 0x010000) && !(buffer[8+3] & 0xF0)))
					temp = 0x4000;						// I don't know this flags meaning. Just reverse engineered value.
				else
					temp = 0;
				}
			else
				temp = 0;
			}

		else if(!strncmp(buffer+2,"db",2)){		// Video Frame (db stand for Data Binary?)
			stream_no = atoi( buffer );		// Taking Stream Number		decreasing performance?
			frame_counter[stream_no]++;		// Stream's Frame count ++
			temp=16;				// db's are key frame
			}
		else{
			buffer[4]=0;
			MemoLogWriter(wxString::Format(_("Warning: Unknown Frame ID: %s detected at Position %u\n"),buffer,pos));
			return false;
			}
		memcpy(index_list_ptr, reinterpret_cast<char*>(&make_littleendian(temp)) , 4);		//Key Frame or Normal Frame?
		index_list_ptr+=4;
		temp = write_position - stream_start;							//Frame Location
		memcpy(index_list_ptr, reinterpret_cast<char*>(&make_littleendian(temp)) , 4);
		index_list_ptr+=4;
		memcpy(index_list_ptr, reinterpret_cast<char*>(&make_littleendian(frame_size)), 4);//Frame Size
		index_list_ptr+=4;
		write_position += frame_size+8;    // +8 for FrameID + size
		if( !Error_Check_Mode)
			return true;
		return true;
		}

	else if( !strncmp( buffer, "LIST", 4) ){
		int chunk_size;
		memcpy(reinterpret_cast<char*>(&chunk_size), buffer+4, 4);
		chunk_size= make_littleendian( chunk_size );
		read_position += 8;
		input->Seek( read_position, wxFromStart );
		if(input->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Input file seek error.\n"),true); return false; }
		input->Read( buffer, chunk_size );
		if(input->Error()){ MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true); return false; }
		read_position += search_frame( buffer, chunk_size, false );
		return true;
		//return LIST_parser( buffer, chunk_size, 0 );
		}

	else if( !strncmp( buffer, "rec ", 4) ){
		read_position += 4;
		return true;
		}

	else if( !strncmp(buffer, "idx1", 4) ){
		MemoLogWriter(wxString(_("Info: "))+_("Original index chunk found at " ) << read_position << wxT("\n"));
		int idx_size = 0;
		memcpy(reinterpret_cast<char*>(&idx_size), buffer+4, 4);	//read index size
		idx_size = make_littleendian(idx_size);
		read_position += idx_size + 8; // 8 for idx1 + size
		input->Seek( read_position, wxFromStart );
		if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file seek error.\n"),true);close_files(true);return false;}
		input->Read( buffer, 24 );
		if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);close_files(true);return false;}
		if( !strncmp(buffer, "RIFF", 4) && !strncmp(buffer+8, "AVIX", 4) &&
			!strncmp(buffer+12, "LIST", 4) && !strncmp(buffer+20, "movi", 4) ){
				read_position += 24;
				return frame_copy( read_position, KeepOriginalFile, CutOutBadParts, Error_Check_Mode);
				}
		return true;
		}
	else if( !strncmp(buffer,"ix", 2) ){
		//MemoLogWriter(wxString(_("Info: "))+_("ix frame found.\n" ));
		MemoLogWriter(wxString(_("Info: ")) << _("Standard index chunk found at " ) << read_position << wxT("\n"));
		int ix_size = 0;
		memcpy(reinterpret_cast<char*>(&ix_size), buffer+4, 4);
		ix_size = make_littleendian(ix_size);
		read_position += ix_size + 8; // 8 for ix00/ix01 + size
//		return frame_copy( read_position, KeepOriginalFile, CutOutBadParts, Error_Check_Mode); //Not necessary and req eof() check here
		return true; //remove error warning and make eof() check
		}
	else
		return false;
	}

bool DivFixppCore::Fix( wxString Source, wxString Target,
						bool KeepOriginalFile,
						bool CutOutBadParts,
						bool Error_Check_Mode,
						bool RecoverFromKeyFrame ){
	KeepOrg = KeepOriginalFile;
	target_file = Target;
	int temp;
	int jump;
	int stream_size;
	memset(index_list , 0, index_size);
	index_list_ptr = index_list ;

//Old code deleteable	strh = new _strh[stream_limit];
	for(int i=0; i<stream_limit ; i++){
		strh[i].stream_header = NULL;
		strh[i].position = 0;
		frame_counter[i] = 0;
		frame_type[i][0] = 0;
		frame_type[i][1] = 0;
		}

	MemoLogWriter( _("Processing file : "));
	MemoLogWriter( Source.AfterLast(wxFileName::GetPathSeparator())+ _T("\n"));

	int StreamID = IdentifyStreamType( Source );
	if( StreamID == MKV ){
		//Here branch to Meteorite Code
		#ifdef _project_Meteorite_
		MemoLogWriter(wxString(_T("Matroska/MKV file detected!\n"))+
							   _T("GUI implementetion is not ready now...\n")+
							   _T("Meteorite inspecting and fixing your file.")
							   ,false);
		Meteorite the_meteorite_object( WxGauge, WxMemoLog, WxProgress);
		return the_meteorite_object.Repair( (string)Source.ToAscii(), (string)Target.ToAscii() );
		#else
		MemoLogWriter(wxString(_T("Matroska/MKV file detected!\n"))+
							   _T("If you really need MKV support,\n")+
							   _T("Please check project_Meteorite home page.\n")+
							   _T("http://meteorite.sourceforge.net\n")
							   ,true);
		return false;
		#endif
		}
	else if( StreamID == AVI ){
		//Go on for AVI fix...
		}
	else{
		#ifdef _project_Meteorite_
			MemoLogWriter(wxString(_("Error: "))+_("Input file is not an AVI or MKV file!\n"));
		#else
			MemoLogWriter(wxString(_("Error: "))+_("Input file is not an AVI file!\n"));
		#endif
		close_files(true);
		return false;
		}

	if(KeepOriginalFile){
		output = tempout;
		if(! input->Open( Source, _T("rb") )){	MemoLogWriter( wxString(_("Error: "))+_("Input file cannot be opened!\n"),true ); input->Close(); return false; }
		if(! Error_Check_Mode )	// if this is not DivFix error check
			if(! output->Open( Target, _T("wb+") )){MemoLogWriter( wxString(_("Error: "))+_("Output file cannot opened!\n"),true ); close_files(); return false;}
		}
	else{					//if we don't keep original file
		if(! input->Open( Source, _T("ab+"))){ MemoLogWriter( wxString(_("Error: "))+_("Input file cannot be opened!\n"),true ); input->Close(); return false; }
		tempout = output;	//input and output is the same file!
		output = input;
		}
//
//	input->Seek( 8 , wxFromStart);
//	if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);close_files(true);return false;}
//
//	input->Read( buffer, 8);
//	if( strncmp(buffer,"AVI LIST",8 )){	MemoLogWriter(wxString(_("Error: "))+_("Input file is not an AVI file!\n"),true);close_files(true);return false;}

	input->Seek( 188 , wxFromStart );
	if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file seek error.\n"),true);close_files(true);return false;}
	input->Read( four_cc, 4);
	if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);close_files(true);return false;}
	four_cc[4] = 0;
	for( int i = 0 ; i < 4 ; i ++ )	//string upper
		if(four_cc[i] >= 'a'  &&  four_cc[i] <= 'z')
			four_cc[i] -= 'a'-'A';
	MemoLogWriter(  _("Four CC is : ") + wxString::FromAscii(four_cc) + _T("\n") );

	for(read_position = 16,jump = 0 ; strncmp(buffer,"movi",4) ;read_position += 8 ){
		read_position += jump;
		input->Seek( read_position, wxFromStart );
		if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file seek error.\n"),true);close_files(true);return false;}
		input->Read( reinterpret_cast<char*>(&jump), 4 );
		jump = make_littleendian(jump);
		if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);close_files(true);return false;}
		input->Read( buffer, 4 );
		if( input->Eof() ){	MemoLogWriter(_("Input's movi section not found!\n"),true); close_files(true); return false; }
		if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);close_files(true);return false;}
		}

	write_position = read_position;
	stream_start = read_position-4;
	input->Seek(0, wxFromStart);
	if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file seek error.\n"),true);close_files(true);return false;}
	input->Read( buffer, read_position);
	if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);close_files(true);return false;}
	if(KeepOriginalFile && !Error_Check_Mode){
		output->Seek( 0, wxFromStart);
		if(output->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Output file seek error.\n"),true);close_files(true);return false;}
		output->Write( buffer, read_position);
		if(output->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Output file write error.\n"),true);close_files(true);return false;}
		}

	memcpy(reinterpret_cast<char*>(&stream_size), buffer+read_position-8, 4);	//LIST****movi
	stream_size = make_littleendian(stream_size);

	int error_count=0;
//	while( read_position < stream_size+stream_start ){
	unsigned long long maxinputsize = input->Length();	//fixing input size because of overwriting could change input size(?).
	while( read_position < maxinputsize ){
		if(read_position%2){
			read_position++;	// To frame start at even byte
			write_position++;
			if( !KeepOriginalFile && !Error_Check_Mode){		// At Overwriting mode padding leaves some char t padded area.
				output->Seek( write_position-1 , wxFromStart);	// this fix that situations.
				if(output->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Output file seek error.\n"),true);close_files(true);return false;}
				output->Write( "\0", 1);
				if(output->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Output file write error.\n"),true);close_files(true);return false;}
				}
			}
		// Check frame and copy required one with frame_copy.
		if( !frame_copy( read_position, KeepOriginalFile, CutOutBadParts, Error_Check_Mode ) ){		//copys frames
			//if there is an problem on stream, IP branch here...
			if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file error.\n"),true);close_files(true);return false;}
			if(!Error_Check_Mode)
				if(output->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Output file error.\n"),true);close_files(true);return false;}
			error_count++;
			MemoLogWriter( wxString::Format( _( "Error detected at byte: %u\n"), read_position ) );
			for( jump = 0; !input->Eof() ;){		//removes free regions at file // I don't trust Eof(), it can give false negatives
				if( !wxThread::This()->IsMain() )	//Checks if functions is running on thread
					if( wxThread::This()->TestDestroy() ){	//Checks if thread has termination request
						close_files( true );				//Releases files
						MemoLogWriter(_("Operation stoped by user.\n"));
						return false;
						}
				input->Seek( read_position, wxFromStart );
				if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file seek error.\n"),true);close_files(true);return false;}
				input->Read( buffer, buffer_size );
				if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);close_files(true);return false;}
				temp = search_frame( buffer, buffer_size, RecoverFromKeyFrame );
				if(temp != -1){						//if frame detected in buffer
					MemoLogWriter(wxString::Format(_("Skipped %d bytes.\n"),jump+temp));
					read_position += temp;
					if( !CutOutBadParts ){
						write_position += temp;
						if( KeepOriginalFile && !Error_Check_Mode ){
							output->Seek( write_position-temp, wxFromStart);
							output->Write( buffer, temp );
							if(output->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Output file write error.\n"),true);close_files(true);return false;}
							}
						}

					if( read_position >= stream_size+stream_start ){
						input->Seek( read_position, wxFromStart );
						if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file seek error.\n"),true);close_files(true);return false;}
						input->Read( buffer, 96);
						if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);close_files(true);return false;}
						bool index_check = false;
						if( strncmp(four_cc, "MJPG",4))
							for( int i = 0 ; i<3 ; i++ )	//check 3 frame
								index_check &= is_frame( buffer+i*16 );

						if( index_check ){
							MemoLogWriter( wxString::Format( _( "Broken index chunk found at byte: %u\n"), read_position ) );
							while( index_check ){
								input->Seek( read_position, wxFromStart );
								if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file seek error.\n"),true);close_files(true);return false;}
								int read_size = input->Read( buffer, buffer_size );
								if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);close_files(true);return false;}
								if( read_size < 16 )
									break;
								for( int i = 0 ; i < read_size/16 ; i++ ){
									index_check = is_frame( buffer + i*16 );
									if( index_check ){
										read_position += 16;
										}
									else
										break;
									}
								}
							}
						else{
							input->Seek( -128, wxFromCurrent);
							if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file seek error.\n"),true);close_files(true);return false;}
							}
						}

					input->Seek( read_position, wxFromStart );
					if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file seek error.\n"),true);close_files(true);return false;}
					input->Read( buffer, 4);
					if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);close_files(true);return false;}
					input->Seek( -4, wxFromCurrent);
					if(is_frame(buffer,RecoverFromKeyFrame))
						break;
					}
				else{								//if there is no frame detected in buffer
					read_position += buffer_size - 16;
					if( !CutOutBadParts ){
						write_position += buffer_size - 16;
						if( KeepOriginalFile && !Error_Check_Mode ){
							output->Write( buffer, buffer_size - 164 );
							if(output->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Output file write error.\n"),true);close_files(true);return false;}
							}
						}
					jump += buffer_size - 16;
					}
				if( !update_gauge( static_cast<int>(read_position*100.0/input->Length() )) )
					return false;
				if(is_frame(buffer, RecoverFromKeyFrame))
					break;
				if( read_position > maxinputsize ) {MemoLogWriter(_("File end reached.\n"));break;}	//wxFFile->Eof() untrust code
				}
			}
		if( !wxThread::This()->IsMain() )	//Checks if functions is running on thread
			if( wxThread::This()->TestDestroy() ){	//Checks if thread has termination request
				close_files(true);				//Releases files
				MemoLogWriter(_("Operation stoped by user.\n"));
				return false;
				}

		if( !update_gauge( static_cast<int>(read_position*100.0/input->Length()) ) )
			return false;

		if( input->Eof() ){
			MemoLogWriter(wxString(_("Info: ")) + _("File end reached.\n"));
			break;
			}
		if(end > 0) break;
		}

	//index writer
	if(write_position%2) write_position++;
	stream_size = write_position - stream_start;

	if(!CutOutBadParts ){ //index placing to it's original place
		input->Seek( stream_start - 4 , wxFromStart );
		if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file seek error.\n"),true);close_files(true);return false;}
		input->Read( reinterpret_cast<char*>(&stream_size), 4 );
		stream_size = make_littleendian(stream_size);

		if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);close_files(true);return false;}
		// Copying original AVI's index to new created file
		// It's not neccesery but Eligable :)
		if(KeepOriginalFile && !Error_Check_Mode ){
			int file_size;
			input->Seek( 4, wxFromStart );
			if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file seek error.\n"),true);close_files(true);return false;}
			input->Read( reinterpret_cast<char*>(&file_size), 4);
			file_size = make_littleendian(file_size);
			if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);close_files(true);return false;}
			input->Seek( (stream_start+stream_size), wxFromStart );
			if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file seek error.\n"),true);close_files(true);return false;}
			output->Seek( stream_start+stream_size, wxFromStart );
			if(output->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Output file seek error.\n"),true);close_files(true);return false;}
			temp = file_size+8 - (stream_start+stream_size);
			while( temp > buffer_size ){
				temp -= buffer_size;
				input->Read( buffer, buffer_size );
				if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);close_files(true);return false;}
				output->Write( buffer, buffer_size);
				if(output->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Output file write error.\n"),true);close_files(true);return false;}
				}
			input->Read( buffer, temp);
			if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);close_files(true);return false;}
			output->Write( buffer, temp);
			if(output->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Output file write error.\n"),true);close_files(true);return false;}
			}
		}

	if(!Error_Check_Mode){
		output->Seek( stream_start - 4 , wxFromStart );
		if(output->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Output file seek error.\n"),true);close_files(true);return false;}
		output->Write( reinterpret_cast<char*>(&make_littleendian(stream_size)), 4 );
		if(output->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Output file write error.\n"),true);close_files(true);return false;}
		output->Seek( stream_size, wxFromCurrent);
		if(output->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Output file seek error.\n"),true);close_files(true);return false;}
		output->Write( "idx1", 4 );
		if(output->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Output file write error.\n"),true);close_files(true);return false;}

		write_position+=4;
		temp = (index_list_ptr - index_list);
//		temp = (frame_counter[0]+frame_counter[1])*16; // Alternative list size computation
		output->Write( reinterpret_cast<char*>(&make_littleendian(temp)), 4 );	// index table size
		if(output->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Output file write error.\n"),true);close_files(true);return false;}
		write_position+=4;
		output->Write( index_list, temp );					// index table
		if(output->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Output file write error.\n"),true);close_files(true);return false;}
		write_position+=temp;
		if(!CutOutBadParts ){// correcting file size
			output->Seek( 4 , wxFromStart );
			if(output->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Output file seek error.\n"),true);close_files(true);return false;}
			output->Read( reinterpret_cast<char*>(&write_position), 4 );
			write_position = make_littleendian(write_position);
			if(output->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Output file read error.\n"),true);close_files(true);return false;}
			write_position += 8;
			junk_padding( 2048, true );
			}
		else{
			temp = write_position - 8;
			output->Seek( 4, wxFromStart );
			if(output->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Output file seek error.\n"),true);close_files(true);return false;}
			output->Write( reinterpret_cast<char*>(&make_littleendian(temp)), 4 );
			if(output->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Output file write error.\n"),true);close_files(true);return false;}
			if( !KeepOriginalFile && CutOutBadParts ){
				// wx has no truncation function. Can you believe this?
				output->Close();								// File closing for truncation
				Truncate( Source, write_position );				// Truncating file
				if(! output->Open( Source, _T("rb+") ))// Opens file as WX
					MemoLogWriter(wxString(_("Error: "))+_("Output file cannot reopened after truncation.\n"),true);
				else
					MemoLogWriter(_("File size decreased.\n"));
				}
			}
		}
	if(!Error_Check_Mode){
		avi_header_fix();
		junk_padding( 2048 );
		}

	MemoLogWriter(wxString::Format(
		 _("\nNew Stream Size \t: %d\n"\
			"Total Error Count \t: %d\n"\
			"Total Frame Count \t: %d\n"\
			"Video Data Frames \t: %d\n"\
			"Audio Data Frames \t: %d\n"),
		 stream_size, error_count, frame_counter[0]+frame_counter[1], frame_counter[0], frame_counter[1]));

	close_files();
	update_gauge( 0 );
	return true;
	}

bool DivFixppCore::update_gauge( int percent ){	//return indicate program live, false on kill req
	if(WxGauge){
		if( WxGauge->GetValue() != percent ){		//Checks value is changed or not
			wxMutexGuiEnter();
			WxGauge->SetValue( percent );
			wxYield();
			wxMutexGuiLeave();
			}
		}
	else if( WxProgress ){
		if(! WxProgress->Update( percent ) ){	//Progress Dialog update
			close_files(true);					//Progress dialog temination
			return false;
			}
		}
	else{
		wxString value;
		wxGetEnv( wxT("COLUMNS"), &value);
		std::cout << "\r" << value.ToAscii() << " "  << target_file.ToAscii() << "\t\%" << percent;
		}
	return true;
	}

void DivFixppCore::close_files( bool delete_output ){
	input->Close();
	output->Close();
	if( KeepOrg && delete_output )
		wxRemoveFile( target_file );
	}

bool DivFixppCore::Strip( wxString strip_file ){
	if(! input->Open( strip_file.c_str(), _T("rb+") ) ){
		MemoLogWriter( wxString(_("Error: "))+_("Input file cannot be opened!\n"),true );
		input->Close();
		return false;
		}

	memset( buffer, '\0', 4 );
	int jump;
	for(read_position = 16,jump = 0 ; strncmp(buffer,"movi",4) ;read_position += jump+8 ){
		input->Seek( read_position, wxFromStart );
		if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file seek error.\n"),true);close_files();return false;}
		input->Read( reinterpret_cast<char*>(&jump), 4 );
		jump = make_littleendian( jump );
		if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);close_files();return false;}
		input->Read( buffer, 4 );
		if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);close_files();return false;}
		if( input->Eof() ){MemoLogWriter(wxString(_("Error: "))+_("Input's idx1 section not found!\n"));close_files();	return false;}
		}
	input->Close();							//File closing for truncation
	return Truncate(strip_file, read_position-4);
	}

bool DivFixppCore::Truncate( wxString cut_filename, unsigned cut_here ){
	//Hand made low level truncate function
	int fdes;									// File descriptor
	fdes = open( cut_filename.ToAscii(), O_RDWR );	// Opens file descriptor
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

int DivFixppCore::IdentifyStreamType( wxString Source){
	if(! input->Open( Source, _T("rb"))){ MemoLogWriter( wxString(_("Error: "))+_("Input file cannot be opened! ")+_T("at IsAvi()\n") ,true);input->Close();return false;}
	input->Seek( 0 , wxFromStart);
	if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file seek error.\n"),true);close_files();return false;}
	input->Read( buffer, 16);
	if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);close_files();return false;}

	close_files();

	if( !strncmp(buffer+8,"AVI LIST",8 ))
		return 1;
	else if( 0xA3DF451A == make_littleendian(*reinterpret_cast<unsigned int*>(buffer)) )	// EBML / Matroska header
		return 2;
	else
		return 0;
	}

bool DivFixppCore::HasAVIGotProperIndex( wxString Source ){
	if(! input->Open( Source, _T("rb"))){MemoLogWriter(wxString(_("Error: "))+_("Input file cannot be opened!\n"),true);	input->Close();return false; }
	int jump = 0;
	int filesize = 0;
	input->Seek( 4, wxFromStart );
	if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file seek error.\n"),true);close_files();return false;}
	input->Read( reinterpret_cast<char*>(&filesize), 4 );
	filesize = make_littleendian( filesize );
	if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);close_files();return false;}
	input->Seek( 8 , wxFromStart);
	if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file seek error.\n"),true);close_files();return false;}
	input->Read( buffer, 8);
	if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);close_files();return false;}
	if( strncmp(buffer,"AVI LIST",8 )){	MemoLogWriter(wxString(_("Error: "))+_("Input file is not an AVI file!\n")); input->Close();return false; }
	for(read_position = 16,jump = 0 ; strncmp(buffer,"movi",4) ;read_position += jump+8 ){
		input->Seek( read_position, wxFromStart );
		if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file seek error.\n"),true);close_files();return false;}
		input->Read( reinterpret_cast<char*>(&jump), 4 );
		jump = make_littleendian( jump );
		if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);close_files();return false;}
		input->Read( buffer, 4 );
		if( input->Eof() ){	MemoLogWriter(wxString(_("Error: "))+_("Input's idx1 section not found!\n")); input->Close();return false; }
		if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);close_files();return false;}
		}
	int file_index_size;
	input->Seek( read_position-4, wxFromStart );
	if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file seek error.\n"),true);close_files();return false;}
	input->Read( buffer, 4 );
	if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);close_files();return false;}
	if( strncmp(buffer,"idx1",4 )) {
		MemoLogWriter(wxString(_("Error: "))+_("Input's idx1 section not found!\n"));
		return false;
		}
	input->Seek( read_position, wxFromStart );
	if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file seek error.\n"),true);close_files();return false;}
	input->Read( reinterpret_cast<char*>(&file_index_size), 4 );
	file_index_size = make_littleendian( file_index_size );
	if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);close_files();return false;}
	for(; 0 < file_index_size ; file_index_size -= 16){
		input->Read( buffer, 16 );
	if( input->Error() ){MemoLogWriter(wxString(_("Error: "))+_("Input file read error.\n"),true);close_files();return false;}
		if( input->Eof() || !is_frame(buffer))// index is broken
			return false;
		}
	input->Close();
	return true;
	}

inline void DivFixppCore::MemoLogWriter( wxString message, bool Error ){
	if(WxMemoLog != NULL){
		//Optionaly we can check if its main thread or not for locking...
		if( ! wxThread::This()->IsMain() )
			wxMutexGuiEnter();
		WxMemoLog->AppendText( message );
		if( ! wxThread::This()->IsMain() )
			wxMutexGuiLeave();
		}
	else if( Error )
		wxLogError(message);
	}

