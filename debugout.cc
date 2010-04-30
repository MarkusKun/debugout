#include <fstream>
#include <iostream>
#include <string> // used for getline(stream,string)
#include "debugout.h"

dout_t dout;

void dout_t::loadConfigFile(const std::string filename){
  using std::cerr;  using std::endl;
  
  using std::cout; // possibly shouldn't be used
  
  std::ifstream fileStream(filename.c_str());
  if (!fileStream.is_open()){
    cerr << "error, file " << filename << " could not be opened" << endl;
    return;
  }
  std::string curLine;

  for (;getline(fileStream,curLine);fileStream.eof()){ // for all lines in file
    { // remove spaces and control chars from the end
      while (true){
        if (0==curLine.size()){ // empty line or deleted completely
          break;
        }
        char lastChar = curLine.at(curLine.size()-1);
        if ((0 <= lastChar) && (lastChar <= 32)){ // non-printable
          curLine=curLine.substr(0,curLine.size()-1); // delete last
          continue;
        }
        break; // we reached a printable char
      }
    }
    cout << "DEBUG: Line: " << curLine << "(" << curLine.size() << ")" ;
    if ("//" == curLine.substr(0,2)){ // line starts with "//"
      cout << " NOT ACTIVE" << endl;
      continue;
    }
    // if we reach here, we have an activated scope
    activatedScopes.insert(curLine);
    cout << " ACTIVE" << endl;
  }
}

void dout_t::printActivatedScopes(std::ostream& outStream){
  using std::endl;
  
  outStream << "----- activated scopes START -----" << endl;
  
  std::set < std::string >::const_iterator scopeIterator;
  for(
    scopeIterator  = activatedScopes.begin();
    scopeIterator != activatedScopes.end();
    scopeIterator++
    )
  {
    outStream << (*scopeIterator) << endl;
  }
  outStream << "----- activated scopes END -----" << endl;
}

void dout_t::printStack(std::ostream& outStream){
  using std::endl;
  
  outStream << "----- stackdisplay START -----" << endl;
  std::stack < std::string> stackCopy = currentScopeStack;
  while(!stackCopy.empty()){
    unsigned int stackDepth=stackCopy.size();
    while(stackDepth>0){
      outStream << " ";
      stackDepth--;
    }
    outStream << stackCopy.top() << endl;
    
    stackCopy.pop();
  }
  
  outStream << "----- stackdisplay STOP -----" << endl;
}


void dout_t::startScope(const std::string scopeName){
  currentScopeStack.push(scopeName);
  currentActive = isActivated(scopeName);
}

bool dout_t::isActivated(const std::string scopeName){
  std::set < std::string >::const_iterator scopeSearcher;
  scopeSearcher = activatedScopes.find(scopeName);
  
  if (activatedScopes.end() == scopeSearcher){ // not found
    return false;
  }
  //else (found)
  return true;
  
}

void dout_t::dout(const std::string debugOutput){
  if (currentActive){
    std::cout << debugOutput << std::endl;
  }
}

dout_t& dout_t::operator<< (const char* output ){
  if (currentActive){
    std::cout << output;
  }
  return *this;
}
dout_t& dout_t::operator<< (const int output ){
  if (currentActive){
    std::cout << output;
  }
  return *this;
}

dout_t& dout_t::operator<< (std::ostream& ( *functionPointer)(std::ostream&)){
  if (currentActive){
    functionPointer(std::cout); // this handles stream manipulators like endl
  }
  return *this;
}

dout_t& dout_t::operator<< (std::ios_base& ( *functionPointer)(std::ios_base&)){
  if (currentActive){
    functionPointer(std::cout); 
  }
  return *this;
}





