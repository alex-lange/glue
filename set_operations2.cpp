#include <vector>
#include <iostream>
#include "set_operations2.h"

using namespace std;

/* A set of functions for set operations when a set is a 64 bit unsigned
   integer, or a vset, which is a c++ vector os unsigned ints.

   All algorithms based off those described in Chapter 1 of the textbook
   "Combinatorial Algorithms" by Donald Kreher and Douglas Stinson
*/

// inserts bit into set
// algorithm 1.3
void set_insert( int u, vset &S ){
  // int j = ( u % intSize );
  // int i = u / intSize;
  S = S | ( shifter << u );
}

// deletes bit from set
// Algorithm 1.4
void set_delete( int u, vset &S ){
  //  int j = ( u % intSize );
  // int i = u / intSize;
  S = S & ~( shifter << u );
}


// removes bit from set
void set_cut( int u, vset &S ){
  vset x = S & ( ( shifter << ( u ) ) - 1 );
  vset y = S & ~( ( shifter << ( u + 1 ) ) - 1 );
  y = y >> 1;
  S = x | y;
}

// determines union of two sets
// Algorithm 1.6
vset set_union( vset S, vset R, int arraySize){
  vset T;
  T = S | R;
  return T;
}

// determines intersection of two sets
// Algorithm 1.7
vset set_intersection( vset S, vset R, int arraySize){
  vset T;
  T = S & R;
  return T;  
}

// completemnts the set
vset set_complement( vset S, int arraySize){
  vset T;
  T = ~S;
  return T;
}

/* gets number of bits in set using kernighan's method
int set_order( vset S, int arraySize ){
  int order = 0;
  for( int i = 0; i < arraySize; i++ ){
    uint64_t c;
    for( c = 0; S; c++ ){
      S &= S - 1;
    }
    order += c;
  }
  return order;
  }*/


// gets the first bit of set
int first_bit( vset S, int arraySize ){
  for( int i = 0; i < arraySize; i++ ){
    int j = -1;
    while( S ){
      // shifts right until 0
      S >>= 1;
      j++;
    }
    if( j >= 0 ) return i * intSize + ( intSize - j - 1 );
  }
  return -1;
}

// determines if a bit is in S (when S is just an int)
bool in_set( int u, uint64_t S ){
  if( S & (shifter << u ) ){
    return true;
  }else{
    return false;
  }
}

// gets number of bits in set using kernighan's method
// (when S is just an int)
int set_order( int S ){
  int c;
  for( c = 0; S; c++ ){
    S &= S - 1;
  }
  return c;
}
