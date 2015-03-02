//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit10.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm10 *Form10;
//---------------------------------------------------------------------------
__fastcall TForm10::TForm10(TComponent* Owner)
        : TForm(Owner)
{

}
//---------------------------------------------------------------------------

void __fastcall TForm10::Button2Click(TObject *Sender)
{
  YesOrNo=false;  
 Close();        
}
//---------------------------------------------------------------------------

void __fastcall TForm10::Button1Click(TObject *Sender)
{
 YesOrNo=true;
Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm10::FormShow(TObject *Sender)
{
 YesOrNo=false;
}
//---------------------------------------------------------------------------
 void __fastcall TForm10::CreateParams(TCreateParams &Param)
{
        TForm::CreateParams(Param);
    //    Param.WindowClass.style=CS_OWNDC;
        Param.WndParent=NULL;

       // Param.Style|=WS_POPUP;
        Param.ExStyle=WS_EX_TOPMOST; //Param.ExStyle|  WS_EX_TOPMOST|

}