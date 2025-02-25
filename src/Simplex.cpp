
#include "RcppEDMCommon.h"

//-------------------------------------------------------------
// 
//-------------------------------------------------------------
r::List Simplex_rcpp( std::string       pathIn,
                      std::string       dataFile,
                      r::DataFrame      dataFrame,
                      std::string       pathOut,
                      std::string       predictFile,
                      std::string       lib,
                      std::string       pred,
                      int               E,
                      int               Tp,
                      int               knn,
                      int               tau,
                      int               exclusionRadius,
                      std::string       columns,
                      std::string       target,
                      bool              embedded,
                      bool              const_predict,
                      bool              verbose,
                      std::vector<bool> validLib,
                      int               generateSteps,
                      bool              parameterList ) {

    SimplexValues S;

    if ( dataFile.size() ) {
        // dataFile specified, dispatch overloaded Simplex, ignore dataFrame
        S = Simplex( pathIn,
                     dataFile,
                     pathOut,
                     predictFile,
                     lib,
                     pred,
                     E, 
                     Tp,
                     knn,
                     tau,
                     exclusionRadius,
                     columns,
                     target, 
                     embedded,
                     const_predict,
                     verbose,
                     validLib,
                     generateSteps,
                     parameterList );
    }
    else if ( dataFrame.size() ) {
        DataFrame< double > dataFrame_ = DFToDataFrame( dataFrame );

        S = Simplex( dataFrame_,
                     pathOut,
                     predictFile,
                     lib,
                     pred,
                     E, 
                     Tp,
                     knn,
                     tau,
                     exclusionRadius,
                     columns,
                     target, 
                     embedded,
                     const_predict,
                     verbose,
                     validLib,
                     generateSteps,
                     parameterList );
    }
    else {
        Rcpp::warning( "Simplex_rcpp(): Invalid input.\n" );
    }

    r::DataFrame df_pred = DataFrameToDF( S.predictions );
    r::List output = r::List::create( r::Named("predictions")  = df_pred );

    if ( parameterList ) {
        // Have to explicitly build the named list
        r::List paramList;
        for ( auto pi =  S.parameterMap.begin();
                   pi != S.parameterMap.end(); ++pi ) {
            paramList[ pi->first ] = pi->second;
        }
        output["parameters"] = paramList;
    }

    return output;
}
