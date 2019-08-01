void run_myPOT(){

  // gSystem->Load("FluggNtuple/FluxNtuple_C.so");
  TString t = ".L myPOT.C++";
  gROOT->ProcessLine(t);
  t = "myPOT().Loop()";
  gROOT->ProcessLine(t);
  gSystem->Exit(0);

}
