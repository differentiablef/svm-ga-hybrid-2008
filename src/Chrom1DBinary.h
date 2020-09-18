#ifndef CHROM1DBINARY_H_
#define CHROM1DBINARY_H_

#include "Chromosome.h"

#include <vector>
#include <string>

using std::string;
using std::vector;

const int DEF_CHROMSIZE = 231;
const double DEF_rate = 0.01;

class Chrom1DBinary : public Chromosome {
	private:
		typedef enum _basepair {
			OFF = 0,
			ON = 1
		} Gene;
		
		int _sz;
		double _mutrate;
		vector<Gene> v;
		
	public:
		Chrom1DBinary(int sz = DEF_CHROMSIZE) : v(_sz)
		{ 
			_sz = sz;
			_mutrate = DEF_rate;
		}
		
		~Chrom1DBinary() {}
	
	public:
		void set_mutation_rate(double rate) { _mutrate = rate; };
		double get_mutation_rate() { return _mutrate; };
	
		Gene get_gene(int pos) { return ( (pos > _sz || pos < 0)?  OFF : v[pos] ); }
		void set_gene(int pos, Gene state) { v[pos % _sz] = state; }
		void set_gene(int pos, int state) {
			switch(state) {
				case 0: v[pos%_sz] = OFF;
				case 1: v[pos%_sz] = ON;
				default: break;
			}
		}
	public:
		void mutate() 
		{
			for(int i = 0; i < _sz; i++) {
				if(drand48() < _mutrate) {
					
					if(v[i] == ON) {
						v[i] = OFF;
					} else {
						v[i] = ON;
					}
				}
				
			}
			
		}
		
		void crossover(Chromosome *u) {
			Chrom1DBinary *c1 = (Chrom1DBinary*) u;
			int first, last;
			
			first = rand() % _sz;
			last = rand() % _sz;
			
			if(first > last) {
				last ^= first;
				first ^= last;
				last ^= first;
			}//End if
			
			//Crossover
			for(int i = first; i < last; i++) {
				Gene temp = c1->v[i];
				c1->v[i] = this->v[i];
				this->v[i] = temp;
			}
			
		}
		
		void copy(Chromosome *u) {
			v = ((Chrom1DBinary*) u)->v;
		}
		
		string toString() {
			string result = "";
			
			for(int i = 0; i < _sz; i++) {
				result += v[i] + " ";
			}
			
			return result;
		}
};//End class definition

#endif /*CHROM1DBINARY_H_*/
