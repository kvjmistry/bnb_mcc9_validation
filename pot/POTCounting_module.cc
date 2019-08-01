//some standard C++ includes

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <utility>      // std::pair, std::make_pair
#include <memory> // std::unique_ptr<>
#include <cmath>
#include <algorithm>
#include <map>
#include <tuple>

//some ROOT includes

#include "TInterpreter.h"
#include "TROOT.h"
#include "TTree.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TString.h"
#include "TStopwatch.h"

//"art" includes (canvas, and gallery)

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "art/Framework/Services/Optional/TFileService.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Utilities/make_tool.h"

#include "canvas/Utilities/InputTag.h"
#include "canvas/Persistency/Common/FindMany.h"
#include "canvas/Persistency/Common/FindOne.h"
#include "canvas/Persistency/Common/FindManyP.h"
#include "canvas/Persistency/Common/FindOneP.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "canvas/Persistency/Common/PtrVector.h"
#include "canvas/Persistency/Common/PtrVectorBase.h"

//"larsoft" object includes

#include "larcore/Geometry/Geometry.h"

#include "larcorealg/Geometry/PlaneGeo.h"
#include "larcorealg/Geometry/WireGeo.h"
#include "larcoreobj/SimpleTypesAndConstants/RawTypes.h" // raw::Compress_t, raw::Channel_t
#include "larcoreobj/SummaryData/POTSummary.h"
#include "larcoreobj/SimpleTypesAndConstants/geo_types.h"

#include "lardata/DetectorInfoServices/DetectorPropertiesService.h"
#include "lardata/DetectorInfoServices/LArPropertiesService.h"
#include "lardata/DetectorInfoServices/DetectorClocksService.h"
#include "lardata/Utilities/AssociationUtil.h"
#include "lardata/Utilities/GeometryUtilities.h"

#include "cetlib/cpu_timer.h"
#include "cetlib_except/exception.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

namespace mynamespace {
	class TTreeCreator;
}


class mynamespace::TTreeCreator : public art::EDAnalyzer {

	public:
		
		explicit TTreeCreator(fhicl::ParameterSet const & p);
		TTreeCreator(TTreeCreator const &) = delete;
		TTreeCreator(TTreeCreator &&) = delete;
		TTreeCreator & operator = (TTreeCreator const &) = delete;
		TTreeCreator & operator = (TTreeCreator &&) = delete;

		void analyze(art::Event const & e) override;
		void beginJob() override;
		void endJob() override;
		void endSubRun(const art::SubRun& sr) override;

	private:

		art::ServiceHandle<art::TFileService> tfs;
		TTree* myPOT;

		std::string fPOTSummaryOverlayLabel;
		std::string fPOTSummaryDataLabel;
		std::string fPOTSummaryDataInstanceNameLabel;

		double pot_total;
		double pot;
		double TotalPOT;
		double POT;

};

// __________________________________________________________________________________________________________________________________________________________________________________________________

mynamespace::TTreeCreator::TTreeCreator(fhicl::ParameterSet const & p):EDAnalyzer(p) {

	fPOTSummaryOverlayLabel = p.get< std::string >("POTSummaryOverlayLabel");
	fPOTSummaryDataLabel = p.get< std::string >("POTSummaryDataLabel");
	fPOTSummaryDataInstanceNameLabel = p.get< std::string >("POTSummaryDataInstanceNameLabel");

}
// _________________________________________________________________________________________________________________________________________________________________________________________________

void mynamespace::TTreeCreator::beginJob()
{
	pot_total = 0;
	pot = 0;

	myPOT = tfs->make<TTree>("myPOT", "myPOT");

	myPOT->Branch("POT", &POT);
	myPOT->Branch("TotalPOT", &TotalPOT);
}
// __________________________________________________________________________________________________________________________________________________________________________________________

void mynamespace::TTreeCreator::analyze(art::Event const & e)
{

//	std::cout << std::endl << std::endl << "Processing " << "Run " << e.run() << ", " << "SubRun " << e.subRun() << ", " << "Event " << e.event() << std::endl;

} // End of the analysis module

// __________________________________________________________________________________________________________________________________________________________________________________________________

void mynamespace::TTreeCreator::endSubRun(const art::SubRun& sr){

	// POT Summary

	art::Handle<sumdata::POTSummary> potListHandle;
	if (sr.getByLabel(fPOTSummaryOverlayLabel,potListHandle)) { pot = potListHandle->totpot; } 
	else if (sr.getByLabel(fPOTSummaryDataLabel,fPOTSummaryDataInstanceNameLabel,potListHandle)) { pot = potListHandle->totpot; } 
	else { pot = 0.; }

	pot_total += pot;
	POT = pot;
	TotalPOT = pot_total;
	myPOT->Fill();

	std::cout << std::endl << "pot = " << pot << " pot_total  = " << pot_total << std::endl << std::endl;

}

// __________________________________________________________________________________________________________________________________________________________________________________________________

void mynamespace::TTreeCreator::endJob()
{

	TFile& file = tfs->file();
	file.cd();

}
//___________________________________________________________________________________________________________________________________________________________________________________________________ 

DEFINE_ART_MODULE(mynamespace::TTreeCreator)
