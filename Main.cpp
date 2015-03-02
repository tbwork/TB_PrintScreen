//---------------------------------------------------------------------------


#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "Unit1.h"
#include "Unit3.h"
#include "Unit4.h"
#include "Unit6.h"
#include "Unit7.h"
#include "Unit8.h"
#include "Unit9.h"

#include <vcl\Clipbrd.hpp>

#define MSG_APPBAR_MSGID (WM_USER+3)
//#define StrToInt StrToInt_User
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "pngimage"
#pragma link "trayicon"
#pragma resource "*.dfm"

TList*bmpsrc;
Tliuxuan *liuxuan;

HMODULE hdll=NULL;
bool ImportDllOK=false;
bool bigshow;//是否是大窗口显示

extern "C" _declspec(dllexport) bool WINAPI starthook(HWND);
extern "C" _declspec(dllexport) bool WINAPI stophook();
//资源位图
Graphics::TBitmap* MainBackGround;
Graphics::TBitmap*SmallMainBackGround;
Graphics::TBitmap* button1_1;
Graphics::TBitmap* button1_2;

 TPNGObject* CloseNoth;
 TPNGObject* CloseMove;
 TPNGObject* CloseDown;
 TPNGObject* MinNoth;
 TPNGObject* MinMove;
 TPNGObject* MinDown;
 TPNGObject* jieping_up;
 TPNGObject* jieping_down;
 TPNGObject* baocun_up;
 TPNGObject* baocun_down;
 TPNGObject* shezhi_up;
 TPNGObject* shezhi_down;
 TPNGObject* tuichu_up;
 TPNGObject* tuichu_down;

 Graphics::TBitmap* TangBo;
 AnsiString ImageInformation;
 bool IsMouseDown;
 int downX,downY;

 int tencifang(int a)
 {
   int re=1;
   while(a>0)
   {
     re*=10;
     a--;
   }
   return re;
 }

 int Tliuxuan::StrToInt_User( AnsiString a)
 {
    int len=a.Length();
    int re=0;
    for(int i=0;i<len;i++)
    {
     re+=(*(a.c_str()+i)-48)*tencifang(len-i-1);
    }
    return re;
 }
//---------------------------------------------------------------------------
__fastcall Tliuxuan::Tliuxuan(TComponent* Owner)
        : TForm(Owner)
{
   AnsiString CurPath=ExtractFilePath(Application->ExeName);
   MainBackGround=new Graphics::TBitmap();
   MainBackGround->LoadFromFile(CurPath+"tbsrcima\\main.bmp");
   SmallMainBackGround=new Graphics::TBitmap();
   SmallMainBackGround->LoadFromFile(CurPath+"tbsrcima\\smallmain.bmp");
   FormBigShow();
   button1_1=new Graphics::TBitmap();
   button1_1->LoadFromFile(CurPath+"tbsrcima\\button1_1.bmp");
   button1_2=new Graphics::TBitmap();
   button1_2->LoadFromFile(CurPath+"tbsrcima\\button1_2.bmp");
   NoPicture=new Graphics::TBitmap();
   NoPicture->LoadFromFile(CurPath+"tbsrcima\\NoPicture.bmp");
   TangBo=new Graphics::TBitmap();
   TangBo->LoadFromFile(CurPath+"tbsrcima\\Start.bmp");
   CloseNoth=new TPNGObject();
   CloseNoth->LoadFromFile(CurPath+"tbsrcima\\CloseNoth.png");
   CloseMove=new TPNGObject();
   CloseMove->LoadFromFile(CurPath+"tbsrcima\\CloseMove.png");
   CloseDown=new TPNGObject();
   CloseDown->LoadFromFile(CurPath+"tbsrcima\\CloseDown.png");
   MinNoth=new TPNGObject();
   MinNoth->LoadFromFile(CurPath+"tbsrcima\\MinNoth.png");
   MinMove=new TPNGObject();
   MinMove->LoadFromFile(CurPath+"tbsrcima\\MinMove.png");
   MinDown=new TPNGObject();
   MinDown->LoadFromFile(CurPath+"tbsrcima\\MinDown.png");
   jieping_up=new TPNGObject();
   jieping_up->LoadFromFile(CurPath+"tbsrcima\\jieping_up.png");
   jieping_down=new TPNGObject();
   jieping_down->LoadFromFile(CurPath+"tbsrcima\\jieping_down.png");
   baocun_up=new TPNGObject();
   baocun_up->LoadFromFile(CurPath+"tbsrcima\\baocun_up.png");
   baocun_down=new TPNGObject();
   baocun_down->LoadFromFile(CurPath+"tbsrcima\\baocun_down.png");
   shezhi_up=new TPNGObject();
   shezhi_up->LoadFromFile(CurPath+"tbsrcima\\shezhi_up.png");
   shezhi_down=new TPNGObject();
   shezhi_down->LoadFromFile(CurPath+"tbsrcima\\shezhi_down.png");
   tuichu_up=new TPNGObject();
   tuichu_up->LoadFromFile(CurPath+"tbsrcima\\tuichu_up.png");
   tuichu_down=new TPNGObject();
   tuichu_down->LoadFromFile(CurPath+"tbsrcima\\tuichu_down.png");
   Image7->Picture->Assign(jieping_up);
   Image8->Picture->Assign(shezhi_up);
   Image9->Picture->Assign(baocun_up);
   Image10->Picture->Assign(tuichu_up);
  // NoPicture=new Graphics::TBitmap();
  // Start=new Graphics::TBitmap();
  CutList=new TList();
  DateList=new TList();

  IsMouseDown=false;
  Image6->Picture->Assign(CloseNoth);
  Image5->Picture->Assign(MinNoth);

  bmpok5=true;
  bmpok6=true;
  bigshow=true;
  pImage=0;

  OldImageWndProc5=Image5->WindowProc;
  Image5->WindowProc=NewImageWndProc5;
  OldImageWndProc6=Image6->WindowProc;
  Image6->WindowProc=NewImageWndProc6;
  ImageInformation="";

  //-------------------bmp initial--------------------------------


  hdll=LoadLibrary("KeyMonitor");
  if(hdll)
  {
   ImportDllOK=true;
  }
 hidemain=false;
 TrayIcon1->Visible=true;
 TrayIcon1->IconIndex=0;
         ShowImageInfo() ;
         BitBlt(Canvas->Handle,204,8,300,20,MainBackGround->Canvas->Handle,204,8,SRCCOPY);
         ::SetBkMode(liuxuan->Canvas->Handle,0);
         TextOut(liuxuan->Canvas->Handle,204,8,ImageInformation.c_str(),ImageInformation.Length());

  intial_key=new TRegistry(KEY_READ|KEY_WRITE);
  InstallFullWatch();
}
//---------------------------------------------------------------------------


__fastcall Tliuxuan::~Tliuxuan()
{           //release resource
// CloseHandle(hdll);
try
{
  delete MainBackGround;
  intial_key->CloseKey();
  delete intial_key;
}
catch(...)
{
 ;//nothing
}
}





void __fastcall Tliuxuan::Image6Click(TObject *Sender)
{
 if(CutList->Count)
 {
  if(MessageBox(Application->Handle,"程序中尚有截图，确定退出？","询问-Wave截屏",MB_OKCANCEL+MB_ICONQUESTION)==1)
  {
      Close();
  }
 }
 else
   Close();

}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::Image5Click(TObject *Sender)
{
   TrayIcon1->Minimize();
}
//---------------------------------------------------------------------------


void __fastcall Tliuxuan::Image5MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  Image5->Picture->Assign(MinDown);

}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::Image5MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
  if(X>=0&&X<=Image5->Width&&Y>=0&&Y<=Image5->Height)
  Image5->Picture->Assign(MinMove);
}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::Image6MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
  if(X>=0&&X<=Image6->Width&&Y>=0&&Y<=Image6->Height)
  Image6->Picture->Assign(CloseMove);
}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::Image6MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  Image6->Picture->Assign(CloseDown);
}
//---------------------------------------------------------------------------




void __fastcall Tliuxuan::FormMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
 //  CleanMin_Close();

 if(IsMouseDown)
 {
    liuxuan->Left=liuxuan->Left+X-downX;
    liuxuan->Top=liuxuan->Top+Y-downY;
 }

}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::FormMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
  IsMouseDown=false;
 // if(liuxuan->Left<=0)

}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::FormMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
 IsMouseDown=true;
   downX=X;
   downY=Y;
}
//---------------------------------------------------------------------------
void __fastcall Tliuxuan::NewImageWndProc6(TMessage&Message)
{
    if(Message.Msg==CM_MOUSELEAVE)
    {
        Image6->Picture->Assign(CloseNoth);
        bmpok6=true  ;
    }
    else if(Message.Msg==CM_MOUSEENTER)
    {
        Image6->Picture->Assign(CloseMove);
        bmpok6=false  ;
    }

    OldImageWndProc6(Message);
}

//----------------------------------------------------------------------
void __fastcall Tliuxuan::NewImageWndProc5(TMessage&Message)
{
    if(Message.Msg==CM_MOUSELEAVE)
    {
        Image5->Picture->Assign(MinNoth);
        bmpok5=true  ;
    }
    else if(Message.Msg==CM_MOUSEENTER)
    {
        Image5->Picture->Assign(MinMove);
        bmpok5=false;
    }

    OldImageWndProc5(Message);
}
//----------------------------------------------------------------------
void __fastcall Tliuxuan::WndProc(TMessage &msg)
 {
    if(msg.Msg==MSG_APPBAR_MSGID)
 {
     switch((UINT)msg.WParam)
        {
        case ABN_FULLSCREENAPP:
            {
                if (true == (BOOL)msg.LParam)
                {
                  //  ShowMessage("一个窗口全屏了\n");
                    //KAppBarMsg::m_bFullScreen = TRUE;
                    hasfullscreen=true;
                }
                else
                {
                   //  ShowMessage("一个窗口取消全屏了\n");
                 //   KAppBarMsg::m_bFullScreen = FALSE;
                 hasfullscreen=false;
                }
            }
            break;
        default:
            break;
        }
 }
 else if(msg.Msg==WM_MOUSELEAVE)
   {
      if(!bmpok5)
      {
       Image5->Picture->Assign(MinNoth);
       bmpok5=true;
      }
      if(!bmpok6)
      {
       Image6->Picture->Assign(CloseNoth);
        bmpok6=true;
      }
   }
    else if(msg.Msg==WM_SHOWWINDOW)
   {
     if(msg.WParam==-1&&msg.LParam==-1)
     {//截全图
      Form1->FullCut();
      if(TBNote->needshow)
       TBNote->Show();
      ShowImageInfo() ;
      BitBlt(Canvas->Handle,204,8,300,20,MainBackGround->Canvas->Handle,204,8,SRCCOPY);
      ::SetBkMode(liuxuan->Canvas->Handle,0);
      TextOut(liuxuan->Canvas->Handle,204,8,ImageInformation.c_str(),ImageInformation.Length());

      return ;
     }
     else if(msg.WParam==-2&&msg.LParam==-2)
     {
       TrayIcon1->Restore();
       liuxuan->Show();
     }
   }

   TForm::WndProc(msg);
 }


void __fastcall Tliuxuan::FormClose(TObject *Sender, TCloseAction &Action)
{
try{
 bool(WINAPI*unhook)();
   unhook=(bool(WINAPI*)())GetProcAddress(hdll,"stophook");
   unhook();
 CutList->Clear();
 delete CutList;
 }
 catch(...)
 {
  ;
 }
}
//---------------------------------------------------------------------------
/*void Tliuxuan::SaveToFile_Main()
{
   AnsiString filename;
    if(Form1->SaveDialog1->Execute())
    {
      filename=Form1->SaveDialog1->FileName;
      if(Form1->SaveDialog1->DefaultExt.IsEmpty())
      {
        switch(Form1->SaveDialog1->FilterIndex)
        {
           case 1:filename+=".png";
                 break;
           case 2:filename+=".bmp";
                 break;
           case 3:filename+=".gif";
                 break;
           default:filename+=".jpg";
         }
       }
       if(*(filename.c_str()+filename.Length()-1)=='g'&&*(filename.c_str()+filename.Length()-2)=='p')
      {//jpg
        TJPEGImage*jpeg=new TJPEGImage();
        jpeg->CompressionQuality=Form1->CompressionValue;
        try
        {
           jpeg->Assign(Image3->Picture->Bitmap);
           jpeg->SaveToFile(filename);
        }
        __finally
        {
           delete   jpeg;
        }
      }
      else if(*(filename.c_str()+filename.Length()-1)=='g'&&*(filename.c_str()+filename.Length()-2)=='n')
      {  //png
         TPNGObject* png=new TPNGObject();
         try
         {
           png->Assign(Image3->Picture->Bitmap);
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
           gif->Assign(Image3->Picture->Bitmap);
           gif->SaveToFile(filename);
         }
         __finally
         {
           delete   gif;
         }
      }
      else
      { //bmp
         Image3->Picture->Bitmap->SaveToFile(filename);
      }
      Form1->Close();
    }

}      */
//-----------------------------------------------------------------------------------------
void Tliuxuan::ShowImageInfo()
{
  if(CutList->Count==0)
  {
  ImageInformation=" 暂无截图，单击截图开始截图。"  ;
  Image4->Picture->Assign(NoPicture);
  Image3->Picture->Assign(TangBo);
  Image2->Picture->Assign(NoPicture);
  pImage=-1;
  }
  else
  ImageInformation=" 序号："+AnsiString(pImage+1)+"/共"+AnsiString(CutList->Count)+"张 "+AnsiString(*((AnsiString*)DateList->Items[pImage]));

}

void __fastcall Tliuxuan::Image4Click(TObject *Sender)
{
   if(pImage<CutList->Count-2)
   {
      Image2->Picture->Bitmap->Assign((Graphics::TBitmap*)CutList->Items[pImage]);
      Image3->Picture->Bitmap->Assign((Graphics::TBitmap*)CutList->Items[pImage+1]);
      Image4->Picture->Bitmap->Assign((Graphics::TBitmap*)CutList->Items[pImage+2]);
      pImage++;
        ShowImageInfo();
           BitBlt(Canvas->Handle,204,8,300,20,MainBackGround->Canvas->Handle,204,8,SRCCOPY);
  ::SetBkMode(liuxuan->Canvas->Handle,0);
  TextOut(liuxuan->Canvas->Handle,204,8,ImageInformation.c_str(),ImageInformation.Length());

   }
   else if(pImage==(CutList->Count-2))
   {
      Image2->Picture->Bitmap->Assign((Graphics::TBitmap*)CutList->Items[pImage]);
      Image3->Picture->Bitmap->Assign((Graphics::TBitmap*)CutList->Items[pImage+1]);
      Image4->Picture->Bitmap=NoPicture;
      pImage++;
      ShowImageInfo();
      BitBlt(Canvas->Handle,204,8,300,20,MainBackGround->Canvas->Handle,204,8,SRCCOPY);
      ::SetBkMode(liuxuan->Canvas->Handle,0);
      TextOut(liuxuan->Canvas->Handle,204,8,ImageInformation.c_str(),ImageInformation.Length());
   }
}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::Image2Click(TObject *Sender)
{
   if(pImage>1)
   {
      Image2->Picture->Bitmap->Assign((Graphics::TBitmap*)CutList->Items[pImage-2]);
      Image3->Picture->Bitmap->Assign((Graphics::TBitmap*)CutList->Items[pImage-1]);
      Image4->Picture->Bitmap->Assign((Graphics::TBitmap*)CutList->Items[pImage]);
      pImage--;
      ShowImageInfo();
      BitBlt(Canvas->Handle,204,8,300,20,MainBackGround->Canvas->Handle,204,8,SRCCOPY);
      ::SetBkMode(liuxuan->Canvas->Handle,0);
      TextOut(liuxuan->Canvas->Handle,204,8,ImageInformation.c_str(),ImageInformation.Length());

   }
   else if(pImage==1)
   {
      Image2->Picture->Bitmap=NoPicture;
      Image3->Picture->Bitmap->Assign((Graphics::TBitmap*)CutList->Items[pImage-1]);
      Image4->Picture->Bitmap->Assign((Graphics::TBitmap*)CutList->Items[pImage]);
      pImage--;
      ShowImageInfo();
             BitBlt(Canvas->Handle,204,8,300,20,MainBackGround->Canvas->Handle,204,8,SRCCOPY);
  ::SetBkMode(liuxuan->Canvas->Handle,0);
  TextOut(liuxuan->Canvas->Handle,204,8,ImageInformation.c_str(),ImageInformation.Length());

   }
}
//---------------------------------------------------------------------------



void __fastcall Tliuxuan::FormShow(TObject *Sender)
{

 //
//   liuxuan->Left=liuxuan->formleft;
//   liuxuan->Top=liuxuan->formtop;
  if(ImportDllOK)
  { //运行以来，第一次显示
    //安装钩子
    try
    {
    bool (WINAPI*sethook)(HWND,HWND);
    sethook=(bool (WINAPI*)(HWND,HWND))GetProcAddress(hdll,"starthook");
    sethook(Form1->Handle,liuxuan->Handle);
    ImportDllOK=false;
    //注册表初始化参数
    intial_key->RootKey=HKEY_LOCAL_MACHINE;
    if(intial_key->OpenKey("Software\\Microsoft\\Wavesoft",false))
    {//
      AnsiString intial_string=intial_key->ReadString("yangmei");
      if(intial_string.Length()<20)
      {
       intial_key->CloseKey();
       intial_key->DeleteKey("Software\\Microsoft\\Wavesoft");
       ShowMessage("注册表被修改，请重新启动程序。");
       Close();
      }
      else
      {
        try
        {
          BYTE temp[13];
          BYTE ptemp=0;
          AnsiString ts="";
          char*p=intial_string.c_str();
          while(*p!='\0')
          {
            if(*p==';')
            {
              temp[ptemp]=StrToInt_User(ts);
              ptemp++;
              ts="";
            }
            else
            {
              ts+=(*p);
            }
            p++;
          }

          // 0; 34; 255;5;5;8;255;255;255;5;1;0;4;
          // 20;120;255;6;7;8;255;255;255;5;1;0;100;
          Form1->RectLineColor=RGB(temp[0],temp[1],temp[2]);
          Form1->Transparency=float(float(temp[3])/10.0f);
          Form1->LowerTransparency=float(float(temp[4])/10.0f);
          Form1->LowestTransparency=float(float(temp[5])/10.0f);
          Form1->InfoTextColor=RGB(temp[6],temp[7],temp[8]);
          Form1->HandleSize=temp[9];
          Form1->HalfHandleSize=int(temp[9]/2);
          Form1->CanToolBarVisible=bool(temp[10]);
          liuxuan->hidemain=bool(temp[11]);
          Form1->CompressionValue=temp[12];
         // 取值初始化
        }
        catch(...)
        {
            intial_key->CloseKey();
            intial_key->DeleteKey("Software\\Microsoft\\Wavesoft");
            ShowMessage("注册表被非法修改，请重新启动程序。");
            Close();
        }
      }
    }
    else
    {//第一次运行
       intial_key->CreateKey("Software\\Microsoft\\Wavesoft");
       intial_key->OpenKey("Software\\Microsoft\\Wavesoft",true);
       intial_key->WriteString("yangmei","20;120;255;6;7;8;255;255;255;5;1;0;100;");
        Form7->ShowModal();
        if(Form7->IsSet)
        {
           Form3->ShowModal();
        }

    }



    }
    catch(...)
    {

     ShowMessage("注册表被禁用，或者数据已经损坏，无法启动。请启用注册表或者重新安装后重试。");
     Close();

    }
  }
}
//---------------------------------------------------------------------------



void __fastcall Tliuxuan::N3Click(TObject *Sender)
{
 Image2->OnClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::N4Click(TObject *Sender)
{
  Image4->OnClick(Sender);
}
//---------------------------------------------------------------------------

void Tliuxuan::SaveToFile_user()
{
   Form9->temp=(Graphics::TBitmap*)CutList->Items[pImage];
   Form9->Button3->Enabled=false;
   Form9->ShowModal();

}

void __fastcall Tliuxuan::Save1Click(TObject *Sender)
{
if(liuxuan->CutList->Count==0)
{
  MessageBox(Application->Handle,"暂无截图","提醒-Wave截屏",MB_ICONASTERISK);
  return;
}
 SaveToFile_user();
}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::Image3DblClick(TObject *Sender)
{
 if(CutList->Count==0)
 {
     ShellExecute(Form1->Handle,"open","http://renren.com/woailuotangbo",NULL,NULL,SW_NORMAL);
 }
 else
 {
     N12->Click();
   // SaveToFile_user();
 }
}
//---------------------------------------------------------------------------


void __fastcall Tliuxuan::N1Click(TObject *Sender)
{
if(liuxuan->CutList->Count==0)
{
  MessageBox(Application->Handle,"暂无截图","提醒-Wave截屏",MB_ICONASTERISK);
  return;
}
   try
    {
        Clipboard()->Open();
        Clipboard()->Assign((Graphics::TBitmap*)CutList->Items[pImage]);

        // Clipboard()->SetAsHandle(MyFormat,DataHandle);
    }
    __finally
    {
        Clipboard()->Close();
    }
 /*      int DataHandle;
  HPALETTE APalette;
  unsigned short MyFormat;

 // ()->SaveToClipboardFormat(MyFormat,DataHandle,APalette);
      Image3->Picture->Bitmap->SaveToClipboardFormat(MyFormat,DataHandle,APalette);
    // save the data to the clipboard using that format and
    // the generated data
  Clipboard()->SetAsHandle(MyFormat,DataHandle); */

}
//---------------------------------------------------------------------------
BOOL Tliuxuan::SaveToClipboard()
{ 
    // 如果本身类中原来没有位图，函数返回FALSE 
 /*   if (!IsValid())
        return FALSE;

    ASSERT(m_pInfo);
    ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER)); 

    // 打开剪贴板 
    if (::OpenClipboard(NULL)) 
    { 
        // 清空剪贴板 
        if (::EmptyClipboard()) 
        { 
            DWORD DibSize = CalculateDIBSize((LPBITMAPINFOHEADER)m_pInfo); 
            DWORD infosize =CalculateDIBInfoSize((LPBITMAPINFOHEADER)m_pInfo); 

            // 分配DIB内部格式内存块 
            HANDLE hDib = ::GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE, DibSize); 
            if (hDib) 
            { 
                LPSTR lpDib = (LPSTR)::GlobalLock(hDib); 
                if (lpDib) 
                { 
                    ::memcpy((void*)lpDib, (const void*)m_pInfo, infosize); 

                    // 获取DIB位数据 
                    HWND hWnd = ::GetDesktopWindow(); 
                    ASSERT(hWnd); 
                    HDC hDC = ::GetDC(hWnd); 
                    if (!::GetDIBits(hDC,(HBITMAP)m_hObject,0,m_pInfo->bmiHeader.biHeight,\ 
                            (LPVOID)((DWORD)lpDib+infosize),(LPBITMAPINFO)lpDib,DIB_RGB_COLORS)) 
                    { 
                        ::GlobalUnlock(hDib); 
                        ::GlobalFree(hDib); 
                        ::CloseClipboard();
                        ::ReleaseDC(hWnd, hDC); 
                        return FALSE; 
                    } 
                    ::ReleaseDC(hWnd, hDC); 

                    ::GlobalUnlock(hDib); 

                    // 将DIB数据贴入剪贴板 
                    if (::SetClipboardData(CF_DIB, hDib)) 
                    {
                        // 如果成功，DIB句柄不应释放
                        ::CloseClipboard();
                        return TRUE;        // 成功
                    } 
                    else 
                    { 
                        ::GlobalFree(hDib); 
                        ::CloseClipboard(); 
                        return FALSE;        // 失败 
                    } 
                } 
                ::GlobalFree(hDib); 
            } 
        } 
        ::CloseClipboard(); 
    } 

    return FALSE;    // 失败    */
}

//-----------------------------------------------------------------
void Tliuxuan::FormBigShow()
{
   HRGN hrgn;
   TPoint p[8];
   p[0].x=0;p[0].y=4;
   p[1].x=4;p[1].y=0;
   p[2].x=678;p[2].y=0;
   p[3].x=682;p[3].y=4;
   p[4].x=682;p[4].y=191;
   p[5].x=676;p[5].y=197;
   p[6].x=6;p[6].y=197;
   p[7].x=0;p[7].y=191;
   hrgn=::CreatePolygonRgn(p,8,ALTERNATE);
   SetWindowRgn(liuxuan->Handle,hrgn,true);
   Image5->Left=572;
   Image5->Top=2;
   Image6->Left=624;
   Image6->Top=2;
}
//-------------------------------------------------------------------
void Tliuxuan::FormSmallShow()
{
   HRGN hrgn;
   TPoint p[8];
   p[0].x=0;p[0].y=4;
   p[1].x=4;p[1].y=0;
   p[2].x=202;p[2].y=0;
   p[3].x=206;p[3].y=5;
   p[4].x=206;p[4].y=191;
   p[5].x=200;p[5].y=197;
   p[6].x=6;p[6].y=197;
   p[7].x=0;p[7].y=191;
   hrgn=::CreatePolygonRgn(p,8,ALTERNATE);
   SetWindowRgn(liuxuan->Handle,hrgn,true);
   Image5->Left=98;
   Image5->Top=2;
   Image6->Left=150;
   Image6->Top=2;
}


void __fastcall Tliuxuan::Image1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if(bigshow)
  {
     FormSmallShow();
     bigshow=false;
     Image1->Picture->Assign(button1_1);
     BitBlt(Canvas->Handle,0,0,ClientWidth,ClientHeight,SmallMainBackGround->Canvas->Handle,0,0,SRCCOPY);

  }
  else
  {
     FormBigShow();
     bigshow=true;
     Image1->Picture->Assign(button1_2);
     BitBlt(Canvas->Handle,0,0,ClientWidth,ClientHeight,MainBackGround->Canvas->Handle,0,0,SRCCOPY);
  }
    Image7->Picture->Assign(jieping_up);
   Image8->Picture->Assign(shezhi_up);
   Image9->Picture->Assign(baocun_up);
   Image10->Picture->Assign(tuichu_up);
    ::SetBkMode(liuxuan->Canvas->Handle,0);
  TextOut(liuxuan->Canvas->Handle,32,8,"Wave截屏",8);
  TextOut(liuxuan->Canvas->Handle,204,8,ImageInformation.c_str(),ImageInformation.Length());
  TextOut(liuxuan->Canvas->Handle,275,168,"上一张",6);
  TextOut(liuxuan->Canvas->Handle,587,168,"下一张",6);


}
//---------------------------------------------------------------------------



void __fastcall Tliuxuan::TrayIcon1Click(TObject *Sender)
{
 liuxuan->Show();
}
//---------------------------------------------------------------------------


void __fastcall Tliuxuan::Show1Click(TObject *Sender)
{

TrayIcon1->Restore();
liuxuan->Show();
}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::Edit1Click(TObject *Sender)
{
Close();        
}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::Image7MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 Image7->Picture->Assign(jieping_down);
}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::Image7MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
  Image7->Picture->Assign(jieping_up);
   
}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::Image8MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
 Image8->Picture->Assign(shezhi_up);
}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::Image8MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 Image8->Picture->Assign(shezhi_down);
}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::Image9MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 Image9->Picture->Assign(baocun_down);
}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::Image9MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
 Image9->Picture->Assign(baocun_up);
}
//---------------------------------------------------------------------------


void __fastcall Tliuxuan::Image10MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 Image10->Picture->Assign(tuichu_down);
}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::Image10MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 Image10->Picture->Assign(tuichu_up);
}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::Image9Click(TObject *Sender)
{
if(liuxuan->CutList->Count==0)
{
  MessageBox(Application->Handle,"暂无截图","提醒-Wave截屏",MB_ICONASTERISK);
  return;
}
 Form4->ShowModal();
}
//---------------------------------------------------------------------------


void __fastcall Tliuxuan::Image10Click(TObject *Sender)
{
 if(CutList->Count)
 {
  if(MessageBox(Application->Handle,"程序中尚有截图，确定退出？","询问-Wave截屏",MB_OKCANCEL+MB_ICONQUESTION)==1)
  {
      Close();
  }
 }
 else
   Close();       
}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::Image8Click(TObject *Sender)
{
    Form3->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::Image7Click(TObject *Sender)
{
//ShowMessage(AnsiString(bmp->Width)+"::"+AnsiString(bmp->Height));
//ShowMessage(AnsiString(Screen->Width)+"::"+AnsiString(Screen->Height));
// bmp->LoadFromFile("1.bmp");
//TCanvas*c0=new TCanvas();
//c0->Handle=GetDC(GetDesktopWindow());
//bmp->Canvas->CopyRect(Rect(0,0,Screen->Width,Screen->Height),c0,Rect(0,0,Screen->Width,Screen->Height));
// BitBlt(bmp->Canvas->Handle,0,0,Screen->Width,Screen->Height,,0,0,SRCCOPY);
try{
  Form1->Show();
 //ShowWindow(Form1->Handle,SW_SHOWNORMAL);
// SendMessage(Form1->Handle,WM_SHOWWINDOW,-1,-1);
}
catch(...)
{
  ShowMessage("不可能吧，刘璇");
}

}
//---------------------------------------------------------------------------


void __fastcall Tliuxuan::FormPaint(TObject *Sender)
{
  if(bigshow)
   BitBlt(Canvas->Handle,0,0,ClientWidth,ClientHeight,MainBackGround->Canvas->Handle,0,0,SRCCOPY);
  else
  BitBlt(Canvas->Handle,0,0,ClientWidth,ClientHeight,SmallMainBackGround->Canvas->Handle,0,0,SRCCOPY);
  ::SetBkMode(liuxuan->Canvas->Handle,0);
  TextOut(liuxuan->Canvas->Handle,32,8,"Wave截屏",8);
  TextOut(liuxuan->Canvas->Handle,204,8,ImageInformation.c_str(),ImageInformation.Length());
  TextOut(liuxuan->Canvas->Handle,275,168,"上一张",6);
  TextOut(liuxuan->Canvas->Handle,587,168,"下一张",6);       
}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::Image3Click(TObject *Sender)
{
 if(CutList->Count==0)
 {
     ShellExecute(Form1->Handle,"open","http://renren.com/woailuotangbo",NULL,NULL,SW_NORMAL);
 }
 else
 {

 }
}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::Button1Click(TObject *Sender)
{
 liuxuan->Hide();
}
//---------------------------------------------------------------------------





void __fastcall Tliuxuan::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if(CutList->Count&&Key==67&&Shift.Contains(ssCtrl))
 {//Ctrl+C
      try
    {
        Clipboard()->Open();
        Clipboard()->Assign((Graphics::TBitmap*)CutList->Items[pImage]);

        // Clipboard()->SetAsHandle(MyFormat,DataHandle);
    }
    __finally
    {
        Clipboard()->Close();
    }
 }


}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::N7Click(TObject *Sender)
{
   if(CutList->Count==0)
   {
       MessageBox(Application->Handle,"暂无截图","提醒-Wave截屏",MB_OK+MB_ICONASTERISK);
       return;
   }
   else
   {
      if(MessageBox(Application->Handle,"确定删除该图？单击“确定”按钮确定，单击“取消”按钮取消。","询问-Wave截屏",MB_OKCANCEL+MB_ICONQUESTION)==1)
      {
         CutList->Delete(pImage);
         DateList->Delete(pImage);
         if(CutList->Count==0)
         {       //没有图了
          ;
         }
         else if(pImage<=CutList->Count-2)
         {//可以往前进一张
           Image3->Picture->Bitmap->Assign((Graphics::TBitmap*)CutList->Items[pImage]);
           Image4->Picture->Bitmap->Assign((Graphics::TBitmap*)CutList->Items[pImage+1]);
          //pImage 不变
         }
         else
         if(pImage==CutList->Count-1)
         {//可以往前进一张
           Image3->Picture->Bitmap->Assign((Graphics::TBitmap*)CutList->Items[pImage]);
           Image4->Picture->Bitmap->Assign(NoPicture);
         }
         else
         {//不能前进了，只能往后退
           if(pImage>=2)
            Image2->Picture->Bitmap->Assign((Graphics::TBitmap*)CutList->Items[pImage-1]);
           else
            Image2->Picture->Bitmap->Assign(NoPicture);
           Image3->Picture->Bitmap->Assign((Graphics::TBitmap*)CutList->Items[pImage-1]);
           pImage--;

         }

        //更新显示
         ShowImageInfo() ;
         BitBlt(Canvas->Handle,204,8,300,20,MainBackGround->Canvas->Handle,204,8,SRCCOPY);
         ::SetBkMode(liuxuan->Canvas->Handle,0);
         TextOut(liuxuan->Canvas->Handle,204,8,ImageInformation.c_str(),ImageInformation.Length());

      }
   }
}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::N8Click(TObject *Sender)
{
   if(CutList->Count==0)
   {
       MessageBox(Application->Handle,"暂无截图","提醒-Wave截屏",MB_OK+MB_ICONASTERISK);
       return;
   }
   else
   {
      if(MessageBox(Application->Handle,"确定全部删除？单击“确定”按钮确定，单击“取消”按钮取消。","询问-Wave截屏",MB_OKCANCEL+MB_ICONQUESTION)==1)
      {
         CutList->Clear();
         DateList->Clear();
        //更新显示
         ShowImageInfo() ;
         BitBlt(Canvas->Handle,204,8,300,20,MainBackGround->Canvas->Handle,204,8,SRCCOPY);
         ::SetBkMode(liuxuan->Canvas->Handle,0);
         TextOut(liuxuan->Canvas->Handle,204,8,ImageInformation.c_str(),ImageInformation.Length());

      }
   }
}
//---------------------------------------------------------------------------

void __fastcall Tliuxuan::N10Click(TObject *Sender)
{
 ShellExecute(Form1->Handle,"open","http://renren.com/woailuotangbo",NULL,NULL,SW_NORMAL);        
}
//---------------------------------------------------------------------------


void __fastcall Tliuxuan::N12Click(TObject *Sender)
{
if(liuxuan->CutList->Count==0)
{
  MessageBox(Application->Handle,"暂无截图","提醒-Wave截屏",MB_ICONASTERISK);
  return;
}
      Form8->show->Assign((Graphics::TBitmap*)CutList->Items[pImage]);
      Form8->Caption="截图预览-"+AnsiString(*((AnsiString*)DateList->Items[pImage]))+"-WaveSoft";
  /*    Form8->Image1->Width=((Graphics::TBitmap*)(CutList->Items[pImage]))->Width;
      Form8->Image1->Height=((Graphics::TBitmap*)(CutList->Items[pImage]))->Height;
      Form8->Image1->Top=0;
      Form8->Image1->Left=0;    */
      Form8->ShowModal();

}
//---------------------------------------------------------------------------
 void Tliuxuan::InstallFullWatch()
 {
   APPBARDATA abd;
memset(&abd, 0, sizeof(abd));
// Specify the structure size and handle to the appbar.
abd.cbSize = sizeof(APPBARDATA);
abd.hWnd =liuxuan->Handle;
abd.uCallbackMessage = MSG_APPBAR_MSGID;
!::SHAppBarMessage(ABM_NEW, &abd);
}
void __fastcall Tliuxuan::N11Click(TObject *Sender)
{
 try
 {
  int reint=int(ShellExecute(liuxuan->Handle, "Open","使用说明.doc", "", ExtractFilePath(Application->ExeName).c_str(), 1))      ;
  if(reint<=32)
 ShellExecute(liuxuan->Handle, "Open","使用说明.text", "", ExtractFilePath(Application->ExeName).c_str(), 1);

 }
 catch(...)
 {
  ShowMessage("说明文件已经损坏");
 }
}
//---------------------------------------------------------------------------





