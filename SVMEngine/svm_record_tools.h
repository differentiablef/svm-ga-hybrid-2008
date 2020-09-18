
#ifndef _SVM_RECORD_TOOLS_H_
#define _SVM_RECORD_TOOLS_H_

#include <stdio.h>

#include "chrom_tools.h"

// /////////////////////////////////////////////////////////////////////////////
// Type Definition: SVM_Record
// Synop: contains parsed feature string and length of stay
typedef struct __svm_rec {
	char *svm_str;
	FeatureVector svm_data;
	union _target {
		double _lenOfStay;
		double _class;
	} tgt;
	
#define svm_lenOfStay	tgt._lenOfStay
#define svm_classifier	tgt._class
} SVM_Record;


// /////////////////////////////////////////////////////////////////////////////
// Name: applyMaskToTrainingData
// Synop: uses the FeatureVector mask, as a binary mask for all of the svm records
//        in dat, placing the result in out returning the number of records
//        in out
extern int applyMaskToTrainingData(FeatureVector mask, SVM_Record out[], SVM_Record dat[], int datlen);

// ////////////////////////////////////////////////////////////////////////////
// Name: renumberTrainingData
// Synop: using mask as the basis for renumbering all the active genes in 
//        the training data dat, we renumber and store the result in out
//        returning the number of records
extern int renumberTrainingData(FeatureVector mask, SVM_Record out[], SVM_Record dat[], int datlen);

// /////////////////////////////////////////////////////////////////////////////
// Name: parseTrainingData
// Synop: read in and parse the training data in fileNm building the list of 
//        records and placing them in out returning the length
extern int parseTrainingData(const char *fileNm, SVM_Record out[]);

// /////////////////////////////////////////////////////////////////////////////
// Name: scanTrainingData
// Synop: scan through trainingData looking for records that match chrom
//        filling outArray with the records, and returning the number matching
extern int scanTrainingData(FeatureVector chrom, SVM_Record outArray[], SVM_Record data[], unsigned int datlen);

// /////////////////////////////////////////////////////////////////////////////
// Name: scanIntersectTrainingData
// Synop: scan data in and place the renumbered intersection into the out
//        returning the number of matches
extern int scanIntersectTrainingData(FeatureVector chrom, SVM_Record out[], SVM_Record data[], unsigned int datlen);

// /////////////////////////////////////////////////////////////////////////////
// Name: dumpTrainingData
// Synop: creates and writes out the contents of recs to the file outFile
//        returning a pointer to the file name
extern char *dumpTrainingData(SVM_Record base[], int baselen, char *outFile);

#endif /*_SVM_RECORD_TOOLS_H_*/
