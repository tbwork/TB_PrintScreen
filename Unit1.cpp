//---------------------------------------------------------------------------

#include <vcl.h>
#include<windows.h>
#pragma hdrstop

#include "Unit1.h"
#include "Main.h"
#include "math.h"
#include "shellapi.h"
#include "Unit3.h"
#include "Unit9.h"



//��ͷ ��ѧ��ʽ����
#define Len (long double)InvSqrt((long double)( (ex-sx)*(ex-sx)+(ey-sy)*(ey-sy) ))
#define K (ex!=sx ? (long double)(ey-sy)/(long double)(ex-sx):0)
#define K_1 (long double)((long double)(-1)/(long double)K)
#define Ax (long double)((long double)(ex)-(long double)((long double)(ex-sx)*(long double)(h)*(long double)(Len)  )  )
#define Ay (long double)((long double)(ey)- (long double)((long double)(ey-sy)*(long double)h*(long double)Len)  )
#define t_x (long double)( ((double)hb)*InvSqrt(   (long double)( (long double)(1.0)+(double)(K_1)*(double)(K_1)  ) ) )
#define t_y (long double)(K_1)*(long double)(t_x)
#define ArrowHigh 9
#define ArrowBottom  4

#define CHARWIDTH Form1->Canvas->TextWidth("2")
#define CHARHEIGHT Form1->Canvas->TextHeight("2")
//ֱ�߶� �е㳣���� �궨��
#define mx  (int((CutRect.left+CutRect.right)/2))
#define my  (int((CutRect.top+CutRect.bottom)/2))
//��峣�������� �궨��
#define MainPanelWidth 144
#define MainPanelHeight 173
#define ColorPanelWidth 273
#define ColorPanelHeight 80
#define StrToInt liuxuan->StrToInt_User
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;


/* ���� ����  ��ɾ��*/
int test=0;

//------------------������---------------------------
bool MouseIn[13]; //from 1
bool Mousedown[12];//from 0
TList*BmpList;
int InImage;//ָ��ǰmousein Ϊ���image  from 1  ��0��ʾû��in
int DownImage;//ָ��ǰdown ��image from 1 ��0��ʾû��down
TMemoryStream *ImageStream[40];//  0~35��36������ťͼ��36��39 ��4�����ڳߴ簴ťͼ
int MinPenSize;//�޶���ǰ���ߵ���С���ʿ�ȣ�1��
int MaxPenSize;//�޶���ǰ���ߵ���󻭱ʿ�ȣ�30��
int MinFontSize;//�޶��������ߴ磺8��
int MaxFontSize;//�޶��������ߴ磺24��
bool ColorTableMouseDownFLAG;
inline void movefun(int i)  ;//i :from 1 to 6

inline void downfun7_12(int i); //7 to 12 ���⴦��
inline void upfun7_12(int i);
inline void SetColorPanelPosition();
void Inlize(int X, int Y, int &dlx,int &dly);//���ڴ���ѵ�ǰ�������񻯵� �滭��������
void  DrawRectLine(TCanvas*canvas,int sx,int sy,int ex,int ey);

Graphics::TBitmap * bmp;



int CurrentMode;//��ǰ�Ļ滭ģʽ
TColor DrawColor;
int DrawWidth;
int TextSize;
int mouseX,mouseY;
int dlx,dly;
TColor TempPenColor;   //������ʱ�Ļ�����ɫ
TColor TempBrushColor;  //������ʱ�Ļ�����ɫ
int  TempWidth;            //������ʱ�Ļ��ʿ��
int  OperationCount; //��¼��Ч�����������������Ϊ0����ô�ǿ��Լ����϶��ġ� //�ڽ���༭״̬���˳�ʱ�Ƿ������϶���һ�㣬���ʲô��û������ô�������϶��ģ���ʼ��Ϊtrue
bool FuncButtonDown;//�ж� �л�ͼ���ܼ�����ʱ��������ͼ
bool MouseHasMove;//��������갴�º��Ƿ��ƶ���

bool FormQuickKey;//���˼��Ƿ��ǿ�ݼ����µģ�����ǣ���ô����Ҫ�ı���˼���ͼ�ꡣ


//text editing
int memosx,memosy;//
bool IsMemoMoving;//�Ƿ����ƶ�memo
int memolength;   
int memolinecount;

/*����memo ������ַ���*/
int BaseWidth;//�����ַ����
int MaxCharPerLine;//һ���������Ŷ��ٸ��ַ�
int MaxLineCount;//�������
int CurLine;
int CharCount;//��ǰ�����ַ���,�����������ַ�
int truecount;//Memo����������
int maxcharline;//��ȡ��ǰ�ı���������е�������
int MaxCharInLine; //��¼��ǰ������ַ����е��ַ�����
bool MaxWidth;
bool firstmove;//�ж�Memo���Ƿ��ǵ�һ���ƶ�������ǣ���ôҪ������֮ǰ������� ��UpdateWindow������
bool Exceptable;//�����жϣ����Ȱ���text��ť��Ȼ���ٰ�������ťʱ��Ӧ��ҲҪʹText�ı���UserPaint�������ܡ�����ʱ�滭ģʽ����text���������øñ�������Ϊ������־

int  movenum;//���ڿ���panel���ƶ�


//------------------Form Main-------------------------
bool drawable;//������������ͼ
bool dragable;//���������϶���ͼ
bool directable;//�����ͼ������ֱ�ƿ��ͼ��ˮƽ�����ƿ��ͼ
bool oncuting;//��ͼ�����Ѿ������ˣ����ڴ����������
bool Triming;//�Ƿ��� ���ٱ༭��ͼ  �У���ĳ����ͼ��ť���º��Ҫ��Ϊtrue��
bool ondrawing;//�Ƿ��� ��ʽ��ͼ�� �У���Mousedown����Ϊtrue
bool InfoVisible;

bool IsDlk;//���ڽ��˫�����µ�bug


Graphics::TBitmap * Srcbmp;      //ԭͼ
Graphics::TBitmap * Graybmp;     //��ɫͼ
Graphics::TBitmap * GrayGraybmp; //���
Graphics::TBitmap * Grayerbmp;   //����
Graphics::TBitmap * BufferBmp;  // ����canvas���ڴ滺��  ***��Ҫ����
Graphics::TBitmap * BufferBmp2; //���ڻ�Memo�ı߽磻ͬʱ��Ӱ��BufferBmp
Graphics::TBitmap * InfoBmp;//���ڸ���Info ��Rgb��Ϣʱʹ�ã�Ϊ�˽�ʡ��Դ
Graphics::TBitmap * DrawBmp;//���ڻ�ͼʱ�Ļ���Bmp      ��CutRect����Ч
Graphics::TBitmap * LastDrawBmp;//���ڱ�����һ�λ滭��״̬   ��CutRect����Ч
Graphics::TBitmap * RecordBmp[5];//��¼��һ���Ĳ�����    0~4      ������� �ݼ�����
//Graphics::TBitmap * tempbmp;


int RecordCount;//��Ч��¼���� ��ʼ��Ϊ 0      ��
int RecordPoint;//ָ�����ӵļ�¼����ʼ��Ϊ -1  ��




TRect CutRect;//��ǰ�Ľ�������
int sx,sy,lx,ly;
int mpx;
int mpy;// mpx mpy ΪMainPanel��ǰһ��λ��
int cpx,cpy;// cpx cpy ΪColorPanel�ļ������ֵ�λ��
HCURSOR TBSizeNWSE;
HCURSOR TBSizeNESW;
HCURSOR TB_WAVE;
HCURSOR TB_DRAW;
int MaxRight;   //��Ļ��
int MaxBottom;   //��Ļ��
int HalfHandleSize;
bool HaveBeenMove;//�ж���갴�º��Ƿ��˶���
bool fillable;

//-------�м���������ڿ�ʼ���壬���Ч��-----------
int rsx,rsy,rex,rey,InfoSy,InfoSx; //InfoSxΪ��Ϣ�����ʼX����,InfoSyΪ��Ϣ�����ʼY����
int wave[8][2];//���˵���״




//----------------User Defined Routine---------------------------------
int CheckRGB(int a)
{ return a>255?255:a<0?0:a;}
//---------------------------------------------
TBRGB Transfer2(TBRGB rgb, float value)
{
        rgb.r=CheckRGB(rgb.r*value);
        rgb.g=CheckRGB(rgb.g*value);
        rgb.b=CheckRGB(rgb.b*value);
        return rgb;
}
//-----------------------------------------------
TColor Transfer(TColor rgb, float value)
{
        rgb=RGB(CheckRGB(value*GetRValue(ColorToRGB(rgb))),CheckRGB(value*GetGValue(ColorToRGB(rgb))),CheckRGB(value*GetBValue(ColorToRGB(rgb))));
        return rgb;
}
//-------------------------------------------------------
AnsiString GetPathName(AnsiString a)
{
  AnsiString b="";
  for(int i=0;i<a.Length();i++)
  {
   if(*(a.c_str()+i)=='.')
   {
    break;
   }
   else
   {
    b+=AnsiString(*(a.c_str()+i));
   }
  }
}

//----------------------ţ�ٵ�����-----------------------------
double   InvSqrt(double   number)
{ 
        __int64   i; 
        double   x2,   y; 
        const   double   threehalfs   =   1.5F;

        x2   =   number   *   0.5F;
        y     =   number; 
        i     =   *   (   __int64   *   )   &y; 
        i     =   0x5fe6ec85e7de30da   -   (   i   >>   1   );
        y     =   *   (   double   *   )   &i; 
        y     =   y   *   (   threehalfs   -   (   x2   *   y   *   y   )   );   //   1st   iteration 
        y     =   y   *   (   threehalfs   -   (   x2   *   y   *   y   )   );   //   2nd   iteration,   this   can   be   removed
        return   y; 
}

bool IsInCutRect(int lx,int ly,int rx,int ry)
{

     if(!(rx<CutRect.left||ry<CutRect.top||lx>CutRect.right||ly>CutRect.bottom))
         return true;
     else
     return false;
}
//-------------------End of User Defined Routine------------------------------

//---------------------------------------------------------------------------
 AnsiString CurPath;


__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
CurPath=ExtractFilePath(Application->ExeName);
movenum=0;   //����mousemove
ImageList1->BkColor=RGB(232,253,251);

//--------------------Form1-------------
Form1->Top=0;
Form1->Left=0;
Form1->Width=Screen->Width;
Form1->Height=Screen->Height;
RectLineColor=RGB(20,120,255);
Transparency=0.6f;
LowerTransparency=0.7f;
LowestTransparency=0.8f;

//---------------------��Ϣ��ʾ��-------------
InfoRectWidth=160;
InfoRectHeight=70;
InfoRectTopWidth=10;
InfoTextColor=RGB(255,255,255);
InfoRectHidden=false;


Srcbmp= new Graphics::TBitmap();



//-----------------------------Graybmp-------------------------------
Graybmp=new Graphics::TBitmap();

//----------------------------Grayerbmp---------------------------------
Grayerbmp=new Graphics::TBitmap();

//-----------------------------GrayGraybmp-------------------------------
GrayGraybmp=new Graphics::TBitmap();


//-------------------------------------Buffer Bmp------------------------------------
BufferBmp=new Graphics::TBitmap();  //���Ǽ�¼��Ļ����һ��״̬



//------------------------------------------BufferBmp2---------------------------------
BufferBmp2=new Graphics::TBitmap();
BufferBmp2->Width=Form1->Width;
BufferBmp2->Height=Form1->Height;


//---------------------------------InfoBmp--------------------------
InfoBmp=new Graphics::TBitmap();
InfoBmp->Width=InfoRectWidth;
InfoBmp->Height=InfoRectHeight;


//----------------------------------DrawBmp------------------------
DrawBmp=new Graphics::TBitmap();
DrawBmp->Width=Form1->Width;
DrawBmp->Height=Form1->Height;


//-------------------------------
memolinecount=0;  //��ʼ��Ϊ0��
MaxWidth=false;

//------------------------------------Srcbmp---------------------------------
Srcbmp->Width=Form1->Width;
Srcbmp->Height=Form1->Height;


//---------------------------------LastDrawBmp--------------------------------------
LastDrawBmp=new Graphics::TBitmap();
LastDrawBmp->Width=Form1->Width;
LastDrawBmp->Height=Form1->Height;


// -------------------------------RecordBmp[5]-----------------------------------
for(int i=0;i<5;i++)
{
 RecordBmp[i]=new Graphics::TBitmap();
 RecordBmp[i]->Width=Form1->Width;
 RecordBmp[i]->Height=Form1->Height;
 //RecordBmp[i]->Canvas->StretchDraw(Rect(0,0,Width,Height),bmp);
}
 RecordCount=0;
 RecordPoint=-1;

//--------------------------
//-----clear flags---------------
drawable=false;
oncuting=false;
dragable=false;
directable=false;
CanDrawZeroRect=true;
CanToolBarVisible=true;
lx=ly=-1;
HaveBeenMove=false;
Triming=false;
InfoVisible=false;
fillable=false;//Ĭ�ϲ����
FuncButtonDown=false;
Exceptable=false;

IsMemoMoving=false;//

//�滭ģʽ ����
CurrentMode=-1;//-1 Ϊ ���� ��ͼ��
DrawColor=clRed;
dlx=-1;
dly=-1;
OperationCount=0;//��ʼû�л�ͼ��������ʼ��Ϊ0
MinPenSize=1;
MaxPenSize=35;
MinFontSize=8;
MaxFontSize=32;
DrawWidth=MinPenSize;
TextSize=MinFontSize;
MouseHasMove=false;

//CompressionValue=100;//��ʼ��jpgѹ������
//------------------Cursor------
TBSizeNWSE=LoadCursor(0,IDC_SIZENWSE);
TBSizeNESW=LoadCursor(0,IDC_SIZENESW);

TB_WAVE=LoadCursorFromFile((CurPath+"wave").c_str());
TB_DRAW=LoadCursorFromFile((CurPath+"draw").c_str());
//-----------------------------------------
MaxRight=Form1->Width;
MaxBottom=Form1->Height;
IsDlk=false;

//tempbmp=new Graphics::TBitmap();
//tempbmp->PixelFormat=6;

//---------------------------------------
Memo1->Font->Name="������";
Memo1->Font->Color=DrawColor;//Ԥ��������ɫ ��ɫ
Memo1->Font->Size=TextSize;
Form1->Canvas->Font->Name="������";
Form1->Canvas->Font->Color=clBlack;//Ԥ��������ɫ ��ɫ
Form1->Canvas->Font->Size=TextSize;
Form1->Canvas->Font=Memo1->Font;
Memo1->Width=Form1->Canvas->TextWidth("2")*5;
Memo1->Height=0-Form1->Canvas->Font->Height;
SizeEdit->Text=IntToStr(TextSize);

Form1->Canvas->Font=Memo1->Font;
Form1->Canvas->Font->Size=Memo1->Font->Size;
Form1->Font=Memo1->Font;
BaseWidth=Canvas->TextWidth("2");
MaxCharPerLine=int(Memo1->Width/BaseWidth);
MaxLineCount=int(Memo1->Height/(-Memo1->Font->Height));
CharCount=Memo1->Text.Length();
truecount=Memo1->Lines->Count;
MaxCharInLine=0;

firstmove=true;

//--------wave point-------------------

wave[0][0]=3;wave[0][1]=8;
wave[1][0]=4;wave[1][1]=7;
wave[2][0]=5;wave[2][1]=7;
wave[3][0]=7;wave[3][1]=9;
wave[4][0]=9;wave[4][1]=9;
wave[5][0]=11;wave[5][1]=7;
wave[6][0]=12;wave[6][1]=7;
wave[7][0]=14;wave[7][1]=9;

PanelInitial();//����ʼ��


bmp=new Graphics::TBitmap();

bmp->Width=Width;
bmp->Height=Height;
//bmp->LoadFromFile("1.bmp") ;
  bmp->PixelFormat=6;
//float Transparency;  //�Ҷ�ͼ �Ҷȳ�ʼֵ 0.6
//float LowerTransparency;//��Ϣ�򱳾� ͸����   0.7
//float LowestTransparency;//�Ҷ���Ϣ�򱳾� ͸����       0.8
for(int ii=0;ii<256;ii++)
{
 _Mask[ii]= CheckRGB(int(ii*Transparency));
 _Masker[ii]= CheckRGB(int(ii*Transparency*LowerTransparency));
 _Maskest[ii]= CheckRGB(int(ii*Transparency*LowerTransparency*LowestTransparency));
}
 ShowMainFormAfterCut=true;
}
//--------------------------------------------------------------------------
__fastcall TForm1::~TForm1() //��Դ�ͷ�
{
BmpList->Free();
//delete  tempbmp; //�ͷ�
delete [5] RecordBmp;//��¼��һ���Ĳ�����    0~4      ������� �ݼ�����
delete LastDrawBmp;//���ڱ�����һ�λ滭��״̬   ��CutRect����Ч
delete DrawBmp;//���ڻ�ͼʱ�Ļ���Bmp      ��CutRect����Ч
delete InfoBmp;//���ڸ���Info ��Rgb��Ϣʱʹ�ã�Ϊ�˽�ʡ��Դ

delete BufferBmp;  // ����canvas���ڴ滺��  ***��Ҫ����
delete BufferBmp2; //���ڻ�Memo�ı߽磻ͬʱ��Ӱ��BufferBmp

delete Graybmp;     //��ɫͼ
delete GrayGraybmp; //���
delete Grayerbmp;   //����
delete bmp;
delete Srcbmp;      //ԭͼ
}
//---------------------------------------------------------------------------


void   __fastcall   TForm1::WndProc(TMessage&   Msg) //�Զ�����Ϣ����
{

       if(Msg.Msg==WM_ERASEBKGND)
       {
            return;
       }
       else if(Msg.Msg==WM_SHOWWINDOW)
       {
          if(Msg.WParam==-1&&Msg.LParam==-1)
          {
            Show();
            return;
          }
       }
       else if(Msg.Msg==WM_PAINT)
       {
         if(dragable)
         {
            return;
         }
         if(IsMemoMoving)
            return;
         if(!Memo1->Visible)
            UserFormPaint();

       }
       else if(Msg.Msg==WM_MOUSEMOVE)
       {
          if(CurrentMode==Text_5)
          {
            if(TextSize<MinFontSize)
            {
               SizeEdit->Text=IntToStr(MinFontSize);
               TextSize=MinFontSize;        //8Ϊ��С����
            }
          }
          mouseX=Msg.LParamLo;
          mouseY=Msg.LParamHi;
         if(Triming)
         {     //���ڻ�ͼ   ���Ϊ��ͼ���
            TrimCutRect(Msg.LParamLo, Msg.LParamHi);
         }
         else if(OperationCount>0)//�Ѿ��༭������Ч������>0
         {
            Form1->Cursor=crDefault;
         }
         else
         {      //����
            CreateCutRect(Msg.LParamLo, Msg.LParamHi);
         }
         //����Ĵ����Image�ؼ� MM�� mousemove��һ���ģ�Ϊ�˷�ֹ���Ϲ�Panel��ʱ���������״̬�İ�ť
         if(InImage!=DownImage&&InImage!=0)
            {
                 ((TImage*)(BmpList->Items[InImage-1]))->Picture->Bitmap->LoadFromStream(ImageStream[(InImage-1)*3+1]);
                   ImageStream[(InImage-1)*3+1]->Position=0;
                   InImage=0;
            }

             return;
       }
        TForm::WndProc(Msg); //������������Ĭ�Ϻ�����

}
//----------------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TForm1::FormPaint(TObject *Sender)
{
//��һ��Ĭ�ϴ������滭����  �������ڻ滭������·�ˢ�´���
  BitBlt(Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
}
//---------------------------------------------------------------------------
void TForm1::DrawTracker(TCanvas*canvas,int sx,int sy,int ex,int ey)
{
 TColor tc=canvas->Pen->Color;
 int pw=canvas->Pen->Width;
 canvas->Pen->Color=RectLineColor;
 canvas->Brush->Color=RectLineColor;
 canvas->Pen->Width=1;
 int midx=int((sx+ex)/2);
 int midy=int((sy+ey)/2);
 if(HalfHandleSize==0)     //�˸���
 {
  canvas->MoveTo(sx,sy);
  canvas->LineTo(ex,sy);
  canvas->MoveTo(ex,sy);
  canvas->LineTo(ex,ey);
  canvas->MoveTo(ex,ey);
  canvas->LineTo(sx,ey);
  canvas->MoveTo(sx,ey);
  canvas->LineTo(sx,sy);
 }
 else
 {
  canvas->Rectangle(sx-HalfHandleSize,sy-HalfHandleSize,sx+HalfHandleSize,sy+HalfHandleSize);  //����
  canvas->Rectangle(midx-HalfHandleSize,sy-HalfHandleSize,midx+HalfHandleSize,sy+HalfHandleSize);  //����
  canvas->MoveTo(sx,sy);
  canvas->LineTo(ex,sy);
  canvas->Rectangle(ex-HalfHandleSize,sy-HalfHandleSize,ex+HalfHandleSize,sy+HalfHandleSize);  //����
  canvas->Rectangle(ex-HalfHandleSize,midy-HalfHandleSize,ex+HalfHandleSize,midy+HalfHandleSize);  //����
  canvas->MoveTo(ex,sy);
  canvas->LineTo(ex,ey);
  canvas->Rectangle(ex-HalfHandleSize,ey-HalfHandleSize,ex+HalfHandleSize,ey+HalfHandleSize);  //����
  canvas->Rectangle(midx-HalfHandleSize,ey-HalfHandleSize,midx+HalfHandleSize,ey+HalfHandleSize);  //����
  canvas->MoveTo(ex,ey);
  canvas->LineTo(sx,ey);
  canvas->Rectangle(sx-HalfHandleSize,ey-HalfHandleSize,sx+HalfHandleSize,ey+HalfHandleSize);  //����
  canvas->Rectangle(sx-HalfHandleSize,midy-HalfHandleSize,sx+HalfHandleSize,midy+HalfHandleSize);  //����
  canvas->MoveTo(sx,ey);
  canvas->LineTo(sx,sy);
  }
  canvas->Pen->Color=tc;
  canvas->Brush->Color=tc;
  canvas->Pen->Width=pw;

}
//--------------------------------------------------------------------------------
void TForm1::DrawInfo(TCanvas*canvas,int sx,int sy,int wt,int ht,int RectSx,int RectSy,int MouseX,int MouseY)
{
  UINT8 r,g,b;
  if(OperationCount==0)
  {
   r=GetRValue(Srcbmp->Canvas->Pixels[MouseX][MouseY]);
   g=GetGValue(Srcbmp->Canvas->Pixels[MouseX][MouseY]);
   b=GetBValue(Srcbmp->Canvas->Pixels[MouseX][MouseY]);
  }
  else
  {
   r=GetRValue(LastDrawBmp->Canvas->Pixels[MouseX][MouseY]);
   g=GetGValue(LastDrawBmp->Canvas->Pixels[MouseX][MouseY]);
   b=GetBValue(LastDrawBmp->Canvas->Pixels[MouseX][MouseY]);

  }
  canvas->TextOutA(sx+4,sy+4,"��ͼ����λ��:  "+IntToStr(RectSx)+"*"+IntToStr(RectSy));
  canvas->TextOutA(sx+4,sy+17,"��ͼ�����С:  "+IntToStr(wt)+"*"+IntToStr(ht));
  canvas->TextOutA(sx+4,sy+32,"����RGBֵ:  ("+IntToStr(r)+","+IntToStr(g)+","+IntToStr(b)+")");
  canvas->TextOutA(sx+25,sy+55,"˫������������ɽ�ͼ��");

  //wave ��־
  canvas->Pen->Color=InfoTextColor;
  canvas->MoveTo(wave[0][0]+sx+4,wave[0][1]+sy+InfoRectHeight-18);
    //����wave
  for(int i=1;i<8;i++)
  {
    canvas->LineTo(wave[i][0]+sx+4,wave[i][1]+sy+InfoRectHeight-18);
  }
  //����
  canvas->MoveTo(sx+4,sy+InfoRectHeight-18);
  canvas->LineTo(sx+20,sy+InfoRectHeight-18);
  canvas->LineTo(sx+20,sy+InfoRectHeight-2);
  canvas->LineTo(sx+4,sy+InfoRectHeight-2) ;
  canvas->LineTo(sx+4,sy+InfoRectHeight-18);
  canvas->MoveTo(sx+8,sy+12);
  //�ָ�����

  canvas->Pen->Color=RectLineColor;
  InfoVisible=true;
}
//===================================================================================
void TForm1::Graylize(Graphics::TBitmap*tbmp,int a)//������
{
  BYTE *ptr=(BYTE*)tbmp->ScanLine[tbmp->Height-1];
  for(int y=0;y<3*tbmp->Height*tbmp->Width;y++)
  {
   if(a==1)
    ptr[y]=_Mask[ptr[y]];     //*degree
   else if(a==2)
    ptr[y]=_Masker[ptr[y]];
   else
    ptr[y]=_Maskest[ptr[y]];
  }
   /*TBRGB*ptr=(TBRGB*)tbmp->ScanLine[tbmp->Height-1];
  /* for(int y=0;y<tbmp->Height;y+=1)
   {
     ptr=(TBRGB*)tbmp->ScanLine[y];
     for(int x=0;x<tbmp->Width;x++)
      ptr[x]=Transfer2(ptr[x],degree);
   }  */
  /* for(int y=0;y<tbmp->Height*tbmp->Width;y++)
   {
      ptr[y]=Transfer2(ptr[y],degree);
   }    */
}

//---------------------------------------------------------------------------------
 void __fastcall TForm1::FormMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
if(IsDlk)
{
 IsDlk=false;
 return;
}
 mouseX=X;
 mouseY=Y;
 if(Button==mbLeft)//�������
 {
  if(Triming)//���ٻ�ͼ�༭  ����
  {
     if(FuncButtonDown)
     {
       if(X>= CutRect.left&&Y>=CutRect.top&&X<=CutRect.right&&Y<=CutRect.bottom)
       {

         if(CurrentMode!=Text_5)
         {    // ������ģʽ


              if(IsInCutRect(ColorPanel->Left,ColorPanel->Top,ColorPanel->Left+ColorPanel->Width,ColorPanel->Top+ColorPanel->Height))
              {
                 ColorPanel->Hide();
              }
              if(IsInCutRect(MainPanel->Left,MainPanel->Top,MainPanel->Left+MainPanel->Width,MainPanel->Top+MainPanel->Height))
              {
                 MainPanel->Hide();
              }

           dlx=X;
           dly=Y;
           ::SetCursor(TB_DRAW);
           DrawBmp->Canvas->Pen->Width=DrawWidth ;
           DrawBmp->Canvas->Pen->Color=DrawColor;
           DrawBmp->Canvas->Brush->Color=DrawColor;
           if(CurrentMode==PenBrush_0)
           BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
           ondrawing=true;
         }
         else
         {
           if(ondrawing)
           {
             if(!ColorPanel->Visible)
                ColorPanel->Show();
             if(!MainPanel->Visible)
                MainPanel->Show();
                BitBlt(Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
                Memo1->Hide();
                ondrawing=false;
           }
           else
           {
              if(IsInCutRect(ColorPanel->Left,ColorPanel->Top,ColorPanel->Left+ColorPanel->Width,ColorPanel->Top+ColorPanel->Height))
              {
                 ColorPanel->Hide();
              }
              if(IsInCutRect(MainPanel->Left,MainPanel->Top,MainPanel->Left+MainPanel->Width,MainPanel->Top+MainPanel->Height))
              {
                 MainPanel->Hide();
              }
              if(X>CutRect.left&&X<CutRect.right&&Y>CutRect.top&&Y<CutRect.bottom)
              {
                 Form1->Canvas->Font=Memo1->Font;
                 Form1->Canvas->Font->Color=DrawColor;//Ԥ��������ɫ ��ɫ
                 Form1->Canvas->Font->Size=TextSize;
                 Form1->Canvas->Font=Memo1->Font;
                 Memo1->Width=CHARWIDTH*5;
                 MaxCharInLine=0;
                 Memo1->Height=CHARHEIGHT;
                 Memo1->Clear();
                 Memo2->Font=Memo1->Font;
                 Memo2->Font->Size=Memo1->Font->Size;
                 Memo2->Width=CutRect.right-Memo1->Left-1;
                 memolinecount=0;
               //  if(CutRect.Width()<=Memo1->Width||CutRect.Height()<=Memo1->Height)
               //  {
                    if(CutRect.Width()-2<=Memo1->Width)
                    {
                         Memo1->Width=CutRect.Width()-2;
                    }
                    if(CutRect.Height()-2<=Memo1->Height)
                    {
                        Memo1->Height=CutRect.Height()-2;
                    }

                // }
               //  else
               //  {
                   if(X>=(CutRect.right-Memo1->Width)&&Y>=(CutRect.bottom-Memo1->Height))
                    {
                     Memo1->Left=CutRect.right-Memo1->Width-1;
                     Memo1->Top=CutRect.bottom-Memo1->Height-1;
                    }
                   else if(X>=(CutRect.right-Memo1->Width))
                    {
                      Memo1->Left=CutRect.right-Memo1->Width-1;
                      Memo1->Top=Y;
                    }
                   else if(Y>=(CutRect.bottom-Memo1->Height))
                    {
                      Memo1->Left=X;
                      Memo1->Top=CutRect.bottom-Memo1->Height-1;
                    }
                   else
                    {
                      Memo1->Left=X;
                      Memo1->Top=Y;
                    }
                 // }
                 Memo1->Show();
                 ondrawing=true;
                 Memo1->SetFocus();
              }

           }
         }
       }
       else
       {
          ::SetCursor(TB_WAVE);
          if(CurrentMode==Text_5)
          {
           if(ondrawing)
           {
                BitBlt(Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
                Memo1->Hide();
                ondrawing=false;
           }
          }
       }
     }
     else
     {
        if(X>= CutRect.left&&Y>=CutRect.top&&X<=CutRect.right&&Y<=CutRect.bottom)
        {
           Form1->Cursor=crDefault; //����һ�������ʽ TB_FINISH
        }
        else
        {
           ::SetCursor(TB_WAVE);
        }
     }
  }
  else if(OperationCount>0)//�Ѿ��༭����������Ч������>0
  {
     Form1->Cursor=crDefault;
  }
  else     //not triming
  {

   HaveBeenMove=false;
   if(!oncuting) //���ٽ�ͼ�����У����Դ����µĽ�ͼ����
   {
    ::SetCursor(TB_WAVE);
    drawable=true;    //��ʼ������������
    sx=X;sy=Y;
   }
   else     //�Ѿ��ڽ�ͼ���̣��������µĽ�ͼ���򲻻ᱻ����
   {
     if(Form1->Cursor==crSizeAll)  //���м�����
     {
       dragable=true;
       sx=X;sy=Y;//��¼������
     }
     else if(Form1->Cursor==crSizeNESW)//�����º����� ��������
     {
       drawable=true;//�ĸ������������������ʵҲ��һ�������ͼ������������drawable��־  ,���޸���ʼ�������
       if(X>=CutRect.right-HalfHandleSize&&Y<=CutRect.top+HalfHandleSize)//������
       {
        sx=CutRect.left;sy=CutRect.bottom;//������Ϊ����
       }
       else if(X<=CutRect.left+HalfHandleSize&&Y>=CutRect.bottom-HalfHandleSize)//������
       {
        sx=CutRect.right;sy=CutRect.top;//������Ϊ����
       }
     }
     else if(Form1->Cursor==crSizeNWSE) //�����Ϻ����� ��������
     {
       drawable=true;//�ĸ������������������ʵҲ��һ�������ͼ������������drawable��־
       if(X<=CutRect.left+HalfHandleSize&&Y<=CutRect.top+HalfHandleSize)//������
       {
        sx=CutRect.right;sy=CutRect.bottom;//������Ϊ����
       }
       else if(X>=CutRect.right-HalfHandleSize&&Y>=CutRect.bottom-HalfHandleSize)//������
       {
         sx=CutRect.left;sy=CutRect.top;//������Ϊ����
       }
     }
     else if(Form1->Cursor==crSizeNS)    //������ ��ֱ�ƶ�
     {
       directable=true; //����ˮƽ������ֱ�Ͽ��ͼ
       if(Y<=CutRect.top+HalfHandleSize)//���� ����  ;��¼���½ǵ�
       {
         sx=CutRect.right;
         sy=CutRect.bottom;
         lx=-2;//�����lx��ʾһ����־��Ϊ-2ʱ��ʾ������������ʼλ��Ϊ�ѵ��������ˮƽ�ߡ�
       }
       else if(Y>=CutRect.bottom-HalfHandleSize) //���º���    ����¼���Ͻǵ�
       {
         sx=CutRect.left;
         sy=CutRect.top;
         lx=-3;//�����lx��ʾһ����־��Ϊ-3ʱ��ʾ������������ʼλ��Ϊ�ѵ��������ˮƽ�ߡ�
       }
     }
     else if(Form1->Cursor==crSizeWE)   //�����Һ����ƶ�
     {
       directable=true; //����ˮƽ������ֱ�Ͽ��ͼ
       if(X<=CutRect.left+HalfHandleSize) // ������ֱ����   ����¼���½ǵ㣻
       {
         sx=CutRect.right;
         sy=CutRect.bottom;

         lx=-2; //�����lx��ʾһ����־��Ϊ-2ʱ��ʾ������������ʼλ��Ϊ�ѵ����������ֱ�ߡ�
       }
       else if(X>=CutRect.right-HalfHandleSize)//������ֱ����  ����¼���Ͻǵ㣻
       {
         sx=CutRect.left;
         sy=CutRect.top;
         lx=-3; //�����lx��ʾһ����־��Ϊ-2ʱ��ʾ������������ʼλ��Ϊ�ѵ����������ֱ�ߡ�
       }
     }
     else //����Ĭ�����
     {
        ::SetCursor(TB_WAVE);//������ͨ��������账��
     }
   }
  }
 }
 else//�Ҽ�����
 {
  if(Triming)//���ٱ༭  ����
  {
    if(X>= CutRect.left&&Y>=CutRect.top&&X<=CutRect.right&&Y<=CutRect.bottom) //�ڱ༭����
    {
    // ondrawing=true;
    }
  }
  else
   ::SetCursor(TB_WAVE);

 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{

// �޽�ͼ����Ҳ���ٴ�����ͼ����Ĺ����У���ôΪ����ͨ�������ƶ������������⴦��
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
 if(Button==mbLeft)//�������
 {
  if(Triming)
  {
       ::SetCursor(TB_DRAW);

     if(CurrentMode!=Text_5)
      {
       if(!ColorPanel->Visible)
          ColorPanel->Show();
       if(!MainPanel->Visible)
          MainPanel->Show();
       if(ondrawing)
       {
        ondrawing=false;
        if(MouseHasMove)
        {
           if(X!=dlx||Y!=dly||CurrentMode==PenBrush_0)
            {

               //��¼������ǰ״̬ ����LastDrawBmp
                BitBlt(LastDrawBmp->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),DrawBmp->Canvas->Handle, CutRect.left,CutRect.top,SRCCOPY);
               //��¼�洢
                RecordPoint=(RecordPoint+1)%5;
                BitBlt(RecordBmp[RecordPoint]->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),DrawBmp->Canvas->Handle, CutRect.left,CutRect.top,SRCCOPY);
                RecordCount=(RecordCount+1>5)?5:(RecordCount+1);
                OperationCount++; //�����һ������
            }
           MouseHasMove=false;
        }

       }
      }
    
      //  Canvas->Pen->Width=TempWidth ;
      //  Canvas->Pen->Color=TempPenColor;
      // Canvas->Brush->Color=TempBrushColor;
  }
  else
  {
       CreateCutRectMouseLeftUp(X,Y);
       //��ӳ�ʼ�� ����
       BitBlt(RecordBmp[0]->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),Srcbmp->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
      // DrawTracker(RecordBmp[0]->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);
       RecordPoint=0;
       RecordCount=1;
  }
 }
 else
 { //�һ�
  if(oncuting&&X>= CutRect.left&&Y>=CutRect.top&&X<=CutRect.right&&Y<=CutRect.bottom)
  {   //�����CutRect�� ������popupmenu
    Poper->Popup(X,Y);
    return;
  }
   if(oncuting)//�һ����ָ��Ҷ�ȫͼ
   {
   BitBlt(LastDrawBmp->Canvas->Handle,0,0,Form1->Width,Form1->Height,Srcbmp->Canvas->Handle,0,0,SRCCOPY);
   //��� PanelͼƬ��record ��������Ŀ��
   if(DownImage>0)
   {
    ((TImage*)(BmpList->Items[DownImage-1]))->Picture->Bitmap->LoadFromStream(ImageStream[3*(DownImage-1)+1]);
    ImageStream[3*(DownImage-1)+1]->Position=0;
   }
   DownImage=0; //��DownImage
   InImage=0;   //��InImage
   FuncButtonDown=false; //û�й��ܰ�ť��
   Triming=false;  //���ڱ༭
   OperationCount=0;    //��������0
   RecordPoint=-1; //��¼ָ��ָ��-1��
   RecordCount=0; //��¼��0
   ondrawing=false; //�Ƿ����ڻ�ͼ����д����
   oncuting=false;//�ָ��޽�ͼ����״̬  ��ȥ���༭״̬
   Form1->MainPanel->Hide();
   Form1->ColorPanel->Hide();
   Form1->Memo1->Clear();
   Form1->Memo1->Hide();
   UpdateWindow(Form1->Handle);
   ::SetCursor(TB_WAVE);
   BitBlt(Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);
   DrawBmp->Canvas->StretchDraw(Rect(0,0,Grayerbmp->Width,Grayerbmp->Height),Srcbmp);//ȥ���滭
  }
  else
  {
    Hide();//�����մ����Hide����
  }
 }
}
//---------------------------------------------------------------------------
TRect TForm1::GetCurRect(int ssx,int ssy,int eex,int eey)
{
  if(ssx!=eex && ssy!=eey)//�����߻򹲵�
  {
    int lx,ly,bx,by;//Сx��Сy����x����y��һ�����ε������������Ȼ�� ��Сx��Сy��������x����y��
    if(ssx>eex)
    {
      lx=eex;
      bx=ssx;
    }
    else
    {
      lx=ssx;
      bx=eex;
    }
    if(ssy<eey)
    {
     ly=ssy;
     by=eey;
    }
    else
    {
     ly=eey;
     by=ssy;
    }
    return Rect(lx,ly,bx,by);
  }
  else
  if(ssx==eex&&ssy==eey)//ˮƽ�ߺ���ֱ�߶� �غ�
  {
     return Rect(ssx,ssy,ssx,ssy);
  }
  else if(ssy==eey)//ˮƽ���غ�
  {
    return Rect(ssx>eex?eex:ssx,ssy,ssx>eex?ssx:eex,ssy);
  }
  else if(ssx==eex)//��ֱ���غ�ssx=eex
  {
    return Rect(ssx,ssy>eey?eey:ssy,ssx,ssy>eey?ssy:eey);
  }
   return Rect(0,0,0,0);
}
//------------------------------------------------------------------------------
void TForm1::PanelInitial()
{
 HRGN hrgn,hrgnp;
 TPoint p[11];
 p[0].x=0;p[0].y=1;
 p[1].x=1;p[1].y=0;
 p[2].x=29;p[2].y=0;
 p[3].x=30;p[3].y=1;
 p[4].x=30;p[4].y=8;
 p[5].x=141;p[5].y=8;
 p[6].x=144;p[6].y=11;
 p[7].x=143;p[7].y=171;
 p[8].x=141;p[8].y=173;
 p[9].x=3;p[9].y=173;
 p[10].x=0;p[10].y=170;
 hrgn=::CreatePolygonRgn(p,11,ALTERNATE);
 SetWindowRgn(MainPanel->Handle,hrgn,true);
 TPoint pp[8];
 pp[0].x=0;pp[0].y=1;
 pp[1].x=1;pp[1].y=0;
 pp[2].x=272;pp[2].y=0;
 pp[3].x=273;pp[3].y=1;
 pp[4].x=273;pp[4].y=77;
 pp[5].x=270;pp[5].y=80;
 pp[6].x=3;pp[6].y=80;
 pp[7].x=0;pp[7].y=77;
 hrgnp=::CreatePolygonRgn(pp,8,ALTERNATE);
 SetWindowRgn(ColorPanel->Handle,hrgnp,true);

 for(int i=0;i<13;i++)
 {
  MouseIn[i]=false;
  if(i<12)
  Mousedown[i]=false;
 }

 BmpList=new TList();
 BmpList->Add(M1);
 BmpList->Add(M2);
 BmpList->Add(M3);
 BmpList->Add(M4);
 BmpList->Add(M5);
 BmpList->Add(M6);
 BmpList->Add(M7);
 BmpList->Add(M8);
 BmpList->Add(M9);
 BmpList->Add(M10);
 BmpList->Add(M11);
 BmpList->Add(M12);
 for(int i=0;i<40;i++)
 {
   ImageStream[i]=new TMemoryStream();

 }
 //image 1 :0~2  0:down  1: noth  2:up
 ImageStream[0]->LoadFromFile(CurPath+"tbsrcima\\PenDown.bmp");
 ImageStream[1]->LoadFromFile(CurPath+"tbsrcima\\PenNoth.bmp");
 ImageStream[2]->LoadFromFile(CurPath+"tbsrcima\\PenUp.bmp");
 //iamge 2: 3~5
 ImageStream[3]->LoadFromFile(CurPath+"tbsrcima\\ArrowDown.bmp");
 ImageStream[4]->LoadFromFile(CurPath+"tbsrcima\\ArrowNoth.bmp");
 ImageStream[5]->LoadFromFile(CurPath+"tbsrcima\\ArrowUp.bmp");

 //iamge 3: 6~8
 ImageStream[6]->LoadFromFile(CurPath+"tbsrcima\\RoundDown.bmp");
 ImageStream[7]->LoadFromFile(CurPath+"tbsrcima\\RoundNoth.bmp");
 ImageStream[8]->LoadFromFile(CurPath+"tbsrcima\\RoundUp.bmp");
 //iamge 4: 9~11
 ImageStream[9]->LoadFromFile(CurPath+"tbsrcima\\RectDown.bmp");
 ImageStream[10]->LoadFromFile(CurPath+"tbsrcima\\RectNoth.bmp");
 ImageStream[11]->LoadFromFile(CurPath+"tbsrcima\\RectUp.bmp");
 //iamge 5: 12~14
 ImageStream[12]->LoadFromFile(CurPath+"tbsrcima\\LineDown.bmp");
 ImageStream[13]->LoadFromFile(CurPath+"tbsrcima\\LineNoth.bmp");
 ImageStream[14]->LoadFromFile(CurPath+"tbsrcima\\LineUp.bmp");
 //iamge 6: 15~17
 ImageStream[15]->LoadFromFile(CurPath+"tbsrcima\\TDown.bmp");
 ImageStream[16]->LoadFromFile(CurPath+"tbsrcima\\TNoth.bmp");
 ImageStream[17]->LoadFromFile(CurPath+"tbsrcima\\TUp.bmp");
 //iamge 7: 18~20
 ImageStream[18]->LoadFromFile(CurPath+"tbsrcima\\BackDown.bmp");
 ImageStream[19]->LoadFromFile(CurPath+"tbsrcima\\BackNoth.bmp");
 ImageStream[20]->LoadFromFile(CurPath+"tbsrcima\\BackUp.bmp");
 //iamge 8: 21~23
 ImageStream[21]->LoadFromFile(CurPath+"tbsrcima\\SaveDown.bmp");
 ImageStream[22]->LoadFromFile(CurPath+"tbsrcima\\SaveNoth.bmp");
 ImageStream[23]->LoadFromFile(CurPath+"tbsrcima\\SaveUp.bmp");
 //iamge 9: 24~26
 ImageStream[24]->LoadFromFile(CurPath+"tbsrcima\\WaveDown.bmp");
 ImageStream[25]->LoadFromFile(CurPath+"tbsrcima\\WaveNoth.bmp");
 ImageStream[26]->LoadFromFile(CurPath+"tbsrcima\\WaveUp.bmp");
 //iamge 10: 27~29
 ImageStream[27]->LoadFromFile(CurPath+"tbsrcima\\QuitDown.bmp");
 ImageStream[28]->LoadFromFile(CurPath+"tbsrcima\\QuitNoth.bmp");
 ImageStream[29]->LoadFromFile(CurPath+"tbsrcima\\QuitUp.bmp");
 //iamge 11: 30~32
 ImageStream[30]->LoadFromFile(CurPath+"tbsrcima\\FinishDown.bmp");
 ImageStream[31]->LoadFromFile(CurPath+"tbsrcima\\FinishNoth.bmp");
 ImageStream[32]->LoadFromFile(CurPath+"tbsrcima\\FinishUp.bmp");
 //iamge 12: 33~35
 ImageStream[33]->LoadFromFile(CurPath+"tbsrcima\\WoodDown.bmp");
 ImageStream[34]->LoadFromFile(CurPath+"tbsrcima\\WoodNoth.bmp");
 ImageStream[35]->LoadFromFile(CurPath+"tbsrcima\\WoodUp.bmp");
 ImageStream[36]->LoadFromFile(CurPath+"tbsrcima\\RYY.bmp");
 ImageStream[37]->LoadFromFile(CurPath+"tbsrcima\\R.bmp");
 ImageStream[38]->LoadFromFile(CurPath+"tbsrcima\\LYY.bmp");
 ImageStream[39]->LoadFromFile(CurPath+"tbsrcima\\L.bmp");
 for(int i=0;i<12;i++)
 {
  ((TImage*)(BmpList->Items[i]))->Picture->Bitmap->LoadFromStream(ImageStream[3*i+1]);
 }

 LBut->Picture->Bitmap->LoadFromStream(ImageStream[38]);
 RBut->Picture->Bitmap->LoadFromStream(ImageStream[36]);
  for(int i=0;i<40;i++)
 {
   ImageStream[i]->Position=0;
 }


 InImage=0;
 DownImage=0;
 ColorTableMouseDownFLAG=false;
 ColorTable->Canvas->Brush->Color=DrawColor;
 ColorTable->Canvas->Rectangle(6,50,30,74);
 Fillable->Color=RGB(233,253,252);
}
void __fastcall TForm1::M1MouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    downfun(1);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::M1MouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
    movefun(1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::M2MouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
  movefun(2);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::M2MouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
  downfun(2);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::M3MouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
 movefun(3);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::M3MouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
  downfun(3);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::M4MouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
movefun(4);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::M4MouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
downfun(4);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::M5MouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
movefun(5);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::M5MouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
downfun(5);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::M6MouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
movefun(6);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::M6MouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
downfun(6);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::M7MouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
movefun(7);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::M7MouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
downfun7_12(7);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::M8MouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
movefun(8);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::M8MouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
downfun7_12(8);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::M9MouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
movefun(9);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::M9MouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
downfun7_12(9);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::M10MouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
movefun(10);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::M10MouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
downfun7_12(10);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::M11MouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
movefun(11);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::M11MouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
downfun7_12(11);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::M12MouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
  movefun(12);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::M12MouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
downfun7_12(12);
}
//---------------------------------------------------------------------------

inline void movefun(int i)  //i :from 1 to 12
{
  if(InImage==0)
   {
    if(DownImage!=i)
    {                        //����û�а���
     ((TImage*)(BmpList->Items[i-1]))->Picture->Bitmap->LoadFromStream(ImageStream[(i-1)*3+2]);  //up
     ImageStream[(i-1)*3+2]->Position=0;
     InImage=i;
    }
   }
   else     //InImage��=0
   {
      if(InImage!=i)    //���ڱ� iamge��
      {
        if(InImage!=DownImage)
        {
         //�����һ��image      ǰһ�������������ť���ǵ�ǰ����ť�� ��Ҳ���ǰ���״̬
          ((TImage*)(BmpList->Items[InImage-1]))->Picture->Bitmap->LoadFromStream(ImageStream[(InImage-1)*3+1]);
           ImageStream[(InImage-1)*3+1]->Position=0;
           if(DownImage!=i)
           {
             //�����Լ���image ���� up.bmp
              ((TImage*)(BmpList->Items[i-1]))->Picture->Bitmap->LoadFromStream(ImageStream[(i-1)*3+2]);
              ImageStream[(i-1)*3+2]->Position=0;
              InImage=i;
           }
       }
       else
       {
           if(DownImage!=i)
           {
             //���µ���Ȼ�������ڰ���״̬
              ((TImage*)(BmpList->Items[i-1]))->Picture->Bitmap->LoadFromStream(ImageStream[(i-1)*3+2]);
              ImageStream[(i-1)*3+2]->Position=0;
              InImage=i;
           }
       }
      }
   }

}
//----------------------------------------------------------------------------------------
inline void TForm1::downfun(int i)
{
 if(DownImage==0) //֮ǰû�а��µİ�ť
 {
    ((TImage*)(BmpList->Items[i-1]))->Picture->Bitmap->LoadFromStream(ImageStream[(i-1)*3]);  //down
    ImageStream[(i-1)*3]->Position=0;
    DownImage=i;
    if(i==1||i==2||i==5)//�ʺͻ�ˢ����ͷ��ֱ��
    {
     Form1->SizeInfo->Caption="���ʳߴ�:";
     Form1->Fillable->Hide();
    }
    else if(i==3||i==4)
    {
     Form1->SizeInfo->Caption="���ʳߴ�:";
     Form1->Fillable->Show();
    }
    else if(i==6)
    {
     Form1->SizeInfo->Caption="�����С:";
     Form1->Fillable->Hide();
    }
     if(CutRect.Top<(Form1->InfoRectHeight+Form1->InfoRectTopWidth))
     {
       //��Ϣ������
       Form1->InfoRectHidden=true;
       BitBlt(BufferBmp->Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);//ȫͼBitBlt
       BitBlt(BufferBmp->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
       Form1->DrawTracker(BufferBmp->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);
       BitBlt(Form1->Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
     }
     else
     {//������ ����
       Form1->InfoRectHidden=false;
     }
     SetColorPanelPosition();
     CurrentMode=i-1;
     if(CurrentMode==Text_5)
     {
       SizeEdit->Hint="��С����8,�������32";
       SizeEdit->Text=IntToStr(TextSize);
     }
     else
     {
       SizeEdit->Hint="��С�ʿ�1,���ʿ�35";
       SizeEdit->Text=IntToStr(DrawWidth);
     }
     Triming=true;
     FuncButtonDown=true;
     Form1->ColorPanel->Show();
 }
 else if(DownImage==i) //֮ǰ�����ť�Ѿ�������
 {
    ((TImage*)(BmpList->Items[i-1]))->Picture->Bitmap->LoadFromStream(ImageStream[(i-1)*3+2]);  //�ָ�
    ImageStream[(i-1)*3+2]->Position=0;
    DownImage=0;
    Form1->ColorPanel->Hide() ;
    if(InfoRectHidden)
    {
    //------�Ұ���ʾ�����---------------------
        rsx=CutRect.left;
        rsy=CutRect.top;
        rex=CutRect.right;
        rey=CutRect.bottom;
        if(rsy<(InfoRectHeight+InfoRectTopWidth))
        {
           if((rex-rsx)<InfoRectWidth &&(rey-rsy)<(InfoRectHeight+InfoRectTopWidth))//��Ϣ��ʾ�����ڲ�---���������������Ƚϸ��� ������ʾ��Ϣ�� ������ʱ�Ļ�ɫ��ʾ��
           {
             if((rey-rsy)<=InfoRectTopWidth)
             {
               if((MaxRight-rsx)<InfoRectWidth)
               {
                BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                 InfoSx=rsx-InfoRectWidth;
               }
               else
               {
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
                 InfoSx=rsx;
               }
             }
             else
             {
              if((MaxRight-rsx)<InfoRectWidth)
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
               InfoSx=rsx-InfoRectWidth;
              }
              else
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,(rex-rsx),(rey-rsy-InfoRectTopWidth),Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);//��1��
               BitBlt(BufferBmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,rsx+InfoRectWidth-rex,(rey-rsy-InfoRectTopWidth),GrayGraybmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,SRCCOPY);//��1��
               BitBlt(BufferBmp->Canvas->Handle,rsx,rey,InfoRectWidth,(InfoRectHeight+InfoRectTopWidth)-(rey-rsy),GrayGraybmp->Canvas->Handle,rsx,rey,SRCCOPY);
               InfoSx=rsx;
              }
             }       //��2��

           }
           else if((rex-rsx)<InfoRectWidth &&(rey-rsy)>=(InfoRectHeight+InfoRectTopWidth))
            {
              if((MaxRight-rsx)<InfoRectWidth)
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                InfoSx=rsx-InfoRectWidth;
              }
              else
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,(rex-rsx),InfoRectHeight,Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
               BitBlt(BufferBmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,InfoRectWidth-(rex-rsx),InfoRectHeight,GrayGraybmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,SRCCOPY);
               InfoSx=rsx;
              }
            }
           else if((rex-rsx)>=InfoRectWidth &&(rey-rsy)<(InfoRectHeight+InfoRectTopWidth))
            {

             if((MaxRight-rsx)<InfoRectWidth)
             {
              BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
               InfoSx=rsx-InfoRectWidth;
             }
             else
             {
               if((rey-rsy)<=InfoRectTopWidth)
               {
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
               }
                else
               {
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,rey-rsy-InfoRectTopWidth,Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rey,InfoRectWidth,rsy-rey+(InfoRectHeight+InfoRectTopWidth),GrayGraybmp->Canvas->Handle,rsx,rey,SRCCOPY);
               }
               InfoSx=rsx;
             }
            }
           else
            {
              if((MaxRight-rsx)<InfoRectWidth)
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                InfoSx=rsx-InfoRectWidth;
              }
              else
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
               InfoSx=rsx;
              }
            }
            //<(InfoRectHeight+InfoRectTopWidth)ʱ�Ұ���Ϣ��ʾ�����ʼλ��
               InfoSy=rsy+InfoRectTopWidth;// ��¼InfoRect��Ϣ�����ʼ��λ��

        }
        else       //rsy>InfoRectHeight+InfoRectTopHeight
        {
          if((MaxRight-rsx)<InfoRectWidth)
          {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy-(InfoRectHeight+InfoRectTopWidth),InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy-(InfoRectHeight+InfoRectTopWidth),SRCCOPY);
               InfoSx=rsx-InfoRectWidth;
          }
          else
          {
           BitBlt(BufferBmp->Canvas->Handle,rsx,rsy-(InfoRectHeight+InfoRectTopWidth),InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx,rsy-(InfoRectHeight+InfoRectTopWidth),SRCCOPY);
             InfoSx=rsx;
          }

          //>(InfoRectHeight+InfoRectTopWidth)ʱ�Ұ���Ϣ��ʾ�����ʼλ��
            InfoSy=rsy-(InfoRectHeight+InfoRectTopWidth); //InfoSxδ��� �ض�=rsx
        }


     SetBkMode(BufferBmp->Canvas->Handle,TRANSPARENT);
     SetTextColor(BufferBmp->Canvas->Handle,InfoTextColor);
     DrawInfo(BufferBmp->Canvas,InfoSx,InfoSy,CutRect.Width(),CutRect.Height(),rsx,rsy,lx,ly);//InfoSx��InfoSy����Ϣ�����ʼ��λ�� ��6����������Ϣ����
     DrawTracker(BufferBmp->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);
     BitBlt(Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
     InfoRectHidden=false;
    }
    FuncButtonDown=false;
    Triming=false;  //�˳��༭״̬
    if(OperationCount==0)//����ܻ�ԭ�����ձ༭����״̬�����ǿ����˳��༭״̬�ģ�������ܻ��˵��ձ༭״̬����ô�༭״̬��һֱ����
    {
     //���LastDrawBmp����ԭΪԭͼ
     BitBlt(LastDrawBmp->Canvas->Handle,0,0,Form1->Width,Form1->Height,Srcbmp->Canvas->Handle,0,0,SRCCOPY);  //��� PanelͼƬ��record ��������Ŀ��

    }
    if(CurrentMode==Text_5)
     {
      if(ondrawing&&Memo1->Visible)
      {  //�������д�֣���ô���д��
         BitBlt(Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
         Memo1->Hide();
         ondrawing=false;
      }
     }

 }
 else if(DownImage!=i) //֮ǰ�а��µİ�ť
 {
    //����գ����Լ�down
    int LastMode=CurrentMode;
    ((TImage*)(BmpList->Items[DownImage-1]))->Picture->Bitmap->LoadFromStream(ImageStream[(DownImage-1)*3+1]);
     ImageStream[(DownImage-1)*3+1]->Position=0;
     ((TImage*)(BmpList->Items[i-1]))->Picture->Bitmap->LoadFromStream(ImageStream[(i-1)*3]);  //down
    ImageStream[(i-1)*3]->Position=0;
    DownImage=i;
      if(i==1||i==2||i==5)//�ʺͻ�ˢ����ͷ��ֱ��
    {
     Form1->SizeInfo->Caption="���ʳߴ�:";
     Form1->Fillable->Hide();
    }
    else if(i==3||i==4)
    {
     Form1->SizeInfo->Caption="���ʳߴ�:";
     Form1->Fillable->Show();
    }
    else if(i==6)
    {
     Form1->SizeInfo->Caption="�����С:";
     Form1->Fillable->Hide();
    }
    if(CutRect.Top<(Form1->InfoRectWidth+Form1->InfoRectTopWidth))
     {  //��Ϣ������
       CurrentMode=i-1;
       Form1->InfoRectHidden=true;
       BitBlt(BufferBmp->Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);//ȫͼBitBlt
       BitBlt(BufferBmp->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
       Form1->DrawTracker(BufferBmp->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);
       BitBlt(Form1->Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
     }
     else
     {//���� inforect��Ϣ��

       CurrentMode=i-1;
       Form1->InfoRectHidden=false;
     }
     if(CurrentMode==Text_5)
     {
       SizeEdit->Hint="��С����8,�������32";
       SizeEdit->Text=IntToStr(TextSize);
     }
     else
     {
       SizeEdit->Hint="��С�ʿ�1,���ʿ�35";
       SizeEdit->Text=IntToStr(DrawWidth);
     }
     if(LastMode==Text_5)
     {
       Exceptable=true;
      if(ondrawing&&Memo1->Visible)
      {  //�������д�֣���ô���д��
         BitBlt(Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
         Memo1->Hide();
         ondrawing=false;
      }
     }

    SetColorPanelPosition();
    Triming=true;
 }

}
//----------------------------------------------------------------------------------------
inline void downfun7_12(int i)
{
//���� downͼƬ
   ((TImage*)(BmpList->Items[i-1]))->Picture->Bitmap->LoadFromStream(ImageStream[(i-1)*3]);  //down
   ImageStream[(i-1)*3]->Position=0;
//   DownImage=i;
}
//---------------------------------------------------------------------------------------
inline void upfun7_12_pop(int i)
{
}
//----------------------------------------------------------------------------------------
inline void upfun7_12(int i)
{
   if(i==7&&!FormQuickKey)
   {
   ((TImage*)(BmpList->Items[i-1]))->Picture->Bitmap->LoadFromStream(ImageStream[(i-1)*3+2]);  //up
   ImageStream[(i-1)*3+2]->Position=0;
   FormQuickKey=false;
   }
   else
   {
   ((TImage*)(BmpList->Items[i-1]))->Picture->Bitmap->LoadFromStream(ImageStream[(i-1)*3+1]);  //up
   ImageStream[(i-1)*3+1]->Position=0;
   }
  //7��12 ������ť�� �����¼���֮���Բ���click�¼�����Ϊ˫���¼���������
   if(i==7)
   {
     if(RecordCount>1)
     {
       RecordPoint=(RecordPoint-1)>=0?(RecordPoint-1):4;   //-1 ѭ��
       BitBlt(LastDrawBmp->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),RecordBmp[RecordPoint]->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
       Form1->DrawTracker(RecordBmp[RecordPoint]->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);
       BitBlt(BufferBmp->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),RecordBmp[RecordPoint]->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
       BitBlt(Form1->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),RecordBmp[RecordPoint]->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
       RecordCount--;
       OperationCount--;
       if(OperationCount==0) //û�в�����¼������LastDrawBmp
           BitBlt(LastDrawBmp->Canvas->Handle,0,0,Form1->Width,Form1->Height,Srcbmp->Canvas->Handle,0,0,SRCCOPY);
     }
     else
     {
      BitBlt(LastDrawBmp->Canvas->Handle,0,0,Form1->Width,Form1->Height,Srcbmp->Canvas->Handle,0,0,SRCCOPY);
      //��� PanelͼƬ��record ��������Ŀ��
      if(DownImage>0)
      {
       ((TImage*)(BmpList->Items[DownImage-1]))->Picture->Bitmap->LoadFromStream(ImageStream[3*(DownImage-1)+1]);
       ImageStream[3*(DownImage-1)+1]->Position=0;
      }
      DownImage=0; //��DownImage
      if(InImage>0)
      {
       ((TImage*)(BmpList->Items[InImage-1]))->Picture->Bitmap->LoadFromStream(ImageStream[3*(InImage-1)+1]);
       ImageStream[3*(InImage-1)+1]->Position=0;
      }
      InImage=0;   //��InImage
      FuncButtonDown=false; //û�й��ܰ�ť��
      Triming=false;  //���ڱ༭
      OperationCount=0;    //��������0
      RecordPoint=-1; //��¼ָ��ָ��-1��
      RecordCount=0; //��¼��0
      ondrawing=false; //�Ƿ����ڻ�ͼ����д����
      oncuting=false;//�ָ��޽�ͼ����״̬  ��ȥ���༭״̬
      Form1->MainPanel->Hide();
      Form1->ColorPanel->Hide();
      Form1->Memo1->Clear();
      Form1->Memo1->Hide();
      UpdateWindow(Form1->Handle);
      ::SetCursor(TB_WAVE);
      BitBlt(Form1->Canvas->Handle,0,0,Form1->Width,Form1->Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);
      DrawBmp->Canvas->StretchDraw(Rect(0,0,Grayerbmp->Width,Grayerbmp->Height),Srcbmp);//ȥ���滭

     }
   }
   else if(i==8)
   {
    //save to file
     Form1->SaveToFile_user();
   }
   else if(i==10)
   {
      Form1->Hide();
   }
   else if(i==11)
   {  //Finish
      Form1->CutingFinish();
   }
   else if(i==9)
   {  //about wave
       ShellExecute(Form1->Handle,"open","http://www.wavechild.com",NULL,NULL,SW_NORMAL);
       Form1->Hide();
   }
   else if(i==12)
   { //about author
        ShellExecute(Form1->Handle,"open","http://renren.com/woailuotangbo",NULL,NULL,SW_NORMAL);
       Form1->Hide();
   }

//   DownImage=0;
}
//--------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------

void __fastcall TForm1::MMMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
          if(CurrentMode==Text_5)
          {
           if(TextSize<MinFontSize)
           {
               SizeEdit->Text=IntToStr(MinFontSize);
               TextSize=MinFontSize;        //8Ϊ��С����
          }
          }

 if(InImage!=DownImage&&InImage!=0)
 {
  ((TImage*)(BmpList->Items[InImage-1]))->Picture->Bitmap->LoadFromStream(ImageStream[(InImage-1)*3+1]);
  ImageStream[(InImage-1)*3+1]->Position=0;
  InImage=0;
 }        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ColorTableMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 if(X>6&&X<265&&Y>6&&Y<44)
 {
  DrawColor=ColorTable->Canvas->Pixels[X][Y];
  ColorTable->Canvas->Brush->Color=ColorTable->Canvas->Pixels[X][Y];
  ColorTable->Canvas->Rectangle(6,50,30,74);
  ColorTableMouseDownFLAG=true;
 // UINT8 red=GetRValue(ColorTable->Canvas->Pixels[X][Y]);
 //    UINT8 green=GetGValue(ColorTable->Canvas->Pixels[X][Y]);
 //    UINT8 black=GetBValue(ColorTable->Canvas->Pixels[X][Y]);
 //    ColorTable->Hint="RGB("+IntToStr(red)+","+ IntToStr(green)+","+IntToStr(black)+")";

 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ColorTableMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
 if(X>6&&X<265&&Y>6&&Y<44)
 {
   if(ColorTableMouseDownFLAG)
   {
     DrawColor=ColorTable->Canvas->Pixels[X][Y];
     Memo1->Font->Color=ColorTable->Canvas->Pixels[X][Y];
     ColorTable->Canvas->Brush->Color=ColorTable->Canvas->Pixels[X][Y];
     ColorTable->Canvas->Rectangle(6,50,30,74);
   }
   ColorTable->Cursor=crCross;
 }
 else
 {
   ColorTable->Cursor=crDefault;
 }
          if(CurrentMode==Text_5)
          {
           if(TextSize<MinFontSize)
           {
                 SizeEdit->Text=IntToStr(MinFontSize);
                 TextSize=MinFontSize;        //MaxFontSize Ϊ��С����
           }
          }



}
//---------------------------------------------------------------------------

void __fastcall TForm1::ColorTableMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
ColorTableMouseDownFLAG=false;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TForm1::LButMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
 LBut->Picture->Bitmap->LoadFromStream(ImageStream[39]);
 ImageStream[39]->Position=0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LButMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
 LBut->Picture->Bitmap->LoadFromStream(ImageStream[38]);
 ImageStream[38]->Position=0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RButMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
 RBut->Picture->Bitmap->LoadFromStream(ImageStream[37]);
 ImageStream[37]->Position=0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RButMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
 RBut->Picture->Bitmap->LoadFromStream(ImageStream[36]);
 ImageStream[36]->Position=0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RButClick(TObject *Sender)
{
if(CurrentMode!=Text_5)
{
 int size=ToData(SizeEdit->Text);
 if(size<MaxPenSize)
 size++;
 SizeEdit->Text=IntToStr(size);
}
else
{
 int size=ToData(SizeEdit->Text);
 if(size<MaxFontSize)
 size++;
 SizeEdit->Text=IntToStr(size);
}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::LButClick(TObject *Sender)
{
if(CurrentMode!=Text_5)
{
 int size=ToData(SizeEdit->Text);
 if(size>MinPenSize)
 size--;
 SizeEdit->Text=IntToStr(size);
}
else
{
 int size=ToData(SizeEdit->Text);
 if(size>MinFontSize)
 size--;
 SizeEdit->Text=IntToStr(size);
}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::RButDblClick(TObject *Sender)
{
if(CurrentMode!=Text_5)
{
 int size=ToData(SizeEdit->Text);
 if(size<MaxPenSize)
 size++;
 SizeEdit->Text=IntToStr(size);
}
else
{
 int size=ToData(SizeEdit->Text);
 if(size<MaxFontSize)
 size++;
 SizeEdit->Text=IntToStr(size);
}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::LButDblClick(TObject *Sender)
{
if(CurrentMode!=Text_5)
{
 int size=ToData(SizeEdit->Text);
 if(size>MinPenSize)
 size--;
 SizeEdit->Text=IntToStr(size);
}
else
{
 int size=ToData(SizeEdit->Text);
 if(size>MinFontSize)
 size--;
 SizeEdit->Text=IntToStr(size);
}
}
//-------------------------------------------------------------------------------------
int TForm1::ToData(AnsiString str)
{
  AnsiString data="";
  if( *(str.c_str())>='0' && *(str.c_str())<='9')
  {
     data+=*(str.c_str());
  }
  if( *(str.c_str()+1)>='0' && *(str.c_str()+1)<='9')
  {
     data+=*(str.c_str()+1);
  }
  if(data.IsEmpty())
  return 0;//>MaxPenSize?MaxPenSize:(DrawWidth<MinPenSize?MinPenSize:DrawWidth);
  return StrToInt(data);
}
void __fastcall TForm1::M7MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
upfun7_12(7);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::M8MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
 upfun7_12(8);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::M9MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
 upfun7_12(9);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::M10MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
  upfun7_12(10);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::M11MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
  upfun7_12(11);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::M12MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
   upfun7_12(12);     
}
//---------------------------------------------------------------------------
inline void SetColorPanelPosition()
{
    if(Form1->MainPanel->Left>CutRect.right)//MainPanel ����
    {

       if((InfoSx+Form1->InfoRectWidth)>CutRect.right ) //InfoRect�Ŀ����CutRect
       {
          if(CutRect.Left>(ColorPanelWidth+2))
          {
            Form1->ColorPanel->Left=CutRect.left-(ColorPanelWidth+2);
            Form1->ColorPanel->Top=CutRect.Top;
          }
          else
          {
            Form1->ColorPanel->Left=Form1->MainPanel->Left;
            Form1->ColorPanel->Top=Form1->MainPanel->Top+MainPanelHeight+2;
          }
       }
       else                    //InfoRect�Ŀ�С��CutRect
       {

          if(MaxRight-(InfoSx+Form1->InfoRectWidth)>(ColorPanelWidth+2)&&InfoSy<CutRect.top)  //����InfoRect�ĺ���
           {
              if((InfoSx+Form1->InfoRectWidth+(ColorPanelWidth+2))<(CutRect.right+10+MainPanelWidth))
              {
                Form1->ColorPanel->Left=Form1->MainPanel->Left-(ColorPanelWidth-MainPanelWidth);
                Form1->ColorPanel->Top=Form1->MainPanel->Top-ColorPanelHeight-2 ;
              }
              else
              {
                Form1->ColorPanel->Left=InfoSx+Form1->InfoRectWidth+5;
                Form1->ColorPanel->Top=Form1->MainPanel->Top-ColorPanelHeight-2 ;
              }
           }
           else if((MaxRight-Form1->MainPanel->Left)>ColorPanelWidth)
           {
                Form1->ColorPanel->Left=Form1->MainPanel->Left;
                Form1->ColorPanel->Top=Form1->MainPanel->Top+MainPanelHeight+2 ;
           }
           else      //�����λ�ö�û�п�ȱ�ˣ�
           {
                if(CutRect.left>=(ColorPanelWidth+2))        //�������п�ȱ
                {
                  Form1->ColorPanel->Left=CutRect.left-(ColorPanelWidth+2);
                  Form1->ColorPanel->Top=CutRect.top;
                }
                else//���Ҳû�п�ȱ��
                {
                  Form1->ColorPanel->Left=CutRect.right-(ColorPanelWidth+2);
                  Form1->ColorPanel->Top=Form1->MainPanel->Top;
                }
           }

       }
    }
    else if(Form1->MainPanel->Left+MainPanelWidth<CutRect.left)//MainPanel �����
    {
        if((Form1->MainPanel->Top+MainPanelHeight-CutRect.bottom)>(ColorPanelHeight+2))
        {
            if((MaxRight-Form1->MainPanel->Left)>(MainPanelWidth+ColorPanelWidth+5))
            {
              Form1->ColorPanel->Left=Form1->MainPanel->Left+MainPanelWidth+5;
              Form1->ColorPanel->Top=CutRect.bottom+2;
            }
            else  //���ܷ����¶�
            {
              Form1->ColorPanel->Left=Form1->MainPanel->Left-ColorPanelWidth-2;
              Form1->ColorPanel->Top=Form1->MainPanel->Top;
            }
        }
        else
        {
            if(CutRect.left>=(ColorPanelWidth+5)&&(Form1->MainPanel->Top+2+MainPanelHeight+ColorPanelHeight)<=MaxBottom)
            {
               Form1->ColorPanel->Left=Form1->MainPanel->Left+MainPanelWidth-ColorPanelWidth;
               Form1->ColorPanel->Top=Form1->MainPanel->Top+MainPanelHeight+2;
            }
            else
            if(Form1->MainPanel->Left>=(ColorPanelWidth+2))
            {
               Form1->ColorPanel->Left=Form1->MainPanel->Left-ColorPanelWidth-2;
               Form1->ColorPanel->Top=Form1->MainPanel->Top;
            }
            else if(CutRect.top>ColorPanelHeight)  //��ʱ��InfoRectҲ��CutRect���ϲ�
            {
              Form1->ColorPanel->Left=InfoSx+Form1->InfoRectWidth+2;
              Form1->ColorPanel->Top=InfoSy;
            }
            else //��ʱ InfoRect��CutRect���ڲ�
            {
              Form1->ColorPanel->Left=InfoSx+Form1->InfoRectWidth+2;
              Form1->ColorPanel->Top=CutRect.top;
            }
        }
    }
    else //MainPanel ���ڲ�
    {
           if(CutRect.top>ColorPanelWidth)
           {
              if(Form1->MainPanel->Top>=CutRect.top)
               {
                 Form1->ColorPanel->Left=Form1->MainPanel->Left-(ColorPanelWidth-MainPanelWidth);
                 Form1->ColorPanel->Top=InfoSy;
               }
               else
               {
                 Form1->ColorPanel->Left=Form1->MainPanel->Left-(ColorPanelWidth+2);
                 Form1->ColorPanel->Top=InfoSy;
               }
           }
           else if(CutRect.Height()<(MainPanelHeight-2))
           {
              Form1->ColorPanel->Left=Form1->MainPanel->Left-(ColorPanelWidth+2);
              Form1->ColorPanel->Top=CutRect.bottom+2;
           }
           else
           {
              Form1->ColorPanel->Left=Form1->MainPanel->Left-(ColorPanelWidth+2);
              Form1->ColorPanel->Top=CutRect.top;
           }

    }
}
//--------------------------------------------------------------------------------------
void __fastcall TForm1::CreateParams(TCreateParams &Param)
{
        TForm::CreateParams(Param);
    //    Param.WindowClass.style=CS_OWNDC;
        Param.WndParent=liuxuan->Handle;

       // Param.Style|=WS_POPUP;
        Param.ExStyle=WS_EX_TOPMOST|WS_EX_TOOLWINDOW ; //Param.ExStyle|  WS_EX_TOPMOST|

}
//--------------------------------------------------------------------------------------
inline void TForm1::CreateCutRect(unsigned short X,unsigned short Y)
{
if(movenum==0)
 movenum++;
//else  if(movenum==1)
//movenum++;
else
 movenum=0;    


if(InImage!=DownImage&&InImage!=0)
{
  ((TImage*)(BmpList->Items[InImage-1]))->Picture->Bitmap->LoadFromStream(ImageStream[(InImage-1)*3+1]);
  ImageStream[(InImage-1)*3+1]->Position=0;
  InImage=0;
}
if(oncuting&&!drawable) //��ͼ�����Ѿ�����   ���Ҳ��������ͼ
{//����������ʽ
 if(!dragable&!directable) //��������ƶ�
 {
    if( (X>=CutRect.left-HalfHandleSize && Y>=CutRect.top-HalfHandleSize && X<=CutRect.left+HalfHandleSize && Y<=CutRect.top+HalfHandleSize)
           || (X>=CutRect.right-HalfHandleSize && Y>=CutRect.bottom-HalfHandleSize && X<=CutRect.right+HalfHandleSize && Y<=CutRect.bottom+HalfHandleSize)
           )//�����ϻ������°ѱ�
    {
      Form1->Cursor=crSizeNWSE;
    }
    else if( (X>=CutRect.right-HalfHandleSize && Y>=CutRect.top-HalfHandleSize && X<=CutRect.right+HalfHandleSize && Y<=CutRect.top+HalfHandleSize)
            || (X>=CutRect.left-HalfHandleSize && Y>=CutRect.bottom-HalfHandleSize && X<=CutRect.left+HalfHandleSize && Y<=CutRect.bottom+HalfHandleSize)
           ) //�����»������ϰѱ�
    {
      Form1->Cursor=crSizeNESW;
    }
    else if( (Y==CutRect.top||Y==CutRect.bottom) && X>CutRect.left && X<CutRect.right
           || (X>=mx-HalfHandleSize && Y>=CutRect.top-HalfHandleSize && X<=mx+HalfHandleSize && Y<=CutRect.top+HalfHandleSize)
           || (X>=mx-HalfHandleSize && Y>=CutRect.bottom-HalfHandleSize && X<=mx+HalfHandleSize && Y<=CutRect.bottom+HalfHandleSize)
           ) //���»����ϰѱ�
    {
      Form1->Cursor=crSizeNS;
    }
    else if( (X==CutRect.left||X==CutRect.right) && Y>CutRect.top && Y<CutRect.bottom
           || (X>=CutRect.left-HalfHandleSize &&X<=CutRect.left+HalfHandleSize &&Y>=my-HalfHandleSize &&Y<my+HalfHandleSize )
           || (X>=CutRect.right-HalfHandleSize &&X<=CutRect.right+HalfHandleSize &&Y>=my-HalfHandleSize &&Y<my+HalfHandleSize )
           )//��������Ұѱ�
    {
      Form1->Cursor=crSizeWE;
    }
    else if(X>CutRect.left&&Y>CutRect.top&&X<CutRect.right&&Y<CutRect.bottom)  //�ڽ�ͼ�����ڲ�
    {
      Form1->Cursor=crSizeAll;
    }
    else
    {
      Form1->Cursor=crDefault;
            ::SetCursor(TB_WAVE);
    }
 //�ػ���Ϣ������Ҫ��Ϊ�˸���RGBֵ

    rsx=CutRect.left;
    rsy=CutRect.top;
    rex=CutRect.right;
    rey=CutRect.bottom;

        if(rsy<(InfoRectHeight+InfoRectTopWidth))
        {
           if((rex-rsx)<InfoRectWidth &&(rey-rsy)<(InfoRectHeight+InfoRectTopWidth))//��Ϣ��ʾ�����ڲ�---���������������Ƚϸ��� ������ʾ��Ϣ�� ������ʱ�Ļ�ɫ��ʾ��
           {
             if((rey-rsy)<=InfoRectTopWidth)
             {
               if((MaxRight-rsx)<InfoRectWidth)
               {
                BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                 InfoSx=rsx-InfoRectWidth;
               }
               else
               {
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
                 InfoSx=rsx;
               }
             }
             else
             {
              if((MaxRight-rsx)<InfoRectWidth)
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                InfoSx=rsx-InfoRectWidth;
              }
              else
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,(rex-rsx),(rey-rsy-InfoRectTopWidth),Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);//��1��
               BitBlt(BufferBmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,rsx+InfoRectWidth-rex,(rey-rsy-InfoRectTopWidth),GrayGraybmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,SRCCOPY);//��1��
               BitBlt(BufferBmp->Canvas->Handle,rsx,rey,InfoRectWidth,(InfoRectHeight+InfoRectTopWidth)-(rey-rsy),GrayGraybmp->Canvas->Handle,rsx,rey,SRCCOPY);
               InfoSx=rsx;
              }
             }       //��2��

           }
           else if((rex-rsx)<InfoRectWidth &&(rey-rsy)>=(InfoRectHeight+InfoRectTopWidth))
            {
              if((MaxRight-rsx)<InfoRectWidth)
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                InfoSx=rsx-InfoRectWidth;
              }
              else
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,(rex-rsx),InfoRectHeight,Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
               BitBlt(BufferBmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,InfoRectWidth-(rex-rsx),InfoRectHeight,GrayGraybmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,SRCCOPY);
               InfoSx=rsx;
              }
            }
           else if((rex-rsx)>=InfoRectWidth &&(rey-rsy)<(InfoRectHeight+InfoRectTopWidth))
            {

             if((MaxRight-rsx)<InfoRectWidth)
             {
              BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
               InfoSx=rsx-InfoRectWidth;
             }
             else
             {
               if((rey-rsy)<=InfoRectTopWidth)
               {
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
               }
                else
               {
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,rey-rsy-InfoRectTopWidth,Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rey,InfoRectWidth,rsy-rey+(InfoRectHeight+InfoRectTopWidth),GrayGraybmp->Canvas->Handle,rsx,rey,SRCCOPY);
               }
               InfoSx=rsx;
             }
            }
           else
            {
              if((MaxRight-rsx)<InfoRectWidth)
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                InfoSx=rsx-InfoRectWidth;
              }
              else
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
               InfoSx=rsx;
              }
            }
            //<(InfoRectHeight+InfoRectTopWidth)ʱ�Ұ���Ϣ��ʾ�����ʼλ��
               InfoSy=rsy+InfoRectTopWidth;// ��¼InfoRect��Ϣ�����ʼ��λ��

        }
        else       //rsy>InfoRectHeight+InfoRectTopHeight
        {
          if((MaxRight-rsx)<InfoRectWidth)
          {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy-(InfoRectHeight+InfoRectTopWidth),InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy-(InfoRectHeight+InfoRectTopWidth),SRCCOPY);
               InfoSx=rsx-InfoRectWidth;
          }
          else
          {
           BitBlt(BufferBmp->Canvas->Handle,rsx,rsy-(InfoRectHeight+InfoRectTopWidth),InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx,rsy-(InfoRectHeight+InfoRectTopWidth),SRCCOPY);
               InfoSx=rsx;
          }
            //>(InfoRectHeight+InfoRectTopWidth)ʱ�Ұ���Ϣ��ʾ�����ʼλ��
               InfoSy=rsy-(InfoRectHeight+InfoRectTopWidth); //InfoSxδ��� �ض�=rsx
        }
     SetBkMode(BufferBmp->Canvas->Handle,TRANSPARENT);
     SetTextColor(BufferBmp->Canvas->Handle,InfoTextColor);
     DrawInfo(BufferBmp->Canvas,InfoSx,InfoSy,CutRect.Width(),CutRect.Height(),rsx,rsy,X,Y);
     DrawTracker(BufferBmp->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);
     BitBlt(Canvas->Handle,InfoSx,InfoSy,InfoRectWidth,InfoRectHeight,BufferBmp->Canvas->Handle,InfoSx,InfoSy,SRCCOPY);
   }
  else if(dragable)// dragable==true �������������ͼ��
  {

       if(CutRect.left+X-sx<=0)
       {
         CutRect.right=CutRect.right-CutRect.left;
         CutRect.left=0;
       }
       else if(CutRect.right+X-sx>=MaxRight)
       {
         CutRect.left+=(MaxRight-CutRect.right) ;
         CutRect.right=MaxRight;
       }
       else
       {
         CutRect.left=CutRect.left+X-sx;
         CutRect.right=CutRect.right+X-sx;
       }

       if(CutRect.top+Y-sy<=0)
       {
         CutRect.bottom=CutRect.bottom-CutRect.top;
         CutRect.top=0;
       }
       else if(CutRect.bottom+Y-sy>=MaxBottom)
       {
         CutRect.top+=(MaxBottom-CutRect.bottom);
         CutRect.bottom=MaxBottom;
       }
       else
       {
         CutRect.top=CutRect.top+Y-sy;
         CutRect.bottom=CutRect.bottom+Y-sy;
       }
     //----��������-------                           HalfHandleSize
     BitBlt(BufferBmp->Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);//ȫͼBitBlt
    // BitBlt(BufferBmp->Canvas->Handle,CutRect.left-HalfHandleSize,CutRect.top-HalfHandleSize,CutRect.Width()+HandleSize,CutRect.Height()+HandleSize,Graybmp->Canvas->Handle,CutRect.left-HalfHandleSize,CutRect.top-HalfHandleSize,SRCCOPY);//����BitBlt
     BitBlt(BufferBmp->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),Srcbmp->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
    //-------------------�Ұ���ʾ�����---------------------
        rsx=CutRect.left;
        rsy=CutRect.top;
        rex=CutRect.right;
        rey=CutRect.bottom;
        if(rsy<(InfoRectHeight+InfoRectTopWidth))
        {
           if((rex-rsx)<InfoRectWidth &&(rey-rsy)<(InfoRectHeight+InfoRectTopWidth))//��Ϣ��ʾ�����ڲ�---���������������Ƚϸ��� ������ʾ��Ϣ�� ������ʱ�Ļ�ɫ��ʾ��
           {
             if((rey-rsy)<=InfoRectTopWidth)
             {
               if((MaxRight-rsx)<InfoRectWidth)
               {
                BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                 InfoSx=rsx-InfoRectWidth;
               }
               else
               {
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
                 InfoSx=rsx;
               }
             }
             else
             {
              if((MaxRight-rsx)<InfoRectWidth)
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
               InfoSx=rsx-InfoRectWidth;
              }
              else
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,(rex-rsx),(rey-rsy-InfoRectTopWidth),Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);//��1��
               BitBlt(BufferBmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,rsx+InfoRectWidth-rex,(rey-rsy-InfoRectTopWidth),GrayGraybmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,SRCCOPY);//��1��
               BitBlt(BufferBmp->Canvas->Handle,rsx,rey,InfoRectWidth,(InfoRectHeight+InfoRectTopWidth)-(rey-rsy),GrayGraybmp->Canvas->Handle,rsx,rey,SRCCOPY);
               InfoSx=rsx;
              }
             }       //��2��

           }
           else if((rex-rsx)<InfoRectWidth &&(rey-rsy)>=(InfoRectHeight+InfoRectTopWidth))
            {
              if((MaxRight-rsx)<InfoRectWidth)
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                InfoSx=rsx-InfoRectWidth;
              }
              else
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,(rex-rsx),InfoRectHeight,Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
               BitBlt(BufferBmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,InfoRectWidth-(rex-rsx),InfoRectHeight,GrayGraybmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,SRCCOPY);
               InfoSx=rsx;
              }
            }
           else if((rex-rsx)>=InfoRectWidth &&(rey-rsy)<(InfoRectHeight+InfoRectTopWidth))
            {

             if((MaxRight-rsx)<InfoRectWidth)
             {
              BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
               InfoSx=rsx-InfoRectWidth;
             }
             else
             {
               if((rey-rsy)<=InfoRectTopWidth)
               {
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
               }
                else
               {
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,rey-rsy-InfoRectTopWidth,Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rey,InfoRectWidth,rsy-rey+(InfoRectHeight+InfoRectTopWidth),GrayGraybmp->Canvas->Handle,rsx,rey,SRCCOPY);
               }
               InfoSx=rsx;
             }
            }
           else
            {
              if((MaxRight-rsx)<InfoRectWidth)
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                InfoSx=rsx-InfoRectWidth;
              }
              else
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
               InfoSx=rsx;
              }
            }
            //<(InfoRectHeight+InfoRectTopWidth)ʱ�Ұ���Ϣ��ʾ�����ʼλ��
               InfoSy=rsy+InfoRectTopWidth;// ��¼InfoRect��Ϣ�����ʼ��λ��

        }
        else       //rsy>InfoRectHeight+InfoRectTopHeight
        {
          if((MaxRight-rsx)<InfoRectWidth)
          {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy-(InfoRectHeight+InfoRectTopWidth),InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy-(InfoRectHeight+InfoRectTopWidth),SRCCOPY);
               InfoSx=rsx-InfoRectWidth;
          }
          else
          {
           BitBlt(BufferBmp->Canvas->Handle,rsx,rsy-(InfoRectHeight+InfoRectTopWidth),InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx,rsy-(InfoRectHeight+InfoRectTopWidth),SRCCOPY);
             InfoSx=rsx;
          }

          //>(InfoRectHeight+InfoRectTopWidth)ʱ�Ұ���Ϣ��ʾ�����ʼλ��
            InfoSy=rsy-(InfoRectHeight+InfoRectTopWidth); //InfoSxδ��� �ض�=rsx
        }


     SetBkMode(BufferBmp->Canvas->Handle,TRANSPARENT);
     SetTextColor(BufferBmp->Canvas->Handle,InfoTextColor);
     DrawInfo(BufferBmp->Canvas,InfoSx,InfoSy,CutRect.Width(),CutRect.Height(),rsx,rsy,X,Y);//InfoSx��InfoSy����Ϣ�����ʼ��λ�� ��6����������Ϣ����
     DrawTracker(BufferBmp->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);
     BitBlt(Canvas->Handle,0,0,BufferBmp->Width,BufferBmp->Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
     sx=X;//������һ�λ�ͼ����ʱ�������λ�ͼ������λ��
     sy=Y;
   if(movenum==1||(!CanToolBarVisible))//��� ��move�¼��������𻭿򣬵��Ƕ����ٶȽ����Ĵ���Update����������move�¼�����һ�Ρ����move����һ�Σ���ô�¿��ƶ���
     return;
       if((MaxRight-CutRect.right)<149)    //����ұ߲���һ��MainPanel
           {
             if(CutRect.left<149)          //������Ҳ����
             {
                 if((MaxBottom-CutRect.top)<173)  //�ײ���ͷ��
                  {
                      Form1->MainPanel->Left=CutRect.right-145;;
                      Form1->MainPanel->Top=MaxBottom-173;
                  }
                  else
                  {
                      Form1->MainPanel->Left=CutRect.right-145;
                      Form1->MainPanel->Top =CutRect.top;
                  }
             }
             else                          //����߹� �������
             {
               if((MaxRight-CutRect.left)<InfoRectWidth && CutRect.top<(InfoRectHeight+InfoRectTopWidth)) //��������Ϣ��������� �����˿���±�����
               {
                 Form1->MainPanel->Left=CutRect.left -149;
                 Form1->MainPanel->Top=InfoSy+InfoRectHeight+5;
               }
               else if((MaxRight-CutRect.left)<InfoRectWidth)  //��Ϣ������ߣ��һ��ڿ�����ϲ�
               {
                 if((MaxBottom-CutRect.top)<173)  //�ײ���ͷ��
                  {
                      Form1->MainPanel->Left=CutRect.left -InfoRectWidth-149;
                      Form1->MainPanel->Top=MaxBottom-173;
                  }
                 else            //�ײ�û��ͷ
                  {
                    Form1->MainPanel->Left=CutRect.left -149;
                    Form1->MainPanel->Top=CutRect.top;
                  }
               }
               else  //��Ϣ�����˿�����Ϸ�
               {
                 if((MaxBottom-CutRect.top)<173)  //�ײ���ͷ��
                  {
                      Form1->MainPanel->Left=CutRect.left-149;
                      Form1->MainPanel->Top=MaxBottom-173;
                  }
                 else
                  {
                      Form1->MainPanel->Left=CutRect.left -149;
                      Form1->MainPanel->Top=CutRect.top;
                  }
               }
             }
           }
           else    //����ұ߹�һ��MainPanel
           {
               if(CutRect.top<(InfoRectHeight+InfoRectTopWidth))//��Ϣ������
                {
                    Form1->MainPanel->Left=CutRect.right+5;
                    if((CutRect.Width()+4)<InfoRectWidth)
                    Form1->MainPanel->Top=CutRect.top+InfoRectTopWidth+InfoRectHeight+5;
                    else
                    Form1->MainPanel->Top=CutRect.top;
                }
                else
                {
                  if((MaxBottom-CutRect.top)<173)  //�ײ���ͷ��
                    {
                      if((CutRect.Width())<InfoRectWidth)
                        Form1->MainPanel->Left=CutRect.left-149;
                      else
                        Form1->MainPanel->Left=CutRect.right+5;
                      Form1->MainPanel->Top=MaxBottom-173;
                    }
                   else
                    {
                      Form1->MainPanel->Left=CutRect.right+5;
                      Form1->MainPanel->Top=CutRect.top;
                    }
                }
          }

      UpdateWindow(Form1->Handle);
     }
  else if(directable)//directable==true ˮƽ������ֱ ���� �Ͽ��ͼ
  {
     Form1->MainPanel->Hide();
     Form1->ColorPanel->Hide();
     if(Form1->Cursor==crSizeNS)//��ֱ��ͼ
     {
       if(lx==-2)//��ʼΪ��ˮƽ��
       CutRect=GetCurRect(CutRect.left,Y,sx,sy);//����Ϊ�µ�Y�������꣩�������겻��
       else if(lx==-3)//��ʼΪ��ˮƽ��
       CutRect=GetCurRect(sx,sy,CutRect.right,Y);//����Ϊ�µ�Y�������꣩�������겻��
     }
     else if(Form1->Cursor==crSizeWE)  //ˮƽ��ͼ
     {
      if(lx==-2)//��ʼΪ����ֱ��
      CutRect=GetCurRect(X,CutRect.top,sx,sy);//����Ϊ�µ�Y�������꣩�������겻��
      else if(lx==-3)//��ʼΪ��ˮƽ��
      CutRect=GetCurRect(sx,sy,X,CutRect.bottom);//����Ϊ�µ�Y�������꣩�������겻��
     }
   //-----------------��������-------------------------------
   BitBlt(BufferBmp->Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);//ȫͼBitBlt
  // BitBlt(BufferBmp->Canvas->Handle,sx-2,sy-2,lx-sx+4,ly-sy+4,Graybmp->Canvas->Handle,sx-2,sy-2,SRCCOPY);//����BitBlt
   BitBlt(BufferBmp->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),Srcbmp->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
   //------�Ұ���ʾ��---------------------
        rsx=CutRect.left;
        rsy=CutRect.top;
        rex=CutRect.right;
        rey=CutRect.bottom;
        if(rsy<(InfoRectHeight+InfoRectTopWidth))
        {
           if((rex-rsx)<InfoRectWidth &&(rey-rsy)<(InfoRectHeight+InfoRectTopWidth))//��Ϣ��ʾ�����ڲ�---���������������Ƚϸ��� ������ʾ��Ϣ�� ������ʱ�Ļ�ɫ��ʾ��
           {
             if((rey-rsy)<=InfoRectTopWidth)
             {
               if((MaxRight-rsx)<InfoRectWidth)
               {
                BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                 InfoSx=rsx-InfoRectWidth;
               }
               else
               {
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
                 InfoSx=rsx;
               }
             }
             else
             {
              if((MaxRight-rsx)<InfoRectWidth)
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                InfoSx=rsx-InfoRectWidth;
              }
              else
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,(rex-rsx),(rey-rsy-InfoRectTopWidth),Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);//��1��
               BitBlt(BufferBmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,rsx+InfoRectWidth-rex,(rey-rsy-InfoRectTopWidth),GrayGraybmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,SRCCOPY);//��1��
               BitBlt(BufferBmp->Canvas->Handle,rsx,rey,InfoRectWidth,(InfoRectHeight+InfoRectTopWidth)-(rey-rsy),GrayGraybmp->Canvas->Handle,rsx,rey,SRCCOPY);
               InfoSx=rsx;
              }
             }       //��2��

           }
           else if((rex-rsx)<InfoRectWidth &&(rey-rsy)>=(InfoRectHeight+InfoRectTopWidth))
            {
              if((MaxRight-rsx)<InfoRectWidth)
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                InfoSx=rsx-InfoRectWidth;
              }
              else
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,(rex-rsx),InfoRectHeight,Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
               BitBlt(BufferBmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,InfoRectWidth-(rex-rsx),InfoRectHeight,GrayGraybmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,SRCCOPY);
               InfoSx=rsx;
              }
            }
           else if((rex-rsx)>=InfoRectWidth &&(rey-rsy)<(InfoRectHeight+InfoRectTopWidth))
            {

             if((MaxRight-rsx)<InfoRectWidth)
             {
                 BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                 InfoSx=rsx-InfoRectWidth;
             }
             else
             {
               if((rey-rsy)<=InfoRectTopWidth)
               {
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
               }
                else
               {
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,rey-rsy-InfoRectTopWidth,Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rey,InfoRectWidth,rsy-rey+(InfoRectHeight+InfoRectTopWidth),GrayGraybmp->Canvas->Handle,rsx,rey,SRCCOPY);
               }
               InfoSx=rsx;
             }
            }
           else
            {
              if((MaxRight-rsx)<InfoRectWidth)
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                InfoSx=rsx-InfoRectWidth;
              }
              else
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
               InfoSx=rsx;
              }
            }
            //<(InfoRectHeight+InfoRectTopWidth)ʱ�Ұ���Ϣ��ʾ�����ʼλ��
               InfoSy=rsy+InfoRectTopWidth;// ��¼InfoRect��Ϣ�����ʼ��λ��

        }
        else       //rsy>InfoRectHeight+InfoRectTopHeight
        {
          if((MaxRight-rsx)<InfoRectWidth)
          {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy-(InfoRectHeight+InfoRectTopWidth),InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy-(InfoRectHeight+InfoRectTopWidth),SRCCOPY);
               InfoSx=rsx-InfoRectWidth;
          }
          else
          {
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy-(InfoRectHeight+InfoRectTopWidth),InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx,rsy-(InfoRectHeight+InfoRectTopWidth),SRCCOPY);
               InfoSx=rsx;
          }
            //>(InfoRectHeight+InfoRectTopWidth)ʱ�Ұ���Ϣ��ʾ�����ʼλ��
               InfoSy=rsy-(InfoRectHeight+InfoRectTopWidth); //InfoSxδ��� �ض�=rsx
        }

     SetBkMode(BufferBmp->Canvas->Handle,TRANSPARENT);
     SetTextColor(BufferBmp->Canvas->Handle,InfoTextColor);
     DrawInfo(BufferBmp->Canvas,InfoSx,InfoSy,CutRect.Width(),CutRect.Height(),rsx,rsy,X,Y);
     DrawTracker(BufferBmp->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);
     BitBlt(Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);

  }

}
else//�޽�ͼ����
if(drawable)  //�����ͼ�������������1���½�����2���޸���������Ϊ˫���ͷʱ�� ��ʼ�㣺sx��sy
{ //�ݵ�ǰ����������ʽ
 if(Form1->MainPanel->Visible)
 {
  Form1->MainPanel->Hide();
  Form1->ColorPanel->Hide();
 }
  if(Form1->Cursor==crSizeNESW||Form1->Cursor==crSizeNWSE)   //�����������Ľ��ʹ��������������޸���������ʱ�ı�־λ��
  {
   if((X<sx&&Y<sy)||(X>sx&&Y>sy))     //����ʼ������ϻ������£�ʹ�����ϵ����µ�˫���ͷ
   {
      ::SetCursor(TBSizeNWSE);
   }
   else  if((X>sx&&Y<sy)||(X<sx&&Y>sy))   //����ʼ������»������ϣ�ʹ�����µ����ϵ�˫���ͷ
   {
      ::SetCursor(TBSizeNESW);
   }
  }

  if(lx==-1)      //��һ�Σ�ֱ�Ӵ������򣬲���Ҫˢ��
  {
    BitBlt(BufferBmp->Canvas->Handle,sx,sy,X-sx,Y-sy,Srcbmp->Canvas->Handle,sx,sy,SRCCOPY);
     if(sx<=X&&sy<=Y)      //1
     {
       rsx=sx;
       rsy=sy;
       rex=X;
       rey=Y;
     }
     else if(sx<=X&&sy>=Y) //4
     {
       rsx=sx;
       rsy=Y;
       rex=X;
       rey=sy;
     }
     else if(sx>=X&&sy>=Y) //3
     {
       rsx=X;
       rsy=Y;
       rex=sx;
       rey=sy;
     }
     else if(sx>=X&&sy<=Y)  //2
     {
       rsx=X;
       rsy=sy;
       rex=sx;
       rey=Y;
     }

        if(rsy<(InfoRectHeight+InfoRectTopWidth))
        {
           if((rex-rsx)<InfoRectWidth &&(rey-rsy)<(InfoRectHeight+InfoRectTopWidth))//��Ϣ��ʾ�����ڲ�---���������������Ƚϸ��� ������ʾ��Ϣ�� ������ʱ�Ļ�ɫ��ʾ��
           {
             if((rey-rsy)<=InfoRectTopWidth)
             {
               if((MaxRight-rsx)<InfoRectWidth)
               {
                BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                 InfoSx=rsx-InfoRectWidth;
               }
               else
               {
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
                 InfoSx=rsx;
               }
             }
             else
             {
              if((MaxRight-rsx)<InfoRectWidth)
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                InfoSx=rsx-InfoRectWidth;
              }
              else
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,(rex-rsx),(rey-rsy-InfoRectTopWidth),Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);//��1��
               BitBlt(BufferBmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,rsx+InfoRectWidth-rex,(rey-rsy-InfoRectTopWidth),GrayGraybmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,SRCCOPY);//��1��
               BitBlt(BufferBmp->Canvas->Handle,rsx,rey,InfoRectWidth,(InfoRectHeight+InfoRectTopWidth)-(rey-rsy),GrayGraybmp->Canvas->Handle,rsx,rey,SRCCOPY);
               InfoSx=rsx;
              }
             }       //��2��

           }
           else if((rex-rsx)<InfoRectWidth &&(rey-rsy)>=(InfoRectHeight+InfoRectTopWidth))
            {
              if((MaxRight-rsx)<InfoRectWidth)
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                InfoSx=rsx-InfoRectWidth;
              }
              else
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,(rex-rsx),InfoRectHeight,Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
               BitBlt(BufferBmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,InfoRectWidth-(rex-rsx),InfoRectHeight,GrayGraybmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,SRCCOPY);
               InfoSx=rsx;
              }
            }
           else if((rex-rsx)>=InfoRectWidth &&(rey-rsy)<(InfoRectHeight+InfoRectTopWidth))
            {

             if((MaxRight-rsx)<InfoRectWidth)
             {
              BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
               InfoSx=rsx-InfoRectWidth;
             }
             else
             {
               if((rey-rsy)<=InfoRectTopWidth)
               {
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
               }
                else
               {
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,rey-rsy-InfoRectTopWidth,Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rey,InfoRectWidth,rsy-rey+(InfoRectHeight+InfoRectTopWidth),GrayGraybmp->Canvas->Handle,rsx,rey,SRCCOPY);
               }
               InfoSx=rsx;
             }
            }
           else
            {
              if((MaxRight-rsx)<InfoRectWidth)
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                InfoSx=rsx-InfoRectWidth;
              }
              else
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
               InfoSx=rsx;
              }
            }
            //<(InfoRectHeight+InfoRectTopWidth)ʱ�Ұ���Ϣ��ʾ�����ʼλ��
               InfoSy=rsy+InfoRectTopWidth;// ��¼InfoRect��Ϣ�����ʼ��λ��

        }
        else       //rsy>InfoRectHeight+InfoRectTopHeight
        {
          if((MaxRight-rsx)<InfoRectWidth)
          {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy-(InfoRectHeight+InfoRectTopWidth),InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy-(InfoRectHeight+InfoRectTopWidth),SRCCOPY);
               InfoSx=rsx-InfoRectWidth;
          }
          else
          {
           BitBlt(BufferBmp->Canvas->Handle,rsx,rsy-(InfoRectHeight+InfoRectTopWidth),InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx,rsy-(InfoRectHeight+InfoRectTopWidth),SRCCOPY);
             InfoSx=rsx;
          }
            //>(InfoRectHeight+InfoRectTopWidth)ʱ�Ұ���Ϣ��ʾ�����ʼλ��
            InfoSy=rsy-(InfoRectHeight+InfoRectTopWidth); //InfoSxδ��� �ض�=rsx
        }


   SetBkMode(BufferBmp->Canvas->Handle,TRANSPARENT);
   SetTextColor(BufferBmp->Canvas->Handle,InfoTextColor);
   DrawInfo(BufferBmp->Canvas,InfoSx,InfoSy,rex-rsx,rey-rsy,rsx,rsy,X,Y);
   DrawTracker(BufferBmp->Canvas,sx,sy,X,Y);
   BitBlt(Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
   lx=X;
   ly=Y;
  }
  else
  {
   BitBlt(BufferBmp->Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);//ȫͼBitBlt
  // BitBlt(BufferBmp->Canvas->Handle,sx-2,sy-2,lx-sx+4,ly-sy+4,Graybmp->Canvas->Handle,sx-2,sy-2,SRCCOPY);//����BitBlt
   BitBlt(BufferBmp->Canvas->Handle,sx,sy,X-sx,Y-sy,Srcbmp->Canvas->Handle,sx,sy,SRCCOPY);
     if(sx<=X&&sy<=Y)      //1
     {
       rsx=sx;
       rsy=sy;
       rex=X;
       rey=Y;
     }
     else if(sx<=X&&sy>=Y) //4
     {
       rsx=sx;
       rsy=Y;
       rex=X;
       rey=sy;
     }
     else if(sx>=X&&sy>=Y) //3
     {
       rsx=X;
       rsy=Y;
       rex=sx;
       rey=sy;
     }
     else if(sx>=X&&sy<=Y)  //2
     {
       rsx=X;
       rsy=sy;
       rex=sx;
       rey=Y;
     }
     //--------------------------�����Ұ���Ϣ��ʾ��-------------------------

        if(rsy<(InfoRectHeight+InfoRectTopWidth))
        {
           if((rex-rsx)<InfoRectWidth &&(rey-rsy)<(InfoRectHeight+InfoRectTopWidth))//��Ϣ��ʾ�����ڲ�---���������������Ƚϸ��� ������ʾ��Ϣ�� ������ʱ�Ļ�ɫ��ʾ��
           {
             if((rey-rsy)<=InfoRectTopWidth)
             {
               if((MaxRight-rsx)<InfoRectWidth)
               {
                BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                 InfoSx=rsx-InfoRectWidth;
               }
               else
               {
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
                 InfoSx=rsx;
               }
             }
             else
             {
              if((MaxRight-rsx)<InfoRectWidth)
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                InfoSx=rsx-InfoRectWidth;
              }
              else
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,(rex-rsx),(rey-rsy-InfoRectTopWidth),Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);//��1��
               BitBlt(BufferBmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,rsx+InfoRectWidth-rex,(rey-rsy-InfoRectTopWidth),GrayGraybmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,SRCCOPY);//��1��
               BitBlt(BufferBmp->Canvas->Handle,rsx,rey,InfoRectWidth,(InfoRectHeight+InfoRectTopWidth)-(rey-rsy),GrayGraybmp->Canvas->Handle,rsx,rey,SRCCOPY);
               InfoSx=rsx;
              }
             }       //��2��

           }
           else if((rex-rsx)<InfoRectWidth &&(rey-rsy)>=(InfoRectHeight+InfoRectTopWidth))
            {
              if((MaxRight-rsx)<InfoRectWidth)
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                InfoSx=rsx-InfoRectWidth;
              }
              else
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,(rex-rsx),InfoRectHeight,Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
               BitBlt(BufferBmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,InfoRectWidth-(rex-rsx),InfoRectHeight,GrayGraybmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,SRCCOPY);
               InfoSx=rsx;
              }
            }
           else if((rex-rsx)>=InfoRectWidth &&(rey-rsy)<(InfoRectHeight+InfoRectTopWidth))
            {

             if((MaxRight-rsx)<InfoRectWidth)
             {
              BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
               InfoSx=rsx-InfoRectWidth;
             }
             else
             {
               if((rey-rsy)<=InfoRectTopWidth)
               {
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
               }
                else
               {
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,rey-rsy-InfoRectTopWidth,Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rey,InfoRectWidth,rsy-rey+(InfoRectHeight+InfoRectTopWidth),GrayGraybmp->Canvas->Handle,rsx,rey,SRCCOPY);
               }
               InfoSx=rsx;
             }
            }
           else
            {
              if((MaxRight-rsx)<InfoRectWidth)
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                InfoSx=rsx-InfoRectWidth;
              }
              else
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
               InfoSx=rsx;
              }
            }
            //<(InfoRectHeight+InfoRectTopWidth)ʱ�Ұ���Ϣ��ʾ�����ʼλ��
               InfoSy=rsy+InfoRectTopWidth;// ��¼InfoRect��Ϣ�����ʼ��λ��

        }
        else       //rsy>InfoRectHeight+InfoRectTopHeight
        {
          if((MaxRight-rsx)<InfoRectWidth)
          {

               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy-(InfoRectHeight+InfoRectTopWidth),InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy-(InfoRectHeight+InfoRectTopWidth),SRCCOPY);
               InfoSx=rsx-InfoRectWidth;
          }
          else
          {
           BitBlt(BufferBmp->Canvas->Handle,rsx,rsy-(InfoRectHeight+InfoRectTopWidth),InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx,rsy-(InfoRectHeight+InfoRectTopWidth),SRCCOPY);
             InfoSx=rsx;
          }
            //>(InfoRectHeight+InfoRectTopWidth)ʱ�Ұ���Ϣ��ʾ�����ʼλ��
            InfoSy=rsy-(InfoRectHeight+InfoRectTopWidth); //InfoSxδ��� �ض�=rsx
        }


     SetBkMode(BufferBmp->Canvas->Handle,TRANSPARENT);
     SetTextColor(BufferBmp->Canvas->Handle,InfoTextColor);
     DrawInfo(BufferBmp->Canvas,InfoSx,InfoSy,rex-rsx,rey-rsy,rsx,rsy,X,Y);
     DrawTracker(BufferBmp->Canvas,sx,sy,X,Y);
     BitBlt(Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
     lx=X;
     ly=Y;
  }

}
else
{

 ::SetCursor(TB_WAVE);
}
    HaveBeenMove=true;
}


//-----------------------------------------------------------------------------
_Operation::_Operation(Operation_Type type, TColor ca,int a,int b,int c,int d)
{   //type= 0 ��1�� 4  Ǧ��&��ˢ����ͷ��ֱ�ߣ�
  op_type=type;
  sx=a;
  sy=b;
  ex=c;
  ey=d;
  PenColor=ca;
}
//--------------------------------------------------------------------------------------------
_Operation::_Operation(Operation_Type type, TColor ca,TColor cb,int a,int b,int c,int d)
{      //type : 2,3     ��Բ������
  op_type=type;
  sx=a;
  sy=b;
  ex=c;
  ey=d;
  PenColor=ca;
  FillColor=cb;
}
//--------------------------------------------------------------------------------------------
_Operation::_Operation(TColor ca,int a,int b,int size)
{      //type=5
  op_type=Text_5;
  sx=a;
  sy=b;
  TextSize=size;
}
//-----------------------------------------------------------------------------------------------
_Operation::~_Operation()
{
   ;
}
//--------------  Operation Excute();------------------------
void _Operation::Excute()
{

}
//------------------------------------------------------------------------------
void TForm1::TrimCutRect(unsigned short X,unsigned short Y)
{
   if(ondrawing)//���ڻ�ͼ           ���������ڱ༭�ı�
   {
        MouseHasMove=true;
        if(X>= CutRect.left&&Y>=CutRect.top&&X<=CutRect.right&&Y<=CutRect.bottom)
        {   //�����CutRect��
             ::SetCursor(TB_DRAW);
             if(CurrentMode==PenBrush_0)
             {
                 DrawBmp->Canvas->MoveTo(dlx,dly);
                 DrawBmp->Canvas->LineTo(X,Y);
                 dlx=X;
                 dly=Y;
             }
             else if(CurrentMode==Arrow_1)
             {
                 BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
                 tbDrawArrow(DrawBmp->Canvas,dlx,dly,X,Y,ArrowHigh,ArrowBottom);
             }
             else if(CurrentMode==Round_2)
             {
                if(fillable)
                { //��Ҫ���
                  BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
                  DrawBmp->Canvas->Ellipse(dlx,dly,X,Y);
                }
                else
                { //����Ҫ���
                  BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
                  DrawBmp->Canvas->Arc(dlx,dly,X,Y,dlx,dly,dlx,dly);
                }
             }
             else if(CurrentMode==Rectangle_3)
             {  // �� �� ��
                   if(fillable)
                    {      //��Ҫ���
                      BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
                      DrawBmp->Canvas->Rectangle(dlx,dly,X,Y);
                    }
                   else
                   {
                      //����Ҫ���
                       BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
                       DrawRectLine(DrawBmp->Canvas,dlx,dly,X,Y);

                    }
             }
             else if(CurrentMode==Line_4)
             {
                  // �� ֱ��
                      BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
                      DrawBmp->Canvas->MoveTo(dlx,dly);
                      DrawBmp->Canvas->LineTo(X,Y);
             }
             else if(CurrentMode==Text_5)
             {    //do nothing  ������ĸ��´��������Դ���
                     // BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
             }

        }
        else
        {   //��CutRect��
             ::SetCursor(TB_WAVE);
           //����dlx dly
              if(CurrentMode==PenBrush_0)
               {
                   Inlize(X,Y,dlx,dly);
               }
              else if(CurrentMode==Arrow_1)
               {
                  int tx,ty;//�淶������ͼ��������
                   BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
                   Inlize(X,Y,tx,ty);
                   tbDrawArrow(DrawBmp->Canvas,dlx,dly,tx,ty,ArrowHigh,ArrowBottom);
               }
              else if(CurrentMode==Round_2)
               {         //�� ��Բ
                  if(fillable)
                   {      //��Ҫ���
                     int tx,ty;//�淶������ͼ��������
                     BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
                     Inlize(X,Y,tx,ty);
                     DrawBmp->Canvas->Ellipse(dlx,dly,tx,ty);
                   }
                   else
                   {
                    //����Ҫ���
                     int tx,ty;//�淶������ͼ��������
                     BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
                     Inlize(X,Y,tx,ty);
                     DrawBmp->Canvas->Arc(dlx,dly,tx,ty,dlx,dly,dlx,dly);

                    }
               }
              else if(CurrentMode==Rectangle_3)
               {     // �� �� ��
                   if(fillable)
                    {      //��Ҫ���
                      int tx,ty;//�淶������ͼ��������
                      BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
                      Inlize(X,Y,tx,ty);
                      DrawBmp->Canvas->Rectangle(dlx,dly,tx,ty);
                    }
                   else
                   {
                      //����Ҫ���
                       int tx,ty;//�淶������ͼ��������
                       BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp ->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
                       Inlize(X,Y,tx,ty);
                       DrawRectLine(DrawBmp->Canvas,dlx,dly,tx,ty);
                    }
                }
               else if(CurrentMode==Line_4)
               {
                  // �� ֱ��
                      int tx,ty;//�淶������ͼ��������
                      BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
                      DrawBmp->Canvas->MoveTo(dlx,dly);
                      Inlize(X,Y,tx,ty);
                      DrawBmp->Canvas->LineTo(X,Y);
               }
               else if(CurrentMode==Text_5)
               {
                       //do nothing  ������ĸ��´��������Դ���
                  //    BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
    
               }

      }
      //����RGB��Ϣ
      if(InfoRectHidden)
      {  //��Ϣ������
          if(Memo1->Visible)   //��ʱBufferBmp����  ʹ��BufferBmp2��Ϊ��������
          {

            BitBlt(BufferBmp2->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),BufferBmp->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
            MemoLine(BufferBmp2->Canvas);
            BitBlt(Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),BufferBmp2->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
          }
          else
          {

            BitBlt(BufferBmp->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),DrawBmp->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
            DrawTracker(BufferBmp->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);
            BitBlt(Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),BufferBmp->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
          }
      }
     else
      { //��Ϣ��δ����

          //��InfoBmp�ϰ���Ϣ�򻨺�
          BitBlt(InfoBmp->Canvas->Handle,0,0,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,InfoSx,InfoSy,SRCCOPY);
          SetBkMode(InfoBmp->Canvas->Handle,TRANSPARENT);
          SetTextColor(InfoBmp->Canvas->Handle,InfoTextColor);
          DrawInfo(InfoBmp->Canvas,0,0,CutRect.Width(),CutRect.Height(),CutRect.left,CutRect.top,X,Y);
          if(Memo1->Visible)   //��ʱBufferBmp����   ��BufferBmp2����Ŵ�Memo����ߵĻ���ͼ
          {
             //��InfoBmp ��DrawBmp�ϵ���Ч���򶼿�����BufferBmp2
             BitBlt(BufferBmp2->Canvas->Handle,InfoSx,InfoSy,CutRect.Width()+InfoRectWidth,CutRect.Height()+InfoRectHeight+InfoRectTopWidth,BufferBmp->Canvas->Handle,InfoSx,InfoSy,SRCCOPY);
             BitBlt(BufferBmp2->Canvas->Handle,InfoSx,InfoSy,InfoRectWidth,InfoRectHeight,InfoBmp->Canvas->Handle,0,0,SRCCOPY);
            // �Ѿ��ο򻭺�
             DrawTracker(BufferBmp2->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);

            //����Memo�����
             MemoLine(BufferBmp2->Canvas);
            //��BufferBmp�ϵ���Ч���򿽱���Form1�����Canvas
             BitBlt(Canvas->Handle,InfoSx,InfoSy,CutRect.Width()+InfoRectWidth,CutRect.Height()+InfoRectHeight+InfoRectTopWidth,BufferBmp2->Canvas->Handle,InfoSx,InfoSy,SRCCOPY);


          }
          else //BufferBmp��ʱ�ı�
          {
             //��InfoBmp ��DrawBmp�ϵ���Ч���򶼿�����BufferBmp
             BitBlt(BufferBmp->Canvas->Handle,InfoSx,InfoSy,InfoRectWidth,InfoRectHeight,InfoBmp->Canvas->Handle,0,0,SRCCOPY);
             BitBlt(BufferBmp->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),DrawBmp->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
            // �Ѿ��ο򻭺�
             DrawTracker(BufferBmp->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);

            //��BufferBmp�ϵ���Ч���򿽱��������Canvas
             BitBlt(Canvas->Handle,InfoSx,InfoSy,CutRect.Width()+InfoRectWidth,CutRect.Height()+InfoRectHeight+InfoRectTopWidth,BufferBmp->Canvas->Handle,InfoSx,InfoSy,SRCCOPY);

          }
      }
   }
   else     //���ڻ�ͼ
   {
      if(FuncButtonDown)
      {
       if(X>= CutRect.left&&Y>=CutRect.top&&X<=CutRect.right&&Y<=CutRect.bottom)
       {   //�����CutRect��
         ::SetCursor(TB_DRAW);
       }
       else
       {   //��CutRect��
         ::SetCursor(TB_WAVE);
       }

       if(!InfoRectHidden)
       {
       //����RGB��Ϣ
        BitBlt(InfoBmp->Canvas->Handle,0,0,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,InfoSx,InfoSy,SRCCOPY);
        SetBkMode(InfoBmp->Canvas->Handle,TRANSPARENT);
        SetTextColor(InfoBmp->Canvas->Handle,InfoTextColor);
        DrawInfo(InfoBmp->Canvas,0,0,CutRect.Width(),CutRect.Height(),CutRect.left,CutRect.top,X,Y);
        BitBlt(Canvas->Handle,InfoSx,InfoSy,InfoRectWidth,InfoRectHeight,InfoBmp->Canvas->Handle,0,0,SRCCOPY);
       }
      }
      else
      {
        if(X>= CutRect.left&&Y>=CutRect.top&&X<=CutRect.right&&Y<=CutRect.bottom)
        {
           Form1->Cursor=crDefault; //����һ�������ʽ TB_FINISH
        }
        else
        {
           ::SetCursor(TB_WAVE);
        }
      }
   }

}

//----------------------------------------------------------------------------------
inline void TForm1::CreateCutRectMouseLeftUp(unsigned short X,unsigned short Y)
{
if(drawable)//��������ͼ��
   {
     drawable=false;//���ٻ�ͼ�ˣ�mousemove��������µ�����
     if(X==sx||Y==sy)//û�в�����������
     {
       if(X==sx&&Y==sy)  //����ȣ��൱��һ���㡣
       {
         if(!HaveBeenMove)//δ�ƶ���ԭ�ص���
         {
          oncuting=false;
          BitBlt(Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);
          CutRect=Rect(0,0,0,0) ;
          return;
         }
       }
       else //ĳ�����غ�
       {
       if(!CanDrawZeroRect)//�����Ի�������  ;//�ָ��Ҷ�ͼ
       {  oncuting=false;  //ȥ����������༭״̬
       // if(!(X==sx&&Y==sy))//����������ʱ����������ͼ �����ָ��Ҷ�ͼ��
          BitBlt(Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);
          CutRect=Rect(0,0,0,0) ;
          Form1->Cursor=crDefault;
          return;
       }
       }
     }
     //���Ի�������
       CutRect=GetCurRect(sx,sy,X,Y) ;
       oncuting=true; //�������ֽ�������༭״̬

     //���� ���ĳ���
     if(!CanToolBarVisible)
     return;
            if((MaxRight-CutRect.right)<149)    //����ұ߲���һ��MainPanel
           {
             if(CutRect.left<149)          //������Ҳ����
             {
                if((MaxBottom-CutRect.top)<173)  //�ײ���ͷ��
                  {
                      Form1->MainPanel->Left=CutRect.right-145;
                      Form1->MainPanel->Top=MaxBottom-173;
                  }
                  else
                  {
                      Form1->MainPanel->Left=CutRect.right-145;
                      Form1->MainPanel->Top =CutRect.top;
                  }
             }
             else                          //����߹� �������
             {
               if((MaxRight-CutRect.left)<InfoRectWidth && CutRect.top<(InfoRectHeight+InfoRectTopWidth)) //��������Ϣ��������� �����˿���±�����
               {
                 Form1->MainPanel->Left=CutRect.left -149;
                 Form1->MainPanel->Top=InfoSy+InfoRectHeight+5;
               }
               else if((MaxRight-CutRect.left)<InfoRectWidth)  //��Ϣ������ߣ��һ��ڿ�����ϲ�
               {
                 if((MaxBottom-CutRect.top)<173)  //�ײ���ͷ��
                  {
                      Form1->MainPanel->Left=CutRect.left -InfoRectWidth-149;
                      Form1->MainPanel->Top=MaxBottom-173;
                  }
                 else            //�ײ�û��ͷ
                  {

                    Form1->MainPanel->Left=CutRect.left -149;
                    Form1->MainPanel->Top=CutRect.top;
                  }
               }
               else  //��Ϣ�����˿�����Ϸ�
               {
                 if((MaxBottom-CutRect.top)<173)  //�ײ���ͷ��
                  {
                      Form1->MainPanel->Left=CutRect.left-149;
                      Form1->MainPanel->Top=MaxBottom-173;
                  }
                 else
                  {
                    Form1->MainPanel->Left=CutRect.left -149;
                    Form1->MainPanel->Top=CutRect.top;
                  }
               }
             }
           }
           else    //����ұ߹�һ��MainPanel
           {
               if(CutRect.top<(InfoRectHeight+InfoRectTopWidth))//��Ϣ������
                {
                    Form1->MainPanel->Left=CutRect.right+5;
                    if((CutRect.Width()+4)<InfoRectWidth)
                    Form1->MainPanel->Top=CutRect.top+InfoRectTopWidth+InfoRectHeight+5;
                    else
                    Form1->MainPanel->Top=CutRect.top;              }
                else
                {
                  if((MaxBottom-CutRect.top)<173)  //�ײ���ͷ��
                    {
                      if(CutRect.Width()<InfoRectWidth)
                        Form1->MainPanel->Left=CutRect.left-149;
                      else
                        Form1->MainPanel->Left=CutRect.right+5;
                      Form1->MainPanel->Top=MaxBottom-173;
                    }
                   else
                    {
                      Form1->MainPanel->Left=CutRect.right+5;
                      Form1->MainPanel->Top=CutRect.top;
                    }
                }
           }

       Form1->MainPanel->Show();
   }
   else if(dragable)//����������Ĵ���ͼ ���� ��������Ҫ���¸���CutRect����mousemove��ͼ���Ѿ�����
   {
     dragable=false;
     BitBlt(Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
    if(!CanToolBarVisible)  //��岻�ɼ�
     return;
     //���ɼ�
           if((MaxRight-CutRect.right)<149)    //����ұ߲���һ��MainPanel
           {
             if(CutRect.left<149)          //������Ҳ����
             {
                if((MaxBottom-CutRect.top)<173)  //�ײ���ͷ��
                  {
                      Form1->MainPanel->Left=CutRect.right-145;;
                      Form1->MainPanel->Top=MaxBottom-173;
                  }
                  else
                  {
                      Form1->MainPanel->Left=CutRect.right-145;
                      Form1->MainPanel->Top =CutRect.top;
                  }
             }
             else                          //����߹� �������
             {
               if((MaxRight-CutRect.left)<InfoRectWidth && CutRect.top<(InfoRectHeight+InfoRectTopWidth)) //��������Ϣ��������� �����˿���±�����
               {

                 Form1->MainPanel->Left=CutRect.left -149;
                 Form1->MainPanel->Top=InfoSy+InfoRectHeight+5;
               }
               else if((MaxRight-CutRect.left)<InfoRectWidth)  //��Ϣ������ߣ��һ��ڿ�����ϲ�
               {
                 if((MaxBottom-CutRect.top)<173)  //�ײ���ͷ��
                  {

                      Form1->MainPanel->Left=CutRect.left -InfoRectWidth-149;
                      Form1->MainPanel->Top=MaxBottom-173;
                  }
                 else            //�ײ�û��ͷ
                  {

                    Form1->MainPanel->Left=CutRect.left -149;
                    Form1->MainPanel->Top=CutRect.top;
                  }
               }
               else  //��Ϣ�����˿�����Ϸ�
               {
                 if((MaxBottom-CutRect.top)<173)  //�ײ���ͷ��
                  {

                      Form1->MainPanel->Left=CutRect.left-149;
                      Form1->MainPanel->Top=MaxBottom-173;
                  }
                 else
                  {

                    Form1->MainPanel->Left=CutRect.left -149;
                    Form1->MainPanel->Top=CutRect.top;
                  }
               }
             }
           }
           else    //����ұ߹�һ��MainPanel
           {
                if(CutRect.top<(InfoRectHeight+InfoRectTopWidth))//��Ϣ������
                {
                    Form1->MainPanel->Left=CutRect.right+5;
                    if((CutRect.Width()+4)<InfoRectWidth)
                    Form1->MainPanel->Top=CutRect.top+InfoRectTopWidth+InfoRectHeight+5;
                    else
                    Form1->MainPanel->Top=CutRect.top;                }
                else
                {
                  if((MaxBottom-CutRect.top)<173)  //�ײ���ͷ��
                    {
                      if(CutRect.Width()<InfoRectWidth)
                        Form1->MainPanel->Left=CutRect.left-149;
                      else
                        Form1->MainPanel->Left=CutRect.right+5;
                      Form1->MainPanel->Top=MaxBottom-173;
                    }
                   else
                    {
                      Form1->MainPanel->Left=CutRect.right+5;
                      Form1->MainPanel->Top=CutRect.top;
                    }
                }
           }

      UpdateWindow(Form1->Handle);

   }
   else if(directable)//��� ��ֱ����ˮƽ �����ͼ ����  ��������Ҫ���¸���CutRect����mousemove��ͼ���Ѿ�����
   {
     directable=false;
     if(X==sx||Y==sy)//û�в�����Ч������������Ϊһ���ߣ�������һ����
     {
       if(!CanDrawZeroRect)//�����Ի�������   //�ָ��Ҷ�ͼ
       {
        BitBlt(Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);
        CutRect=Rect(0,0,0,0) ;
        oncuting=false; //���ڽ��������У���Ϊ����������ʧ
        Form1->Cursor=crDefault;
       }
        return;
     }
     //������� �ɼ����
     if(!CanToolBarVisible)  //��岻�ɼ�
     return;
     //���ɼ�
             if((MaxRight-CutRect.right)<149)    //����ұ߲���һ��MainPanel
           {
             if(CutRect.left<149)          //������Ҳ����
             {
               if((MaxBottom-CutRect.top)<173)  //�ײ���ͷ��
                  {
                      Form1->MainPanel->Left=CutRect.right-145;;
                      Form1->MainPanel->Top=MaxBottom-173;
                  }
                  else
                  {
                      Form1->MainPanel->Left=CutRect.right-145;
                      Form1->MainPanel->Top =CutRect.top;
                  }
             }
             else                          //����߹� �������
             {
               if((MaxRight-CutRect.left)<InfoRectWidth && CutRect.top<(InfoRectHeight+InfoRectTopWidth)) //��������Ϣ��������� �����˿���±�����
               {

                 Form1->MainPanel->Left=CutRect.left -149;
                 Form1->MainPanel->Top=InfoSy+InfoRectHeight+5;
               }
               else if((MaxRight-CutRect.left)<InfoRectWidth)  //��Ϣ������ߣ��һ��ڿ�����ϲ�
               {
                 if((MaxBottom-CutRect.top)<173)  //�ײ���ͷ��
                  {

                      Form1->MainPanel->Left=CutRect.left -InfoRectWidth-149;
                      Form1->MainPanel->Top=MaxBottom-173;
                  }
                 else            //�ײ�û��ͷ
                  {

                    Form1->MainPanel->Left=CutRect.left -149;
                    Form1->MainPanel->Top=CutRect.top;
                  }
               }
               else  //��Ϣ�����˿�����Ϸ�
               {
                 if((MaxBottom-CutRect.top)<173)  //�ײ���ͷ��
                  {

                      Form1->MainPanel->Left=CutRect.left-149;
                      Form1->MainPanel->Top=MaxBottom-173;
                  }
                 else
                  {

                    Form1->MainPanel->Left=CutRect.left -149;
                    Form1->MainPanel->Top=CutRect.top;
                  }
               }
             }
           }
           else    //����ұ߹�һ��MainPanel
           {
              if(CutRect.top<(InfoRectHeight+InfoRectTopWidth))//��Ϣ������
                {
                    Form1->MainPanel->Left=CutRect.right+5;
                    if((CutRect.Width()+4)<InfoRectWidth)
                    Form1->MainPanel->Top=CutRect.top+InfoRectTopWidth+InfoRectHeight+5;
                    else
                    Form1->MainPanel->Top=CutRect.top;                }
                else
                {
                  if((MaxBottom-CutRect.top)<173)  //�ײ���ͷ��
                    {
                      if(CutRect.Width()<InfoRectWidth)
                        Form1->MainPanel->Left=CutRect.left-149;
                      else
                        Form1->MainPanel->Left=CutRect.right+5;
                      Form1->MainPanel->Top=MaxBottom-173;
                    }
                   else
                    {
                      Form1->MainPanel->Left=CutRect.right+5;
                      Form1->MainPanel->Top=CutRect.top;
                    }
                }
           }

      Form1->MainPanel->Show();
   }
}



//--------------------------------------------------------------------------------------------
void Inlize(int X, int Y, int &dlx,int &dly)
{
  if(X<CutRect.left&&Y<CutRect.top)
        {
         dlx=CutRect.left;
         dly=CutRect.top;
        }
        else if(X<=CutRect.left&&Y>=CutRect.top&&Y<=CutRect.bottom)
        {
            dlx=CutRect.left;
            dly=Y;
        }
        else if(X<=CutRect.left&&Y>=CutRect.bottom)
        {
            dlx=CutRect.left;
            dly=CutRect.bottom;
        }
        else if(X>=CutRect.left&&X<=CutRect.right&&Y<=CutRect.top)
        {
            dlx=X;
            dly=CutRect.top;
        }
        else if(X>=CutRect.left&&X<=CutRect.right&&Y>=CutRect.bottom)
        {
            dlx=X;
            dly=CutRect.bottom;
        }
        else if(X>=CutRect.right&&Y<=CutRect.top)
        {
            dlx=CutRect.right;
            dly=CutRect.top;
        }
        else if(X>=CutRect.right&&Y>=CutRect.top&&Y<=CutRect.bottom)
        {
            dlx=CutRect.right;
            dly=Y;
        }
        else if(X>=CutRect.right&&Y>=CutRect.bottom)
        {
            dlx=CutRect.right;
            dly=CutRect.bottom;
        }
  }
void __fastcall TForm1::FillableClick(TObject *Sender)
{
 if(Fillable->Checked)
 fillable=true;
 else
 fillable=false;        
}
//---------------------------------------------------------------------------
void  DrawRectLine(TCanvas*canvas,int sx,int sy,int ex,int ey)
{
  canvas->MoveTo(sx,sy);
  canvas->LineTo(ex,sy);
  canvas->LineTo(ex,ey);
  canvas->LineTo(sx,ey);
  canvas->LineTo(sx,sy);
}

//-----------------------------------------------------------------


void __fastcall TForm1::SizeEditChange(TObject *Sender)
{
 if(!SizeEdit->Text.IsEmpty())
 {

    if(CurrentMode!=Text_5)
    {//��д��
     DrawWidth=ToData(SizeEdit->Text);
     if(DrawWidth>MaxPenSize)
     {
        SizeEdit->Text=AnsiString(MaxPenSize);
     }
      else if(DrawWidth<MinPenSize)
       {
         SizeEdit->Text=IntToStr(MinPenSize);
       }
    }
    else if(!Memo1->Visible)
    {
      TextSize=ToData(SizeEdit->Text);
      Memo1->Font->Size=TextSize;
    }
    else if(Memo1->Visible)
    {//д��
       TextSize=ToData(SizeEdit->Text);
       if(TextSize>MaxFontSize)
       {
         SizeEdit->Text=AnsiString(MaxFontSize);
         TextSize=MaxFontSize;
       }
       else if(TextSize<MinFontSize)
       {
         SizeEdit->Text=AnsiString(MinFontSize);
         TextSize=MinFontSize;        //8Ϊ��С����
       }
       Form1->Canvas->Font->Size=TextSize;
       int charnum=int(Memo1->Width/BaseWidth);
       int linenum=int(Memo1->Height/(0-Memo1->Font->Height));
       int temp2;
       BaseWidth=Canvas->TextWidth("2");
        temp2=Memo1->Left+BaseWidth*charnum<CutRect.right?BaseWidth*charnum:CutRect.right-Memo1->Left-1;
       if(temp2>Memo1->Width)
         Memo1->Width=temp2;
       //update height of memo
       temp2=Memo1->Top+linenum*(0-Canvas->Font->Height)<CutRect.bottom?linenum*(0-Canvas->Font->Height):CutRect.bottom-Memo1->Top-1;
       if(temp2>Memo1->Height)
         Memo1->Height=temp2;
       MaxCharPerLine=int(Memo1->Width/BaseWidth);
       MaxLineCount=int(Memo1->Height/(0-Form1->Canvas->Font->Height));
       CharCount=Memo1->Text.Length();
       char *text=new char[Memo1->Text.Length()+1];
       int i=0;               
       int CurCharPerLine=0;
       int linecount=1;    //һ��ʼ����һ�С�������һ����ĸ
       for(;i<Memo1->Text.Length();i++)
       {
          if(*(Memo1->Text.c_str()+i)=='\r'||CurCharPerLine>=MaxCharPerLine)
           {//�Զ����к��ֶ����м��
               if(linecount<MaxLineCount)
                {
                  CurCharPerLine=0;
                  linecount++;
                  text[i]=*(Memo1->Text.c_str()+i);
                  i++; //jump cross the '\n'
                  text[i]=*(Memo1->Text.c_str()+i);
                }
               else
                   break;
           }
           else
           {
                text[i]=*(Memo1->Text.c_str()+i);
                CurCharPerLine++;
           }
       }
       text[i]='\0';
       //ģ�����롣
       // char *temp=new char[MaxCharPerLine*MaxLineCount;
       Memo1->Text=AnsiString(text);
       Memo1->Font->Size=TextSize;
       Memo2->Font->Size=TextSize;
       BitBlt(BufferBmp2->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),BufferBmp->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
       MemoLine(BufferBmp2->Canvas);
       BitBlt(Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),BufferBmp2->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);

    }
 }
 else
 SizeEdit->Text=IntToStr(DrawWidth);
}
//---------------------------------------------------------------------------
void TForm1::tbDrawArrow(TCanvas*canvas,int sx,int sy,int ex,int ey,int h,int hb)   //h:high ; hb :half bottom
{
  canvas->MoveTo(sx,sy);
  canvas->LineTo(ex,ey);
  TPoint p[3];
  if(Len!=0)
  {

     if(ey==sy|| abs(ey-sy)<4)
      {
       p[0].x=int ( ex  );
       p[0].y=int ( ey  );
       p[1].x=ex>sx? int ( ex-h):int (ex+h);
       p[1].y=int ( ey-hb );
       p[2].x=ex>sx? int ( ex-h):int (ex+h);
       p[2].y=int ( ey+hb ) ;
         canvas->Polygon(p,2);
      }
     else if(ex==sx|| abs(ex-sx)<4)
     {

       p[0].x=int ( ex  );
       p[0].y=int ( ey  );
       p[1].x=int ( ex-hb);
       p[1].y=ey>sy? int ( ey-h ):int(ey+h);
       p[2].x=int ( ex+hb);
       p[2].y=ey>sy? int ( ey-h ):int(ey+h);
         canvas->Polygon(p,2);
     }
     else
      {
       try{
       p[0].x=int ( ex  );
       p[0].y=int ( ey  );
       p[1].x=int ( Ax+t_x );
       p[1].y=int ( Ay+t_y );
       p[2].x=int ( Ax-t_x );
       p[2].y=int ( Ay-t_y ) ;
         canvas->Polygon(p,2);
       }
       catch(...)
       {
       }
      }
 }


}


void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 //���Ʒ���  Key_left=37  Key_Down=40      Up->38   Right->39
 if(Triming||OperationCount>0)
 return;

 if(Key==37)
 {    //left
  if(Shift.Contains(ssCtrl))
  CutRectMove(-5,0);
  else
  CutRectMove(-1,0);
 }
 else if(Key==38)
 {    //up
  if(Shift.Contains(ssCtrl))
   CutRectMove(0,-5);
  else
   CutRectMove(0,-1);
 }
 else if(Key==39)
 {    //right
  if(Shift.Contains(ssCtrl))
   CutRectMove(5,0);
  else
   CutRectMove(1,0);
 }
 else if(Key==40)
 {    //down
  if(Shift.Contains(ssCtrl))
  CutRectMove(0,5);
  else
  CutRectMove(0,1);
 }
 else
 return;
 //��ʼ����¼
 BitBlt(RecordBmp[0]->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),Srcbmp->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
// DrawTracker(RecordBmp[0]->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);
 RecordPoint=0;
 RecordCount=1;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SizeEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 if(Key==38)
 {    //up
    if(CurrentMode!=Text_5)
     {
       int size_=ToData(SizeEdit->Text);
       if(size_<MaxPenSize)
        size_++;
        SizeEdit->Text=IntToStr(size_);
     }
     else
     {
        int size_=ToData(SizeEdit->Text);
        if(size_<MaxFontSize)
        size_++;
        SizeEdit->Text=(IntToStr(size_));//User_ToStr(size);
      }

 }
 else if(Key==40)
 {    //down
      if(CurrentMode!=Text_5)
        {
         int size=ToData(SizeEdit->Text);
         if(size>MinPenSize)
         size--;
         SizeEdit->Text=IntToStr(size);
        }
       else
        {
         int size=ToData(SizeEdit->Text);
         if(size>MinFontSize)
         size--;
         SizeEdit->Text=IntToStr(size);
        }
 }
}
//---------------------------------------------------------------------------



void __fastcall TForm1::LButMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
          if(CurrentMode==Text_5)
          {
           if(TextSize<MinFontSize)
           {
               SizeEdit->Text=IntToStr(MinFontSize);
               TextSize=MinFontSize;        //8Ϊ��С����
           }
          }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::RButMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
          if(CurrentMode==Text_5)
          {
           if(TextSize<MinFontSize)
           {
               SizeEdit->Text=IntToStr(MinFontSize);
               TextSize=MinFontSize;        //8Ϊ��С����
          }
          }

}
//---------------------------------------------------------------------------


void __fastcall TForm1::Memo1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  IsMemoMoving=true;
  firstmove=true;
  memosx=X;
  memosy=Y;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Memo1MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
   IsMemoMoving=false;
   UpdateWindow(Form1->Handle);
   MemoLine(Canvas);
}
//---------------------------------------------------------------------------
void TForm1::tbMove(TControl*control, int X,int Y)
{
  if((control->Left+X-memosx)>CutRect.left)
  {
    if((control->Left+X-memosx+control->Width)<CutRect.right-1)
    {
      control->Left+=(X-memosx);
    }
    else
    {
      control->Left=CutRect.right-control->Width-1;
    }
  }
  else
  {
   control->Left=CutRect.left+1;
  }

  if(control->Top+Y-memosy>CutRect.top)
  {
    if(control->Top+Y-memosy+control->Height<CutRect.bottom)
    {
       control->Top+=(Y-memosy);
    }
    else
    {
       control->Top=CutRect.bottom-control->Height-1;
    }
  }
  else
  {
       control->Top=CutRect.top+1;
  }

}

void __fastcall TForm1::Memo1MouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
 if(IsMemoMoving)
 {
  tbMove(Memo1,X,Y);
  if(MaxWidth)
  {
   MaxWidth=false;
  }

  Memo2->Font=Memo1->Font;
  Memo2->Font->Size=Memo1->Font->Size;
  Memo2->Width=CutRect.right-Memo1->Left-1;
  if(firstmove)
  {
  BitBlt(BufferBmp2->Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
  MemoLine(BufferBmp2->Canvas);
  BitBlt(Canvas->Handle,0,0,Width,Height,BufferBmp2->Canvas->Handle,0,0,SRCCOPY);

   firstmove=false;
  }
  else
  {
   firstmove=true;
  }

 }
 else
 {
  //�������
 }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Memo1Change(TObject *Sender)
{
  CharCount=Memo1->Text.Length();
  int MaxChar=MaxCharLine(Memo1->Text);
  if(!MaxWidth)
  {
   if(MaxChar>MaxCharInLine)
   {//���³� ���֣�����memo�Ŀ��
      MaxCharInLine=MaxChar;
      Memo1->Width=Memo1->Left+(MaxChar+4)*CHARWIDTH<CutRect.right?(MaxChar+4)*CHARWIDTH:CutRect.right-Memo1->Left-1;
      UpdateMaxChar(); //Memo�ĳ��Ϳ����˸ı䡣 ��Ӧ����������ַ������������ҲҪ�ı�
      MaxWidth=Memo1->Left+(MaxChar+4)*CHARWIDTH>=CutRect.right;
   }

  }
 if(Memo1->Lines->Count>memolinecount)
 {
    int tempwidth=Memo1->Top+(0-Memo1->Font->Height)*(Memo1->Lines->Count+1)<CutRect.bottom?(0-Memo1->Font->Height)*(Memo1->Lines->Count+1):CutRect.bottom-Memo1->Top-1;
    if(tempwidth>Memo1->Height)
    Memo1->Height=tempwidth;
    UpdateMaxChar();
 }
 memolinecount=Memo1->Lines->Count;

 UpdateWindow(Form1->Handle);//������µ���Ļ�ϣ���������Ϣѭ��
 MemoLine(Canvas);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Memo1KeyPress(TObject *Sender, char &Key)
{
 CurLine=SendMessage(Memo1->Handle,EM_LINEFROMCHAR,Memo1->SelStart,0);
 int Cpos=SendMessage(Memo1->Handle,EM_LINEINDEX,CurLine,0);
 int virtualcount=Memo1->Lines->Count;
 if(Memo1->Lines->Strings[CurLine].IsEmpty()&&*(Memo1->Text.c_str()+Memo1->SelStart)!='\r')//���о���Countû������������ȴ�Ǵ��ڣ�Ҳ���ǣ��б���ڣ�count ������,���������ǿմ�,���������û�� \r\n
 {
  virtualcount++;
 }
 int AbleCharPerLine=(CutRect.right-Memo1->Left-1)/Form1->Canvas->TextWidth('2');
 int AbleLineCount=(CutRect.bottom-Memo1->Top-1)/(0-Memo1->Font->Height);
 if(virtualcount>=AbleLineCount) //MaxLineCount
  {   //�����Ѿ��� ����������
    if(Key==13)     //�س�
    {

      if(CurLine==MaxLineCount-1)
      {
       MessageBeep(0); //��ֹ���������
        Key='\0';
      //  Button2->Caption="�����";
      }
      else if((*(Memo1->Text.c_str()+Cpos+Memo1->Lines->Strings[CurLine].Length())!='\r')//������������һ���ַ���Ϊ�س�
       &&((Memo1->SelStart-Cpos)<Memo1->Lines->Strings[CurLine].Length())  //�����ַ�����Ϊ�գ�������һ���������ǰ�ַ�������<���г���Strings[CurLine].Length()
       &&( (MaxCharPerLine-(Memo1->SelStart-Cpos)+Memo1->Lines->Strings[CurLine+1].Length())<=MaxCharPerLine))
      {      //������й���Ժ���ַ���+��һ�е��ַ�����ƴ��һ��(����һ��������)����ô����س���ǰ���ǡ�����Ժ���ַ�����Ϊ�գ�Ҳ��Ϊ�س�
        //  Button2->Caption="yes";
          Memo1->MaxLength=CharCount+2;
          Key=13;
      }
      else
      {
        MessageBeep(0);
        Key='\0';
      }
    }
    else if(Key==8||Key==24||Key==26)
    {              //�˸�������м���������
     // ����Ҫ��������
    }
    else if(Key==22)
    {
     AnsiString text=AnsiString(GetClipboardText());
     int temp=caninput(MaxLineCount,CurLine);
     if(!canpaste(text,temp))
     {
       Memo1->MaxLength=CharCount!=0?CharCount:1;
       MessageBeep(0);
     }

    }
    else
    {
      int can=caninput(MaxLineCount,CurLine);
      if(CharCount==0&&can==0)
      Key='\0';
      else
      {
       Memo1->MaxLength=can+CharCount;
       if(Memo1->MaxLength==CharCount)    //�����Ѿ����ˡ�����ж��Ƿ������������
         MessageBeep(0);
      }
    }
  }
  else
  {
   Memo1->MaxLength=0;
   if(Key==22)
   {                //����
      AnsiString text=Memo1->Lines->Strings[CurLine];
      text+=AnsiString(GetClipboardText());
      if(!canpaste2(text,AbleLineCount-virtualcount+1,AbleCharPerLine))//
      {
        if(CharCount)
        Memo1->MaxLength=CharCount;
        else
        Key='\0';
        MessageBeep(0);
      }
   }
  }
}
//---------------------------------------------------------------------------
int TForm1::caninput(int LineCount,int CurLine)
{
  if(Memo1->Lines->Strings[CurLine].Length()<MaxCharPerLine)
  {
     return  MaxCharPerLine-Memo1->Lines->Strings[CurLine].Length();
  }
  else   //����
  {
    int Cpos=SendMessage(Memo1->Handle,EM_LINEINDEX,CurLine,0);
    int i=0;
    char*p=(Memo1->Text.c_str()+Cpos);
    while(*p!='\0'&& *p!='\r'&&*(p+1)!='\n'&&i<MaxCharPerLine)
    {
       p++;
       i++;
    }
    if(*p=='\0')
    {   //�����������Ҳû�ѵ����س�����
         return    MaxCharPerLine-Memo1->Lines->Strings[Memo1->Lines->Count-1].Length();

    }
    else//p=='\r' or i==MaxCharPerLine
    {
        if(*p=='\r')
        {//���н����Ǿ��ǻس�
             return 0;
        }
        else//����û�лس�����ȴ������
        {   //������һ��
             return  caninput(LineCount,CurLine+1);
        }
    }
  }

}
//=======================================================================================
void TForm1::UpdateMaxChar()
{
   Form1->Canvas->Font->Size=Memo1->Font->Size;
   BaseWidth=Canvas->TextWidth("2");
   MaxCharPerLine=int(Memo1->Width/BaseWidth);
   MaxLineCount=int(Memo1->Height/(-Memo1->Font->Height));
   CharCount=Memo1->Text.Length();
   truecount=Memo1->Lines->Count;
}
//-------------------------------------------------------------------------------------
int TForm1::MaxCharLine(AnsiString text)
{
  int lastcount=0;
  int re=0;
  int p=0;
  while(*(text.c_str()+p)!='\0')
  {
     if(*(text.c_str()+p)!='\r')
    {
      re++;
      p++;
    }
    else
    {
     p++;
     p++;
     if(re>lastcount)
      lastcount=re;
      re=0;
    }
  }
  if(re>lastcount)
      lastcount=re;
  return lastcount;
}
//--------------------------------------------------------------------
//---------------------------------------------------------------------------
char * TForm1::GetClipboardText()
{
  ::OpenClipboard(NULL);
  HANDLE hClip = ::GetClipboardData(CF_TEXT);
  char* buffer=(char*)GlobalLock(hClip);

//  DWORD error = GetLastError();
  ::CloseClipboard();
   return buffer;
}
//---------------------------------------------------------------------------
bool TForm1::canpaste(AnsiString t,int d)
{
  char*p=t.c_str();
  int i=0;
  while(*p!='\0')
  {
     i++;
     if(*p=='\r')
      return false;
     if(i>d)
      return false;
     p++;
  }

  return true;
}
//-------------------------------------------------------------------------
bool TForm1::canpaste2(AnsiString t,int al,int w)//���� ճ���ı��������������������ÿ�е�����ַ���
{
   Memo2->Text=t;
   if(Memo2->Lines->Count<=al)
   return true;
   else
   return false;
}
//------------------------------------------------------------------------
void TForm1::MemoLine(TCanvas*canvas)
{
  TColor tempc=canvas->Pen->Color;
  int tempw=canvas->Pen->Width;
  canvas->Pen->Width=1;
  canvas->Pen->Color=clBlack;
  canvas->MoveTo(Memo1->Left-1>=0?Memo1->Left-1:0,Memo1->Top-1>=0?Memo1->Top-1:0);
  canvas->LineTo(Memo1->Left+Memo1->Width,Memo1->Top-1);
  canvas->LineTo(Memo1->Left+Memo1->Width,Memo1->Top+Memo1->Height);
  canvas->LineTo(Memo1->Left-1>=0?Memo1->Left-1:0,Memo1->Top+Memo1->Height);
  canvas->LineTo(Memo1->Left-1>=0?Memo1->Left-1:0,Memo1->Top-1>=0?Memo1->Top-1:0);
  canvas->Pen->Width=tempw;
  canvas->Pen->Color=tempc;
}
//------------------------------------------------------------------------
void TForm1::UserFormPaint()
{
 if(CurrentMode==Text_5||Exceptable)
 {
  if(!IsMemoMoving)
  {
    if(!Memo1->Text.IsEmpty())
    {
      // ��Ϊ�գ�����ʾ�����Ҵ����¼
       LastDrawBmp->Canvas->Font=Memo1->Font;
       //��¼������ǰ״̬ ����LastDrawBmp
       int i=0;
       AnsiString temp="";
       for(i=0;i<Memo1->Lines->Count;i++)
       {
         temp=Memo1->Lines->Strings[i];
         SetBkMode(LastDrawBmp->Canvas->Handle,TRANSPARENT);
         TextOut(LastDrawBmp->Canvas->Handle,Memo1->Left,Memo1->Top-i*Memo1->Font->Height,temp.c_str(),temp.Length());
       }
        BitBlt(BufferBmp->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
        DrawTracker(BufferBmp->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);
        Memo1->Clear();
       // BitBlt(Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
       //��¼�洢
       RecordPoint=(RecordPoint+1)%5;
       BitBlt(RecordBmp[RecordPoint]->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle, CutRect.left,CutRect.top,SRCCOPY);
       RecordCount=(RecordCount+1>5)?5:(RecordCount+1);
       OperationCount++; //�����һ������
    }
    else
    {
         BitBlt(Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
  //  ShowMessage("��") ;
    }
  }
  else
  {//IsMemoMoving
     BitBlt(Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
  }
 }

  
}
//-----------------------------------------------------------------------------------------------------------------
void TForm1::ReShowInitial()
{
 BitBlt(BufferBmp->Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);
 BitBlt(Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
 memolinecount=0;  //��ʼ��Ϊ0��
 MaxWidth=false;
 RecordCount=0;
 RecordPoint=-1;
 drawable=false;
 oncuting=false;
 dragable=false;
 directable=false;
 HaveBeenMove=false;
 Triming=false;
 InfoVisible=false;
 FuncButtonDown=false;
 Exceptable=false;
 CurrentMode=-1;//-1 Ϊ ���� ��ͼ��
 OperationCount=0;//��ʼû�л�ͼ��������ʼ��Ϊ0
 MouseHasMove=false;
 firstmove=true;
}
//-------------------------------------------------------------------------------------------------------------
void TForm1::CutRectMove( int x,int y)
{
     if(CutRect.left+x<=0)
       {
         CutRect.right=CutRect.right-CutRect.left;
         CutRect.left=0;
       }
       else if(CutRect.right+x>=MaxRight)
       {
         CutRect.left+=(MaxRight-CutRect.right) ;
         CutRect.right=MaxRight;
       }
       else
       {
         CutRect.left=CutRect.left+x;
         CutRect.right=CutRect.right+x;
       }

       if(CutRect.top+y<=0)
       {
         CutRect.bottom=CutRect.bottom-CutRect.top;
         CutRect.top=0;
       }
       else if(CutRect.bottom+y>=MaxBottom)
       {
         CutRect.top+=(MaxBottom-CutRect.bottom);
         CutRect.bottom=MaxBottom;
       }
       else
       {
         CutRect.top=CutRect.top+y;
         CutRect.bottom=CutRect.bottom+y;
       }
     //----��������-------                           HalfHandleSize
     BitBlt(BufferBmp->Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);//ȫͼBitBlt
    // BitBlt(BufferBmp->Canvas->Handle,CutRect.left-HalfHandleSize,CutRect.top-HalfHandleSize,CutRect.Width()+HandleSize,CutRect.Height()+HandleSize,Graybmp->Canvas->Handle,CutRect.left-HalfHandleSize,CutRect.top-HalfHandleSize,SRCCOPY);//����BitBlt
     BitBlt(BufferBmp->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),Srcbmp->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
    //-------------------�Ұ���ʾ�����---------------------
        rsx=CutRect.left;
        rsy=CutRect.top;
        rex=CutRect.right;
        rey=CutRect.bottom;
        if(rsy<(InfoRectHeight+InfoRectTopWidth))
        {
           if((rex-rsx)<InfoRectWidth &&(rey-rsy)<(InfoRectHeight+InfoRectTopWidth))//��Ϣ��ʾ�����ڲ�---���������������Ƚϸ��� ������ʾ��Ϣ�� ������ʱ�Ļ�ɫ��ʾ��
           {
             if((rey-rsy)<=InfoRectTopWidth)
             {
               if((MaxRight-rsx)<InfoRectWidth)
               {
                BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                 InfoSx=rsx-InfoRectWidth;
               }
               else
               {
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
                 InfoSx=rsx;
               }
             }
             else
             {
              if((MaxRight-rsx)<InfoRectWidth)
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
               InfoSx=rsx-InfoRectWidth;
              }
              else
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,(rex-rsx),(rey-rsy-InfoRectTopWidth),Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);//��1��
               BitBlt(BufferBmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,rsx+InfoRectWidth-rex,(rey-rsy-InfoRectTopWidth),GrayGraybmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,SRCCOPY);//��1��
               BitBlt(BufferBmp->Canvas->Handle,rsx,rey,InfoRectWidth,(InfoRectHeight+InfoRectTopWidth)-(rey-rsy),GrayGraybmp->Canvas->Handle,rsx,rey,SRCCOPY);
               InfoSx=rsx;
              }
             }       //��2��

           }
           else if((rex-rsx)<InfoRectWidth &&(rey-rsy)>=(InfoRectHeight+InfoRectTopWidth))
            {
              if((MaxRight-rsx)<InfoRectWidth)
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                InfoSx=rsx-InfoRectWidth;
              }
              else
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,(rex-rsx),InfoRectHeight,Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
               BitBlt(BufferBmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,InfoRectWidth-(rex-rsx),InfoRectHeight,GrayGraybmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,SRCCOPY);
               InfoSx=rsx;
              }
            }
           else if((rex-rsx)>=InfoRectWidth &&(rey-rsy)<(InfoRectHeight+InfoRectTopWidth))
            {

             if((MaxRight-rsx)<InfoRectWidth)
             {
              BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
               InfoSx=rsx-InfoRectWidth;
             }
             else
             {
               if((rey-rsy)<=InfoRectTopWidth)
               {
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
               }
                else
               {
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,rey-rsy-InfoRectTopWidth,Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
                 BitBlt(BufferBmp->Canvas->Handle,rsx,rey,InfoRectWidth,rsy-rey+(InfoRectHeight+InfoRectTopWidth),GrayGraybmp->Canvas->Handle,rsx,rey,SRCCOPY);
               }
               InfoSx=rsx;
             }
            }
           else
            {
              if((MaxRight-rsx)<InfoRectWidth)
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy+InfoRectTopWidth,SRCCOPY);
                InfoSx=rsx-InfoRectWidth;
              }
              else
              {
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,InfoRectWidth,InfoRectHeight,Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);
               InfoSx=rsx;
              }
            }
            //<(InfoRectHeight+InfoRectTopWidth)ʱ�Ұ���Ϣ��ʾ�����ʼλ��
               InfoSy=rsy+InfoRectTopWidth;// ��¼InfoRect��Ϣ�����ʼ��λ��

        }
        else       //rsy>InfoRectHeight+InfoRectTopHeight
        {
          if((MaxRight-rsx)<InfoRectWidth)
          {
               BitBlt(BufferBmp->Canvas->Handle,rsx-InfoRectWidth,rsy-(InfoRectHeight+InfoRectTopWidth),InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx-InfoRectWidth,rsy-(InfoRectHeight+InfoRectTopWidth),SRCCOPY);
               InfoSx=rsx-InfoRectWidth;
          }
          else
          {
           BitBlt(BufferBmp->Canvas->Handle,rsx,rsy-(InfoRectHeight+InfoRectTopWidth),InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,rsx,rsy-(InfoRectHeight+InfoRectTopWidth),SRCCOPY);
             InfoSx=rsx;
          }

          //>(InfoRectHeight+InfoRectTopWidth)ʱ�Ұ���Ϣ��ʾ�����ʼλ��
            InfoSy=rsy-(InfoRectHeight+InfoRectTopWidth); //InfoSxδ��� �ض�=rsx
        }


     SetBkMode(BufferBmp->Canvas->Handle,TRANSPARENT);
     SetTextColor(BufferBmp->Canvas->Handle,InfoTextColor);
     DrawInfo(BufferBmp->Canvas,InfoSx,InfoSy,CutRect.Width(),CutRect.Height(),rsx,rsy,sx,sy);//InfoSx��InfoSy����Ϣ�����ʼ��λ�� ��6����������Ϣ����
     DrawTracker(BufferBmp->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);
     BitBlt(Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
     sx=(sx+x>=0?(sx+x<=MaxRight?(sx+x):MaxRight):0);//������һ�λ�ͼ����ʱ�������λ�ͼ������λ��
     sy=(sy+y>=0?(sy+y<=MaxRight?(sy+y):MaxBottom):0);
    if(!CanToolBarVisible)  //��岻�ɼ�
     return;
     //���ɼ�
       if((MaxRight-CutRect.right)<149)    //����ұ߲���һ��MainPanel
           {
             if(CutRect.left<149)          //������Ҳ����
             {
                 if((MaxBottom-CutRect.top)<173)  //�ײ���ͷ��
                  {
                      Form1->MainPanel->Left=CutRect.right-145;;
                      Form1->MainPanel->Top=MaxBottom-173;
                  }
                  else
                  {
                      Form1->MainPanel->Left=CutRect.right-145;
                      Form1->MainPanel->Top =CutRect.top;
                  }
             }
             else                          //����߹� �������
             {
               if((MaxRight-CutRect.left)<InfoRectWidth && CutRect.top<(InfoRectHeight+InfoRectTopWidth)) //��������Ϣ��������� �����˿���±�����
               {
                 Form1->MainPanel->Left=CutRect.left -149;
                 Form1->MainPanel->Top=InfoSy+InfoRectHeight+5;
               }
               else if((MaxRight-CutRect.left)<InfoRectWidth)  //��Ϣ������ߣ��һ��ڿ�����ϲ�
               {
                 if((MaxBottom-CutRect.top)<173)  //�ײ���ͷ��
                  {
                      Form1->MainPanel->Left=CutRect.left -InfoRectWidth-149;
                      Form1->MainPanel->Top=MaxBottom-173;
                  }
                 else            //�ײ�û��ͷ
                  {
                    Form1->MainPanel->Left=CutRect.left -149;
                    Form1->MainPanel->Top=CutRect.top;
                  }
               }
               else  //��Ϣ�����˿�����Ϸ�
               {
                 if((MaxBottom-CutRect.top)<173)  //�ײ���ͷ��
                  {
                      Form1->MainPanel->Left=CutRect.left-149;
                      Form1->MainPanel->Top=MaxBottom-173;
                  }
                 else
                  {
                      Form1->MainPanel->Left=CutRect.left -149;
                      Form1->MainPanel->Top=CutRect.top;
                  }
               }
             }
           }
           else    //����ұ߹�һ��MainPanel
           {
               if(CutRect.top<(InfoRectHeight+InfoRectTopWidth))//��Ϣ������
                {
                    Form1->MainPanel->Left=CutRect.right+5;
                    if((CutRect.Width()+4)<InfoRectWidth)
                    Form1->MainPanel->Top=CutRect.top+InfoRectTopWidth+InfoRectHeight+5;
                    else
                    Form1->MainPanel->Top=CutRect.top;
                }
                else
                {
                  if((MaxBottom-CutRect.top)<173)  //�ײ���ͷ��
                    {
                      if((CutRect.Width())<InfoRectWidth)
                        Form1->MainPanel->Left=CutRect.left-149;
                      else
                        Form1->MainPanel->Left=CutRect.right+5;
                      Form1->MainPanel->Top=MaxBottom-173;
                    }
                   else
                    {
                      Form1->MainPanel->Left=CutRect.right+5;
                      Form1->MainPanel->Top=CutRect.top;
                    }
                }
          }

      UpdateWindow(Form1->Handle);
}
////////--------------------------------------------------------------------------------------------
void TForm1::CutRectChange(int x,int y)  //��������
{
  
}

//-------------------------------------------------------------------------------------------------------



void __fastcall TForm1::Button2Click(TObject *Sender)
{
//BitBlt(Canvas->Handle,0,0,Width,Height,LastDrawBmp->Canvas->Handle,0,0,SRCCOPY);

}
//---------------------------------------------------------------------------




void __fastcall TForm1::N11Click(TObject *Sender)
{
 if(CurrentMode!=0)  //�Ѿ����ڸ�״̬����Ҫ�ı�
  downfun(1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N21Click(TObject *Sender)
{
 if(CurrentMode!=1)
  downfun(2);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N31Click(TObject *Sender)
{
 if(CurrentMode!=2)
downfun(3);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N51Click(TObject *Sender)
{
 if(CurrentMode!=3)
downfun(4);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N53Click(TObject *Sender)
{
 if(CurrentMode!=4)
downfun(5);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N52Click(TObject *Sender)
{
 if(CurrentMode!=5)
downfun(6);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N1Click(TObject *Sender)
{
//downfun7_12(7);
upfun7_12(7);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N7Click(TObject *Sender)
{
//downfun7_12(8);
upfun7_12(8);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N6Click(TObject *Sender)
{
//downfun7_12(11);
upfun7_12(11);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N8Click(TObject *Sender)
{
//downfun7_12(10);
upfun7_12(10);
}
//---------------------------------------------------------------------------







void TForm1::CutingFinish_Continue()
{
     Graphics::TBitmap*temp=new Graphics::TBitmap();
      temp->Width=CutRect.Width();
      temp->Height=CutRect.Height();
      BitBlt(temp->Canvas->Handle,0,0,temp->Width,temp->Height,LastDrawBmp->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
                                                               //LastDrawBmp->
      _SYSTEMTIME time;
      GetLocalTime(&time);
      AnsiString str="����ʱ�䣺";
      str+=AnsiString(time.wYear)+"��"+AnsiString(time.wMonth)+"��"+AnsiString(time.wDay)+"�� ";
      str+=AnsiString(time.wHour)+":"+AnsiString(time.wMinute)+":"+AnsiString(time.wSecond);
      AnsiString *date=new AnsiString(str);
      if(liuxuan->CutList->Count==0)
      {
       liuxuan->Image3->Picture->Bitmap=temp;
       liuxuan->pImage=0;
      }
      else
      {
       liuxuan->Image3->Picture->Bitmap=temp;
       liuxuan->pImage=liuxuan->CutList->Count; //��ʱ��δ����
       liuxuan->Image2->Picture->Bitmap->Assign((Graphics::TBitmap*)liuxuan->CutList->Items[liuxuan->CutList->Count-1]);
       liuxuan->Image4->Picture->Bitmap=liuxuan->NoPicture;
      }

         try
      {
        Clipboard()->Open();
        Clipboard()->Assign(temp);

        // Clipboard()->SetAsHandle(MyFormat,DataHandle);
      }
    __finally
    {
        Clipboard()->Close();
    }

      liuxuan->CutList->Add(temp) ;
      liuxuan->DateList->Add(date);
      liuxuan->ShowImageInfo();
}


void  TForm1::CutingFinish()
{
      Graphics::TBitmap*temp=new Graphics::TBitmap();
      temp->Width=CutRect.Width();
      temp->Height=CutRect.Height();
      BitBlt(temp->Canvas->Handle,0,0,temp->Width,temp->Height,LastDrawBmp->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
                                                               //LastDrawBmp->
      _SYSTEMTIME time;
      GetLocalTime(&time);
      AnsiString str="����ʱ�䣺";
      str+=AnsiString(time.wYear)+"��"+AnsiString(time.wMonth)+"��"+AnsiString(time.wDay)+"�� ";
      str+=AnsiString(time.wHour)+":"+AnsiString(time.wMinute)+":"+AnsiString(time.wSecond);
      AnsiString *date=new AnsiString(str);
      if(liuxuan->CutList->Count==0)
      {
       liuxuan->Image3->Picture->Bitmap=temp;
       liuxuan->pImage=0;
      }
      else
      {
       liuxuan->Image3->Picture->Bitmap=temp;
       liuxuan->pImage=liuxuan->CutList->Count; //��ʱ��δ����
       liuxuan->Image2->Picture->Bitmap->Assign((Graphics::TBitmap*)liuxuan->CutList->Items[liuxuan->CutList->Count-1]);
       liuxuan->Image4->Picture->Bitmap=liuxuan->NoPicture;
      }

  /*    if(liuxuan->CutList->Count==1)
      {
       liuxuan->Image4->Picture->Bitmap=temp;
      }
      else if(liuxuan->pImage==liuxuan->CutList->Count-1)
      {
        liuxuan->Image4->Picture->Bitmap=temp;
      }               */
         try
      {
        Clipboard()->Open();
        Clipboard()->Assign(temp);

        // Clipboard()->SetAsHandle(MyFormat,DataHandle);
      }
    __finally
    {
        Clipboard()->Close();
    }

      liuxuan->CutList->Add(temp) ;
      liuxuan->DateList->Add(date);
      liuxuan->ShowImageInfo();
      Form1->Hide();

}
//--------------------------------------------------------------------------------
void TForm1::SaveToFile_user()
{
 Graphics::TBitmap*temp=new Graphics::TBitmap();
  temp->Width=CutRect.Width();
  temp->Height=CutRect.Height();
  BitBlt(temp->Canvas->Handle,0,0,temp->Width,temp->Height,LastDrawBmp->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
  Form9->temp=temp;
  Form9->Button3->Enabled=true;
  Form9->ShowModal();  //��ɱ���
   //ˢ��liuxuan
   if(Form9->saved)
   {//����ɹ�
      if(!Form9->isgoingon)
      Close();
      _SYSTEMTIME time;
      GetLocalTime(&time);
      AnsiString str="����ʱ�䣺";
      str=AnsiString(time.wYear)+"��"+AnsiString(time.wMonth)+"��"+AnsiString(time.wDay)+"�� ";
      str+=AnsiString(time.wHour)+":"+AnsiString(time.wMinute)+":"+AnsiString(time.wSecond);

      AnsiString *date=new AnsiString(str);

       if(liuxuan->CutList->Count==0)
      {// ����ǽ�ͼ�б��ǿյ�
       liuxuan->Image3->Picture->Bitmap=temp; //3��ʾ��һ��ͼ
       liuxuan->pImage=0;  //image3 ��ŵ��ǵ�0�ţ���0��ʼ��
      }
      else
      {      //�����ͼ�б��ǿյ�
       liuxuan->Image3->Picture->Bitmap=temp;  //   3��ʾ���µ�һ��ͼ
       liuxuan->pImage=liuxuan->CutList->Count; //��ʱ��δ���� ����count��û�仯    image3 ��ŵ��ǵڡ����¡���ͼ
       liuxuan->Image2->Picture->Bitmap->Assign((Graphics::TBitmap*)liuxuan->CutList->Items[liuxuan->CutList->Count-1]);
       liuxuan->Image4->Picture->Bitmap=liuxuan->NoPicture;
      }
/*
      if(liuxuan->CutList->Count==0)
      {
       liuxuan->Image3->Picture->Bitmap=temp;
       liuxuan->pImage=0;
      }
      else if(liuxuan->CutList->Count==1)
      {
       liuxuan->Image4->Picture->Bitmap=temp;
      }            */
      liuxuan->CutList->Add(temp) ;
      liuxuan->DateList->Add(date);
      liuxuan->ShowImageInfo();
    }
    else
    {   //����ȡ��
     delete temp;
    }

}




void __fastcall TForm1::FormHide(TObject *Sender)
{
   BitBlt(LastDrawBmp->Canvas->Handle,0,0,Width,Height,Srcbmp->Canvas->Handle,0,0,SRCCOPY);
   BitBlt(BufferBmp2->Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);
   BitBlt(BufferBmp->Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);
//   BitBlt(Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);

   //��� PanelͼƬ��record ��������Ŀ��
   if(DownImage>0)
   {
    ((TImage*)(BmpList->Items[DownImage-1]))->Picture->Bitmap->LoadFromStream(ImageStream[3*(DownImage-1)+1]);
    ImageStream[3*(DownImage-1)+1]->Position=0;
   }
   DownImage=0; //��DownImage
   InImage=0;   //��InImage
   FuncButtonDown=false; //û�й��ܰ�ť��
   Triming=false;  //���ڱ༭
   OperationCount=0;    //��������0
   RecordPoint=-1; //��¼ָ��ָ��-1��
   RecordCount=0; //��¼��0
   ondrawing=false; //�Ƿ����ڻ�ͼ����д����
   oncuting=false;
   Form1->MainPanel->Visible=false;
   Form1->ColorPanel->Visible=false;
   Form1->Memo1->Clear();
   Form1->Memo1->Hide();
    // Sleep(20);
    if(ShowMainFormAfterCut)
  {
   liuxuan->TrayIcon1->Restore();
   liuxuan->Show();
  }
}
//---------------------------------------------------------------------------








void __fastcall TForm1::FormDblClick(TObject *Sender)
{
 if(oncuting)                       //&&!ColorPanel->Visible
         if(mouseX>=CutRect.left&&mouseY>=CutRect.top&&mouseX<=CutRect.right&&mouseY<=CutRect.bottom)
          {
            IsDlk=true;
            Form1->CutingFinish();
          }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
   BitBlt(LastDrawBmp->Canvas->Handle,0,0,Width,Height,Srcbmp->Canvas->Handle,0,0,SRCCOPY);
   BitBlt(BufferBmp2->Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);
   BitBlt(BufferBmp->Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);
//   BitBlt(Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);

   //��� PanelͼƬ��record ��������Ŀ��
   if(DownImage>0)
   {
    ((TImage*)(BmpList->Items[DownImage-1]))->Picture->Bitmap->LoadFromStream(ImageStream[3*(DownImage-1)+1]);
    ImageStream[3*(DownImage-1)+1]->Position=0;
   }
   DownImage=0; //��DownImage
   InImage=0;   //��InImage
   FuncButtonDown=false; //û�й��ܰ�ť��
   Triming=false;  //���ڱ༭
   OperationCount=0;    //��������0
   RecordPoint=-1; //��¼ָ��ָ��-1��
   RecordCount=0; //��¼��0
   ondrawing=false; //�Ƿ����ڻ�ͼ����д����
   oncuting=false;
   Form1->MainPanel->Visible=false;
   Form1->ColorPanel->Visible=false;
   Form1->Memo1->Clear();
   Form1->Memo1->Hide();
  // Sleep(20);
  if(ShowMainFormAfterCut)
  {
   liuxuan->TrayIcon1->Restore();
   liuxuan->Show();
  }
    // liuxuan->Show();
}
//---------------------------------------------------------------------------
void TForm1::IntialBmp(Graphics::TBitmap * tempbmp)
{
//BitBlt(tempbmp->Canvas->Handle,0,0,Width,Height,tempbmp->Canvas->Handle,0,0,SRCCOPY);
//tempbmp->Assign(bmp);

Graybmp->PixelFormat=6;
Graybmp->Width=Width;
Graybmp->Height=Height;
BitBlt(Graybmp->Canvas->Handle,0,0,Width,Height,tempbmp->Canvas->Handle,0,0,SRCCOPY);
Graylize(Graybmp,1);    //�䰵 �Ҷȿɱ�


Grayerbmp->PixelFormat=6;
Grayerbmp->Width=Width;
Grayerbmp->Height=Height;
BitBlt(Grayerbmp->Canvas->Handle,0,0,Width,Height,tempbmp->Canvas->Handle,0,0,SRCCOPY);
Graylize(Grayerbmp,2);    //�䰵 �Ҷȿɱ�


GrayGraybmp->PixelFormat=6;
GrayGraybmp->Width=Width;
GrayGraybmp->Height=Height;
BitBlt(GrayGraybmp->Canvas->Handle,0,0,Width,Height,tempbmp->Canvas->Handle,0,0,SRCCOPY);
Graylize(GrayGraybmp,3);    //�䰵 �Ҷȿɱ�


BufferBmp->Width=Width;
BufferBmp->Height=Height;
BitBlt(BufferBmp->Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);
SetTextColor(BufferBmp->Canvas->Handle,RGB(255,255,255));
BufferBmp->Canvas->Pen->Color=RectLineColor;
BufferBmp->Canvas->Brush->Color=RectLineColor;

DrawBmp->Canvas->StretchDraw(Rect(0,0,Width,Height),tempbmp);
Srcbmp->Canvas->StretchDraw(Rect(0,0,Width,Height),tempbmp);
LastDrawBmp->Canvas->StretchDraw(Rect(0,0,Width,Height),tempbmp);

}





void __fastcall TForm1::FormShow(TObject *Sender)
{
  if(liuxuan->hidemain&&liuxuan->Visible)
  {
      liuxuan->Hide();
      Sleep(20);
      BitBlt(bmp->Canvas->Handle,0,0,Width,Height,GetDC(GetDesktopWindow()),0,0,SRCCOPY);
       Form1->IntialBmp(bmp);
      Form1->Width=Screen->Width;//TrueScreen->right-TrueScreen->left;
      Form1->Height=Screen->Height;//TrueScreen->bottom-TrueScreen->top;
      MaxRight=Form1->Width;
      MaxBottom=Form1->Height;
         LockSetForegroundWindow(LSFW_UNLOCK);
         SetForegroundWindow(Handle);

   //   liuxuan->Show();
   //
  }
  else
  {
      BitBlt(bmp->Canvas->Handle,0,0,Width,Height,GetDC(GetDesktopWindow()),0,0,SRCCOPY);
      Form1->IntialBmp(bmp);

      Form1->Width=Screen->Width;//TrueScreen->right-TrueScreen->left;
      Form1->Height=Screen->Height;//TrueScreen->bottom-TrueScreen->top;
      MaxRight=Form1->Width;
      MaxBottom=Form1->Height;
         LockSetForegroundWindow(LSFW_UNLOCK);
         SetForegroundWindow(Handle);
 //    }
  //    liuxuan->Show();
  }

 // liuxuan->TrayIcon1->Restore();
}
//---------------------------------------------------------------------------
void  TForm1::FullCut()
{
       Graphics::TBitmap*temp=new Graphics::TBitmap();
      temp->Width=Width;
      temp->Height=Height;
      BitBlt(temp->Canvas->Handle,0,0,Width,Height,GetDC(GetDesktopWindow()),0,0,SRCCOPY);
    //  liuxuan->Button1->Caption=AnsiString(Width);  liuxuan->Button2->Caption=AnsiString(Height);
      _SYSTEMTIME time;
      GetLocalTime(&time);
      AnsiString str="����ʱ�䣺";
      str+=AnsiString(time.wYear)+"��"+AnsiString(time.wMonth)+"��"+AnsiString(time.wDay)+"�� ";
      str+=AnsiString(time.wHour)+":"+AnsiString(time.wMinute)+":"+AnsiString(time.wSecond);
      AnsiString *date=new AnsiString(str);
      if(liuxuan->CutList->Count==0)
      {
       liuxuan->Image3->Picture->Bitmap=temp;
       liuxuan->pImage=0;
      }
      else
      {
       liuxuan->Image3->Picture->Bitmap=temp;
       liuxuan->pImage=liuxuan->CutList->Count; //��ʱ��δ����
       liuxuan->Image2->Picture->Bitmap->Assign((Graphics::TBitmap*)liuxuan->CutList->Items[liuxuan->CutList->Count-1]);
       liuxuan->Image4->Picture->Bitmap=liuxuan->NoPicture;
      }

  /*    if(liuxuan->CutList->Count==1)
      {
       liuxuan->Image4->Picture->Bitmap=temp;
      }
      else if(liuxuan->pImage==liuxuan->CutList->Count-1)
      {
        liuxuan->Image4->Picture->Bitmap=temp;
      }               */
      liuxuan->CutList->Add(temp) ;
      liuxuan->DateList->Add(date);
      liuxuan->ShowImageInfo();

}
void __fastcall TForm1::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 if(Key==44)
 { //printscreen
   Form1->FullCut();
 //  ShowMessage("ddd");
 }
 else if(Key==77)
 { //alt +M
      if(oncuting&&Shift.Contains(ssAlt))
      {
        N2->Click();
      }
 }
 else if(Key==27)
 {//ESC
    if(oncuting)//�һ����ָ��Ҷ�ȫͼ
   {
   BitBlt(LastDrawBmp->Canvas->Handle,0,0,Form1->Width,Form1->Height,Srcbmp->Canvas->Handle,0,0,SRCCOPY);
   //��� PanelͼƬ��record ��������Ŀ��
   if(DownImage>0)
   {
    ((TImage*)(BmpList->Items[DownImage-1]))->Picture->Bitmap->LoadFromStream(ImageStream[3*(DownImage-1)+1]);
    ImageStream[3*(DownImage-1)+1]->Position=0;
   }
   DownImage=0; //��DownImage
   InImage=0;   //��InImage
   FuncButtonDown=false; //û�й��ܰ�ť��
   Triming=false;  //���ڱ༭
   OperationCount=0;    //��������0
   RecordPoint=-1; //��¼ָ��ָ��-1��
   RecordCount=0; //��¼��0
   ondrawing=false; //�Ƿ����ڻ�ͼ����д����
   oncuting=false;//�ָ��޽�ͼ����״̬  ��ȥ���༭״̬
   Form1->MainPanel->Hide();
   Form1->ColorPanel->Hide();
   Form1->Memo1->Clear();
   Form1->Memo1->Hide();
   UpdateWindow(Form1->Handle);
   ::SetCursor(TB_WAVE);
   BitBlt(Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);
   DrawBmp->Canvas->StretchDraw(Rect(0,0,Grayerbmp->Width,Grayerbmp->Height),Srcbmp);//ȥ���滭
  }
  else
  {
    Hide();//�����մ����Hide����
  }
 }
 else if(Key==90)
 {
   if(Shift.Contains(ssCtrl))
   {
   FormQuickKey=true;
    upfun7_12(7);
   }
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N3Click(TObject *Sender)
{
//���浽��ͼ�б����


//���½�ͼ����
 BitBlt(LastDrawBmp->Canvas->Handle,0,0,Width,Height,Srcbmp->Canvas->Handle,0,0,SRCCOPY);
   //��� PanelͼƬ��record ��������Ŀ��
   if(DownImage>0)
   {
    ((TImage*)(BmpList->Items[DownImage-1]))->Picture->Bitmap->LoadFromStream(ImageStream[3*(DownImage-1)+1]);
    ImageStream[3*(DownImage-1)+1]->Position=0;
   }
   DownImage=0; //��DownImage
   InImage=0;   //��InImage
   FuncButtonDown=false; //û�й��ܰ�ť��
   Triming=false;  //���ڱ༭
   OperationCount=0;    //��������0
   RecordPoint=-1; //��¼ָ��ָ��-1��
   RecordCount=0; //��¼��0
   ondrawing=false; //�Ƿ����ڻ�ͼ����д����
   oncuting=false;//�ָ��޽�ͼ����״̬  ��ȥ���༭״̬
   Form1->MainPanel->Hide();
   Form1->ColorPanel->Hide();
   Form1->Memo1->Clear();
   Form1->Memo1->Hide();
   UpdateWindow(Form1->Handle);
   ::SetCursor(TB_WAVE);
   BitBlt(Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);
   DrawBmp->Canvas->StretchDraw(Rect(0,0,Grayerbmp->Width,Grayerbmp->Height),Srcbmp);//ȥ���滭
}
//---------------------------------------------------------------------------


void __fastcall TForm1::N9Click(TObject *Sender)
{
CutingFinish_Continue();
   BitBlt(LastDrawBmp->Canvas->Handle,0,0,Width,Height,Srcbmp->Canvas->Handle,0,0,SRCCOPY);
   //��� PanelͼƬ��record ��������Ŀ��
   if(DownImage>0)
   {
    ((TImage*)(BmpList->Items[DownImage-1]))->Picture->Bitmap->LoadFromStream(ImageStream[3*(DownImage-1)+1]);
    ImageStream[3*(DownImage-1)+1]->Position=0;
   }
   DownImage=0; //��DownImage
   InImage=0;   //��InImage
   FuncButtonDown=false; //û�й��ܰ�ť��
   Triming=false;  //���ڱ༭
   OperationCount=0;    //��������0
   RecordPoint=-1; //��¼ָ��ָ��-1��
   RecordCount=0; //��¼��0
   ondrawing=false; //�Ƿ����ڻ�ͼ����д����
   oncuting=false;//�ָ��޽�ͼ����״̬  ��ȥ���༭״̬
   Form1->MainPanel->Hide();
   Form1->ColorPanel->Hide();
   Form1->Memo1->Clear();
   Form1->Memo1->Hide();
   UpdateWindow(Form1->Handle);
   ::SetCursor(TB_WAVE);
   BitBlt(Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);
   DrawBmp->Canvas->StretchDraw(Rect(0,0,Grayerbmp->Width,Grayerbmp->Height),Srcbmp);//ȥ���滭

}
//---------------------------------------------------------------------------

void __fastcall TForm1::N2Click(TObject *Sender)
{
 if(MainPanel->Visible)
 {
   MainPanel->Hide();
   CanToolBarVisible=false;
 }
 else
 {
          if((MaxRight-CutRect.right)<149)    //����ұ߲���һ��MainPanel
           {
             if(CutRect.left<149)          //������Ҳ����
             {
                 if((MaxBottom-CutRect.top)<173)  //�ײ���ͷ��
                  {
                      Form1->MainPanel->Left=CutRect.right-145;
                      Form1->MainPanel->Top=MaxBottom-173;
                  }
                  else
                  {
                      Form1->MainPanel->Left=CutRect.right-145;
                      Form1->MainPanel->Top =CutRect.top;
                  }
             }
             else                          //����߹� �������
             {
               if((MaxRight-CutRect.left)<InfoRectWidth && CutRect.top<(InfoRectHeight+InfoRectTopWidth)) //��������Ϣ��������� �����˿���±�����
               {
                 Form1->MainPanel->Left=CutRect.left -149;
                 Form1->MainPanel->Top=InfoSy+InfoRectHeight+5;
               }
               else if((MaxRight-CutRect.left)<InfoRectWidth)  //��Ϣ������ߣ��һ��ڿ�����ϲ�
               {
                 if((MaxBottom-CutRect.top)<173)  //�ײ���ͷ��
                  {
                      Form1->MainPanel->Left=CutRect.left -InfoRectWidth-149;
                      Form1->MainPanel->Top=MaxBottom-173;
                  }
                 else            //�ײ�û��ͷ
                  {
                    Form1->MainPanel->Left=CutRect.left -149;
                    Form1->MainPanel->Top=CutRect.top;
                  }
               }
               else  //��Ϣ�����˿�����Ϸ�
               {
                 if((MaxBottom-CutRect.top)<173)  //�ײ���ͷ��
                  {
                      Form1->MainPanel->Left=CutRect.left-149;
                      Form1->MainPanel->Top=MaxBottom-173;
                  }
                 else
                  {
                      Form1->MainPanel->Left=CutRect.left -149;
                      Form1->MainPanel->Top=CutRect.top;
                  }
               }
             }
           }
           else    //����ұ߹�һ��MainPanel
           {
               if(CutRect.top<(InfoRectHeight+InfoRectTopWidth))//��Ϣ������
                {
                    Form1->MainPanel->Left=CutRect.right+5;
                    if((CutRect.Width()+4)<InfoRectWidth)
                    Form1->MainPanel->Top=CutRect.top+InfoRectTopWidth+InfoRectHeight+5;
                    else
                    Form1->MainPanel->Top=CutRect.top;
                }
                else
                {
                  if((MaxBottom-CutRect.top)<173)  //�ײ���ͷ��
                    {
                      if((CutRect.Width())<InfoRectWidth)
                        Form1->MainPanel->Left=CutRect.left-149;
                      else
                        Form1->MainPanel->Left=CutRect.right+5;
                      Form1->MainPanel->Top=MaxBottom-173;
                    }
                   else
                    {
                      Form1->MainPanel->Left=CutRect.right+5;
                      Form1->MainPanel->Top=CutRect.top;
                    }
                }
          }

   MainPanel->Show();
   CanToolBarVisible=true;

 }
}
//---------------------------------------------------------------------------








