#include <iostream>
#include <string>
#include "ga_main.h"

#include "../src/Chrom1DTernary.h"

extern "C" {

#include "../SVMEngine/svm_record_tools.h"
#include "../SVMEngine/svm_engine.h"

}

#define DEBUG(x) { \
	std::cout << " "#x" = " << (x) << std::endl;\
}

using std::string;
using std::cout;
using std::endl;

#define SVM_CHROMLEN	231

class life : public LifeForm {
	
	public: // static member variables
		static SVM_Record traindat[3000];
		static int trainlen;
		static FeatureVector mask;
		
	protected: // protected member variables
		Chrom1DTernary chrom;
		
	public:
		life() : chrom(SVM_CHROMLEN) { };
		~life() { };
		
		// ////////////////////////////////////////////////////////////////////
		// Name: set_training_data
		// Synop: initialize the traindat array with info in filename
		void set_training_data(const char *fname) {
			if(trainlen == -1) {
				trainlen = parseTrainingData(fname, traindat);
				build_mask();
			}
		}
		
		void build_mask() {
			if( trainlen != -1 ) {
				int i = 0, j, stat;
				
				for( ; i < trainlen; i++ ) {
					for( j = 0; j < SVM_CHROMLEN; j++ ) {
						stat = FeatureVectorGetGene(traindat[i].svm_data, j);
						if( stat == 1 )
							FeatureVectorSetGene( mask, j+1, stat );
					}

				}
				
			}
		}
		
		// ////////////////////////////////////////////////////////////////////
		// Name: init
		// Synop: called to initialize new lifeform
		void init() { 
			int i = 0, ct=0;
			
			for( ; i < SVM_CHROMLEN; i++ ) {
				chrom.set_gene(i,2);
			}
			
			for( i = 0; i < SVM_CHROMLEN; i++ ) {
				if( FeatureVectorGetGene(mask, i) ) {
					if( drand48() < 0.6 ) {
						chrom.set_gene(i, rand()%3);
	
					}
				}
			}
			
			eval();
		};
		
		
	public: // Inherited interface
		
		// ////////////////////////////////////////////////////////////////////
		// Name: eval
		// Synop: takes this lifeform applys it to the training set and 
		//        preforms a fourfold training on the new data set.
		//
		void eval() 
		{
			SVM_Record trndat[ trainlen ];
			FourFoldCTX ctx;
			SVM_Model mdl;
			Measure train_rsts, cross_rsts;
			int stat, i = 0, j, trnlen = 0, keep, statm;
			
			// first we apply this chromosome to the training data set 
			// and copy it to a local var
			for( ; i < trainlen; i++ ) {
				
				keep = 1;
				
				for( j = 0; j < SVM_CHROMLEN && keep; j++ ) {
					
					stat = FeatureVectorGetGene(traindat[i].svm_data, j);
					trndat[trnlen].svm_classifier = traindat[i].svm_classifier;
					
					switch(chrom.get_gene(j)) {
						case 2: {
							FeatureVectorSetGene(
								trndat[trnlen].svm_data, 
								j+1, 
								stat
							);
						} break;
						
						case 1: {
							//if( stat == 1 ) {
								FeatureVectorSetGene(
									trndat[trnlen].svm_data, 
									j+1, 
									stat
								);
							//}// else {
							//	keep = 0;
							//}
						} break;
							
						case 0: {
							FeatureVectorSetGene(
								trndat[trnlen].svm_data, 
								j+1, 
								0
							);
						} break;
							
						default: {
							cout << "fuck you and your mother..." << endl;
						} break;
							
					}	
					
				
				}

				if( keep ) 
					trnlen++;
				
			}
			
			if( trnlen == 0 ) {
				set_fitness(0.0);
				return ;
			}
			
			mdl.mdl_type = Classification; // model type
			mdl.param_c = 1.0; // model cost parameter
			mdl.param_g = 0.1; // model gamma parameter
			
			// now we estimate the optimal cost parameter and initialize
			// the four fold context
			mdl.param_j = FourFold_Setup(&ctx, // context to setup
										 trndat, // training data 
										 trnlen, // length of training data
										 "cat"); // data filename
			
			
			// now we train using SVM_Model's parameters in the context
			// we setup
			stat = FourFold_EvalModel(&ctx, &mdl);
			
			// after the model has been evaluated we can extract the 
			// information about this models training sets and 
			// crossvalidation sets
			stat = FourFold_GetStatistics(&ctx, &train_rsts, &cross_rsts, NULL);
		
			// after evaluation we HAVE TO clean out the context
			FourFold_CleanUp(&ctx);	
			
			
			this->set_fitness(
				  (cross_rsts.Accuracy
				+ cross_rsts.Precision
				+ cross_rsts.Recall)/3.0);
			
		};
		
		void mutate() 
		{ 
			int i =0, stat, cur;
			for( ; i < SVM_CHROMLEN; i++ ) {
				stat = FeatureVectorGetGene( mask, i );
			
				if( stat ) {
					if( drand48() < chrom.get_mutation_rate() ) {
						cur = chrom.get_gene(i);
						chrom.set_gene(i, (cur+1) % 3);
					}
				} else {
					chrom.set_gene(i,2);
				}
			}
		};
		
		void mate(LifeForm *a) 
		{
			life *pl = (life*)a;
			if( drand48() < 0.60 ) {
				chrom.crossover(&(pl->chrom));
			}
		};
		
		void copy(LifeForm *a)
		{
			life *p =(life*)a;
			chrom.copy(&(p->chrom));
		};
		
		// ////////////////////////////////////////////////////////////////////
		// Name: toString
		// Synop: converts lifeform to a string
		std::string toString() { return chrom.toString(); }
		
		
};

SVM_Record life::traindat[3000];
int life::trainlen = -1;
FeatureVector life::mask = {0};

int POPSIZE = 200;

int main
(int argc, char **argv) {
	int i = 0;
	life tmp;
	std::vector< life > beg;
	GeneticAlgorithm< life> ga(POPSIZE);
	double maxft;
	svm_init();
	
	if( argc != 2 ) {
		cout << "Need svm data file name" << endl;
		return -1;
	}
	
	tmp.set_training_data(argv[1]);
	
	for( ; i < POPSIZE; i++ ) {
		tmp.init();
		beg.push_back(tmp);
	}
	
	ga.initial_population(beg);
	
	for(i = 0; i < 200; i++) {
		vector<life>::iterator pt, end;
		
		pt = ga.current_generation().begin();
		end = ga.current_generation().end();
		
		maxft =0.0;
		
		while(pt != end) {
			cout << pt->get_fitness() << "\t" << pt->toString() << endl;
			if( pt->get_fitness() > maxft ) {
				maxft = pt->get_fitness();
			}
			pt++;
		}
		cout << "MaxFit: " << maxft << endl;
		
		ga.iterate();
	}
	
	return 0;
}


int svm_engine_example(int argc, char **argv) {

	SVM_Record train[ 5000 ];
	int trainlen;
	// initialize svm engine
	svm_init();
	
	if( argc != 2 ) {
		cout << "Need svm cluster file" << endl;
		return -1;
	}
	
	// read in the training file and place contents into train
	trainlen = parseTrainingData(argv[1], train);
	DEBUG( trainlen );
	
	
	cout << "Results of svm_learn/svm_classify" << endl;
	{ // How to call svm_learn and svm_classify
		SVM_Model mdl; // model definition (parameters etc)
		Measure rsts; // struct to hold stats produced by classify
		int stat; // return status of svm_learn/svm_classify
		
		mdl.mdl_type = Classification; // model type
		mdl.param_c = 0.88; // model cost parameter
		mdl.param_g = 0.1; // model gamma parameter
		mdl.param_j = 3; // model cost ratio parameter
		
		stat = svm_learn(&mdl, // model definition
						 argv[1]); // training filename
		
		DEBUG(mdl.xialpha_error);
		DEBUG(mdl.xialpha_precision);
		DEBUG(mdl.xialpha_recall);
		DEBUG(mdl.support_vectors);
		DEBUG(mdl.learn_time);
		
		stat = svm_classify(&mdl, // trained model 
						    &rsts, // place for stats on model
						    argv[1]); // data file to classify
		
		DEBUG(rsts.Accuracy);
		DEBUG(rsts.Precision);
		DEBUG(rsts.Recall);
		
	}
	
	cout << endl << "Results of FourFold_EvalModel" << endl;
	
	{ // How to call and use the FourFold training system
		FourFoldCTX ctx; // four fold training context
		SVM_Model mdl; // training parameters
		Measure train_rsts, cross_rsts;
		int stat;
		
		mdl.mdl_type = Classification; // model type
		mdl.param_c = 0.88; // model cost parameter
		mdl.param_g = 0.1; // model gamma parameter
		
		// now we estimate the optimal cost parameter and initialize
		// the four fold context
		mdl.param_j = FourFold_Setup(&ctx, // context to setup
									 train, // training data 
									 trainlen, // length of training data
									 argv[1]); // data filename
		
		
		// now we train using SVM_Model's parameters in the context
		// we setup
		stat = FourFold_EvalModel(&ctx, &mdl);
		
		// after the model has been evaluated we can extract the 
		// information about this models training sets and 
		// crossvalidation sets
		stat = FourFold_GetStatistics(&ctx, &train_rsts, &cross_rsts, NULL);
		
		DEBUG(train_rsts.Accuracy);
		DEBUG(train_rsts.Precision);
		DEBUG(train_rsts.Recall);
		
		DEBUG(cross_rsts.Accuracy);
		DEBUG(cross_rsts.Precision);
		DEBUG(cross_rsts.Recall);
	
		// after evaluation we HAVE TO clean out the context
		FourFold_CleanUp(&ctx);	
	}
	
	return 0;
}


