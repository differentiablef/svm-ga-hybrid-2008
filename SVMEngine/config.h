
#ifndef _CONFIG_H_
#define _CONFIG_H_

#define LEVEL1_CLUSTERDIR_TCT		"./Level1Clusters_TrainCrossTest"

#define LEVEL2_CLUSTERDIR_TCT		"./Level2Clusters_TrainCrossTest"
#define LEVEL2_CLUSTERDIR_TT		"./Level2Clusters_TrainTest"
#define LEVEL2_CLUSTERDIR_T			"./Level2Clusters_Train"

#define LEVEL2_FITNESSDIR	"./Level2Clusters_Fitness"

#define CURRENT_DIR		LEVEL2_CLUSTERDIR_T

#define TRAINFILE_FMT	CURRENT_DIR"/SVM_Traincluster%d_Version%d"
#define CROSSFILE_FMT	CURRENT_DIR"/SVM_crossvalidcluster%d_Version%d"
#define TESTFILE_FMT	CURRENT_DIR"/SVM_testcluster%d_Version%d"

#define GA_TRAINFILE_FMT	CURRENT_DIR"/GA_Traincluster%d_Version%d"
#define GA_CROSSFILE_FMT	CURRENT_DIR"/GA_crossvalidcluster%d_Version%d"
#define GA_TESTFILE_FMT		CURRENT_DIR"/GA_testcluster%d_Version%d"

#define GA_TOPGRIDDATA_FMT		"./Results/Grids/grid-%02d"
#define GA_TOP_PARAMLEN			10

#define GA_UDGRIDDATA_1_FMT		"./Results/Grids/grid_ud1"
#define GA_UDGRIDDATA_2_FMT		"./Results/Grids/grid_ud2"
#define GA_UD_PARAMLEN			30

#define GA_ONEPOINT_FMT		"./Results/Grids/grid-1pt-%02d"
#define GA_ONE_PARAMLEN		1

// These are the formats used in the genetic algorithm
#define GA_GRID_FMT			GA_ONEPOINT_FMT
#define GA_PARAMLEN			GA_ONE_PARAMLEN

#define FITNESS_TRAIN_FMT	LEVEL2_FITNESSDIR"/SVM_TrainingProfile%d_Type%d"
#define FITNESS_TEST_FMT	LEVEL2_FITNESSDIR"/SVM_TestingProfile%d_Type%d"

#endif /*_CONFIG_H_*/


