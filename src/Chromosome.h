// ////////////////////////////////////////////////////////////////////////////
// Name: Chromosome.h
// Contains: Abstract Chromosome class definition
// Class Info: A Choromosome is a low level container of genetic material
//             which has the ability to crossover with another Chromosome
//             and can randomly mutate into another chromosome.
//             as such the class definition of a chromosome requires that 
//             you define the two functions mutate and crossover
//
#ifndef CHROMOSOME_H_
#define CHROMOSOME_H_

#include <string>

class Chromosome
{
	public: // Basic Constructors and Deconstructors
		Chromosome();
		virtual ~Chromosome();
		
	public: // Virtual interface
		// ////////////////////////////////////////////////////////////////////////////
		// Name: mutate
		// Synop: preform random mutation operation on this chromosome
		virtual void mutate() = 0;
		
		// ////////////////////////////////////////////////////////////////////////////
		// Name: crossover
		// Synop: take the Chromosomes a, and blend them with us and place
		//        the result in this Chromosome and in the other result in a
		virtual void crossover(Chromosome *a) = 0;
		
		// ////////////////////////////////////////////////////////////////////////////
		// Name: copyChrom
		// Synop: preform deep copy of A into this
		virtual void copy( Chromosome *A ) = 0;
		
		// ////////////////////////////////////////////////////////////////////////////
		// Name: toString
		// Synop: converts the given chromosome to a string for human readable
		//        input output
		virtual std::string toString() = 0;
	
	protected: //
	
};

#endif /*CHROMOSOME_H_*/
