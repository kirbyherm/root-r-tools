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

      class   RMinimizer  :   public  ROOT::Math::BasicMinimizer    {
	protected:
		std::string fMethod;

	public:
RMinimizer(Option_t *method);
   virtual ~RMinimizer() {}
virtual bool Minimize();
// virtual void SetFunction(const ROOT::Math::IMultiGenFunction & func);
// virtual void SetFunction(const ROOT::Math::IMultiGradFunction & func) { BasicMinimizer::SetFunction(func);}
         virtual unsigned int NCalls() const; 
	private:
//         TVectorD fErrors;
         std::vector<double> fCovMatrix;
      };
//RMinimizer::~RMinimizer(){}

   }
}
#endif
