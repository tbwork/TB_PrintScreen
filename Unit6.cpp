//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit6.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTBNote *TBNote;
//---------------------------------------------------------------------------
__fastcall TTBNote::TTBNote(TComponent* Owner)
        : TForm(Owner)
{
 needshow=true;

}
//---------------------------------------------------------------------------
void __fastcall TTBNote::Button1Click(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------
void __fastcall TTBNote::CheckBox1Click(TObject *Sender)
{
  if(CheckBox1->Checked)
  needshow=false;
  else
  needshow=true;
}
//---------------------------------------------------------------------------
