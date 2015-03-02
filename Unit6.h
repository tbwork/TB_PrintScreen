//---------------------------------------------------------------------------

#ifndef Unit6H
#define Unit6H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TTBNote : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TCheckBox *CheckBox1;
        TButton *Button1;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall CheckBox1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
bool needshow;
        __fastcall TTBNote(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTBNote *TBNote;
//---------------------------------------------------------------------------
#endif
