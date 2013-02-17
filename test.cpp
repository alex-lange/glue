#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "g.h"

using namespace std;

int * tab0;
int * tab1;
int * tab2;
//uint64_t * tab1;
//uint64_t * tab2;
int * valid_cones;

int main( int argc, char *argv[] ){
  clock_t start, stop;

  // get all variables from command line
  //int is = atoi( argv[1] );

  vector<string> y_graphs;
  string g_string;

  // get the Y (k_{n-1}) graphs
  while( getline( cin, g_string ) ){
      y_graphs.push_back( g_string );
  }

  int g_count = 0;

  int order;
  int min_d;
    
  // glue!
  for( vector<string>::iterator yit = y_graphs.begin();
       yit != y_graphs.end(); yit++ ){

    g_count++;

    g y( (*yit)[0] - 63 );
    y.read_g6( *yit );
    order = y.order();

    cout << y.max_independent_set().size() << endl;
      
  }

  cerr << endl;
  return 0;
}
