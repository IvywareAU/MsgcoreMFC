// Copyright © 2006, 2026 Ivyware Pty Ltd, Khrustal & Mann
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
//  Implementation for CTime extensions
//
#include "StdAfx.h"
#include "Sys/timeb.h"

#include "CTime_Ext.h"

//
//  Local pre-definitions
const short c_nQuarter[] = { 0, 1,1,1, 4,4,4, 7,7,7, 10,10,10 };

//
//  Trouble shooting
void
CTime_Print ( const CTime& oCTime, FILE *fd, LPCTSTR lpszFormat )
{
    if ( lpszFormat == nullptr )
      lpszFormat = _T("CTime=%Y-%m-%d %H:%M:%S\n");
    CString str;
    str = oCTime.Format ( lpszFormat );
   _ftprintf ( fd, str );
}

//
//  Days in month calculation
//
//  Parameters:  int nYear
//
//               int nMonth
//
//  Returns:     int
//               Number of days in month
int
DaysInMonth ( int nYear, int nMonth )
{
    // Introduce locals
    const short nDaysInMonth[12] = { 31, 28, 31, 30, 31, 30
                                   , 31, 31, 30, 31, 30, 31 };

    // To be sure to be sure
    if ( nMonth <  1 ||
         nMonth > 12    )
      return 0;

    // Anything but February
    if ( nMonth != 2 )
      return nDaysInMonth[nMonth-1];

    // Leap years
    if ( nYear%4 != 0 )
      return 28;
    else if ( nYear%100 == 0 && nYear%400 )
      return 28;
    return 29;
}

int
CTime_GetQuarter ( const CTime& oCTime )
{
    const short nQuarter[] = { 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4 };
    return nQuarter[oCTime.GetMonth()];
}

///////////////////////////////////////////////////////////////////////
//  YYYYMMDD formats

MsgcoreMFC_EXT CTime
YYYYMMDD_CTime ( int nYYYYMMDD )
{
    int nYear  =  nYYYYMMDD/10000;
    int nMonth = (nYYYYMMDD/100)%100;
    int nDay   =  nYYYYMMDD%100;
    return CTime ( nYear, nMonth, nDay, 0, 0, 0 );
}

MsgcoreMFC_EXT int
CTime_YYYYMMDD ( const CTime& oCTime )
{
    return oCTime.GetYear()*10000 + oCTime.GetMonth()*100 + oCTime.GetDay();
}

MsgcoreMFC_EXT COleDateTime
CTime2COleTime ( const CTime& oCTime )
{
    return COleDateTime(oCTime.GetYear(),oCTime.GetMonth(),oCTime.GetDay(),oCTime.GetHour(),oCTime.GetMinute(),oCTime.GetSecond() );
}
//MsgcoreMFC_EXT int
//YYYYMMDD_Year ( int nYYYYMMDD );

//
//  Transitioning
bool
CTime_IsYearTransistion ( const CTime& oCTimePrev, const CTime& oCTimeNext )
{
    return oCTimePrev.GetYear() != oCTimeNext.GetYear() ? true : false;
}
bool
CTime_IsQuarterTransistion ( const CTime& oCTimePrev, const CTime& oCTimeNext )
{
    if ( oCTimePrev.GetTime() <= 0 )
      return true;
    return CTime_SoQuarter(oCTimePrev) != CTime_SoQuarter(oCTimeNext) ? true : false;
}
bool
CTime_IsMonthTransistion ( const CTime& oCTimePrev, const CTime& oCTimeNext )
{
    if ( oCTimePrev.GetTime() <= 0 )
      return true;
    return oCTimePrev.GetMonth() != oCTimeNext.GetMonth() ? true : false;
}
bool
CTime_IsWeekTransistion ( const CTime& oCTimePrev, const CTime& oCTimeNext )
{
    if ( oCTimePrev.GetTime() <= 0 )
      return true;
    return CTime_SoWeek(oCTimePrev) != CTime_SoWeek(oCTimeNext) ? true : false;
}
bool
CTime_IsDayTransistion ( const CTime& oCTimePrev, const CTime& oCTimeNext )
{
    return oCTimePrev.GetDay() != oCTimeNext.GetDay() ? true : false;
}

//
//  Epochs etc
//  NOTES: Start and end of Year, Quarter, Month, Week and Day translations
CTime
CTime_SoYear ( const CTime& oCTime )
{
    return CTime(oCTime.GetYear(),1,1,0,0,0);
}
CTime
CTime_EoYear ( const CTime& oCTime )
{
    return CTime(oCTime.GetYear(),12,31,23,59,59);
}
CTime
CTime_SoQuarter ( const CTime& oCTime )
{
    return CTime(oCTime.GetYear(),c_nQuarter[oCTime.GetMonth()],1,0,0,0);
}
CTime
CTime_EoQuarter ( const CTime& oCTime )
{
    int nYear  = oCTime.GetYear();
    int nMonth = c_nQuarter[oCTime.GetMonth()] + 2;
    int nDay   = DaysInMonth ( nYear, nMonth );
    return CTime(nYear,nMonth,nDay,23,59,59);
}
CTime
CTime_SoMonth ( const CTime& oCTime )
{
    return CTime(oCTime.GetYear(),oCTime.GetMonth(),1,0,0,0);
}
CTime
CTime_EoMonth ( const CTime& oCTime )
{
    int nYear  = oCTime.GetYear();
    int nMonth = oCTime.GetMonth();
    int nDay   = DaysInMonth ( nYear, nMonth );
    return CTime(nYear,nMonth,nDay,23,59,59);
}
CTime
CTime_SoWeek ( const CTime& oCTime )
{
    int nDoWeek = oCTime.GetDayOfWeek();
        nDoWeek = (nDoWeek == 1) ? 7 : nDoWeek - 1;
    return CTime_AddDays ( oCTime, 1-nDoWeek );
}
CTime
CTime_EoWeek ( const CTime& oCTime )
{
    int    nDoWeek = oCTime.GetDayOfWeek();
           nDoWeek = (nDoWeek == 1) ? 7 : nDoWeek - 1;
    CTime dtWeek   = CTime_AddDays ( oCTime, 7 - nDoWeek );
    return CTime (dtWeek.GetYear(),dtWeek.GetMonth(),dtWeek.GetDay(),23,59,59);
}
CTime
CTime_SoDay ( const CTime& oCTime )
{
    return CTime(oCTime.GetYear(),oCTime.GetMonth(),oCTime.GetDay(),0,0,0);
}
CTime
CTime_EoDay ( const CTime& oCTime )
{
    return CTime(oCTime.GetYear(),oCTime.GetMonth(),oCTime.GetDay(),23,59,59);
}

///////////////////////////////////////////////////////////////////////
//  Arithmetic
MsgcoreMFC_EXT CTime
CTime_AddSeconds ( const CTime& oCTime, int nSeconds )
{
    CTime oCTemp = oCTime.GetTime() + nSeconds;
    return oCTemp;
}
MsgcoreMFC_EXT CTime
CTime_AddMinutes ( const CTime& oCTime, int nMinutes )
{
    CTime oCTemp = oCTime.GetTime() + nMinutes*60;
    return oCTemp;
}
MsgcoreMFC_EXT CTime
CTime_AddHours ( const CTime& oCTime, int nHours )
{
    CTime oCTemp = oCTime.GetTime() + nHours*60*60;
    return oCTemp;
}
MsgcoreMFC_EXT CTime
CTime_AddDays ( const CTime& oCTime, int nDays )
{
    int nYear  = oCTime.GetYear();
    int nMonth = oCTime.GetMonth();
    int nDay   = oCTime.GetDay() + nDays;
    while ( nDay > DaysInMonth(nYear,nMonth) )
    {
      nDay -= DaysInMonth(nYear,nMonth);
      nMonth++;
      if ( nMonth <= 12 )
        continue;
      nYear++;
      nMonth = 1;
    }
    while ( nDay <= 0 )
    {
      if ( --nMonth < 1 )
      {
        nMonth = 12;
        nYear--;
      }
      nDay += DaysInMonth(nYear,nMonth);
    }
    return CTime ( nYear, nMonth, nDay, oCTime.GetHour(), oCTime.GetMinute(), oCTime.GetSecond() );
}
MsgcoreMFC_EXT CTime
CTime_AddWeeks ( const CTime& oCTime, int nWeeks )
{
    return CTime_AddDays ( oCTime, nWeeks*7 );
}
MsgcoreMFC_EXT CTime
CTime_AddMonths ( const CTime& oCTime, int nMonths )
{
    nMonths += oCTime.GetMonth() + oCTime.GetYear() * 12;
    int nYear  = (nMonths - 1) / 12;
    int nMonth =  nMonths - nYear * 12;
    int nDay   =  oCTime.GetDay();
    if ( nDay > DaysInMonth(nYear,nMonth) )
      nDay = DaysInMonth ( nYear, nMonth );
    return CTime ( nYear, nMonth, nDay
                 , oCTime.GetHour(),oCTime.GetMinute(),oCTime.GetSecond() );
}
MsgcoreMFC_EXT CTime
CTime_AddQuarters ( const CTime& oCTime, int nQuarters )
{
    return CTime_AddMonths ( oCTime, nQuarters*3 );
}
MsgcoreMFC_EXT CTime
CTime_AddYears ( const CTime& oCTime, int nYears )
{
    return CTime ( oCTime.GetYear()+nYears,oCTime.GetMonth(),oCTime.GetDay()
                 , oCTime.GetHour(),oCTime.GetMinute(),oCTime.GetSecond() );
}

///////////////////////////////////////////////////////////////////////
//  Arithmetic

MsgcoreMFC_EXT __time64_t
CTime_SecondsDiff ( const CTime& oCTime1, const CTime& oCTime2 )
{
  __time64_t nDiff = oCTime1.GetTime() - oCTime2.GetTime();
    return nDiff;
}
MsgcoreMFC_EXT __time64_t
CTime_MinutesDiff ( const CTime& oCTime1, const CTime& oCTime2 )
{
  __time64_t nDiff = oCTime1.GetTime() - oCTime2.GetTime();
    return nDiff / 60;
}
MsgcoreMFC_EXT __time64_t
CTime_HoursDiff ( const CTime& oCTime1, const CTime& oCTime2 )
{
  __time64_t nDiff = oCTime1.GetTime() - oCTime2.GetTime();
    return nDiff / 3600;
}
MsgcoreMFC_EXT __time64_t
CTime_DaysDiff ( const CTime& oCTime1, const CTime& oCTime2 )
{
  __time64_t nDiff = oCTime1.GetTime() - oCTime2.GetTime();
    return nDiff / 86400;
}
MsgcoreMFC_EXT __time64_t
CTime_WeeksDiff ( const CTime& oCTime1, const CTime& oCTime2 )
{
  __time64_t nDiff = oCTime1.GetTime() - oCTime2.GetTime();
    return nDiff / (86400 * 7);
}
MsgcoreMFC_EXT __time64_t
CTime_MonthsDiff ( const CTime& oCTime1, const CTime& oCTime2 )
{
  __time64_t nDiff = oCTime1.GetYear()*12 + oCTime1.GetMonth()
                   - oCTime2.GetYear()*12 - oCTime2.GetMonth();
    return nDiff;
}
MsgcoreMFC_EXT __time64_t
CTime_QuartersDiff ( const CTime& oCTime1, const CTime& oCTime2 )
{
  __time64_t nDiff = oCTime1.GetYear()*12 + oCTime1.GetMonth()
                   - oCTime2.GetYear()*12 - oCTime2.GetMonth();
    return (nDiff + 2) / 3;
}
MsgcoreMFC_EXT __time64_t
CTime_YearsDiff ( const CTime& oCTime1, const CTime& oCTime2 )
{
  __time64_t nDiff = oCTime1.GetYear() - oCTime2.GetYear();
    return nDiff;
}
