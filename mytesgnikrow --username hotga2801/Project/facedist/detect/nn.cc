#if !defined(hpux) && !defined(WINNT)
extern "C" {
#include <math.h>
}
#endif

#if defined(hpux) || defined(WINNT)
#include <math.h>
#endif

#include "system.hpp"
#include "nn.hpp"

#ifdef WINNT
#undef random
#define random rand
#endif

#define BigMatrix Mat
#define BigVector Vec

#define MinRate 0.0001

Vec Sigmoid(const Vec &vec)
{
  int len=vec.rows;
  Vec ans(len);
  Element *vPtr=vec.data->elements;
  Element *aPtr=ans.data->elements;
  for (int i=0; i<len; i++)
    *(aPtr++)=1.0/(1.0+exp(-(*(vPtr++))));
  return ans;
}

Vec DerivSigmoid(const Vec &vec)
{
  int len=vec.rows;
  Vec ans(len);
  Element *vPtr=vec.data->elements;
  Element *aPtr=ans.data->elements;
  for (int i=0; i<len; i++)
  {
    Element val=*(vPtr++);
    *(aPtr++)=val*(1.0-val);
  }
  return ans;
}

double TrainThreeLayer(Mat &con1, Mat &con2, 
                       Vec *inputs, Vec *outputs,
                       int samples, int epochs)
{
  int blockSize=100;
  int pos=0, neg=0;
  for (int i=0; i<samples; i++)
  {
    for (int j=0; j<inputs[i].rows-1; j++)
      inputs[i](j)=1.0/inputs[i](j);
    inputs[i](inputs[i].rows-1)=0.5;
    if (outputs[i](0)>0.5) pos++; else neg++;
  }
// Sort of permute the samples
  for (i=0; i<samples; i++)
  {
    int dest=random() % samples;
    Vec tmp=inputs[i]; inputs[i]=inputs[dest]; inputs[dest]=tmp;
    tmp=outputs[i]; outputs[i]=outputs[dest]; outputs[dest]=tmp;
  }
//  Element posWeight=0.5/(Element)pos;
//  Element negWeight=0.5/(Element)neg;
  Element posWeight=1.0; // /(Element)(pos+neg);
  Element negWeight=1.0; // /(Element)(pos+neg);
  Element *weights=new Element[samples];
  for (i=0; i<samples; i++)
    if (outputs[i](0)>0.5) weights[i]=posWeight; else
      weights[i]=negWeight;
  Element rate=0.01;
  Element momentum=0.5;

  BigMatrix prevDelta1(con1.rows,con1.cols);
  BigMatrix prevDelta2(con2.rows,con2.cols);
  BigMatrix delta1(con1.rows,con1.cols);
  BigMatrix delta2(con2.rows,con2.cols);
  BigVector *hiddenOutput=new Vec[samples];
  BigVector *output=new Vec[samples];
  BigVector *errorOutput=new Vec[samples];

  BigVector input, hiddenInput, outputInput, 
    outputDelta, hiddenDelta;

  int iter=0;

  while (iter<epochs)
  {
    for (int block=0; block<samples; block+=blockSize)
    {
      double error=0;
      int facePos=0, faceNeg=0, nonFacePos=0, nonFaceNeg=0;
      for (int i=block; i<block+blockSize && i<samples; i++)
      {
        hiddenInput=con1*inputs[i];
        hiddenOutput[i]=Sigmoid(hiddenInput);
        outputInput=con2*hiddenOutput[i];
        output[i]=Sigmoid(outputInput);
        errorOutput[i]=outputs[i]-output[i];
        error+=weights[i]*Dot(errorOutput[i],errorOutput[i]);
        if (outputs[i](0)>0.5) 
        {
          if (output[i](0)>0.5) facePos++; else faceNeg++;
        } else
        {
          if (output[i](0)>0.5) nonFacePos++; else nonFaceNeg++;
        }
      }
      error=sqrt(error/blockSize);
      fprintf(stderr,"  Batch Errors: %g   %d/%d %d/%d  tot=%d\n",
              error,facePos,faceNeg,nonFacePos,nonFaceNeg,faceNeg+nonFacePos);
      delta1=Zero(delta1.rows,delta1.cols);
      delta2=Zero(delta2.rows,delta2.cols);
      for (i=block; i<block+blockSize && i<samples; i++)
      {
        outputDelta=MultiplyTerms(DerivSigmoid(output[i]),errorOutput[i]);
        hiddenDelta=MultiplyTerms(DerivSigmoid(hiddenOutput[i]),
                                  Transpose(con2)*outputDelta);
        delta2+=outputDelta*Transpose(weights[i]*hiddenOutput[i]);
        delta1+=hiddenDelta*Transpose(weights[i]*inputs[i]);
      }
      prevDelta1=rate*delta1+momentum*prevDelta1;
      prevDelta2=rate*delta2+momentum*prevDelta2;
      con1+=prevDelta1;
      con2+=prevDelta2;
      
      for (i=0; i<con1.cols; i++) con1(con1.rows-1,i)=0;
    }

    Element error=0;
    int facePos=0, faceNeg=0, nonFacePos=0, nonFaceNeg=0;
    for (int i=0; i<samples; i++)
    {
      outputs[i]=Sigmoid(con2*Sigmoid(con1*inputs[i]));
      errorOutput[i]=outputs[i]-output[i];
      error+=weights[i]*Dot(errorOutput[i],errorOutput[i]);
      if (outputs[i](0)>0.5) 
      {
        if (output[i](0)>0.5) facePos++; else faceNeg++;
      } else
      {
        if (output[i](0)>0.5) nonFacePos++; else nonFaceNeg++;
      }
    }
    error=sqrt(error/samples);
    fprintf(stderr,"Epoch=%d: Errors: %g   %d/%d %d/%d  tot=%d\n",
            iter++,
            error,facePos,faceNeg,nonFacePos,nonFaceNeg,faceNeg+nonFacePos);
  }

  delete[] weights;
  delete[] hiddenOutput;
  delete[] output;
  delete[] errorOutput;
  return 0.0;
}

#ifdef notdef
double TrainThreeLayer(Mat &con1, Mat &con2, 
                       Vec *inputs, Vec *outputs,
                       int samples, int epochs)
{
  int pos=0, neg=0;
  for (int i=0; i<samples; i++)
  {
    for (int j=0; j<inputs[i].rows-1; j++)
      inputs[i](j)=1.0/inputs[i](j);
    inputs[i](inputs[i].rows-1)=0.5;
    if (outputs[i](0)>0.5) pos++; else neg++;
  }
//  Element posWeight=0.5/(Element)pos;
//  Element negWeight=0.5/(Element)neg;
  Element posWeight=1.0/(Element)(pos+neg);
  Element negWeight=1.0/(Element)(pos+neg);
  Element *weights=new Element[samples];
  for (i=0; i<samples; i++)
    if (outputs[i](0)>0.5) weights[i]=posWeight; else
      weights[i]=negWeight;
  Element rate=1.0;
  Element momentum=0.5;

  Element bestError=-1.0;
  BigMatrix best1=con1;
  BigMatrix best2=con2;
  BigMatrix prevCon1=con1;
  BigMatrix prevCon2=con2;
  Element prevError=-1.0;
  BigMatrix prevDelta1(con1.rows,con1.cols);
  BigMatrix prevDelta2(con2.rows,con2.cols);
  BigMatrix delta1(con1.rows,con1.cols);
  BigMatrix delta2(con2.rows,con2.cols);
  BigVector *hiddenOutput=new Vec[samples];
  BigVector *output=new Vec[samples];
  BigVector *errorOutput=new Vec[samples];

  BigVector input, hiddenInput, outputInput, 
    outputDelta, hiddenDelta;

  int iter=0;

  while (iter<epochs)
  {
    Element error=0;
    int facePos=0, faceNeg=0, nonFacePos=0, nonFaceNeg=0;
    for (int i=0; i<samples; i++)
    {
      hiddenInput=con1*inputs[i];
      hiddenOutput[i]=Sigmoid(hiddenInput);
      outputInput=con2*hiddenOutput[i];
      output[i]=Sigmoid(outputInput);
      errorOutput[i]=outputs[i]-output[i];
      error+=weights[i]*Dot(errorOutput[i],errorOutput[i]);
//      error+=Abs(errorOutput[i]);
      if (outputs[i](0)>0.5) 
      {
        if (output[i](0)>0.5) facePos++; else faceNeg++;
      } else
      {
        if (output[i](0)>0.5) nonFacePos++; else nonFaceNeg++;
      }
    }
    error=sqrt(error);
//    error=error/samples;
    fprintf(stderr,"Errors: %g (prev=%g) %d/%d %d/%d  tot=%d\n",
            error,prevError,
            facePos,faceNeg,nonFacePos,nonFaceNeg,faceNeg+nonFacePos);
//    if (error>prevError && prevError>=0)
    if (0)
    {
      con1=prevCon1;
      con2=prevCon2;
      prevDelta1=Zero(prevDelta1.rows,prevDelta1.cols);
      prevDelta2=Zero(prevDelta2.rows,prevDelta2.cols);
      rate*=0.5;
      fprintf(stderr,"Cutting rate to %g\n",rate);
      if (rate<MinRate) break;
      continue;
    }
//    if (error<prevError) rate+=0.1;
    if (bestError<0 || error<bestError)
    {
      best1=con1;
      best2=con2;
      bestError=error;
    }
    prevError=error;
    prevCon1=con1;
    prevCon2=con2;
    delta1=Zero(delta1.rows,delta1.cols);
    delta2=Zero(delta2.rows,delta2.cols);
    for (i=0; i<samples; i++)
    {
      outputDelta=MultiplyTerms(DerivSigmoid(output[i]),errorOutput[i]);
      hiddenDelta=MultiplyTerms(DerivSigmoid(hiddenOutput[i]),
                                Transpose(con2)*outputDelta);
      delta2+=outputDelta*Transpose(weights[i]*hiddenOutput[i]);
      delta1+=hiddenDelta*Transpose(weights[i]*inputs[i]);
    }
    prevDelta1=rate*delta1+momentum*prevDelta1;
    prevDelta2=rate*delta2+momentum*prevDelta2;
    con1+=prevDelta1;
    con2+=prevDelta2;

    for (i=0; i<con1.cols; i++) con1(con1.rows-1,i)=0;

    fprintf(stderr,"Iter=%d, Rate=%g\n",iter++,rate);
  }

  con1=best1;
  con2=best2;

  delete[] weights;
  delete[] hiddenOutput;
  delete[] output;
  delete[] errorOutput;
  return bestError;
}
#endif

#define DeterministicPerceptron

#ifndef DeterministicPerceptron
double TrainTwoLayer(Mat &con1, 
                     Vec *inputs, Vec *outputs,
                     int samples, int epochs)
{
  int pos=0, neg=0;
  for (int i=0; i<samples; i++)
  {
    for (int j=0; j<inputs[i].rows-1; j++)
      inputs[i](j)=log(inputs[i](j));
    inputs[i](inputs[i].rows-1)=0.5;
    if (outputs[i](0)>0.5) pos++; else neg++;
  }
//  Element posWeight=0.5/(Element)pos;
//  Element negWeight=0.5/(Element)neg;
  Element posWeight=1.0; // /(Element)(pos+neg);
  Element negWeight=1.0; // /(Element)(pos+neg);

  Element *weights=new Element[samples];
  for (i=0; i<samples; i++)
    if (outputs[i](0)>0.5) weights[i]=posWeight; else
      weights[i]=negWeight;
  Element rate=0.1;
  Element momentum=0.9;

  Element bestError=-1.0;
  BigMatrix best1=con1;
  BigMatrix prevCon1=con1;
  Element prevError=-1.0;
  BigMatrix prevDelta1(con1.rows,con1.cols);
  BigMatrix delta1(con1.rows,con1.cols);
  BigVector *output=new Vec[samples];
  BigVector *errorOutput=new Vec[samples];

  BigVector input, outputDelta, outputInput;

  int iter=0;

  while (iter<epochs)
  {
    Element error=0;
    int facePos=0, faceNeg=0, nonFacePos=0, nonFaceNeg=0;
    for (int i=0; i<samples; i++)
    {
      outputInput=con1*inputs[i];
      output[i]=outputInput;
      errorOutput[i]=outputs[i]-output[i];
      error+=weights[i]*Dot(errorOutput[i],errorOutput[i]);
      if (outputs[i](0)>0.5) 
      {
        if (output[i](0)>0.5) facePos++; else faceNeg++;
      } else
      {
        if (output[i](0)>0.5) nonFacePos++; else nonFaceNeg++;
      }
    }
    error=sqrt(error);
    fprintf(stderr,"Errors: %g (prev=%g) %d/%d %d/%d  tot=%d\n",
            error,prevError,
            facePos,faceNeg,nonFacePos,nonFaceNeg,faceNeg+nonFacePos);
    if (error>prevError && prevError>=0)
    {
      con1=prevCon1;
      prevDelta1=Zero(prevDelta1.rows,prevDelta1.cols);
      rate*=0.5;
      fprintf(stderr,"Cutting rate to %g\n",rate);
      if (rate<MinRate) break;
      continue;
    }
    if (error<prevError) rate+=0.05;
    if (bestError<0 || error<bestError)
    {
      best1=con1;
      bestError=error;
    }
    prevError=error;
    prevCon1=con1;
    delta1=Zero(delta1.rows,delta1.cols);
    for (i=0; i<samples; i++)
    {
      outputDelta=errorOutput[i];
      delta1+=outputDelta*Transpose(weights[i]*inputs[i]);
    }
    prevDelta1=rate*delta1+momentum*prevDelta1;
    con1+=prevDelta1;

    fprintf(stderr,"Iter=%d, Rate=%g\n",iter++,rate);
  }

  con1=best1;

  delete[] weights;
  delete[] output;
  delete[] errorOutput;
  return bestError;
}
#endif

#ifdef DeterministicPerceptron
double TrainTwoLayer(Mat &con1, 
                     Vec *inputs, Vec *outputs,
                     int samples, int epochs)
{
  fprintf(stderr,"Initializing...\n");
  int pos=0, neg=0, len=inputs[0].rows, i, j;
  Element *weights=new Element[samples];
  for (i=0; i<samples; i++)
  {
//    for (j=0; j<inputs[i].rows-1; j++)
//      inputs[i](j)=log(inputs[i](j));
    inputs[i](inputs[i].rows-1)=0.5;
    if (outputs[i](0)>0.5) pos++; else neg++;
  }
//  Element posWeight=0.5/(Element)pos;
//  Element negWeight=0.5/(Element)neg;
  Element posWeight=1.0/(Element)(pos+neg);
  Element negWeight=1.0/(Element)(pos+neg);
  for (i=0; i<samples; i++)
    if (outputs[i](0)>0.5) weights[i]=posWeight; else weights[i]=negWeight;
  fprintf(stderr,"Computing covariance...\n");
  Mat cov(len,len);
  for (i=0; i<len; i++) for (j=0; j<len; j++)
  {
    double tot=0.0;
    for (int k=0; k<samples; k++)
      tot+=weights[k]*inputs[k](i)*inputs[k](j);
    cov(i,j)=tot;
  }
  cov=LUInverse(cov);
  fprintf(stderr,"Fitting plane...\n");
  con1=Zero(len);
  for (i=0; i<samples; i++)
    con1+=outputs[i](0)*weights[i]*cov*inputs[i];
  con1=Transpose(con1);
  delete[] weights;
  return 0.0;
}
#endif

double TrainThreeLayerBinary(Mat &con1, Mat &con2, 
                             signed char *data, Vec *outputs,
                             int samples, int epochs)
{
  return 0.0;
}

