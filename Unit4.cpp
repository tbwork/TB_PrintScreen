//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Main.h"
#include "Unit1.h"
#include "Unit4.h"

#include "Unit5.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm4 *Form4;
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
        : TForm(Owner)
{
DirPath="C:\\Documents and Settings\\Administrator\\My Documents\\My Pictures\\";
}
//---------------------------------------------------------------------------

void __fastcall TForm4::Button3Click(TObject *Sender)
{
 Form5->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TForm4::FormShow(TObject *Sender)
{

   Edit1->Text=DirPath;

}
//---------------------------------------------------------------------------
void __fastcall TForm4::Button1Click(TObject *Sender)
{
  try
  {
   AnsiString filename=Edit2->Text;
   TPNGObject* png=new TPNGObject();
   TGIFImage * gif=new TGIFImage();
   TJPEGImage*jpeg=new TJPEGImage();
  for(int i=0;i<liuxuan->CutList->Count;i++)
  {
   if(SaveStyle->ItemIndex==0)
   {     //bmp
        ((Graphics::TBitmap*)liuxuan->CutList->Items[i])->SaveToFile(DirPath+filename+"_"+AnsiString(i)+".bmp");
   }
   else if(SaveStyle->ItemIndex==1)
   {     //png
        png->Assign((Graphics::TBitmap*)liuxuan->CutList->Items[i]);
        png->SaveToFile(DirPath+filename+"_"+AnsiString(i)+".png");
   }
   else if(SaveStyle->ItemIndex==2)
   {    //jpg
        jpeg->CompressionQuality=Form1->CompressionValue;
        jpeg->Assign((Graphics::TBitmap*)liuxuan->CutList->Items[i]);
           jpeg->SaveToFile(DirPath+filename+"_"+AnsiString(i)+".jpg");
   }
   else if(SaveStyle->ItemIndex==3)
   {    //gif
      gif->ColorReduction=rmQuantizeWindows;    //量化Gif图，或者用rmQuantizeWindows 也行
           gif->Assign((Graphics::TBitmap*)liuxuan->CutList->Items[i]);
           gif->SaveToFile(DirPath+filename+"_"+AnsiString(i)+".gif");
   }
  }
  delete jpeg;
  delete gif;
  delete png;
  if(MessageBox(Application->Handle,"保存完成,点击确定打开文件夹","Wave截屏-提示",MB_OKCANCEL+MB_ICONINFORMATION)==1)
  {
    ShellExecute(Application->Handle,"open",DirPath.c_str(),NULL,NULL,SW_SHOW);
  }

 }
 catch(...)
 {
  MessageBox(Application->Handle,"保存失败","Wave截屏-提示",MB_OK+MB_ICONINFORMATION);
 }
}
//---------------------------------------------------------------------------
void __fastcall TForm4::Button2Click(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------
