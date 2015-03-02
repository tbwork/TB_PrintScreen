//---------------------------------------------------------------------------

#ifndef Unit9H
#define Unit9H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "JAMControls.hpp"
#include "ShellControls.hpp"
#include "ShellLink.hpp"
#include "TntComCtrls.hpp"
#include "TntStdCtrls.hpp"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <pngimage.hpp>
#include <jpeg.hpp>
#include <GIFImage.hpp>
//---------------------------------------------------------------------------
class TForm9 : public TForm
{
__published:	// IDE-managed Components
        TStatusBar *StatusBar1;
        TSplitter *Splitter1;
        TJamShellList *ShellListView;
        TPanel *Panel2;
        TSplitter *Splitter2;
        TLabel *Label3;
        TJamShellTree *ShellTree;
        TJamThumbnailImage *JamThumbnailImage1;
        TPanel *Panel1;
        TSpeedButton *SpeedButton1;
        TSpeedButton *SpeedButton3;
        TSpeedButton *SpeedButton4;
        TSpeedButton *SpeedButton7;
        TSpeedButton *SpeedButton8;
        TSpeedButton *SpeedButton9;
        TSpeedButton *SpeedButton10;
        TLabel *Label4;
        TJamShellCombo *JamShellCombo1;
        TLabel *Label2;
        TLabel *Label1;
        TJamShellLink *JamShellLink;
        TComboBox *ComboBox2;
        TComboBox *ComboBox1;
        TButton *Button2;
        TButton *Button1;
        TButton *Button3;
        void __fastcall SpeedButton1Click(TObject *Sender);
        void __fastcall SpeedButton9Click(TObject *Sender);
        void __fastcall SpeedButton10Click(TObject *Sender);
        void __fastcall SpeedButton3Click(TObject *Sender);
        void __fastcall SpeedButton4Click(TObject *Sender);
        void __fastcall SpeedButton7Click(TObject *Sender);
        void __fastcall SpeedButton8Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall ShellListViewClick(TObject *Sender);
        void __fastcall ComboBox2Change(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall ShellListViewSelectItem(TObject *Sender,
          TListItem *Item, bool Selected);
        void __fastcall Button3Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
  AnsiString filename;
  Graphics::TBitmap*temp;
  bool saved;
  bool isgoingon;
  void __fastcall CreateParams(TCreateParams &Param) ;
        __fastcall TForm9(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm9 *Form9;
//---------------------------------------------------------------------------
#endif
