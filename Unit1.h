//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Mask.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <jpeg.hpp>
#include <GIFImage.hpp>
#include <ActnList.hpp>
#include <pngimage.hpp>
#include <ExtDlgs.hpp>
//---------------------operation-type enum-----------------------
enum Operation_Type{PenBrush_0,Arrow_1,Round_2,Rectangle_3,Line_4,Text_5};

//---------------------User Defined Class   1--------------------------------
typedef class _Operation
{
public:
  _Operation(TColor ca,int a,int b,int size);
  _Operation(Operation_Type type, TColor ca,TColor cb,int a,int b,int c,int d);
  _Operation(Operation_Type type, TColor ca,int a,int b,int c,int d);
  ~_Operation();
  void Excute();
protected:
  Operation_Type op_type;
  int sx; //��ʼ��x
  int sy; //��ʼ��y
  int ex; //��β�� x  Textʱ����Ҫ
  int ey; //��β�� y  Textʱ����Ҫ
  TColor PenColor;
  TColor FillColor;
  int TextSize;


}Operation;

//--------------------------User Defined Class  2-----------------------------
typedef class DEFINEDRGB
{
public:
 /*  DEFINEDRGB(UINT8 d,UINT8 e,UINT8 f)
   {
     d=b;
     g=e;
     r=f;
   }             */
  UINT8 b;
  UINT8 g;
  UINT8 r;
}TBRGB;
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TPanel *ColorPanel;
        TImage *ColorTable;
        TImage *RBut;
        TImage *LBut;
        TLabel *SizeInfo;
        TMaskEdit *SizeEdit;
        TCheckBox *Fillable;
        TPanel *MainPanel;
        TImage *MM;
        TImage *M1;
        TImage *M2;
        TImage *M3;
        TImage *M5;
        TImage *M4;
        TImage *M6;
        TImage *M8;
        TImage *M7;
        TImage *M9;
        TImage *M11;
        TImage *M10;
        TImage *M12;
        TMemo *Memo1;
        TMemo *Memo2;
        TPopupMenu *Poper;
        TImageList *ImageList1;
        TMenuItem *N11;
        TMenuItem *N21;
        TMenuItem *N31;
        TMenuItem *N51;
        TMenuItem *N52;
        TMenuItem *N71;
        TMenuItem *N53;
        TMenuItem *N1;
        TMenuItem *N2;
        TMenuItem *N3;
        TMenuItem *N4;
        TMenuItem *N5;
        TMenuItem *N6;
        TMenuItem *N7;
        TMenuItem *N8;
        TActionList *ActionList1;
        TMenuItem *N9;
        TMenuItem *N10;
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall M1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall M2MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall M2MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall M3MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall M3MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall M4MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall M4MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall M5MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall M5MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall M6MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall M6MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall M7MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall M7MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall M8MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall M8MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall M9MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall M9MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall M10MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall M10MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall M11MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall M11MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall M12MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall M12MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall MMMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall ColorTableMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ColorTableMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall ColorTableMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall M1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall LButMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall LButMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall RButMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall RButMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall LButClick(TObject *Sender);
        void __fastcall LButDblClick(TObject *Sender);
        void __fastcall RButClick(TObject *Sender);
        void __fastcall RButDblClick(TObject *Sender);
        void __fastcall M7MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall M8MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall M9MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall M10MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall M11MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall M12MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall FillableClick(TObject *Sender);
        void __fastcall SizeEditChange(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall SizeEditKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall LButMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall RButMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall Memo1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Memo1MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall Memo1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall Memo1Change(TObject *Sender);
        void __fastcall Memo1KeyPress(TObject *Sender, char &Key);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall N11Click(TObject *Sender);
        void __fastcall N21Click(TObject *Sender);
        void __fastcall N31Click(TObject *Sender);
        void __fastcall N51Click(TObject *Sender);
        void __fastcall N53Click(TObject *Sender);
        void __fastcall N52Click(TObject *Sender);
        void __fastcall N1Click(TObject *Sender);
        void __fastcall N7Click(TObject *Sender);
        void __fastcall N6Click(TObject *Sender);
        void __fastcall N8Click(TObject *Sender);
        void __fastcall FormHide(TObject *Sender);
        void __fastcall FormDblClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall N3Click(TObject *Sender);
        void __fastcall N9Click(TObject *Sender);
        void __fastcall N2Click(TObject *Sender);
private:	// User declarations

public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
        __fastcall ~TForm1();
//User defined
void Graylize(Graphics::TBitmap*tbmp,int);
void DrawTracker(TCanvas*canvas,int ,int ,int ,int );
void DrawInfo(TCanvas*canvas,int ,int ,int ,int,int,int,int,int );
TRect GetCurRect(int ssx,int ssy,int eex,int eey);
void PanelInitial();//��������ɫ���ĳ�ʼ������


BYTE _Mask[256];
BYTE _Masker[256];
BYTE _Maskest[256];


void ReShowInitial();
int ToData(AnsiString str);
void   __fastcall   WndProc(TMessage&   Msg); //�Զ�����Ϣ����
void __fastcall CreateParams(TCreateParams &Param) ;
void TrimCutRect(unsigned short X,unsigned short Y);
inline void CreateCutRect(unsigned short X,unsigned short Y) ;
inline void CreateCutRectMouseLeftUp(unsigned short X,unsigned short Y);
inline void downfun(int i);//1��6��ŵİ�ť�İ����¼�
int caninput(int lineCount,int CurLine);
char * GetClipboardText();
bool canpaste(AnsiString ,int);//������״̬�µ�ճ�� ��֤
bool canpaste2(AnsiString,int,int);//��δ����״̬�µ�ճ����֤
void MemoLine(TCanvas*);
void UserFormPaint();
void CutRectMove(int x,int y);
void CutRectChange(int x,int y);
void IntialBmp(Graphics::TBitmap * );


void tbDrawArrow(TCanvas*canvas,int sx,int sy,int ex,int ey,int h,int hb);//����ͷ ����
void tbMove(TControl*control, int X,int Y);
void UpdateMaxChar();
int MaxCharLine(AnsiString text);
void CutingFinish();//��ɽ�ͼ����ͼ���ڹرգ��ѽ�ͼ�����ڽ�ͼ�б��С�
void CutingFinish_Continue(); //���һ�ν�ͼ���ѽ�ͼ�����ڽ�ͼ�б��У���ͼ���ڲ��رգ����Լ�����ͼ��
void FullCut();
void SaveToFile_user();

///--------------------User defined Varibles--------
bool CanDrawZeroRect;//�Ƿ������û���0����
TColor RectLineColor;//���ο����ɫ
float Transparency;  //�Ҷ�ͼ �Ҷȳ�ʼֵ 0.6
float LowerTransparency;//��Ϣ�򱳾� ͸����   0.7
float LowestTransparency;//�Ҷ���Ϣ�򱳾� ͸����       0.8
TColor InfoTextColor;     //��Ϣ��ʾ��������ɫ��Ĭ��Ϊ��ɫ
int HandleSize;//Tracker ��Ƥ���˸����㣨�����Ĵ�С��Ĭ��Ϊ 5 ��Ӧ��Ϊ����
int HalfHandleSize;
bool CanToolBarVisible;
int CompressionValue; //jpeg ѹ������
bool ShowMainFormAfterCut;

//-----������������δ�����ԣ���ʱ���ṩ�û��޸�

int InfoRectWidth,InfoRectHeight;//��Ϣ��ʾ��Ŀ�͸�
int InfoRectTopWidth;//��Ϣ��ʾ�����ͼ���ľ��룬��ʼ��Ϊ10


//----------------------System Defined Varibles--------------------------------
bool InfoRectHidden;

//--------------------------other function


};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
 