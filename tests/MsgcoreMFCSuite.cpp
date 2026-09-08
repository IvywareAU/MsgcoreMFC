// Copyright © 2026 Ivyware Pty Ltd, Khrustal & Mann
//              MELBOURNE, VICTORIA, AUSTRALIA, 3000
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
// implied. See the License for the specific language governing
// permissions and limitations under the License.
//
// MsgcoreMFCSuite.cpp
//
// The headless half of this library: the exported helpers that need no window,
// no device context and no message pump, and can therefore be asserted on
// directly.
//
// WHAT IS DELIBERATELY NOT HERE. Most of MsgcoreMFC's surface takes a live
// CWnd, CDC or CListCtrl, and a test that creates one needs a message pump and
// a desktop. Those are not stubbed: a stub that always passes is worse than an
// absent case because it reads like coverage. The window-bound surface is
// genuinely untested, and saying so plainly is the honest state.
//
// The cases below assert the behaviour the implementations actually have,
// including two quirks that are easy to "fix" by accident:
//   * CString_Truncate uses `idx > 0`, so a separator in position 0 truncates
//     nothing and the whole string comes back.
//   * CString_Extract returns an EMPTY string when the separator is the last
//     character, not the whole string.
// Both are pinned below so a future tidy-up has to be deliberate.

#include <afx.h>
#include <afxwin.h>

#include "TestFramework.h"

#include "../CString_Ext.h"
#include "../CRect_Ext.h"
#include "../COleTime_Ext.h"
#include "../CStringParser.h"

// ---------------------------------------------------------------------------
static void CString_Ext_Cases()
{
    TF_CASE("CString_Truncate: cuts at the first separator")
    {
        TF_CHECK(CString_Truncate(CString(L"name.ext"), L".") == CString(L"name"));
        TF_CHECK(CString_Truncate(CString(L"a.b.c"),    L".") == CString(L"a"));
    }

    TF_CASE("CString_Truncate: no separator returns the whole string")
    {
        TF_CHECK(CString_Truncate(CString(L"noseparator"), L".") == CString(L"noseparator"));
    }

    TF_CASE("CString_Truncate: a leading separator truncates nothing (idx > 0)")
    {
        // Quirk, pinned deliberately: index 0 fails the `> 0` test.
        TF_CHECK(CString_Truncate(CString(L".hidden"), L".") == CString(L".hidden"));
    }

    TF_CASE("CString_Extract: returns the tail after the separator")
    {
        TF_CHECK(CString_Extract(CString(L"name.ext"), L".") == CString(L"ext"));
    }

    TF_CASE("CString_Extract: no separator returns the whole string")
    {
        TF_CHECK(CString_Extract(CString(L"noseparator"), L".") == CString(L"noseparator"));
    }

    TF_CASE("CString_Extract: a trailing separator yields empty, not the input")
    {
        // Quirk, pinned deliberately.
        TF_CHECK(CString_Extract(CString(L"trailing."), L".").IsEmpty());
    }

    TF_CASE("CString_RightCompare: matches and rejects a suffix")
    {
        TF_CHECK( CString_RightCompare(CString(L"filename.txt"), L".txt"));
        TF_CHECK(!CString_RightCompare(CString(L"filename.txt"), L".doc"));
    }

    TF_CASE("CString_FindNocase: case-insensitive position, -1 when absent")
    {
        TF_CHECK_EQ(CString_FindNocase(CString(L"Hello World"), CString(L"WORLD")), 6);
        TF_CHECK_EQ(CString_FindNocase(CString(L"Hello World"), CString(L"world")), 6);
        TF_CHECK_EQ(CString_FindNocase(CString(L"Hello World"), CString(L"zzz")), -1);
    }

    TF_CASE("CString_ReplaceTabs: expands to the next tab stop, not a fixed run")
    {
        // "a" puts the column at 1; the tab fills columns 1,2,3 to reach 4.
        TF_CHECK(CString_ReplaceTabs(CString(L"a\tb"), 4) == CString(L"a   b"));
        // A tab at column 0 fills a whole tab width.
        TF_CHECK(CString_ReplaceTabs(CString(L"\tb"), 4)  == CString(L"    b"));
    }

    TF_CASE("CString_FormatwithCommas: groups the integer part only")
    {
        TF_CHECK(CString_FormatwithCommas(CString(L"1234567"))  == CString(L"1,234,567"));
        TF_CHECK(CString_FormatwithCommas(CString(L"123"))      == CString(L"123"));
        TF_CHECK(CString_FormatwithCommas(CString(L"1234.5678")) == CString(L"1,234.5678"));
    }
}

// ---------------------------------------------------------------------------
static void CRect_Ext_Cases()
{
    const CRect oBase(0, 0, 10, 10);

    TF_CASE("CRect_Intersect: overlapping rectangles intersect")
    {
        TF_CHECK(CRect_Intersect(oBase, CRect(5, 5, 15, 15)));
        TF_CHECK(CRect_Intersect(oBase, CRect(-5, -5, 5, 5)));
    }

    TF_CASE("CRect_Intersect: a contained rectangle intersects")
    {
        TF_CHECK(CRect_Intersect(oBase, CRect(2, 2, 8, 8)));
    }

    TF_CASE("CRect_Intersect: separated rectangles do not")
    {
        TF_CHECK(!CRect_Intersect(oBase, CRect(20, 20, 30, 30)));
        TF_CHECK(!CRect_Intersect(oBase, CRect(0, 20, 10, 30)));
    }

    TF_CASE("CRect_Intersect: edge contact counts as intersecting")
    {
        // The test is >/< rather than >=/<=, so touching edges intersect.
        TF_CHECK(CRect_Intersect(oBase, CRect(10, 0, 20, 10)));
    }
}

// ---------------------------------------------------------------------------
static void COleTime_Ext_Cases()
{
    TF_CASE("YYYYMMDD round-trips through COleDateTime")
    {
        const COleDateTime oDate = YYYYMMDD_COleTime(20260908);
        TF_CHECK_EQ(oDate.GetYear(),  2026);
        TF_CHECK_EQ(oDate.GetMonth(), 9);
        TF_CHECK_EQ(oDate.GetDay(),   8);
        TF_CHECK_EQ(COleTime_YYYYMMDD(oDate), 20260908);
    }

    TF_CASE("YYYYMMDD round-trips across a year boundary")
    {
        TF_CHECK_EQ(COleTime_YYYYMMDD(YYYYMMDD_COleTime(20251231)), 20251231);
        TF_CHECK_EQ(COleTime_YYYYMMDD(YYYYMMDD_COleTime(20260101)), 20260101);
    }

    TF_CASE("COleTime_GetQuarter maps all twelve months")
    {
        const int nExpected[12] = { 1,1,1, 2,2,2, 3,3,3, 4,4,4 };
        for (int nMonth = 1; nMonth <= 12; ++nMonth)
        {
            const COleDateTime oDate(2026, nMonth, 1, 0, 0, 0);
            TF_CHECK_EQ(COleTime_GetQuarter(oDate), nExpected[nMonth - 1]);
        }
    }

    TF_CASE("COleTime_SoYear / EoYear bracket the year")
    {
        const COleDateTime oMid(2026, 7, 15, 12, 30, 0);
        const COleDateTime oSo = COleTime_SoYear(oMid);
        const COleDateTime oEo = COleTime_EoYear(oMid);

        TF_CHECK_EQ(oSo.GetYear(), 2026);
        TF_CHECK_EQ(oSo.GetMonth(), 1);
        TF_CHECK_EQ(oSo.GetDay(),   1);

        TF_CHECK_EQ(oEo.GetYear(), 2026);
        TF_CHECK_EQ(oEo.GetMonth(), 12);
        TF_CHECK_EQ(oEo.GetDay(),   31);

        TF_CHECK(oSo <= oMid);
        TF_CHECK(oMid <= oEo);
    }

    TF_CASE("COleTime_IsValid accepts a real date")
    {
        const COleDateTime oDate(2026, 9, 8, 0, 0, 0);
        TF_CHECK(COleTime_IsValid(oDate));
    }

    TF_CASE("Normalised2DATE and DATE2Normalised are inverses")
    {
        // NOTE: both are currently identity functions - each returns its
        // argument on either side of its `>= 0.0` test. This case asserts the
        // round-trip, which is the contract the names promise, so it keeps
        // passing if they are ever given a real implementation.
        const DATE dt = COleDateTime(2026, 9, 8, 6, 0, 0);
        TF_CHECK(Normalised2DATE(DATE2Normalised(dt)) == dt);
    }
}

// ---------------------------------------------------------------------------
static void CStringParser_Cases()
{
    TF_CASE("CStringParser::Argument splits on a delimiter, zero-based")
    {
        CStringParser oParser(L"alpha beta gamma");
        TF_CHECK(oParser.Argument(0, L" ") == CString(L"alpha"));
        TF_CHECK(oParser.Argument(1, L" ") == CString(L"beta"));
        TF_CHECK(oParser.Argument(2, L" ") == CString(L"gamma"));
    }

    TF_CASE("CStringParser::Argument past the end yields empty")
    {
        CStringParser oParser(L"alpha beta");
        TF_CHECK(oParser.Argument(9, L" ").IsEmpty());
    }

    TF_CASE("CStringParser::Argument honours an alternative delimiter")
    {
        CStringParser oParser(L"one,two,three");
        TF_CHECK(oParser.Argument(0, L",") == CString(L"one"));
        TF_CHECK(oParser.Argument(2, L",") == CString(L"three"));
    }
}

// ---------------------------------------------------------------------------
void RunMsgcoreMFCSuite()
{
    CString_Ext_Cases();
    CRect_Ext_Cases();
    COleTime_Ext_Cases();
    CStringParser_Cases();
}
