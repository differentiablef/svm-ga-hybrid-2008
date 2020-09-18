#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <stdlib.h>

#include "chrom_tools.h"
#include "svm_record_tools.h"

// ////////// Begin Definitions ////////////////////////////////////////////////

// Marker prints "obj" 
#define MARKER(obj) { \
	fprintf(stderr, " "#obj"\n"); \
}

// Debug, prints "obj = " value of obj, of type 
#define DEBUG(obj,type) {\
	fprintf(stderr, " "#obj" = %"#type"\n", (obj));\
}


#define LIFEFORM_NOEVAL		0 // keep old fitness value
#define LIFEFORM_EVAL		1 // eval with current paramset--with cache;
#define LIFEFORM_REEVAL		2 // complete reeval with paramset--no cache;
 
#define X_OFFSET	1.0 // Zoom in X offset
#define Y_OFFSET	1.0 // Zoom in Y offset

#define EVALPAR_REFINE	5	  // number of points to refine around top point
#define EVALPAR_TOP		6	  // number of top points to pick
#define EVALPAR_IVAL	10	  // number of generations to go before global reeval
#define PROB_MATE		0.6	  // Probability of mating
#define PROB_MUTATION   _prob_mutate  // Probability of mutation

#define PARAM_SZ		500   // Maxium number of (C,g) parameters we handle
#define LFEVAL_THRSD	10
#define CHROM_SIZE		231   // Length of a Chromosome
#define MAX_LIFE		1000  // Maxium number of life forms (not used)
#define INIT_LIFE		200   // Initial number of life forms in population
#define GENERATION		250   // Number of generations before model terminates


// the fitness function (acts on Measure structure defined in svm_engine.h)
#define FITNESS(rec)		(0.5*(rec.Accuracy + \
				2.0*rec.Precision * rec.Recall / (rec.Precision + rec.Recall)))


// ///////// End Globals ///////////////////////////////////////////////////////

#endif /* _COMMON_H */

