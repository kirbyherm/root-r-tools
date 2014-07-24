#ifndef ROOT_RMinimizer
#define ROOT_RMinimizer

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

 

class   RMinimizer  :   public  ROOT::Math::BasicMinimizer    {
	protected:
		std::string fMethod;

	public:
RMinimizer(Option_t *method);
virtual ~RMinimizer();
virtual bool Minimize();
virtual void SetFunction(const ROOT::Math::IMultiGenFunction & func);
	private:
};


RMinimizer::~RMinimizer(){}
#endif
