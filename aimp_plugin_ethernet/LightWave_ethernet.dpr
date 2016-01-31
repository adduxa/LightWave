library LightWave_ethernet;

{$R *.RES}

uses
  AIMPSDKCore in 'SDK\AIMPSDKCore.pas',
  AIMPSDKVisual in 'SDK\AIMPSDKVisual.pas',
  LightWaveMain in 'LightWaveMain.pas',
  Unit1 in 'Unit1.pas' {Form1};

function AIMP_QueryVisual3(out AHeader: IAIMPVisualPlugin3): LongBool; stdcall;
begin
  AHeader := TAIMPVisualPlugin.Create;
  Result := True;
end;

exports
  AIMP_QueryVisual3;

begin
end.
