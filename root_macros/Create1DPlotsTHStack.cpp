#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <THStack.h>
#include <TString.h>
#include <TStyle.h>
#include <TLegend.h>
#include <iostream>
#include <vector>

using namespace std;

#include "./SecondaryCode/CenterAxisTitle.cpp"
#include "./SecondaryCode/SetOffsetAndSize.cpp"
#include "./SecondaryCode/ToString.cpp"

void Create1DPlotsTHStack()
{

    TH1D::SetDefaultSumw2();
    vector<TString> PlotNames;

    TString PathToFiles = "myMCC9FirstValidation/May/";
    //	TString PathToFiles = "myMCC9FirstValidation/OlderSamples/CSU_Workshop_Samples/";

    TString StorePath = "May";

    TString WhichRun = "Run1";
    //	TString WhichRun = "Run3";

    //	PlotNames.push_back("dEdxPlot"); PlotNames.push_back("dQdxPlot");
    //	PlotNames.push_back("NumberTracksPlot"); PlotNames.push_back("TrackLengthPlot");
    //	PlotNames.push_back("TrackPhiPlot"); PlotNames.push_back("TrackCosThetaPlot"); PlotNames.push_back("TrackKineticEnergyPlot");
    //	PlotNames.push_back("TrackStartXPlot"); PlotNames.push_back("TrackStartYPlot"); PlotNames.push_back("TrackStartZPlot");
    //	PlotNames.push_back("TrackEndXPlot"); PlotNames.push_back("TrackEndYPlot"); PlotNames.push_back("TrackEndZPlot");

    PlotNames.push_back("HitPeakTimePlane0Plot");
    PlotNames.push_back("HitPeakTimePlane1Plot");
    PlotNames.push_back("HitPeakTimePlane2Plot");
    //	PlotNames.push_back("NumberHitsPlot"); PlotNames.push_back("HitPeakTimePlot"); PlotNames.push_back("HitMultiplicityPlot"); PlotNames.push_back("HitChargePlot");
    //	PlotNames.push_back("HitChargePlane0Plot"); PlotNames.push_back("HitChargePlane1Plot"); PlotNames.push_back("HitChargePlane2Plot");
    //	PlotNames.push_back("HitChannelPlot"); PlotNames.push_back("HitChannelPlane0Plot"); PlotNames.push_back("HitChannelPlane1Plot"); PlotNames.push_back("HitChannelPlane2Plot");
    //	PlotNames.push_back("HitWirePlot");
    //	PlotNames.push_back("HitAmplitudePlot");
    //	PlotNames.push_back("HitAmplitudePlane0Plot"); PlotNames.push_back("HitAmplitudePlane1Plot"); PlotNames.push_back("HitAmplitudePlane2Plot");

    //	PlotNames.push_back("Track_NumberAssHitsPlot"); PlotNames.push_back("Track_AssHitPeakTimePlot"); PlotNames.push_back("Track_AssHitMultiplicityPlot");
    //	PlotNames.push_back("Track_AssHitIntegralPlot"); PlotNames.push_back("Track_AssHitChannelPlot"); PlotNames.push_back("Track_AssHitPeakAmplitudePlot");

    //	PlotNames.push_back("NumberShowersPlot"); PlotNames.push_back("ShowerStartXPlot"); PlotNames.push_back("ShowerStartYPlot"); PlotNames.push_back("ShowerStartZPlot");
    //	PlotNames.push_back("ShowerOpenAnglePlot"); PlotNames.push_back("ShowerLengthPlot");

    //	PlotNames.push_back("NumberBeamOpHitsPlot"); PlotNames.push_back("BeamOpHitsOpChannelPlot");
    //	PlotNames.push_back("BeamOpHitsPEPlot"); PlotNames.push_back("BeamOpHitsPeakTimePlot");
    //	PlotNames.push_back("BeamOpHitsPeakAmplitudePlot");PlotNames.push_back("BeamOpHitsAreaPlot");
    //	PlotNames.push_back("BeamFlashesPEPerPMT28Plot");
    //	PlotNames.push_back("BeamFlashesPEPerPMT29Plot");

    //	PlotNames.push_back("NumberBeamFlashesPlot");
    //	PlotNames.push_back("BeamFlashesYWidthPlot"); PlotNames.push_back("BeamFlashesZWidthPlot");
    //	PlotNames.push_back("BeamFlashesYCenterPlot");
    //	PlotNames.push_back("BeamFlashesZCenterPlot");
    //	PlotNames.push_back("BeamFlashesTimePlot");
    //	PlotNames.push_back("BeamFlashesTotalPEPlot");

    //	PlotNames.push_back("NumberCosmicOpHitsPlot");
    //	PlotNames.push_back("CosmicOpHitsOpChannelPlot"); PlotNames.push_back("CosmicOpHitsPEPlot"); PlotNames.push_back("CosmicOpHitsPeakTimePlot");
    //	PlotNames.push_back("CosmicOpHitsPeakAmplitudePlot"); PlotNames.push_back("CosmicOpHitsAreaPlot");

    //	PlotNames.push_back("NumberCosmicFlashesPlot");PlotNames.push_back("CosmicFlashesYWidthPlot"); PlotNames.push_back("CosmicFlashesZWidthPlot");
    //	PlotNames.push_back("CosmicFlashesYCenterPlot");PlotNames.push_back("CosmicFlashesZCenterPlot");
    //	PlotNames.push_back("CosmicFlashesTimePlot");
    //	PlotNames.push_back("CosmicFlashesTotalPEPlot");
    //	PlotNames.push_back("CosmicFlashesPEPerPMTPlot");
    //	PlotNames.push_back("CosmicFlashesPEPerPMT28Plot");
    //	PlotNames.push_back("CosmicFlashesPEPerPMT29Plot");

    //	PlotNames.push_back("NumberVerticesPlot"); PlotNames.push_back("VertexXPlot"); PlotNames.push_back("VertexYPlot"); PlotNames.push_back("VertexZPlot");

    const int N1DPlots = PlotNames.size();
    cout << "Number of 1D Plots = " << N1DPlots << endl;

    vector<TCanvas *> PlotCanvas;
    PlotCanvas.clear();
    vector<vector<TH1D *>> Plots;
    Plots.clear();
    vector<THStack *> THStacks;
    THStacks.clear();
    vector<vector<TH1D *>> hratio;
    hratio.clear();
    vector<int> Colors;
    Colors.clear();
    gStyle->SetPalette(55);
    const Int_t NCont = 999;
    gStyle->SetNumberContours(NCont);
    gStyle->SetTitleSize(0.07, "t");
    SetOffsetAndSize();
    vector<TLegend *> leg;
    leg.clear();

    vector<TString> LabelsOfSamples;
    vector<TString> NameOfSamples;

    // v12
    //	double DataPOT = 4.552e+19;
    //	double E1DCNT_wcut = 10096723.;
    //	double EXT = 64275293.;
    //	double OverlayPOT = 6.20767e+20;

    //	// v13
    //	double DataPOT = 1.567e+19;
    //	double E1DCNT_wcut = 3393920.0;
    //	double EXT = 4155539.;
    //	double OverlayPOT = 5.57158e+19;

    // v13a
    double DataPOT = 4.329e+19;
    double E1DCNT_wcut = 9600332.;
    double EXT = 23256319.;
    double OverlayPOT = 3.41341e+20;

    vector<double> POTOfCurrentSample;
    //	POTOfCurrentSample.push_back(ReferencePOT); POTOfCurrentSample.push_back(3.982e+19);  POTOfCurrentSample.push_back(1.599e+20);
    //	POTOfCurrentSample.push_back(ReferencePOT); POTOfCurrentSample.push_back(3.982e+19);  POTOfCurrentSample.push_back(ReferencePOT * E1DCNT_wcut / EXT);

    POTOfCurrentSample.push_back(1.);
    POTOfCurrentSample.push_back(DataPOT / OverlayPOT);
    POTOfCurrentSample.push_back(E1DCNT_wcut / EXT);
    //	POTOfCurrentSample.push_back(ReferencePOT); POTOfCurrentSample.push_back(4.03e+19);  POTOfCurrentSample.push_back(1.655e+20);

    if (WhichRun == "Run1")
    {
        //		NameOfSamples.push_back("Run1Data9_Run1"); NameOfSamples.push_back("Combined_Overlay9_Run1"); NameOfSamples.push_back("ExtBNB9_Run1");
        NameOfSamples.push_back("Run1Data9_Run1");
        NameOfSamples.push_back("Overlay9_Run1");
        NameOfSamples.push_back("ExtBNB9_Run1");
        LabelsOfSamples.push_back("Data Run-1");
        LabelsOfSamples.push_back("Overlay Run-1");
        LabelsOfSamples.push_back("ExtBNB Run-1");
        Colors.push_back(kBlack);
        Colors.push_back(kRed);
        Colors.push_back(kBlue);
    }

    if (WhichRun == "Run3")
    {
        //		NameOfSamples.push_back("Run1Data9_Run1"); NameOfSamples.push_back("Overlay9_Run3"); NameOfSamples.push_back("ExtBNB9_Run3");
        //		LabelsOfSamples.push_back("Data Run-1"); LabelsOfSamples.push_back("Overlay Run-3"); LabelsOfSamples.push_back("ExtBNB Run-3");

        NameOfSamples.push_back("Run1Data9_Run3");
        NameOfSamples.push_back("Overlay9_Run3");
        NameOfSamples.push_back("ExtBNB9_Run3");
        LabelsOfSamples.push_back("Data Run-3");
        LabelsOfSamples.push_back("Overlay Run-3");
        LabelsOfSamples.push_back("ExtBNB Run-3");

        Colors.push_back(kBlack);
        Colors.push_back(kRed);
        Colors.push_back(kBlue);
    }

    const int NSamples = NameOfSamples.size();
    vector<TFile *> FileSample;
    FileSample.clear();

    for (int WhichSample = 0; WhichSample < NSamples; WhichSample++)
    {

        FileSample.push_back(TFile::Open(PathToFiles + NameOfSamples[WhichSample] + "_Analysis.root"));

        vector<TH1D *> CurrentPlots;
        CurrentPlots.clear();
        vector<TH1D *> Currenthratio;
        Currenthratio.clear();

        double ScalingFactor = POTOfCurrentSample[WhichSample];

        for (int WhichPlot = 0; WhichPlot < N1DPlots; WhichPlot++)
        {

            TH1D *hist = (TH1D *)(FileSample[WhichSample]->Get(PlotNames[WhichPlot]));

            //			double ScalingFactor = 1. / hist->GetEntries() ; // default
            //			double ScalingFactor = 1. / hist->Integral() ;

            hist->Scale(ScalingFactor);
            CenterAxisTitle(hist);
            hist->SetLineColor(Colors[WhichSample]);
            if (NameOfSamples[WhichSample] == "Run1Data9_Run1" || NameOfSamples[WhichSample] == "Run1Data9_Run3")
            {
                hist->SetMarkerStyle(20);
            }

            if (PlotNames[WhichPlot] == "HitPeakTimePlane0Plot" || PlotNames[WhichPlot] == "HitPeakTimePlane1Plot" || PlotNames[WhichPlot] == "HitPeakTimePlane2Plot")
            {

                for (int i = 0; i < 1; i++)
                {
                    hist->Rebin();
                }
            }

            CurrentPlots.push_back(hist);
            Currenthratio.push_back((TH1D *)hist->Clone());
        }

        Plots.push_back(CurrentPlots);
        hratio.push_back(Currenthratio);
    }

    // Loop over the plots

    for (int WhichPlot = 0; WhichPlot < N1DPlots; WhichPlot++)
    {

        PlotCanvas.push_back(new TCanvas(PlotNames[WhichPlot], PlotNames[WhichPlot], 500, 500));
        PlotCanvas[WhichPlot]->cd();

        THStacks.push_back(new THStack(PlotNames[WhichPlot], ""));

        TPad *topPad = new TPad("topPad", "", 0.005, 0.92, 0.995, 0.995);
        TPad *midPad = new TPad("midPad", "", 0.005, 0.3, 0.995, 0.92);
        TPad *botPad = new TPad("botPad", "", 0.005, 0.005, 0.995, 0.3);
        topPad->SetTopMargin(0.3);
        topPad->SetBottomMargin(0.0);
        midPad->SetBottomMargin(0.0);
        midPad->SetTopMargin(0.0);
        botPad->SetTopMargin(0.);
        botPad->SetBottomMargin(0.25);
        botPad->SetGridy();
        topPad->Draw();
        midPad->Draw();
        botPad->Draw();

        leg.push_back(new TLegend(0.1, 0.005, 0.9, 0.995));
        leg[WhichPlot]->SetBorderSize(0);
        leg[WhichPlot]->SetNColumns(2);

        double max = -99.;

        // Loop over the samples

        for (int WhichSample = 0; WhichSample < NSamples; WhichSample++)
        {

            midPad->cd();
            Plots[WhichSample][WhichPlot]->SetTitle("");
            Plots[WhichSample][WhichPlot]->SetLineWidth(4);

            Plots[WhichSample][WhichPlot]->GetXaxis()->SetTitleFont(12);
            Plots[WhichSample][WhichPlot]->GetXaxis()->SetLabelFont(12);
            Plots[WhichSample][WhichPlot]->GetXaxis()->SetNdivisions(5);
            Plots[WhichSample][WhichPlot]->GetXaxis()->SetLabelSize(0);

            Plots[WhichSample][WhichPlot]->GetYaxis()->SetTitleFont(12);
            Plots[WhichSample][WhichPlot]->GetYaxis()->SetLabelFont(12);
            Plots[WhichSample][WhichPlot]->GetYaxis()->SetNdivisions(5);
            //			Plots[WhichSample][WhichPlot]->GetYaxis()->SetLabelSize(0);
            Plots[WhichSample][WhichPlot]->GetYaxis()->SetTickSize(0);

            if (PlotNames[WhichPlot] == "HitChannelPlane0Plot")
            {
                Plots[WhichSample][WhichPlot]->GetXaxis()->SetRangeUser(0., 2700.);
            }
            if (PlotNames[WhichPlot] == "HitChannelPlane1Plot")
            {
                Plots[WhichSample][WhichPlot]->GetXaxis()->SetRangeUser(2000., 5000.);
            }
            if (PlotNames[WhichPlot] == "HitChannelPlane2Plot")
            {
                Plots[WhichSample][WhichPlot]->GetXaxis()->SetRangeUser(4500., 9000.);
            }

            double localmax = Plots[WhichSample][WhichPlot]->GetMaximum();
            if (localmax > max)
            {
                max = localmax;
            }
            Plots[0][WhichPlot]->GetYaxis()->SetRangeUser(0., 1.1 * max);

            if (NameOfSamples[WhichSample] == "Run1Data9_Run1" || NameOfSamples[WhichSample] == "Run1Data9_Run3")
            {
                Plots[WhichSample][WhichPlot]->Draw("e1 same");
            }
            else
            {
                Plots[WhichSample][WhichPlot]->SetLineColor(Colors[WhichSample]);
                Plots[WhichSample][WhichPlot]->SetFillColor(Colors[WhichSample]);
                THStacks[WhichPlot]->Add(Plots[WhichSample][WhichPlot], "hist");
                if (WhichSample == 2)
                {
                    THStacks[WhichPlot]->Draw("same");
                }
            }

            if (NameOfSamples[WhichSample] == "Run1Data9_Run1" || NameOfSamples[WhichSample] == "Run1Data9_Run3")
            {
                leg[WhichPlot]->AddEntry(Plots[WhichSample][WhichPlot], LabelsOfSamples[WhichSample], "lep");
            }
            else
                leg[WhichPlot]->AddEntry(Plots[WhichSample][WhichPlot], LabelsOfSamples[WhichSample], "f");

            if (WhichSample == 2)
            {
                hratio[1][WhichPlot]->Add(Plots[2][WhichPlot]);
            }

        } // End of the loop over the samples

        Plots[0][WhichPlot]->Draw("e1 same");

        hratio[0][WhichPlot]->Divide(hratio[1][WhichPlot]);

        if (PlotNames[WhichPlot] == "HitChannelPlane0Plot")
        {
            hratio[0][WhichPlot]->GetXaxis()->SetRangeUser(0., 2700.);
        }
        if (PlotNames[WhichPlot] == "HitChannelPlane1Plot")
        {
            hratio[0][WhichPlot]->GetXaxis()->SetRangeUser(2000., 5000.);
        }
        if (PlotNames[WhichPlot] == "HitChannelPlane2Plot")
        {
            hratio[0][WhichPlot]->GetXaxis()->SetRangeUser(4500., 9000.);
        }

        hratio[0][WhichPlot]->GetXaxis()->SetTitleFont(12);
        hratio[0][WhichPlot]->GetXaxis()->SetLabelFont(12);
        hratio[0][WhichPlot]->GetYaxis()->SetTitle("#frac{Data}{MC + Cosmics}");
        hratio[0][WhichPlot]->GetXaxis()->SetTitle(Plots[0][WhichPlot]->GetXaxis()->GetTitle());
        hratio[0][WhichPlot]->GetXaxis()->SetTitleSize(0.16);
        hratio[0][WhichPlot]->GetXaxis()->SetLabelSize(0.12);
        hratio[0][WhichPlot]->GetXaxis()->SetTitleOffset(0.65);
        hratio[0][WhichPlot]->GetXaxis()->SetNdivisions(5);

        hratio[0][WhichPlot]->GetYaxis()->SetTitleFont(12);
        hratio[0][WhichPlot]->GetYaxis()->SetTitleSize(0.12);
        hratio[0][WhichPlot]->GetYaxis()->SetLabelFont(12);
        hratio[0][WhichPlot]->GetYaxis()->SetRangeUser(0.9, 1.2);
        //		hratio[0][WhichPlot]->GetYaxis()->SetRangeUser(0.8*hratio[0][WhichPlot]->GetMinimum(),1.2*hratio[0][WhichPlot]->GetMaximum());
        hratio[0][WhichPlot]->GetYaxis()->SetNdivisions(4);
        hratio[0][WhichPlot]->GetYaxis()->SetTitleOffset(0.35);
        hratio[0][WhichPlot]->GetYaxis()->SetTitleSize(0.12);
        hratio[0][WhichPlot]->GetYaxis()->SetLabelSize(0.11);

        botPad->cd();
        hratio[0][WhichPlot]->Draw("e1 hist same");

        topPad->cd();
        leg[WhichPlot]->SetTextSize(0.5);
        //leg[WhichPlot]->SetTextSize(0.7);
        leg[WhichPlot]->SetTextFont(12);
        leg[WhichPlot]->Draw();

        //		PlotCanvas[WhichPlot]->SaveAs("./myPlots/pdf_"+StorePath+"/1D/"+PlotNames[WhichPlot]+"_"+WhichRun+".pdf");
        //		PlotCanvas[WhichPlot]->SaveAs("/home/afroditi/Dropbox/PhD/MIT_TAU/MITAU_GENIE_Adi/cosmicOverlay/note/plotsMCC9/Afro/CalibratedPlots/"+PlotNames[WhichPlot]+".pdf");
        //		PlotCanvas[WhichPlot]->SaveAs("./myPlots/eps_"+StorePath+"/1D/"+PlotNames[WhichPlot]+"_"+WhichRun+".eps");
        //delete PlotCanvas[WhichPlot];

    } // End of the loop over the plots

} // End of the program
