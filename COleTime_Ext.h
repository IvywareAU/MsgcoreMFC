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
//  COleDateTime extension prototypes and definitions
//

#pragma   once
#include "MsgcoreMFC.h"
#include "ATLComTime.h"

//
//  Trouble shooting
MsgcoreMFC_EXT void
COleTime_Print ( const COleDateTime& oCOleTime, FILE *fd, LPCTSTR lpszFormat = nullptr );
MsgcoreMFC_EXT BOOL
COleTime_IsValid ( const DATE& dt ) noexcept;
MsgcoreMFC_EXT BOOL
COleTime_IsValid ( const COleDateTime& oCOleTime ) noexcept;

//
//  Translations
MsgcoreMFC_EXT DATE
Normalised2DATE ( DATE oleDATE );
MsgcoreMFC_EXT DATE
DATE2Normalised ( DATE oDATE );
MsgcoreMFC_EXT DATE
CTime2DATEnorm ( __time64_t nSeconds );
MsgcoreMFC_EXT BOOL
DATE2FileTime ( DATE oleDATE, FILETIME& ft );

//
//  Generic time parsing
MsgcoreMFC_EXT COleDateTime
YYYYMMDD_COleTime ( int nYYYYMMDD );
MsgcoreMFC_EXT int
COleTime_YYYYMMDD ( const COleDateTime& oCOleTime );
MsgcoreMFC_EXT int
YYYYMMDD_Year ( int nYYYYMMDD );
MsgcoreMFC_EXT CTime
COleTime2CTime ( const COleDateTime& oCOleTime );
MsgcoreMFC_EXT int
COleTime_GetQuarter ( const COleDateTime& oCOleTime );

//
//  Transitioning
//  
MsgcoreMFC_EXT bool
COleTime_IsYearTransistion ( const COleDateTime& oCOleTimePrev, const COleDateTime& oCOleTimeNext );
MsgcoreMFC_EXT bool
COleTime_IsQuarterTransistion ( const COleDateTime& oCOleTimePrev, const COleDateTime& oCOleTimeNext );
MsgcoreMFC_EXT bool
COleTime_IsMonthTransistion ( const COleDateTime& oCOleTimePrev, const COleDateTime& oCOleTimeNext );
MsgcoreMFC_EXT bool
COleTime_IsWeekTransistion ( const COleDateTime& oCOleTimePrev, const COleDateTime& oCOleTimeNext );
MsgcoreMFC_EXT bool
COleTime_IsDayTransistion ( const COleDateTime& oCOleTimePrev, const COleDateTime& oCOleTimeNext );

//
//  Epochs etc
MsgcoreMFC_EXT COleDateTime
COleTime_SoYear ( const COleDateTime& oCOleTime );
MsgcoreMFC_EXT COleDateTime
COleTime_EoYear ( const COleDateTime& oCOleTime );
MsgcoreMFC_EXT COleDateTime
COleTime_SoQuarter ( const COleDateTime& oCOleTime );
MsgcoreMFC_EXT COleDateTime
COleTime_EoQuarter ( const COleDateTime& oCOleTime );
MsgcoreMFC_EXT COleDateTime
COleTime_SoMonth ( const COleDateTime& oCOleTime );
MsgcoreMFC_EXT COleDateTime
COleTime_EoMonth ( const COleDateTime& oCOleTime );
MsgcoreMFC_EXT COleDateTime
COleTime_SoWeek ( const COleDateTime& oCOleTime );
MsgcoreMFC_EXT COleDateTime
COleTime_EoWeek ( const COleDateTime& oCOleTime );
MsgcoreMFC_EXT COleDateTime
COleTime_SoDay ( const COleDateTime& oCOleTime );
MsgcoreMFC_EXT COleDateTime
COleTime_EoDay ( const COleDateTime& oCOleTime );

//
//  Arithmetic
MsgcoreMFC_EXT COleDateTime
COleTime_AddSeconds ( const COleDateTime oCOleTime, int nSeconds );
MsgcoreMFC_EXT COleDateTime
COleTime_AddMinutes ( const COleDateTime oCOleTime, int nMinutes );
MsgcoreMFC_EXT COleDateTime
COleTime_AddHours ( const COleDateTime oCOleTime, int nHours );
MsgcoreMFC_EXT COleDateTime
COleTime_AddDays ( const COleDateTime& oCOleTime, int nDays );
MsgcoreMFC_EXT COleDateTime
COleTime_AddWeeks ( const COleDateTime& oCOleTime, int nWeeks );
MsgcoreMFC_EXT COleDateTime
COleTime_AddMonths ( const COleDateTime& oCOleTime, int nMonths );
MsgcoreMFC_EXT COleDateTime
COleTime_AddQuarters ( const COleDateTime& oCOleTime, int nQuarters );
MsgcoreMFC_EXT COleDateTime
COleTime_AddYears ( const COleDateTime& oCOleTime, int nYears );
MsgcoreMFC_EXT COleDateTime
COleTime_AddPUnits ( const COleDateTime& oCOleTime, int ePUnits, int nUnits );
//
//  Differences
MsgcoreMFC_EXT __time64_t
COleTime_SecondsDiff ( const COleDateTime& oCOleTime1, const COleDateTime& oCOleTime2 );
MsgcoreMFC_EXT __time64_t
COleTime_MinutesDiff ( const COleDateTime& oCOleTime1, const COleDateTime& oCOleTime2 );
MsgcoreMFC_EXT __time64_t
COleTime_HoursDiff ( const COleDateTime& oCOleTime1, const COleDateTime& oCOleTime2 );
MsgcoreMFC_EXT __time64_t
COleTime_DaysDiff ( const COleDateTime& oCOleTime1, const COleDateTime& oCOleTime2 );
MsgcoreMFC_EXT __time64_t
COleTime_WeeksDiff ( const COleDateTime& oCOleTime1, const COleDateTime& oCOleTime2 );
MsgcoreMFC_EXT __time64_t
COleTime_MonthsDiff ( const COleDateTime& oCOleTime1, const COleDateTime& oCOleTime2 );
MsgcoreMFC_EXT __time64_t
COleTime_QuartersDiff ( const COleDateTime& oCOleTime1, const COleDateTime& oCOleTime2 );
MsgcoreMFC_EXT __time64_t
COleTime_YearsDiff ( const COleDateTime& oCOleTime1, const COleDateTime& oCOleTime2 );

