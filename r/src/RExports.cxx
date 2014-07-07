/*************************************************************************
 * Copyright (C) 2013-2014, Omar Andres Zapata Mesa                      *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/
#include<RExports.h>
#include<TRFunction.h>
#include<Rcpp/Vector.h>
namespace Rcpp {
//TString
   template<> SEXP wrap(const TString &s)
   {
      return wrap(s.Data());
   }
   template<> TString as(SEXP s)
   {
      return TString(::Rcpp::as<const char *>(s));
   }
//TVectorD
   template<> SEXP wrap(const TVectorD &v)
   {
      std::vector<double> vec(v.GetMatrixArray(), v.GetMatrixArray() + v.GetNoElements());
      return wrap(vec);
   }

   template<> TVectorD as(SEXP v)
   {
      std::vector<Double_t> vec =::Rcpp::as<std::vector<Double_t> >(v);
      return TVectorT<Double_t>(vec.size(), vec.data());
   }
//TMatrixD
   template<> SEXP wrap(const TMatrixD &m)
   {
      Int_t rows = m.GetNrows();
      Int_t cols = m.GetNcols();
      Double_t *data = new Double_t[rows * cols];
      m.GetMatrix2Array(data, "F"); //ROOT has a bug here(Fixed)
      NumericMatrix mat(rows, cols, data);
      return wrap(mat);
   }

   template<> TMatrixD as(SEXP m)
   {
      NumericMatrix mat =::Rcpp::as<NumericMatrix>(m);
      return TMatrixD(mat.rows(), mat.cols(), mat.begin(), "F");
   }

   Char_t *as(SEXP str)
   {
      return const_cast<Char_t *>(Rcpp::as<std::string>(str).c_str());
   }
}
namespace ROOT {
   namespace R {
      VARIABLE_IS_NOT_USED SEXP ModuleSymRef = NULL;
   }
}