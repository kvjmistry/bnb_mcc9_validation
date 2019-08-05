// -----------------------------------------------------------------------------
// Function to increase to axes labels
void IncreaseLabelSize(TH1D* h){

    // h->GetXaxis()->SetRangeUser(0,3.5);
    h->GetXaxis()->SetLabelSize(0.05);
    h->GetXaxis()->SetTitleSize(0.05);
    h->GetYaxis()->SetLabelSize(0.05);
    h->GetYaxis()->SetTitleSize(0.05);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.12);
}
// -----------------------------------------------------------------------------
void IncreaseLabelSize(TH2D* h){

    // h->GetXaxis()->SetRangeUser(0,3.5);
    h->GetXaxis()->SetLabelSize(0.05);
    h->GetXaxis()->SetTitleSize(0.05);
    h->GetYaxis()->SetLabelSize(0.05);
    h->GetYaxis()->SetTitleSize(0.05);
    h->GetZaxis()->SetLabelSize(0.05);
    h->GetZaxis()->SetTitleSize(0.05);
    gPad->SetLeftMargin(0.15);
    gPad->SetRightMargin(0.2);
    gPad->SetBottomMargin(0.14);
    h->SetMarkerSize(1.8);
    h->GetYaxis()->SetTitleOffset(1);
    // gPad->SetGridx(); 
}
// -----------------------------------------------------------------------------
bool GetHist(TFile* f, TH1D* &h, TString string){
    h = (TH1D*) f->Get(string);
    if (h == NULL) {
        std::cout << "\nfailed to get:\t" << string <<
         "\tThis histogram might not exist in the file\n" << std::endl;
        return false;
    }
    else {
        return true;
    }
}
// -----------------------------------------------------------------------------
bool GetHist(TFile* f, TH2D* &h, TString string){
    h = (TH2D*) f->Get(string);
    if (h == NULL) {
        std::cout << "\nfailed to get:\t" << string <<
         "\tThis histogram might not exist in the file\n" << std::endl;
        return false;
    }
    else {
        return true;
    }
}
// -----------------------------------------------------------------------------
bool GetTree(TFile* f, TTree* &T, TString string){
    T = (TTree*) f->Get(string);
    if (T == NULL) {
        std::cout << "\nfailed to get:\t" << string <<
         "\tThis tree might not exist in the file\n" << std::endl;
        return false;
    }
    else {
        return true;
    }
}
// -----------------------------------------------------------------------------
bool GetFile(TFile* &f , TString string){
    f = TFile::Open(string);
    
    if (f == NULL) {
        std::cout << "failed to get:\t" << string << 
        "\tThis file might not exist in the file" << std::endl;
        return false;
    }
    else {
        return true;
    }
}
// -----------------------------------------------------------------------------
void DrawTH1D_SAME(TH1D* hist, std::string variation, TLegend* legend, std::string histname){
    
    IncreaseLabelSize(hist);
    legend->SetTextSize(.05);

    std::string draw_spec;

    // Find the ratio in the string
    bool bool_ratio{false};
    size_t found = histname.find("ratio"); 
    
    // Got a ratio plot instead
    if (found != std::string::npos) bool_ratio = true;

    draw_spec = "his, same";
    if (bool_ratio) draw_spec = "E1, same";

    // ----------------------
    //    Axis Specifiers
    // ----------------------
    if (histname == "h_TrackBiasStartX" || histname == "h_TrackBiasStartX_ratio"){
        hist->SetTitle(";Track Start X Bias [cm];Entries");
        if (bool_ratio) hist->SetTitle(";Track Start X Bias [cm];Ratio to CV");
        // hist->GetYaxis()->SetRangeUser(0,4500);
    }
    else if (histname == "h_TrackBiasStartY" || histname == "h_TrackBiasStartY_ratio"){
        hist->SetTitle(";Track Start Y Bias [cm];Entries");
        if (bool_ratio) hist->SetTitle(";Track Start Y Bias [cm];Ratio to CV");
        // hist->GetYaxis()->SetRangeUser(0,4500);
    }
    else if (histname == "h_TrackBiasStartZ" || histname == "h_TrackBiasStartZ_ratio"){
        hist->SetTitle(";Track Start Z Bias [cm];Entries");
        if (bool_ratio) hist->SetTitle(";Track Start Z Bias [cm];Ratio to CV");
        // hist->GetYaxis()->SetRangeUser(0,4500);
    }
    else if (histname == "h_TrackBiasEndX" || histname == "h_TrackBiasEndX_ratio"){
        hist->SetTitle(";Track End X Bias [cm];Entries");
        if (bool_ratio) hist->SetTitle(";Track End X Bias [cm];Ratio to CV");
        // hist->GetYaxis()->SetRangeUser(0,4500);
    }
    else if (histname == "h_TrackBiasEndY" || histname == "h_TrackBiasEndY_ratio"){
        hist->SetTitle(";Track End Y Bias [cm];Entries");
        if (bool_ratio) hist->SetTitle(";Track End Y Bias [cm];Ratio to CV");
        // hist->GetYaxis()->SetRangeUser(0,4500);
    }
    else if (histname == "h_TrackBiasEndZ" || histname == "h_TrackBiasEndZ_ratio"){
        hist->SetTitle(";Track End Z Bias [cm];Entries");
        if (bool_ratio) hist->SetTitle(";Track End Z Bias [cm];Ratio to CV");
        // hist->GetYaxis()->SetRangeUser(0,4500);
    }
    else if (histname == "h_TrackBiasLength" || histname == "h_TrackBiasLength_ratio"){
        hist->SetTitle(";Track Length Bias [cm];Entries");
        if (bool_ratio) hist->SetTitle(";Track Length Bias [cm];Ratio to CV");
        // hist->GetYaxis()->SetRangeUser(0,4500);
    }
    else if (histname == "h_TrackBiasTheta" || histname == "h_TrackBiasTheta_ratio"){
        hist->SetTitle(";Track Theta Bias [cm];Entries");
        if (bool_ratio) hist->SetTitle(";Track Theta Bias [cm];Ratio to CV");
        // hist->GetYaxis()->SetRangeUser(0,4500);
    }

    // ----------------------
    //    Draw Specifiers
    // ----------------------
    if (variation == "BNBCV"){
        hist->SetLineColor(kBlack);
        hist->SetLineWidth(2);
        hist->SetLineStyle(1);
        legend->AddEntry(hist, "CV", "l");
        hist->Draw(draw_spec.c_str());
    } 
    else if  (variation == "BNBwithDIC"){
        hist->SetLineColor(kMagenta+2);
        hist->SetLineWidth(2);
        hist->SetLineStyle(1);
        legend->AddEntry(hist, "DIC", "l");
        hist->Draw(draw_spec.c_str());
    }
    else if  (variation == "BNBEnhancedTPCVis"){ 
        hist->SetLineColor(30);
        hist->SetLineWidth(2);
        legend->AddEntry(hist, "Enhanced TPC Vis.", "l");
        hist->SetLineStyle(1);
        hist->Draw(draw_spec.c_str());
    }
    else if  (variation == "BNBaltDeadChannels"){ 
        hist->SetLineColor(38);
        hist->SetLineWidth(2);
        legend->AddEntry(hist, "Alt. Dead Chan.", "l");
        hist->SetLineStyle(1);
        hist->Draw(draw_spec.c_str());
    }
    else if  (variation == "BNBdeadSaturatedChannels"){
        hist->SetLineColor(28);
        hist->SetLineWidth(2);
        legend->AddEntry(hist, "Dead Sat. Chan.", "l");
        hist->SetLineStyle(1);
        hist->Draw(draw_spec.c_str());
        
    }
    else if  (variation == "BNBstretchResp"){
        hist->SetLineColor(36);
        hist->SetLineWidth(2);
        legend->AddEntry(hist, "Stretch Resp.", "l");
        hist->SetLineStyle(1);
        hist->Draw(draw_spec.c_str());
    }
    else if  (variation == "BNBsqueezeResp"){
        hist->SetLineColor(1001);
        hist->SetLineWidth(2);
        legend->AddEntry(hist, "Squeeze Resp.", "l");
        hist->SetLineStyle(1);
        hist->Draw(draw_spec.c_str());
    }
    else if  (variation == "BNBupPEnoise"){
        hist->SetLineColor(kBlue+1);
        hist->SetLineWidth(2);
        legend->AddEntry(hist, "PE Noise Up", "l");
        hist->SetLineStyle(1);
        hist->Draw(draw_spec.c_str());
    }
    else if  (variation == "BNBnoiseAmpDown"){
        hist->SetLineColor(42);
        hist->SetLineWidth(2);
        legend->AddEntry(hist, "Noise Amp. Down", "l");
        hist->SetLineStyle(1);
        hist->Draw(draw_spec.c_str());
    }
    else if  (variation == "BNBdownPEnoise"){
        hist->SetLineColor(50);
        hist->SetLineWidth(2);
        legend->AddEntry(hist, "PE Noise Down", "l");
        hist->SetLineStyle(1);
        hist->Draw(draw_spec.c_str());
    }
    else if  (variation == "BNBnoiseAmpUp"){
        hist->SetLineColor(kOrange+10);
        hist->SetLineWidth(2);
        legend->AddEntry(hist, "Noise Amp. Up", "l");
        hist->SetLineStyle(1);
        hist->Draw(draw_spec.c_str());
    }
    else if  (variation == "BNBDTdown"){
        hist->SetLineColor(kOrange+1);
        hist->SetLineWidth(2);
        legend->AddEntry(hist, "DT Down", "l");
        hist->SetLineStyle(1);
        hist->Draw(draw_spec.c_str());
    }
    else if  (variation == "BNBDTup"){
        hist->SetLineColor(kMagenta-10);
        hist->SetLineWidth(2);
        legend->AddEntry(hist, "DT Up", "l");
        hist->SetLineStyle(1);
        hist->Draw(draw_spec.c_str());
    }
    else if  (variation == "BNBDLup"){
        hist->SetLineColor(kMagenta);
        hist->SetLineWidth(2);
        legend->AddEntry(hist, "DL Up", "l");
        hist->SetLineStyle(1);
        hist->Draw(draw_spec.c_str());
    }
    else if  (variation == "BNBDLdown"){
        hist->SetLineColor(kTeal+6);
        hist->SetLineWidth(2);
        legend->AddEntry(hist, "DL Down", "l");
        hist->SetLineStyle(1);
        hist->Draw(draw_spec.c_str());
    }
    else if  (variation == "BNBdataSCE"){
        hist->SetLineColor(kAzure-9);
        hist->SetLineWidth(2);
        legend->AddEntry(hist, "SCE", "l");
        hist->SetLineStyle(1);
        hist->Draw(draw_spec.c_str());
    }
    else if  (variation == "BNBLArG4BugFix"){
        hist->SetLineColor(kSpring-7);
        hist->SetLineWidth(2);
        legend->AddEntry(hist, "LArG4BugFix", "l");
        hist->SetLineStyle(1);
        hist->Draw(draw_spec.c_str());
    }
    else if  (variation == "BNBBirksRecomb"){
        hist->SetLineColor(kRed+1);
        hist->SetLineWidth(2);
        legend->AddEntry(hist, "Birks Recomb.","l");
        hist->SetLineStyle(1);
        hist->Draw(draw_spec.c_str());
    }
    else {
        std::cout << "Error! Could not match varations:\t" << variation << std::endl;
        return;
    }
}
// -----------------------------------------------------------------------------
void DrawTH2D_SAME(TH2D* hist, std::string variation, std::string histname){
    
    IncreaseLabelSize(hist);

    std::string draw_spec;

    // Find the ratio in the string
    bool bool_ratio{false};
    size_t found = histname.find("ratio"); 
    
    // Got a ratio plot instead
    if (found != std::string::npos) bool_ratio = true;

    draw_spec = "colz";

    std::string variation_name = " ";

    // ----------------------
    //    Draw Specifiers
    // ----------------------
    if (variation == "BNBCV"){
        variation_name = "CV";
    } 
    else if  (variation == "BNBwithDIC"){
        variation_name = "DIC";
    }
    else if  (variation == "BNBEnhancedTPCVis"){ 
        variation_name = "Enhanced TPC Vis.";
    }
    else if  (variation == "BNBaltDeadChannels"){ 
        variation_name = "Alt. Dead Chan.";
    }
    else if  (variation == "BNBdeadSaturatedChannels"){
        variation_name = "Dead Sat. Chan.";
    }
    else if  (variation == "BNBstretchResp"){
        variation_name = "Stretch Resp.";
    }
    else if  (variation == "BNBsqueezeResp"){
        variation_name = "Squeeze Resp.";
    }
    else if  (variation == "BNBupPEnoise"){
        variation_name = "PE Noise Up";
    }
    else if  (variation == "BNBnoiseAmpDown"){
        variation_name = "Noise Amp. Down";
    }
    else if  (variation == "BNBdownPEnoise"){
        variation_name = "PE Noise Down";
    }
    else if  (variation == "BNBnoiseAmpUp"){
        variation_name = "Noise Amp. Up";
    }
    else if  (variation == "BNBDTdown"){
        variation_name = "DT Down";
    }
    else if  (variation == "BNBDTup"){
        variation_name = "DT Up";
    }
    else if  (variation == "BNBDLup"){
        variation_name = "DL Up";
    }
    else if  (variation == "BNBDLdown"){
        variation_name = "DL Down";
    }
    else if  (variation == "BNBdataSCE"){
        variation_name = "SCE";
    }
    else if  (variation == "BNBLArG4BugFix"){
        variation_name = "LArG4BugFix";
    }
    else if  (variation == "BNBBirksRecomb"){
        variation_name = "Birks Recomb.";
    }
    else {
        std::cout << "Error! Could not match varations:\t" << variation << std::endl;
        return;
    }

    // ----------------------
    //    Axis Specifiers
    // ----------------------
    if (histname == "h_TrackBiasStartXY" || histname == "h_TrackBiasStartXY_ratio"){
        hist->SetTitle(Form("%s;Track Bias Start X [cm]; Track Bias Start Y [cm]", variation_name.c_str()));
        if (bool_ratio) hist->SetTitle(Form("%s Ratio to CV;Track Bias Start X [cm]; Track Bias Start Y [cm]", variation_name.c_str()));
        // hist->GetYaxis()->SetRangeUser(0,4500);
    }
    else if (histname == "h_TrackBiasStartYZ" || histname == "h_TrackBiasStartYZ_ratio"){
        hist->SetTitle(Form("%s;Track Bias Start Y [cm];Track Bias Start Z [cm]", variation_name.c_str()));
        if (bool_ratio) hist->SetTitle(Form("%s Ratio to CV;Track Bias Start Y [cm]; Track Bias Start Z [cm]", variation_name.c_str()));
        // hist->GetYaxis()->SetRangeUser(0,4500);
    }
    else if (histname == "h_TrackBiasEndXY" || histname == "h_TrackBiasEndXY_ratio"){
        hist->SetTitle(Form("%s;Track Bias End X [cm]; Track Bias End Y [cm]", variation_name.c_str()));
        if (bool_ratio) hist->SetTitle(Form("%s Ratio to CV;Track Bias End X [cm]; Track Bias End Y [cm]", variation_name.c_str()));
        // hist->GetYaxis()->SetRangeUser(0,4500);
    }
    else if (histname == "h_TrackBiasEndYZ" || histname == "h_TrackBiasEndYZ_ratio"){
        hist->SetTitle(Form("%s;Track Bias End Y [cm]; Track Bias End Z [cm]", variation_name.c_str()));
        if (bool_ratio) hist->SetTitle(Form("%s Ratio to CV;Track Bias End Y [cm];Track Bias End Z [cm]", variation_name.c_str()));
        // hist->GetYaxis()->SetRangeUser(0,4500);
    }
    
    gPad->SetLogz();
    hist->Draw(draw_spec.c_str());

}
// -----------------------------------------------------------------------------
// Function that grabs the reweighted histogram names for plotting
std::vector<std::string> GrabDirs(TFile* f_var_out) {
    std::vector<std::string> variations;

    f_var_out->cd();
    
    TKey *key;
    TIter nextkey(gDirectory->GetListOfKeys());

    TString same_plots = "SAME_Plots";

    std::cout << "\n=================================================" << std::endl;	
    std::cout << "Getting variation modes:" << std::endl;	
      while ( ( key =  (TKey*)nextkey()) ) { // Extra brackets to omit a warning 
        if (key->IsFolder()) {
            std::cout << key->GetName() << std::endl; // Print the variations
            if (key->GetName() == same_plots ) continue; // Skip this
            variations.push_back(key->GetName());
        }
    }
    std::cout << "=================================================\n" << std::endl;

    return (variations);
}
// -----------------------------------------------------------------------------
void PlotVariatons(TFile* f_var_out){
    f_var_out->cd();
    
    // Grab the variation folders in the file
    std::vector<std::string> variations = GrabDirs(f_var_out); 

    // ************************** 1D Histograms ********************************
    std::vector<std::string> histnames = {"h_TrackBiasStartX", "h_TrackBiasStartY", "h_TrackBiasStartZ",
                                          "h_TrackBiasEndX",   "h_TrackBiasEndY",   "h_TrackBiasEndZ",
                                          "h_TrackBiasLength", "h_TrackBiasTheta"};

    // Loop over the histograms
    for (int j=0; j < histnames.size(); j++){
        TH1D* hist;
        
        // Canvas + Legend
        TCanvas* c = new TCanvas();
        TLegend* legend;
        if (histnames[j] == "h_ldg_shwr_CTheta") legend = new TLegend(0.15, 0.55, 0.37, 0.85); // Reposition
        else if (histnames[j] == "h_tpc_obj_vtx_x" || histnames[j] == "h_tpc_obj_vtx_y" || histnames[j] == "h_tpc_obj_vtx_z" )
                legend = new TLegend(0.15, 0.15, 0.37, 0.45); // Reposition
        else if (histnames[j] == "h_largest_flash_z" ) legend = new TLegend(0.35, 0.59, 0.57, 0.89);
        else legend = new TLegend(0.72, 0.59, 0.94, 0.89);

        legend->SetBorderSize(0);
        legend->SetFillStyle(0);

        // Loop over variation directories
        for (int i=0; i < variations.size(); i++){
            char name[500];
            snprintf(name, 500, "%s/%s", variations[i].c_str(),histnames[j].c_str() );

            hist = (TH1D*)f_var_out->Get(name);
            if (hist == NULL ) std::cout << "ERROR: Can't get Histogram!" << std::endl;

            DrawTH1D_SAME(hist, variations[i], legend, histnames[j]);

        }

        // Print the Canvas
        char Canvas_name[500];
        snprintf(Canvas_name, 500, "plots/%s.pdf",histnames[j].c_str() ); 
        legend->Draw();
        c->Print(Canvas_name);

    }
    // ************************ 1D Histograms ratio ****************************
    // Make a Ratio Plot for leading shower phi and theta
    // Canvas + Legend
    std::string histname_ratio;
    
    for (int j=0; j < histnames.size(); j++){
        TH1D* hist;
        TH1D* hist_CV;
        
        // Canvas + Legend
        TCanvas* c = new TCanvas();
        TLegend* legend = new TLegend(0.72, 0.59, 0.94, 0.89);

        legend->SetBorderSize(0);
        legend->SetFillStyle(0);

        // Loop over variation directories
        for (int i=0; i < variations.size(); i++){
            char name[500];
            snprintf(name, 500, "%s/%s", variations[i].c_str(), histnames[j].c_str() );
            hist = (TH1D*)f_var_out->Get(name);
            if (hist == NULL ) std::cout << "ERROR: Can't get Histogram!" << std::endl;

            char name_CV[500];
            snprintf(name_CV, 500, "BNBCV/%s", histnames[j].c_str() );
            hist_CV = (TH1D*)f_var_out->Get(name_CV);
            if (hist_CV == NULL ) std::cout << "ERROR: Can't get CV Histogram!" << std::endl;

            histname_ratio = histnames[j] + "_ratio";
            
            // hist->Sumw2();
            // hist_CV->Sumw2();

            TH1D* hist_divide = (TH1D*) hist->Clone("hist_divide");
            
            hist_divide->Divide(hist_CV);

            if (variations[i] == "BNBCV"){
                for (unsigned int k=1; k < hist_divide->GetNbinsX()+1; 	k++ ){
                    hist_divide->SetBinContent(k, 1);
                    hist_divide->SetBinError(k, 0);
                }
            }

            DrawTH1D_SAME(hist_divide, variations[i], legend, histname_ratio);

        }

        // Print the Canvas
        char Canvas_name[500];
        snprintf(Canvas_name, 500, "plots/%s.pdf",histname_ratio.c_str() ); 
        legend->Draw();
        c->Print(Canvas_name);
    }

    // ************************** 2D Histograms ********************************
    std::vector<std::string> histnames_2D = {"h_TrackBiasStartXY", "h_TrackBiasStartYZ",
                                             "h_TrackBiasEndXY", "h_TrackBiasEndYZ"};

    // Loop over the histograms
    for (int j=0; j < histnames_2D.size(); j++){
        TH2D* hist;
        
        // Loop over variation directories
        for (int i=0; i < variations.size(); i++){
            // Canvas + Legend
            TCanvas* c = new TCanvas();

            char name[500];
            snprintf(name, 500, "%s/%s", variations[i].c_str(),histnames_2D[j].c_str() );

            hist = (TH2D*)f_var_out->Get(name);
            if (hist == NULL ) std::cout << "ERROR: Can't get Histogram!" << std::endl;

            DrawTH2D_SAME(hist, variations[i], histnames_2D[j]);

            // Print the Canvas
            char Canvas_name[1000];
            snprintf(Canvas_name, 1000, "plots/%s_%s.pdf",histnames_2D[j].c_str(), variations[i].c_str() ); 
            c->Print(Canvas_name);
            c->Close();

        }

    }
    // ************************ 2D Histograms ratio ****************************
    for (int j=0; j < histnames_2D.size(); j++){
        TH2D* hist;
        TH2D* hist_CV;
 
        

        // Loop over variation directories
        for (int i=0; i < variations.size(); i++){
            // Canvas + Legend
            TCanvas* c = new TCanvas();

            char name[500];
            snprintf(name, 500, "%s/%s", variations[i].c_str(), histnames_2D[j].c_str() );
            hist = (TH2D*)f_var_out->Get(name);
            if (hist == NULL ) std::cout << "ERROR: Can't get Histogram!" << std::endl;

            char name_CV[500];
            snprintf(name_CV, 500, "BNBCV/%s", histnames_2D[j].c_str() );
            hist_CV = (TH2D*)f_var_out->Get(name_CV);
            if (hist_CV == NULL ) std::cout << "ERROR: Can't get CV Histogram!" << std::endl;

            histname_ratio = histnames_2D[j] + "_ratio";

            TH2D* hist_divide = (TH2D*) hist->Clone("hist_divide");

            hist_divide->Divide(hist_CV);

            DrawTH2D_SAME(hist_divide, variations[i], histname_ratio);

            // Print the Canvas
            char Canvas_name[500];
            snprintf(Canvas_name, 500, "plots/%s_%s.pdf",histname_ratio.c_str(), variations[i].c_str() ); 
            c->Print(Canvas_name);
            c->Close();

        }

    }
 
    // Close the file
    f_var_out->Close();

}
// -----------------------------------------------------------------------------
void DrawTH1D(TH1D* h, double POT_Scaling){
    TCanvas* c = new TCanvas();
    c->cd();

    h->SetLineColor(kMagenta+3);
    h->SetLineWidth(2);
    h->SetLineStyle(1);

    h->Scale(POT_Scaling);
    h->Draw("his");
    c->Close();

}
// -----------------------------------------------------------------------------
void DrawTH2D(TH2D* h, double POT_Scaling){
    TCanvas* c = new TCanvas();
    c->cd();

    h->Scale(POT_Scaling);
    h->Draw("colz");
    c->Close();

}
// -----------------------------------------------------------------------------
// Get the length of a track
double GetTrackLength(double startx, double starty, double startz,
                      double endx,   double endy,   double endz){

        double length = (endx - startx)*(endx - startx)
                      + (endy - starty)*(endy - starty)
                      + (endz - startz)*(endz - startz);

        return length;

}
// -----------------------------------------------------------------------------
double GetPOT(const char * _file1){
    double POT{0};
    std::string line;

    std::string filename;
    std::string temp_filename = _file1; // cast to string
    filename = "File: " + temp_filename;

    std::cout << filename << std::endl;

    // Get the variation name by stripping the input file name
    std::vector<std::string> variations = {"BNBCV", "BNBDLup"};

    bool bool_string{false};
    // Find the variation name in the string
    for (int i = 0; i < variations.size(); i++){
    
        size_t found = filename.find(variations.at(i)); 
        
        // Got the variation match
        if (found != std::string::npos) {
            bool_string = true;
            filename = variations.at(i);
        }   
    }

    if (!bool_string) {
        std::cout << "Unable to open file" << std::endl; 
        return POT;
    }

    std::ifstream myfile ("POT_List.txt");
    int i_POT{0};
    
    if (myfile.is_open()) {

        // Loop over lines in file
        while ( getline (myfile,line) ) {

            if (i_POT == 1){
                line.erase(0, 21);
                POT = std::stod(line); // Convert string to double
                std::cout << "POT in File:\t"<< POT << std::endl;
                return POT;
            }

            // Look for var name in the file
            size_t found = line.find(filename); 
        
            // Got the variation match
            if (found != std::string::npos) {
                std::cout << "Found match for POT file"<< std::endl;
                i_POT++;
            }   
            
        }

        myfile.close();
    }
    else std::cout << "Unable to open file" << std::endl; 
    
    std::cout << "Could not find a match for POT file"<< std::endl;
    return POT;
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------