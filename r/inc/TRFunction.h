// @(#)root/r:$Id$
// Author: Omar Zapata   16/06/2013

/*************************************************************************
* Copyright (C) 2013,  Gfif Developers                                   *
* Grupo de Fenomenologia de Interacciones Fundamentales                  *
* http://gfif.udea.edu.co                                                *
* División de ciencias de la computación Gfifdev                         *
* Class Created and Maintained By Omar Andres Zapata Mesa                *
* All rights reserved.                                                   *
*                                                                        *
*                                                                        *
* For the list of contributors see $ROOTSYS/README/CREDITS.              *
*************************************************************************/
#ifndef ROOT_R_TRFunction
#define ROOT_R_TRFunction

#ifndef ROOT_R_RExports
#include<RExports.h>
#endif

//________________________________________________________________________________________________________
/**
   This is a base class to pass functions from ROOT to R


   @ingroup R
*/

namespace ROOT {
   namespace R {

      class TRInterface;
      class TRFunction: public TObject {
         friend class TRInterface;
         friend SEXP Rcpp::wrap<TRFunction>(const TRFunction &f);
      protected:
         Rcpp::InternalFunction *f;
      public:
         TRFunction(): TObject() {
            f = NULL;
         }

         TRFunction(const TRFunction &fun);
#if !defined(__CINT__)
         template<class T> TRFunction(T fun) {
            f = new Rcpp::InternalFunction(fun);
         }
#endif
//         operator SEXP(){return *f;}
         ClassDef(TRFunction, 0) //
      };
   }
}
#endif