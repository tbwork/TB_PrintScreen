//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit5.h"
#include "Unit4.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JAMControls"
#pragma link "ShellControls"
#pragma link "ShellLink"
#pragma link "TntComCtrls"
#pragma link "TntStdCtrls"
#pragma resource "*.dfm"
TForm5 *Form5;
//---------------------------------------------------------------------------
__fastcall TForm5::TForm5(TComponent* Owner)
        : TForm(Owner)
{
 JamShellCombo1->ItemIndex=0;
}
//---------------------------------------------------------------------------

void __fastcall TForm5::Button1Click(TObject *Sender)
{
 Form4->DirPath=Dir->Path;
 Form4->Edit1->Text=Dir->Path;
}
//---------------------------------------------------------------------------

