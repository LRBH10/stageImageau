/*
 * ReportData.cpp
 *
 *  Created on: 25 mai 2012
 *      Author: rli
 */

#include "ReportData.h"

using namespace OperationsSMD;

ReportData::ReportData (long id, bool status, const string &contents) :
   id (id), status (status), contents (contents)
{

}

ReportData::ReportData (long id) :
   id (id), status (true), contents ()
{

}

ReportData::~ReportData ()
{

}

void ReportData::append (ReportData *other)
{
   this->contents.append (other->contents);
   this->status = this->status && other->status;
}
