
#include "string.h"

using namespace std;

#if defined(__MINGW32__) || defined(__CYGWIN__)

#include <sstream>
string to_string(int val){
   stringstream ss;
   ss << val;
   return ss.str();
}

string to_string(unsigned val){
   stringstream ss;
   ss << val;
   return ss.str();
}

string to_string(long val){
   stringstream ss;
   ss << val;
   return ss.str();
}

string to_string(unsigned long val){
   stringstream ss;
   ss << val;
   return ss.str();
}

string to_string(long long val){
   stringstream ss;
   ss << val;
   return ss.str();
}

string to_string(unsigned long long val){
   stringstream ss;
   ss << val;
   return ss.str();
}

string to_string(float val){
   stringstream ss;
   ss << val;
   return ss.str();
}

string to_string(double val){
   stringstream ss;
   ss << val;
   return ss.str();
}

string to_string(long double val){
   stringstream ss;
   ss << val;
   return ss.str();
}

#endif
