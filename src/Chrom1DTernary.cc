#include <iostream>
#include <cstdlib>

using std::cout;
using std::endl;

#include "Chrom1DTernary.h"

Chrom1DTernary::Chrom1DTernary(int len) : _genes(len)
{
	_len = len;
	_mutateRate = __CHROM_MUTATE_1DT;
}

Chrom1DTernary::~Chrom1DTernary()
{
	
}

// ////////////////////////////////////////////////////////////////////////////
// Name: mutate
// Synop: preform random mutation operation on this chromosome
void Chrom1DTernary::mutate()
{
	
	unsigned int len = _genes.size();
	
	while(len > 0) { 
		// flip a coin to see if we mutate this gene
		if( drand48() < _mutateRate ) {
			
			// mutate according to the shift map
			//     (OFF, ON, NA) -> (ON, OFF, NA)
			//
			if( _genes[len] == OFF ) {
				_genes[len] = ON;
			} else if( _genes[len] == ON ) {
				_genes[len] = NA;
			} else if( _genes[len] == NA ) {
				_genes[len] = OFF;
			}
			
		}
		
		len--;
	}
	
}

// ////////////////////////////////////////////////////////////////////////////
// Name: crossover
// Synop: take the Chromosomes a blend it with us and place
//        the result in this Chromosome and in the other result in a
void Chrom1DTernary::crossover(Chromosome *a)
{
	Chrom1DTernary *p1 = (Chrom1DTernary*)a; // cast to our type
	unsigned int len, fir, last, i; 
	
	// Crossover is done using the 2 point method
	// moreover (this), and (a) are assumed to already be initialized
	// with the genetic material of the parents 
	
	len = _genes.size();
	
	if( p1->_genes.size() != len ) {
		// todo bitch more....
		throw;
	}
	
	fir = rand() % (len);	
	last = fir + (rand() % (len - fir));
	
	
	for( i = fir; i < last; i++ ) {
		
		Gene tmp = p1->_genes[i];
		p1->_genes[i] = _genes[i];
		_genes[i] = tmp;
		
	}
	
	
}

// ////////////////////////////////////////////////////////////////////////////
// Name: copyChrom
// Synop: preform deep copy of A into this
void Chrom1DTernary::copy( Chromosome *A )
{
	Chrom1DTernary *p1 = (Chrom1DTernary*)A;
	
	this->_genes = p1->_genes;
	
}

// ////////////////////////////////////////////////////////////////////////////
// Name: toString
// Synop: converts the given chromosome to a string for human readable
//        input output
std::string Chrom1DTernary::toString()
{
	int i = 0, len = _genes.size();
	std::string ret = "";
	
	for( ; i < len; i++ ) {
		switch(_genes[i]) {
			case ON:
				ret += '1'; break;
			case OFF:
				ret += '0'; break;
			case NA:
				ret += '*'; break;
			
			default: break;
		}
	}
	
	return ret;
	
}
