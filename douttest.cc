#include <iostream>
#include "debugout.h"


int main(){
  using std::cout; using std::endl;
  
  dout.loadConfigFile("debug.config");
  
  dout.startScope("main");
  #ifdef SHOW_ACTIVE_SCOPES
  {
    cout << "Aktivierte Scopes: " << endl;
    dout.printActivatedScopes(cout);
  }
  #endif
  
  if (dout.isActivated("main")){
    cout << "main ist active" << endl;
  }else{
    cout << "main ist nicht active" << endl;
  }
  
  dout.startScope("foo");
  if (dout.isActivated("foo")){
    cout << "foo ist active" << endl;
  }else{
    cout << "foo ist nicht active" << endl;
  }
  
  
  dout.printStack(cout);
  
  dout << "hello!" << endl;
  dout << "hallo!" << endl << "foo";
  dout << std::dec << 0x13 << endl;
  
  //dout::endScope("main");
  return 0;
  
  
}
