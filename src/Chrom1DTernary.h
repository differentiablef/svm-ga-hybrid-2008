#ifndef CHROM1DTRINARY_H_
#define CHROM1DTRINARY_H_

#include <vector>
#include <string>

using std::vector;
using std::string;

#define __CHROM_SZ_1DT	231 // Default length of Chromosome if none given
#define __CHROM_MUTATE_1DT	0.01 // Default mutation rate

#include "Chromosome.h"

class Chrom1DTernary : public Chromosome
{
	public: // local typedefs
	
		typedef enum _BasePair {
			OFF = 0,
			ON = 1,
			NA = 2
		} Gene;
	
	public:
	
		Chrom1DTernary(int len = __CHROM_SZ_1DT);
		~Chrom1DTernary();
		
	public: // inherited interface
		// ////////////////////////////////////////////////////////////////////////////
		// Name: mutate
		// Synop: preform random mutation operation on this chromosome
		void mutate();
		
		// ////////////////////////////////////////////////////////////////////////////
		// Name: crossover
		// Synop: take the Chromosomes a blend it with us and place
		//        the result in this Chromosome and in the other result in a
		void crossover(Chromosome *a);
		
		// ////////////////////////////////////////////////////////////////////////////
		// Name: copy
		// Synop: preform deep copy of A into this
		void copy( Chromosome *A );
		
		// ////////////////////////////////////////////////////////////////////////////
		// Name: toString
		// Synop: converts the given chromosome to a string for human readable
		//        output
		std::string toString();
	
	public: // local interface
		double get_mutation_rate() { return _mutateRate; };
		void set_mutation_rate(double r0) { _mutateRate = r0; };
		
		void set_gene(int pos, int state) {
			switch(state) {
				case 0: _genes[pos%_len] = OFF; break;
				case 1: _genes[pos%_len] = ON; break;
				case 2: _genes[pos%_len] = NA; break;
				default: break;
			}
		}
		void set_gene(int pos, Gene state) { _genes[ pos % _len ] = state; }
		
		Gene get_gene(int pos) {
			return ( (pos > _len || pos < 0)?  OFF : _genes[pos] ); 
		}
	
	protected: // local member variables
		int _len;
		double _mutateRate;
		vector<Gene> _genes;

};

#endif /*CHROM1DTRINARY_H_*/
