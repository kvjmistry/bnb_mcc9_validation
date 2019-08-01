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

    std::string draw_spec = "his, same";

    // ----------------------
	//    Axis Specifiers
	// ----------------------
	if (histname == "h_RunNumber"){
		hist->SetTitle(";Run Number;Entries");
		// hist->GetYaxis()->SetRangeUser(0,4500);
	}
    else if (histname == "h_TrackResStartX"){
        hist->SetTitle(";Track Start X Resolution;Entries");
		// hist->GetYaxis()->SetRangeUser(0,4500);
    }
    else if (histname == "h_TrackResStartY"){
        hist->SetTitle(";Track Start Y Resolution;Entries");
		// hist->GetYaxis()->SetRangeUser(0,4500);
    }
    else if (histname == "h_TrackResStartZ"){
        hist->SetTitle(";Track Start Z Resolution;Entries");
		// hist->GetYaxis()->SetRangeUser(0,4500);
    }
    else if (histname == "h_TrackResEndX"){
        hist->SetTitle(";Track End X Resolution;Entries");
		// hist->GetYaxis()->SetRangeUser(0,4500);
    }
    else if (histname == "h_TrackResEndY"){
        hist->SetTitle(";Track End Y Resolution;Entries");
		// hist->GetYaxis()->SetRangeUser(0,4500);
    }
    else if (histname == "h_TrackResEndZ"){
        hist->SetTitle(";Track End Z Resolution;Entries");
		// hist->GetYaxis()->SetRangeUser(0,4500);
    }
    else if (histname == "h_TrackResLength"){
        hist->SetTitle(";Track Length Resolution;Entries");
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

    
    std::vector<std::string> histnames = {"h_RunNumber",
                                          "h_TrackResStartX", "h_TrackResStartY", "h_TrackResStartZ",
                                          "h_TrackResEndX",   "h_TrackResEndY",   "h_TrackResEndZ",
                                          "h_TrackResLength"};

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
    
        size_t found = fileame.find(variations.at(i)); 
        
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
// -----------------------------------------------------------------------------