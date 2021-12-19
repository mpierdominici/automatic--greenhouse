#include "memEdgeDetector.h"

memEdgeDetector::memEdgeDetector()
{
  newData=0;
  currentData=0;
  isRising=0;
  isFalling=0;
}

bool memEdgeDetector::risingEdge(void){
  bool tempRes;
  tempRes=isRising;
  isRising=false;
  return tempRes;
}

bool memEdgeDetector::fallingEdge(void){
  bool tempRes;
  tempRes=isFalling;
  isFalling=false;
  return tempRes;
}

bool memEdgeDetector::getState(void){
  return currentData;
}

void memEdgeDetector::updateMme(bool newData){
  this->newData=newData;

  if(currentData==false && this->newData==true){
    isRising=true;
  }else if(currentData==true && this->newData==false){
    isFalling=true;    
  }
  currentData=this->newData;
}

  
