#include "SubProperties.h"

SubProperties::SubProperties (const Properties & pp, const string & pf) :
   prop (pp), prefix (pf)
{
}

SubProperties::SubProperties (const SubProperties & pp, const string & pf) :
   prop (pp.prop), prefix (pp.prefix + pf)
{
}

SubProperties::~SubProperties ()
{
}

string SubProperties::get (const string & key, bool optional, const string & defVal) const
{
   return this->prop.get (this->prefix + key, optional, defVal);
}

int SubProperties::getInt (const string & key, bool optional, int defVal) const
{
   return this->prop.getInt (this->prefix + key, optional, defVal);
}

double SubProperties::getDouble (const string & key, bool optional, double defVal) const
{
   return this->prop.getDouble (this->prefix + key, optional, defVal);
}

string SubProperties::getPrefix () const
{
   return prefix;
}

const Properties & SubProperties::getProperties () const
{
   return this->prop;
}

void SubProperties::setPrefix (string prefix)
{
   this->prefix = prefix;
}

bool SubProperties::getBoolean (const string & key, bool optional, bool defVal) const
{
   return this->prop.getBoolean (this->prefix + key, optional, defVal);
}

