/***************************************************************************
                          classPersonalia.cpp

  Keeps track of different people

    begin                : Sun Feb 15 2004
    copyright            : (C) 2004 by Bob Mottram
    email                : fuzzgun@btinternet.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef CLASS_PERSONALIA_H
#define CLASS_PERSONALIA_H


/**
  *@author Bob Mottram
  */

//the maximum number of people identifiable o the robot
#define MAX_PEOPLE 10

//the number of historical records to keep
#define PERSONAL_HISTORY 10


#include "stdafx.h"
#include <afxwin.h>
//#include "classHumunculus.h"



/// <summary>
/// This class is used to store various pieces of statistical information about individuals.
/// </summary>
class classPersonalia {
private:

  int prev_personID;

  long history[MAX_PEOPLE][PERSONAL_HISTORY][10];

  //set to true if the person has been observed
  bool initialised[MAX_PEOPLE];

  //how often they have been observed within a fixed time period
  long familiarity[MAX_PEOPLE][2];

  //the average amount of time spent interacting with the robot
  long engagement[MAX_PEOPLE][2];

  //the average physical distance of the person from the robot
  long intimacy[MAX_PEOPLE][2];

  //the variety of face and body movements of the person
  long expressiveness[MAX_PEOPLE][2];

  //the ratio of time spent in direct eye contact to the time spent looking away from the robot
  long intensity[MAX_PEOPLE][2];
                
  //the amount of time the person enegaged in activity which is considered hostile to the robot
  long aggressiveness[MAX_PEOPLE][2];
           
  //the average amount of time the person spends activating the robots speech interface
  long verbosity[MAX_PEOPLE][2];
                    
  //amount of time spent engaging in teaching activities
  long instructiveness[MAX_PEOPLE][2];

  //colours/textures of clothing typically worn by this person
  int wardrobe[MAX_PEOPLE][5][3];
  int currentWardrobeID[MAX_PEOPLE];
  
  //updates the colour of clothing worn
  void updateWardrobe(int personID, int r, int g, int b);

public:

  //name of the person
  char name[MAX_PEOPLE][20];

  //the current time
  long currentTime;

  void update(classHumunculus *persondetector);
  void updateAverages(long time_elapsed);
  void save(char *filename);
  void load(char *filename);

  classPersonalia();
  ~classPersonalia();
};

#endif
