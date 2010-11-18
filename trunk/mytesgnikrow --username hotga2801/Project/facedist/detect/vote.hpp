#ifndef VoteIncluded
#define VoteIncluded

class AccumElement : public ListNode<AccumElement>
{
  public:
  int loc[5];
  int value;
  AccumElement(int x, int y, int s, int d, int t, int v) {
    loc[0]=x; loc[1]=y; loc[2]=s; loc[3]=d; loc[4]=t;
    value=v;
    next=NULL;
  }
};

void NewFuzzyVote(List<Detection> *outputList, 
                  List<Detection> *inputList, int numInputLists,
                  int width, int height, int maskWidth, int maskHeight,
                  int spread, int threshold, int overlap);

void NewFuzzyVote2(List<Detection> *outputList, 
                   List<Detection> *inputList, int numInputLists,
                   int width, int height, int maskWidth, int maskHeight,
                   int spread, int threshold, int overlap);

void DetectionListCornerToCenter(List<Detection> *detections);

#endif


