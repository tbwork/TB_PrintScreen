//---------------------------------------------------------------------------

#ifndef Unit5H
#define Unit5H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <FileCtrl.hpp>
#include "JAMControls.hpp"
#include "ShellControls.hpp"
#include "ShellLink.hpp"
#include "TntComCtrls.hpp"
#include "TntStdCtrls.hpp"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TForm5 : public TForm
{
__published:	// IDE-managed Components
        TButton *Button1;
        TJamShellList *Dir;
        TJamShellLink *JamShellLink1;
        TJamShellCombo *JamShellCombo1;
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm5(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm5 *Form5;
//---------------------------------------------------------------------------
#endif
