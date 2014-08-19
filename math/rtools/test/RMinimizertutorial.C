
//set RMinimizer as the default minimizer and BFGS as the default method
ROOT::Math::MinimizerOptions mo;
mo.SetDefaultMinimizer("RMinimizer","BFGS");

//fits can now be run using RMinimizer in the same manner as usual 
//TF1->Fit(...) will implement RMinimizer

//to find a minimization point follow the same setup as described in tutorials/fit/NumericalMinimization.C
   ROOT::Math::Minimizer* min = 
   ROOT::Math::Factory::CreateMinimizer("RMinimizer", "BFGS");

   // set tolerance , etc...
   min->SetMaxFunctionCalls(1000000); // for Minuit/Minuit2 
   min->SetMaxIterations(10000);  // for GSL 
   min->SetTolerance(0.001);
   min->SetPrintLevel(1);

   // create funciton wrapper for minmizer
   // a IMultiGenFunction type 
   ROOT::Math::Functor f(&func,2); 
   double step[2] = {0.01,0.01};
   // starting point
    
   double variable[2] = { -1.,1.2};
   if (randomSeed >= 0) { 
      TRandom2 r(randomSeed);
      variable[0] = r.Uniform(-20,20);
      variable[1] = r.Uniform(-20,20);
   }
 
   min->SetFunction(f);
 
   // Set the free variables to be minimized!
   min->SetVariable(0,"x",variable[0], step[0]);
   min->SetVariable(1,"y",variable[1], step[1]);
 
   // do the minimization
   min->Minimize(); 
 

