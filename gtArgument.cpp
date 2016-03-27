//
// Created by GreysTone on 26/03/16.
//

#include "gtArgument.h"

gtArgument::gtArgument() {
  userScene = false;
  shadowActivated = false;

  maxStep = 0;
}
gtArgument::~gtArgument() { }

bool gtArgument::handleArguments(int argc, char **argv) {
  if (argc < 3) {
    std::cout << "Missing arguments ... use:\n";
    std::cout << "./raycast [-u | -d] step_max <options>\n";
    return false;
  }

  // user defined scene
  if (strcmp(argv[1], "-u") == 0)
    userScene = true;
//    set_up_user_scene();
//  } else { // default scene
//    set_up_default_scene();
//  }

  maxStep = atoi(argv[2]); // maximum level of recursions

  // Optional arguments
  for (int i = 3; i < argc; i++) {
    if (strcmp(argv[i], "-s") == 0) shadowActivated = true;
  }
  return true;
}