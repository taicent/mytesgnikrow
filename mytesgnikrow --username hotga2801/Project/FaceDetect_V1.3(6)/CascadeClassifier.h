class CascadeClassifier
{
public:
	int m_iCount;
	AdaBoostClassifier* m_AdaClassifiers;

	CascadeClassifier();
	~CascadeClassifier();
	void Clear(void);
	CascadeClassifier& operator=(const CascadeClassifier& source);

	void WriteToFile(ofstream& f) const;
	void ReadFromFile(ifstream& f);
	void LoadDefaultCascade(void);

	virtual const int ApplyImagePatch(const IntImage& im) const;
	virtual void ApplyOriginalSize(IntImage& original,const CString filename) ;
	virtual void ApplyOriginalSizeForInputBoosting(const CString filename,int& pointer) const;

	void DrawResults(IntImage& image, const vector<CRect>& results) const;
	void CheckResults(const vector<CRect>& results);

	virtual const bool OneRound(const int round);
};

void PostProcess(vector<CRect>& result, const int combine_min);