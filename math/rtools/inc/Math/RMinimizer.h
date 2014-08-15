// Author: K. Hermansen and L. Moneta, Aug 2014 

// Implementation file for class RMinimizer

#ifndef ROOT_Math_RMinimizer
#define ROOT_Math_RMinimizer

#ifndef ROOT_TRInterface
#include "TRInterface.h"
#endif

#ifndef ROOT_Math_Functor
#include "Math/Functor.h"
#endif

#ifndef ROOT_Math_IParamFunctionfwd
#include "Math/IParamFunctionfwd.h"
#endif

#ifndef ROOT_Math_BasicMinimizer
#include "Math/BasicMinimizer.h"
#endif

namespace ROOT {
   namespace Math{	

      /*! \brief RMinimizer class.
       *
       *    Minimizer class that uses the ROOT/R interface to pass functions and minimize them in R.
       */
      class   RMinimizer  :   public  ROOT::Math::BasicMinimizer    {
         protected:
            std::string fMethod; /*!< minimizer method to be used, must be of a type listed in R optim or optimx descriptions */
         
         private:
            TVectorD    fErrors; /*!< vector of parameter errors */
            TMatrixD    fCovMatrix; /*!< covariant matrix */
            TMatrixD    fHessMatrix; /*!< Hessian matrix */
         
         public:
            /*! \brief Default constructor
             *
             * Default constructor with option for the method of minimization, can be any of the following:
            *"Nelder-Mead", "BFGS", "CG", "L-BFGS-B", "SANN", "Brent" (Brent only for 1D minimization)
            *
            * See R optim or optimx descriptions for more details and options
            */	
            RMinimizer(Option_t *method);
            ///Destructor
            virtual ~RMinimizer() {}
            ///Function to find the minimum
            virtual bool Minimize();
            ///Returns the number of function calls
            virtual unsigned int NCalls() const;
            ///Returns the ith jth component of the covariant matrix
            double CovMatrix(unsigned int i, unsigned int j) const;
            ///Returns the vector of parameter errors
//            TVectorD Errors() const;
            ///Returns the ith jth component of the Hessian matrix
            double HessMatrix(unsigned int i, unsigned int j) const;

      };

   }
}
#endif
