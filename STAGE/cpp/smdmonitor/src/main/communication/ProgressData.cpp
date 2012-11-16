/*
 * OperatipnProgress.cpp
 *
 *  Created on: 20 juin 2012
 *      Author: rli
 */

#include "ProgressData.h"

using namespace OperationsSMD;

ProgressData::ProgressData () :
   id (0), title (), value (0.0)
{

}

ProgressData::ProgressData (uint32 id, const string &title, double value) :
   id (id), title (title), value (value)
{

}

ProgressData::~ProgressData ()
{

}

void ProgressData::debug ()
{
   cout << "Id : " << id << endl;
   cout << "Title : " << title << endl;
   cout << "Value : " << value << endl;
}
