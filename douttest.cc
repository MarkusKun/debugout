#include <iostream>
#include "debugout.h"


int main(){
  using std::cout; using std::endl;
  
  dout.activateScope("dout_t::loadConfigFile");
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
  
  { // foo
    dout.startScope("foo");
    dout << "We are in foo" << endl;
    if (dout.isActivated("foo")){
      cout << "foo ist active" << endl;
    }else{
      cout << "foo ist nicht active" << endl;
    }
    { // sub2
      dout.startScope("sub2");
      dout.endScope();
    }
      
    
    dout.endScope();
  }
  
  dout.printStack(cout);
  
  dout.endScope("main");
  return 0;
  
  
}
