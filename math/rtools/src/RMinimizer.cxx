

#include "Math/RMinimizer.h"
#include "Math/IFunction.h"
#include <TVectorD.h>
#include "Math/BasicMinimizer.h"
//#include "Math/MultiNumGradFunction.h"

namespace ROOT {
   namespace Math{

      const ROOT::Math::IMultiGenFunction *gFunction;
//      const ROOT::Math::IMultiGradFunction *gGradFunction;

      double minfunction(TVectorD x){
	return (*gFunction)(x.GetMatrixArray());
      }
/*      TVectorD mingradfunction(TVectorD y){
         unsigned int size = y.GetNoElements();
         const double * yy = y.GetMatrixArray();
         double z[size];
         gGradFunction->Gradient(yy,z);
         TVectorD zz(size,z);
         return zz;
      }
*/

RMinimizer::RMinimizer(Option_t *method){
	fMethod=method;
        if (fMethod.empty() ) fMethod="BFGS";

}

void RMinimizer::SetFunction(const ROOT::Math::IMultiGenFunction & func) { 
   // set the function to minimizer 
   // need to calculate numerically the derivatives: do via class MultiNumGradFunction
   gFunction = &func;
   //ROOT::Math::MultiNumGradFunction gradFunc(func); 
   // function is cloned inside so can be delete afterwards
   // called base class method setfunction 
   // (note: write explicitly otherwise it will call back itself)
   BasicMinimizer::SetFunction(func);
}

//SetFunctions

bool RMinimizer::Minimize()   {


   (gFunction)= ObjFunction();
  // (gGradFunction) = GradObjFunction();
/*
 *"Nelder-Mead", "BFGS", "CG", "L-BFGS-B", "SANN", "Brent" (Brent only for 1D minimization)
 */	
	
//std::cout<<"Number of dimension ="<< NDim()  << std::endl;
//pass function to R
ROOT::R::TRInterface &r=gR->Instance();
	
r["minfunction"] = ROOT::R::TRFunction((minfunction));
//r["gradFunc"] = ROOT::R::TRFunction((mingradfunction));

r["method"] = fMethod.c_str();
std::vector<double> stepSizes(StepSizes(), StepSizes()+NDim());
std::vector<double> values(X(), X()+NDim());

r["stepsizes"] = stepSizes;
r["initialparams"] = values;

//find minimum in R

TString cmd = TString::Format("result <- optim( initialparams, minfunction,method='%s',control = list(ndeps=stepsizes,maxit=%d,trace=%d,abstol=%e))",fMethod.c_str(),MaxIterations(),PrintLevel(),Tolerance());
//std::cout << "Calling R with command " << cmd << std::endl;

r.Parse(cmd.Data());
//get result from R
TVectorD  vector=r.ParseEval("result$par").ToVector<Double_t>();
const double *min=vector.GetMatrixArray();
//std::cout.precision(8);
//print results
//std::cout<<"-----------------------------------------"<<std::endl;
//std::cout<<"Minimum x="<<min[0]<<" y="<<min[1]<<std::endl;
SetFinalValues(min);
SetMinValue((*gFunction)(min));
//std::cout<<"Value at minimum ="<<MinValue()<<std::endl;
/*
r.Parse("optimHess(result$par, minfunction, gradFunc)");
TString cmd2 = TString::Format("hresult <- optim( initialparams, minfunction,NULL, method='%s',hessian = TRUE, control = list(ndeps=stepsizes,maxit=%d,trace=%d,abstol=%e))",fMethod.c_str(),MaxIterations(),PrintLevel(),Tolerance());
r.Parse(cmd2.Data());
//getting the min calculated with the gradient
TVectorD  hmin=r.ParseEval("hresult$par").ToVector<Double_t>();
*/
return kTRUE;
}
   }
}
