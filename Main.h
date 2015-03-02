//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "GIFImage.hpp"
#include <Graphics.hpp>
#include <Buttons.hpp>
#include <Menus.hpp>
#include <Clipbrd.hpp>
#include "pngimage.hpp"
#include "trayicon.h"
#include <ImgList.hpp>
#include <Registry.hpp>

//---------------------------------------------------------------------------
class Tliuxuan : public TForm
{
__published:	// IDE-managed Components
        TImage *Image1;
        TImage *Image2;
        TImage *Image3;
        TImage *Image4;
        TImage *Image5;
        TImage *Image6;
        TPopupMenu *PopupMenu1;
        TMenuItem *N1;
        TMenuItem *Save1;
        TMenuItem *N2;
        TMenuItem *N3;
        TMenuItem *N4;
        TTrayIcon *TrayIcon1;
        TImageList *ImageList1;
        TPopupMenu *PopupMenu2;
        TMenuItem *Show1;
        TMenuItem *N5;
        TMenuItem *Edit1;
        TImage *Image7;
        TImage *Image8;
        TImage *Image9;
        TImage *Image10;
        TMenuItem *N6;
        TMenuItem *N7;
        TMenuItem *N8;
        TMenuItem *N9;
        TMenuItem *N10;
        TMenuItem *N11;
        TMenuItem *N12;
        TMenuItem *N13;
        void __fastcall Image6Click(TObject *Sender);
        void __fastcall Image5Click(TObject *Sender);
        void __fastcall Image5MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Image5MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall Image6MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall Image6MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall Image4Click(TObject *Sender);
        void __fastcall Image2Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall N3Click(TObject *Sender);
        void __fastcall N4Click(TObject *Sender);
        void __fastcall Save1Click(TObject *Sender);
        void __fastcall Image3DblClick(TObject *Sender);
        void __fastcall N1Click(TObject *Sender);
        void __fastcall Image1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall TrayIcon1Click(TObject *Sender);
        void __fastcall Show1Click(TObject *Sender);
        void __fastcall Edit1Click(TObject *Sender);
        void __fastcall Image7MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Image7MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall Image8MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall Image8MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Image9MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Image9MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall Image10MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Image10MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Image9Click(TObject *Sender);
        void __fastcall Image10Click(TObject *Sender);
        void __fastcall Image8Click(TObject *Sender);
        void __fastcall Image7Click(TObject *Sender);
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall Image3Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall N7Click(TObject *Sender);
        void __fastcall N8Click(TObject *Sender);
        void __fastcall N10Click(TObject *Sender);
        void __fastcall N12Click(TObject *Sender);
        void __fastcall N11Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
 void CleanMin_Close();
 bool bmpok5;//判定Image5当前最小化和关闭按钮是否处于Noth 状态
 bool bmpok6;//判定Image5当前最小化和关闭按钮是否处于Noth 状态
 void __fastcall WndProc(TMessage &msg);
 TList * CutList;
// void SaveToFile_Main();
 int pImage;   //记录中间显示的图片
 void ShowImageInfo();
 BOOL SaveToClipboard() ;
 void SaveToFile_user();
 TList * DateList;
 void FormBigShow();
 void FormSmallShow();
 void InstallFullWatch();  //安装对全屏窗口的监控，利用自定义消息 MSG_APPBAR_MSGID
 TRegistry * intial_key;
 bool hidemain;
 Graphics::TBitmap* NoPicture;//方便Form1的调用
 int formleft,formtop;
 //int StrToInt(const char*p);
 int StrToInt_User(AnsiString);

 bool hasfullscreen;
 //重载Image控件的WndProc
 TWndMethod   OldImageWndProc5;
 TWndMethod   OldImageWndProc6;
 void __fastcall  NewImageWndProc5(TMessage&   Message);
 void __fastcall  NewImageWndProc6(TMessage&   Message);
        __fastcall Tliuxuan(TComponent* Owner);
        __fastcall ~Tliuxuan();
};
//---------------------------------------------------------------------------
extern PACKAGE Tliuxuan *liuxuan;
//---------------------------------------------------------------------------
#endif
