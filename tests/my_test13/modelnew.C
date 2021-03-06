//-------------------------------------------------------------------------
//                  The Motion Strategy Library (MSL)
//-------------------------------------------------------------------------
//
// Copyright (c) 2003 University of Illinois and Steven M. LaValle
// All rights reserved.
//
// Developed by:                Motion Strategy Laboratory
//                              University of Illinois
//                              http://msl.cs.uiuc.edu/msl/
//
// Versions of the Motion Strategy Library from 1999-2001 were developed
// in the Department of Computer Science, Iowa State University.
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal with the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
// 
//     * Redistributions of source code must retain the above copyright 
//       notice, this list of conditions and the following disclaimers.
//     * Redistributions in binary form must reproduce the above copyright 
//       notice, this list of conditions and the following disclaimers in 
//       the documentation and/or other materials provided with the 
//       distribution.
//     * Neither the names of the Motion Strategy Laboratory, University
//       of Illinois, nor the names of its contributors may be used to 
//       endorse or promote products derived from this Software without 
//       specific prior written permission.
//
// The software is provided "as is", without warranty of any kind,
// express or implied, including but not limited to the warranties of
// merchantability, fitness for a particular purpose and
// noninfringement.  In no event shall the contributors or copyright
// holders be liable for any claim, damages or other liability, whether
// in an action of contract, tort or otherwise, arising from, out of or
// in connection with the software or the use of other dealings with the
// software.
//
//-------------------------------------------------------------------------









#include <fstream>
#include <math.h>
using namespace std;

#include </home/roomba/Documents/tri/MSLlibrary/include/MSL/model2d.h>
#include </home/roomba/Documents/tri/MSLlibrary/include/MSL/model3d.h>

#include "modelnew.h"


// *********************************************************************
// *********************************************************************
// CLASS:     Model2DRigidMulti2
// 
// *********************************************************************
// *********************************************************************


// Constructor
Model2DRigidMulti2::Model2DRigidMulti2(string path = ""):Model2DRigidMulti(path) {
  MSLVector u;
  int i,j;
  time = 0.0;
  READ_PARAMETER_OR_ERROR(NumBodies);

  StateDim = 3*NumBodies+1; //+1 indicates time is added to parameters
  InputDim = 3*NumBodies;
  READ_PARAMETER_OR_DEFAULT(LowerState,MSLVector(StateDim));
  READ_PARAMETER_OR_DEFAULT(UpperState,MSLVector(StateDim));

  u = MSLVector(StateDim);
  Inputs.clear();
  for (i = 0; i < StateDim; i++) {
    for (j = 0; j < StateDim; j++)
      u[j] = (i==j) ? 1.0 : 0.0; 
    Inputs.push_back(u);
    for (j = 0; j < StateDim; j++)
      u[j] = (i==j) ? -1.0 : 0.0; 
    Inputs.push_back(u);
  }
}


double Model2DRigidMulti2::Metric(const MSLVector &x1, const MSLVector &x2) {

  double d,fd,dtheta;
  int i;

  d = 0.0;
    if((x1[3*NumBodies]-x2[3*NumBodies])>0) return 100000.0;

  for (i = 0; i < NumBodies; i++) {
    fd = fabs(x1[3*i+2]-x2[3*i+2]);
    dtheta = min(fd,2.0*PI - fd);
    //d += sqr(x1[3*i] - x2[3*i]);
    d += pow(x1[3*i] - x2[3*i],2);
    //d += sqr(x1[3*i+1] - x2[3*i+1]);
    d += pow(x1[3*i+1] - x2[3*i+1],2);
    //d += sqr(dtheta);
    d += pow(dtheta,2);
  }

  return sqrt(d);
}

MSLVector Model2DRigidMulti2::StateToConfiguration(const MSLVector &x)
{
  return x;
}

