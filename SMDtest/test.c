#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <iostream>
#include <map>
#include <string>

typedef boost::function<void(void)> fun_t;
typedef std::map<std::string, fun_t> funs_t;

void foo() { std::cout <<"\n"; }
void bar(int p) { std::cout<<"("<<p<<")\n"; }
void goo(std::string p) { std::cout<<"("<<p<<")\n";}

int main()
{
 funs_t f;
 f["foo"] = boost::bind(foo);
 f["bar"] = boost::bind(bar, 17);
 f["goo"] = boost::bind(goo, "I am goo");

 for (funs_t::iterator it = f.begin(); it != f.end(); ++it)
 {
  std::cout << (*it).first << "->";
  (*it).second();
 }

 f["bar"]();

 return 0;
}



n     ->      50
i     ->       ?
