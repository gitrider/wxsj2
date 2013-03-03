///////////////////////////////////////////////////////////////////////////////
// Name:        tests/geometry/rect.cpp
// Purpose:     wxRect unit test
// Author:      Vadim Zeitlin
// Created:     2004-12-11
// RCS-ID:      $Id: rect.cpp,v 1.3 2005/02/04 11:04:43 VZ Exp $
// Copyright:   (c) 2004 wxWindows
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "testprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/gdicmn.h"
#endif // WX_PRECOMP

// ----------------------------------------------------------------------------
// test class
// ----------------------------------------------------------------------------

class RectTestCase : public CppUnit::TestCase
{
public:
    RectTestCase() { }

private:
    CPPUNIT_TEST_SUITE( RectTestCase );
        CPPUNIT_TEST( InflateDeflate );
        CPPUNIT_TEST( Operators );
        CPPUNIT_TEST( Union );
    CPPUNIT_TEST_SUITE_END();

    void InflateDeflate();
    void Operators();
    void Union();

    DECLARE_NO_COPY_CLASS(RectTestCase)
};

// register in the unnamed registry so that these tests are run by default
CPPUNIT_TEST_SUITE_REGISTRATION( RectTestCase );

// also include in it's own registry so that these tests can be run alone
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( RectTestCase, "RectTestCase" );

void RectTestCase::InflateDeflate()
{
    // This is the rectangle from the example in the documentation of wxRect::Inflate().
    const wxRect r1(10, 10, 20, 40);

    CPPUNIT_ASSERT(r1.Inflate( 10,  10)==wxRect(  0,   0, 40,  60));
    CPPUNIT_ASSERT(r1.Inflate( 20,  30)==wxRect(-10, -20, 60, 100));
    CPPUNIT_ASSERT(r1.Inflate(-10, -10)==wxRect( 20,  20,  0,  20));
    CPPUNIT_ASSERT(r1.Inflate(-15, -15)==wxRect( 20,  25,  0,  10));

    CPPUNIT_ASSERT(r1.Inflate( 10,  10)==r1.Deflate(-10, -10));
    CPPUNIT_ASSERT(r1.Inflate( 20,  30)==r1.Deflate(-20, -30));
    CPPUNIT_ASSERT(r1.Inflate(-10, -10)==r1.Deflate( 10,  10));
    CPPUNIT_ASSERT(r1.Inflate(-15, -15)==r1.Deflate( 15,  15));
}

void RectTestCase::Operators()
{
    // test + operator which works like Union but does not ignore empty rectangles
    static const struct RectData
    {
        int x1, y1, w1, h1;
        int x2, y2, w2, h2;
        int x, y, w, h;

        wxRect GetFirst() const { return wxRect(x1, y1, w1, h1); }
        wxRect GetSecond() const { return wxRect(x2, y2, w2, h2); }
        wxRect GetResult() const { return wxRect(x, y, w, h); }
    } s_rects[] =
    {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 2, 2 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 4, 4 },
        { 1, 1, 2, 2, 4, 4, 1, 1, 1, 1, 4, 4 },
        { 2, 2, 2, 2, 4, 4, 4, 4, 2, 2, 6, 6 },
        { 1, 1, 4, 4, 4, 4, 1, 1, 1, 1, 4, 4 }
    };

    for ( size_t n = 0; n < WXSIZEOF(s_rects); n++ )
    {
        const RectData& data = s_rects[n];

        CPPUNIT_ASSERT(
            ( data.GetFirst() + data.GetSecond() ) == data.GetResult()
        );

        CPPUNIT_ASSERT(
            ( data.GetSecond() + data.GetFirst() ) == data.GetResult()
        );
    }
}

void RectTestCase::Union()
{
    static const struct RectData
    {
        int x1, y1, w1, h1;
        int x2, y2, w2, h2;
        int x, y, w, h;

        wxRect GetFirst() const { return wxRect(x1, y1, w1, h1); }
        wxRect GetSecond() const { return wxRect(x2, y2, w2, h2); }
        wxRect GetResult() const { return wxRect(x, y, w, h); }
    } s_rects[] =
    {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 4, 4 },
        { 1, 1, 2, 2, 4, 4, 1, 1, 1, 1, 4, 4 },
        { 2, 2, 2, 2, 4, 4, 4, 4, 2, 2, 6, 6 },
        { 1, 1, 4, 4, 4, 4, 1, 1, 1, 1, 4, 4 }
    };

    for ( size_t n = 0; n < WXSIZEOF(s_rects); n++ )
    {
        const RectData& data = s_rects[n];

        CPPUNIT_ASSERT(
            data.GetFirst().Union(data.GetSecond()) == data.GetResult()
        );

        CPPUNIT_ASSERT(
            data.GetSecond().Union(data.GetFirst()) == data.GetResult()
        );
    }
}
