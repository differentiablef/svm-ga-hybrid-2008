
#include "common.h"
#include "svm_record_tools.h"


// /////////////////////////////////////////////////////////////////////////////
// Name: parseTrainingData
// Synop: read in and parse the training data in fileNm building the list of 
//        records and placing them in out returning the length
int parseTrainingData
(const char *fileNm, SVM_Record out[]) {
	unsigned int flen = 0;
	
	// May cause big memory leak
	char *fileBuffer = NULL;
	int fileLen = -1;
	
	int recNum = 0;
	FILE *svmF;
	
	svmF = fopen( fileNm, "r" );
	
	if( svmF == NULL ) {
		//TODO: Bitch
		perror(fileNm);
		exit(-1);
	}
	
	// get the length of the file
	fseek( svmF, 0, SEEK_END );
	flen = ftell( svmF );
	fseek( svmF, 0, SEEK_SET );
	
	
	// allocate enough memory to read in the whole file
	fileBuffer = (char *)malloc( flen + 1 );
	fileLen = flen + 1;
	
	if(fileBuffer == NULL) {
		// TODO: bitch
		perror(fileNm);
		exit(-1);
	}
	
	// clear the buffer out, readying it for the read
	(void)memset(fileBuffer, 0x00, fileLen );
	
	fread(fileBuffer, 1, flen, svmF);
	
	// close the file
	fclose(svmF);
	
	{ // Actually do the Parsing of the data we just read in 
		char *p0 = fileBuffer, *p1 = NULL, *p2 = NULL;
		
		p1 = index( p0, '\n' );
		
		while( p1 != NULL ) {
			
			if( *p1 == '\n' )
				*p1 = '\0';
			p1++;
			p2 = p1;
			
			
			out[ recNum ].svm_str = NULL;
			
			FeatureVectorFromSVMStr(
				p0, strlen(p0), 
				&out[recNum].svm_lenOfStay, 
				out[recNum].svm_data
			);
			
			recNum++;
			
			p0 = p1;
			p1 = index(p0, '\n');
			
			
		}
		
	}
	
	free(fileBuffer);
	//DEBUG( recNum, i );
	
	return recNum;
	
	
}

// /////////////////////////////////////////////////////////////////////////////
// Name: scanTrainingData
// Synop: scan through trainingData looking for records that match chrom
//        filling outArray with the records, and returning the number matching
int scanTrainingData
(FeatureVector chrom, SVM_Record outArray[], SVM_Record data[], unsigned int datlen) {
	int i = 0, j = 0;
	
	for( ; i < datlen; i++ ) {
		if( FeatureVectorIntersect( chrom, 
								 data[i].svm_data, 
								 outArray[j].svm_data ) ) 
		{
					
			FeatureVectorCopy(data[i].svm_data,outArray[j].svm_data);
			outArray[j].svm_str = data[i].svm_str;
			outArray[j].svm_lenOfStay = data[i].svm_lenOfStay;
			
			j++;
			
		}
	}
	
	return j;
	
}


// /////////////////////////////////////////////////////////////////////////////
// Name: scanIntersectTrainingData
// Synop: scan data in and place the renumbered intersection into the out
//        returning the number of matches
int scanIntersectTrainingData
(FeatureVector chrom, SVM_Record out[], SVM_Record data[], unsigned int datlen) {
	int i = 0, j, len = 0, k = 0;
	FeatureVector tmp;
	int chromTrans[ CHROM_SIZE ]; // renumbered FeatureVector
	
	for(; i < CHROM_SIZE; i++ ) {
		
		if( FeatureVectorGetGene( chrom, i+1 ) ) {
			
			chromTrans[len++] = i+1;
			
		}
		
	}
	
	for( i = 0; i < datlen; i++ ) {
		
		FeatureVectorClearAll( out[k].svm_data );
		
		if( FeatureVectorIntersect( chrom, data[i].svm_data, tmp ) != 0 ) {
			
			
			// reform the FeatureVector
			for( j = 0; j < len; j++ ) {
				if( FeatureVectorGetGene( tmp, chromTrans[j] ) ) {
					
					FeatureVectorSetGene( out[k].svm_data, j+1, 1 );
					
				}
				
			}
			
			out[k].svm_lenOfStay = data[i].svm_lenOfStay;
			out[k].svm_str = data[i].svm_str;
			
			//DEBUG( FeatureVectorToSVMStr( out[k].svm_data ), s );
			
			k++;
			
		}
		
	}
	
	return k;
	
}

// /////////////////////////////////////////////////////////////////////////////
// Name: applyMaskToTrainingData
// Synop: uses the FeatureVector mask, as a binary mask for all of the svm records
//        in dat, placing the result in out returning the number of records
//        in out
int applyMaskToTrainingData
(FeatureVector mask, SVM_Record out[], SVM_Record dat[], int datlen) {
	
	register int i = 0;
	
	for( ; i < datlen; i++ ) {
		
		FeatureVectorIntersect(mask, dat[i].svm_data, out[i].svm_data);
		out[i].svm_lenOfStay = dat[i].svm_lenOfStay;
		
	}
	
	return i;
	
}

// ////////////////////////////////////////////////////////////////////////////
// Name: renumberTrainingData
// Synop: using mask as the basis for renumbering all the active genes in 
//        the training data dat, we renumber and store the result in out
//        returning the number of records
int renumberTrainingData
(FeatureVector mask, SVM_Record out[], SVM_Record dat[], int datlen) {
	
	int i = 0, mapLen = 0, k;
	int map[ CHROM_SIZE ];
	FeatureVector c0;
	
	// first generate the mapping p:Old#->New# and store it in map
	for( ; i < CHROM_SIZE; i++ ) {
		if( FeatureVectorGetGene(mask, i+1) ) {
			
			map[mapLen++] = i;
			
		}
		
	}
	
	// Now use the mapping to renumber the intersection of mask with each
	// record in dat, placing the result into out
	for( i = 0; i < datlen; i++ ) {
		
		FeatureVectorClearAll( out[i].svm_data );
		
		// if the intersection of the record and the mask is non-trival
		// then renumber the intersection
		if(FeatureVectorIntersect(mask, dat[i].svm_data, c0) != 0) {
		
			for( k = 0; k < mapLen; k++ ) {
				// if map[k] + 1 is set the set k + 1,
				// this induces a mapping g: g(map[k]) = k 
				if(FeatureVectorGetGene( c0, map[k] + 1 )) {
					FeatureVectorSetGene( out[i].svm_data, k+1, 1 );
				}
				
			}
			
		}
		
		out[i].svm_lenOfStay = dat[i].svm_lenOfStay;
		out[i].svm_str       = dat[i].svm_str;
		
		
	}
	
	return datlen;
	
}

// /////////////////////////////////////////////////////////////////////////////
// Name: dumpTrainingData
// Synop: creates and writes out the contents of recs to the file fileNm 
//        returning a pointer to the file name
// Warning: !!THIS IS NOT THREAD SAFE!!
char *dumpTrainingData
(SVM_Record base[], int baselen, char *outFile) {
	int i =0;
	FILE *fout;
	
	fout = fopen( outFile, "w+" );
	
	if( fout == NULL ) {
		perror( outFile );
		exit(-1);
	}
	
	for(; i < baselen; i++) {
		
		fprintf(fout, 
			"%.3f%s\n", 
			base[i].svm_lenOfStay, 
			FeatureVectorToSVMStr( base[i].svm_data ) 
		);
	
	}
	
	fflush(fout);
	fclose(fout);
	
	return outFile;
	
}

