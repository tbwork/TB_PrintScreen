object Form5: TForm5
  Left = 597
  Top = 175
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = #36873#25321#20445#23384#36335#24452'-WaveSoft'
  ClientHeight = 258
  ClientWidth = 308
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 16
    Top = 230
    Width = 273
    Height = 25
    Caption = #30830#35748
    ModalResult = 1
    TabOrder = 0
    OnClick = Button1Click
  end
  object Dir: TJamShellList
    Left = 16
    Top = 32
    Width = 273
    Height = 193
    Filter = '*'
    ShellLink = JamShellLink1
    ShowHidden = True
    ShowFiles = False
    ViewStyle = vsReport
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    IconOptions.AutoArrange = True
    TabOrder = 1
  end
  object JamShellCombo1: TJamShellCombo
    Left = 16
    Top = 8
    Width = 273
    Height = 22
    ItemIndex = -1
    ShellLink = JamShellLink1
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ItemHeight = 16
    TabOrder = 2
    DesignSize = (
      273
      22)
  end
  object JamShellLink1: TJamShellLink
    Left = 168
    Top = 112
  end
end
