#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllServer.h>

#include "FROG.h"

R__ADD_INCLUDE_PATH($MY_INSTALL/include)
#include "myanadst/MyAnaDstBasic.h"

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libMyAnaDst.so)

int Run(
		const int nEvents = 0,
		const char * inputFile = "G4sPHENIX.root"
		)
{
  //---------------
  // Load libraries
  //---------------

	gSystem->Load("libfun4all.so");
  gSystem->Load("libg4dst.so");

  // Enabling file finding in dCache
  FROG *fr = new FROG();

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);
  // just if we set some flags somewhere in this macro
  //recoConsts *rc = recoConsts::instance();
  // By default every random number generator uses
  // PHRandomSeed() which reads /dev/urandom to get its seed
  // if the RANDOMSEED flag is set its value is taken as seed
  // You ca neither set this to a random value using PHRandomSeed()
  // which will make all seeds identical (not sure what the point of
  // this would be:
  //  rc->set_IntFlag("RANDOMSEED",PHRandomSeed());
  // or set it to a fixed value so you can debug your code
  // rc->set_IntFlag("RANDOMSEED", 12345);


	MyAnaDstBasic *ana1 = new MyAnaDstBasic();
	se->registerSubsystem(ana1);

	Fun4AllInputManager *hitsin = new Fun4AllDstInputManager("DSTin");
	if (strstr(inputFile,"root")){
		hitsin->fileopen(inputFile);
	}else{
		hitsin->AddListFile(inputFile);
	}
	se->registerInputManager(hitsin);

	gSystem->ListLibraries();
  se->run(nEvents);

  //-----
  // Exit
  //-----

  se->End();
  std::cout << "All done" << std::endl;
  delete se;
  gSystem->Exit(0);
	return 0;
}
