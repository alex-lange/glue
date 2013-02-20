#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <list>
#include <limits>

#include "g.h"
#include "/home/alex/software/nauty24r2/gtools.h"

using namespace std;

int * tab0;
int * tab1;
uint64_t * tab2;
int * tab3;
int num_cones;

int ** valid_cones;
int * tnum_cones;

int is, d, xe, e_min, e_max, y_p;
g * y_g;
bool check_all;
int y_order;

list<string> g_list;

int g_count;

double avg_time, min_time, max_time, cur_time;
double avg_numg, min_numg, max_numg, cur_numg;
double avg_conesize, min_conesize, max_conesize, cur_conesize;

char opt;


int g_out;

const double DBL_MAX = numeric_limits<double>::max( );

void add_graph( string g_string ){
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
  g_list.push_back( can_g.substr(0,can_g.size()-1));
  delete [] cstr;
}

void print_graphs( ostream * out ){
  g_out = 0;
  if( !g_list.empty() ){
    g_list.sort();
    string prev = g_list.front();
    *out << prev << endl;

    list<string>::iterator it = g_list.begin();
    it++;

    for( it; it != g_list.end(); it++ ){
      if( it->compare( prev ) != 0  ){
	g_out++;
	*out << *it << endl;
      }
      prev = *it;
    }
  }
}

void print_log( ostream * out ){
  *out << "Total out = " << g_out << endl;
  *out << endl;
  *out << "Time" << endl;
  *out << "Avg = " << avg_time << endl;
  *out << "Min = " << min_time << endl;
  *out << "Max = " << max_time << endl;
  *out << endl;
  *out << "Number of Graphs Glued" << endl;
  *out << "Avg = " << avg_numg << endl;
  *out << "Min = " << min_numg << endl;
  *out << "Max = " << max_numg << endl;
  *out << endl;
  *out << "Number of Cones per Glue" << endl;
  *out << "Avg = " << avg_conesize << endl;
  *out << "Min = " << min_conesize << endl;
  *out << "Max = " << max_conesize << endl;

  if( opt == 'r' ){

  }

}

void get_valid( int v, int s ){
  int num_to_check = tnum_cones[v-1];
  int num_valid = 0;
  bool good;
  int c;
  for( int i = 0; i < num_to_check; i++ ){
    c = valid_cones[v-1][i];
    if( !(s & c) ){
      valid_cones[v][num_valid] = c;
      num_valid++;
    }
  }
  for( int i = num_valid; i < num_cones; i++ ){
    valid_cones[v][i] = 0;
  }
 
  tnum_cones[v] = num_valid;
}

/* Recursive algorithm for gluing
    v = current vertex ( 0 <= v <= n-1)
    cones = vector of cones computed so far
    y_p = 2^order(y) (number of cones)
*/
void glue_bt( int v, vector<int> cones ){
  int s;
  bool in_edge = false;

  // the first 2 * xe vertices are in edges
  if( v < 2 * xe ){
    in_edge = true;
    if( v == 0 ) 
      s = 0;
    else if( v % 2 == 0 ) 
      s = cones[ v - 2] + 1;
    else 
      s = 0;
  }
  // the isolated vertices can have ordered cones
  else{
    if( v == 2*xe ) 
      s = 0;
    else
      s = cones[ v - 1 ] + 1;
  }

  for( int c = 0; c < tnum_cones[v]; c++ ){
    //  for( s; s < y_p; s++ ){
    // s = tab4[c];
    s = valid_cones[v][c];
  //    if( tab0[s] ){
      // forces minimum degree = d for X
    if( (in_edge && ( set_order( s ) >= d - 2 ) ) ||
	(!in_edge && ( set_order( s ) >= d - 1 ) ) ){
      
      // make sure no two cones intersect
      bool no_intersection = true;
      /*	for( int i = 0; i < v && no_intersection; i++ ){
		if( s & cones[i] )
		no_intersection = false;
		}*/
      if( no_intersection ){
	
	// if edge, make sure there's no vertex adjacent to both cones
	bool closure_okay = true;
	if( in_edge && v % 2 == 1 ){
	  if( tab2[s] & cones[v-1] )
	    closure_okay = false;
	}
	
	if( closure_okay ){
	  
	  // check independence number
	  bool no_is = true;
	  if( d >= 2 ){
	    for( int x_s = pow(2,v); x_s < pow(2,v+1) && no_is; x_s++ ){
	      if( set_order( x_s ) >= 2 ){
		// get all cones of x_s
		int c_is = s;
		for( int ii = 0; ii < v; ii++ ){
		  if( in_set( ii, x_s ) ){
		    c_is = c_is | cones[ii];
		  }
		}
		
		// if the indepence number of x_s + the independence number
		// of Y \ cones(x_s) creates IS of size is, BAD
		if( (tab3[x_s] + tab1[c_is]) >= is ){
		  no_is = false;
		}
	      }
	    }
	  }
	  
	  // if no IS, we're good!
	  if( no_is ){
	    cones[v] = s;
	    
	    // if at last vertex, we have a graph!!
	    if( v == d - 1 ){
	      g good( d + y_g->order() + 1 );
	      good.glue_graphs( d, xe, y_g, cones );
	      if( good.min_degree() == d ){
		add_graph( good.to_g6() );
		
		// Log
		cur_numg++;
		
	      }
	    }
	    else{
	      get_valid( v + 1, s );
	      glue_bt( v + 1, cones );
	    }
	  }
	}
      }
    }
  }
}

void drop_v(){
  int num_vs = y_g->order();
  for( int i = 0; i < num_vs; i++ ){
    vector< int > cut;
    cut.push_back(i);
    g y_copy = *y_g;
    y_copy.remove_vs( cut, 1 );
    add_graph( y_copy.to_g6() );
  }
}

void remove_neighbors( ostream * out ){
  int num_vs = y_g ->order();
  for( int i = 0; i < num_vs; i++ ){
    g y_copy = *y_g;
    vector<int> neighbors = y_copy.neighbors( i );
    if( y_copy.degree(i) != neighbors.size() ){
      cerr << "Error with neighbors" << endl;
    }
    neighbors.push_back(i);
    y_copy.remove_vs( neighbors, neighbors.size() );
    add_graph( y_copy.to_g6() );
  }
}

void remove_e( ostream * out ){
  bool found = false;
  int v = y_g->order();
  cerr << v << endl;
  for( int i = 0; i < v - 1 && !found; i++ ){
    for( int j = i + 1; j < v && !found; j++ ){
      if( y_g->is_edge( i, j ) ){
	y_g->remove_edge( i, j );
	
	
	vector<int> mis = y_g->max_independent_set();
	if( mis.size() < is ){
	  for( vector<int>::iterator it = mis.begin(); it != mis.end(); it++ ){
	    cerr << *it << " ";
	  }
	  cerr << endl;
	  add_graph( y_g->to_g6() );
	 *out << g_count << " (" << i 
	      << " " << j << ") " << endl;
	}
	y_g->add_edge(i,j);
	
      }
    }
  }
}

void compute_x_is(){
  tab3[0] = 0;
  
  for( int v = 0; v < d; v++ ){
    for( int x_s = pow(2,v); x_s < pow(2,v+1); x_s++ ){
      bool in_edge = false;

      int s = x_s & ~( 1 << v );
      
      // the first 2 * xe are in edges
      if( v < 2 * xe )
	in_edge = true;

      if( ( in_edge && v % 2 == 0 && in_set( v + 1, s ) ) ||
	  ( in_edge && v % 2 == 1 && in_set( v - 1, s ) ) )
	tab3[ x_s ] = tab3[ s ];
      else if( in_edge )
	tab3[ x_s ] = tab3[ s ] + 1;
      else
	tab3[ x_s ] = tab3[ s ] + 1;
    }
  }
}


int main( int argc, char *argv[] ){
  clock_t start, stop;
  bool all = false;
  string in_file, out_file, log_file;

  // get all variables from command line
  if( argc == 6 ){
    is = atoi( argv[1] );
    d = atoi( argv[2] );
    e_min = 0;
    e_max = 50;
    all = true;
    in_file = argv[3];
    out_file = argv[4];
    log_file = argv[5];
    opt = 'g';
  }
  else if( argc == 7 ){
    is = atoi( argv[2] );
    d = atoi( argv[3] );
    e_min = 0;
    e_max = 50;
    all = true;
    in_file = argv[4];
    out_file = argv[5];
    log_file = argv[6];
    opt = argv[1][1];
  }
  else if( argc == 8 ){
    is = atoi( argv[1] );
    d = atoi(argv[2]);
    e_min = atoi(argv[3]);
    e_max = atoi(argv[4]);
    all = true;
    in_file = argv[5];
    out_file = argv[6];
    log_file = argv[7];
  }
  else if( argc == 9 ){
    is = atoi( argv[1] );
    d = atoi(argv[2]);
    xe = atoi(argv[3]);
    e_min = atoi(argv[4]);
    e_max = atoi(argv[5]);
    in_file = argv[6];
    out_file = argv[7];
    log_file = argv[8];
  }
  else{
    cerr << "Error: Invalid args" << endl;
    return 0;
  }

  // set up log
  // log_file = out_file + ".log";
  avg_time = 0; min_time = DBL_MAX; max_time = 0;
  avg_numg = 0; min_numg = DBL_MAX; max_numg = 0;
  avg_conesize = 0; min_conesize = DBL_MAX; max_conesize = 0;


  check_all = true;
    
  vector<string> y_graphs;
  string g_string;
  ifstream ifs ( in_file.c_str() );
  if( !ifs.is_open() ){
    cerr << "Error opening file " + in_file << endl;
    return 0;
  }

  ofstream ofs( out_file.c_str() );
  if( !ofs.is_open() ){
    cerr << "Error opening " << out_file << endl;
    return 0;
  }

  ofstream log( log_file.c_str() );
  if( !log.is_open() ){
    cerr << "Error opening " << out_file << endl;
    return 0;
  }


  cerr << "****************" << endl;
  cerr << "is = " << is << endl;
  cerr << "d = " << d << endl;

  // get the Y (k_{n-1}) graphs
  //  while( getline( ifs, g_string ) ){
  //    y_graphs.push_back( g_string );
  //}

 

  g_count = 0;
  int d_ex = pow(2,d);
  tab3 = new int[ d_ex ];
  if( check_all ) compute_x_is();

  //  y_order = y_graphs[0][0] - 63;
  // tab5 = new int * [ y_order ];
  
    
  // glue!
  //for( vector<string>::iterator yit = y_graphs.begin();
  //     yit != y_graphs.end(); yit++ ){
  while( getline( ifs, g_string ) ){ 
    g_count++;

    start = clock();
    
    g y( g_string[0] - 63 );
    y.read_g6( g_string );
    y_order = y.order();

    // reset currents
    cur_time = 0; cur_numg = 0; cur_conesize = 0;

    // if opt is d we're dropping vertices, not gluing
    if( opt == 'd' ){
      y_g = &y;
      drop_v();
    }
    // if opt is n, we're dropping vertex and it's neighborhood
    // (this is the opposite of gluing
    if( opt == 'n' ){
      y_g = &y;
      remove_neighbors( &log );
    }
    // if opt is r, we're removing every edge (once)
    else if( opt == 'r' ){
      y_g = &y;
      remove_e( &log );
    }

    // if d = 0, we just add a vertex to Y (no backtracking)
    else if( d == 0 ){
      g good( y.order() + 1 );
      vector<int> cones;
      good.glue_graphs( d, xe, &y, cones );
      good.print_g6( &ofs );
    }

    // else, we need to actually glue
    else{
      // create tables that precompute useful data for gluing
      y_p = pow( 2, y.order() );
      tab0 = new int[ y_p ];
      tab1 = new int[ y_p ];
      tab2 = new uint64_t[ y_p ];
      
      // get all legit cones (those with out P3 endpoints)
      num_cones = y.get_p3s2( tab0, y_p );

      int cone_ind = 0;
      tnum_cones = new int[ y_order ];
      tnum_cones[0] = num_cones;
      valid_cones = new int*[y_order];
      for( int i = 0; i < y_order; i++ ){
	valid_cones[i] = new int[ num_cones ];
      }
      for( int i = 0; i < y_p; i++ ){
	if( tab0[i] ){
	  valid_cones[0][ cone_ind ] = i;
	  cone_ind++;
	}
      }

      // get all independent numbers of V(Y) \ S
      y.get_independences4( tab1, y_p, is - 2 );
      
      // get all vertices that are adjacent to v \in S
      y.get_closures2( tab2, y_p );
      
      y_g = &y;
     
      //glue( d, xe, e_min, e_max, &y, p );
      if( all ){
	for( int i = 0; i <= d / 2; i++ ){
	  vector<int> cones( y.order(), 0);
	  xe = i;
	  compute_x_is();

	  // Glue!
	  glue_bt( 0, cones );

	  tnum_cones[0] = num_cones;
	  cone_ind = 0;
	  for( int j = 0; j < y_p; j++ ){
	    if( tab0[j] ){
	      valid_cones[0][ cone_ind ] = j;
	      cone_ind++;
	    }
	  }
	}
      }
      else{
	vector<int> cones( y.order(), 0);
	glue_bt( 0, cones );
      }

      
      delete [] tab0;
      delete [] tab1;
      delete [] tab2;
      //   delete [] tab4;
      delete [] tnum_cones;
      for( int i = 0; i < y_order; i++ ){
	delete [] valid_cones[i];
      }
      delete [] valid_cones;
    }

    stop = clock();

    cur_time = ((float)(stop - start))/((float)CLOCKS_PER_SEC);
    avg_time += cur_time;
    avg_numg += cur_numg;
    avg_conesize += cur_conesize;
    if( cur_time < min_time )
      min_time = cur_time;
    if( cur_time > max_time )
      max_time = cur_time;
    if( cur_numg < min_numg )
      min_numg = cur_numg;
    if( cur_time > max_numg )
      max_numg = cur_numg;
      
    //if( ( g_count - 1 ) % 100 == 0 )
          cerr << g_count << ": " << cur_time << endl;
    //cerr << ((float)(stop - start))/((float)CLOCKS_PER_SEC) << endl;
 
  }
  
  avg_time = avg_time / g_count;
  avg_numg = avg_numg / g_count;
  avg_conesize = avg_conesize / g_count;
  
  
  print_graphs( &ofs );
  print_log( &log );
  ifs.close();
  ofs.close();
  log.close();

  cerr << endl;
  delete [] tab3;
  return 0;
}
