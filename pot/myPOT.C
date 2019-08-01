#define myPOT_cxx
#include "myPOT.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>

using namespace std;
// -----------------------------------------------------------------------------
void myPOT::Loop() {
     
    if (fChain == 0) { 
        std::cout << "Error no chain" << std::endl;
        return;
    }

    Long64_t nentries = fChain->GetEntriesFast();
    Long64_t nbytes = 0, nb = 0;

    double POTCount = 0.;

    for (Long64_t jentry=0; jentry<nentries;jentry++) {

        Long64_t ientry = LoadTree(jentry); if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);   nbytes += nb;

        POTCount += POT;

    } // End of the loop over the events

    std::cout << "Total POT = " <<  POTCount << std::endl;
    
} // End of the program
// -----------------------------------------------------------------------------