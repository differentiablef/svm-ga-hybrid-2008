
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>

#include "config.h"
#include "common.h"
#include "svm_engine.h"

#include "svm_light/svm_common.h"
#include "svm_light/svm_learn.h"


KERNEL_CACHE *__kernel_cache;
LEARN_PARM __learn_parm;
KERNEL_PARM __kernel_parm;
MODEL *__model = NULL;

// /////////////////////////////////////////////////////////////////////////////
// Name: svm_init
// Synop: setup enviorment for execution of svm programs using our engine
//
void svm_init
() {
	
	strcpy (__learn_parm.alphafile, ""); 
	verbosity=1;
	__learn_parm.biased_hyperplane=1;
	__learn_parm.sharedslack=0;
	__learn_parm.remove_inconsistent=0;
	__learn_parm.skip_final_opt_check=0;
	__learn_parm.svm_maxqpsize=10;
	__learn_parm.svm_newvarsinqp=0;
	__learn_parm.svm_iter_to_shrink=100;
	__learn_parm.kernel_cache_size=40;
	__learn_parm.eps=0.1;
	__learn_parm.transduction_posratio=-1.0;
	__learn_parm.svm_costratio_unlab=1.0;
	__learn_parm.svm_unlabbound=1E-5;
	__learn_parm.epsilon_crit=0.001;
	__learn_parm.epsilon_a=1E-15;
	__learn_parm.compute_loo=0;
	__learn_parm.rho=1.0;
	__learn_parm.xa_depth=0;
	__kernel_parm.poly_degree=3;
	__kernel_parm.coef_lin=1;
	__kernel_parm.coef_const=1;
	strcpy(__kernel_parm.custom,"empty");
	//strcpy(type,"c");
	
	__learn_parm.type = CLASSIFICATION;
	__kernel_parm.kernel_type=RBF;
	__kernel_parm.rbf_gamma=1.0; /////////////////////////
	__learn_parm.svm_costratio=1.0; /////////////////////
	__learn_parm.svm_c=0.0; ///////////////////
	__learn_parm.maxiter=SVM_MAXITER; ////////////////
	strcpy(__learn_parm.predfile, "trans_predictions"); //
	
	verbosity = 0;
	
	return;
}


// /////////////////////////////////////////////////////////////////////////////
// Name: svm_classify
// Synop: make a call to svm_classify using modelFile as the svm model,
//        and predFile as the place to store the predictions, and datFile as
//        its input, filling results with the appropriate infomation 
//        from svm_classify and returning the exit status of svm_classify
//        if results == NULL then the output of svm_classify is ignored
int svm_classify
(SVM_Model *mdl, Measure *results, char *datFile/*, char *predFile*/) {
	int stat = 0;
	DOC *doc;   /* test example */
	WORD *words;
	long max_docs,max_words_doc,lld;
	long totdoc=0,queryid,slackid;
	long correct=0,incorrect=0,no_accuracy=0;
	long res_a=0,res_b=0,res_c=0,res_d=0,wnum,pred_format;
	long j;
	double t1,runtime=0;
	double dist,doc_label,costfactor;
	char *line,*comment; 
	FILE *predfl,*docfl;
	
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// !!! TODO !!!!
	//    1) turn our SVM_Record into an object based on the svm_light DOC type
	//    2) remove file I/O for classifcation and training
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	__kernel_parm.rbf_gamma = mdl->param_g;
	__learn_parm.svm_c = mdl->param_c;
	
	if( mdl->mdl_type == Classification ) {
		
		__learn_parm.svm_costratio = mdl->param_j;
		
	}
	
	if(__model == NULL) {
		
		return -1;
		
	}
	
	
	nol_ll(datFile,&max_docs,&max_words_doc,&lld); /* scan size of input file */
	max_words_doc+=2;
	lld+=2;
	
	line = (char *)malloc(sizeof(char)*lld);
	words = (WORD *)malloc(sizeof(WORD)*(max_words_doc+10));
	
	//__model=read_model(mdl->mdl_file);
	
	if(__model->kernel_parm.kernel_type == 0) { /* linear kernel */
		/* compute weight vector */
		add_weight_vector_to_linear_model(__model);
	}
	
	if(verbosity) {
		printf("Classifying test examples.."); fflush(stdout);
	}
	
	if ((docfl = fopen (datFile, "r")) == NULL) {
		perror (datFile); 
		exit (1); 
	}
	
	while((!feof(docfl)) && fgets(line,(int)lld,docfl)) {
		
		if(line[0] == '#') 
			continue;  /* line contains comments */
		
		parse_document(line,words,&doc_label,&queryid,&slackid,&costfactor,&wnum,
					   max_words_doc,&comment);
		totdoc++;
		
		if(__model->kernel_parm.kernel_type == 0) {   /* linear kernel */
			
			SVECTOR *vt;
			for(j=0;(words[j]).wnum != 0;j++) {  /* Check if feature numbers   */
				if((words[j]).wnum > __model->totwords) /* are not larger than in     */
					(words[j]).wnum=0;               /* model. Remove feature if   */
			}                                 /* necessary.                 */
			
			
			vt = create_svector(words,comment,1.0);
			doc = create_example(-1,0,0,0.0,vt);
			
			dist=classify_example_linear(__model,doc);
			
			free_example(doc,1);
			
		} else {                             /* non-linear kernel */
			SVECTOR *vt = create_svector(words,comment,1.0);
			doc = create_example(-1,0,0,0.0,vt);
			
			dist=classify_example(__model,doc);
			
			
			free_example(doc,1);
		}
		
		if(dist>0) {
			
			if(doc_label>0) 
				correct++; 
			else 
				incorrect++;
			
			if(doc_label>0)
				res_a++; 
			else 
				res_b++;
			
		} else {
			
			if(doc_label<0)
				correct++; 
			else 
				incorrect++;
			
			if(doc_label>0)
				res_c++; 
			else 
				res_d++;
			
		}
		
	}
	
	free(line);
	free(words);
//	free(comment);
	fclose(docfl);
	
	
	results->Accuracy = (correct)*100.0/totdoc;
	
	if( res_a+res_b == 0.0 )
		results->Precision = -1.0;
	else 
		results->Precision = (res_a)*100.0/(res_a+res_b);
	
	if( res_a + res_c == 0.0 ) 
		results->Recall= -1.0;
	else 
		results->Recall = (res_a)*100.0/(res_a+res_c);
	
	return stat;
	
}


/* Example output of svm_learn

00 Scanning examples...done
01 Reading examples into memory...100..OK. (164 examples read)
02 Setting default regularization parameter C=0.3452
03 Optimizing.............................................done. (92 iterations)
04 Optimization finished (41 misclassified, maxdiff=0.00088).
05 Runtime in cpu-seconds: 0.01
06 Number of SV: 126 (including 103 at upper bound)
07 L1 loss: loss=80.27758
08 Norm of weight vector: |w|=3.54359
09 Norm of longest example vector: |x|=3.46410
10 Estimated VCdim of classifier: VCdim<=88.89927
11 Computing XiAlpha-estimates...done
12 Runtime for XiAlpha-estimates in cpu-seconds: 0.00
13 XiAlpha-estimate of the error: error<=75.61% (rho=1.00,depth=0)
14 XiAlpha-estimate of the recall: recall=>29.07% (rho=1.00,depth=0)
15 XiAlpha-estimate of the precision: precision=>28.41% (rho=1.00,depth=0)
16 Number of kernel evaluations: 7023
17 Writing model file...done
*/

// /////////////////////////////////////////////////////////////////////////////
// Name: svm_learn
// Synop: calls svm_learn and setups a model using trainFile as the example
//        set and mdlFile as the place to put the model, it also will set up
//        any needed fields in SVM_Model
int svm_learn
(SVM_Model *mdl, char *trainFile) {
	int stat = 0;
	MODEL *tmpMdl;
//	FILE *nulldev= fopen( "/dev/null", "w" ), *tmp, *tmp2;
	DOC **docs;  /* training examples */
	long totwords,totdoc,i;
	double *target;
	double *alpha_in=NULL;
	double tim;
	struct timeval t0, t1;
	
	// mark off the initial time
	gettimeofday(&t0, NULL);
	
	if( __model != NULL )
		free_model(__model,1);
	
	__model = (MODEL *)malloc(sizeof(MODEL));
	
	__learn_parm.svm_c = mdl->param_c;
	__kernel_parm.rbf_gamma = mdl->param_g;
	__kernel_parm.kernel_type = RBF;
	
	// now deal with type specific options to svm
	if( mdl->mdl_type == Classification ) {
		__learn_parm.type = CLASSIFICATION;
		__learn_parm.svm_costratio = mdl->param_j;
		
	} else {
		
		__learn_parm.eps = mdl->param_w;
		
	}
	
	if( mdl->mdl_type == Classification ) {
		
		read_documents(trainFile,&docs,&target,&totwords,&totdoc);
		
		__kernel_cache=kernel_cache_init(totdoc,__learn_parm.kernel_cache_size);
		
    	svm_learn_classification(docs,target,totdoc,totwords,&__learn_parm,
			     &__kernel_parm,__kernel_cache,__model,alpha_in);
		//write_model(mdlFile,__model);
		
		mdl->xialpha_error = __model->xa_error;
		mdl->xialpha_precision = __model->xa_precision;
		mdl->xialpha_recall = __model->xa_recall;
		mdl->support_vectors = __model->sv_num;
		
		mdl->vc_dim = -1;
	} else {
		
		mdl->support_vectors = -1;
		mdl->xialpha_recall = -1;
		mdl->xialpha_precision = -1;
		mdl->xialpha_error = -1;
		mdl->vc_dim = -1;
		
	}
	
	//DEBUG(__model->sv_num,d);
	
	// mark off end time
	gettimeofday(&t1, NULL); 
	
//	mdl->mdl_file = mdlFile;
	mdl->learn_time = 0.0;
	
	kernel_cache_cleanup(__kernel_cache);

	tmpMdl = copy_model( __model );
	
	free_model(__model, 0);
	
	__model = tmpMdl;
	
	free(alpha_in);
	
	for(i=0;i<totdoc;i++)
		free_example(docs[i],1);
	
	free(docs);
	free(target);
	
	tim = (t1.tv_sec - t0.tv_sec)
		+ (double)(t1.tv_usec - t0.tv_usec) / 1000000.0;
	
	mdl->learn_time = tim;
	
	// return the exit status of svm_learn
	return stat;
	
}

// /////////////////////////////////////////////////////////////////////////////
// Name: FourFold_Setup
// Synop: must be called before preforming the four fold training. This 
//        function setups the FourFoldCTX evaulation enviorment and 
//        gets the system ready to do the fourfold training, it reads in
//        trainDat, divides it into positive and negative classes,
//        and then builds the four sets of training and crossvalidation 
//        pairs
double FourFold_Setup
(FourFoldCTX *ctx, SVM_Record trainDat[], int tlen, char *trainingFile) {
	SVM_Record pos[tlen], neg[tlen];
	int poslen, neglen;
	int len;
	char *p0;
	int n0, n1, i;

	// go ahead and make this null
	ctx->model = NULL;

	ctx->masterTrainingFile = trainingFile;
	
	{ // now break it up into two classes, one negative and one positive
		
		poslen = neglen = 0;
		
		for(i = 0; i < tlen; i++) {
			
			if( trainDat[i].svm_classifier < 0 ) {
				neg[ neglen++ ] = trainDat[i];
			}
			
			if( trainDat[i].svm_classifier > 0 ) {
				pos[ poslen++ ] = trainDat[i];
			}
			
		}
		
	}
	
	{ // now generate the four training and crossvalidation files
		FILE *ktrn, *kcrs;
		
		for( n0 = 0; n0 < 4; n0++ ) {
			snprintf( 
				ctx->trainingFiles[n0],
				sizeof(ctx->trainingFiles[n0]), 
				KN_TRAINFILE_FMT, trainingFile, n0, getpid() );
			
			ktrn = fopen( ctx->trainingFiles[n0], "w+" );
			
			if( ktrn == NULL ) {
				perror(ctx->trainingFiles[n0]);
				exit(-1);
			}
			
			snprintf( ctx->crossvalFiles[n0], 
				sizeof(ctx->crossvalFiles[n0]), 
				KN_CROSSFILE_FMT, trainingFile, n0, getpid() );
			
			kcrs = fopen( ctx->crossvalFiles[n0], "w+" ); 
			
			if( kcrs == NULL ) {
				perror(ctx->crossvalFiles[n0] );
				exit(-1);
			}
		
			len = poslen + neglen;
			// place the correct number of positive examples into the 
			// training and crossvalidation files
			for( n1 = 0; n1 < poslen - (poslen % 4); n1 += 4 ) {
				p0 = FeatureVectorToSVMStr(pos[ n1 + ((n0 + 0) % 4) ].svm_data); 
				fprintf(ktrn, "%f %s\n",
					pos[ n1 + ((n0 + 0) % 4) ].svm_classifier,
					p0
				);
				
				p0 = FeatureVectorToSVMStr(pos[ n1 + ((n0 + 1) % 4) ].svm_data); 
				fprintf(ktrn, "%f %s\n",
					pos[ n1 + ((n0 + 1) % 4) ].svm_classifier,
					p0
				);
				
				p0 = FeatureVectorToSVMStr(pos[ n1 + ((n0 + 2) % 4) ].svm_data); 
				fprintf(ktrn, "%f %s\n",
					pos[ n1 + ((n0 + 2) % 4) ].svm_classifier,
					p0
				);
				
				p0 = FeatureVectorToSVMStr(pos[ n1 + ((n0 + 3) % 4) ].svm_data); 
				fprintf(kcrs, "%f %s\n",
					pos[ n1 + ((n0 + 3) % 4) ].svm_classifier,
					p0
				);
				
			}
			
			// if there is anything left over throw it into the crossvalidation
			// file
			if( (poslen % 4) > 0 ) { 
				
				for( n1 = 0; n1 < (poslen % 4); n1++ ) {
					p0 = FeatureVectorToSVMStr(pos[ (poslen-(poslen%4))/4 + n1 ].svm_data);
					fprintf(ktrn, "%f %s\n",
						pos[ (poslen-(poslen%4))/4 + n1 ].svm_classifier,
						p0
					);
				}
				
			}
			
			// place the correct number of negative examples into the 
			// training and crossvalidation files
			for( n1 = 0; n1 < neglen - (neglen % 4); n1 += 4 ) {

				p0 = FeatureVectorToSVMStr(neg[ n1 + ((n0 + 0) % 4) ].svm_data); 
				fprintf(ktrn, "%f %s\n",
					neg[ n1 + ((n0 + 0) % 4) ].svm_classifier,
					p0
				);
				
				p0 = FeatureVectorToSVMStr(neg[ n1 + ((n0 + 1) % 4) ].svm_data); 
				fprintf(ktrn, "%f %s\n",
					neg[ n1 + ((n0 + 1) % 4) ].svm_classifier,
					p0
				);
				
				p0 = FeatureVectorToSVMStr(neg[ n1 + ((n0 + 2) % 4) ].svm_data); 
				fprintf(ktrn, "%f %s\n",
					neg[ n1 + ((n0 + 2) % 4) ].svm_classifier,
					p0
				);
				
				p0 = FeatureVectorToSVMStr(neg[ n1 + ((n0 + 3) % 4) ].svm_data); 
				fprintf(kcrs, "%f %s\n",
					neg[ n1 + ((n0 + 3) % 4) ].svm_classifier,
					p0
				);
			}
			
			// if there is anything left over throw it into the crossvalidation
			// file
			if( (neglen % 4) > 0 ) {
			
				for( n1 = 0; n1 < (neglen % 4); n1++ ) {
					p0 = FeatureVectorToSVMStr(neg[ (neglen-(neglen%4))/4 + n1 ].svm_data);
					fprintf(ktrn, "%f %s\n",
						neg[ (neglen-(neglen%4))/4 + n1 ].svm_classifier,
						p0
					);
					
				}
				
			}
			
			fclose(kcrs);
			fclose(ktrn);
			
		}
		
	}
	
	ctx->numneg = neglen;
	ctx->numpos = poslen;
	ctx->testingFile = NULL;
	
	snprintf(
		ctx->tmp_predictions,
		sizeof(ctx->tmp_predictions),
		"/tmp/svm_pred_%d-%d", 
		getpid(),
		neglen * poslen
	);
	
	
	snprintf(
		ctx->tmp_model,
		sizeof(ctx->tmp_model),
		"/tmp/svm_model_%d-%d", 
		getpid(),
		neglen * poslen
	);
	
	
	return (double)poslen / (double)neglen;
	
}


// /////////////////////////////////////////////////////////////////////////////
// Name: FourFold_CleanUp
// Synop: Remove and cleanup the context and enviorment
void FourFold_CleanUp
(FourFoldCTX *ctx) {
	register int i = 0;

	for(; i < 4; i++) {
		unlink( ctx->trainingFiles[i] );
//		free(ctx->trainingFiles[i]);
		
		unlink( ctx->crossvalFiles[i] );
//		free( ctx->crossvalFiles[i] );
	
	}
	
	unlink( ctx->tmp_predictions );
	unlink( ctx->tmp_model );
	
}


// /////////////////////////////////////////////////////////////////////////////
// Name: FourFold_EvalModel
// Synop: evaluate mdl, using fourfold training
int FourFold_EvalModel
(FourFoldCTX *ctx, SVM_Model *mdl) { 
	register int i = 0;
	int stat;
	double tot_time = 0.0;
	
	for(; i < 4; i++ ) {
		
		stat = svm_learn(mdl, ctx->trainingFiles[i]);
		
		if( stat != 0 ) {
			switch( stat ) {
				case 14: {
					fprintf(stderr, "ERROR: svm_learn (%d fold) timeout..\n", i);
					return -1;
				} break;
				
				default: {
					fprintf(stderr, "ERROR: svm_learn (%d fold) (%d stat)..\n", i, stat);
					return -1;
				} break;
				
			}
			
		}
		
		ctx->trainingResults[i].Accuracy  = 100.0 - mdl->xialpha_error;
		ctx->trainingResults[i].Precision = mdl->xialpha_precision;
		ctx->trainingResults[i].Recall    = mdl->xialpha_recall;
		
		tot_time += mdl->learn_time;
		
		stat = svm_classify(
			mdl,
			&(ctx->crossvalResults[i]), 
			ctx->crossvalFiles[i]//,
			//ctx->tmp_predictions
		);
		
		if( stat != 0 ) {
		
			switch( stat ) {
				case 14: {
					fprintf(stderr, "ERROR: svm_classify (%d fold) timeout..\n", i);
					return -1;
				} break;
				
				default: {
					fprintf(stderr, "ERROR: svm_classify (%d fold) (%d stat)..\n", i, stat);
					return -1;
				} break;
				
			}
		
		}
		
		/*
		if( ctx->testingFile != NULL ) {
			stat = svm_classify(
				mdl,
				&(ctx->testingResults[i]), 
				ctx->testingFile,
				ctx->tmp_predictions
			);
			
			if( stat != 0 ) {
			
				switch( stat ) {
					case 14: {
						fprintf(stderr, "ERROR: svm_classify (%d fold) timeout..\n", i);
							return -1;
					} break;
					
					default: {
						fprintf(stderr, "ERROR: svm_classify (%d fold) (%d stat)..\n", i, stat);
						return -1;
					} break;
				
				}
		
			}
			
		}
		*/
		
		
	}
	
	ctx->model = mdl;
	ctx->learn_time = tot_time;

	return 0;
}

// /////////////////////////////////////////////////////////////////////////////
// Name: FourFold_GetStatistics
// Synop: collect the statistics generated from the previous fourfold 
//        evaluation
int FourFold_GetStatistics
(FourFoldCTX *ctx, Measure *train, Measure *cross, Measure *test) {
	int stat = 0;
	if( ctx->model == NULL )
		return -1;

	if( train != NULL ) {	
		train->Accuracy = 0.25 * ( ctx->trainingResults[0].Accuracy
				+ ctx->trainingResults[1].Accuracy
				+ ctx->trainingResults[2].Accuracy
				+ ctx->trainingResults[3].Accuracy );
		
		train->Precision = 0.25 * ( ctx->trainingResults[0].Precision
				+ ctx->trainingResults[1].Precision
				+ ctx->trainingResults[2].Precision
				+ ctx->trainingResults[3].Precision );
		
		train->Recall = 0.25 * ( ctx->trainingResults[0].Recall
				+ ctx->trainingResults[1].Recall
				+ ctx->trainingResults[2].Recall
				+ ctx->trainingResults[3].Recall );
		
	}
				
	if( cross != NULL ) {
		cross->Accuracy = 0.25 * ( ctx->crossvalResults[0].Accuracy
				+ ctx->crossvalResults[1].Accuracy
				+ ctx->crossvalResults[2].Accuracy
				+ ctx->crossvalResults[3].Accuracy );
			
		cross->Precision = 0.25 * ( ctx->crossvalResults[0].Precision
				+ ctx->crossvalResults[1].Precision
				+ ctx->crossvalResults[2].Precision
				+ ctx->crossvalResults[3].Precision );
				
		cross->Recall = 0.25 * ( ctx->crossvalResults[0].Recall
				+ ctx->crossvalResults[1].Recall
				+ ctx->crossvalResults[2].Recall
				+ ctx->crossvalResults[3].Recall );
		
	}
	
	if( test != NULL && ctx->testingFile ) {
	/*
		test->Accuracy = 0.25 * ( ctx->testingResults[0].Accuracy
				+ ctx->testingResults[1].Accuracy
				+ ctx->testingResults[2].Accuracy
				+ ctx->testingResults[3].Accuracy );
			
		test->Precision = 0.25 * ( ctx->testingResults[0].Precision
				+ ctx->testingResults[1].Precision
				+ ctx->testingResults[2].Precision
				+ ctx->testingResults[3].Precision );
				
		test->Recall = 0.25 * ( ctx->testingResults[0].Recall
				+ ctx->testingResults[1].Recall
				+ ctx->testingResults[2].Recall
				+ ctx->testingResults[3].Recall );
				
	*/
	/*
		// /////////////////// Begin Modification //////////////////////////////
		int i =0, bestFold = 0;
		double fmsr, fmax;
		
		// do testing with model trained on best 3-folds
		
		fmsr = fmax = 0.0;
		
		// find the 3-fold model with the best crossval results
		for( ; i < 4; i++ ) {
			fmsr = FITNESS( ctx->crossvalResults[i] );
			if( fmsr > fmax ) {
				fmax = fmsr;
				bestFold = i;
			}
		}
		
		// train with that training profile
		stat = svm_learn( ctx->model, ctx->trainingFiles[i], ctx->tmp_model );
		if( stat != 0 ) {
			if( stat == 14 ) {
				fprintf( stderr, "ERROR: svm_learn timeout (testing)..\n" );
				return -1;
			} else {
				fprintf( stderr, "ERROR: svm_learn (testing %d) bailing..\n", stat);
				exit(-1);
			}
		}
		
		// eval the results
		stat = svm_classify( ctx->model, test, ctx->testingFile, ctx->tmp_predictions);
		if( stat != 0 ) {
			fprintf( stderr, "ERROR: svm_classify (testing) bailing..\n");
			return (-1);
		}

		// /////////////////// End Modification ////////////////////////////////
	*/
	
	
		// preform learn with full profile and evaluate the testing data
		
		

		stat = svm_learn( ctx->model, ctx->masterTrainingFile ); 
		if( stat != 0 ) {
			if( stat == 14 ) {
				fprintf( stderr, "ERROR: svm_learn timeout (testing)..\n" );
				return -1;
			} else {
				fprintf( stderr, "ERROR: svm_learn (testing %d) bailing..\n", stat);
				exit(-1);
			}
		}
		
		stat = svm_classify( ctx->model, test, ctx->testingFile/*, ctx->tmp_predictions*/);
		if( stat != 0 ) {
			fprintf( stderr, "ERROR: svm_classify (testing) bailing..\n");
			return (-1);
		}
	
		
	}
	
	return 0;

}



