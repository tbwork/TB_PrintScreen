object Form9: TForm9
  Left = 282
  Top = 129
  Width = 699
  Height = 542
  BorderIcons = [biSystemMenu]
  Caption = #21478#23384#20026'...'
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 185
    Top = 30
    Width = 3
    Height = 466
    Cursor = crHSplit
  end
  object Label2: TLabel
    Left = 208
    Top = 447
    Width = 81
    Height = 13
    AutoSize = False
    Caption = #20445#23384#31867#22411'(&T):'
  end
  object Label1: TLabel
    Left = 208
    Top = 415
    Width = 73
    Height = 13
    AutoSize = False
    Caption = #25991#20214#21517'(&N):'
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 496
    Width = 691
    Height = 19
    Panels = <>
    SimplePanel = False
  end
  object ShellListView: TJamShellList
    Left = 188
    Top = 37
    Width = 488
    Height = 362
    Filter = '*.jpg;*.bmp;*.png;*.gif'
    ShellLink = JamShellLink
    NoFillOnStartup = True
    ViewStyle = vsReport
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = #23435#20307
    Font.Style = []
    IconOptions.AutoArrange = True
    TabOrder = 2
    OnClick = ShellListViewClick
    OnSelectItem = ShellListViewSelectItem
  end
  object Panel2: TPanel
    Left = 0
    Top = 30
    Width = 185
    Height = 466
    Align = alLeft
    BevelOuter = bvNone
    Caption = 'Panel2'
    TabOrder = 3
    object Splitter2: TSplitter
      Left = 0
      Top = 309
      Width = 185
      Height = 3
      Cursor = crVSplit
      Align = alBottom
    end
    object Label3: TLabel
      Left = 4
      Top = 298
      Width = 60
      Height = 13
      Caption = #22270#29255#39044#35272#65306
    end
    object ShellTree: TJamShellTree
      Left = 0
      Top = 8
      Width = 185
      Height = 289
      ShellLink = JamShellLink
      SpecialFolder = SF_DRIVES
      Filter = '*'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = #23435#20307
      Font.Style = []
      Indent = 19
      TabOrder = 0
    end
    object JamThumbnailImage1: TJamThumbnailImage
      Left = 0
      Top = 312
      Width = 185
      Height = 154
      ShellLink = JamShellLink
      Align = alBottom
      BevelInner = bvLowered
      BevelOuter = bvLowered
      Color = clWhite
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 691
    Height = 30
    Align = alTop
    BevelInner = bvRaised
    BevelOuter = bvLowered
    TabOrder = 4
    object SpeedButton1: TSpeedButton
      Left = 280
      Top = 4
      Width = 25
      Height = 23
      Hint = #19978#19968#32423
      Glyph.Data = {
        B6030000424DB603000000000000360000002800000012000000100000000100
        1800000000008003000000000000000000000000000000000000BFBFBFBFBFBF
        BFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBF
        BFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBF0000BFBFBFBFBFBFBFBFBFBFBFBFBFBF
        BFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBF
        BFBFBFBFBFBFBFBF0000BFBFBF00000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000BFBFBFBFBFBF
        0000BFBFBF000000FFFFFF00FFFFFFFFFF00FFFFFFFFFF00FFFFFFFFFF00FFFF
        FFFFFF00FFFFFFFFFF00FFFFFFFFFF000000BFBFBFBFBFBF0000BFBFBF000000
        00FFFFFFFFFF00FFFFFFFFFF00FFFFFFFFFF00FFFFFFFFFF00FFFFFFFFFF00FF
        FFFFFFFF00FFFF000000BFBFBFBFBFBF0000BFBFBF000000FFFFFF00FFFFFFFF
        FF00FFFF000000000000000000000000000000000000FFFFFF00FFFFFFFFFF00
        0000BFBFBFBFBFBF0000BFBFBF00000000FFFFFFFFFF00FFFFFFFFFF000000FF
        FFFF00FFFFFFFFFF00FFFFFFFFFF00FFFFFFFFFF00FFFF000000BFBFBFBFBFBF
        0000BFBFBF000000FFFFFF00FFFFFFFFFF00FFFF00000000FFFFFFFFFF00FFFF
        FFFFFF00FFFFFFFFFF00FFFFFFFFFF000000BFBFBFBFBFBF0000BFBFBF000000
        00FFFFFFFFFF000000000000000000000000000000FFFFFF00FFFFFFFFFF00FF
        FFFFFFFF00FFFF000000BFBFBFBFBFBF0000BFBFBF000000FFFFFF00FFFFFFFF
        FF000000000000000000FFFFFF00FFFFFFFFFF00FFFFFFFFFF00FFFFFFFFFF00
        0000BFBFBFBFBFBF0000BFBFBF00000000FFFFFFFFFF00FFFFFFFFFF000000FF
        FFFF00FFFFFFFFFF00FFFFFFFFFF00FFFFFFFFFF00FFFF000000BFBFBFBFBFBF
        0000BFBFBF000000FFFFFF00FFFFFFFFFF00FFFFFFFFFF00FFFFFFFFFF00FFFF
        FFFFFF00FFFFFFFFFF00FFFFFFFFFF000000BFBFBFBFBFBF0000BFBFBF000000
        0000000000000000000000000000000000000000000000000000000000000000
        00000000000000BFBFBFBFBFBFBFBFBF0000BFBFBFBFBFBF00000000FFFFFFFF
        FF00FFFFFFFFFF00FFFF000000BFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBF
        BFBFBFBFBFBFBFBF0000BFBFBFBFBFBFBFBFBF00000000000000000000000000
        0000BFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBF
        0000BFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBF
        BFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBF0000}
      ParentShowHint = False
      ShowHint = True
      OnClick = SpeedButton1Click
    end
    object SpeedButton3: TSpeedButton
      Left = 388
      Top = 4
      Width = 25
      Height = 23
      Hint = #21024#38500
      Glyph.Data = {
        4E010000424D4E01000000000000760000002800000013000000120000000100
        040000000000D800000000000000000000001000000010000000000000000000
        80000080000000808000800000008000800080800000C0C0C000808080000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00777777777777
        7777777000007777777777777777777000007777077777777770777000007770
        0077777777777770000077700007777777077770000077770007777770777770
        0000777770007777007777700000777777000770077777700000777777700000
        7777777000007777777700077777777000007777777000007777777000007777
        7700077007777770000077770000777700777770000077700007777770077770
        0000777000777777777077700000777777777777777777700000777777777777
        777777700000777777777777777777700000}
      ParentShowHint = False
      ShowHint = True
      OnClick = SpeedButton3Click
    end
    object SpeedButton4: TSpeedButton
      Left = 412
      Top = 4
      Width = 25
      Height = 23
      Hint = #22823#22270#26631
      Glyph.Data = {
        4E010000424D4E01000000000000760000002800000013000000120000000100
        040000000000D800000000000000000000001000000010000000000000000000
        80000080000000808000800000008000800080800000C0C0C000808080000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00777777777777
        7777777000007777777777777777777000007777777777000000777000007777
        77777777777777700000777777777774444777700000777777777774FF477770
        0000777777777774FF4777700000777777777774F44777700000770000007774
        4477777000007777777777777777777000007774444777777777777000007774
        FF4777777777777000007774FF4777777777777000007774F447777777777770
        0000777444777777777777700000777777777777777777700000777777777777
        777777700000777777777777777777700000}
      ParentShowHint = False
      ShowHint = True
      OnClick = SpeedButton4Click
    end
    object SpeedButton7: TSpeedButton
      Tag = 2
      Left = 436
      Top = 4
      Width = 25
      Height = 23
      Hint = #23567#22270#26631
      Glyph.Data = {
        4E010000424D4E01000000000000760000002800000013000000120000000100
        040000000000D800000000000000000000001000000010000000000000000000
        80000080000000808000800000008000800080800000C0C0C000808080000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00777777777777
        7777777000007777777777777777777000007777777777777777777000007744
        47777744477777700000774F4700074F47000770000077447777774477777770
        0000777777777777777777700000777777777777777777700000774447777744
        477777700000774F4700074F4700077000007744777777447777777000007777
        7777777777777770000077777777777777777770000077444777774447777770
        0000774F4700074F470007700000774477777744777777700000777777777777
        777777700000777777777777777777700000}
      ParentShowHint = False
      ShowHint = True
      OnClick = SpeedButton7Click
    end
    object SpeedButton8: TSpeedButton
      Tag = 3
      Left = 460
      Top = 4
      Width = 25
      Height = 23
      Hint = #35814#32454#20449#24687
      Glyph.Data = {
        4E010000424D4E01000000000000760000002800000013000000120000000100
        040000000000D800000000000000000000001000000010000000000000000000
        80000080000000808000800000008000800080800000C0C0C000808080000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00777777777777
        7777777000007777777777777777777000007777777777777777777000007477
        0000700070007770000077777777777777777770000074770000700070007770
        0000777777777777777777700000747700007000700077700000777777777777
        7777777000007477000070007000777000007777777777777777777000007477
        0000700070007770000077777777777777777770000074444444444444444770
        0000777777777777777777700000777700007000700077700000777777777777
        777777700000777777777777777777700000}
      ParentShowHint = False
      ShowHint = True
      OnClick = SpeedButton8Click
    end
    object SpeedButton9: TSpeedButton
      Left = 307
      Top = 4
      Width = 38
      Height = 23
      Hint = #19978#19968#27493
      Caption = 'Back'
      ParentShowHint = False
      ShowHint = True
      OnClick = SpeedButton9Click
    end
    object SpeedButton10: TSpeedButton
      Left = 344
      Top = 4
      Width = 40
      Height = 23
      Hint = #19979#19968#27493
      Caption = 'Redo'
      ParentShowHint = False
      ShowHint = True
      OnClick = SpeedButton10Click
    end
    object Label4: TLabel
      Left = 9
      Top = 9
      Width = 51
      Height = 13
      Caption = #20445#23384#22312'(&I) :'
    end
    object JamShellCombo1: TJamShellCombo
      Left = 75
      Top = 4
      Width = 184
      Height = 23
      ItemIndex = -1
      ShellLink = JamShellLink
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = #23435#20307
      Font.Style = []
      ItemHeight = 16
      TabOrder = 0
      DesignSize = (
        184
        23)
    end
  end
  object ComboBox2: TComboBox
    Left = 293
    Top = 444
    Width = 220
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    ItemIndex = 0
    TabOrder = 5
    Text = 'JPG  (*.jpg)'
    OnChange = ComboBox2Change
    Items.Strings = (
      'JPG  (*.jpg)'
      'BMP  (*.bmp)'
      'GIF  (*.gif)'
      'PNG  (*.png)')
  end
  object ComboBox1: TComboBox
    Left = 293
    Top = 411
    Width = 220
    Height = 21
    ItemHeight = 13
    TabOrder = 6
    Text = 'WaveSoft'#25130#23631
  end
  object Button2: TButton
    Left = 561
    Top = 465
    Width = 96
    Height = 25
    Cancel = True
    Caption = #21462#28040
    ModalResult = 2
    TabOrder = 7
    OnClick = Button2Click
  end
  object Button1: TButton
    Left = 561
    Top = 409
    Width = 96
    Height = 25
    Caption = #20445#23384#24182#36864#20986'  '
    Default = True
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button3: TButton
    Left = 561
    Top = 437
    Width = 96
    Height = 25
    Caption = #20445#23384#24182#32487#32493'  '
    TabOrder = 8
    OnClick = Button3Click
  end
  object JamShellLink: TJamShellLink
    Left = 189
    Top = 1
  end
end
