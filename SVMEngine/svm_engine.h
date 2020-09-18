#ifndef _SVM_ENGINE_H
#define _SVM_ENGINE_H


#include "chrom_tools.h"
#include "svm_record_tools.h"

// /////////////////////////////////////////////////////////////////////////////
// Definitions: SVM_MAXITER
// Synop: indicates the maximum number of iterations for svm learn 
//		  (svmlight's default is 100000)
//
#define SVM_MAXITER		100000
// /////////////////////////////////////////////////////////////////////////////
// Type: Model_Type
// Synop: enum type for different types of svm models we will compute
//
typedef enum _Mdl_Type {
	Classification = 0,
	Regression
} Model_Type;

// /////////////////////////////////////////////////////////////////////////////
// Type: Measure
// Synop: contains evaluation information for a specific set of svm predicitons
//
typedef struct _measure {
	float Accuracy;
	float Precision;
	float Recall;
} Measure;

// /////////////////////////////////////////////////////////////////////////////
// Type: SVM_Model
// Synop: contains information about the model we wish to compute or the 
//        model that has already been computed
typedef struct _svmmodel {
	Model_Type mdl_type;
	
	double param_g; // used in all model types
	double param_c;
	
	union _params {
		struct _regress {
			double w; // tube width (regression)
		} reg_param;
		
		struct _class {
			double j; // j cost factor
		} cla_param;
	} param;

#define param_w		param.reg_param.w
#define param_j		param.cla_param.j

	double learn_time;
	
	int support_vectors;
	double xialpha_recall;
	double xialpha_precision;
	double xialpha_error;
	double vc_dim;
	
	char *mdl_file; // should be an array, but this is easier
	
} SVM_Model;

// /////////////////////////////////////////////////////////////////////////////
// Name: svm_init
// Synop: setup enviorment for execution of svm programs using our engine
//
void svm_init();

// /////////////////////////////////////////////////////////////////////////////
// Name: svm_classify
// Synop: make a call to svm_classify using modelFile as the svm model,
//        and predFile as the place to store the predictions, and datFile as
//        its input, filling results with the appropriate infomation 
//        from svm_classify and returning the exit status of svm_classify
//        if results == NULL then the output of svm_classify is ignored
int svm_classify(SVM_Model *mdl, Measure *results, char *datFile/*, char *predFile*/);

// /////////////////////////////////////////////////////////////////////////////
// Name: svm_learn
// Synop: calls svm_learn and setups a model using trainFile as the example
//        set and mdlFile as the place to put the model, it also will set up
//        any needed fields in SVM_Model
int svm_learn(SVM_Model *mdl, char *trainFile);


// /////////////////////////////////////////////////////////////////////////////
// ///////////////////// Begin Four Fold ///////////////////////////////////////

#define KN_TRAINFILE_FMT	"%s_train_k%d-%d"
#define KN_CROSSFILE_FMT	"%s_cross_k%d-%d"

// /////////////////////////////////////////////////////////////////////////////
// Type Definition: FourFoldCTX
// Synop: execution context for 4 fold training, contained within is all the 
//        necessary information to evaluate the training method
typedef struct _4fld_train {
	int numpos, numneg;

	char trainingFiles[4][400]; // the four training profiles;
	char crossvalFiles[4][400]; // the four crossvalidation profiles
	char *testingFile;
	
	char *masterTrainingFile;
	
	Measure trainingResults[4];
	Measure testingResults[4];
	Measure crossvalResults[4];
	
	SVM_Model *model; // last model trained with
	
	double learn_time;
	
	char tmp_model[200];
	char tmp_predictions[200];
	
} FourFoldCTX;

// /////////////////////////////////////////////////////////////////////////////
// Name: FourFold_Setup
// Synop: must be called before preforming the four fold training. This 
//        function setups the FourFoldCTX evaulation enviorment and 
//        gets the system ready to do the fourfold training, it reads in
//        trainDat, divides it into positive and negative classes,
//        and then builds the four sets of training and crossvalidation 
//        pairs
double FourFold_Setup
(FourFoldCTX *ctx, SVM_Record trainDat[], int tlen, char *trainingFile);


// /////////////////////////////////////////////////////////////////////////////
// Name: FourFold_CleanUp
// Synop: Remove and cleanup the context and enviorment
void FourFold_CleanUp
(FourFoldCTX *ctx);

// /////////////////////////////////////////////////////////////////////////////
// Name: FourFold_EvalModel
// Synop: evaluate mdl, using fourfold training
int FourFold_EvalModel
(FourFoldCTX *ctx, SVM_Model *mdl);

// /////////////////////////////////////////////////////////////////////////////
// Name: FourFold_GetStatistics
// Synop: collect the statistics generated from the previous fourfold 
//        evaluation
int FourFold_GetStatistics
(FourFoldCTX *ctx, Measure *train, Measure *cross, Measure *test);

// /////////////////////// End Four Fold ///////////////////////////////////////
// /////////////////////////////////////////////////////////////////////////////


#endif /* _SVM_ENGINE_H */

