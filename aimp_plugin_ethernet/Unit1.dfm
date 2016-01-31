object Form1: TForm1
  Left = 136
  Top = 134
  Width = 327
  Height = 153
  BorderIcons = [biSystemMenu]
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 16
    Width = 36
    Height = 13
    Caption = 'Arduino'
  end
  object Label2: TLabel
    Left = 8
    Top = 40
    Width = 25
    Height = 13
    Caption = #1055#1086#1088#1090
  end
  object Edit1: TEdit
    Left = 48
    Top = 15
    Width = 217
    Height = 24
    TabOrder = 0
  end
  object SpinEdit1: TSpinEdit
    Left = 48
    Top = 38
    Width = 81
    Height = 26
    MaxValue = 0
    MinValue = 0
    TabOrder = 1
    Value = 0
  end
  object BitBtn1: TBitBtn
    Left = 81
    Top = 73
    Width = 76
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 3
    NumGlyphs = 2
  end
  object BitBtn2: TBitBtn
    Left = 166
    Top = 72
    Width = 75
    Height = 26
    Cancel = True
    Caption = #1054#1090#1084#1077#1085#1072
    ModalResult = 2
    TabOrder = 2
    NumGlyphs = 2
  end
end
