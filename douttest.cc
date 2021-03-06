#include <iostream>
#include <iomanip>

#include <stdint.h>

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

  dout.activateScope("iomanips");
  dout.startScope("iomanips");
  dout << 9 << " "<<std::setfill('x') << std::setw(6) << 9 << endl;
  dout.endScope("iomanips");
  
  { // type testing
    uint8_t testNumber=12;
    char testChar='B';
    int8_t testNumber2=-30;
    dout << "number12 " << testNumber << endl;
    dout << "CharB " << testChar << endl;
    dout << "number-30 " << testNumber2 << endl;
  }
  
  
  dout.endScope("main");
  return 0;
  
  
}
