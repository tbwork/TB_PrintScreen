//---------------------------------------------------------------------------

#include <vcl.h>
#include <windows.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("Unit7.cpp", Form7);
USEFORM("Unit8.cpp", Form8);
USEFORM("Unit9.cpp", Form9);
USEFORM("Main.cpp", liuxuan);
USEFORM("Unit3.cpp", Form3);
USEFORM("Unit1.cpp", Form1);
USEFORM("Unit4.cpp", Form4);
USEFORM("Unit5.cpp", Form5);
USEFORM("Unit6.cpp", TBNote);
USEFORM("Unit10.cpp", Form10);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        const char *Atom ="boboaixuaner20100508";
       try
        {
                 Application->Initialize();
                 Application->Hint="���ߣ�����";
                 Application->Title = "����byľͷ";
                 Application->Name="TB_PrintScreen";
                 Application->ShowHint=true;
                 HANDLE wh=NULL;
                 wh=FindWindow("Tliuxuan",NULL);
                 if(wh)
                 {
                    ::MessageBox(NULL, "�����Ѿ����У�",Application->Title.c_str(),MB_OK|MB_SYSTEMMODAL);
                    SendMessage((void*)wh,WM_SHOWWINDOW,-2,-2);
                    return 0;
                 }
             ////    if(FindProcess("TBPS"))
               ///  {
                ///   ::MessageBox(NULL, "�����Ѿ����У�",Application->Title.c_str(),MB_OK|MB_SYSTEMMODAL);
               ///    return 0;
              ///   }
             //   if(GlobalFindAtom(Atom)!=0)//��ϵͳԭ�ӱ�����ԭ��
               // {
               //  GlobalDeleteAtom(GlobalFindAtom(Atom));//ɾ��ԭ��
               //
               //  return   0;
                // }
                // GlobalAddAtom(Atom);//����ԭ��

                  //ע��������

                 Application->CreateForm(__classid(Tliuxuan), &liuxuan);
                 Application->CreateForm(__classid(TForm1), &Form1);
                 Application->CreateForm(__classid(TForm3), &Form3);
                 Application->CreateForm(__classid(TForm4), &Form4);
                 Application->CreateForm(__classid(TForm5), &Form5);
                 Application->CreateForm(__classid(TForm7), &Form7);
                 Application->CreateForm(__classid(TForm8), &Form8);
                 Application->CreateForm(__classid(TForm9), &Form9);
                 Application->CreateForm(__classid(TTBNote), &TBNote);
                 Application->CreateForm(__classid(TForm10), &Form10);
                 Application->Run();

        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        catch (...)
        {
                 try
                 {
                         throw Exception("");
                 }
                 catch (Exception &exception)
                 {
                         Application->ShowException(&exception);
                 }
        }
         GlobalDeleteAtom(GlobalFindAtom(Atom));//ɾ��ԭ��
        return 0;
}
//---------------------------------------------------------------------------
