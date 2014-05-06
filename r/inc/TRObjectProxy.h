// @(#)root/r:$Id$
// Author: Omar Zapata   29/05/2013

/*************************************************************************
 * Copyright (C) 2013-2014, Omar Andres Zapata Mesa                      *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/
#ifndef ROOT_R_TRObjectProxy
#define ROOT_R_TRObjectProxy

#ifndef ROOT_R_RExports
#include<RExports.h>
#endif
//________________________________________________________________________________________________________
/**
   This is a class to get ROOT's objects from R's objects


   @ingroup R
*/

namespace ROOT {
   namespace R {
      class TRObjectProxy: public TObject {
      private:
         Rcpp::RObject x;
      public:
         TRObjectProxy():TObject() {};
         TRObjectProxy(SEXP xx);

         void operator=(SEXP xx);
         TString ToString();
         template<class Type> Type ToScalar();
         template<class Type> TVectorT<Type> ToVector();
         template<class Type> std::vector<Type> ToStdVector();
         template<class TypeClass, class TypeData> TypeClass   ToArray();
         template<class Type> TMatrixT<Type>   ToMatrix();
         template <typename T> operator T() {
            return ::Rcpp::as<T>(x);
         }
         ClassDef(TRObjectProxy, 0) //
      };

   }

#ifndef __CINT__
//______________________________________________________________________________
   template<class Type> Type ROOT::R::TRObjectProxy::ToScalar()
   {
      //Returns R-scalars like integers and doubles as Int_t and Doublet_t.
      return ::Rcpp::as<Type>(x);
   }

//______________________________________________________________________________
   template<class Type> TVectorT<Type> ROOT::R::TRObjectProxy::ToVector()
   {
      //Returns R-vectors like TVectorT template with the indicated type.
      std::vector<Type> vec =::Rcpp::as<std::vector<Type> >(x);
      return TVectorT<Type>(vec.size(), vec.data());
   }

//______________________________________________________________________________
   template<class Type> std::vector<Type> ROOT::R::TRObjectProxy::ToStdVector()
   {
      return ::Rcpp::as<std::vector<Type> >(x);
   }
   
   
//______________________________________________________________________________
   template<class TypeClass, class TypeData> TypeClass   ROOT::R::TRObjectProxy::ToArray()
   {
      std::vector<TypeData> vec =::Rcpp::as<std::vector<TypeData> >(x);
      return TypeClass(vec.size(), vec.data());
   }

//______________________________________________________________________________
   template<class Type> TMatrixT<Type>   ROOT::R::TRObjectProxy::ToMatrix()
   {
      //Returns R-matrixes like TMatrixT template with the indicated type.
      Rcpp::NumericMatrix mat =::Rcpp::as<Rcpp::NumericMatrix>(x);
      return TMatrixT<Type>(mat.nrow(), mat.ncol(), mat.begin(), "F");
   }

#endif

}


#endif
