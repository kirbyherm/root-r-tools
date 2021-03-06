//numerical integration using R
//passing the function from ROOT
#include<TMath.h>
#include<TRInterface.h>
#include<Math/Integrator.h>
#include<TF1.h>

//To integrate using R the function should be vectorized
//The idea is just to receive a vector like an argument,to evaluate 
//every element saving the result in another vector 
//and return the resultant vector.
TVectorD  BreitWignerVectorized(TVectorD xx)
{
  TVectorD result(xx.GetNoElements());
  for(Int_t i=0;i<xx.GetNoElements();i++)
  {
    result[i]=TMath::BreitWigner(xx[i]);
  }
  return result;
}

double BreitWignerWrap( double x){ 
   return TMath::BreitWigner(x);
}


void Integration()
{
  #if defined(__CINT__) && !defined(__MAKECINT__) 
  cout << "WARNING: This tutorial can run only using ACliC, you must run it by doing: " << endl;
  cout << "cd  $ROOTSYS/tutorials/r/" << endl; 
  cout << "\t .x Integration.C+" << endl; 
  return;
  #endif

  ROOT::R::TRInterface &r=gR->Instance();
  
  r["BreitWigner"]=ROOT::R::TRFunction(BreitWignerVectorized);
  
  Double_t value=r.ParseEval("integrate(BreitWigner, lower = -2, upper = 2)$value");

  std::cout.precision(18);
  std::cout<<"Integral of the BreitWigner Function in the interval [-2, 2] R        = "<<value<<std::endl;

  
  ROOT::Math::WrappedFunction<> wf(BreitWignerWrap);
  ROOT::Math::Integrator i(wf);
  value=i.Integral(-2,2);
  std::cout<<"Integral of the BreitWigner Function in the interval [-2, 2] MathMore = "<<value<<std::endl;

  
  TF1 f1("BreitWigner","BreitWignerWrap(x)");
  value=f1.Integral(-2,2);
  std::cout<<"Integral of the BreitWigner Function in the interval [-2, 2] TF1      = "<<value<<std::endl;
  
  //infinte limits
  value=r.ParseEval("integrate(BreitWigner, lower = -Inf, upper = Inf)$value");
  std::cout<<"Integral of BreitWigner Function in the interval [-Inf, Inf] R    = "<<value<<std::endl;

}
