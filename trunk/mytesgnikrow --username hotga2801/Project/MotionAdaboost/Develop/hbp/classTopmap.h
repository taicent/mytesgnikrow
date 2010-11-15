/*
Rodney robot - Topological map
Copyright (C) 2002  Bob Mottram

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

//---------------------------------------------------------------------------

#ifndef classTopmapH
#define classTopmapH
//---------------------------------------------------------------------------

/// <summary>
/// This class implements a classical topological map algorithm as described by Tuev Kohonen
/// </summary>
class Ttopmap
{
private:
  float Dist(int x, int y);
  float similarity(int x, int y);
  float randVal(float value);
  float Rnd();
  void rescaleHits();
  float average_similarity;

protected:

public:
  int map_width, map_height;
  float ****unit;
  unsigned char ****image;
  unsigned char **outputs;
  bool **availability;
  int **hits;
  int max_hits;
  float minValue;
  float h;
  //a number used to classify a point on the map
  unsigned char **classification;
  int ***classificationMulti;
  long ClassificationColour[255][3];
  char classificationName[20][20];
  char portrait[20][150];  //a jpg filename associated with the class
  float randomness;   //  0<=r<=1
  int RadiusExcite;
  unsigned char **inputs;
  float Threshold;


  int inputs_width,inputs_height;
  int WinnerX, WinnerY;
  float learningRate; //  0<=r<=1

  void setClassificationColour(int classificationID, int Red, int Green, int Blue);
  void getClassificationColour(int classificationID, int &Red, int &Green, int &Blue);
  void setInput(int x, int y, unsigned char value);
  float getOutput(int x, int y);
  void update();
  void adjustWeights(int x, int y, float value);
  void learn();
  void initWeights(float minVal, float maxVal);
  void randomInputs();
  void setClassification(int classificationID);
  int getNearestClassification(int x, int y);
  int getClassification(int x, int y);
  void classify(int classificationID);
  void setClassificationMulti(int classificationIndex, int value);
  int getClassificationMulti(int x, int y, int classificationIndex);
  void save(char *filename);
  void load(char *filename);
  void getClassificationName(int classID, char *className);
  void setClassificationName(int classID, char *className);
  void getPortrait(int classID, char *filename);
  void setPortrait(int classID, char *filename);

  Ttopmap(int inputsWidth, int inputsHeight, int mapWidth, int mapHeight);
  ~Ttopmap();
};


#endif
