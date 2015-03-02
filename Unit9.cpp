//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit9.h"
#include "Unit1.h"
#include "Unit10.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JAMControls"
#pragma link "ShellControls"
#pragma link "ShellLink"
#pragma link "TntComCtrls"
#pragma link "TntStdCtrls"
#pragma resource "*.dfm"
TForm9 *Form9;
//---------------------------------------------------------------------------
__fastcall TForm9::TForm9(TComponent* Owner)
        : TForm(Owner)
{
 JamShellCombo1->ItemIndex=0;
// ShellListView->Path="";

  filename="Wavesoft截屏.jpg";
}
//---------------------------------------------------------------------------

void __fastcall TForm9::SpeedButton1Click(TObject *Sender)
{
 ShellTree->GoUp();        
}
//---------------------------------------------------------------------------
void __fastcall TForm9::SpeedButton9Click(TObject *Sender)
{
 ShellTree->MoveInHistory(-1);            
}
//---------------------------------------------------------------------------
void __fastcall TForm9::SpeedButton10Click(TObject *Sender)
{
  ShellTree->MoveInHistory(1);           
}
//---------------------------------------------------------------------------
void __fastcall TForm9::SpeedButton3Click(TObject *Sender)
{
  if (ShellListView->Focused())
    ShellListView->InvokeCommandOnSelected("delete");
  if (ShellTree->Focused())
    ShellTree->InvokeCommandOnSelected("delete");        
}
//---------------------------------------------------------------------------
void __fastcall TForm9::SpeedButton4Click(TObject *Sender)
{
   ShellListView->ViewStyle = TViewStyle(((TComponent*)Sender)->Tag + vsIcon);

}
//---------------------------------------------------------------------------
void __fastcall TForm9::SpeedButton7Click(TObject *Sender)
{
  ShellListView->ViewStyle = TViewStyle(((TComponent*)Sender)->Tag + vsIcon);

}
//---------------------------------------------------------------------------
void __fastcall TForm9::SpeedButton8Click(TObject *Sender)
{
   ShellListView->ViewStyle = TViewStyle(((TComponent*)Sender)->Tag + vsIcon);

}
//---------------------------------------------------------------------------
void __fastcall TForm9::FormShow(TObject *Sender)
{
 ComboBox1->Text="WaveSoft截屏";
 filename="WaveSoft截屏.jpg";
 ComboBox2->ItemIndex=0;     
}
//---------------------------------------------------------------------------
void __fastcall TForm9::ShellListViewClick(TObject *Sender)
{
//ComboBox1->Text=ShellListView->Items[ShellListView->ItemIndex].Item->Caption;

}
//---------------------------------------------------------------------------
void __fastcall TForm9::ComboBox2Change(TObject *Sender)
{

 int len=ComboBox1->Text.Length();
  char last[4];
  AnsiString substr="";
if(len>=4)
{
  last[0]=ComboBox1->Text.operator [](len-3);
  last[1]=ComboBox1->Text.operator [](len-2);
  last[2]=ComboBox1->Text.operator [](len-1);
  last[3]=ComboBox1->Text.operator [](len-0);
  substr=ComboBox1->Text.SubString(0,len-4);
 // ShowMessage(substr);
  if(last[0]=='.'&& last[1]=='j' && last[2]=='p' && last[3]=='g' ||//jpg
    last[0]=='.'&& last[1]=='b' && last[2]=='m' && last[3]=='p' ||//bmp
    last[0]=='.'&& last[1]=='g' && last[2]=='i' && last[3]=='f' ||//gif
    last[0]=='.'&& last[1]=='p' && last[2]=='n' && last[3]=='g' )//png
  {

  //  *(ComboBox1->Text.c_str()+len-4)='.'  ;
    if(ComboBox2->ItemIndex==0)
    {  //jpg
     ComboBox1->Text=substr+".jpg";

    }
    else if(ComboBox2->ItemIndex==1)
    {//bmp
     // *(ComboBox1->Text.c_str()+len-3)='b'  ;
     // *(ComboBox1->Text.c_str()+len-2)='m'  ;
     // *(ComboBox1->Text.c_str()+len-1)='p'  ;
     ComboBox1->Text=substr+".bmp";
    }
    else if(ComboBox2->ItemIndex==2)
    {//gif
     ComboBox1->Text=substr+".gif";
    }
    else
    {
    //png
      ComboBox1->Text=substr+".png";
    }
  }
  else
  {
     if(ComboBox2->ItemIndex==0)
    {  //jpg
       ComboBox1->Text=ComboBox1->Text+".jpg";
    }
    else if(ComboBox2->ItemIndex==1)
    {//bmp
       ComboBox1->Text=ComboBox1->Text+".bmp";
    }
    else if(ComboBox2->ItemIndex==2)
    {//gif
       ComboBox1->Text=ComboBox1->Text+".gif";
    }
    else
    {
    //png
      ComboBox1->Text=ComboBox1->Text+".png";
    }
  }
}
else//<4
{
      if(ComboBox2->ItemIndex==0)
    {  //jpg
       ComboBox1->Text=ComboBox1->Text+".jpg";
    }
    else if(ComboBox2->ItemIndex==1)
    {//bmp
       ComboBox1->Text=ComboBox1->Text+".bmp";
    }
    else if(ComboBox2->ItemIndex==2)
    {//gif
       ComboBox1->Text=ComboBox1->Text+".gif";
    }
    else
    {
    //png
      ComboBox1->Text=ComboBox1->Text+".png";
    }
}
filename=ComboBox1->Text;
}
//---------------------------------------------------------------------------

void __fastcall TForm9::Button2Click(TObject *Sender)
{
saved=false;
Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm9::Button1Click(TObject *Sender)
{

    AnsiString ext="";
  AnsiString substr="";
  if(ComboBox2->ItemIndex==0)
    {  //jpg
       ext=".jpg";
    }
    else if(ComboBox2->ItemIndex==1)
    {//bmp
       ext=".bmp";
    }
    else if(ComboBox2->ItemIndex==2)
    {//gif
       ext=".gif";
    }
    else
    {
    //png
      ext=".png";
    }
int len=ComboBox1->Text.Length();
int i;
for(i=len-1;i>=0;i--)
{
     if(*(ComboBox1->Text.c_str()+i)=='.')
     break;
}
if(i==-1)
{//无 已知扩展名
     filename=ComboBox1->Text+ext;
}
else
{
   substr=ComboBox1->Text.SubString(0,i);
   filename=substr+ext;
}
  if(FileExists(AnsiString(ShellListView->Path)+AnsiString(filename)))
  {
   //已经存在
   Form10->Label1->Caption="文件 '"+filename+"' 已经存在,是否覆盖该文件？";
   Form10->ShowModal();
   if(!Form10->YesOrNo)
   {
      saved=false;
   //   Close();
      return;
   }
  }
  //直接保存。

   filename=AnsiString(ShellListView->Path)+AnsiString(filename);
   if(*(filename.c_str()+filename.Length()-1)=='g'&&*(filename.c_str()+filename.Length()-2)=='p')
      {//jpg
        TJPEGImage*jpeg=new TJPEGImage();
        jpeg->CompressionQuality=Form1->CompressionValue;
        try
        {
           jpeg->Assign(temp);
           jpeg->SaveToFile(filename);
        }
        __finally
        {
             delete jpeg;
        }
      }
      else if(*(filename.c_str()+filename.Length()-1)=='g'&&*(filename.c_str()+filename.Length()-2)=='n')
      {  //png
         TPNGObject* png=new TPNGObject();
         try
         {
           png->Assign(temp);
           png->SaveToFile(filename);
         }
         __finally
         {
           delete   png;
         }

      }
      else if(*(filename.c_str()+filename.Length()-1)=='f')
      { //gif
         TGIFImage * gif=new TGIFImage();
        try
         {
           gif->ColorReduction=rmQuantizeWindows;    //量化Gif图，或者用rmQuantizeWindows 也行
           gif->Assign(temp);
           gif->SaveToFile(filename);
         }
         __finally
         {
           delete   gif;
         }
      }
      else
      { //bmp
         temp->SaveToFile(filename);
      }
  saved=true;
   isgoingon=false;
   Close();

}
//---------------------------------------------------------------------------



void __fastcall TForm9::ShellListViewSelectItem(TObject *Sender,
      TListItem *Item, bool Selected)
{
 if(ShellListView->ItemIndex>=0)
 {
  if(!(ShellListView->Items->Item[ShellListView->ItemIndex]->SubItems->Strings[1].operator ==("本地磁盘")
  || ShellListView->IsFolder(ShellListView->ItemIndex)
  || ShellListView->Items->Item[ShellListView->ItemIndex]->SubItems->Strings[1].operator ==("系统文件夹")
  || ShellListView->Items->Item[ShellListView->ItemIndex]->SubItems->Strings[1].operator ==("CD驱动器")
  || ShellListView->Items->Item[ShellListView->ItemIndex]->SubItems->Strings[1].operator ==("可移动磁盘")
  ))
  ComboBox1->Text=ShellListView->Items->Item[ShellListView->ItemIndex]->Caption;
 }
}
//---------------------------------------------------------------------------
void __fastcall TForm9::CreateParams(TCreateParams &Param)
{
        TForm::CreateParams(Param);
    //    Param.WindowClass.style=CS_OWNDC;
        Param.WndParent=NULL;

       // Param.Style|=WS_POPUP;
        Param.ExStyle=WS_EX_TOPMOST; //Param.ExStyle|  WS_EX_TOPMOST|

}
void __fastcall TForm9::Button3Click(TObject *Sender)
{
 Button1->Click();
 isgoingon=true;
}
//---------------------------------------------------------------------------

