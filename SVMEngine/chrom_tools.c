
#include "chrom_tools.h"

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// Begin FeatureVector & Tools //////


// /////////////////////////////////////////////////////////////////////////////
// Name: FeatureVectorCopy
// Synop: copy cz1 in to cz2
void FeatureVectorCopy
(FeatureVector cz1, FeatureVector cz2) {
	register int i= 0;
	
	for(; i < CHROM_LEN; i++)
		cz2[i] = cz1[i];
	
}

// /////////////////////////////////////////////////////////////////////////////
// Name: FeatureVectorCompare
// Synop: Compare the genes in two FeatureVectors return the number of genes 
//        common to both
int FeatureVectorCompare
(FeatureVector cz1, FeatureVector cz2) {
	register unsigned int i = 0, r1, r2;
	int sim = 0;
	
	
	// compare the bits from the two FeatureVectors and return the 
	// number of similarities
	for(; i < CHROM_LEN; i++) {
		
		r1 = cz1[i] & cz2[i];
		
		if( r1 ) {
			r2 = 1;
			
			while( r2 ) {
				if(r1 & r2)
					sim++;
				
				r2 *= 2;
			}
			
		}
		
	}
	
	return sim;

}

// /////////////////////////////////////////////////////////////////////////////
// Name: FeatureVectorToStr
// Synop: Take chro, build a string that represents it, returning a pointer 
//        to the newly created string
// Warning: !!THIS IS NOT THREAD SAFE!!!
char *FeatureVectorToStr
(FeatureVector chro) {
	
	static char _buf[ CHROM_SIZE * 16 + 1 ], _tmp[ 32 ];
	int i = 0, j, k, n0;
	
	_buf[0] = 0x00;
	
	for(; i < CHROM_LEN; i++) {
		
		if(chro[i] != 0) {
			
			j = 1;
			k = 0;
			
			while(j) {
			
				if( j & chro[i] ) {
					n0 = i * BITS_IN_INT + k + 1;
					
					(void)snprintf(
						_tmp,
						sizeof(_tmp),
						" %i", 
						n0
					);
				
					(void)strncat(
						_buf, 
						_tmp, 
						sizeof(_buf)
					);
				}
			
				j*=2;
				k++;
			}
			
		}
		
	}
	
	return _buf;
	
}

// /////////////////////////////////////////////////////////////////////////////
// Name: FeatureVectorToSVMStr
// Synop: Take chro, build a string that represents it, returning a pointer 
//        to the newly created string string is formated like 
//        an SVM record
// Warning: !!THIS IS NOT THREAD SAFE!!!
char *FeatureVectorToSVMStr
(FeatureVector chro) {
	
	static char _buf[ CHROM_SIZE * 16 + 1 ], _tmp[ 32 ];
	
	int i = 0, j, k, n0;
	
	_buf[0] = 0x00;
	
	for(; i < CHROM_LEN; i++) {
		
		if( chro[i] != 0 ) {
			j = 1;
			k = 0;
			while(j) {
				
				if( j & chro[i] ) {
					n0 = i * BITS_IN_INT + k + 1;
					
					(void)snprintf(
						_tmp,
						sizeof(_tmp),
						" %i:1.0", 
						n0
					);
					
					(void)strncat(
						_buf, 
						_tmp, 
						sizeof(_buf)
					);
				}
				
				j*=2;
				k++;
				
			}
			
		}
		
	}
	
	return _buf;
	
}

// /////////////////////////////////////////////////////////////////////////////
// Name: FeatureVectorGetGene
// Synop: check if the FeatureVector has the specified gene set and return 
//        1 if it does 0 if not
inline int FeatureVectorGetGene
(FeatureVector chrom, int gene) {
 	return ( ( chrom[ gene/ BITS_IN_INT ] & BIT( gene % BITS_IN_INT ) ) != 0 );
}


// /////////////////////////////////////////////////////////////////////////////
// Name: FeatureVectorSetGene
// Synop: set the gene in the FeatureVector chrom, to state (1 or 0) returning 
//        non zero if the gene cannot be set, and 0 otherwise
int FeatureVectorSetGene
(FeatureVector chrom, int gene, int state) {
	
	unsigned int r0 = (gene - 1) / BITS_IN_INT, r1 = (gene - 1) % BITS_IN_INT;
	
	if(gene > CHROM_SIZE || 1 > gene)
		return 0;
	
	state &= 1; // make state a 1 or 0
	
	if( state ) 
		chrom[r0] |= ( BIT( r1 + 1 ) );
	else
		chrom[r0] &= ~(BIT( r1 + 1 ));
	return (1);
	
}

// /////////////////////////////////////////////////////////////////////////////
// Name: FeatureVectorClearAll
// Synop: sets all the genes in the FeatureVector to off state
void FeatureVectorClearAll
(FeatureVector chrom) {
	register int i = 0;
	
	for(; i < CHROM_LEN; i++)
		chrom[i] = 0x0;
	
}

// /////////////////////////////////////////////////////////////////////////////
// Name: FeatureVectorIsEmpty
// Synop: returns wether or not chrom has any genes set
int FeatureVectorIsEmpty
(FeatureVector chrom) {
	int r0 = 0;
	register int i = 0;
	
	for(; i < CHROM_LEN; i++) {
		if( chrom[i] != 0 ) {
			r0 = 1;
			break;
		}
	}
	
	return r0;
	
}

// /////////////////////////////////////////////////////////////////////////////
// Name: FeatureVectorIntersect
// Synop: preform logical 'and' on the genes returning the result, which is
//        the interection of the two FeatureVectors placing the result in 
//        output if it is non-null, returning 1 if the interection is 
//        non-empty and 0 otherwise
int FeatureVectorIntersect
(FeatureVector cz1, FeatureVector cz2, FeatureVector output) {
	register unsigned int i = 0, tmp=0;
	
	if( output == NULL ) {
		
		for(; i < CHROM_LEN && tmp != 0; i++)
			tmp |= cz1[i] & cz2[i];;
	
		return (tmp != 0);
		
	}
	
	for(; i < CHROM_LEN; i++) {
		output[i] = cz1[i] & cz2[i];
		tmp |= output[i]; 
	}
	
	return (tmp != 0);
}

// /////////////////////////////////////////////////////////////////////////////
// Name: FeatureVectorLength
// Synop: calculate and return the number of active genes in the FeatureVector
int FeatureVectorLength
(FeatureVector chrom) {
	register int i = 0, j;
	int tmp = 0;
	
	for(; i < CHROM_LEN; i++) {
		
		if(chrom[i]) {
			j = 1;
			
			while( j ) {

				if( j & chrom[i] )
					tmp++;
			
				j *= 2;
			}
		}
	}
	
	return tmp;
}

// /////////////////////////////////////////////////////////////////////////////
// Name: FeatureVectorMakeRandom
// Synop: make a randome FeatureVector with len genes, storing the result in
//        out
void FeatureVectorMakeRandom
(FeatureVector out, int len) {
	int randGene;
	
	// clear the output
	FeatureVectorClearAll( out );
	
	while(len) {
		// randomly pick a gene to turn on
		randGene = rand() % CHROM_SIZE;
		
		// quickly turn the gene on if its not already on
		if( !FeatureVectorGetGene( out, randGene ) ) {
			out[ randGene / BITS_IN_INT ] |= BIT( (randGene % BITS_IN_INT) + 1 );	
			len--;
		}
	
	}
	
}

// /////////////////////////////////////////////////////////////////////////////
// Name: FeatureVectorFromSVMStr
// Synop: Take in svm record and generate a FeatureVector for that record 
//        storing the result in out and the length of stay in lenOfStay
void FeatureVectorFromSVMStr
(char buf[], int buflen, double *lenOfStay, FeatureVector out) {
	int gene;
	char *pb = buf;
	
	// find where the record starts
	while(*pb != ' ' && buflen) {
		pb++;
		buflen--;
	}
	
	buflen--;
	pb++;
	
	// store the length of stay in lenOfStay
	if( lenOfStay != NULL )
		*lenOfStay = atof( buf );
	
	// clear the output FeatureVector
	FeatureVectorClearAll( out );
	
	// parse the record entries
	while( *pb != '\n' && *pb != 0x00 && buflen) {

		if( *pb == ' ' ) {
			pb++;
			buflen--;
		}
		
		// read in the gene
		gene = atoi( pb );
		
		// set the gene to the on state in out
		FeatureVectorSetGene( out, gene, 1 );
		
		// move to next record
		while(*pb != ' ' && *pb != 0x00 && *pb != '\n' && buflen) {
			pb++;
			buflen--;
		}
		
	}
	
}

//////////////////////////////////////////////// End Chromosome & Tools ////////
////////////////////////////////////////////////////////////////////////////////

