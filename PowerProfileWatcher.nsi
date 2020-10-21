;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General

  !define PRODUCT_VERSION "1.0.0.0"
  !define VERSION "1.0.0.0"
  VIProductVersion "${PRODUCT_VERSION}"
  VIFileVersion "${VERSION}"
  VIAddVersionKey "FileVersion" "${VERSION}"
  VIAddVersionKey "LegalCopyright" "(C) None!"
  VIAddVersionKey "FileDescription" "Power Profile Watcher"

  ;Name and file
  Name "Power Profile Watcher"
  OutFile "PowerProfWSetup.exe"
  Unicode True

  ;Default installation folder
  InstallDir "$PROGRAMFILES64\PowerProfileWatcher"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\PowerProfileWatcher" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel admin

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING
  !define MUI_ICON "PowerProfileWatcher\Resources\element_lightning.ico"
  !define MUI_FINISHPAGE_RUN "$INSTDIR\PowerProfileWatcher.exe"
  !define MUI_FINISHPAGE_RUN_TEXT "Run PowerProfileWatcher"

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "License.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "Application" App

  SetOutPath "$INSTDIR"
  
  File /r "x64\Release\*"

  ;create start-menu items
  CreateDirectory "$SMPROGRAMS\PowerProfileWatcher"
  CreateShortCut "$SMPROGRAMS\PowerProfileWatcher\Uninstall PowerProfileWatcher.lnk" "$INSTDIR\Uninstall.exe" "" "$INSTDIR\Uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\PowerProfileWatcher\PowerProfileWatcher.lnk" "$INSTDIR\PowerProfileWatcher.exe" "" "$INSTDIR\PowerProfileWatcher.exe" 0
  
  ;Store installation folder
  WriteRegStr HKCU "Software\PowerProfileWatcher" "" $INSTDIR

  ;Add to Apps list
  WriteRegStr HKLM64 "Software\Microsoft\Windows\CurrentVersion\Uninstall\PowerProfileWatcher" "DisplayName" "PowerProfileWatcher"
  WriteRegStr HKLM64 "Software\Microsoft\Windows\CurrentVersion\Uninstall\PowerProfileWatcher" "DisplayVersion" "${Version}"
  WriteRegStr HKLM64 "Software\Microsoft\Windows\CurrentVersion\Uninstall\PowerProfileWatcher" "DisplayIcon" "$\"$INSTDIR\PowerProfileWatcher.exe$\""
  WriteRegStr HKLM64 "Software\Microsoft\Windows\CurrentVersion\Uninstall\PowerProfileWatcher" "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

Function .onInit
  SectionSetFlags ${App} 17
FunctionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_App ${LANG_ENGLISH} "Application files"

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${App} $(DESC_App)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  Delete "$INSTDIR\Uninstall.exe"

  RMDir /r "$INSTDIR"
  Delete "$SMPROGRAMS\PowerProfileWatcher\PowerProfileWatcher.lnk"
  Delete "$SMPROGRAMS\PowerProfileWatcher\Uninstall PowerProfileWatcher.lnk"

  DeleteRegKey /ifempty HKCU "Software\PowerProfileWatcher"
  DeleteRegKey HKLM64 "Software\Microsoft\Windows\CurrentVersion\Uninstall\PowerProfileWatcher"

SectionEnd