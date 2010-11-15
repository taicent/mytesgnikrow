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

#include <stdio.h>
#include <math.h>
#include "classPersonalia.h"


/// <summary>
/// Constructor
/// </summary>
classPersonalia::classPersonalia()
{
  int i,j,k;

  currentTime=0;
  prev_personID=-1;
  
  for (i=0;i<MAX_PEOPLE;i++)
  {
	initialised[i]=false;
	currentWardrobeID[i]=0;

    for (j=0;j<PERSONAL_HISTORY;j++)
	  for (k=0;k<10;k++)
	    history[i][j][k]=0;

    for (j=0;j<5;j++)
	  for (k=0;k<3;k++)
	    wardrobe[i][j][k]=0;

    for (j=0;j<2;j++)
	{
      familiarity[i][j]=0;
	  intimacy[i][j]=0;
	  engagement[i][j]=0;
	  expressiveness[i][j]=0;
	  intensity[i][j]=0;
	  aggressiveness[i][j]=0;
	  verbosity[i][j]=0;
	  instructiveness[i][j]=0;
	}
  }
  load("c:\\develop\\hbp\\people.dat");
}

/// <summary>
/// Destructor
/// </summary>
classPersonalia::~classPersonalia()
{
  updateAverages(currentTime);
  save("c:\\develop\\hbp\\people.dat");
}



/// <summary>
/// update the colour of clothing
/// </summary>
/// <param name="personID">A unique ID number for the person</param>
/// <param name="r">red component of the clothing colour</param>
/// <param name="g">green component of the clothing colour</param>
/// <param name="b">blue component of the clothing colour</param>
void classPersonalia::updateWardrobe(int personID, int r, int g, int b)
{
  int i,min,dist,winner;

  winner = currentWardrobeID[personID];
  dist = abs(wardrobe[personID][winner][0] - r) +
		   abs(wardrobe[personID][winner][1] - g) +
		   abs(wardrobe[personID][winner][2] - b);
  if (dist>50)
  {
    min=99999;
    dist=0;
    winner=-1;
    for (i=0;i<5;i++)
	{
      dist = abs(wardrobe[personID][i][0] - r) +
		     abs(wardrobe[personID][i][1] - g) +
		     abs(wardrobe[personID][i][2] - b);
	  if (dist<min)
	  {
	    min=dist;
	    winner=i;
	  }
	}
  }

  if (winner>-1)
  {
	//update the wardrobe for the closest match
    wardrobe[personID][winner][0] = wardrobe[personID][winner][0] + ((r - wardrobe[personID][winner][0])/2);
    wardrobe[personID][winner][1] = wardrobe[personID][winner][1] + ((g - wardrobe[personID][winner][1])/2);
    wardrobe[personID][winner][2] = wardrobe[personID][winner][2] + ((b - wardrobe[personID][winner][2])/2);
	currentWardrobeID[personID] = winner;
  }
}

/// <summary>
/// update averages
/// </summary>
void classPersonalia::updateAverages(long time_elapsed)
{
  int i,j,k;
  long observation_time;

  if (time_elapsed>0)
  {
    for (i=0;i<MAX_PEOPLE;i++)
	{
      familiarity[i][0] = (familiarity[i][0] + ((familiarity[i][1] *100) / time_elapsed))/2;
	  if (initialised[i])
	  {
		//these parameters are updated only if the person was observed
		observation_time = familiarity[i][1];
		if (observation_time>0)
		{
	      intimacy[i][0] = (intimacy[i][0] + (intimacy[i][1] / observation_time))/2;
          engagement[i][0] = (engagement[i][0] + ((engagement[i][1] * 100) / observation_time))/2;
          expressiveness[i][0]  = 0;
          intensity[i][0]       = 0;
          aggressiveness[i][0]  = 0;
          verbosity[i][0]       = 0;
          instructiveness[i][0] = 0;
		}
	  }

	  //store the history
      for (j=PERSONAL_HISTORY-1;j>0;j--)
	    for (k=0;k<10;k++)
		  history[i][j][k] = history[i][j-1][k];

	  history[i][0][0] = familiarity[i][0];
	  history[i][0][1] = intimacy[i][0];
	  history[i][0][2] = engagement[i][0];
	  history[i][0][3] = expressiveness[i][0];
	  history[i][0][4] = intensity[i][0];
	  history[i][0][5] = aggressiveness[i][0];
      history[i][0][6] = verbosity[i][0];
	  history[i][0][7] = instructiveness[i][0];
	  history[i][0][8] = currentWardrobeID[i];

	}
  }
}


/// <summary>
/// update using the given person detector
/// </summary>
/// <param name="persondetector">person detection object</param>
void classPersonalia::update(classHumunculus *persondetector)
{
  int personID;

  if (persondetector!=NULL)
  {
    personID = persondetector->personID - 1;

    if ((personID>-1) && (personID<MAX_PEOPLE) && (personID==prev_personID))
	{

	  if (!initialised[personID])
	  {
	    //get the person's name
        persondetector->getPersonName(personID,name[personID]);
	    initialised[personID]=true;
	  }

	  //increase the familiarity with this person
      familiarity[personID][1]++;

	  //calculate intimacy
	  intimacy[personID][1] += persondetector->bodyWidth;

	  //if the person is within "personal space" range then assume they are engaging the robot
	  if (persondetector->bodyWidth>25) engagement[personID][1]++;

	  //update colour of clothing being worn
	  updateWardrobe(personID,persondetector->bodyColour[0],persondetector->bodyColour[1],persondetector->bodyColour[2]);
	}

    prev_personID = personID;
  }
}



/// <summary>
/// Save details about observed people
/// </summary>
/// <param name="filename">The filename to save to</param>
void classPersonalia::save(char *filename)
{
  int i,j,k;
  FILE *fp;
 
  fp = fopen(filename, "w");

  if (fp != NULL)
  {

    fprintf(fp,"%d\n",MAX_PEOPLE);
    for (i=0;i<MAX_PEOPLE;i++)
	{
      fprintf(fp,"%ld\n",familiarity[i][0]);
	  fprintf(fp,"%ld\n",engagement[i][0]);
	  fprintf(fp,"%ld\n",expressiveness[i][0]);
	  fprintf(fp,"%ld\n",intimacy[i][0]);
	  fprintf(fp,"%ld\n",intensity[i][0]);
	  fprintf(fp,"%ld\n",aggressiveness[i][0]);
	  fprintf(fp,"%ld\n",verbosity[i][0]);
	  fprintf(fp,"%ld\n",instructiveness[i][0]);

	  fprintf(fp,"%d\n",currentWardrobeID[i]);
      for (j=0;j<5;j++)
	  {
		for (k=0;k<3;k++)
	      fprintf(fp,"%d\n",wardrobe[i][j][k]);
	  }

      for (j=0;j<PERSONAL_HISTORY;j++)
	    for (k=0;k<10;k++)
	      fprintf(fp,"%ld\n",history[i][j][k]);
	}

	fclose(fp);
  }
}


/// <summary>
/// Load details about observed people
/// </summary>
/// <param name="filename">The filename to load from</param>
void classPersonalia::load(char *filename)
{
  int i,j,k,NoOfPeople,p;
  long v;
  FILE *fp;
 
  fp = fopen(filename, "r");

  if (fp != NULL)
  {

    fscanf(fp,"%d\n",&NoOfPeople);
    for (i=0;i<NoOfPeople;i++)
	{
      fscanf(fp,"%ld\n",&v);
	  familiarity[i][0] = v;

	  fscanf(fp,"%ld\n",&v);
	  engagement[i][0] = v;

	  fscanf(fp,"%ld\n",&v);
	  expressiveness[i][0] = v;

	  fscanf(fp,"%ld\n",&v);
	  intimacy[i][0] = v;

	  fscanf(fp,"%ld\n",&v);
	  intensity[i][0] = v;

	  fscanf(fp,"%ld\n",&v);
	  aggressiveness[i][0] = v;

	  fscanf(fp,"%ld\n",&v);
	  verbosity[i][0] = v;

	  fscanf(fp,"%ld\n",&v);
	  instructiveness[i][0] = v;

	  fscanf(fp,"%d\n",&p);
	  currentWardrobeID[i] = p;
      for (j=0;j<5;j++)
	  {
		for (k=0;k<3;k++)
		{
	      fscanf(fp,"%d\n",&p);
		  wardrobe[i][j][k]=p;
		}
	  }
	
      for (j=0;j<PERSONAL_HISTORY;j++)
	  {
	    for (k=0;k<10;k++)
		{
	      fscanf(fp,"%ld\n",&v);
		  history[i][j][k] = v;
		}
	  }
	}

	fclose(fp);
  }
}


