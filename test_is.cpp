#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "g.h"

using namespace std;

int * tab1;
bool * tab2;
int * tab3;
uint8_t * tab4;
int * valid_cones;

int main( int argc, char *argv[] ){
  clock_t start, stop;

  // get all variables from command line
  int is = atoi( argv[1] );

  uint8_t test = 0;

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
    min_d = y.min_degree();

    // create tables that precompute useful data for gluing
    int p = pow( 2, order );
    tab1 = new int[ p ];
    tab2 = new bool[ p ];
    //tab2 = new int[ p ];
    // tab1 = new uint64_t[ p ];
    tab3 = new int[ p ];
    tab4 = new uint8_t[p];
    
    // get all legit cones (those with out P3 endpoints)
    //int num_valid = y.get_p3s( tab0, p );
    //int valid_cones[ num_valid ];

    //cerr << "Done with legit cones" << endl;
      

    // test first IS
    cout << "Testing IS" << endl;
    start = clock();
    y.get_independences2( tab3, p, is-2 );
    stop = clock();
    cerr << "IS 2 took " <<  ((float)(stop - start))/((float)CLOCKS_PER_SEC) << endl;
    
    // test second IS
    start = clock();
    y.get_independences4( tab4, p, is-2 );
    stop = clock();
    cerr << "IS 4 took " <<  ((float)(stop - start))/((float)CLOCKS_PER_SEC) << endl;
    cout << "Sizes: " << sizeof(*tab3) << " " << sizeof(*tab4) <<  endl;

    int not_same = 0;
    for( int i = 0; i < p; i++ ){
      if( tab3[i] != tab4[i] ){
	not_same++;
	//	cout << i << ": " << tab1[i] << " " << tab2[i] << endl;
      }
    }
    cout << "Diff = " << not_same << endl;
    cout << p << endl;

    cout << endl;
    // test first P3
    cout << "Testing P3" << endl;
    start = clock();
    int c1 = y.get_p3s( tab1, p );
    stop = clock();
    cerr << "P3 1 took " <<  ((float)(stop - start))/((float)CLOCKS_PER_SEC) << endl;
    
    // test second P3
    start = clock();
    int c2 = y.get_p3s2( tab2, p );
    stop = clock();
    cerr << "P3 2 took " <<  ((float)(stop - start))/((float)CLOCKS_PER_SEC) << endl;

    cout << c1 << " " << c2 << endl;

    cout << "Sizes: " << sizeof(*tab1) << " " << sizeof(*tab2) <<  endl;
    /*// TEST CLOSURE 1
    start = clock();
    y.get_closures( tab1, p );
    stop = clock();
    cerr << "P3 1 took " <<  ((float)(stop - start))/((float)CLOCKS_PER_SEC) << endl;

    // TEST CLOSURE 2
    start = clock();
    y.get_closures2( tab2, p );
    stop = clock();
    cerr << "P3 1 took " <<  ((float)(stop - start))/((float)CLOCKS_PER_SEC) << endl;*/


    not_same = 0;
    for( int i = 0; i < p; i++ ){
      if( tab1[i] != tab2[i] ){
	not_same++;
	//	cout << i << ": " << tab1[i] << " " << tab2[i] << endl;
      }
    }


    cout << "Diff = " << not_same << endl;
    cout << p << endl;
    

    /* COUNTING CLIQUES FOR IS TESTS
    int count = 0;
    int clique_counts[order];

    for( int i = 0; i < order; i++ )
      clique_counts[i] = 0;

    for( uint64_t i = 0; i < p; i++ ){
      if( tab2[i] == 0 )
	count++;
      else{
	clique_counts[set_order(i)-1]++;
      }
    }

    for( int i = 0; i < order; i++ ){
      cout << i+1 << ": " << clique_counts[ i ] << endl;
    }

    cout << p << endl;
    cout << count << endl;

    int independs1[is];
    int independs2[is];
    for( int i = 0; i < is; i++ ){
      independs1[i] = 0;
      independs2[i] = 0;
    }
    
    for( int i = 0; i < p; i++ ){
      independs1[ tab1[i] ]++;
      independs2[ tab2[i] ]++;
    }

    cout << endl;
    for( int i = 0; i < is; i++ ){
      cout << i << ": " << independs1[i] << " " << independs2[i] << endl;
      } */
    
    delete [] tab1;
    delete [] tab2;
 
    cerr << endl;
      
    // if( ( g_count - 1 ) % 100 == 0 )
    //  cerr << g_count << ": " << ((float)(stop - start))/((float)CLOCKS_PER_SEC) << endl;
      
  }

  cerr << endl;
  return 0;
}
