//
// Created by GreysTone on 26/03/16.
//

#ifndef PROJECT_GTARGUMENT_H
#define PROJECT_GTARGUMENT_H

#include <iostream>
#include <stdlib.h>
#include <string.h>

class gtArgument {
public:
  gtArgument ();
  virtual ~gtArgument();

  bool handleArguments(int argc, char **argv);

  bool userScene;
  bool shadowActivated;

  int maxStep;
};


#endif //PROJECT_GTARGUMENT_H
