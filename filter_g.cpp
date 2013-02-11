#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <set>
#include <math.h>

#include "g.h"

void less_memory( ifstream * ifs, int e, string in_file ){
  stringstream ss;  ss << e;
  string out_file = in_file + "_" + ss.str();
  ofstream ofs( out_file.c_str() );

  string g_string;
  while( getline( *ifs, g_string ) ){
    g y( g_string[0] - 63 );
    y.read_g6( g_string );
    int edge_num = y.num_edges();
    if( edge_num == e )
      ofs << g_string << endl;

  }
}

int main( int argc, char *argv[] ){
  string in_file;
  string out_file;
  int max_e;
  char opt = ' ';
  
  if( argc == 3 ){
    in_file = argv[2];
    max_e = atoi( argv[1] );
  }
  else if( argc == 4){
    opt = argv[1][1];
    in_file = argv[3];
    max_e = atoi( argv[2] );
  }
  else{
    cerr << "Incorrect arg number" << endl;
    return 1;
  }

  ifstream ifs( in_file.c_str() );
  string g_string;
  int edge_nums[max_e];
  vector<string> graphs[max_e];

  // opt m is for large graphs where we don't want to load them all into mem
  if( opt == 'm' ){
    less_memory( &ifs, max_e, in_file );
    return 0;
  }

  int edge_min = max_e;

  for( int i = 0; i < max_e; i++ )
    edge_nums[i] = 0;

  while( getline( ifs, g_string ) ){
    g y( g_string[0] - 63 );
    y.read_g6( g_string );
    int edge_num = y.num_edges();

    edge_nums[edge_num]++;
    graphs[edge_num].push_back( g_string );

    if( edge_num < edge_min )
      edge_min = edge_num;
  }

  cout << "Edge Counts" << endl;

  for( int i = edge_min; i < max_e; i++ ){
    if( edge_nums[i] > 0 ){
      cout << " " << i << ": " << edge_nums[i] << endl; 
      stringstream ss;
      ss << i;
      string out_file = in_file + "_" + ss.str();
      ofstream ofs( out_file.c_str() );
      for( vector<string>::iterator it = graphs[i].begin();
	   it != graphs[i].end(); it++ ){
	
	ofs << *it << endl;

      }
      ofs.close();
    }
  }
}
