//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit3.h"
#include "Unit1.h"
#include "Unit7.h"
#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm3 *Form3;
bool down1;
bool down2;
Graphics::TBitmap*back;
 TPNGObject*up;
TPNGObject*down;
bool IsMouseDown;
int downX,downY;

TRegistry *AutoRun;
int Int(float a)//a should >1 and step >1
{
      a+=0.2f ;
      return int(a);
}

int CheckRGB(int a)
{ return a>255?255:a<0?0:a;}

int checkindex_int(TComboBox*box,AnsiString val)
{
 for(int i=0;i<box->Items->Count;i++)
 {
  if(box->Items->Strings[i].operator ==(val))
  return i;
 }
 return 0;
}
int checkindex_float(TComboBox*box,float val)
{
 for(int i=0;i<box->Items->Count;i++)
 {
  if((StrToFloat(box->Items->Strings[i])-val)<0.01f||(val-StrToFloat(box->Items->Strings[i]))>-0.01f)
  return i;
 }
 return 0;
}

//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
        : TForm(Owner)
{
   AnsiString CurPath=ExtractFilePath(Application->ExeName);
HRGN hrgn;
   TPoint p[8];
   p[0].x=0;p[0].y=5;
   p[1].x=5;p[1].y=0;
   p[2].x=405;p[2].y=0;
   p[3].x=410;p[3].y=5;
   p[4].x=410;p[4].y=273;
   p[5].x=405;p[5].y=278;
   p[6].x=5;p[6].y=278;
   p[7].x=0;p[7].y=273;
   hrgn=::CreatePolygonRgn(p,8,ALTERNATE);
   SetWindowRgn(Form3->Handle,hrgn,true);

back=new Graphics::TBitmap();
back->LoadFromFile(CurPath+"tbsrcima//set.bmp");
up=new TPNGObject();
up->LoadFromFile(CurPath+"tbsrcima//up.png");
down=new TPNGObject();
down->LoadFromFile(CurPath+"tbsrcima//down.png");
 Image3->Picture->Assign(up);
 CheckBox1->Color=RGB(232,253,251);       
  CheckBox2->Color=RGB(232,253,251);
    CheckBox3->Color=RGB(232,253,251);
   CheckBox4->Color=RGB(232,253,251);
   CheckBox5->Color=RGB(232,253,251);
    fromform7=false;

  AutoRun=new TRegistry(KEY_WRITE|KEY_READ);
  AutoRun->RootKey=HKEY_LOCAL_MACHINE;
  AutoRun->OpenKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",false) ;
  if(AutoRun->ValueExists("zhangxiu"))
   CheckBox4->Checked=true;
  else
   CheckBox4->Checked=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm3::ComboBox1Change(TObject *Sender)
{
Form1->Transparency=StrToFloat(ComboBox1->Text);
for(int ii=0;ii<256;ii++)
{
 Form1->_Mask[ii]= CheckRGB(int(ii*Form1->Transparency));
}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::ComboBox2Change(TObject *Sender)
{
Form1->LowerTransparency=StrToFloat(ComboBox2->Text);
for(int ii=0;ii<256;ii++)
{
 Form1->_Masker[ii]= CheckRGB(int(ii*Form1->Transparency*Form1->LowerTransparency));
}

}
//---------------------------------------------------------------------------

void __fastcall TForm3::ComboBox3Change(TObject *Sender)
{
 Form1->LowestTransparency=StrToFloat(ComboBox3->Text);
 for(int ii=0;ii<256;ii++)
{
 Form1->_Maskest[ii]= CheckRGB(int(ii*Form1->Transparency*Form1->LowerTransparency*Form1->LowestTransparency));
}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::ComboBox4Change(TObject *Sender)
{
  Form1->HandleSize=StrToInt(ComboBox4->Text);
  Form1->HalfHandleSize=int(Form1->HandleSize/2);
}
//---------------------------------------------------------------------------

void __fastcall TForm3::ComboBox5Change(TObject *Sender)
{
 Form1->CompressionValue=StrToInt(ComboBox5->Text);
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Image1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 down1=true;
         Canvas->Brush->Color=Image1->Canvas->Pixels[X][Y];
        Form1->RectLineColor=Image1->Canvas->Pixels[X][Y];
        Canvas->Rectangle(32,205,73,220);
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Image1MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
down1=false;        
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Image1MouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
  if(down1)
  {
      if(X>=0&&X<=261&&Y>=0&&Y<=38)
      {
        Canvas->Brush->Color=Image1->Canvas->Pixels[X][Y];
        Form1->RectLineColor=Image1->Canvas->Pixels[X][Y];
        Canvas->Rectangle(32,205,73,220);
      }
      else
      {
        Canvas->Brush->Color=clWhite;
        Form1->RectLineColor=clWhite;
        Canvas->Rectangle(32,205,73,220);
      }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Image2MouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
  if(down2)
  {
     if(X>=0&&X<=261&&Y>=0&&Y<=38)
     {
      Canvas->Brush->Color=Image2->Canvas->Pixels[X][Y];
      Form1->InfoTextColor=Image2->Canvas->Pixels[X][Y];
      Canvas->Rectangle(32,252,73,267);
     }
      else
      {
        Canvas->Brush->Color=clWhite;
        Form1->InfoTextColor=clWhite;
        Canvas->Rectangle(32,252,73,267);
      }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Image2MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
down2=true;
      Canvas->Brush->Color=Image2->Canvas->Pixels[X][Y];
      Form1->InfoTextColor=Image2->Canvas->Pixels[X][Y];
      Canvas->Rectangle(32,252,73,267);
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Image2MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
 down2=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm3::FormPaint(TObject *Sender)
{

      BitBlt(Canvas->Handle,0,0,ClientWidth,ClientHeight,back->Canvas->Handle,0,0,SRCCOPY);
            Canvas->Brush->Color=Form1->RectLineColor;
      Canvas->Rectangle(32,205,73,220);
      Canvas->Brush->Color=Form1->InfoTextColor;
      Canvas->Rectangle(32,252,73,267);
}
//---------------------------------------------------------------------------


void __fastcall TForm3::CheckBox1Click(TObject *Sender)
{
 if(CheckBox1->Checked)
 {
  Form1->CanDrawZeroRect=true;
 }
 else
  Form1->CanDrawZeroRect=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm3::CheckBox2Click(TObject *Sender)
{
 if(CheckBox2->Checked)
 {
  Form1->CanToolBarVisible=false;   //вўВи
 }
 else
  Form1->CanToolBarVisible=true;
}
//---------------------------------------------------------------------------


void __fastcall TForm3::FormMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
   IsMouseDown=true;
   downX=X;
   downY=Y;        
}
//---------------------------------------------------------------------------

void __fastcall TForm3::FormMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
 if(IsMouseDown)
 {
    Form3->Left=Form3->Left+X-downX;
    Form3->Top=Form3->Top+Y-downY;
 }        
}
//---------------------------------------------------------------------------

void __fastcall TForm3::FormMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
  IsMouseDown=false;        
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Image3Click(TObject *Sender)
{
Close();        
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Image3MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 Image3->Picture->Assign(down);
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Image3MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
 Image3->Picture->Assign(up);
}
//---------------------------------------------------------------------------



void __fastcall TForm3::CheckBox3Click(TObject *Sender)
{
 if(CheckBox3->Checked)
 {
  liuxuan->hidemain=true;
 }
 else
  liuxuan->hidemain=false;
}
//---------------------------------------------------------------------------


void __fastcall TForm3::FormShow(TObject *Sender)
{
 if(fromform7)
 {
   Form7->Close();
   Sleep(20);
   fromform7=false;
 }

 if(Form1->CanDrawZeroRect){ CheckBox1->Checked=true;}else{CheckBox1->Checked=false;}
 if(liuxuan->hidemain){ CheckBox3->Checked=true;}else{CheckBox3->Checked=false;}
 if(Form1->CanToolBarVisible){ CheckBox2->Checked=false;}else{CheckBox2->Checked=true;}
 if(Form1->ShowMainFormAfterCut){ CheckBox5->Checked=false;}else{CheckBox5->Checked=true;}
 ComboBox1->ItemIndex=checkindex_float(ComboBox1,(Form1->Transparency));
 ComboBox2->ItemIndex=checkindex_float(ComboBox2,(Form1->LowerTransparency));
 ComboBox3->ItemIndex=checkindex_float(ComboBox3,(Form1->LowestTransparency));
 ComboBox4->ItemIndex=checkindex_int(ComboBox4,IntToStr(Form1->HandleSize));
 ComboBox5->ItemIndex=checkindex_int(ComboBox5,IntToStr(Form1->CompressionValue));

}
//---------------------------------------------------------------------------

void __fastcall TForm3::FormClose(TObject *Sender, TCloseAction &Action)
{

 AnsiString write="";
 write=AnsiString(GetRValue((DWORD)Form1->RectLineColor))+";"+AnsiString(GetGValue((DWORD)Form1->RectLineColor))+";"+AnsiString(GetBValue((DWORD)Form1->RectLineColor))+";";
 write+=AnsiString(Int(Form1->Transparency*10.0f))+";";
 write+=AnsiString(Int(Form1->LowerTransparency*10.0f))+";";
 write+=AnsiString(Int(Form1->LowestTransparency*10.0f))+";";
 write+=AnsiString(GetRValue((DWORD)Form1->InfoTextColor))+";"+AnsiString(GetGValue((DWORD)Form1->InfoTextColor))+";"+AnsiString(GetBValue((DWORD)Form1->InfoTextColor))+";";
 write+=AnsiString(Form1->HandleSize)+";";
 write+=AnsiString(int(Form1->CanToolBarVisible))+";";
 write+=AnsiString(int(liuxuan->hidemain))+";";
 write+=AnsiString(Form1->CompressionValue)+";";
 liuxuan->intial_key->WriteString("yangmei",write);
 if(CheckBox4->Checked)
 {
    try
  {
   AutoRun->WriteString("zhangxiu",Application->ExeName);
  }
  catch(...)
  {
   ;
  }


 }
 else
 {
  try
  {
  AutoRun->DeleteValue("zhangxiu");
  }
  catch(...)
  {
   ;
  }
 }


}
//---------------------------------------------------------------------------

__fastcall TForm3::~TForm3()
{
  AutoRun->CloseKey();
  delete AutoRun;
}
void __fastcall TForm3::CheckBox5Click(TObject *Sender)
{
 if(CheckBox5->Checked)
    Form1->ShowMainFormAfterCut=false;
 else
    Form1->ShowMainFormAfterCut=true;
}
//---------------------------------------------------------------------------



