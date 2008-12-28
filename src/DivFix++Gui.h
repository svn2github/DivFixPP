///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 17 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __DivFixppGui__
#define __DivFixppGui__

#include <wx/intl.h>

#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/bmpbuttn.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/listbox.h>
#include <wx/gauge.h>
#include <wx/checkbox.h>
#include <wx/frame.h>
#include <wx/choice.h>
#include <wx/statbox.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define ID_SAVE 1000
#define ID_LOG 1001
#define ID_ADD 1002
#define ID_REMOVE 1003
#define ID_CLEAR 1004
#define ID_ABOUT 1005
#define ID_REBUID 1006
#define ID_STRIP 1007
#define ID_CHECKERRORS 1008
#define ID_STOP 1009
#define ID_CHK_LOGERRORS 1010
#define ID_LANG 1011

///////////////////////////////////////////////////////////////////////////////
/// Class DivFixpp_Gui
///////////////////////////////////////////////////////////////////////////////
class DivFixpp_Gui : public wxFrame 
{
	private:
	
	protected:
		wxBitmapButton* wxbitbtn_savepath;
		wxTextCtrl* textCtrl_savepath;
		wxBitmapButton* wxbitbtn_logpath;
		wxTextCtrl* textCtrl_logpath;
		wxStaticText* m_staticText1;
		wxStaticText* m_staticText2;
		wxListBox* FileListBox;
		wxTextCtrl* TextCtrl_log;
		wxGauge* m_gauge;
		wxButton* wxbtn_add;
		wxButton* wxbtn_remove;
		wxButton* wxbtn_clear;
		wxButton* wxbtn_preferences;
		wxButton* wxbtn_about;
		wxButton* wxbtn_fix;
		wxButton* wxbtn_strip;
		wxButton* wxbtn_checkerrors;
		wxButton* wxbtn_stop;
		wxCheckBox* wxchk_keeporiginal;
		wxCheckBox* wxchk_cutout;
		wxCheckBox* wxchk_logerrors;
		
		// Virtual event handlers, overide them in your derived class
		virtual void DivFixppClose( wxCloseEvent& event ){ event.Skip(); }
		virtual void OnPathClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnLogClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnAddClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnRemoveClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnClearClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnPreferencesClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnAboutClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnFixClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnStripClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCheckClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnStopClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnKeepOriginalClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnLogErrorClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		DivFixpp_Gui( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("DivFix++"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 627,348 ), long style = wxCAPTION|wxCLOSE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU|wxTAB_TRAVERSAL );
		~DivFixpp_Gui();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class PreferencesDialog_Gui
///////////////////////////////////////////////////////////////////////////////
class PreferencesDialog_Gui : public wxDialog 
{
	private:
	
	protected:
		wxChoice* wxchc_language;
		wxBitmapButton* wxbitbtn_player;
		wxTextCtrl* textCtrl_playerpath;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ){ event.Skip(); }
		virtual void OnPlayerClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		PreferencesDialog_Gui( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("DivFix++ Preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE );
		~PreferencesDialog_Gui();
	
};

#endif //__DivFix++Gui__
