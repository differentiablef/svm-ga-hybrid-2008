
#ifndef _CHROM_TOOLS_H_
#define _CHROM_TOOLS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef CHROM_SIZE
#define CHROM_SIZE		231   // Length of a Chromosome
#endif /*_CHROM_SIZE_*/

#ifdef BITS_IN_INT
#undef BITS_IN_INT
#endif /*BITS_IN_INT*/

#ifdef CHROM_LEN
#undef CHROM_LEN
#endif /*CHROM_LEN*/

#define BITS_IN_INT	( sizeof(unsigned int) * 8 )
#define CHROM_LEN	(CHROM_SIZE/BITS_IN_INT + 1)

#define BIT(x)		(0x1<<((x)-1))


typedef unsigned int FeatureVector[ CHROM_SIZE/BITS_IN_INT + 1 ];


////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// Begin FeatureVector & Tools //////

// /////////////////////////////////////////////////////////////////////////////
// Name: FeatureVectorCopy
// Synop: copy cz1 in to cz2
void FeatureVectorCopy(FeatureVector cz1, FeatureVector cz2);

// /////////////////////////////////////////////////////////////////////////////
// Name: FeatureVectorCompare
// Synop: Compare the genes in two FeatureVectors return the number of genes 
//        common to both
int FeatureVectorCompare(FeatureVector cz1, FeatureVector cz2);

// /////////////////////////////////////////////////////////////////////////////
// Name: FeatureVectorToStr
// Synop: Take chro, build a string that represents it, returning a pointer 
//        to the newly created string
// Warning: !!THIS IS NOT THREAD SAFE!!!
char *FeatureVectorToStr(FeatureVector chro);

// /////////////////////////////////////////////////////////////////////////////
// Name: FeatureVectorToSVMStr
// Synop: Take chro, build a string that represents it, returning a pointer 
//        to the newly created string string is formated like 
//        an SVM record
// Warning: !!THIS IS NOT THREAD SAFE!!!
char *FeatureVectorToSVMStr(FeatureVector chro);

// /////////////////////////////////////////////////////////////////////////////
// Name: FeatureVectorGetGene
// Synop: check if the FeatureVector has the specified gene set and return 
//        1 if it does 0 if not
int FeatureVectorGetGene(FeatureVector chrom, int gene);


// /////////////////////////////////////////////////////////////////////////////
// Name: FeatureVectorSetGene
// Synop: set the gene in the FeatureVector chrom, to state (1 or 0) returning 
//        non zero if the gene cannot be set, and 0 otherwise
int FeatureVectorSetGene(FeatureVector chrom, int gene, int state);

// /////////////////////////////////////////////////////////////////////////////
// Name: FeatureVectorClearAll
// Synop: sets all the genes in the FeatureVector to off state
void FeatureVectorClearAll(FeatureVector chrom);

// /////////////////////////////////////////////////////////////////////////////
// Name: FeatureVectorIsEmpty
// Synop: returns wether or not chrom has any genes set
int FeatureVectorIsEmpty(FeatureVector chrom);

// /////////////////////////////////////////////////////////////////////////////
// Name: FeatureVectorIntersect
// Synop: preform logical 'and' on the genes returning the result, which is
//        the interection of the two FeatureVectors placing the result in 
//        output if it is non-null, returning 1 if the interection is 
//        non-empty and 0 otherwise
int FeatureVectorIntersect(FeatureVector cz1, FeatureVector cz2, FeatureVector output);

// /////////////////////////////////////////////////////////////////////////////
// Name: FeatureVectorLength
// Synop: calculate and return the number of active genes in the FeatureVector
int FeatureVectorLength(FeatureVector chrom);

// /////////////////////////////////////////////////////////////////////////////
// Name: FeatureVectorMakeRandom
// Synop: make a randome FeatureVector with len genes, storing the result in
//        out
void FeatureVectorMakeRandom(FeatureVector out, int len);

// /////////////////////////////////////////////////////////////////////////////
// Name: FeatureVectorFromSVMStr
// Synop: Take in svm record and generate a FeatureVector for that record 
//        storing the result in out and the length of stay in lenOfStay
void FeatureVectorFromSVMStr(char buf[], int buflen, double *lenOfStay, FeatureVector out);

//////////////////////////////////////////////// End Chromosome & Tools ////////
////////////////////////////////////////////////////////////////////////////////


#endif /*_CHROM_TOOLS_H_*/
