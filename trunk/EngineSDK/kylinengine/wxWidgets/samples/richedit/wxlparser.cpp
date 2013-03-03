/*-*- c++ -*-********************************************************
 * wxlparser.h : parsers,  import/export for wxLayoutList           *
 *                                                                  *
 * (C) 1998,1999 by Karsten Ball�der (Ballueder@usa.net)            *
 *                                                                  *
 * $Id: wxlparser.cpp,v 1.14 2004/10/06 20:54:17 ABX Exp $
 *******************************************************************/

#ifdef __GNUG__
#   pragma implementation "wxlparser.h"
#endif

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#  pragma hdrstop
#endif

#include "Mpch.h"

#ifdef M_PREFIX
#   include "gui/wxllist.h"
#   include "gui/wxlparser.h"
#else
#   include "wxllist.h"
#   include "wxlparser.h"
#endif

#define   BASE_SIZE 12

inline static bool IsEndOfLine(const wxChar *p)
{
   // the end of line is either just '\n' or "\r\n" - we understand both (even
   // though the second is used only under DOS/Windows) to be able to import
   // DOS text files even under Unix
   return (*p == '\n') || ((*p == '\r') && (*(p + 1) == '\n'));
}

void wxLayoutImportText(wxLayoutList *list, wxString const &str)
{
   if ( !str )
      return;

   // we change the string only temporarily inside this function
   // VZ: I still don't like it... the string data may be shared...
   wxChar * cptr = (wxChar *)str.c_str(); // const_cast
   const wxChar * begin = cptr;
   wxUnusedVar(begin);
   wxChar  backup;

   for(;;)
   {
      begin = cptr;
      while( *cptr && !IsEndOfLine(cptr) )
         cptr++;
      backup = *cptr;
      *cptr = '\0';
      list->Insert(begin);
      *cptr = backup;

      // check if it's the end of this line
      if ( IsEndOfLine(cptr) )
      {
         // if it was "\r\n", skip the following '\n'
         if ( *cptr == '\r' )
            cptr++;
         list->LineBreak();
      }
      else if(backup == '\0') // reached end of string
         break;
      cptr++;
   }
}

static
wxString wxLayoutExportCmdAsHTML(wxLayoutObjectCmd const & cmd,
                                 wxLayoutStyleInfo *styleInfo,
                                 bool firstTime)
{
   static wxChar buffer[20];
   wxString html;

   wxLayoutStyleInfo *si = cmd.GetStyle();

   int size, sizecount;

   html += _T("<font ");

   if(si->m_fg_valid)
   {
      html += _T("color=");
      wxSprintf(buffer,_T("\"#%02X%02X%02X\""), si->m_fg.Red(),si->m_fg.Green(),si->m_fg.Blue());
      html += buffer;
   }

   if(si->m_bg_valid)
   {
      html += _T(" bgcolor=");
      wxSprintf(buffer,_T("\"#%02X%02X%02X\""), si->m_bg.Red(),si->m_bg.Green(),si->m_bg.Blue());
      html += buffer;
   }

   switch(si->family)
   {
   case wxSWISS:
   case wxMODERN:
      html += _T(" face=\"Arial,Helvetica\""); break;
   case wxROMAN:
      html += _T(" face=\"Times New Roman, Times\""); break;
   case wxTELETYPE:
      html += _T(" face=\"Courier New, Courier\""); break;
   default:
      ;
   }

   size = BASE_SIZE; sizecount = 0;
   while(size < si->size && sizecount < 5)
   {
      sizecount ++;
      size = (size*12)/10;
   }
   while(size > si->size && sizecount > -5)
   {
      sizecount --;
      size = (size*10)/12;
   }
   html += _T("size=");
   wxSprintf(buffer,_T("%+1d"), sizecount);
   html += buffer;

   html += _T(">");

   if(styleInfo != NULL && ! firstTime)
      html = _T("</font>")+html; // terminate any previous font command

   if((si->weight == wxBOLD) && ( (!styleInfo) || (styleInfo->weight != wxBOLD)))
      html += _T("<b>");
   else
      if(si->weight != wxBOLD && ( styleInfo && (styleInfo->weight == wxBOLD)))
         html += _T("</b>");

   if(si->style == wxSLANT)
      si->style = wxITALIC; // the same for html

   if((si->style == wxITALIC) && ( (!styleInfo) || (styleInfo->style != wxITALIC)))
      html += _T("<i>");
   else
      if(si->style != wxITALIC && ( styleInfo && (styleInfo->style == wxITALIC)))
         html += _T("</i>");

   if(si->underline && ( (!styleInfo) || ! styleInfo->underline))
      html += _T("<u>");
   else if(si->underline == false && ( styleInfo && styleInfo->underline))
      html += _T("</u>");


   *styleInfo = *si; // update last style info

   return html;
}



wxLayoutExportStatus::wxLayoutExportStatus(wxLayoutList *list)
{
   m_si = list->GetDefaultStyleInfo();
   m_line = list->GetFirstLine();
   m_iterator = m_line->GetFirstObject();
   m_FirstTime = true;
}



#define   WXLO_IS_TEXT(type) \
( type == WXLO_TYPE_TEXT \
  || (type == WXLO_TYPE_CMD \
      && mode == WXLO_EXPORT_AS_HTML))


wxLayoutExportObject *wxLayoutExport(wxLayoutExportStatus *status,
                                     int mode, int flags)
{
   wxLayoutObjectList::iterator nulled(NULL);
   wxASSERT(status);
   wxLayoutExportObject * exp;

   if(status->m_iterator == nulled) // end of line
   {
      if(!status->m_line || status->m_line->GetNextLine() == NULL)
         // reached end of list
         return NULL;
   }
   exp = new wxLayoutExportObject();
   wxLayoutObjectType type;
   if(status->m_iterator != nulled)
   {
      type = (** status->m_iterator).GetType();
      if( mode == WXLO_EXPORT_AS_OBJECTS || ! WXLO_IS_TEXT(type)) // simple case
      {
         exp->type = WXLO_EXPORT_OBJECT;
         exp->content.object = *status->m_iterator;
         status->m_iterator++;
         return exp;
      }
   }
   else
   {  // iterator == nulled
      if(mode == WXLO_EXPORT_AS_OBJECTS)
      {
         exp->type = WXLO_EXPORT_EMPTYLINE;
         exp->content.object = NULL; //empty line
         status->m_line = status->m_line->GetNextLine();
         if(status->m_line)
            status->m_iterator = status->m_line->GetFirstObject();
         return exp;
      }
      else
         type = WXLO_TYPE_TEXT;
   }
   wxUnusedVar(type);

   wxString *str = new wxString();
   // text must be concatenated
   for(;;)
   {
      while(status->m_iterator == nulled)
      {
         if(mode & WXLO_EXPORT_AS_HTML)
            *str += _T("<br>");
         if(flags & WXLO_EXPORT_WITH_CRLF)
            *str += _T("\r\n");
         else
            *str += '\n';

         status->m_line = status->m_line->GetNextLine();
         if(status->m_line)
            status->m_iterator = status->m_line->GetFirstObject();
         else
            break; // end of list
      }
      if(! status->m_line)  // reached end of list, fall through
         break;
      type = (** status->m_iterator).GetType();
      if(type == WXLO_TYPE_ICON)
         break;
      switch(type)
      {
      case WXLO_TYPE_TEXT:
         *str += ((wxLayoutObjectText *)*status->m_iterator)->GetText();
         break;
      case WXLO_TYPE_CMD:
         if(mode == WXLO_EXPORT_AS_HTML)
            *str += wxLayoutExportCmdAsHTML(
               *(wxLayoutObjectCmd const *)*status->m_iterator,
               & status->m_si, status->m_FirstTime);
         status->m_FirstTime = false;
         break;
      default:  // ignore icons
         ;
      }
      status->m_iterator++;
   }
   exp->type = (mode == WXLO_EXPORT_AS_HTML)
      ?  WXLO_EXPORT_HTML : WXLO_EXPORT_TEXT;
   exp->content.text = str;
   return exp;
}

