	enum TRAIN_METHODS {TRAIN_ADA = 1, TRAIN_FFS= 2};
	enum LINEAR_CLASSIFIER {LC_ORIGINAL = 1, LC_LAC = 2, LC_FDA = 3};

struct CascadeClassifier;

extern CString 	gSetup_Filename;

extern int gTotal_fp;

extern CString gTrainset_Filename;
extern CString gValidationset_Filename;
extern CString gClassifier_Filename;
extern CString gAda_Log_Filename;
extern CString gCascade_Filename;
extern CString FFS_WeakClassifiers_filename;
extern CString FFS_log_filename;
extern CString FileUsage_log_filename;
extern CString gBootstrap_Database_Filename;
extern CString gBackup_Directory_Name;
extern CString gTestSet_Filename;
extern int gSx;
extern int gSy;
extern int gTrain_Method;
extern int linear_classifier;
extern int bootstrap_level;
extern int max_bootstrap_level;
extern vector<REAL> bootstrap_resizeratio;
extern vector<int> bootstrap_increment;
extern int gTotalFeatures;
extern int gMaxNumFiles;
extern int gGoal_Method;
extern REAL gNode_Det_Goal;
extern REAL gNode_fp_Goal;
extern int gFirst_Feature;
extern REAL asym_ratio;
extern int gMaxNumNodes;
extern vector<int> gNof;
extern int gStartingNode;
extern int gFaceCount, gValidFaceCount;

extern IntImage* gTrainSet;
extern IntImage* gValidationSet;
extern int gTotalCount;
extern int gValidationCount;	

extern CascadeClassifier* gCascade;
extern REAL* gWeights;
extern int** gTable;
extern SimpleClassifier* gClassifiers;

extern REAL* gFeatures;
extern int* gLabels;

extern int* gFileUsed;
extern int gBootstrapSize;
extern CString* gBootstrap_Filenames;

extern REAL gMean_Min,gMean_Max,gSq_Min,gSq_Max,gVar_Min,gVar_Max;

void ReadOneTrainingSample(ifstream& is,IntImage& image);
void GetFeatureValues0(REAL* const features,const int from,const int to,const int x1,const int x2,const int x3,const int y1,const int y3);
void GetFeatureValues1(REAL* const features,const int from,const int to,const int x1,const int x3,const int y1,const int y2,const int y3);
void GetFeatureValues2(REAL* const features,const int from,const int to,const int x1,const int x2,const int x3,const int x4,const int y1,const int y3);
void GetFeatureValues3(REAL* const features,const int from,const int to,const int x1,const int x3,const int y1,const int y2,const int y3,const int y4);
void GetFeatureValues4(REAL* const features,const int from,const int to,const int x1,const int x2,const int x3,const int y1,const int y2,const int y3);
void QuickSort(REAL* values,int* labels,const int lo,const int hi);
void QuickSort(REAL* values,int* labels,REAL* weights,int* parity,const int lo,const int hi);
void FillTheTable(int* const row,const SimpleClassifier& sc);
void ClearUpGlobalData(void);
const int CountTrainFaces(void);
const int CountValidFaces(void);
void ReadTrainSet(CString filename);
void ReadValidSet(CString filename);
void SaveTrainSet(CString filename);
void SaveValidSet(CString filename);
void InitializeAuxiliaryVariables();
void InitializeWeights(void);
void ReadClassifiers(void);
void InitGlobalData(void);
void NormalizeWeight(void);
void SkewWeight(const REAL skew_ratio);
void ValidateTheThreshold(AdaBoostClassifier& ada,ofstream& f);

void MatrixInversion(REAL** a,int n);
void WithinClassScatter(AdaBoostClassifier& ada);

void ReadRangeFile(void);
void WriteRangeFile(void);
void LoadOptions();