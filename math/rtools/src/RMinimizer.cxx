

#include "Math/RMinimizer.h"
#include "Math/IFunction.h"
#include <TVectorD.h>
#include "Math/BasicMinimizer.h"

namespace ROOT {
   namespace Math{

      /// function wrapper for the function to be minimized
      const ROOT::Math::IMultiGenFunction *gFunction;
      /// function wrapper for the gradient of the function to be minimized
      const ROOT::Math::IMultiGradFunction *gGradFunction;
      /// integer for the number of function calls
      int gNCalls = 0;       

      ///function to return the function values at point x
      double minfunction(TVectorD x){
        gNCalls++; 
	return (*gFunction)(x.GetMatrixArray());
      }
      ///function to return the gradient values at point y
      TVectorD mingradfunction(TVectorD y){
         unsigned int size = y.GetNoElements();
         const double * yy = y.GetMatrixArray();
         double z[size];
         gGradFunction->Gradient(yy,z);
         TVectorD zz(size,z);
         return zz;
      }

      /*Default constructor with option for the method of minimization, can be any of the following:
      *
      *"Nelder-Mead", "BFGS", "CG", "L-BFGS-B", "SANN", "Brent" (Brent only for 1D minimization)
      */	
      RMinimizer::RMinimizer(Option_t *method){
         fMethod=method;
         if (fMethod.empty() || fMethod=="Migrad") fMethod="BFGS";        
      }

      ///returns number of function calls
      unsigned int RMinimizer::NCalls() const { return gNCalls; }

      ///function for finding the minimum
      bool RMinimizer::Minimize()   {

         //Set the functions
         (gFunction)= ObjFunction();
         (gGradFunction) = GradObjFunction();
         
         gNCalls = 0; 
	
         //pass functions and variables to R
         ROOT::R::TRInterface &r=gR->Instance();
	
         r["minfunction"] = ROOT::R::TRFunction((minfunction));
         r["mingradfunction"] = ROOT::R::TRFunction((mingradfunction));
         r["method"] = fMethod.c_str();
         std::vector<double> stepSizes(StepSizes(), StepSizes()+NDim());
         std::vector<double> values(X(), X()+NDim());
         r["ndim"] = NDim();
         r["stepsizes"] = stepSizes;
         r["initialparams"] = values;

         //check if optimx is available
         bool optimxloaded = FALSE;
         r["optimxloaded"] = optimxloaded;
         r.Parse("optimxloaded<-library(optimx,logical.return=TRUE)");
         int ibool = r.ParseEval("optimxloaded").ToScalar<Int_t>();
         if (ibool==1) optimxloaded=kTRUE;
         
         //string for the command to be processed in R
         TString cmd;
         
         //optimx is available and loaded
         if (optimxloaded==kTRUE) {
            if (!gGradFunction) { 
               // not using gradient function
               cmd = TString::Format("result <- optimx( initialparams, minfunction,method='%s',control = list(ndeps=stepsizes,maxit=%d,trace=%d,abstol=%e),hessian=TRUE)",fMethod.c_str(),MaxIterations(),PrintLevel(),Tolerance());
            }
            else { 
               // using user provided gradient 
               cmd = TString::Format("result <- optimx( initialparams, minfunction,mingradfunction, method='%s', control = list(ndeps=stepsizes,maxit=%d,trace=%d,abstol=%e),hessian=TRUE)",fMethod.c_str(),MaxIterations(),PrintLevel(),Tolerance());

            }
         } 
         
         //optimx is not available
         else {  
            if (!gGradFunction) { 
               // not using gradient function
               cmd = TString::Format("result <- optim( initialparams, minfunction,method='%s',control = list(ndeps=stepsizes,maxit=%d,trace=%d,abstol=%e))",fMethod.c_str(),MaxIterations(),PrintLevel(),Tolerance());
            }
            else { 
               // using user provided gradient 
               cmd = TString::Format("result <- optim( initialparams, minfunction,mingradfunction, method='%s', control = list(ndeps=stepsizes,maxit=%d,trace=%d,abstol=%e))",fMethod.c_str(),MaxIterations(),PrintLevel(),Tolerance());
            }
         }
         
         std::cout << "Calling R with command " << cmd << std::endl;   
         r.Parse(cmd.Data());
         
         //results with optimx
         if (optimxloaded){
            //get result from R
            r.Parse("par<-coef(result)");
            //get hessian matrix (in list form)
            r.Parse("hess<-attr(result,\"details\")[,\"nhatend\"]");
            //convert hess to a matrix
            r.Parse("hess<-sapply(hess,function(x) x)");
            //convert to square matrix
            r.Parse("hess<-matrix(hess,c(ndim,ndim))");
            //find covariant matrix from inverse of hess
            r.Parse("cov<-solve(hess)");
            //get errors from the sqrt of the diagonal of cov
            r.Parse("errors<-sqrt(abs(diag(cov)))");
         }

         //results with optim
         else {
            r.Parse("par<-result$par");
            r.Parse("hess<-result$hessian");
            r.Parse("cov<-solve(hess)");
            r.Parse("errors<-sqrt(abs(diag(cov)))");
         }
         
         //return the minimum to ROOT
         TVectorD vector = r.ParseEval("par").ToVector<Double_t>();

         //the following are still under development
         //   fCovMatrix = r.ParseEval("cov").ToMatrix<Double_t>();
         //   fErrors = r.ParseEval("errors").ToVector<Double_t>();
         //   fHessMatrix = r.ParseEval("hess").ToMatrix<Double_t>();
         
         //get values and show minimum
         const double *min=vector.GetMatrixArray();
         SetFinalValues(min);
         SetMinValue((*gFunction)(min));
         std::cout<<"Value at minimum ="<<MinValue()<<std::endl;

         return kTRUE;
      }
      //Returns the ith jth component of the covarient matrix
      double RMinimizer::CovMatrix(unsigned int i, unsigned int j) const {
         unsigned int ndim = NDim();
         if (fCovMatrix==0) return 0;
         if (i > ndim || j > ndim) return 0;
         return fCovMatrix[i][j];
      }
      //the following is still in development
      /*TVectorD RMinimizer::Errors() const {
         return fErrors;
      }*/
      //Returns the ith jth component of the Hessian matrix
      double RMinimizer::HessMatrix(unsigned int i, unsigned int j) const {
         unsigned int ndim = NDim();
         if (fHessMatrix==0) return 0;
         if (i > ndim || j > ndim) return 0;
         return fHessMatrix[i][j];
      }  
   }
}
