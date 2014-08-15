

#include "Math/RMinimizer.h"
#include "Math/IFunction.h"
#include <TVectorD.h>
#include "Math/BasicMinimizer.h"
//#include "Math/MultiNumGradFunction.h"

namespace ROOT {
   namespace Math{

      const ROOT::Math::IMultiGenFunction *gFunction;
      const ROOT::Math::IMultiGradFunction *gGradFunction;

      int gNCalls = 0;

      double minfunction(TVectorD x){
        gNCalls++; 
	return (*gFunction)(x.GetMatrixArray());
      }
      TVectorD mingradfunction(TVectorD y){
         unsigned int size = y.GetNoElements();
         const double * yy = y.GetMatrixArray();
         double z[size];
         gGradFunction->Gradient(yy,z);
         TVectorD zz(size,z);
         return zz;
      }


      RMinimizer::RMinimizer(Option_t *method){
         fMethod=method;
         if (fMethod.empty() || fMethod=="Migrad") fMethod="BFGS";        
      }

      unsigned int RMinimizer::NCalls() const { return gNCalls; }



//SetFunctions

bool RMinimizer::Minimize()   {


   (gFunction)= ObjFunction();
   (gGradFunction) = GradObjFunction();
   gNCalls = 0; 
/*
 *"Nelder-Mead", "BFGS", "CG", "L-BFGS-B", "SANN", "Brent" (Brent only for 1D minimization)
 */	
	
//pass function to R
ROOT::R::TRInterface &r=gR->Instance();
	
r["minfunction"] = ROOT::R::TRFunction((minfunction));
r["mingradfunction"] = ROOT::R::TRFunction((mingradfunction));

r["method"] = fMethod.c_str();
std::vector<double> stepSizes(StepSizes(), StepSizes()+NDim());
std::vector<double> values(X(), X()+NDim());

r["ndim"] = NDim();
r["stepsizes"] = stepSizes;
r["initialparams"] = values;

//find minimum in R
bool optimxloaded = FALSE;
r["optimxloaded"] = optimxloaded;
r.Parse("optimxloaded<-library(optimx,logical.return=TRUE)");
int ibool = r.ParseEval("optimxloaded").ToScalar<Int_t>();
TString cmd;
if (ibool==1) optimxloaded=kTRUE;

if (optimxloaded==kTRUE) {
   if (!gGradFunction) { 
   // not using gradient function

   cmd = TString::Format("result <- optimx( initialparams, minfunction,method='%s',control = list(ndeps=stepsizes,maxit=%d,trace=%d,abstol=%e),hessian=TRUE)",fMethod.c_str(),MaxIterations(),PrintLevel(),Tolerance());

   }
   else { 
   // using user provided gradient 

   //r.Parse("optimHess(result$par, minfunction, mingradfunction)");
   cmd = TString::Format("result <- optimx( initialparams, minfunction,mingradfunction, method='%s', control = list(ndeps=stepsizes,maxit=%d,trace=%d,abstol=%e),hessian=TRUE)",fMethod.c_str(),MaxIterations(),PrintLevel(),Tolerance());

   }
} 
   
else {  
   if (!gGradFunction) { 
   // not using gradient function

   cmd = TString::Format("result <- optim( initialparams, minfunction,method='%s',control = list(ndeps=stepsizes,maxit=%d,trace=%d,abstol=%e))",fMethod.c_str(),MaxIterations(),PrintLevel(),Tolerance());

   }
   else { 
   // using user provided gradient 

   //r.Parse("optimHess(result$par, minfunction, mingradfunction)");
   cmd = TString::Format("result <- optim( initialparams, minfunction,mingradfunction, method='%s', control = list(ndeps=stepsizes,maxit=%d,trace=%d,abstol=%e))",fMethod.c_str(),MaxIterations(),PrintLevel(),Tolerance());

   }
}
   r.Parse("print(minfunction)");  
std::cout << "Calling R with command " << cmd << std::endl;
   
   r.Parse("print(ndim)");
   r.Parse(cmd.Data());
if (optimxloaded){
//get result from R
   r.Parse("par<-coef(result)");
//get hessian matrix (in list form)
   r.Parse("hess<-attr(result,\"details\")[,\"nhatend\"]");
   r.Parse("print(hess)");
//convert hess to a matrix
   r.Parse("hess<-sapply(hess,function(x) x)");
   r.Parse("print(hess)");
//convert to square matrix
   r.Parse("hess<-matrix(hess,c(ndim,ndim))");
   r.Parse("print(hess)");
//find covariant matrix from inverse of hess
   r.Parse("cov<-solve(hess)");
//get errors from the sqrt of the diagonal of cov
   r.Parse("errors<-sqrt(abs(diag(cov)))");
   r.Parse("print(errors)");
}
else {
   r.Parse("par<-result$par");
   r.Parse("hess<-result$hessian");
   r.Parse("cov<-solve(hess)");
   r.Parse("errors<-sqrt(abs(diag(cov)))");
}
   TVectorD vector = r.ParseEval("par").ToVector<Double_t>();
//   fCovMatrix = r.ParseEval("cov").ToMatrix<Double_t>();
//   fErrors = r.ParseEval("errors").ToVector<Double_t>();
//   fHessMatrix = r.ParseEval("hess").ToMatrix<Double_t>();
   const double *min=vector.GetMatrixArray();

SetFinalValues(min);
SetMinValue((*gFunction)(min));
std::cout<<"Value at minimum ="<<MinValue()<<std::endl;

return kTRUE;
}

double RMinimizer::CovMatrix(unsigned int i, unsigned int j) const {
   unsigned int ndim = NDim();
   if (fCovMatrix==0) return 0;
   if (i > ndim || j > ndim) return 0;
   return fCovMatrix[i][j];
}
/*TVectorD RMinimizer::Errors() const {
   return fErrors;
}*/
double RMinimizer::HessMatrix(unsigned int i, unsigned int j) const {
   unsigned int ndim = NDim();
   if (fHessMatrix==0) return 0;
   if (i > ndim || j > ndim) return 0;
   return fHessMatrix[i][j];
}  
   }
}
