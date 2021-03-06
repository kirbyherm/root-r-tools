//Example based  in
//http://root.cern.ch/root/html/tutorials/fit/NumericalMinimization.C.html
//http://stat.ethz.ch/R-manual/R-devel/library/stats/html/optim.html
//Author: Omar Zapata

#include<TRInterface.h>

//in the next function the *double pointer should be changed by a TVectorD,
//because the pointer has no meaning in R enviroment.
Double_t RosenBrock(const TVectorD xx )
{
  const Double_t x = xx[0];
  const Double_t y = xx[1];
  const Double_t tmp1 = y-x*x;
  const Double_t tmp2 = 1-x;
  return 100*tmp1*tmp1+tmp2*tmp2;
}

TVectorD RosenBrockGrad(const TVectorD xx )
{
  const Double_t x = xx[0];
  const Double_t y = xx[1];
  TVectorD grad(2);
  grad[0]=-400 * x * (y - x * x) - 2 * (1 - x);
  grad[1]=200 * (y - x * x);
  return grad;
}


void Minimization()
{
#if defined(__CINT__) && !defined(__MAKECINT__) 
  cout << "WARNING: This tutorial can run only using ACliC, you must run it by doing: " << endl;
  cout << "cd  $ROOTSYS/tutorials/r/" << endl; 
  cout << "\t .x Minimization.C+" << endl; 
  return;
#endif
 ROOT::R::TRInterface &r=gR->Instance();
 
 //passsing RosenBrock function to R
 r["RosenBrock"]=ROOT::R::TRFunction(RosenBrock);
 
 //passsing RosenBrockGrad function to R
 r["RosenBrockGrad"]=ROOT::R::TRFunction(RosenBrockGrad);
 
 //the option "method" could be "Nelder-Mead", "BFGS", "CG", "L-BFGS-B", "SANN","Brent"
 //the option "control" lets you put some constraints like 
 //"maxit" The maximum number of iterations.
 //"abstol" The absolute convergence tolerance.
 //"reltol" Relative convergence tolerance.
 r.Parse("result <- optim( c(0.01,0.01), RosenBrock,method='BFGS',control = list(maxit = 1000000) )");
 
 //Getting results from R
 TVectorD  min=r.ParseEval("result$par").ToVector<Double_t>();
 
 std::cout.precision(8);
 //printing results
 std::cout<<"-----------------------------------------"<<std::endl;
 std::cout<<"Minimum x="<<min[0]<<" y="<<min[1]<<std::endl;
 std::cout<<"Value at minimum ="<<RosenBrock(min)<<std::endl;
 
 //using the gradient
 r.Parse("optimHess(result$par, RosenBrock, RosenBrockGrad)");
 r.Parse("hresult <- optim(c(-1.2,1), RosenBrock, NULL, method = 'BFGS', hessian = TRUE)");
 //getting the min calculated with the gradient
 TVectorD  hmin=r.ParseEval("hresult$par").ToVector<Double_t>();

 //printing results
 std::cout<<"-----------------------------------------"<<std::endl;
 std::cout<<"Minimization with the Gradient"<<endl;
 std::cout<<"Minimum x="<<hmin[0]<<" y="<<hmin[1]<<std::endl;
 std::cout<<"Value at minimum ="<<RosenBrock(hmin)<<std::endl;
 
}
