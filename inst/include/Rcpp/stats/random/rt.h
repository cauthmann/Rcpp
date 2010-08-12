// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 4 -*-
//
// rt.h: Rcpp R/C++ interface class library -- 
//
// Copyright (C) 2010 Douglas Bates, Dirk Eddelbuettel and Romain Francois
//
// This file is part of Rcpp.
//
// Rcpp is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Rcpp is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Rcpp.  If not, see <http://www.gnu.org/licenses/>.

#ifndef Rcpp__stats__random_rt_h
#define Rcpp__stats__random_rt_h

namespace Rcpp {
namespace stats {

class TGenerator : public ::Rcpp::Generator<false,double> {
public:
	
	TGenerator( double df_ ) : df(df_), df_2(df_/2.0) {}
	
	inline double operator()() const {
		/* Some compilers (including MW6) evaluated this from right to left
		return norm_rand() / sqrt(rchisq(df) / df); */
		double num = norm_rand();
		
		// return num / sqrt(rchisq(df) / df);
		// replaced by the followoing line to skip the test in 
		// rchisq because we already know
		return num / ::sqrt( ::Rf_rgamma(df_2, 2.0) / df);
	}
	
private:
	double df, df_2 ;
} ;
} // stats

inline NumericVector rt( int n, double df ){
	// special case
	if (ISNAN(df) || df <= 0.0)
		return NumericVector( n, R_NaN ) ;
	
	// just generating a N(0,1)
	if(!R_FINITE(df))
		return NumericVector( n, norm_rand ) ;
	
	// general case
	return NumericVector( n, stats::TGenerator( df ) ) ;
}

} // Rcpp

#endif