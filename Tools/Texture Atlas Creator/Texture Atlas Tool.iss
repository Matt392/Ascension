; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{EC7B3174-60AD-4E0D-ADBB-BDA4E41B25A8}
AppName=Texture Atlas Tool
AppVersion=1.0
AppPublisher=Phoenix Code Labs
DefaultDirName={pf}\Phoenix Code Labs\Texture Atlas Tool
DisableDirPage=yes
DefaultGroupName=Phoenix Code Labs\Texture Atlas Tool
DisableProgramGroupPage=yes
OutputBaseFilename=Texture Atlas Tool Setup
Compression=lzma
SolidCompression=yes
UninstallDisplayName=Texture Atlas Tool Uninst
;WizardImageFile=icon_installer_small.bmp
;WizardSmallImageFile=icon_installer_small.bmp

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; 
;Flags: unchecked

[Files]
Source: "RuntimeFolder\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "ExtraInstalls\oalinst.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "ExtraInstalls\vcredist_x86.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "ExtraInstalls\DirectX\*"; DestDir: "{app}\DirectX"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\Texture Atlas Tool"; Filename: "{app}\Texture Atlas Creator.exe";   
Name: "{group}\{cm:UninstallProgram,Texture Atlas Tool}"; Filename: "{uninstallexe}"; 
Name: "{commondesktop}\Texture Atlas Creator"; Filename: "{app}\Texture Atlas Creator.exe"; 

[Run]
Filename: "{app}\Texture Atlas Creator.exe"; Description: "{cm:LaunchProgram,Texture Atlas Tool}"; Flags: nowait postinstall skipifsilent
Filename: "{app}\oalinst.exe"; Parameters: "/s"
Filename: "{app}\vcredist_x86.exe"; Parameters: "/q"
Filename: "{app}\DirectX\DXSETUP.exe"; Parameters: "/Silent"
;Filename: "{app}\Readme.txt"; Description: "View the README file"; Flags: postinstall shellexec skipifsilent

