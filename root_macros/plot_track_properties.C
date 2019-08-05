// Function to plot the resolution of various track variables
// for the mega retreat MCC9.1 validation.

#include "functions.h"

void plot_track_properties(const char * _file1, TString plot_config){


    
    enum TH1D_names{kTrackBiasStartX, kTrackBiasStartY, kTrackBiasStartZ,
                    kTrackBiasEndX,   kTrackBiasEndY,   kTrackBiasEndZ,
                    kTrackBiasLength, kTrackBiasTheta,
                    kTH1D_names_MAX};
   
    enum TH2D_names{ kTrackBiasStartXY,  kTrackBiasStartYZ,  
                     kTrackBiasEndXY,    kTrackBiasEndYZ,  
                     kTH2D_names_MAX};
    
    std::vector<TH1D*> TH1D_hist(kTH1D_names_MAX);
    std::vector<TH2D*> TH2D_hist(kTH2D_names_MAX);

    TH1D_hist.at(kTrackBiasStartX) = new TH1D("h_TrackBiasStartX", "TrackBiasStartX", 100, -400, 400);
    TH1D_hist.at(kTrackBiasStartY) = new TH1D("h_TrackBiasStartY", "TrackBiasStartY", 100, -400, 400);
    TH1D_hist.at(kTrackBiasStartZ) = new TH1D("h_TrackBiasStartZ", "TrackBiasStartZ", 100, -400, 400);
    TH1D_hist.at(kTrackBiasEndX)   = new TH1D("h_TrackBiasEndX",   "TrackBiasEndX",   100, -400, 400);
    TH1D_hist.at(kTrackBiasEndY)   = new TH1D("h_TrackBiasEndY",   "TrackBiasEndY",   100, -400, 400);
    TH1D_hist.at(kTrackBiasEndZ)   = new TH1D("h_TrackBiasEndZ",   "TrackBiasEndZ",   100, -400, 400);
    TH1D_hist.at(kTrackBiasLength) = new TH1D("h_TrackBiasLength", "TrackBiasLength", 100, -400, 400);
    TH1D_hist.at(kTrackBiasTheta)  = new TH1D("h_TrackBiasTheta",  "TrackBiasTheta",  100, -4,   4);
    
    TH2D_hist.at(kTrackBiasStartXY) = new TH2D("h_TrackBiasStartXY", "TrackBiasStartXY", 100, -400, 400, 100, -400, 400);
    TH2D_hist.at(kTrackBiasStartYZ) = new TH2D("h_TrackBiasStartYZ", "TrackBiasStartYZ", 100, -400, 400, 100, -400, 400);
    TH2D_hist.at(kTrackBiasEndXY)   = new TH2D("h_TrackBiasEndXY",   "TrackBiasEndXY",   100, -400, 400, 100, -400, 400);
    TH2D_hist.at(kTrackBiasEndYZ)   = new TH2D("h_TrackBiasEndYZ",   "TrackBiasEndYZ",   100, -400, 400, 100, -400, 400);

    // create plots folder if it does not exist
    system("if [ ! -d \"plots\" ]; then echo \"\nPlots folder does not exist... creating\"; mkdir plots; fi");

    gStyle->SetOptStat(0); // say no to stats box

    
    TFile *f_var_out = new TFile("plots/track_properties_out.root","UPDATE");

    //*************************** RUN PLOTTING FUNCTION PLOT********************
    bool PlotVar{false};
    // if bool true just run this function
    if (plot_config == "same") PlotVar = true; 
    else PlotVar = false; 

    if (PlotVar) {
        PlotVariatons(f_var_out); 
        return; 
    }
    //*************************** POT Scaling **********************************
    std::cout << "=================================================\n" << std::endl;
    
    // ADD POT Scaling Stuff here
   double POT_Scaling;
    // double POT_Scaling =  CV_POT / GetPOT(_file1);
    POT_Scaling =  1.0;
    // POT_Scaling =  4.10997e+18/4.50468e+18;
    std::cout << "POT Scaling:\t" << POT_Scaling << std::endl;
    std::cout << "=================================================\n" << std::endl;
    
    //*********** Open Variation File and get branches *************************
    // Check if the outfile opened successfully
    if ( f_var_out->IsOpen() ) std::cout << "Variation File opened successfully\n" << std::endl;
    
    // Get Variation file
    TFile* inFile = new TFile(_file1);
    
    // Get TPC Obj Information from file
    bool booltree;
    TTree* myTTree;
    booltree = GetTree(inFile, myTTree, "CaliTTCreatorFinder/myTTree");
    if (booltree == false) gSystem->Exit(0);
    
    int RunNumber;
    std::vector<int> *Track_MCParticle_Origin{nullptr};
    
    std::vector<double> *Track_MCParticle_Vx{nullptr},   *Track_MCParticle_Vy{nullptr},   *Track_MCParticle_Vz{nullptr},
                        *Track_MCParticle_EndX{nullptr}, *Track_MCParticle_EndY{nullptr}, *Track_MCParticle_EndZ{nullptr},
                        *Track_MCParticle_Theta{nullptr},
                        *Track_StartX{nullptr},          *Track_StartY{nullptr},          *Track_StartZ{nullptr},
                        *Track_EndX{nullptr},            *Track_EndY{nullptr},            *Track_EndZ{nullptr},
                        *Track_Theta{nullptr};
    
    // Truth 
    myTTree->SetBranchAddress("Track_StartX", &Track_StartX);
    myTTree->SetBranchAddress("Track_StartY", &Track_StartY);
    myTTree->SetBranchAddress("Track_StartZ", &Track_StartZ);
    myTTree->SetBranchAddress("Track_EndX",   &Track_EndX);
    myTTree->SetBranchAddress("Track_EndY",   &Track_EndY);
    myTTree->SetBranchAddress("Track_EndZ",   &Track_EndZ);
    myTTree->SetBranchAddress("Track_Theta",  &Track_Theta);

    // Reco
    myTTree->SetBranchAddress("Track_MCParticle_Vx",     &Track_MCParticle_Vx);
    myTTree->SetBranchAddress("Track_MCParticle_Vy",     &Track_MCParticle_Vy);
    myTTree->SetBranchAddress("Track_MCParticle_Vz",     &Track_MCParticle_Vz);
    myTTree->SetBranchAddress("Track_MCParticle_EndX",   &Track_MCParticle_EndX);
    myTTree->SetBranchAddress("Track_MCParticle_EndY",   &Track_MCParticle_EndY);
    myTTree->SetBranchAddress("Track_MCParticle_EndZ",   &Track_MCParticle_EndZ);
    myTTree->SetBranchAddress("Track_MCParticle_Origin", &Track_MCParticle_Origin);
    myTTree->SetBranchAddress("Track_MCParticle_Theta",  &Track_MCParticle_Theta);


    // Num events in the tree
    const int tree_total_entries = myTTree->GetEntries();
    std::cout << "Total Events: " << tree_total_entries << std::endl;

    // ----------------------
    //      Event loop
    // ----------------------
    std::cout << "Starting Eventloop..." << std::endl;
    for (int event = 0; event < tree_total_entries; event++){

        if (event % 1000 == 0) std::cout << "On entry " << event/1000.0 << "k" << std::endl;
        
        myTTree->GetEntry(event);
        
        int n_tracks = Track_MCParticle_Vx->size();

        for (int k=0; k < n_tracks; k++){

            // Only look at neutrino tracks
            if (Track_MCParticle_Origin->at(k) == 1) {

                double TrueTrackLength = GetTrackLength(Track_StartX->at(k), Track_StartY->at(k), Track_StartZ->at(k),
                                                        Track_EndX->at(k),   Track_EndY->at(k),   Track_EndZ->at(k));

                double RecoTrackLength = GetTrackLength(Track_MCParticle_Vx->at(k),     Track_MCParticle_Vy->at(k),     Track_MCParticle_Vz->at(k),
                                                        Track_MCParticle_EndX->at(k),   Track_MCParticle_EndY->at(k),   Track_MCParticle_EndZ->at(k));

                TH1D_hist.at(kTrackBiasStartX)->Fill(Track_MCParticle_Vx->at(k)   - Track_StartX->at(k));
                TH1D_hist.at(kTrackBiasStartY)->Fill(Track_MCParticle_Vy->at(k)   - Track_StartY->at(k));
                TH1D_hist.at(kTrackBiasStartZ)->Fill(Track_MCParticle_Vz->at(k)   - Track_StartZ->at(k));
                TH1D_hist.at(kTrackBiasEndX)  ->Fill(Track_MCParticle_EndX->at(k) - Track_EndX->at(k));
                TH1D_hist.at(kTrackBiasEndY)  ->Fill(Track_MCParticle_EndY->at(k) - Track_EndY->at(k));
                TH1D_hist.at(kTrackBiasEndZ)  ->Fill(Track_MCParticle_EndZ->at(k) - Track_EndZ->at(k));
                TH1D_hist.at(kTrackBiasLength)->Fill(RecoTrackLength - TrueTrackLength);
                TH1D_hist.at(kTrackBiasTheta) ->Fill(Track_MCParticle_Theta  - Track_Theta);

                TH2D_hist.at(kTrackBiasStartXY)->Fill(Track_MCParticle_Vx->at(k) - Track_StartX->at(k), Track_MCParticle_Vy->at(k) - Track_StartY->at(k));
                TH2D_hist.at(kTrackBiasStartYZ)->Fill(Track_MCParticle_Vy->at(k) - Track_StartY->at(k), Track_MCParticle_Vz->at(k) - Track_StartZ->at(k));
                TH2D_hist.at(kTrackBiasEndXY)->Fill(Track_MCParticle_EndX->at(k) - Track_EndX->at(k), Track_MCParticle_EndY->at(k) - Track_EndY->at(k));
                TH2D_hist.at(kTrackBiasEndYZ)->Fill(Track_MCParticle_EndY->at(k) - Track_EndY->at(k), Track_MCParticle_EndZ->at(k) - Track_EndZ->at(k));
            }

        }


    } // END EVENT LOOP
    std::cout << "Finished Eventloop..." << std::endl;

    // std::cout << "--------------- MC Truth COUNTERS -----------------" << std::endl;
    // // std::cout << "MC Nue CC Counter      --- " << mc_nue_cc_counter << std::endl;
    // std::cout << "---------------------------------------------------" << std::endl;
    // std::cout << "--------------- Cut COUNTERS ----------------------" << std::endl;

    std::cout << "---------------------------------------------------" << std::endl;
    // ----------------------
    //    Save to a file
    // ----------------------
    TDirectory* savedir = gDirectory; //  Create the directory
    TDirectory* subdir;

    // Get the directory for the file	
    std::string dirname = _file1;
    
    // Get the variation name by stripping the input file name
    std::vector<std::string> variations = {"BNBCV", "BNBDLup"};

    bool bool_string{false};
    // Find the variation name in the string
    for (int i = 0; i < variations.size(); i++){
    
        size_t found = dirname.find(variations.at(i)); 
        
        // Got the variation match
        if (found != std::string::npos) {
            bool_string = true;
            dirname = variations.at(i);
        }   
    }

    if (!bool_string) {
        std::cout << "Error variation string not found in the file, you need to fix file name" << std::endl;
        gSystem->Exit(0);

    }

    std::cout << "dirname:\t" << dirname << std::endl;

    f_var_out->cd();

    // If directory does not exist then make it
    savedir = (TDirectory*)f_var_out->Get(dirname.c_str());
    if (savedir == NULL ) {
        savedir = gDirectory;
        std::cout << dirname << " directory does not exist, creating..." << std::endl;
        subdir = savedir->mkdir(dirname.c_str() ) ;
    }
    else {
        std::cout << dirname <<" directory exists, overwriting..." << std::endl;
        subdir = savedir;
    }
    
    subdir->cd();

    // -------------------------------------
    //   Draw Histograms and Write and close
    //   the TFile to new/updated directory
    // -------------------------------------
    for (int i=0; i < TH1D_hist.size(); i++){
        DrawTH1D(TH1D_hist.at(i), POT_Scaling);
        TH1D_hist.at(i)->Write("",TObject::kOverwrite);
    } 

    for (int i=0; i < TH2D_hist.size(); i++){
        DrawTH2D(TH2D_hist.at(i), POT_Scaling);
        TH2D_hist.at(i)->Write("",TObject::kOverwrite);
    }
    
    f_var_out->Close(); 

}