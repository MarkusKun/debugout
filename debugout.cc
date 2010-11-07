#include <fstream>
#include <iostream>
#include <string> // used for getline(stream,string)
#include "debugout.h"

dout_t dout;

void dout_t::loadConfigFile(const std::string filename){
  #ifdef NO_OUTPUT
  { // do nothing, return
    return;
  }
  #endif
  // activate this scope to get more info while reading
  startScope("dout_t::loadConfigFile");  
  using std::cerr;  using std::endl;
  
  std::ifstream fileStream(filename.c_str());
  if (!fileStream.is_open()){
    cerr << "error, file " << filename << " could not be opened" << endl;
    endScope("dout_t::loadConfigFile");
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
    (*this) << ": Line: " << curLine << "(" << curLine.size() << ")" ;
    if ("//" == curLine.substr(0,2)){ // line starts with "//"
      (*this) << " NOT ACTIVE" << endl;
      continue;
    }
    // if we reach here, we have an activated scope
    activateScope(curLine);
    (*this) << " ACTIVE" << endl;
  }
  endScope("dout_t::loadConfigFile");
}



void dout_t::startScope(const std::string scopeName){
  #ifdef NO_OUTPUT
  { // do nothing, return
    return;
  }
  #endif
  // add to stack
  currentScopeStack.push(scopeName); 
  
  // check whether new scope is active
  currentActive = isActivated(scopeName);
  
  /*
   * print entering to screen
   * Note: By using dout, we don't need to check against active scope,
   * ourselves.
   */
  (*this) << ">>" << currentScopeStack.size() << " " << scopeName << std::endl;
}
void dout_t::endScope(){
  #ifdef NO_OUTPUT
  { // do nothing, return
    return;
  }
  #endif
  { // remove old scope from stack 
    std::string currentScope = currentScopeStack.top();
    /*
     * print exiting to screen
     * Note: By using dout, we don't need to check against active scope,
     * ourselves.
     */
    (*this) << "<<" << currentScopeStack.size() << " " << currentScope << std::endl;
    currentScopeStack.pop(); // remove from stack
  }
  { // check whether next scope is active
    if (!currentScopeStack.empty()){
      currentActive = isActivated(currentScopeStack.top());
    }else{ // if stack is empty, we are not active
      currentActive = false;
    }
  }
}
void dout_t::endScope(const std::string scopeName){
  #ifdef NO_OUTPUT
  { // do nothing, return
    return;
  }
  #endif
  using std::cerr; using std::endl;
  
  std::string currentScope=currentScopeStack.top();
  if (scopeName!=currentScope){
    cerr << "----------------------------" << endl;
    cerr << "debug error on ending Scope!" << endl;
    cerr << "Given Scope  : " << scopeName << endl;
    cerr << "Current Scope: " << currentScope << endl;
    cerr << "Ending current Scope, may lead to more errors!" << endl;
    cerr << "----------------------------" << endl;
  }
  endScope();
}


void dout_t::dout(const std::string debugOutput){
  #ifdef NO_OUTPUT
  { // do nothing, return
    return;
  }
  #endif
  if (currentActive){
    std::cout << debugOutput << std::endl;
  }
}

// output functions, "inheritance" of <<   BEGIN 
  dout_t& dout_t::operator<< (const char* output ){
    #ifndef NO_OUTPUT
    { // only if output is globally active
      if (currentActive){
        std::cout << output;
      }
    }
    #endif
    return *this;
  }
  dout_t& dout_t::operator<< (const int output ){
    #ifndef NO_OUTPUT
    { // only if output is globally active
      if (currentActive){
        std::cout << output;
      }
    }
    #endif 
    return *this;
  }
  dout_t& dout_t::operator<< (const std::string output){
    #ifndef NO_OUTPUT
    { // only if output is globally active
      if (currentActive){
        std::cout << output;
      }
    }
    #endif 
    return *this;
  }

  dout_t& dout_t::operator<< (std::ostream& ( *functionPointer)(std::ostream&)){
    #ifndef NO_OUTPUT
    { // only if output is globally active
      if (currentActive){
        functionPointer(std::cout); // this handles stream manipulators like endl
      }
    }
    #endif 
    return *this;
  }
  dout_t& dout_t::operator<< (std::ios_base& ( *functionPointer)(std::ios_base&)){
    #ifndef NO_OUTPUT
    { // only if output is globally active
      if (currentActive){
        functionPointer(std::cout); 
      }
    }
    #endif 
    return *this;
  }
// output functions, "inheritance" of <<   END 

// setting and getting activation of scopes BEGIN 
void dout_t::activateScope(const std::string scopeName){
  #ifdef NO_OUTPUT
  { // do nothing, return
    return;
  }
  #endif
  setScopeActive(scopeName,true);
}
    
void dout_t::deactivateScope(const std::string scopeName){
  #ifdef NO_OUTPUT
  { // do nothing, return
    return;
  }
  #endif
  setScopeActive(scopeName,false);
}

void dout_t::setScopeActive(const std::string scopeName, bool activate){
  #ifdef NO_OUTPUT
  { // do nothing, return
    return;
  }
  #endif
  if (activate){ // activate: put string into set
    activatedScopes.insert(scopeName);
  }else{ // deactivate: remove from set
    activatedScopes.erase(scopeName);
  }
}



bool dout_t::isActivated(const std::string scopeName){
  #ifdef NO_OUTPUT
  { // nothing is activated
    return false;
  }
  #endif
  std::set < std::string >::const_iterator scopeSearcher;
  scopeSearcher = activatedScopes.find(scopeName);
  
  if (activatedScopes.end() == scopeSearcher){ // not found
    return false;
  }
  //else (found)
  return true;
}
// setting and getting activation of scopes END 

// debugdebug output START
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
    
    outStream << "----- stackdisplay END -----" << endl;
  }
// debugedebug output END

