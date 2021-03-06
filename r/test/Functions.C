//script to test TRFunction
#include<TRInterface.h>
#include<TMath.h>
double funv(TVectorD v)
{
   return v[0] * v[1];
}

void funm(TMatrixD m)
{
   m.Print();
}

void funs(TString s)
{
   std::cout << "hello " << s.Data() << std::endl;
}

Double_t fun3(Double_t *x, Double_t *par)
{
   return x[0] * par[0];
}

Double_t fun4(Double_t x)
{
   return x * 3;;
}


void Functions()
{
#if defined(__CINT__) && !defined(__MAKECINT__) 
  cout << "WARNING: This test can run only using ACliC, you must run it by doing: " << endl;
  cout << "cd  $ROOTSYS/r/tests" << endl; 
  cout << "\t .x Functions.C+" << endl; 
  return;
#endif


   gR->SetVerbose(kFALSE);

   gR->Assign(ROOT::R::TRFunction(funv), "funv");
   gR->Parse("print(funv(c(2,3)))");

   gR->Assign(ROOT::R::TRFunction(funm), "funm");
   gR->Parse("cat(funm(matrix(c(1,2,3,4),2,2)))");

   gR->Assign(ROOT::R::TRFunction(funs), "funs");
   gR->Parse("cat(funs('ROOTR'))");

   gR->Assign(ROOT::R::TRFunction(TMath::DiLog), "DiLog");
   gR->Parse("print(DiLog(2))");

   gR->Parse("x <- seq(0,10,0.01)");
   gR->Parse("y <- NULL ");
   gR->Parse("for(i in seq(along=x)) { \
		y <- c(y,DiLog(i)) \
	     }");
   gR->Xwin();
   gR->plot("x,y");

}
