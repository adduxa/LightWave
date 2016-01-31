unit AIMPSDKVisual;

{************************************************}
{*                                              *}
{*                AIMP Plugins API              *}
{*             v3.50.1238 (13.03.2013)          *}
{*                 Visual Plugins               *}
{*                                              *}
{*              (c) Artem Izmaylov              *}
{*                 www.aimp.ru                  *}
{*             Mail: artem@aimp.ru              *}
{*              ICQ: 345-908-513                *}
{*                                              *}
{************************************************}

interface

uses
  Windows, AIMPSDKCore;

const
  // flags for IAIMPVisualPlugin3.GetPluginFlags
  AIMP_VISUAL_FLAGS_RQD_DATA_WAVE       = 1;
  AIMP_VISUAL_FLAGS_RQD_DATA_SPECTRUM   = 2;
  AIMP_VISUAL_FLAGS_NOT_SUSPEND         = 4;

  AIMP_VISUAL_SPECTRUM_MAX = 256;
  AIMP_VISUAL_WAVEFORM_MAX = 512;

type
  TWaveForm = array[0..1, 0..AIMP_VISUAL_WAVEFORM_MAX - 1] of ShortInt;
  TSpectrum = array[0..1, 0..AIMP_VISUAL_SPECTRUM_MAX - 1] of Byte;

  PAIMPVisualData = ^TAIMPVisualData;
  TAIMPVisualData = packed record
    LevelR, LevelL: Integer;
    Spectrum: TSpectrum;
    WaveForm: TWaveForm;
  end;

  { IAIMPVisualPlugin3 }

  // Must be implemented by Plugin
  IAIMPVisualPlugin3 = interface(IUnknown)
    function GetPluginAuthor: PWideChar; stdcall;
    function GetPluginInfo: PWideChar; stdcall;
    function GetPluginName: PWideChar; stdcall;
    function GetPluginFlags: DWORD; stdcall; // See AIMP_VISUAL_FLAGS_XXX
    function Initialize(ACoreUnit: IAIMPCoreUnit): HRESULT; stdcall;
    function Finalize: HRESULT; stdcall;
    procedure DisplayClick(X, Y: Integer); stdcall;
    procedure DisplayRender(DC: HDC; AData: PAIMPVisualData); stdcall;
    procedure DisplayResize(AWidth, AHeight: Integer); stdcall;
  end;

  TAIMPVisualPluginProc = function (out AHeader: IAIMPVisualPlugin3): LongBool; stdcall;
  // Export function name: AIMP_QueryVisual3

implementation

end.
