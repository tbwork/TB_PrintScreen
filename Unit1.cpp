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



//箭头 数学公式计算
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
//直线段 中点常变量 宏定义
#define mx  (int((CutRect.left+CutRect.right)/2))
#define my  (int((CutRect.top+CutRect.bottom)/2))
//面板常变量参数 宏定义
#define MainPanelWidth 144
#define MainPanelHeight 173
#define ColorPanelWidth 273
#define ColorPanelHeight 80
#define StrToInt liuxuan->StrToInt_User
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;


/* 调试 参数  可删除*/
int test=0;

//------------------面板参数---------------------------
bool MouseIn[13]; //from 1
bool Mousedown[12];//from 0
TList*BmpList;
int InImage;//指向当前mousein 为真的image  from 1  ；0表示没有in
int DownImage;//指向当前down 的image from 1 ；0表示没有down
TMemoryStream *ImageStream[40];//  0~35：36幅主按钮图，36～39 ：4幅调节尺寸按钮图
int MinPenSize;//限定当前工具的最小画笔宽度：1，
int MaxPenSize;//限定当前工具的最大画笔宽度：30，
int MinFontSize;//限定最大字体尺寸：8，
int MaxFontSize;//限定最大字体尺寸：24，
bool ColorTableMouseDownFLAG;
inline void movefun(int i)  ;//i :from 1 to 6

inline void downfun7_12(int i); //7 to 12 特殊处理
inline void upfun7_12(int i);
inline void SetColorPanelPosition();
void Inlize(int X, int Y, int &dlx,int &dly);//用于处理把当前的坐标规格化到 绘画的区域里
void  DrawRectLine(TCanvas*canvas,int sx,int sy,int ex,int ey);

Graphics::TBitmap * bmp;



int CurrentMode;//当前的绘画模式
TColor DrawColor;
int DrawWidth;
int TextSize;
int mouseX,mouseY;
int dlx,dly;
TColor TempPenColor;   //储存临时的画笔颜色
TColor TempBrushColor;  //储存临时的画布颜色
int  TempWidth;            //储存临时的画笔宽度
int  OperationCount; //记录有效操作数，如果操作数为0，那么是可以继续拖动的。 //在进入编辑状态后，退出时是否允许拖动。一般，如果什么都没画，那么是允许拖动的；初始化为true
bool FuncButtonDown;//判断 有画图功能键按下时，才允许画图
bool MouseHasMove;//窗体上鼠标按下后是否移动过

bool FormQuickKey;//回退键是否是快捷键导致的，如果是，那么不需要改变回退键的图标。


//text editing
int memosx,memosy;//
bool IsMemoMoving;//是否在移动memo
int memolength;   
int memolinecount;

/*控制memo 的最大字符数*/
int BaseWidth;//单个字符宽度
int MaxCharPerLine;//一行最多允许放多少个字符
int MaxLineCount;//最大行数
int CurLine;
int CharCount;//当前的总字符数,中文算两个字符
int truecount;//Memo真正的行数
int maxcharline;//获取当前文本最大字数行的字数。
int MaxCharInLine; //记录当前的最大字符数行的字符数。
bool MaxWidth;
bool firstmove;//判断Memo上是否是第一次移动，如果是，那么要擦除掉之前画的外框 用UpdateWindow来更新
bool Exceptable;//特例判断，当先按下text按钮，然后再按其他按钮时，应当也要使Text文本被UserPaint函数接受。但这时绘画模式不是text，所以设置该变量来作为特例标志

int  movenum;//用于控制panel的移动


//------------------Form Main-------------------------
bool drawable;//控制鼠标拉框绘图
bool dragable;//中心区域拖动绘图
bool directable;//单向绘图，即竖直移框绘图和水平横向移框绘图
bool oncuting;//截图区域已经产生了，正在处理这个区域
bool Triming;//是否在 快速编辑截图  中，在某个绘图按钮按下后就要置为true。
bool ondrawing;//是否在 正式画图中 中，在Mousedown后，设为true
bool InfoVisible;

bool IsDlk;//用于解决双击导致的bug


Graphics::TBitmap * Srcbmp;      //原图
Graphics::TBitmap * Graybmp;     //灰色图
Graphics::TBitmap * GrayGraybmp; //最灰
Graphics::TBitmap * Grayerbmp;   //更灰
Graphics::TBitmap * BufferBmp;  // 窗体canvas的内存缓冲  ***需要画框
Graphics::TBitmap * BufferBmp2; //用于画Memo的边界；同时不影响BufferBmp
Graphics::TBitmap * InfoBmp;//用于更新Info 的Rgb信息时使用，为了节省资源
Graphics::TBitmap * DrawBmp;//用于画图时的缓冲Bmp      仅CutRect内有效
Graphics::TBitmap * LastDrawBmp;//用于保存上一次绘画的状态   仅CutRect内有效
Graphics::TBitmap * RecordBmp[5];//记录上一步的操作。    0~4      递增添加 递减访问
//Graphics::TBitmap * tempbmp;


int RecordCount;//有效记录个数 初始化为 0      ；
int RecordPoint;//指向刚添加的记录。初始化为 -1  ；




TRect CutRect;//当前的截屏区域
int sx,sy,lx,ly;
int mpx;
int mpy;// mpx mpy 为MainPanel的前一个位置
int cpx,cpy;// cpx cpy 为ColorPanel的即将出现的位置
HCURSOR TBSizeNWSE;
HCURSOR TBSizeNESW;
HCURSOR TB_WAVE;
HCURSOR TB_DRAW;
int MaxRight;   //屏幕宽
int MaxBottom;   //屏幕高
int HalfHandleSize;
bool HaveBeenMove;//判断鼠标按下后，是否运动过
bool fillable;

//-------中间变量，放在开始定义，提高效率-----------
int rsx,rsy,rex,rey,InfoSy,InfoSx; //InfoSx为信息框的起始X坐标,InfoSy为信息框的起始Y坐标
int wave[8][2];//波浪的形状




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

//----------------------牛顿迭代法-----------------------------
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
movenum=0;   //控制mousemove
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

//---------------------信息提示框-------------
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
BufferBmp=new Graphics::TBitmap();  //总是记录屏幕的上一个状态



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
memolinecount=0;  //初始化为0行
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
fillable=false;//默认不填充
FuncButtonDown=false;
Exceptable=false;

IsMemoMoving=false;//

//绘画模式 控制
CurrentMode=-1;//-1 为 不在 绘图中
DrawColor=clRed;
dlx=-1;
dly=-1;
OperationCount=0;//开始没有画图操作，初始化为0
MinPenSize=1;
MaxPenSize=35;
MinFontSize=8;
MaxFontSize=32;
DrawWidth=MinPenSize;
TextSize=MinFontSize;
MouseHasMove=false;

//CompressionValue=100;//初始化jpg压缩比率
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
Memo1->Font->Name="新宋体";
Memo1->Font->Color=DrawColor;//预设字体颜色 黑色
Memo1->Font->Size=TextSize;
Form1->Canvas->Font->Name="新宋体";
Form1->Canvas->Font->Color=clBlack;//预设字体颜色 黑色
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

PanelInitial();//面板初始化


bmp=new Graphics::TBitmap();

bmp->Width=Width;
bmp->Height=Height;
//bmp->LoadFromFile("1.bmp") ;
  bmp->PixelFormat=6;
//float Transparency;  //灰度图 灰度初始值 0.6
//float LowerTransparency;//信息框背景 透明度   0.7
//float LowestTransparency;//灰度信息框背景 透明度       0.8
for(int ii=0;ii<256;ii++)
{
 _Mask[ii]= CheckRGB(int(ii*Transparency));
 _Masker[ii]= CheckRGB(int(ii*Transparency*LowerTransparency));
 _Maskest[ii]= CheckRGB(int(ii*Transparency*LowerTransparency*LowestTransparency));
}
 ShowMainFormAfterCut=true;
}
//--------------------------------------------------------------------------
__fastcall TForm1::~TForm1() //资源释放
{
BmpList->Free();
//delete  tempbmp; //释放
delete [5] RecordBmp;//记录上一步的操作。    0~4      递增添加 递减访问
delete LastDrawBmp;//用于保存上一次绘画的状态   仅CutRect内有效
delete DrawBmp;//用于画图时的缓冲Bmp      仅CutRect内有效
delete InfoBmp;//用于更新Info 的Rgb信息时使用，为了节省资源

delete BufferBmp;  // 窗体canvas的内存缓冲  ***需要画框
delete BufferBmp2; //用于画Memo的边界；同时不影响BufferBmp

delete Graybmp;     //灰色图
delete GrayGraybmp; //最灰
delete Grayerbmp;   //更灰
delete bmp;
delete Srcbmp;      //原图
}
//---------------------------------------------------------------------------


void   __fastcall   TForm1::WndProc(TMessage&   Msg) //自定义消息处理
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
               TextSize=MinFontSize;        //8为最小字体
            }
          }
          mouseX=Msg.LParamLo;
          mouseY=Msg.LParamHi;
         if(Triming)
         {     //框内画图   鼠标为画图鼠标
            TrimCutRect(Msg.LParamLo, Msg.LParamHi);
         }
         else if(OperationCount>0)//已经编辑过，有效操作数>0
         {
            Form1->Cursor=crDefault;
         }
         else
         {      //建框
            CreateCutRect(Msg.LParamLo, Msg.LParamHi);
         }
         //下面的代码和Image控件 MM的 mousemove是一样的，为了防止速拖过Panel的时候产生按下状态的按钮
         if(InImage!=DownImage&&InImage!=0)
            {
                 ((TImage*)(BmpList->Items[InImage-1]))->Picture->Bitmap->LoadFromStream(ImageStream[(InImage-1)*3+1]);
                   ImageStream[(InImage-1)*3+1]->Position=0;
                   InImage=0;
            }

             return;
       }
        TForm::WndProc(Msg); //别忘了最后调用默认函数。

}
//----------------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TForm1::FormPaint(TObject *Sender)
{
//有一段默认代码来绘画窗口  ，必须在绘画完后重新粉刷下窗体
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
 if(HalfHandleSize==0)     //八个柄
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
  canvas->Rectangle(sx-HalfHandleSize,sy-HalfHandleSize,sx+HalfHandleSize,sy+HalfHandleSize);  //左上
  canvas->Rectangle(midx-HalfHandleSize,sy-HalfHandleSize,midx+HalfHandleSize,sy+HalfHandleSize);  //上中
  canvas->MoveTo(sx,sy);
  canvas->LineTo(ex,sy);
  canvas->Rectangle(ex-HalfHandleSize,sy-HalfHandleSize,ex+HalfHandleSize,sy+HalfHandleSize);  //右上
  canvas->Rectangle(ex-HalfHandleSize,midy-HalfHandleSize,ex+HalfHandleSize,midy+HalfHandleSize);  //右中
  canvas->MoveTo(ex,sy);
  canvas->LineTo(ex,ey);
  canvas->Rectangle(ex-HalfHandleSize,ey-HalfHandleSize,ex+HalfHandleSize,ey+HalfHandleSize);  //右下
  canvas->Rectangle(midx-HalfHandleSize,ey-HalfHandleSize,midx+HalfHandleSize,ey+HalfHandleSize);  //下中
  canvas->MoveTo(ex,ey);
  canvas->LineTo(sx,ey);
  canvas->Rectangle(sx-HalfHandleSize,ey-HalfHandleSize,sx+HalfHandleSize,ey+HalfHandleSize);  //左下
  canvas->Rectangle(sx-HalfHandleSize,midy-HalfHandleSize,sx+HalfHandleSize,midy+HalfHandleSize);  //左中
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
  canvas->TextOutA(sx+4,sy+4,"截图区域位置:  "+IntToStr(RectSx)+"*"+IntToStr(RectSy));
  canvas->TextOutA(sx+4,sy+17,"截图区域大小:  "+IntToStr(wt)+"*"+IntToStr(ht));
  canvas->TextOutA(sx+4,sy+32,"像素RGB值:  ("+IntToStr(r)+","+IntToStr(g)+","+IntToStr(b)+")");
  canvas->TextOutA(sx+25,sy+55,"双击高亮区域完成截图。");

  //wave 标志
  canvas->Pen->Color=InfoTextColor;
  canvas->MoveTo(wave[0][0]+sx+4,wave[0][1]+sy+InfoRectHeight-18);
    //构造wave
  for(int i=1;i<8;i++)
  {
    canvas->LineTo(wave[i][0]+sx+4,wave[i][1]+sy+InfoRectHeight-18);
  }
  //方框
  canvas->MoveTo(sx+4,sy+InfoRectHeight-18);
  canvas->LineTo(sx+20,sy+InfoRectHeight-18);
  canvas->LineTo(sx+20,sy+InfoRectHeight-2);
  canvas->LineTo(sx+4,sy+InfoRectHeight-2) ;
  canvas->LineTo(sx+4,sy+InfoRectHeight-18);
  canvas->MoveTo(sx+8,sy+12);
  //恢复画笔

  canvas->Pen->Color=RectLineColor;
  InfoVisible=true;
}
//===================================================================================
void TForm1::Graylize(Graphics::TBitmap*tbmp,int a)//暗淡化
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
 if(Button==mbLeft)//左键单击
 {
  if(Triming)//快速绘图编辑  代码
  {
     if(FuncButtonDown)
     {
       if(X>= CutRect.left&&Y>=CutRect.top&&X<=CutRect.right&&Y<=CutRect.bottom)
       {

         if(CurrentMode!=Text_5)
         {    // 非文字模式


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
                 Form1->Canvas->Font->Color=DrawColor;//预设字体颜色 黑色
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
           Form1->Cursor=crDefault; //设置一个鼠标样式 TB_FINISH
        }
        else
        {
           ::SetCursor(TB_WAVE);
        }
     }
  }
  else if(OperationCount>0)//已经编辑过，及草有效操作数>0
  {
     Form1->Cursor=crDefault;
  }
  else     //not triming
  {

   HaveBeenMove=false;
   if(!oncuting) //不再截图过程中，可以创建新的截图区域
   {
    ::SetCursor(TB_WAVE);
    drawable=true;    //开始允许创建区域了
    sx=X;sy=Y;
   }
   else     //已经在截图过程，其他的新的截图区域不会被创建
   {
     if(Form1->Cursor==crSizeAll)  //在中间区域
     {
       dragable=true;
       sx=X;sy=Y;//记录出发点
     }
     else if(Form1->Cursor==crSizeNESW)//在左下和右上 顶点区域
     {
       drawable=true;//四个顶点区域的拖拉，其实也是一种拉框绘图，这里借用这个drawable标志  ,并修改起始点的坐标
       if(X>=CutRect.right-HalfHandleSize&&Y<=CutRect.top+HalfHandleSize)//在右上
       {
        sx=CutRect.left;sy=CutRect.bottom;//出发点为左下
       }
       else if(X<=CutRect.left+HalfHandleSize&&Y>=CutRect.bottom-HalfHandleSize)//在左下
       {
        sx=CutRect.right;sy=CutRect.top;//出发点为右上
       }
     }
     else if(Form1->Cursor==crSizeNWSE) //在左上和右下 顶点区域
     {
       drawable=true;//四个顶点区域的拖拉，其实也是一种拉框绘图，这里借用这个drawable标志
       if(X<=CutRect.left+HalfHandleSize&&Y<=CutRect.top+HalfHandleSize)//在左上
       {
        sx=CutRect.right;sy=CutRect.bottom;//出发点为右下
       }
       else if(X>=CutRect.right-HalfHandleSize&&Y>=CutRect.bottom-HalfHandleSize)//在右下
       {
         sx=CutRect.left;sy=CutRect.top;//出发点为左上
       }
     }
     else if(Form1->Cursor==crSizeNS)    //在上下 竖直移动
     {
       directable=true; //启用水平或者竖直拖框绘图
       if(Y<=CutRect.top+HalfHandleSize)//在上 横线  ;记录右下角点
       {
         sx=CutRect.right;
         sy=CutRect.bottom;
         lx=-2;//这里的lx表示一个标志，为-2时表示拉动线条的起始位置为已得区域的上水平线。
       }
       else if(Y>=CutRect.bottom-HalfHandleSize) //在下横线    ；记录左上角点
       {
         sx=CutRect.left;
         sy=CutRect.top;
         lx=-3;//这里的lx表示一个标志，为-3时表示拉动线条的起始位置为已得区域的下水平线。
       }
     }
     else if(Form1->Cursor==crSizeWE)   //在左右横向移动
     {
       directable=true; //启用水平或者竖直拖框绘图
       if(X<=CutRect.left+HalfHandleSize) // 在左竖直线上   ；记录右下角点；
       {
         sx=CutRect.right;
         sy=CutRect.bottom;

         lx=-2; //这里的lx表示一个标志，为-2时表示拉动线条的起始位置为已得区域的左竖直线。
       }
       else if(X>=CutRect.right-HalfHandleSize)//在右竖直线上  ；记录左上角点；
       {
         sx=CutRect.left;
         sy=CutRect.top;
         lx=-3; //这里的lx表示一个标志，为-2时表示拉动线条的起始位置为已得区域的右竖直线。
       }
     }
     else //其他默认情况
     {
        ::SetCursor(TB_WAVE);//其他普通情况，无需处理
     }
   }
  }
 }
 else//右键单击
 {
  if(Triming)//快速编辑  代码
  {
    if(X>= CutRect.left&&Y>=CutRect.top&&X<=CutRect.right&&Y<=CutRect.bottom) //在编辑框内
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

// 无截图区域，也不再创建截图区域的过程中，那么为最普通的正常移动，不进行特殊处理
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
 if(Button==mbLeft)//左键单击
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

               //记录画布当前状态 更新LastDrawBmp
                BitBlt(LastDrawBmp->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),DrawBmp->Canvas->Handle, CutRect.left,CutRect.top,SRCCOPY);
               //记录存储
                RecordPoint=(RecordPoint+1)%5;
                BitBlt(RecordBmp[RecordPoint]->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),DrawBmp->Canvas->Handle, CutRect.left,CutRect.top,SRCCOPY);
                RecordCount=(RecordCount+1>5)?5:(RecordCount+1);
                OperationCount++; //添加了一个操作
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
       //添加初始化 画布
       BitBlt(RecordBmp[0]->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),Srcbmp->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
      // DrawTracker(RecordBmp[0]->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);
       RecordPoint=0;
       RecordCount=1;
  }
 }
 else
 { //右击
  if(oncuting&&X>= CutRect.left&&Y>=CutRect.top&&X<=CutRect.right&&Y<=CutRect.bottom)
  {   //如果在CutRect中 ，弹出popupmenu
    Poper->Popup(X,Y);
    return;
  }
   if(oncuting)//右击，恢复灰度全图
   {
   BitBlt(LastDrawBmp->Canvas->Handle,0,0,Form1->Width,Form1->Height,Srcbmp->Canvas->Handle,0,0,SRCCOPY);
   //清空 Panel图片、record 、操作数目；
   if(DownImage>0)
   {
    ((TImage*)(BmpList->Items[DownImage-1]))->Picture->Bitmap->LoadFromStream(ImageStream[3*(DownImage-1)+1]);
    ImageStream[3*(DownImage-1)+1]->Position=0;
   }
   DownImage=0; //无DownImage
   InImage=0;   //无InImage
   FuncButtonDown=false; //没有功能按钮。
   Triming=false;  //不在编辑
   OperationCount=0;    //操作数归0
   RecordPoint=-1; //记录指针指向-1，
   RecordCount=0; //记录归0
   ondrawing=false; //是否正在绘图或者写文字
   oncuting=false;//恢复无截图区域状态  ，去除编辑状态
   Form1->MainPanel->Hide();
   Form1->ColorPanel->Hide();
   Form1->Memo1->Clear();
   Form1->Memo1->Hide();
   UpdateWindow(Form1->Handle);
   ::SetCursor(TB_WAVE);
   BitBlt(Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);
   DrawBmp->Canvas->StretchDraw(Rect(0,0,Grayerbmp->Width,Grayerbmp->Height),Srcbmp);//去除绘画
  }
  else
  {
    Hide();//相关清空代码见Hide代码
  }
 }
}
//---------------------------------------------------------------------------
TRect TForm1::GetCurRect(int ssx,int ssy,int eex,int eey)
{
  if(ssx!=eex && ssy!=eey)//不共边或共点
  {
    int lx,ly,bx,by;//小x，小y，大x，大y，一个矩形的两个主顶点必然是 （小x，小y），（大x，大y）
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
  if(ssx==eex&&ssy==eey)//水平线和竖直线都 重合
  {
     return Rect(ssx,ssy,ssx,ssy);
  }
  else if(ssy==eey)//水平线重合
  {
    return Rect(ssx>eex?eex:ssx,ssy,ssx>eex?ssx:eex,ssy);
  }
  else if(ssx==eex)//竖直线重合ssx=eex
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
    {                        //本身没有按下
     ((TImage*)(BmpList->Items[i-1]))->Picture->Bitmap->LoadFromStream(ImageStream[(i-1)*3+2]);  //up
     ImageStream[(i-1)*3+2]->Position=0;
     InImage=i;
    }
   }
   else     //InImage！=0
   {
      if(InImage!=i)    //不在本 iamge里
      {
        if(InImage!=DownImage)
        {
         //清空上一个image      前一个鼠标所在区域按钮不是当前区域按钮， 且也不是按下状态
          ((TImage*)(BmpList->Items[InImage-1]))->Picture->Bitmap->LoadFromStream(ImageStream[(InImage-1)*3+1]);
           ImageStream[(InImage-1)*3+1]->Position=0;
           if(DownImage!=i)
           {
             //处理自己的image 加载 up.bmp
              ((TImage*)(BmpList->Items[i-1]))->Picture->Bitmap->LoadFromStream(ImageStream[(i-1)*3+2]);
              ImageStream[(i-1)*3+2]->Position=0;
              InImage=i;
           }
       }
       else
       {
           if(DownImage!=i)
           {
             //按下的仍然让它处于按下状态
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
 if(DownImage==0) //之前没有按下的按钮
 {
    ((TImage*)(BmpList->Items[i-1]))->Picture->Bitmap->LoadFromStream(ImageStream[(i-1)*3]);  //down
    ImageStream[(i-1)*3]->Position=0;
    DownImage=i;
    if(i==1||i==2||i==5)//笔和画刷，箭头，直线
    {
     Form1->SizeInfo->Caption="画笔尺寸:";
     Form1->Fillable->Hide();
    }
    else if(i==3||i==4)
    {
     Form1->SizeInfo->Caption="画笔尺寸:";
     Form1->Fillable->Show();
    }
    else if(i==6)
    {
     Form1->SizeInfo->Caption="字体大小:";
     Form1->Fillable->Hide();
    }
     if(CutRect.Top<(Form1->InfoRectHeight+Form1->InfoRectTopWidth))
     {
       //信息栏隐藏
       Form1->InfoRectHidden=true;
       BitBlt(BufferBmp->Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);//全图BitBlt
       BitBlt(BufferBmp->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
       Form1->DrawTracker(BufferBmp->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);
       BitBlt(Form1->Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
     }
     else
     {//不隐藏 不变
       Form1->InfoRectHidden=false;
     }
     SetColorPanelPosition();
     CurrentMode=i-1;
     if(CurrentMode==Text_5)
     {
       SizeEdit->Hint="最小字体8,最大字体32";
       SizeEdit->Text=IntToStr(TextSize);
     }
     else
     {
       SizeEdit->Hint="最小笔宽1,最大笔宽35";
       SizeEdit->Text=IntToStr(DrawWidth);
     }
     Triming=true;
     FuncButtonDown=true;
     Form1->ColorPanel->Show();
 }
 else if(DownImage==i) //之前这个按钮已经被按下
 {
    ((TImage*)(BmpList->Items[i-1]))->Picture->Bitmap->LoadFromStream(ImageStream[(i-1)*3+2]);  //恢复
    ImageStream[(i-1)*3+2]->Position=0;
    DownImage=0;
    Form1->ColorPanel->Hide() ;
    if(InfoRectHidden)
    {
    //------灰暗提示框代码---------------------
        rsx=CutRect.left;
        rsy=CutRect.top;
        rex=CutRect.right;
        rey=CutRect.bottom;
        if(rsy<(InfoRectHeight+InfoRectTopWidth))
        {
           if((rex-rsx)<InfoRectWidth &&(rey-rsy)<(InfoRectHeight+InfoRectTopWidth))//信息提示区域内部---这个部分理解起来比较复杂 用于提示信息的 见截屏时的灰色提示框
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
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,(rex-rsx),(rey-rsy-InfoRectTopWidth),Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);//灰1区
               BitBlt(BufferBmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,rsx+InfoRectWidth-rex,(rey-rsy-InfoRectTopWidth),GrayGraybmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,SRCCOPY);//黑1区
               BitBlt(BufferBmp->Canvas->Handle,rsx,rey,InfoRectWidth,(InfoRectHeight+InfoRectTopWidth)-(rey-rsy),GrayGraybmp->Canvas->Handle,rsx,rey,SRCCOPY);
               InfoSx=rsx;
              }
             }       //黑2区

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
            //<(InfoRectHeight+InfoRectTopWidth)时灰暗信息提示框的起始位置
               InfoSy=rsy+InfoRectTopWidth;// 记录InfoRect信息框的起始点位置

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

          //>(InfoRectHeight+InfoRectTopWidth)时灰暗信息提示框的起始位置
            InfoSy=rsy-(InfoRectHeight+InfoRectTopWidth); //InfoSx未标出 必定=rsx
        }


     SetBkMode(BufferBmp->Canvas->Handle,TRANSPARENT);
     SetTextColor(BufferBmp->Canvas->Handle,InfoTextColor);
     DrawInfo(BufferBmp->Canvas,InfoSx,InfoSy,CutRect.Width(),CutRect.Height(),rsx,rsy,lx,ly);//InfoSx，InfoSy是信息框的起始点位置 后6个参数是信息传递
     DrawTracker(BufferBmp->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);
     BitBlt(Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
     InfoRectHidden=false;
    }
    FuncButtonDown=false;
    Triming=false;  //退出编辑状态
    if(OperationCount==0)//如果能还原到“空编辑”的状态，还是可以退出编辑状态的，如果不能回退到空编辑状态，那么编辑状态将一直存在
    {
     //清空LastDrawBmp，复原为原图
     BitBlt(LastDrawBmp->Canvas->Handle,0,0,Form1->Width,Form1->Height,Srcbmp->Canvas->Handle,0,0,SRCCOPY);  //清空 Panel图片、record 、操作数目；

    }
    if(CurrentMode==Text_5)
     {
      if(ondrawing&&Memo1->Visible)
      {  //如果正在写字，那么完成写字
         BitBlt(Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
         Memo1->Hide();
         ondrawing=false;
      }
     }

 }
 else if(DownImage!=i) //之前有按下的按钮
 {
    //先清空，后自己down
    int LastMode=CurrentMode;
    ((TImage*)(BmpList->Items[DownImage-1]))->Picture->Bitmap->LoadFromStream(ImageStream[(DownImage-1)*3+1]);
     ImageStream[(DownImage-1)*3+1]->Position=0;
     ((TImage*)(BmpList->Items[i-1]))->Picture->Bitmap->LoadFromStream(ImageStream[(i-1)*3]);  //down
    ImageStream[(i-1)*3]->Position=0;
    DownImage=i;
      if(i==1||i==2||i==5)//笔和画刷，箭头，直线
    {
     Form1->SizeInfo->Caption="画笔尺寸:";
     Form1->Fillable->Hide();
    }
    else if(i==3||i==4)
    {
     Form1->SizeInfo->Caption="画笔尺寸:";
     Form1->Fillable->Show();
    }
    else if(i==6)
    {
     Form1->SizeInfo->Caption="字体大小:";
     Form1->Fillable->Hide();
    }
    if(CutRect.Top<(Form1->InfoRectWidth+Form1->InfoRectTopWidth))
     {  //信息框被隐藏
       CurrentMode=i-1;
       Form1->InfoRectHidden=true;
       BitBlt(BufferBmp->Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);//全图BitBlt
       BitBlt(BufferBmp->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
       Form1->DrawTracker(BufferBmp->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);
       BitBlt(Form1->Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
     }
     else
     {//保留 inforect信息框

       CurrentMode=i-1;
       Form1->InfoRectHidden=false;
     }
     if(CurrentMode==Text_5)
     {
       SizeEdit->Hint="最小字体8,最大字体32";
       SizeEdit->Text=IntToStr(TextSize);
     }
     else
     {
       SizeEdit->Hint="最小笔宽1,最大笔宽35";
       SizeEdit->Text=IntToStr(DrawWidth);
     }
     if(LastMode==Text_5)
     {
       Exceptable=true;
      if(ondrawing&&Memo1->Visible)
      {  //如果正在写字，那么完成写字
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
//加载 down图片
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
  //7～12 各个按钮的 单击事件，之所以不用click事件是因为双击事件会有歧义
   if(i==7)
   {
     if(RecordCount>1)
     {
       RecordPoint=(RecordPoint-1)>=0?(RecordPoint-1):4;   //-1 循坏
       BitBlt(LastDrawBmp->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),RecordBmp[RecordPoint]->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
       Form1->DrawTracker(RecordBmp[RecordPoint]->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);
       BitBlt(BufferBmp->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),RecordBmp[RecordPoint]->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
       BitBlt(Form1->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),RecordBmp[RecordPoint]->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
       RecordCount--;
       OperationCount--;
       if(OperationCount==0) //没有操作记录，更新LastDrawBmp
           BitBlt(LastDrawBmp->Canvas->Handle,0,0,Form1->Width,Form1->Height,Srcbmp->Canvas->Handle,0,0,SRCCOPY);
     }
     else
     {
      BitBlt(LastDrawBmp->Canvas->Handle,0,0,Form1->Width,Form1->Height,Srcbmp->Canvas->Handle,0,0,SRCCOPY);
      //清空 Panel图片、record 、操作数目；
      if(DownImage>0)
      {
       ((TImage*)(BmpList->Items[DownImage-1]))->Picture->Bitmap->LoadFromStream(ImageStream[3*(DownImage-1)+1]);
       ImageStream[3*(DownImage-1)+1]->Position=0;
      }
      DownImage=0; //无DownImage
      if(InImage>0)
      {
       ((TImage*)(BmpList->Items[InImage-1]))->Picture->Bitmap->LoadFromStream(ImageStream[3*(InImage-1)+1]);
       ImageStream[3*(InImage-1)+1]->Position=0;
      }
      InImage=0;   //无InImage
      FuncButtonDown=false; //没有功能按钮。
      Triming=false;  //不在编辑
      OperationCount=0;    //操作数归0
      RecordPoint=-1; //记录指针指向-1，
      RecordCount=0; //记录归0
      ondrawing=false; //是否正在绘图或者写文字
      oncuting=false;//恢复无截图区域状态  ，去除编辑状态
      Form1->MainPanel->Hide();
      Form1->ColorPanel->Hide();
      Form1->Memo1->Clear();
      Form1->Memo1->Hide();
      UpdateWindow(Form1->Handle);
      ::SetCursor(TB_WAVE);
      BitBlt(Form1->Canvas->Handle,0,0,Form1->Width,Form1->Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);
      DrawBmp->Canvas->StretchDraw(Rect(0,0,Grayerbmp->Width,Grayerbmp->Height),Srcbmp);//去除绘画

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
               TextSize=MinFontSize;        //8为最小字体
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
                 TextSize=MinFontSize;        //MaxFontSize 为最小字体
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
    if(Form1->MainPanel->Left>CutRect.right)//MainPanel 在右
    {

       if((InfoSx+Form1->InfoRectWidth)>CutRect.right ) //InfoRect的宽大于CutRect
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
       else                    //InfoRect的宽小于CutRect
       {

          if(MaxRight-(InfoSx+Form1->InfoRectWidth)>(ColorPanelWidth+2)&&InfoSy<CutRect.top)  //放在InfoRect的后面
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
           else      //上面的位置都没有空缺了，
           {
                if(CutRect.left>=(ColorPanelWidth+2))        //如果左端有空缺
                {
                  Form1->ColorPanel->Left=CutRect.left-(ColorPanelWidth+2);
                  Form1->ColorPanel->Top=CutRect.top;
                }
                else//左端也没有空缺了
                {
                  Form1->ColorPanel->Left=CutRect.right-(ColorPanelWidth+2);
                  Form1->ColorPanel->Top=Form1->MainPanel->Top;
                }
           }

       }
    }
    else if(Form1->MainPanel->Left+MainPanelWidth<CutRect.left)//MainPanel 在左边
    {
        if((Form1->MainPanel->Top+MainPanelHeight-CutRect.bottom)>(ColorPanelHeight+2))
        {
            if((MaxRight-Form1->MainPanel->Left)>(MainPanelWidth+ColorPanelWidth+5))
            {
              Form1->ColorPanel->Left=Form1->MainPanel->Left+MainPanelWidth+5;
              Form1->ColorPanel->Top=CutRect.bottom+2;
            }
            else  //不能放在下端
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
            else if(CutRect.top>ColorPanelHeight)  //这时，InfoRect也在CutRect的上部
            {
              Form1->ColorPanel->Left=InfoSx+Form1->InfoRectWidth+2;
              Form1->ColorPanel->Top=InfoSy;
            }
            else //这时 InfoRect在CutRect的内部
            {
              Form1->ColorPanel->Left=InfoSx+Form1->InfoRectWidth+2;
              Form1->ColorPanel->Top=CutRect.top;
            }
        }
    }
    else //MainPanel 在内部
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
if(oncuting&&!drawable) //截图区域已经产生   并且不在拉框绘图
{//控制鼠标的样式
 if(!dragable&!directable) //鼠标正常移动
 {
    if( (X>=CutRect.left-HalfHandleSize && Y>=CutRect.top-HalfHandleSize && X<=CutRect.left+HalfHandleSize && Y<=CutRect.top+HalfHandleSize)
           || (X>=CutRect.right-HalfHandleSize && Y>=CutRect.bottom-HalfHandleSize && X<=CutRect.right+HalfHandleSize && Y<=CutRect.bottom+HalfHandleSize)
           )//在左上或者右下把柄
    {
      Form1->Cursor=crSizeNWSE;
    }
    else if( (X>=CutRect.right-HalfHandleSize && Y>=CutRect.top-HalfHandleSize && X<=CutRect.right+HalfHandleSize && Y<=CutRect.top+HalfHandleSize)
            || (X>=CutRect.left-HalfHandleSize && Y>=CutRect.bottom-HalfHandleSize && X<=CutRect.left+HalfHandleSize && Y<=CutRect.bottom+HalfHandleSize)
           ) //在左下或者右上把柄
    {
      Form1->Cursor=crSizeNESW;
    }
    else if( (Y==CutRect.top||Y==CutRect.bottom) && X>CutRect.left && X<CutRect.right
           || (X>=mx-HalfHandleSize && Y>=CutRect.top-HalfHandleSize && X<=mx+HalfHandleSize && Y<=CutRect.top+HalfHandleSize)
           || (X>=mx-HalfHandleSize && Y>=CutRect.bottom-HalfHandleSize && X<=mx+HalfHandleSize && Y<=CutRect.bottom+HalfHandleSize)
           ) //在下或者上把柄
    {
      Form1->Cursor=crSizeNS;
    }
    else if( (X==CutRect.left||X==CutRect.right) && Y>CutRect.top && Y<CutRect.bottom
           || (X>=CutRect.left-HalfHandleSize &&X<=CutRect.left+HalfHandleSize &&Y>=my-HalfHandleSize &&Y<my+HalfHandleSize )
           || (X>=CutRect.right-HalfHandleSize &&X<=CutRect.right+HalfHandleSize &&Y>=my-HalfHandleSize &&Y<my+HalfHandleSize )
           )//在左或者右把柄
    {
      Form1->Cursor=crSizeWE;
    }
    else if(X>CutRect.left&&Y>CutRect.top&&X<CutRect.right&&Y<CutRect.bottom)  //在截图区域内部
    {
      Form1->Cursor=crSizeAll;
    }
    else
    {
      Form1->Cursor=crDefault;
            ::SetCursor(TB_WAVE);
    }
 //重绘信息区，主要是为了更新RGB值

    rsx=CutRect.left;
    rsy=CutRect.top;
    rex=CutRect.right;
    rey=CutRect.bottom;

        if(rsy<(InfoRectHeight+InfoRectTopWidth))
        {
           if((rex-rsx)<InfoRectWidth &&(rey-rsy)<(InfoRectHeight+InfoRectTopWidth))//信息提示区域内部---这个部分理解起来比较复杂 用于提示信息的 见截屏时的灰色提示框
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
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,(rex-rsx),(rey-rsy-InfoRectTopWidth),Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);//灰1区
               BitBlt(BufferBmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,rsx+InfoRectWidth-rex,(rey-rsy-InfoRectTopWidth),GrayGraybmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,SRCCOPY);//黑1区
               BitBlt(BufferBmp->Canvas->Handle,rsx,rey,InfoRectWidth,(InfoRectHeight+InfoRectTopWidth)-(rey-rsy),GrayGraybmp->Canvas->Handle,rsx,rey,SRCCOPY);
               InfoSx=rsx;
              }
             }       //黑2区

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
            //<(InfoRectHeight+InfoRectTopWidth)时灰暗信息提示框的起始位置
               InfoSy=rsy+InfoRectTopWidth;// 记录InfoRect信息框的起始点位置

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
            //>(InfoRectHeight+InfoRectTopWidth)时灰暗信息提示框的起始位置
               InfoSy=rsy-(InfoRectHeight+InfoRectTopWidth); //InfoSx未标出 必定=rsx
        }
     SetBkMode(BufferBmp->Canvas->Handle,TRANSPARENT);
     SetTextColor(BufferBmp->Canvas->Handle,InfoTextColor);
     DrawInfo(BufferBmp->Canvas,InfoSx,InfoSy,CutRect.Width(),CutRect.Height(),rsx,rsy,X,Y);
     DrawTracker(BufferBmp->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);
     BitBlt(Canvas->Handle,InfoSx,InfoSy,InfoRectWidth,InfoRectHeight,BufferBmp->Canvas->Handle,InfoSx,InfoSy,SRCCOPY);
   }
  else if(dragable)// dragable==true 拖拉中心区域绘图中
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
     //----公共代码-------                           HalfHandleSize
     BitBlt(BufferBmp->Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);//全图BitBlt
    // BitBlt(BufferBmp->Canvas->Handle,CutRect.left-HalfHandleSize,CutRect.top-HalfHandleSize,CutRect.Width()+HandleSize,CutRect.Height()+HandleSize,Graybmp->Canvas->Handle,CutRect.left-HalfHandleSize,CutRect.top-HalfHandleSize,SRCCOPY);//部分BitBlt
     BitBlt(BufferBmp->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),Srcbmp->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
    //-------------------灰暗提示框代码---------------------
        rsx=CutRect.left;
        rsy=CutRect.top;
        rex=CutRect.right;
        rey=CutRect.bottom;
        if(rsy<(InfoRectHeight+InfoRectTopWidth))
        {
           if((rex-rsx)<InfoRectWidth &&(rey-rsy)<(InfoRectHeight+InfoRectTopWidth))//信息提示区域内部---这个部分理解起来比较复杂 用于提示信息的 见截屏时的灰色提示框
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
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,(rex-rsx),(rey-rsy-InfoRectTopWidth),Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);//灰1区
               BitBlt(BufferBmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,rsx+InfoRectWidth-rex,(rey-rsy-InfoRectTopWidth),GrayGraybmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,SRCCOPY);//黑1区
               BitBlt(BufferBmp->Canvas->Handle,rsx,rey,InfoRectWidth,(InfoRectHeight+InfoRectTopWidth)-(rey-rsy),GrayGraybmp->Canvas->Handle,rsx,rey,SRCCOPY);
               InfoSx=rsx;
              }
             }       //黑2区

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
            //<(InfoRectHeight+InfoRectTopWidth)时灰暗信息提示框的起始位置
               InfoSy=rsy+InfoRectTopWidth;// 记录InfoRect信息框的起始点位置

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

          //>(InfoRectHeight+InfoRectTopWidth)时灰暗信息提示框的起始位置
            InfoSy=rsy-(InfoRectHeight+InfoRectTopWidth); //InfoSx未标出 必定=rsx
        }


     SetBkMode(BufferBmp->Canvas->Handle,TRANSPARENT);
     SetTextColor(BufferBmp->Canvas->Handle,InfoTextColor);
     DrawInfo(BufferBmp->Canvas,InfoSx,InfoSy,CutRect.Width(),CutRect.Height(),rsx,rsy,X,Y);//InfoSx，InfoSy是信息框的起始点位置 后6个参数是信息传递
     DrawTracker(BufferBmp->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);
     BitBlt(Canvas->Handle,0,0,BufferBmp->Width,BufferBmp->Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
     sx=X;//更新上一次绘图结束时，即当次绘图出发点位置
     sy=Y;
   if(movenum==1||(!CanToolBarVisible))//充分 让move事件进行无损画框，但是对于速度较慢的窗体Update，采用两次move事件触发一次。如果move触发一次，那么下框不移动。
     return;
       if((MaxRight-CutRect.right)<149)    //框的右边不够一个MainPanel
           {
             if(CutRect.left<149)          //框的左边也不够
             {
                 if((MaxBottom-CutRect.top)<173)  //底部到头了
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
             else                          //仅左边够 画在左边
             {
               if((MaxRight-CutRect.left)<InfoRectWidth && CutRect.top<(InfoRectHeight+InfoRectTopWidth)) //顶部的信息框画在左边且 画在了框的下边线下
               {
                 Form1->MainPanel->Left=CutRect.left -149;
                 Form1->MainPanel->Top=InfoSy+InfoRectHeight+5;
               }
               else if((MaxRight-CutRect.left)<InfoRectWidth)  //信息框画在左边，且画在框的左上部
               {
                 if((MaxBottom-CutRect.top)<173)  //底部到头了
                  {
                      Form1->MainPanel->Left=CutRect.left -InfoRectWidth-149;
                      Form1->MainPanel->Top=MaxBottom-173;
                  }
                 else            //底部没到头
                  {
                    Form1->MainPanel->Left=CutRect.left -149;
                    Form1->MainPanel->Top=CutRect.top;
                  }
               }
               else  //信息框画在了框的正上方
               {
                 if((MaxBottom-CutRect.top)<173)  //底部到头了
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
           else    //框的右边够一个MainPanel
           {
               if(CutRect.top<(InfoRectHeight+InfoRectTopWidth))//信息框下置
                {
                    Form1->MainPanel->Left=CutRect.right+5;
                    if((CutRect.Width()+4)<InfoRectWidth)
                    Form1->MainPanel->Top=CutRect.top+InfoRectTopWidth+InfoRectHeight+5;
                    else
                    Form1->MainPanel->Top=CutRect.top;
                }
                else
                {
                  if((MaxBottom-CutRect.top)<173)  //底部到头了
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
  else if(directable)//directable==true 水平或者竖直 单向 拖框绘图
  {
     Form1->MainPanel->Hide();
     Form1->ColorPanel->Hide();
     if(Form1->Cursor==crSizeNS)//竖直绘图
     {
       if(lx==-2)//起始为上水平线
       CutRect=GetCurRect(CutRect.left,Y,sx,sy);//更新为新的Y（纵坐标），横坐标不变
       else if(lx==-3)//起始为下水平线
       CutRect=GetCurRect(sx,sy,CutRect.right,Y);//更新为新的Y（纵坐标），横坐标不变
     }
     else if(Form1->Cursor==crSizeWE)  //水平绘图
     {
      if(lx==-2)//起始为左竖直线
      CutRect=GetCurRect(X,CutRect.top,sx,sy);//更新为新的Y（纵坐标），横坐标不变
      else if(lx==-3)//起始为下水平线
      CutRect=GetCurRect(sx,sy,X,CutRect.bottom);//更新为新的Y（纵坐标），横坐标不变
     }
   //-----------------公共代码-------------------------------
   BitBlt(BufferBmp->Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);//全图BitBlt
  // BitBlt(BufferBmp->Canvas->Handle,sx-2,sy-2,lx-sx+4,ly-sy+4,Graybmp->Canvas->Handle,sx-2,sy-2,SRCCOPY);//部分BitBlt
   BitBlt(BufferBmp->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),Srcbmp->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
   //------灰暗提示框---------------------
        rsx=CutRect.left;
        rsy=CutRect.top;
        rex=CutRect.right;
        rey=CutRect.bottom;
        if(rsy<(InfoRectHeight+InfoRectTopWidth))
        {
           if((rex-rsx)<InfoRectWidth &&(rey-rsy)<(InfoRectHeight+InfoRectTopWidth))//信息提示区域内部---这个部分理解起来比较复杂 用于提示信息的 见截屏时的灰色提示框
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
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,(rex-rsx),(rey-rsy-InfoRectTopWidth),Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);//灰1区
               BitBlt(BufferBmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,rsx+InfoRectWidth-rex,(rey-rsy-InfoRectTopWidth),GrayGraybmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,SRCCOPY);//黑1区
               BitBlt(BufferBmp->Canvas->Handle,rsx,rey,InfoRectWidth,(InfoRectHeight+InfoRectTopWidth)-(rey-rsy),GrayGraybmp->Canvas->Handle,rsx,rey,SRCCOPY);
               InfoSx=rsx;
              }
             }       //黑2区

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
            //<(InfoRectHeight+InfoRectTopWidth)时灰暗信息提示框的起始位置
               InfoSy=rsy+InfoRectTopWidth;// 记录InfoRect信息框的起始点位置

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
            //>(InfoRectHeight+InfoRectTopWidth)时灰暗信息提示框的起始位置
               InfoSy=rsy-(InfoRectHeight+InfoRectTopWidth); //InfoSx未标出 必定=rsx
        }

     SetBkMode(BufferBmp->Canvas->Handle,TRANSPARENT);
     SetTextColor(BufferBmp->Canvas->Handle,InfoTextColor);
     DrawInfo(BufferBmp->Canvas,InfoSx,InfoSy,CutRect.Width(),CutRect.Height(),rsx,rsy,X,Y);
     DrawTracker(BufferBmp->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);
     BitBlt(Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);

  }

}
else//无截图区域
if(drawable)  //拉框绘图，有两种情况：1，新建区域；2，修改区域（鼠标变为双向箭头时） 起始点：sx，sy
{ //据当前点更改鼠标样式
 if(Form1->MainPanel->Visible)
 {
  Form1->MainPanel->Hide();
  Form1->ColorPanel->Hide();
 }
  if(Form1->Cursor==crSizeNESW||Form1->Cursor==crSizeNWSE)   //这两个变量的结合使用巧妙的用作了修改区域拉框时的标志位。
  {
   if((X<sx&&Y<sy)||(X>sx&&Y>sy))     //在起始点的左上或者右下，使用左上到右下的双向箭头
   {
      ::SetCursor(TBSizeNWSE);
   }
   else  if((X>sx&&Y<sy)||(X<sx&&Y>sy))   //在起始点的左下或者右上，使用左下到右上的双向箭头
   {
      ::SetCursor(TBSizeNESW);
   }
  }

  if(lx==-1)      //第一次，直接创建区域，不需要刷新
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
           if((rex-rsx)<InfoRectWidth &&(rey-rsy)<(InfoRectHeight+InfoRectTopWidth))//信息提示区域内部---这个部分理解起来比较复杂 用于提示信息的 见截屏时的灰色提示框
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
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,(rex-rsx),(rey-rsy-InfoRectTopWidth),Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);//灰1区
               BitBlt(BufferBmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,rsx+InfoRectWidth-rex,(rey-rsy-InfoRectTopWidth),GrayGraybmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,SRCCOPY);//黑1区
               BitBlt(BufferBmp->Canvas->Handle,rsx,rey,InfoRectWidth,(InfoRectHeight+InfoRectTopWidth)-(rey-rsy),GrayGraybmp->Canvas->Handle,rsx,rey,SRCCOPY);
               InfoSx=rsx;
              }
             }       //黑2区

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
            //<(InfoRectHeight+InfoRectTopWidth)时灰暗信息提示框的起始位置
               InfoSy=rsy+InfoRectTopWidth;// 记录InfoRect信息框的起始点位置

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
            //>(InfoRectHeight+InfoRectTopWidth)时灰暗信息提示框的起始位置
            InfoSy=rsy-(InfoRectHeight+InfoRectTopWidth); //InfoSx未标出 必定=rsx
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
   BitBlt(BufferBmp->Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);//全图BitBlt
  // BitBlt(BufferBmp->Canvas->Handle,sx-2,sy-2,lx-sx+4,ly-sy+4,Graybmp->Canvas->Handle,sx-2,sy-2,SRCCOPY);//部分BitBlt
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
     //--------------------------创建灰暗信息提示区-------------------------

        if(rsy<(InfoRectHeight+InfoRectTopWidth))
        {
           if((rex-rsx)<InfoRectWidth &&(rey-rsy)<(InfoRectHeight+InfoRectTopWidth))//信息提示区域内部---这个部分理解起来比较复杂 用于提示信息的 见截屏时的灰色提示框
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
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,(rex-rsx),(rey-rsy-InfoRectTopWidth),Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);//灰1区
               BitBlt(BufferBmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,rsx+InfoRectWidth-rex,(rey-rsy-InfoRectTopWidth),GrayGraybmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,SRCCOPY);//黑1区
               BitBlt(BufferBmp->Canvas->Handle,rsx,rey,InfoRectWidth,(InfoRectHeight+InfoRectTopWidth)-(rey-rsy),GrayGraybmp->Canvas->Handle,rsx,rey,SRCCOPY);
               InfoSx=rsx;
              }
             }       //黑2区

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
            //<(InfoRectHeight+InfoRectTopWidth)时灰暗信息提示框的起始位置
               InfoSy=rsy+InfoRectTopWidth;// 记录InfoRect信息框的起始点位置

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
            //>(InfoRectHeight+InfoRectTopWidth)时灰暗信息提示框的起始位置
            InfoSy=rsy-(InfoRectHeight+InfoRectTopWidth); //InfoSx未标出 必定=rsx
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
{   //type= 0 ，1， 4  铅笔&画刷，箭头，直线，
  op_type=type;
  sx=a;
  sy=b;
  ex=c;
  ey=d;
  PenColor=ca;
}
//--------------------------------------------------------------------------------------------
_Operation::_Operation(Operation_Type type, TColor ca,TColor cb,int a,int b,int c,int d)
{      //type : 2,3     椭圆，矩形
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
   if(ondrawing)//正在绘图           或者是正在编辑文本
   {
        MouseHasMove=true;
        if(X>= CutRect.left&&Y>=CutRect.top&&X<=CutRect.right&&Y<=CutRect.bottom)
        {   //如果在CutRect中
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
                { //需要填充
                  BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
                  DrawBmp->Canvas->Ellipse(dlx,dly,X,Y);
                }
                else
                { //不需要填充
                  BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
                  DrawBmp->Canvas->Arc(dlx,dly,X,Y,dlx,dly,dlx,dly);
                }
             }
             else if(CurrentMode==Rectangle_3)
             {  // 画 方 框
                   if(fillable)
                    {      //需要填充
                      BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
                      DrawBmp->Canvas->Rectangle(dlx,dly,X,Y);
                    }
                   else
                   {
                      //不需要填充
                       BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
                       DrawRectLine(DrawBmp->Canvas,dlx,dly,X,Y);

                    }
             }
             else if(CurrentMode==Line_4)
             {
                  // 画 直线
                      BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
                      DrawBmp->Canvas->MoveTo(dlx,dly);
                      DrawBmp->Canvas->LineTo(X,Y);
             }
             else if(CurrentMode==Text_5)
             {    //do nothing  在下面的更新代码里会加以处理
                     // BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
             }

        }
        else
        {   //在CutRect外
             ::SetCursor(TB_WAVE);
           //更新dlx dly
              if(CurrentMode==PenBrush_0)
               {
                   Inlize(X,Y,dlx,dly);
               }
              else if(CurrentMode==Arrow_1)
               {
                  int tx,ty;//规范化到绘图区的坐标
                   BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
                   Inlize(X,Y,tx,ty);
                   tbDrawArrow(DrawBmp->Canvas,dlx,dly,tx,ty,ArrowHigh,ArrowBottom);
               }
              else if(CurrentMode==Round_2)
               {         //画 椭圆
                  if(fillable)
                   {      //需要填充
                     int tx,ty;//规范化到绘图区的坐标
                     BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
                     Inlize(X,Y,tx,ty);
                     DrawBmp->Canvas->Ellipse(dlx,dly,tx,ty);
                   }
                   else
                   {
                    //不需要填充
                     int tx,ty;//规范化到绘图区的坐标
                     BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
                     Inlize(X,Y,tx,ty);
                     DrawBmp->Canvas->Arc(dlx,dly,tx,ty,dlx,dly,dlx,dly);

                    }
               }
              else if(CurrentMode==Rectangle_3)
               {     // 画 方 框
                   if(fillable)
                    {      //需要填充
                      int tx,ty;//规范化到绘图区的坐标
                      BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
                      Inlize(X,Y,tx,ty);
                      DrawBmp->Canvas->Rectangle(dlx,dly,tx,ty);
                    }
                   else
                   {
                      //不需要填充
                       int tx,ty;//规范化到绘图区的坐标
                       BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp ->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
                       Inlize(X,Y,tx,ty);
                       DrawRectLine(DrawBmp->Canvas,dlx,dly,tx,ty);
                    }
                }
               else if(CurrentMode==Line_4)
               {
                  // 画 直线
                      int tx,ty;//规范化到绘图区的坐标
                      BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
                      DrawBmp->Canvas->MoveTo(dlx,dly);
                      Inlize(X,Y,tx,ty);
                      DrawBmp->Canvas->LineTo(X,Y);
               }
               else if(CurrentMode==Text_5)
               {
                       //do nothing  在下面的更新代码里会加以处理
                  //    BitBlt(DrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle,CutRect.Left,CutRect.Top,SRCCOPY);
    
               }

      }
      //更新RGB信息
      if(InfoRectHidden)
      {  //信息框被隐藏
          if(Memo1->Visible)   //这时BufferBmp不变  使用BufferBmp2作为拷屏缓存
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
      { //信息框未隐藏

          //在InfoBmp上把信息框花好
          BitBlt(InfoBmp->Canvas->Handle,0,0,InfoRectWidth,InfoRectHeight,GrayGraybmp->Canvas->Handle,InfoSx,InfoSy,SRCCOPY);
          SetBkMode(InfoBmp->Canvas->Handle,TRANSPARENT);
          SetTextColor(InfoBmp->Canvas->Handle,InfoTextColor);
          DrawInfo(InfoBmp->Canvas,0,0,CutRect.Width(),CutRect.Height(),CutRect.left,CutRect.top,X,Y);
          if(Memo1->Visible)   //这时BufferBmp不变   用BufferBmp2来存放带Memo外框线的缓存图
          {
             //把InfoBmp 和DrawBmp上的有效区域都拷贝到BufferBmp2
             BitBlt(BufferBmp2->Canvas->Handle,InfoSx,InfoSy,CutRect.Width()+InfoRectWidth,CutRect.Height()+InfoRectHeight+InfoRectTopWidth,BufferBmp->Canvas->Handle,InfoSx,InfoSy,SRCCOPY);
             BitBlt(BufferBmp2->Canvas->Handle,InfoSx,InfoSy,InfoRectWidth,InfoRectHeight,InfoBmp->Canvas->Handle,0,0,SRCCOPY);
            // 把矩形框画好
             DrawTracker(BufferBmp2->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);

            //画好Memo黑外框
             MemoLine(BufferBmp2->Canvas);
            //把BufferBmp上的有效区域拷贝到Form1窗体的Canvas
             BitBlt(Canvas->Handle,InfoSx,InfoSy,CutRect.Width()+InfoRectWidth,CutRect.Height()+InfoRectHeight+InfoRectTopWidth,BufferBmp2->Canvas->Handle,InfoSx,InfoSy,SRCCOPY);


          }
          else //BufferBmp随时改变
          {
             //把InfoBmp 和DrawBmp上的有效区域都拷贝到BufferBmp
             BitBlt(BufferBmp->Canvas->Handle,InfoSx,InfoSy,InfoRectWidth,InfoRectHeight,InfoBmp->Canvas->Handle,0,0,SRCCOPY);
             BitBlt(BufferBmp->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),DrawBmp->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
            // 把矩形框画好
             DrawTracker(BufferBmp->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);

            //把BufferBmp上的有效区域拷贝到窗体的Canvas
             BitBlt(Canvas->Handle,InfoSx,InfoSy,CutRect.Width()+InfoRectWidth,CutRect.Height()+InfoRectHeight+InfoRectTopWidth,BufferBmp->Canvas->Handle,InfoSx,InfoSy,SRCCOPY);

          }
      }
   }
   else     //不在绘图
   {
      if(FuncButtonDown)
      {
       if(X>= CutRect.left&&Y>=CutRect.top&&X<=CutRect.right&&Y<=CutRect.bottom)
       {   //如果在CutRect中
         ::SetCursor(TB_DRAW);
       }
       else
       {   //在CutRect外
         ::SetCursor(TB_WAVE);
       }

       if(!InfoRectHidden)
       {
       //更新RGB信息
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
           Form1->Cursor=crDefault; //设置一个鼠标样式 TB_FINISH
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
if(drawable)//鼠标拉框绘图中
   {
     drawable=false;//不再画图了，mousemove不会产生新的区域
     if(X==sx||Y==sy)//没有产生截屏区域
     {
       if(X==sx&&Y==sy)  //都相等，相当于一个点。
       {
         if(!HaveBeenMove)//未移动，原地单击
         {
          oncuting=false;
          BitBlt(Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);
          CutRect=Rect(0,0,0,0) ;
          return;
         }
       }
       else //某条边重合
       {
       if(!CanDrawZeroRect)//不可以画零区域  ;//恢复灰度图
       {  oncuting=false;  //去除截屏区域编辑状态
       // if(!(X==sx&&Y==sy))//当横向拉线时，不产生画图 ，即恢复灰度图；
          BitBlt(Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);
          CutRect=Rect(0,0,0,0) ;
          Form1->Cursor=crDefault;
          return;
       }
       }
     }
     //可以画零区域
       CutRect=GetCurRect(sx,sy,X,Y) ;
       oncuting=true; //继续保持截屏区域编辑状态

     //处理 面板的出现
     if(!CanToolBarVisible)
     return;
            if((MaxRight-CutRect.right)<149)    //框的右边不够一个MainPanel
           {
             if(CutRect.left<149)          //框的左边也不够
             {
                if((MaxBottom-CutRect.top)<173)  //底部到头了
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
             else                          //仅左边够 画在左边
             {
               if((MaxRight-CutRect.left)<InfoRectWidth && CutRect.top<(InfoRectHeight+InfoRectTopWidth)) //顶部的信息框画在左边且 画在了框的下边线下
               {
                 Form1->MainPanel->Left=CutRect.left -149;
                 Form1->MainPanel->Top=InfoSy+InfoRectHeight+5;
               }
               else if((MaxRight-CutRect.left)<InfoRectWidth)  //信息框画在左边，且画在框的左上部
               {
                 if((MaxBottom-CutRect.top)<173)  //底部到头了
                  {
                      Form1->MainPanel->Left=CutRect.left -InfoRectWidth-149;
                      Form1->MainPanel->Top=MaxBottom-173;
                  }
                 else            //底部没到头
                  {

                    Form1->MainPanel->Left=CutRect.left -149;
                    Form1->MainPanel->Top=CutRect.top;
                  }
               }
               else  //信息框画在了框的正上方
               {
                 if((MaxBottom-CutRect.top)<173)  //底部到头了
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
           else    //框的右边够一个MainPanel
           {
               if(CutRect.top<(InfoRectHeight+InfoRectTopWidth))//信息框下置
                {
                    Form1->MainPanel->Left=CutRect.right+5;
                    if((CutRect.Width()+4)<InfoRectWidth)
                    Form1->MainPanel->Top=CutRect.top+InfoRectTopWidth+InfoRectHeight+5;
                    else
                    Form1->MainPanel->Top=CutRect.top;              }
                else
                {
                  if((MaxBottom-CutRect.top)<173)  //底部到头了
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
   else if(dragable)//鼠标拖拉中心处绘图 结束 ——不需要重新更新CutRect，在mousemove绘图中已经更新
   {
     dragable=false;
     BitBlt(Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
    if(!CanToolBarVisible)  //面板不可见
     return;
     //面板可见
           if((MaxRight-CutRect.right)<149)    //框的右边不够一个MainPanel
           {
             if(CutRect.left<149)          //框的左边也不够
             {
                if((MaxBottom-CutRect.top)<173)  //底部到头了
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
             else                          //仅左边够 画在左边
             {
               if((MaxRight-CutRect.left)<InfoRectWidth && CutRect.top<(InfoRectHeight+InfoRectTopWidth)) //顶部的信息框画在左边且 画在了框的下边线下
               {

                 Form1->MainPanel->Left=CutRect.left -149;
                 Form1->MainPanel->Top=InfoSy+InfoRectHeight+5;
               }
               else if((MaxRight-CutRect.left)<InfoRectWidth)  //信息框画在左边，且画在框的左上部
               {
                 if((MaxBottom-CutRect.top)<173)  //底部到头了
                  {

                      Form1->MainPanel->Left=CutRect.left -InfoRectWidth-149;
                      Form1->MainPanel->Top=MaxBottom-173;
                  }
                 else            //底部没到头
                  {

                    Form1->MainPanel->Left=CutRect.left -149;
                    Form1->MainPanel->Top=CutRect.top;
                  }
               }
               else  //信息框画在了框的正上方
               {
                 if((MaxBottom-CutRect.top)<173)  //底部到头了
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
           else    //框的右边够一个MainPanel
           {
                if(CutRect.top<(InfoRectHeight+InfoRectTopWidth))//信息框下置
                {
                    Form1->MainPanel->Left=CutRect.right+5;
                    if((CutRect.Width()+4)<InfoRectWidth)
                    Form1->MainPanel->Top=CutRect.top+InfoRectTopWidth+InfoRectHeight+5;
                    else
                    Form1->MainPanel->Top=CutRect.top;                }
                else
                {
                  if((MaxBottom-CutRect.top)<173)  //底部到头了
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
   else if(directable)//鼠标 竖直或者水平 拉框绘图 结束  ——不需要重新更新CutRect，在mousemove绘图中已经更新
   {
     directable=false;
     if(X==sx||Y==sy)//没有产生有效截屏区域，区域为一条线，或者是一个点
     {
       if(!CanDrawZeroRect)//不可以画零区域   //恢复灰度图
       {
        BitBlt(Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);
        CutRect=Rect(0,0,0,0) ;
        oncuting=false; //不在截屏过程中，因为截屏区域消失
        Form1->Cursor=crDefault;
       }
        return;
     }
     //处理面板 可见与否
     if(!CanToolBarVisible)  //面板不可见
     return;
     //面板可见
             if((MaxRight-CutRect.right)<149)    //框的右边不够一个MainPanel
           {
             if(CutRect.left<149)          //框的左边也不够
             {
               if((MaxBottom-CutRect.top)<173)  //底部到头了
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
             else                          //仅左边够 画在左边
             {
               if((MaxRight-CutRect.left)<InfoRectWidth && CutRect.top<(InfoRectHeight+InfoRectTopWidth)) //顶部的信息框画在左边且 画在了框的下边线下
               {

                 Form1->MainPanel->Left=CutRect.left -149;
                 Form1->MainPanel->Top=InfoSy+InfoRectHeight+5;
               }
               else if((MaxRight-CutRect.left)<InfoRectWidth)  //信息框画在左边，且画在框的左上部
               {
                 if((MaxBottom-CutRect.top)<173)  //底部到头了
                  {

                      Form1->MainPanel->Left=CutRect.left -InfoRectWidth-149;
                      Form1->MainPanel->Top=MaxBottom-173;
                  }
                 else            //底部没到头
                  {

                    Form1->MainPanel->Left=CutRect.left -149;
                    Form1->MainPanel->Top=CutRect.top;
                  }
               }
               else  //信息框画在了框的正上方
               {
                 if((MaxBottom-CutRect.top)<173)  //底部到头了
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
           else    //框的右边够一个MainPanel
           {
              if(CutRect.top<(InfoRectHeight+InfoRectTopWidth))//信息框下置
                {
                    Form1->MainPanel->Left=CutRect.right+5;
                    if((CutRect.Width()+4)<InfoRectWidth)
                    Form1->MainPanel->Top=CutRect.top+InfoRectTopWidth+InfoRectHeight+5;
                    else
                    Form1->MainPanel->Top=CutRect.top;                }
                else
                {
                  if((MaxBottom-CutRect.top)<173)  //底部到头了
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
    {//非写字
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
    {//写字
       TextSize=ToData(SizeEdit->Text);
       if(TextSize>MaxFontSize)
       {
         SizeEdit->Text=AnsiString(MaxFontSize);
         TextSize=MaxFontSize;
       }
       else if(TextSize<MinFontSize)
       {
         SizeEdit->Text=AnsiString(MinFontSize);
         TextSize=MinFontSize;        //8为最小字体
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
       int linecount=1;    //一开始就有一行。哪怕是一个字母
       for(;i<Memo1->Text.Length();i++)
       {
          if(*(Memo1->Text.c_str()+i)=='\r'||CurCharPerLine>=MaxCharPerLine)
           {//自动换行和手动换行检测
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
       //模拟输入。
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
 //控制方向  Key_left=37  Key_Down=40      Up->38   Right->39
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
 //初始化记录
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
               TextSize=MinFontSize;        //8为最小字体
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
               TextSize=MinFontSize;        //8为最小字体
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
  //鼠标设置
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
   {//有新长 出现，更新memo的宽度
      MaxCharInLine=MaxChar;
      Memo1->Width=Memo1->Left+(MaxChar+4)*CHARWIDTH<CutRect.right?(MaxChar+4)*CHARWIDTH:CutRect.right-Memo1->Left-1;
      UpdateMaxChar(); //Memo的长和宽发生了改变。 相应的最大行内字符数和最大行数也要改变
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

 UpdateWindow(Form1->Handle);//立马更新到屏幕上，不经过消息循环
 MemoLine(Canvas);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Memo1KeyPress(TObject *Sender, char &Key)
{
 CurLine=SendMessage(Memo1->Handle,EM_LINEFROMCHAR,Memo1->SelStart,0);
 int Cpos=SendMessage(Memo1->Handle,EM_LINEINDEX,CurLine,0);
 int virtualcount=Memo1->Lines->Count;
 if(Memo1->Lines->Strings[CurLine].IsEmpty()&&*(Memo1->Text.c_str()+Memo1->SelStart)!='\r')//虚行就是Count没有算它，但它却是存在，也就是，行标存在，count 不算它,它的特征是空串,并且其后面没有 \r\n
 {
  virtualcount++;
 }
 int AbleCharPerLine=(CutRect.right-Memo1->Left-1)/Form1->Canvas->TextWidth('2');
 int AbleLineCount=(CutRect.bottom-Memo1->Top-1)/(0-Memo1->Font->Height);
 if(virtualcount>=AbleLineCount) //MaxLineCount
  {   //行数已经满 包括虚行数
    if(Key==13)     //回车
    {

      if(CurLine==MaxLineCount-1)
      {
       MessageBeep(0); //禁止输入的声音
        Key='\0';
      //  Button2->Caption="最后行";
      }
      else if((*(Memo1->Text.c_str()+Cpos+Memo1->Lines->Strings[CurLine].Length())!='\r')//光标所在行最后一个字符不为回车
       &&((Memo1->SelStart-Cpos)<Memo1->Lines->Strings[CurLine].Length())  //光标后字符串不为空，至少有一个，即光标前字符串长度<本行长度Strings[CurLine].Length()
       &&( (MaxCharPerLine-(Memo1->SelStart-Cpos)+Memo1->Lines->Strings[CurLine+1].Length())<=MaxCharPerLine))
      {      //如果该行光标以后的字符串+下一行的字符串能拼成一行(超出一行则不允许)，那么允许回车；前提是。光标以后的字符串不为空，也不为回车
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
    {              //退格键，剪切键，撤销建
     // 不需要发出声音
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
       if(Memo1->MaxLength==CharCount)    //字数已经满了。如何判断是否是输入操作～
         MessageBeep(0);
      }
    }
  }
  else
  {
   Memo1->MaxLength=0;
   if(Key==22)
   {                //复制
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
  else   //满行
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
    {   //如果搜索结束也没搜到“回车”符
         return    MaxCharPerLine-Memo1->Lines->Strings[Memo1->Lines->Count-1].Length();

    }
    else//p=='\r' or i==MaxCharPerLine
    {
        if(*p=='\r')
        {//这行结束是就是回车
             return 0;
        }
        else//这行没有回车，但却是满行
        {   //交给下一行
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
bool TForm1::canpaste2(AnsiString t,int al,int w)//输入 粘贴文本，允许输入的行数，和每行的最大字符数
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
      // 不为空，则显示，并且存入记录
       LastDrawBmp->Canvas->Font=Memo1->Font;
       //记录画布当前状态 更新LastDrawBmp
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
       //记录存储
       RecordPoint=(RecordPoint+1)%5;
       BitBlt(RecordBmp[RecordPoint]->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),LastDrawBmp->Canvas->Handle, CutRect.left,CutRect.top,SRCCOPY);
       RecordCount=(RecordCount+1>5)?5:(RecordCount+1);
       OperationCount++; //添加了一个操作
    }
    else
    {
         BitBlt(Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
  //  ShowMessage("空") ;
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
 memolinecount=0;  //初始化为0行
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
 CurrentMode=-1;//-1 为 不在 绘图中
 OperationCount=0;//开始没有画图操作，初始化为0
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
     //----公共代码-------                           HalfHandleSize
     BitBlt(BufferBmp->Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);//全图BitBlt
    // BitBlt(BufferBmp->Canvas->Handle,CutRect.left-HalfHandleSize,CutRect.top-HalfHandleSize,CutRect.Width()+HandleSize,CutRect.Height()+HandleSize,Graybmp->Canvas->Handle,CutRect.left-HalfHandleSize,CutRect.top-HalfHandleSize,SRCCOPY);//部分BitBlt
     BitBlt(BufferBmp->Canvas->Handle,CutRect.left,CutRect.top,CutRect.Width(),CutRect.Height(),Srcbmp->Canvas->Handle,CutRect.left,CutRect.top,SRCCOPY);
    //-------------------灰暗提示框代码---------------------
        rsx=CutRect.left;
        rsy=CutRect.top;
        rex=CutRect.right;
        rey=CutRect.bottom;
        if(rsy<(InfoRectHeight+InfoRectTopWidth))
        {
           if((rex-rsx)<InfoRectWidth &&(rey-rsy)<(InfoRectHeight+InfoRectTopWidth))//信息提示区域内部---这个部分理解起来比较复杂 用于提示信息的 见截屏时的灰色提示框
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
               BitBlt(BufferBmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,(rex-rsx),(rey-rsy-InfoRectTopWidth),Grayerbmp->Canvas->Handle,rsx,rsy+InfoRectTopWidth,SRCCOPY);//灰1区
               BitBlt(BufferBmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,rsx+InfoRectWidth-rex,(rey-rsy-InfoRectTopWidth),GrayGraybmp->Canvas->Handle,rex,rsy+InfoRectTopWidth,SRCCOPY);//黑1区
               BitBlt(BufferBmp->Canvas->Handle,rsx,rey,InfoRectWidth,(InfoRectHeight+InfoRectTopWidth)-(rey-rsy),GrayGraybmp->Canvas->Handle,rsx,rey,SRCCOPY);
               InfoSx=rsx;
              }
             }       //黑2区

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
            //<(InfoRectHeight+InfoRectTopWidth)时灰暗信息提示框的起始位置
               InfoSy=rsy+InfoRectTopWidth;// 记录InfoRect信息框的起始点位置

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

          //>(InfoRectHeight+InfoRectTopWidth)时灰暗信息提示框的起始位置
            InfoSy=rsy-(InfoRectHeight+InfoRectTopWidth); //InfoSx未标出 必定=rsx
        }


     SetBkMode(BufferBmp->Canvas->Handle,TRANSPARENT);
     SetTextColor(BufferBmp->Canvas->Handle,InfoTextColor);
     DrawInfo(BufferBmp->Canvas,InfoSx,InfoSy,CutRect.Width(),CutRect.Height(),rsx,rsy,sx,sy);//InfoSx，InfoSy是信息框的起始点位置 后6个参数是信息传递
     DrawTracker(BufferBmp->Canvas,CutRect.left,CutRect.top,CutRect.right,CutRect.bottom);
     BitBlt(Canvas->Handle,0,0,Width,Height,BufferBmp->Canvas->Handle,0,0,SRCCOPY);
     sx=(sx+x>=0?(sx+x<=MaxRight?(sx+x):MaxRight):0);//更新上一次绘图结束时，即当次绘图出发点位置
     sy=(sy+y>=0?(sy+y<=MaxRight?(sy+y):MaxBottom):0);
    if(!CanToolBarVisible)  //面板不可见
     return;
     //面板可见
       if((MaxRight-CutRect.right)<149)    //框的右边不够一个MainPanel
           {
             if(CutRect.left<149)          //框的左边也不够
             {
                 if((MaxBottom-CutRect.top)<173)  //底部到头了
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
             else                          //仅左边够 画在左边
             {
               if((MaxRight-CutRect.left)<InfoRectWidth && CutRect.top<(InfoRectHeight+InfoRectTopWidth)) //顶部的信息框画在左边且 画在了框的下边线下
               {
                 Form1->MainPanel->Left=CutRect.left -149;
                 Form1->MainPanel->Top=InfoSy+InfoRectHeight+5;
               }
               else if((MaxRight-CutRect.left)<InfoRectWidth)  //信息框画在左边，且画在框的左上部
               {
                 if((MaxBottom-CutRect.top)<173)  //底部到头了
                  {
                      Form1->MainPanel->Left=CutRect.left -InfoRectWidth-149;
                      Form1->MainPanel->Top=MaxBottom-173;
                  }
                 else            //底部没到头
                  {
                    Form1->MainPanel->Left=CutRect.left -149;
                    Form1->MainPanel->Top=CutRect.top;
                  }
               }
               else  //信息框画在了框的正上方
               {
                 if((MaxBottom-CutRect.top)<173)  //底部到头了
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
           else    //框的右边够一个MainPanel
           {
               if(CutRect.top<(InfoRectHeight+InfoRectTopWidth))//信息框下置
                {
                    Form1->MainPanel->Left=CutRect.right+5;
                    if((CutRect.Width()+4)<InfoRectWidth)
                    Form1->MainPanel->Top=CutRect.top+InfoRectTopWidth+InfoRectHeight+5;
                    else
                    Form1->MainPanel->Top=CutRect.top;
                }
                else
                {
                  if((MaxBottom-CutRect.top)<173)  //底部到头了
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
void TForm1::CutRectChange(int x,int y)  //单边增长
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
 if(CurrentMode!=0)  //已经处在该状态则不需要改变
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
      AnsiString str="截屏时间：";
      str+=AnsiString(time.wYear)+"年"+AnsiString(time.wMonth)+"月"+AnsiString(time.wDay)+"日 ";
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
       liuxuan->pImage=liuxuan->CutList->Count; //这时还未插入
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
      AnsiString str="截屏时间：";
      str+=AnsiString(time.wYear)+"年"+AnsiString(time.wMonth)+"月"+AnsiString(time.wDay)+"日 ";
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
       liuxuan->pImage=liuxuan->CutList->Count; //这时还未插入
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
  Form9->ShowModal();  //完成保存
   //刷新liuxuan
   if(Form9->saved)
   {//保存成功
      if(!Form9->isgoingon)
      Close();
      _SYSTEMTIME time;
      GetLocalTime(&time);
      AnsiString str="截屏时间：";
      str=AnsiString(time.wYear)+"年"+AnsiString(time.wMonth)+"月"+AnsiString(time.wDay)+"日 ";
      str+=AnsiString(time.wHour)+":"+AnsiString(time.wMinute)+":"+AnsiString(time.wSecond);

      AnsiString *date=new AnsiString(str);

       if(liuxuan->CutList->Count==0)
      {// 如果是截图列表是空的
       liuxuan->Image3->Picture->Bitmap=temp; //3显示第一张图
       liuxuan->pImage=0;  //image3 存放的是第0张（从0开始）
      }
      else
      {      //如果截图列表不是空的
       liuxuan->Image3->Picture->Bitmap=temp;  //   3显示最新的一张图
       liuxuan->pImage=liuxuan->CutList->Count; //这时还未插入 所以count还没变化    image3 存放的是第“最新”张图
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
    {   //保存取消
     delete temp;
    }

}




void __fastcall TForm1::FormHide(TObject *Sender)
{
   BitBlt(LastDrawBmp->Canvas->Handle,0,0,Width,Height,Srcbmp->Canvas->Handle,0,0,SRCCOPY);
   BitBlt(BufferBmp2->Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);
   BitBlt(BufferBmp->Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);
//   BitBlt(Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);

   //清空 Panel图片、record 、操作数目；
   if(DownImage>0)
   {
    ((TImage*)(BmpList->Items[DownImage-1]))->Picture->Bitmap->LoadFromStream(ImageStream[3*(DownImage-1)+1]);
    ImageStream[3*(DownImage-1)+1]->Position=0;
   }
   DownImage=0; //无DownImage
   InImage=0;   //无InImage
   FuncButtonDown=false; //没有功能按钮。
   Triming=false;  //不在编辑
   OperationCount=0;    //操作数归0
   RecordPoint=-1; //记录指针指向-1，
   RecordCount=0; //记录归0
   ondrawing=false; //是否正在绘图或者写文字
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

   //清空 Panel图片、record 、操作数目；
   if(DownImage>0)
   {
    ((TImage*)(BmpList->Items[DownImage-1]))->Picture->Bitmap->LoadFromStream(ImageStream[3*(DownImage-1)+1]);
    ImageStream[3*(DownImage-1)+1]->Position=0;
   }
   DownImage=0; //无DownImage
   InImage=0;   //无InImage
   FuncButtonDown=false; //没有功能按钮。
   Triming=false;  //不在编辑
   OperationCount=0;    //操作数归0
   RecordPoint=-1; //记录指针指向-1，
   RecordCount=0; //记录归0
   ondrawing=false; //是否正在绘图或者写文字
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
Graylize(Graybmp,1);    //变暗 灰度可变


Grayerbmp->PixelFormat=6;
Grayerbmp->Width=Width;
Grayerbmp->Height=Height;
BitBlt(Grayerbmp->Canvas->Handle,0,0,Width,Height,tempbmp->Canvas->Handle,0,0,SRCCOPY);
Graylize(Grayerbmp,2);    //变暗 灰度可变


GrayGraybmp->PixelFormat=6;
GrayGraybmp->Width=Width;
GrayGraybmp->Height=Height;
BitBlt(GrayGraybmp->Canvas->Handle,0,0,Width,Height,tempbmp->Canvas->Handle,0,0,SRCCOPY);
Graylize(GrayGraybmp,3);    //变暗 灰度可变


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
      AnsiString str="截屏时间：";
      str+=AnsiString(time.wYear)+"年"+AnsiString(time.wMonth)+"月"+AnsiString(time.wDay)+"日 ";
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
       liuxuan->pImage=liuxuan->CutList->Count; //这时还未插入
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
    if(oncuting)//右击，恢复灰度全图
   {
   BitBlt(LastDrawBmp->Canvas->Handle,0,0,Form1->Width,Form1->Height,Srcbmp->Canvas->Handle,0,0,SRCCOPY);
   //清空 Panel图片、record 、操作数目；
   if(DownImage>0)
   {
    ((TImage*)(BmpList->Items[DownImage-1]))->Picture->Bitmap->LoadFromStream(ImageStream[3*(DownImage-1)+1]);
    ImageStream[3*(DownImage-1)+1]->Position=0;
   }
   DownImage=0; //无DownImage
   InImage=0;   //无InImage
   FuncButtonDown=false; //没有功能按钮。
   Triming=false;  //不在编辑
   OperationCount=0;    //操作数归0
   RecordPoint=-1; //记录指针指向-1，
   RecordCount=0; //记录归0
   ondrawing=false; //是否正在绘图或者写文字
   oncuting=false;//恢复无截图区域状态  ，去除编辑状态
   Form1->MainPanel->Hide();
   Form1->ColorPanel->Hide();
   Form1->Memo1->Clear();
   Form1->Memo1->Hide();
   UpdateWindow(Form1->Handle);
   ::SetCursor(TB_WAVE);
   BitBlt(Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);
   DrawBmp->Canvas->StretchDraw(Rect(0,0,Grayerbmp->Width,Grayerbmp->Height),Srcbmp);//去除绘画
  }
  else
  {
    Hide();//相关清空代码见Hide代码
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
//保存到截图列表代码


//重新截图代码
 BitBlt(LastDrawBmp->Canvas->Handle,0,0,Width,Height,Srcbmp->Canvas->Handle,0,0,SRCCOPY);
   //清空 Panel图片、record 、操作数目；
   if(DownImage>0)
   {
    ((TImage*)(BmpList->Items[DownImage-1]))->Picture->Bitmap->LoadFromStream(ImageStream[3*(DownImage-1)+1]);
    ImageStream[3*(DownImage-1)+1]->Position=0;
   }
   DownImage=0; //无DownImage
   InImage=0;   //无InImage
   FuncButtonDown=false; //没有功能按钮。
   Triming=false;  //不在编辑
   OperationCount=0;    //操作数归0
   RecordPoint=-1; //记录指针指向-1，
   RecordCount=0; //记录归0
   ondrawing=false; //是否正在绘图或者写文字
   oncuting=false;//恢复无截图区域状态  ，去除编辑状态
   Form1->MainPanel->Hide();
   Form1->ColorPanel->Hide();
   Form1->Memo1->Clear();
   Form1->Memo1->Hide();
   UpdateWindow(Form1->Handle);
   ::SetCursor(TB_WAVE);
   BitBlt(Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);
   DrawBmp->Canvas->StretchDraw(Rect(0,0,Grayerbmp->Width,Grayerbmp->Height),Srcbmp);//去除绘画
}
//---------------------------------------------------------------------------


void __fastcall TForm1::N9Click(TObject *Sender)
{
CutingFinish_Continue();
   BitBlt(LastDrawBmp->Canvas->Handle,0,0,Width,Height,Srcbmp->Canvas->Handle,0,0,SRCCOPY);
   //清空 Panel图片、record 、操作数目；
   if(DownImage>0)
   {
    ((TImage*)(BmpList->Items[DownImage-1]))->Picture->Bitmap->LoadFromStream(ImageStream[3*(DownImage-1)+1]);
    ImageStream[3*(DownImage-1)+1]->Position=0;
   }
   DownImage=0; //无DownImage
   InImage=0;   //无InImage
   FuncButtonDown=false; //没有功能按钮。
   Triming=false;  //不在编辑
   OperationCount=0;    //操作数归0
   RecordPoint=-1; //记录指针指向-1，
   RecordCount=0; //记录归0
   ondrawing=false; //是否正在绘图或者写文字
   oncuting=false;//恢复无截图区域状态  ，去除编辑状态
   Form1->MainPanel->Hide();
   Form1->ColorPanel->Hide();
   Form1->Memo1->Clear();
   Form1->Memo1->Hide();
   UpdateWindow(Form1->Handle);
   ::SetCursor(TB_WAVE);
   BitBlt(Canvas->Handle,0,0,Width,Height,Graybmp->Canvas->Handle,0,0,SRCCOPY);
   DrawBmp->Canvas->StretchDraw(Rect(0,0,Grayerbmp->Width,Grayerbmp->Height),Srcbmp);//去除绘画

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
          if((MaxRight-CutRect.right)<149)    //框的右边不够一个MainPanel
           {
             if(CutRect.left<149)          //框的左边也不够
             {
                 if((MaxBottom-CutRect.top)<173)  //底部到头了
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
             else                          //仅左边够 画在左边
             {
               if((MaxRight-CutRect.left)<InfoRectWidth && CutRect.top<(InfoRectHeight+InfoRectTopWidth)) //顶部的信息框画在左边且 画在了框的下边线下
               {
                 Form1->MainPanel->Left=CutRect.left -149;
                 Form1->MainPanel->Top=InfoSy+InfoRectHeight+5;
               }
               else if((MaxRight-CutRect.left)<InfoRectWidth)  //信息框画在左边，且画在框的左上部
               {
                 if((MaxBottom-CutRect.top)<173)  //底部到头了
                  {
                      Form1->MainPanel->Left=CutRect.left -InfoRectWidth-149;
                      Form1->MainPanel->Top=MaxBottom-173;
                  }
                 else            //底部没到头
                  {
                    Form1->MainPanel->Left=CutRect.left -149;
                    Form1->MainPanel->Top=CutRect.top;
                  }
               }
               else  //信息框画在了框的正上方
               {
                 if((MaxBottom-CutRect.top)<173)  //底部到头了
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
           else    //框的右边够一个MainPanel
           {
               if(CutRect.top<(InfoRectHeight+InfoRectTopWidth))//信息框下置
                {
                    Form1->MainPanel->Left=CutRect.right+5;
                    if((CutRect.Width()+4)<InfoRectWidth)
                    Form1->MainPanel->Top=CutRect.top+InfoRectTopWidth+InfoRectHeight+5;
                    else
                    Form1->MainPanel->Top=CutRect.top;
                }
                else
                {
                  if((MaxBottom-CutRect.top)<173)  //底部到头了
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








