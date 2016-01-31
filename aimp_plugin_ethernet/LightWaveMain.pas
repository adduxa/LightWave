unit LightWaveMain;

interface

uses
  Windows, Types, AIMPSDKCore, AIMPSDKVisual, IdUDPClient, SysUtils, Registry, Unit1, Controls;

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
  udp: TIdUDPClient;
  Mode : byte;
  Host : string;
  Port:integer;
  r:TRegistry;

implementation

{ TAIMPVisualPlugin }

function TAIMPVisualPlugin.Initialize(ACoreUnit: IAIMPCoreUnit): HRESULT;
begin
  //FWaveLinePen := CreatePen(PS_SOLID, 1, $FFFFFF); // White Pen

  r:=TRegistry.Create;
  r.RootKey:=HKEY_CURRENT_USER;
  r.OpenKey('Software\LightWave\LightWave_ethernet_plugin',true);
  Host:='192.168.0.108';
  Port:=8888;
  Mode:=0;
  if r.ValueExists('Host') then Host:=r.ReadString('Host');
  if r.ValueExists('Port') then Port:=r.ReadInteger('Port');

  udp:=TIdUDPClient.Create(nil);
  udp.Host:=Host;
  udp.Port:=Port;

  Result := S_OK;
end;

function TAIMPVisualPlugin.Finalize: HRESULT;
begin
  udp.Free;
  r.CloseKey;
  r.Free;

  Result := S_OK;
end;

procedure TAIMPVisualPlugin.DisplayClick(X, Y: Integer);
begin
  form1:=TForm1.Create(nil);
  form1.Edit1.Text:=Host;
  form1.SpinEdit1.Value:=Port;
  if form1.ShowModal=mrOK then
  begin
    Host:=form1.Edit1.Text;
    Port:=form1.SpinEdit1.Value;
    r.WriteString('Host',Host);
    r.WriteInteger('Port',Port);
    udp.Host:=Host;
    udp.Port:=Port;
  end;
  form1.Free;
end;

procedure TAIMPVisualPlugin.DisplayRender(DC: HDC; AData: PAIMPVisualData);
var
  i : Integer;
  bytes: array [0..257] of Char;
begin
  bytes[0]:= Char('w');
  for i:=0 to 255 do
  begin
    bytes[i+1]:= Char(round((AData^.Spectrum[0,i]+AData^.Spectrum[1,i])/2)+1);
  end;
  bytes[257]:= Char(round(AData^.LevelL + AData^.LevelR) div 2 + 1);
  udp.Send(bytes);
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
  Result := 'LightWave ethernet plugin for AVR lumiere project';
end;

function TAIMPVisualPlugin.GetPluginName: PWideChar;
begin
  Result := 'LightWave ethernet plugin';
end;

end.
