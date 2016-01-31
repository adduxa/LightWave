unit LightWaveMain;

interface

uses
  Windows, Types, AIMPSDKCore, AIMPSDKVisual, IdUDPClient, SysUtils, Registry, Unit1, Controls, Dialogs;

type
  { TAIMPVisualPlugin }

  TAIMPVisualPlugin = class(TInterfacedObject, IAIMPVisualPlugin3)
  public
    // IAIMPVisualPlugin3
    function GetPluginAuthor: PWideChar; stdcall;
    function GetPluginInfo: PWideChar; stdcall;
    function GetPluginName: PWideChar; stdcall;
    function GetPluginFlags: DWORD; stdcall; // See AIMP_VISUAL_FLAGS_XXX
    function Initialize(ACoreUnit: IAIMPCoreUnit): HRESULT; stdcall;
    function Finalize: HRESULT; stdcall;
    procedure DisplayClick(X, Y: Integer); stdcall;
    procedure DisplayRender(DC: HDC; AData: PAIMPVisualData); stdcall;
    procedure DisplayResize(AWidth, AHeight: Integer); stdcall;
    //
  end;

var
  com: integer;
  Port:string;
  Spectr : array[1..10] of integer;
  AvgLevel:integer;
  r:TRegistry;
  bitR:boolean;
  DCB : TDCB;
  
implementation

{ TAIMPVisualPlugin }

function TAIMPVisualPlugin.Initialize(ACoreUnit: IAIMPCoreUnit): HRESULT;
begin
  //FWaveLinePen := CreatePen(PS_SOLID, 1, $FFFFFF); // White Pen

  r:=TRegistry.Create;
  r.RootKey:=HKEY_CURRENT_USER;
  r.OpenKey('Software\LightWave\LightWave_serial_plugin',true);
  Port:='COM3';
  if r.ValueExists('Port') then Port:=r.ReadString('Port');

  com:= CreateFile(PAnsiChar(Port), GENERIC_READ or GENERIC_WRITE, 0, nil, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  if com=INVALID_HANDLE_VALUE then begin
    ShowMessage('Unable to open COM port');
  end;
  GetCommState(com,DCB);
  DCB.BaudRate:=CBR_9600;
  DCB.Parity:=NOPARITY;
  DCB.ByteSize:=8;
  DCB.StopBits:=OneStopBit;
  DCB.EvtChar:=chr(13);
  SetCommState(com,DCB);

  Result := S_OK;
end;

function TAIMPVisualPlugin.Finalize: HRESULT;
begin
  TerminateThread(com, 0);
  CloseHandle(com);
  r.CloseKey;
  r.Free;

  Result := S_OK;
end;

procedure TAIMPVisualPlugin.DisplayClick(X, Y: Integer);
begin
  form1:=TForm1.Create(nil);
  form1.Edit1.Text:=Port;
  if form1.ShowModal=mrOK then
  begin
    Port:=form1.Edit1.Text;
    r.WriteString('Port',Port);
    TerminateThread(com, 0);
    CloseHandle(com);
    com:= CreateFile(PAnsiChar(Port), GENERIC_READ or GENERIC_WRITE, 0, nil, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if com=INVALID_HANDLE_VALUE then begin
      ShowMessage('Unable to open COM port');
    end;
    GetCommState(com,DCB);
    DCB.BaudRate:=CBR_9600;
    DCB.Parity:=NOPARITY;
    DCB.ByteSize:=8;
    DCB.StopBits:=OneStopBit;
    DCB.EvtChar:=chr(13);
    SetCommState(com,DCB);
  end;
  form1.Free;
end;

procedure TAIMPVisualPlugin.DisplayRender(DC: HDC; AData: PAIMPVisualData);
var
  i : Integer;
  bytes: array [0..2] of Char;
  bytesWrited: cardinal;
begin
  bytes[0]:= Char('W');
  bytes[1]:= Char(round((AData^.Spectrum[0,0]+AData^.Spectrum[1,0])/2)+1);
  bytes[2]:= Char(round(AData^.LevelL + AData^.LevelR) div 2 + 1);
//  for i:=0 to 255 do
//  begin
//    bytes[i+1]:= Char(round((AData^.Spectrum[0,i]+AData^.Spectrum[1,i])/2)+1);
//  end;
//  bytes[256]:= Char(round(AData^.LevelL + AData^.LevelR) div 2 + 1);
  bytesWrited:= 0;
  WriteFile(com, bytes, 3, bytesWrited, nil);
end;

procedure TAIMPVisualPlugin.DisplayResize(AWidth, AHeight: Integer);
begin
  //
end;

function TAIMPVisualPlugin.GetPluginAuthor: PWideChar;
begin
  Result := 'adduxa';
end;

function TAIMPVisualPlugin.GetPluginFlags: DWORD;
begin
  Result := AIMP_VISUAL_FLAGS_RQD_DATA_SPECTRUM + AIMP_VISUAL_FLAGS_NOT_SUSPEND;
end;

function TAIMPVisualPlugin.GetPluginInfo: PWideChar;
begin
  Result := 'LightWave serial plugin for AVR lumiere project';
end;

function TAIMPVisualPlugin.GetPluginName: PWideChar;
begin
  Result := 'LightWave serial plugin';
end;

end.
