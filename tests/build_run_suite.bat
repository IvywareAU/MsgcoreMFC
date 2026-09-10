@echo off
REM Copyright © 2026 Ivyware Pty Ltd, Khrustal & Mann
REM              MELBOURNE, VICTORIA, AUSTRALIA, 3000
REM
REM Licensed under the Apache License, Version 2.0 (the "License");
REM you may not use this file except in compliance with the License.
REM You may obtain a copy of the License at
REM
REM     http://www.apache.org/licenses/LICENSE-2.0
REM
REM Unless required by applicable law or agreed to in writing, software
REM distributed under the License is distributed on an "AS IS" BASIS,
REM WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
REM implied. See the License for the specific language governing
REM permissions and limitations under the License.
REM
REM  Build and run the MsgcoreMFC unit suites (x64, Debug).
REM
REM      build_run_suite.bat
REM
REM  Modelled on Msgcore's tests\build_run_suite.bat, and deliberately a
REM  near-copy of it rather than a shared helper: the toolchain probe and the
REM  import-library search are the parts that actually break, and having them in
REM  one place in the other repository did not stop that. What differs is the
REM  source list, the output name, and that TWO import libraries are needed here
REM  (MsgcoreMFC and the Msgcore it links against) rather than one.
REM
REM  NOTES: Everything this script produces goes to tests\out\, which
REM         .gitignore's depth-independent "out/" already covers.
REM       : The runner links the DLL build of both libraries and copies both
REM         DLLs beside the exe. There is no static mode: MsgcoreMFC has no
REM         static configuration to link.
REM       : /std:c++20 matches the library's own setting. Building the suite at
REM         a different standard than the DLL it exercises is exactly the split
REM         this repository just removed from its own configurations.

setlocal
set "HERE=%~dp0"
set "REPO=%~dp0.."
set "MSCS=%~dp0..\.."
set "OUTDIR=%~dp0out"

REM ---- Visual Studio build environment --------------------------------------
if defined VCINSTALLDIR goto :have_vc
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" goto :no_vs
for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSDIR=%%i"
if not defined VSDIR goto :no_vs
call "%VSDIR%\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
if errorlevel 1 goto :no_vs
:have_vc

REM ---- Locate the import libraries ------------------------------------------
REM  Both are written to the same per-platform, per-configuration directory by
REM  Directory.Build.props, so one search finds both.
set "LIBDIR="
if defined WDMSCS_LIB if exist "%WDMSCS_LIB%\x64\Debug\MsgcoreMFC.lib" set "LIBDIR=%WDMSCS_LIB%\x64\Debug"
if not defined LIBDIR if exist "%MSCS%\lib\x64\Debug\MsgcoreMFC.lib" set "LIBDIR=%MSCS%\lib\x64\Debug"
if not defined LIBDIR goto :nothing_built
if not exist "%LIBDIR%\Msgcore.lib" goto :no_msgcore

REM ---- Build ----------------------------------------------------------------
if not exist "%OUTDIR%" mkdir "%OUTDIR%"

echo === BUILD ===
cl /nologo /EHsc /MDd /std:c++20 /Zc:wchar_t ^
   /D_DEBUG /D_CONSOLE /D_UNICODE /DUNICODE /D_AFXDLL /D_WIN32_WINNT=0x0603 ^
   /I"%REPO%" /I"%MSCS%\Msgcore" /Fo"%OUTDIR%\\" /Fd"%OUTDIR%\suite.pdb" ^
   "%HERE%SuiteMain.cpp" "%HERE%TestFramework.cpp" ^
   "%HERE%MsgcoreMFCSuite.cpp" "%HERE%P2PmsgBridgeSuite.cpp" ^
   /link /LIBPATH:"%LIBDIR%" MsgcoreMFC.lib Msgcore.lib comsuppwd.lib ^
   /OUT:"%OUTDIR%\msgcoremfc_suite.exe"
if errorlevel 1 goto :fail_build

REM ---- Stage the DLLs beside the exe -----------------------------------------
set "MFCDLL=%REPO%\out\x64\Debug\MsgcoreMFC.dll"
set "COREDLL=%MSCS%\Msgcore\out\x64\Debug\Msgcore.dll"
if not exist "%MFCDLL%"  goto :no_dll
if not exist "%COREDLL%" goto :no_dll
copy /Y "%MFCDLL%"  "%OUTDIR%" >nul
copy /Y "%COREDLL%" "%OUTDIR%" >nul

REM ---- Run ------------------------------------------------------------------
echo.
echo === RUN ===
pushd "%OUTDIR%"
"%OUTDIR%\msgcoremfc_suite.exe"
set "RC=%errorlevel%"
popd
endlocal & exit /b %RC%

REM ---- Diagnostics ----------------------------------------------------------
:no_vs
echo.
echo ERROR: no Visual Studio C++ toolset found.
echo        Run this from a "x64 Native Tools Command Prompt for VS 2026", or
echo        install the "Desktop development with C++" workload.
endlocal & exit /b 1

:nothing_built
echo.
echo ERROR: MsgcoreMFC has not been built for x64 Debug.
echo        Looked for an import library at
if defined WDMSCS_LIB echo            %WDMSCS_LIB%\x64\Debug\MsgcoreMFC.lib
echo            %MSCS%\lib\x64\Debug\MsgcoreMFC.lib
echo        and found neither. Build Debug^|x64 first:
echo            msbuild "MsgcoreMFC(2026).sln" /p:Configuration=Debug /p:Platform=x64
endlocal & exit /b 1

:no_msgcore
echo.
echo ERROR: %LIBDIR%\Msgcore.lib is missing.
echo        MsgcoreMFC.lib is there but the Msgcore it links against is not.
echo        Building MsgcoreMFC produces both, so build Debug^|x64 again.
endlocal & exit /b 1

:no_dll
echo.
echo ERROR: a required DLL is missing.
echo            %MFCDLL%
echo            %COREDLL%
echo        The import libraries exist but a DLL does not; build Debug^|x64.
endlocal & exit /b 1

:fail_build
echo.
echo ERROR: the suite failed to compile or link.
endlocal & exit /b 1
