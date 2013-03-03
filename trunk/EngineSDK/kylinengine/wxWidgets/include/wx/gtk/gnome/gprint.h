/////////////////////////////////////////////////////////////////////////////
// Name:        gprint.h
// Author:      Robert Roebling
// Purpose:     GNOME printing support
// Created:     09/20/04
// Copyright:   Robert Roebling
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __gprint_H__
#define __gprint_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface
#endif

// Include wxWindows' headers

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#if wxUSE_LIBGNOMEPRINT

#include "wx/print.h"
#include "wx/prntbase.h"
#include "wx/printdlg.h"

typedef struct _GnomePrintJob GnomePrintJob;
typedef struct _GnomePrintContext GnomePrintContext;
typedef struct _GnomePrintConfig GnomePrintConfig;

//----------------------------------------------------------------------------
// wxGnomePrintNativeData
//----------------------------------------------------------------------------

class wxGnomePrintNativeData: public wxPrintNativeDataBase
{
public:
    wxGnomePrintNativeData();
    virtual ~wxGnomePrintNativeData();
    
    virtual bool TransferTo( wxPrintData &data );
    virtual bool TransferFrom( const wxPrintData &data );
    
    virtual bool Ok() const { return true; }
    
    GnomePrintConfig* GetPrintConfig() { return m_config; }
    void SetPrintJob( GnomePrintJob *job ) { m_job = job; }
    GnomePrintJob* GetPrintJob() { return m_job; }
    
    
private:
    GnomePrintConfig  *m_config;
    GnomePrintJob     *m_job;
    
private:
    DECLARE_DYNAMIC_CLASS(wxGnomePrintNativeData)
};
    
//----------------------------------------------------------------------------
// wxGnomePrintFactory
//----------------------------------------------------------------------------

class wxGnomePrintFactory: public wxPrintFactory
{
public:
    virtual wxPrinterBase *CreatePrinter( wxPrintDialogData *data );
    
    virtual wxPrintPreviewBase *CreatePrintPreview( wxPrintout *preview, 
                                                    wxPrintout *printout = NULL, 
                                                    wxPrintDialogData *data = NULL );
    virtual wxPrintPreviewBase *CreatePrintPreview( wxPrintout *preview, 
                                                    wxPrintout *printout,
                                                    wxPrintData *data );
                                                    
    virtual wxPrintDialogBase *CreatePrintDialog( wxWindow *parent, 
                                                  wxPrintDialogData *data = NULL );
    virtual wxPrintDialogBase *CreatePrintDialog( wxWindow *parent, 
                                                  wxPrintData *data );

    virtual wxPageSetupDialogBase *CreatePageSetupDialog( wxWindow *parent,
                                                          wxPageSetupDialogData * data = NULL );
          
    virtual bool HasPrintSetupDialog();
    virtual wxDialog *CreatePrintSetupDialog( wxWindow *parent, wxPrintData *data );
    virtual bool HasOwnPrintToFile();
    virtual bool HasPrinterLine();
    virtual wxString CreatePrinterLine();
    virtual bool HasStatusLine();
    virtual wxString CreateStatusLine();
    
    virtual wxPrintNativeDataBase *CreatePrintNativeData();
};

//----------------------------------------------------------------------------
// wxGnomePrintDialog
//----------------------------------------------------------------------------

class wxGnomePrintDialog: public wxPrintDialogBase
{
public:
    wxGnomePrintDialog( wxWindow *parent,
                         wxPrintDialogData* data = NULL );
    wxGnomePrintDialog( wxWindow *parent, wxPrintData* data);
    ~wxGnomePrintDialog();

    wxPrintData& GetPrintData()
        { return m_printDialogData.GetPrintData(); }
    wxPrintDialogData& GetPrintDialogData()
        { return m_printDialogData; }
        
    wxDC *GetPrintDC();

    virtual int ShowModal();

    virtual bool Validate();
    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();

private:    
    // Implement some base class methods to do nothing to avoid asserts and
    // GTK warnings, since this is not a real wxDialog.
    virtual void DoSetSize(int WXUNUSED(x), int WXUNUSED(y),
                           int WXUNUSED(width), int WXUNUSED(height),
                           int WXUNUSED(sizeFlags) = wxSIZE_AUTO) {}
    virtual void DoMoveWindow(int WXUNUSED(x), int WXUNUSED(y),
                              int WXUNUSED(width), int WXUNUSED(height)) {}
                              
    void Init();
    wxPrintDialogData   m_printDialogData;
    
private:
    DECLARE_DYNAMIC_CLASS(wxGnomePrintDialog)
};

//----------------------------------------------------------------------------
// wxGnomePageSetupDialog
//----------------------------------------------------------------------------

class wxGnomePageSetupDialog: public wxPageSetupDialogBase
{
public:
    wxGnomePageSetupDialog( wxWindow *parent,
                            wxPageSetupDialogData* data = NULL );
    ~wxGnomePageSetupDialog();

    virtual wxPageSetupDialogData& GetPageSetupDialogData();

    virtual int ShowModal();

    virtual bool Validate();
    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();

private:    
    // Implement some base class methods to do nothing to avoid asserts and
    // GTK warnings, since this is not a real wxDialog.
    virtual void DoSetSize(int WXUNUSED(x), int WXUNUSED(y),
                           int WXUNUSED(width), int WXUNUSED(height),
                           int WXUNUSED(sizeFlags) = wxSIZE_AUTO) {}
    virtual void DoMoveWindow(int WXUNUSED(x), int WXUNUSED(y),
                              int WXUNUSED(width), int WXUNUSED(height)) {}
                              
    wxPageSetupDialogData   m_pageDialogData;
    
private:
    DECLARE_DYNAMIC_CLASS(wxGnomePageSetupDialog)
};

//----------------------------------------------------------------------------
// wxGnomePrinter
//----------------------------------------------------------------------------

class wxGnomePrinter: public wxPrinterBase
{
public:
    wxGnomePrinter(wxPrintDialogData *data = NULL);
    virtual ~wxGnomePrinter();

    virtual bool Print(wxWindow *parent,
                       wxPrintout *printout,
                       bool prompt = true);
    virtual wxDC* PrintDialog(wxWindow *parent);
    virtual bool Setup(wxWindow *parent);
    
    GnomePrintContext *GetPrintContext() { return m_gpc; }
    
private:
    GnomePrintContext *m_gpc;
    bool               m_native_preview;

private:
    DECLARE_DYNAMIC_CLASS(wxGnomePrinter)
    DECLARE_NO_COPY_CLASS(wxGnomePrinter)
};

//-----------------------------------------------------------------------------
// wxGnomePrintDC
//-----------------------------------------------------------------------------

class wxGnomePrintDC: public wxDC
{
public:
    wxGnomePrintDC( wxGnomePrinter *printer );
    ~wxGnomePrintDC();
    
    bool Ok() const;
    
    virtual void BeginDrawing() {}
    virtual void EndDrawing() {}

    bool DoFloodFill(wxCoord x1, wxCoord y1, const wxColour &col, int style=wxFLOOD_SURFACE );
    bool DoGetPixel(wxCoord x1, wxCoord y1, wxColour *col) const;
    void DoDrawLine(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2);
    void DoCrossHair(wxCoord x, wxCoord y);
    void DoDrawArc(wxCoord x1,wxCoord y1,wxCoord x2,wxCoord y2,wxCoord xc,wxCoord yc);
    void DoDrawEllipticArc(wxCoord x,wxCoord y,wxCoord w,wxCoord h,double sa,double ea);
    void DoDrawPoint(wxCoord x, wxCoord y);
    void DoDrawLines(int n, wxPoint points[], wxCoord xoffset = 0, wxCoord yoffset = 0);
    void DoDrawPolygon(int n, wxPoint points[], wxCoord xoffset = 0, wxCoord yoffset = 0, int fillStyle=wxODDEVEN_RULE);
    void DoDrawPolyPolygon(int n, int count[], wxPoint points[], wxCoord xoffset = 0, wxCoord yoffset = 0, int fillStyle=wxODDEVEN_RULE);
    void DoDrawRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height);
    void DoDrawRoundedRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height, double radius = 20.0);
    void DoDrawEllipse(wxCoord x, wxCoord y, wxCoord width, wxCoord height);
    void DoDrawSpline(wxList *points);
    
    bool DoBlit(wxCoord xdest, wxCoord ydest, wxCoord width, wxCoord height,
            wxDC *source, wxCoord xsrc, wxCoord ysrc, int rop = wxCOPY, bool useMask = false,
            wxCoord xsrcMask = wxDefaultCoord, wxCoord ysrcMask = wxDefaultCoord);
    void DoDrawIcon( const wxIcon& icon, wxCoord x, wxCoord y );
    void DoDrawBitmap( const wxBitmap& bitmap, wxCoord x, wxCoord y, bool useMask = false  );
    bool CanDrawBitmap() const { return true; }
    
    void DoDrawText(const wxString& text, wxCoord x, wxCoord y );
    void DoDrawRotatedText(const wxString& text, wxCoord x, wxCoord y, double angle);
    void Clear();
    void SetFont( const wxFont& font );
    void SetPen( const wxPen& pen );
    void SetBrush( const wxBrush& brush );
    void SetLogicalFunction( int function );
    void SetBackground( const wxBrush& brush );
    
    void DoSetClippingRegion(wxCoord x, wxCoord y, wxCoord width, wxCoord height);
    void DestroyClippingRegion();
    void DoSetClippingRegionAsRegion( const wxRegion &WXUNUSED(clip) ) { }
    
    bool StartDoc(const wxString& message);
    void EndDoc();
    void StartPage();
    void EndPage();
    
    wxCoord GetCharHeight() const;
    wxCoord GetCharWidth() const;
    bool CanGetTextExtent() const { return true; }
    void DoGetTextExtent(const wxString& string, wxCoord *x, wxCoord *y,
                     wxCoord *descent = (wxCoord *) NULL,
                     wxCoord *externalLeading = (wxCoord *) NULL,
                     wxFont *theFont = (wxFont *) NULL ) const;
                     
    void DoGetSize(int* width, int* height) const;
    void DoGetSizeMM(int *width, int *height) const;
    wxSize GetPPI() const;
    void SetAxisOrientation( bool xLeftRight, bool yBottomUp );
    void SetDeviceOrigin( wxCoord x, wxCoord y );
    
    virtual int GetDepth() const { return 24; }
  
    void SetBackgroundMode(int WXUNUSED(mode)) { }
    void SetPalette(const wxPalette& WXUNUSED(palette)) { }
    
    wxPrintData& GetPrintData() { return m_printData; }
    void SetPrintData(const wxPrintData& data) { m_printData = data; }
    
    static void SetResolution(int ppi);
    static int GetResolution();

private:
    static float ms_PSScaleFactor;
    
private:
    PangoContext           *m_context;
    PangoLayout            *m_layout;
    PangoFontDescription   *m_fontdesc;
    
    unsigned char           m_currentRed;
    unsigned char           m_currentGreen;
    unsigned char           m_currentBlue;
    wxPrintData             m_printData;
    
    wxGnomePrinter         *m_printer;
    GnomePrintContext      *m_gpc;

private:
    wxCoord XDEV2LOG(wxCoord x) const
    {
        wxCoord new_x = x - m_deviceOriginX;
        if (new_x > 0)
            return (wxCoord)((double)(new_x) / m_scaleX + 0.5) * m_signX + m_logicalOriginX;
        else
            return (wxCoord)((double)(new_x) / m_scaleX - 0.5) * m_signX + m_logicalOriginX;
    }
    wxCoord XDEV2LOGREL(wxCoord x) const
    {
        if (x > 0)
            return (wxCoord)((double)(x) / m_scaleX + 0.5);
        else
            return (wxCoord)((double)(x) / m_scaleX - 0.5);
    }
    wxCoord YDEV2LOG(wxCoord y) const
    {
        wxCoord new_y = y - m_deviceOriginY;
        if (new_y > 0)
            return (wxCoord)((double)(new_y) / m_scaleY + 0.5) * m_signY + m_logicalOriginY;
        else
            return (wxCoord)((double)(new_y) / m_scaleY - 0.5) * m_signY + m_logicalOriginY;
    }
    wxCoord YDEV2LOGREL(wxCoord y) const
    {
        if (y > 0)
            return (wxCoord)((double)(y) / m_scaleY + 0.5);
        else
            return (wxCoord)((double)(y) / m_scaleY - 0.5);
    }
    wxCoord XLOG2DEV(wxCoord x) const
    {
        wxCoord new_x = x - m_logicalOriginX;
        if (new_x > 0)
            return (wxCoord)((double)(new_x) * m_scaleX + 0.5) * m_signX + m_deviceOriginX;
        else
            return (wxCoord)((double)(new_x) * m_scaleX - 0.5) * m_signX + m_deviceOriginX;
    }
    wxCoord XLOG2DEVREL(wxCoord x) const
    {
        if (x > 0)
            return (wxCoord)((double)(x) * m_scaleX + 0.5);
        else
            return (wxCoord)((double)(x) * m_scaleX - 0.5);
    }
    wxCoord YLOG2DEV(wxCoord y) const
    {
        wxCoord new_y = y - m_logicalOriginY;
        if (new_y > 0)
            return (wxCoord)((double)(new_y) * m_scaleY + 0.5) * m_signY + m_deviceOriginY;
        else
            return (wxCoord)((double)(new_y) * m_scaleY - 0.5) * m_signY + m_deviceOriginY;
    }
    wxCoord YLOG2DEVREL(wxCoord y) const
    {
        if (y > 0)
            return (wxCoord)((double)(y) * m_scaleY + 0.5);
        else
            return (wxCoord)((double)(y) * m_scaleY - 0.5);
    }
private:
    DECLARE_DYNAMIC_CLASS(wxGnomePrintDC)
    DECLARE_NO_COPY_CLASS(wxGnomePrintDC)
};

#endif
    // wxUSE_LIBGNOMEPRINT

#endif
