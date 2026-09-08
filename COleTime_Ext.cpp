// Copyright © 2014, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//
//  Implementation for COleDateTime extensions
//
#include "StdAfx.h"
#include "Sys/timeb.h"

#include "CTime_Ext.h"
#include "COleTime_Ext.h"

//
//  Local pre-definitions
const short c_nQuarter[] = { 0, 1,1,1, 4,4,4, 7,7,7, 10,10,10 };

//
//  Trouble shooting
void
COleTime_Print ( const COleDateTime& oCOleTime, FILE *fd, LPCTSTR lpszFormat )
{
    if ( lpszFormat == nullptr )
      lpszFormat = L"COleDateTime=%Y-%m-%d %H:%M:%S\n";
    CString str;
    str = oCOleTime.Format ( lpszFormat );
   _ftprintf ( fd, str );
}

MsgcoreMFC_EXT BOOL
COleTime_IsValid ( const DATE& dt ) noexcept
{
    COleDateTime oCOleDT = dt;
    return COleTime_IsValid ( oCOleDT );
}

MsgcoreMFC_EXT BOOL
COleTime_IsValid ( const COleDateTime& oCOleTime ) noexcept
{
    return oCOleTime.GetStatus() == COleDateTime::DateTimeStatus::valid ? TRUE : FALSE;
}

///////////////////////////////////////////////////////////////////////
//  Translations
MsgcoreMFC_EXT DATE
Normalised2DATE ( DATE oleDATE )
{
    if ( oleDATE >= 0.0 )
      return oleDATE;
    return oleDATE;
}
MsgcoreMFC_EXT DATE
DATE2Normalised ( DATE oDATE )
{
    if ( oDATE >= 0.0 )
      return oDATE;
    return oDATE;
}
MsgcoreMFC_EXT DATE
CTime2DATEnorm ( __time64_t nSeconds )
{
    COleDateTime dt ( nSeconds );
    return DATE2Normalised ( dt );
}

MsgcoreMFC_EXT BOOL
DATE2FileTime ( DATE oleDATE, FILETIME& ft)
{
    COleDateTime dt ( oleDATE );
    SYSTEMTIME   st;
                 dt.GetAsSystemTime ( st );
    return SystemTimeToFileTime ( &st, &ft );
}

///////////////////////////////////////////////////////////////////////
//  YYYYMMDD formats

MsgcoreMFC_EXT COleDateTime
YYYYMMDD_COleTime ( int nYYYYMMDD )
{
    int nYear  =  nYYYYMMDD/10000;
    int nMonth = (nYYYYMMDD/100)%100;
    int nDay   =  nYYYYMMDD%100;
    return COleDateTime ( nYear, nMonth, nDay, 0, 0, 0 );
}

MsgcoreMFC_EXT int
COleTime_YYYYMMDD ( const COleDateTime& oCOleTime )
{
    return oCOleTime.GetYear()*10000 + oCOleTime.GetMonth()*100 + oCOleTime.GetDay();
}

MsgcoreMFC_EXT CTime
COleTime2CTime ( const COleDateTime& oCOleTime )
{
    return CTime(oCOleTime.GetYear(),oCOleTime.GetMonth(),oCOleTime.GetDay(),oCOleTime.GetHour(),oCOleTime.GetMinute(),oCOleTime.GetSecond() );
}

int
COleTime_GetQuarter ( const COleDateTime& oCOleTime )
{
    const short nQuarter[] = { 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4 };
    return nQuarter[oCOleTime.GetMonth()];
}

//MsgcoreMFC_EXT int
//YYYYMMDD_Year ( int nYYYYMMDD );

//
//  Transitioning
bool
COleTime_IsYearTransistion ( const COleDateTime& oCOleTimePrev, const COleDateTime& oCOleTimeNext )
{
    return oCOleTimePrev.GetYear() != oCOleTimeNext.GetYear() ? true : false;
}
bool
COleTime_IsQuarterTransistion ( const COleDateTime& oCOleTimePrev, const COleDateTime& oCOleTimeNext )
{
    if ( oCOleTimePrev.m_dt <= 0 )
      return true;
    return COleTime_SoQuarter(oCOleTimePrev) != COleTime_SoQuarter(oCOleTimeNext) ? true : false;
}
bool
COleTime_IsMonthTransistion ( const COleDateTime& oCOleTimePrev, const COleDateTime& oCOleTimeNext )
{
    if ( oCOleTimePrev.m_dt <= 0 )
      return true;
    return oCOleTimePrev.GetMonth() != oCOleTimeNext.GetMonth() ? true : false;
}
bool
COleTime_IsWeekTransistion ( const COleDateTime& oCOleTimePrev, const COleDateTime& oCOleTimeNext )
{
    if ( oCOleTimePrev.m_dt <= 0 )
      return true;
    return COleTime_SoWeek(oCOleTimePrev) != COleTime_SoWeek(oCOleTimeNext) ? true : false;
}
bool
COleTime_IsDayTransistion ( const COleDateTime& oCOleTimePrev, const COleDateTime& oCOleTimeNext )
{
    return oCOleTimePrev.GetDay() != oCOleTimeNext.GetDay() ? true : false;
}

//
//  Epochs etc
//  NOTES: Start and end of Year, Quarter, Month, Week and Day translations
COleDateTime
COleTime_SoYear ( const COleDateTime& oCOleTime )
{
    return COleDateTime(oCOleTime.GetYear(),1,1,0,0,0);
}
COleDateTime
COleTime_EoYear ( const COleDateTime& oCOleTime )
{
    return COleDateTime(oCOleTime.GetYear(),12,31,23,59,59);
}
COleDateTime
COleTime_SoQuarter ( const COleDateTime& oCOleTime )
{
    return COleDateTime(oCOleTime.GetYear(),c_nQuarter[oCOleTime.GetMonth()],1,0,0,0);
}
COleDateTime
COleTime_EoQuarter ( const COleDateTime& oCOleTime )
{
    const int nYear  = oCOleTime.GetYear();
    const int nMonth = c_nQuarter[oCOleTime.GetMonth()] + 2;
    const int nDay   = DaysInMonth ( nYear, nMonth );
    return COleDateTime(nYear,nMonth,nDay,23,59,59);
}
COleDateTime
COleTime_SoMonth ( const COleDateTime& oCOleTime )
{
    return COleDateTime(oCOleTime.GetYear(),oCOleTime.GetMonth(),1,0,0,0);
}
COleDateTime
COleTime_EoMonth ( const COleDateTime& oCOleTime )
{
    const int nYear  = oCOleTime.GetYear();
    const int nMonth = oCOleTime.GetMonth();
    const int nDay   = DaysInMonth ( nYear, nMonth );
    return COleDateTime(nYear,nMonth,nDay,23,59,59);
}
COleDateTime
COleTime_SoWeek ( const COleDateTime& oCOleTime )
{
    int nDoWeek = oCOleTime.GetDayOfWeek();
        nDoWeek = (nDoWeek == 1) ? 7 : nDoWeek - 1;
    COleDateTime dtSoWeek = COleTime_AddDays ( oCOleTime, 1-nDoWeek );
#ifdef _DEBUG
    ASSERT(dtSoWeek.GetDayOfWeek()==2);
#endif
    return dtSoWeek;
}
COleDateTime
COleTime_EoWeek ( const COleDateTime& oCOleTime )
{
    int    nDoWeek = oCOleTime.GetDayOfWeek();
           nDoWeek = (nDoWeek == 1) ? 7 : nDoWeek - 1;
    COleDateTime dtWeek   = COleTime_AddDays ( oCOleTime, 7 - nDoWeek );
    return COleDateTime (dtWeek.GetYear(),dtWeek.GetMonth(),dtWeek.GetDay(),23,59,59);
}
COleDateTime
COleTime_SoDay ( const COleDateTime& oCOleTime )
{
    return COleDateTime(oCOleTime.GetYear(),oCOleTime.GetMonth(),oCOleTime.GetDay(),0,0,0);
}
COleDateTime
COleTime_EoDay ( const COleDateTime& oCOleTime )
{
    return COleDateTime(oCOleTime.GetYear(),oCOleTime.GetMonth(),oCOleTime.GetDay(),23,59,59);
}

///////////////////////////////////////////////////////////////////////
//  Arithmetic
//MsgcoreMFC_EXT COleDateTime
//COleTime_AddSeconds ( const COleDateTime& oCOleTime, int nSeconds )
//{
//    COleDateTime oCTemp = oCOleTime.GetTime() + nSeconds;
//    return oCTemp;
//}
//MsgcoreMFC_EXT COleDateTime
//COleTime_AddMinutes ( const COleDateTime& oCOleTime, int nMinutes )
//{
//    COleDateTime oCTemp = oCOleTime.GetTime() + nMinutes*60;
//    return oCTemp;
//}
//MsgcoreMFC_EXT COleDateTime
//COleTime_AddHours ( const COleDateTime& oCOleTime, int nHours )
//{
//    COleDateTime oCTemp = oCOleTime.GetTime() + nHours*60*60;
//    return oCTemp;
//}
MsgcoreMFC_EXT COleDateTime
COleTime_AddDays ( const COleDateTime& oCOleTime, int nDays )
{
    COleDateTime dtResult  = oCOleTime;
                 dtResult += COleDateTimeSpan ( nDays, 0, 0, 0 );
    return dtResult;
}
MsgcoreMFC_EXT COleDateTime
COleTime_AddWeeks ( const COleDateTime& oCOleTime, int nWeeks )
{
    COleDateTime dtResult  = oCOleTime;
                 dtResult += COleDateTimeSpan ( nWeeks*7, 0, 0, 0 );
    return dtResult;
}
MsgcoreMFC_EXT COleDateTime
COleTime_AddMonths ( const COleDateTime& oCOleTime, int nMonths )
{
    nMonths += oCOleTime.GetMonth() + oCOleTime.GetYear() * 12;
    const int nYear  = (nMonths - 1) / 12;
    const int nMonth =  nMonths - nYear * 12;
          int nDay   =  oCOleTime.GetDay();
    if ( nDay > DaysInMonth(nYear,nMonth) )
      nDay = DaysInMonth ( nYear, nMonth );
    return COleDateTime ( nYear, nMonth, nDay
                 , oCOleTime.GetHour(),oCOleTime.GetMinute(),oCOleTime.GetSecond() );
}
MsgcoreMFC_EXT COleDateTime
COleTime_AddQuarters ( const COleDateTime& oCOleTime, int nQuarters )
{
    return COleTime_AddMonths ( oCOleTime, nQuarters*3 );
}
MsgcoreMFC_EXT COleDateTime
COleTime_AddYears ( const COleDateTime& oCOleTime, int nYears )
{
    return COleDateTime ( oCOleTime.GetYear()+nYears,oCOleTime.GetMonth(),oCOleTime.GetDay()
                 , oCOleTime.GetHour(),oCOleTime.GetMinute(),oCOleTime.GetSecond() );
}
MsgcoreMFC_EXT COleDateTime
COleTime_AddPUnits ( const COleDateTime& oCOleTime, int ePUnits, int nPUnits )
{
    if ( ePUnits == 5 )
      return COleTime_AddDays ( oCOleTime, nPUnits );
    if ( ePUnits == 4 )
      return COleTime_AddWeeks ( oCOleTime, nPUnits );
    if ( ePUnits == 3 )
      return COleTime_AddMonths ( oCOleTime, nPUnits );
    if ( ePUnits == 2 )
      return COleTime_AddQuarters ( oCOleTime, nPUnits );
    if ( ePUnits == 1 )
      return COleTime_AddYears ( oCOleTime, nPUnits );
    ASSERT(0);
    return oCOleTime;
}

///////////////////////////////////////////////////////////////////////
//  Arithmetic

MsgcoreMFC_EXT __time64_t
COleTime_SecondsDiff ( const COleDateTime& oCOleTime1, const COleDateTime& oCOleTime2 )
{
    COleDateTimeSpan ts = oCOleTime1 - oCOleTime2;
    return (__time64_t)(ts.GetTotalSeconds()+0.5);
}
MsgcoreMFC_EXT __time64_t
COleTime_MinutesDiff ( const COleDateTime& oCOleTime1, const COleDateTime& oCOleTime2 )
{
    COleDateTimeSpan ts = oCOleTime1 - oCOleTime2;
    return (__time64_t)(ts.GetTotalMinutes()+0.5);
}
MsgcoreMFC_EXT __time64_t
COleTime_HoursDiff ( const COleDateTime& oCOleTime1, const COleDateTime& oCOleTime2 )
{
    COleDateTimeSpan ts = oCOleTime1 - oCOleTime2;
    return (__time64_t)(ts.GetTotalHours()+0.5);
}
MsgcoreMFC_EXT __time64_t
COleTime_DaysDiff ( const COleDateTime& oCOleTime1, const COleDateTime& oCOleTime2 )
{
    const COleDateTimeSpan ts = oCOleTime1 - oCOleTime2;
    return (__time64_t)(ts.GetTotalDays()+0.5);
}
MsgcoreMFC_EXT __time64_t
COleTime_WeeksDiff ( const COleDateTime& oCOleTime1, const COleDateTime& oCOleTime2 )
{
    COleDateTimeSpan ts = oCOleTime1 - oCOleTime2;
    return (__time64_t)(ts.GetTotalDays()/7.0+0.5);
}
MsgcoreMFC_EXT __time64_t
COleTime_MonthsDiff ( const COleDateTime& oCOleTime1, const COleDateTime& oCOleTime2 )
{
  __time64_t nDiff = oCOleTime1.GetYear()*12 + oCOleTime1.GetMonth()
                   - oCOleTime2.GetYear()*12 - oCOleTime2.GetMonth();
    return nDiff;
}
MsgcoreMFC_EXT __time64_t
COleTime_QuartersDiff ( const COleDateTime& oCOleTime1, const COleDateTime& oCOleTime2 )
{
  __time64_t nDiff = oCOleTime1.GetYear()*12 + oCOleTime1.GetMonth()
                   - oCOleTime2.GetYear()*12 - oCOleTime2.GetMonth();
    return (nDiff + 2) / 3;
}
MsgcoreMFC_EXT __time64_t
COleTime_YearsDiff ( const COleDateTime& oCOleTime1, const COleDateTime& oCOleTime2 )
{
  __time64_t nDiff = oCOleTime1.GetYear() - oCOleTime2.GetYear();
    return nDiff;
}
