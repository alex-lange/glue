#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <map>

#include "g.h"
#include "/home/alex/software/nauty24r2/gtools.h"

using namespace std;

int main( int argc, char *argv[] ){
  string compare_to, comparing, results;
  clock_t start, stop;
  int is;

  if( argc == 5 ){
    is = atoi( argv[1] );
    comparing = argv[2];
    compare_to = argv[3];
    results = argv[4];
  }
  else{
    cerr << "Error: Invalid args" << endl;
    return 0;
  }

  ifstream in_compare( compare_to.c_str() );
  ifstream in_test( comparing.c_str() );
  ofstream out_results( results.c_str() );

  map<string,int> compared_graphs;
  string g_string;
  out_results << "comparing " << comparing << " to " << compare_to << endl;
  out_results << "graph# e_removed found?" << endl;

  int g_read = 0;

  while( getline( in_compare, g_string ) ){
    g_read++;
    int m = 1;
    int len = g_string.length();
    char * cstr = new char[len];
    for( int i = 0; i < len; i++ ){
      cstr[i] = g_string[i];
    }
    int n = graphsize( cstr );
    graph inG[m*n];
    graph outG[m*n];
    stringtograph( cstr, inG, m );
    fcanonise(inG, m, n, outG, NULL, 0 );
    char * out_cstr = ntog6( outG, m, n );
    string can_g( out_cstr );
    compared_graphs.insert( pair<string,int>( can_g, g_read ) );
  }



  string g_string_initial;
  int g_count = 0;
  double total_time;
  double cur_time;
  while( getline( in_test, g_string_initial ) ){

    start = clock();

    g_count++;
    int v = g_string_initial[0] - 63;
    g y( v );
    y.read_g6( g_string_initial );
    bool found = false;
    for( int i = 0; i < v - 1 && !found; i++ ){
      for( int j = i + 1; j < v && !found; j++ ){

	if( y.is_edge( i, j ) ){
	  y.remove_edge( i, j );

	  if( y.max_independent_set( ).size() < is ){

	    out_results << g_count << " (" << i 
			<< " " << j << ") ";
	  
	    int m = 1;
	    g_string = y.to_g6();
	    int len = g_string.length();
	    char * cstr = new char[len];
	    for( int k = 0; k < len; k++ ){
	      cstr[k] = g_string[k];
	    }
	    int n = graphsize( cstr );
	    graph inG[m*n];
	    graph outG[m*n];
	    stringtograph( cstr, inG, m );
	    fcanonise(inG, m, n, outG, NULL, 0 );
	    char * out_cstr = ntog6( outG, m, n );
	    string can_g( out_cstr );

	    map<string,int>::iterator it = compared_graphs.find( can_g );
	    if( it != compared_graphs.end() ){
	      out_results << "1." << it->second << endl;
	    }
	    else{
	      out_results << "0." << endl;
	    }

	  }
	  y.add_edge(i,j);
	    
	}
      }
    }

    stop = clock();
    cur_time = ((float)(stop - start))/((float)CLOCKS_PER_SEC);
    total_time += cur_time;
    cout << g_count << " " << cur_time << endl;
  }

  out_results << "* " << g_count << " to " << compared_graphs.size()
	      << " in " << total_time << "s" << endl;
    
  in_compare.close();
  in_test.close();
  out_results.close();
  

}
