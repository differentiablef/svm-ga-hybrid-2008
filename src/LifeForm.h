// ////////////////////////////////////////////////////////////////////////////
// Name: LifeForm.h
// Contains: Basic LifeForm Proto-class to be the parent of any subsequent 
//           lifeform definitions.
// Class Info:
//       Contained in this low level object definition of a lifeform are 
//       three key elements, first the fitness value, second an id 
//       tag--which is not required to be set but is a practical mehtod 
//       for tracking changes over lineages spanning more than one generation.
//
#ifndef LIFEFORM_H_
#define LIFEFORM_H_


// ////////////////////////////////////////////////////////////////////////////
// Class: LifeForm
// Synop: (see above)
class LifeForm
{
	
	public: // Constructor(s) & Deconstructor(s) 
		LifeForm();
		virtual ~LifeForm();
		
	public: // virtual interface do be implemented by derived classes
	
		// ////////////////////////////////////////////////////////////////////////////
		// Name: eval
		// Synop: evaluate the fitness of this lifeform
		virtual void eval() = 0;
		
		// ////////////////////////////////////////////////////////////////////////////
		// Name: mate
		// Synop: mate the two lifeforms a and b, and assign the result to this 
		virtual void mate(LifeForm *a) = 0;
		
		// ////////////////////////////////////////////////////////////////////////////
		// Name: mutate
		// Synop: randomly mutate this lifeform
		virtual void mutate() = 0;
		
		// ////////////////////////////////////////////////////////////////////////
		// Name: copy
		// Synop: provides deep copy of lifeform
		virtual void copy(LifeForm *)=0;
		
	public: // public interface
		inline unsigned int get_idtag() { return this->lf_id; };
		inline void set_idtag(unsigned int id) { this->lf_id = id; };
		inline double get_fitness() { return this->lf_fitness; };
		inline void set_fitness(double fit) { this->lf_fitness = fit; };
		
		
	private: // member variables (id, mother, father, fitness)
		double lf_fitness; // floating point fitness value
		unsigned int lf_id; // lifeform id tag
	
};

#endif /*LIFEFORM_H_*/
