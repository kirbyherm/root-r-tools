

#include "RMinimizer.h"
#include "Math/IFunction.h"
#include <TVectorD.h>
#include "Math/BasicMinimizer.h"



const ROOT::Math::IMultiGenFunction *gFunction;

double minfunction(TVectorD x){
	return (*gFunction)(x.GetMatrixArray());
}


RMinimizer::RMinimizer(Option_t *method){
	fMethod=method;
        if (fMethod.empty() ) fMethod="BFGS";

}
//SetFunctions

bool RMinimizer::Minimize()   {


   (gFunction)= ObjFunction();
/*
 *"Nelder-Mead", "BFGS", "CG", "L-BFGS-B", "SANN", "Brent" (Brent only for 1D minimization)
 */	
	
//std::cout<<"Number of dimension ="<< NDim()  << std::endl;
//pass function to R
ROOT::R::TRInterface &r=gR->Instance();
	
r["minfunction"] = ROOT::R::TRFunction((minfunction));

r["method"] = fMethod.c_str();
std::vector<double> stepSizes(StepSizes(), StepSizes()+NDim());
std::vector<double> values(X(), X()+NDim());

r["stepsizes"] = stepSizes;
r["initialparams"] = values;

//find minimum in R

TString cmd = TString::Format("result <- optim( initialparams, minfunction,method='%s',control = list(ndeps=stepsizes,maxit=%d,trace=%d,abstol=%e))",fMethod.c_str(),MaxIterations(),0/*PrintLevel()*/,Tolerance());
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

return kTRUE;
}
