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
//  CTime extension prototypes and definitions
//

#pragma   once
#include "MsgcoreMFC.h"

//
//  Trouble shooting
MsgcoreMFC_EXT void
CTime_Print ( const CTime& oCTime, FILE *fd, LPCTSTR lpszFormat = nullptr );

//
//  Generic time manipulations
//  NOTES: Static functions used to support CTime
MsgcoreMFC_EXT int
DaysInMonth ( int nYear, int nMonth );
MsgcoreMFC_EXT int
CTime_GetQuarter ( const CTime& oCTime );

//
//  Generic time parsing
MsgcoreMFC_EXT CTime
YYYYMMDD_CTime ( int nYYYYMMDD );
MsgcoreMFC_EXT int
CTime_YYYYMMDD ( const CTime& oCTime );
MsgcoreMFC_EXT int
YYYYMMDD_Year ( int nYYYYMMDD );
MsgcoreMFC_EXT COleDateTime
CTime2COleTime ( const CTime& dt );

//
//  Transitioning
//  
MsgcoreMFC_EXT bool
CTime_IsYearTransistion ( const CTime& oCTimePrev, const CTime& oCTimeNext );
MsgcoreMFC_EXT bool
CTime_IsQuarterTransistion ( const CTime& oCTimePrev, const CTime& oCTimeNext );
MsgcoreMFC_EXT bool
CTime_IsMonthTransistion ( const CTime& oCTimePrev, const CTime& oCTimeNext );
MsgcoreMFC_EXT bool
CTime_IsWeekTransistion ( const CTime& oCTimePrev, const CTime& oCTimeNext );
MsgcoreMFC_EXT bool
CTime_IsDayTransistion ( const CTime& oCTimePrev, const CTime& oCTimeNext );

//
//  Epochs etc
MsgcoreMFC_EXT CTime
CTime_SoYear ( const CTime& oCTime );
MsgcoreMFC_EXT CTime
CTime_EoYear ( const CTime& oCTime );
MsgcoreMFC_EXT CTime
CTime_SoQuarter ( const CTime& oCTime );
MsgcoreMFC_EXT CTime
CTime_EoQuarter ( const CTime& oCTime );
MsgcoreMFC_EXT CTime
CTime_SoMonth ( const CTime& oCTime );
MsgcoreMFC_EXT CTime
CTime_EoMonth ( const CTime& oCTime );
MsgcoreMFC_EXT CTime
CTime_SoWeek ( const CTime& oCTime );
MsgcoreMFC_EXT CTime
CTime_EoWeek ( const CTime& oCTime );
MsgcoreMFC_EXT CTime
CTime_SoDay ( const CTime& oCTime );
MsgcoreMFC_EXT CTime
CTime_EoDay ( const CTime& oCTime );

//
//  Arithmetic
MsgcoreMFC_EXT CTime
CTime_AddSeconds ( const CTime& oCTime, int nSeconds );
MsgcoreMFC_EXT CTime
CTime_AddMinutes ( const CTime& oCTime, int nMinutes );
MsgcoreMFC_EXT CTime
CTime_AddHours ( const CTime& oCTime, int nHours );
MsgcoreMFC_EXT CTime
CTime_AddDays ( const CTime& oCTime, int nDays );
MsgcoreMFC_EXT CTime
CTime_AddWeeks ( const CTime& oCTime, int nWeeks );
MsgcoreMFC_EXT CTime
CTime_AddMonths ( const CTime& oCTime, int nMonths );
MsgcoreMFC_EXT CTime
CTime_AddQuarters ( const CTime& oCTime, int nQuarters );
MsgcoreMFC_EXT CTime
CTime_AddYears ( const CTime& oCTime, int nYears );
//
//  Differences
MsgcoreMFC_EXT __time64_t
CTime_SecondsDiff ( const CTime& oCTime1, const CTime& oCTime2 );
MsgcoreMFC_EXT __time64_t
CTime_MinutesDiff ( const CTime& oCTime1, const CTime& oCTime2 );
MsgcoreMFC_EXT __time64_t
CTime_HoursDiff ( const CTime& oCTime1, const CTime& oCTime2 );
MsgcoreMFC_EXT __time64_t
CTime_DaysDiff ( const CTime& oCTime1, const CTime& oCTime2 );
MsgcoreMFC_EXT __time64_t
CTime_WeeksDiff ( const CTime& oCTime1, const CTime& oCTime2 );
MsgcoreMFC_EXT __time64_t
CTime_MonthsDiff ( const CTime& oCTime1, const CTime& oCTime2 );
MsgcoreMFC_EXT __time64_t
CTime_QuartersDiff ( const CTime& oCTime1, const CTime& oCTime2 );
MsgcoreMFC_EXT __time64_t
CTime_YearsDiff ( const CTime& oCTime1, const CTime& oCTime2 );

