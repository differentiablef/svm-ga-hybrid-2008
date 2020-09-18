#ifndef GENETICALGORITHM_H_
#define GENETICALGORITHM_H_

#include <vector>
#include <cstdlib>
#include <time.h>

using std::vector;

template<class SimpleLife>
class GeneticAlgorithm {
	public:
		GeneticAlgorithm(unsigned int popsize)
		{
			_popsz = popsize;
			_curlfpos = 0;
			_fitsum = 0.0;
			// seed random number generator
			srand48( time( NULL ) );
			srand( time( NULL ) );
		}
		
		~GeneticAlgorithm() { };
		
	public: // public component interface for building _nextgen
		// ////////////////////////////////////////////////////////////////////////
		// Name: tournament
		// Synop: preform a trounament by grouping lifeforms into groups of size
		//        groups and place the winner into _nextgen. Each winner is placed
		//        into _nextgen at a position of pos=pos + i/groups
		void tournament(int groups, unsigned int pos) 
		{
			unsigned int i = 0;
			int j, win;
			double mx;
			SimpleLife tmp;
			
			if( pos >= _popsz ) {
				// TODO: add real out of bound exception
				throw; 
			}
				
			for(; i < _popsz; i+=groups) { 
				mx = _curgen[i].get_fitness();
				win = i;
				
				for( j =0; j< groups; j++ ) {
					
					if(mx < _curgen[i+j].get_fitness()){
						
						win = i+j;
						mx = _curgen[win].get_fitness();
						
					}
					
				}
				
				tmp.copy(&_curgen[win]);
				
				_nextgen[pos++].copy(&tmp);
				
				
			}
			
		}
		
		// ////////////////////////////////////////////////////////////////////////
		// Name: permute_population
		// Synop: randomly swap lifeforms in _nextgen 
		void permute_population() 
		{ 
			SimpleLife tmp;
			unsigned int idx[_popsz], i = 0;
			
			for( ; i < _popsz; i++ ){
				idx[i] = i;
			}
			
			for( i = 0; i <_popsz; i++) {
				int id = rand() % _popsz;
				
				idx[i] ^= idx[id];
				idx[id] ^= idx[i];
				idx[i] ^= idx[id];
				
			}
			
			for( i = 0; i < _popsz; i++ ) {
				
				tmp.copy(&_nextgen[i]);
				_nextgen[i].copy(&_nextgen[idx[i]]);
				_nextgen[idx[i]].copy(&tmp);
			
			}
			
		}
		
		// ////////////////////////////////////////////////////////////////////////
		// Name: random_selection
		// Synop: randomly select N lifeforms from _curgen using roulette wheel
		//        placing the suriviors into _nextgen starting at pos
		void random_selection(unsigned int N, unsigned int pos)
		{
			unsigned int i = 0,j;
			double sum, max;
			
			if( N+pos >= _popsz ) {
				// TODO: thorw real out of bounds exception
				throw;
			}
			
			for(; i < N; i++) {
				
				max = drand48() * _fitsum;
				sum = 0.0;
				for(j = 0; j < _popsz; j++) {
					
					sum += _curgen[j].get_fitness();
					
					if( sum >= max )
						break;
					
				}
				
				if( j == _popsz )
					j--;
					
				_nextgen[pos++].copy(&_curgen[j]);
					
			}
			
			
		}
		
	public:
		// provide the initial population to the algorithm
		void initial_population(vector<SimpleLife> &init) {
			unsigned int sz = init.size(), i =0;
			
			if( sz != _popsz ) {
				// TODO: make this throw real exception
				throw;
			}
			
			// set generation count to 0
			_genct = 0;
		
		
			// compute initial fitness sum
			_fitsum = 0.0;
			for(; i < sz; i++)
				_fitsum += init[i].get_fitness();
			
			// initialize _curgen and _nextgen
			_curgen = init;
			_nextgen = init;
			
		}
		
		// ////////////////////////////////////////////////////////////////////////
		// Name: iterate
		// Synop: build the _nextgen from _curgen, and mate the result into 
		//        _curgen
		void iterate() {
			unsigned int i = 0;

			// two person touranment
			tournament(2, 0);
			
			// four person tournament
			tournament(4,_popsz/2);
			
			// randomly select the last fourth of _nextgen
			random_selection(_popsz/4, _popsz/2+_popsz/4 - 1);

			permute_population();
			
			_fitsum = 0.0;
			
			for( i = 0; i < _popsz; i+=2 ) {
				_curgen[i].copy(&(_nextgen[i]));
				_curgen[i+1].copy(&(_nextgen[i+1]));
				
				_curgen[i].mate(&_curgen[i+1]);
				
				_curgen[i].mutate();
				_curgen[i+1].mutate();
				
				_curgen[i].eval();
				_curgen[i+1].eval();
				
				_fitsum += _curgen[i].get_fitness();
				_fitsum += _curgen[i+1].get_fitness();
				
			}
			
			_genct++;
			
		}
		
		std::vector< SimpleLife > &current_generation()
		{
			return _curgen;
		};
		
	protected:
		double _fitsum;
		unsigned int _popsz, _genct,_curlfpos;
		std::vector< SimpleLife > _curgen;
		std::vector< SimpleLife > _nextgen;
	
};


#endif /*GENETICALGORITHM_H_*/
