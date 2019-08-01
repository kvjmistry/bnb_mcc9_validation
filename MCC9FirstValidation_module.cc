//some standard C++ includes

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <utility> // std::pair, std::make_pair
#include <memory>  // std::unique_ptr<>
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
#include "lardataobj/AnalysisBase/ParticleID_VariableTypeEnums.h"

#include "lardata/DetectorInfoServices/DetectorPropertiesService.h"
#include "lardata/DetectorInfoServices/LArPropertiesService.h"
#include "lardata/DetectorInfoServices/DetectorClocksService.h"
#include "lardata/Utilities/AssociationUtil.h"
#include "lardata/Utilities/GeometryUtilities.h"

#include "lardataobj/AnalysisBase/BackTrackerMatchingData.h"
#include "lardataobj/AnalysisBase/Calorimetry.h"
#include "lardataobj/RecoBase/Cluster.h"
#include "lardataobj/AnalysisBase/ParticleID.h"
#include "lardataobj/AnalysisBase/T0.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/MCBase/MCTrack.h"
#include "lardataobj/RecoBase/OpFlash.h"
#include "lardataobj/RecoBase/OpHit.h"
#include "lardataobj/RecoBase/PFParticle.h"
#include "lardataobj/Simulation/SimChannel.h"
#include "lardataobj/RecoBase/SpacePoint.h"
#include "lardataobj/RecoBase/Track.h"
#include "lardataobj/RecoBase/Shower.h"
#include "lardataobj/RecoBase/Vertex.h"

#include "larreco/RecoAlg/TrackMomentumCalculator.h" // Momentum from range
#include "larreco/RecoAlg/TrajectoryMCSFitter.h"     // Multiple Coulomb Scattering (MCS)

#include "larsim/MCCheater/BackTrackerService.h"
#include "larsim/MCCheater/ParticleInventoryService.h"
#include "larsim/EventWeight/Base/MCEventWeight.h"

#include "larpandora/LArPandoraInterface/LArPandoraHelper.h"

#include "nusimdata/SimulationBase/MCParticle.h"
#include "nusimdata/SimulationBase/MCTruth.h"
#include "nusimdata/SimulationBase/GTruth.h"

#include "nutools/ParticleNavigation/ParticleList.h"
#include "nutools/ParticleNavigation/EveIdCalculator.h"

#include "ubana/AnalysisTree/MCTruth/IMCTruthMatching.h" // New backtracker
#include "ubana/AnalysisTree/MCTruth/MCTruthBase/MCTruthAssociations.h"

#include "ubana/UBXSec/Algorithms/UBXSecHelper.h"

#include "ubana/MyClasses/BackTrackerTruthMatch.h"
#include "ubana/MyClasses/TruncMean.h"

#include "ubobj/Trigger/ubdaqSoftwareTriggerData.h"
#include "ubobj/CRT/CRTHit.hh"

#include "cetlib/cpu_timer.h"
#include "cetlib_except/exception.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

namespace mynamespace
{
class TTreeCreator;
}

class mynamespace::TTreeCreator : public art::EDAnalyzer
{

public:
    explicit TTreeCreator(fhicl::ParameterSet const &p);
    TTreeCreator(TTreeCreator const &) = delete;
    TTreeCreator(TTreeCreator &&) = delete;
    TTreeCreator &operator=(TTreeCreator const &) = delete;
    TTreeCreator &operator=(TTreeCreator &&) = delete;

    void analyze(art::Event const &e) override;
    void beginJob() override;
    void endJob() override;
    void endSubRun(const art::SubRun& sr) override;

private:
    std::string fSoftwareTriggerModuleLabel;
    std::string fTrackModuleLabel;
    //		std::string fShowerModuleLabel;
    std::string fHitModuleLabel;
    std::string fBeamFlashModuleLabel;
    std::string fCosmicFlashModuleLabel;
    std::string fBeamOpHitModuleLabel;
    std::string fCosmicOpHitModuleLabel;
    //		std::string fVertexModuleLabel;
    std::string fPFParticleModuleLabel;
    std::string fMCParticleModuleLabel;
    std::string fMCTruthModuleLabel;
    //		std::string fGTruthModuleLabel;
    std::string fCalorimetryModuleLabel;
    std::string fPIDaModuleLabel;
    std::string fMCSFitResultModuleLabel;
    std::string fHitsPerTrackAssModuleLabel;
    std::string fMCParticleToMCTruthAssModuleLabel;
    //		std::string fVertexToPFParticleAssModuleLabel;
    //		std::string fPFParticleToVertexAssModuleLabel;
    std::string fPFParticleToTrackAssModuleLabel;
    std::string fTrackToPFParticleAssModuleLabel;
    std::string fSoftwareTriggerAlgo;
    std::string fPOTSummaryOverlayLabel;
    std::string fPOTSummaryDataLabel;
    std::string fPOTSummaryDataInstanceNameLabel;

    art::ServiceHandle<art::TFileService> tfs;
    art::ServiceHandle<geo::Geometry> geom;

    TTree *myTTree;
    TTree* myPOT;
    int NEvents;
    int isData;
    int RunNumber;
    int SubRunNumber;
    int EventNumber;
    int EventPassedSwTrigger;
    int ientry;

    double pot_total;
    double pot;
    double TotalPOT;
    double POT;

    double Cosmic;

    //____________________________________________________________________________________________________________________________________________________________________________________

    std::vector<double> EventWeight;

    //____________________________________________________________________________________________________________________________________________________________________________________

    // Tracks

    int NumberTracks;
    std::vector<int> Track_IsBeamEvent;
    std::vector<double> Track_StartX;
    std::vector<double> Track_StartY;
    std::vector<double> Track_StartZ;
    std::vector<double> Track_EndX;
    std::vector<double> Track_EndY;
    std::vector<double> Track_EndZ;
    std::vector<int> Track_ID;
    std::vector<int> Track_ParticleId;
    std::vector<double> Track_Length;
    std::vector<double> Track_Phi;
    std::vector<double> Track_Theta;
    std::vector<double> Track_AzimuthAngle;
    std::vector<double> Track_ZenithAngle;
    //		std::vector<double> Track_VertexX;
    //		std::vector<double> Track_VertexY;
    //		std::vector<double> Track_VertexZ;
    std::vector<double> Track_Chi2;
    std::vector<double> Track_Chi2PerNdof;
    std::vector<int> Track_NPoints;
    std::vector<int> Track_NumberTrajectoryPoints;
    std::vector<double> Track_Calorimetry_Plane0_PlaneID;
    std::vector<float> Track_Calorimetry_Plane1_PlaneID;
    std::vector<float> Track_Calorimetry_Plane2_PlaneID;
    std::vector<float> Track_Calorimetry_Plane0_KineticEnergy;
    std::vector<float> Track_Calorimetry_Plane1_KineticEnergy;
    std::vector<float> Track_Calorimetry_Plane2_KineticEnergy;
    std::vector<std::vector<float>> Track_Calorimetry_Plane0_ResidualRange;
    std::vector<std::vector<float>> Track_Calorimetry_Plane1_ResidualRange;
    std::vector<std::vector<float>> Track_Calorimetry_Plane2_ResidualRange;
    std::vector<std::vector<float>> Track_Calorimetry_Plane0_dEdx;
    std::vector<std::vector<float>> Track_Calorimetry_Plane1_dEdx;
    std::vector<std::vector<float>> Track_Calorimetry_Plane2_dEdx;
    std::vector<std::vector<float>> Track_Calorimetry_Plane0_dQdx;
    std::vector<std::vector<float>> Track_Calorimetry_Plane1_dQdx;
    std::vector<std::vector<float>> Track_Calorimetry_Plane2_dQdx;
    std::vector<std::vector<float>> Track_Calorimetry_Plane0_TruncdEdx;
    std::vector<std::vector<float>> Track_Calorimetry_Plane1_TruncdEdx;
    std::vector<std::vector<float>> Track_Calorimetry_Plane2_TruncdEdx;
    std::vector<std::vector<float>> Track_Calorimetry_Plane0_TruncdQdx;
    std::vector<std::vector<float>> Track_Calorimetry_Plane1_TruncdQdx;
    std::vector<std::vector<float>> Track_Calorimetry_Plane2_TruncdQdx;
    std::vector<double> Track_Momentum;
    std::vector<double> Track_Momentum_MCS;
    std::vector<double> Track_MCParticle_P;
    std::vector<double> Track_MCParticle_E;
    std::vector<double> Track_MCParticle_Mass;
    std::vector<double> Track_MCParticle_Momentum_E;
    std::vector<double> Track_MCParticle_Momentum_Px;
    std::vector<double> Track_MCParticle_Momentum_Py;
    std::vector<double> Track_MCParticle_Momentum_Pz;
    std::vector<int> Track_MCParticle_PdgCode;
    std::vector<int> Track_MCParticle_TrackId;
    std::vector<int> Track_MCParticle_MCParticleID;
    std::vector<double> Track_MCParticle_Purity;
    std::vector<int> Track_MCParticle_Origin;
    std::vector<int> Track_MCParticle_StatusCode;
    std::vector<int> Track_MCParticle_Mother;
    std::vector<double> Track_MCParticle_Completeness;
    std::vector<double> Track_MCParticle_EndX;
    std::vector<double> Track_MCParticle_EndY;
    std::vector<double> Track_MCParticle_EndZ;
    std::vector<double> Track_MCParticle_Vx;
    std::vector<double> Track_MCParticle_Vy;
    std::vector<double> Track_MCParticle_Vz;
    std::vector<std::string> Track_MCParticle_Process;
    std::vector<double> Track_MCParticle_Nu_Vx;
    std::vector<double> Track_MCParticle_Nu_Vy;
    std::vector<double> Track_MCParticle_Nu_Vz;
    std::vector<double> Track_MCParticle_Pt;
    std::vector<double> Track_MCParticle_QSqr;
    std::vector<double> Track_MCParticle_Theta;
    std::vector<double> Track_MCParticle_X;
    std::vector<double> Track_MCParticle_Y;
    std::vector<double> Track_MCParticle_W;
    std::vector<int> Track_MCParticle_CCNC;
    std::vector<int> Track_MCParticle_Mode;
    std::vector<int> Track_MCParticle_Target;
    std::vector<int> Track_MCParticle_NParticles;
    std::vector<int> Track_MCParticle_InteractionType;
    //		std::vector<double> Track_Vertex_PositionX;
    //		std::vector<double> Track_Vertex_PositionY;
    //		std::vector<double> Track_Vertex_PositionZ;

    //		std::vector<int> Track_ParticleId_Pdg;
    //		std::vector<double> Track_ParticleId_MinChi2;
    //		std::vector<double> Track_ParticleId_Chi2Proton;
    //		std::vector<double> Track_ParticleId_Chi2Kaon;
    //		std::vector<double> Track_ParticleId_Chi2Pion;
    //		std::vector<double> Track_ParticleId_Chi2Muon;
    //		std::vector<double> Track_ParticleId_PIDA;
    //		std::vector<double> Track_ParticleId_Ndf;
    //		std::vector<double> Track_ParticleId_DeltaChi2;
    //		std::vector<double> Track_ParticleId_MissingE;
    //		std::vector<double> Track_ParticleId_MissingEavg;
    //		std::vector<double> Track_ParticleId_PlaneID;

    std::vector<double> Track_ParticleId_MIPScore_BraggPeak_YPlane;
    std::vector<double> Track_ParticleId_MuonScore_BraggPeak_YPlane;
    std::vector<double> Track_ParticleId_ProtonScore_BraggPeak_YPlane;
    std::vector<double> Track_ParticleId_PionScore_BraggPeak_YPlane;

    std::vector<double> Track_ParticleId_MIPScore_BraggPeak_UPlane;
    std::vector<double> Track_ParticleId_MuonScore_BraggPeak_UPlane;
    std::vector<double> Track_ParticleId_ProtonScore_BraggPeak_UPlane;
    std::vector<double> Track_ParticleId_PionScore_BraggPeak_UPlane;

    std::vector<double> Track_ParticleId_MIPScore_BraggPeak_VPlane;
    std::vector<double> Track_ParticleId_MuonScore_BraggPeak_VPlane;
    std::vector<double> Track_ParticleId_ProtonScore_BraggPeak_VPlane;
    std::vector<double> Track_ParticleId_PionScore_BraggPeak_VPlane;

    std::vector<double> Track_ParticleId_MIPScore_Chi2_YPlane;
    std::vector<double> Track_ParticleId_MuonScore_Chi2_YPlane;
    std::vector<double> Track_ParticleId_ProtonScore_Chi2_YPlane;
    std::vector<double> Track_ParticleId_PionScore_Chi2_YPlane;

    std::vector<double> Track_ParticleId_MIPScore_Chi2_UPlane;
    std::vector<double> Track_ParticleId_MuonScore_Chi2_UPlane;
    std::vector<double> Track_ParticleId_ProtonScore_Chi2_UPlane;
    std::vector<double> Track_ParticleId_PionScore_Chi2_UPlane;

    std::vector<double> Track_ParticleId_MIPScore_Chi2_VPlane;
    std::vector<double> Track_ParticleId_MuonScore_Chi2_VPlane;
    std::vector<double> Track_ParticleId_ProtonScore_Chi2_VPlane;
    std::vector<double> Track_ParticleId_PionScore_Chi2_VPlane;

    // Wires & Ticks

    //		std::vector<std::vector<double>> Track_PlaneID;
    //		std::vector<std::vector<double> > Track_Start_Wire;
    //		std::vector<std::vector<double>> Track_Start_Time;
    //		std::vector<std::vector<double>> Track_End_Wire;
    //		std::vector<std::vector<double>> Track_End_Time;

    // Track_AssHit

    std::vector<int> Track_NumberAssHits;
    std::vector<std::vector<int>> Track_AssHit_Channel;
    std::vector<std::vector<double>> Track_AssHit_Integral;
    std::vector<std::vector<double>> Track_AssHit_Multiplicity;
    std::vector<std::vector<double>> Track_AssHit_PeakTime;
    std::vector<std::vector<double>> Track_AssHit_PeakAmplitude;
    std::vector<std::vector<double>> Track_AssHit_RMS;
    std::vector<std::vector<double>> Track_AssHit_Wire;
    std::vector<std::vector<int>> Track_AssHit_WireID_Plane;

    //____________________________________________________________________________________________________________________________________________________________________________________

    // Hits

    int NumberHits;
    /*		std::vector<int> Hit_IsBeamEvent;*/
    std::vector<int> Hit_Channel;
    std::vector<double> Hit_Integral;
    std::vector<double> Hit_Multiplicity;
    std::vector<double> Hit_PeakTime;
    std::vector<double> Hit_PeakAmplitude;
    std::vector<double> Hit_RMS;
    std::vector<double> Hit_Wire;
    std::vector<double> Hit_SigmaPeakAmplitude;
    std::vector<double> Hit_SigmaPeakTime;
    std::vector<int> Hit_WireID_Plane;
    /*		std::vector<int> Hit_MCParticle_PdgCode;
        std::vector<double> Hit_MCParticle_Purity;
        std::vector<double> Hit_MCParticle_EndX;
        std::vector<double> Hit_MCParticle_EndY;
        std::vector<double> Hit_MCParticle_EndZ;
        std::vector<double> Hit_MCParticle_Vx;
        std::vector<double> Hit_MCParticle_Vy;
        std::vector<double> Hit_MCParticle_Vz;*/

    //____________________________________________________________________________________________________________________________________________________________________________________

    // Beam Flashes

    int NumberFlashesBeam;
    std::vector<double> FlashesBeam_Ywidth;
    std::vector<double> FlashesBeam_Zwidth;
    std::vector<double> FlashesBeam_Twidth;
    std::vector<double> FlashesBeam_Ycenter;
    std::vector<double> FlashesBeam_Zcenter;
    std::vector<double> FlashesBeam_Time;
    std::vector<double> FlashesBeam_TotalPE;
    std::vector<std::vector<double>> FlashesBeam_PE_Per_PMT;

    //____________________________________________________________________________________________________________________________________________________________________________________

    // Cosmic Flashes

    int NumberFlashesCosmic;
    std::vector<double> FlashesCosmic_Ywidth;
    std::vector<double> FlashesCosmic_Zwidth;
    std::vector<double> FlashesCosmic_Twidth;
    std::vector<double> FlashesCosmic_Ycenter;
    std::vector<double> FlashesCosmic_Zcenter;
    std::vector<double> FlashesCosmic_Time;
    std::vector<double> FlashesCosmic_TotalPE;
    std::vector<std::vector<double>> FlashesCosmic_PE_Per_PMT;

    //____________________________________________________________________________________________________________________________________________________________________________________

    // Beam OpHits

    int NumberOpHitsBeam;
    std::vector<double> OpHitsBeam_Amplitude;
    std::vector<double> OpHitsBeam_Area;
    std::vector<double> OpHitsBeam_OpChannel;
    std::vector<double> OpHitsBeam_PE;
    std::vector<double> OpHitsBeam_PeakTime;
    std::vector<double> OpHitsBeam_Width;

    //____________________________________________________________________________________________________________________________________________________________________________________

    // Cosmic OpHits

    int NumberOpHitsCosmic;
    std::vector<double> OpHitsCosmic_Amplitude;
    std::vector<double> OpHitsCosmic_Area;
    std::vector<double> OpHitsCosmic_OpChannel;
    std::vector<double> OpHitsCosmic_PE;
    std::vector<double> OpHitsCosmic_PeakTime;
    std::vector<double> OpHitsCosmic_Width;

    //____________________________________________________________________________________________________________________________________________________________________________________

    // Vertices

    //		int NumberVertices;
    //		std::vector<int> Vertex_IsBeamEvent;
    //		std::vector<double> Vertex_PositionX;
    //		std::vector<double> Vertex_PositionY;
    //		std::vector<double> Vertex_PositionZ;

    //____________________________________________________________________________________________________________________________________________________________________________________

    // MCParticles

    int NumberMCParticles;
    std::vector<int> MCParticle_Mother;
    std::vector<int> MCParticle_PdgCode;
    std::vector<int> MCParticle_StatusCode;
    std::vector<int> MCParticle_TrackId;
    std::vector<double> MCParticle_Vx;
    std::vector<double> MCParticle_Vy;
    std::vector<double> MCParticle_Vz;
    std::vector<double> MCParticle_EndX;
    std::vector<double> MCParticle_EndY;
    std::vector<double> MCParticle_EndZ;
    std::vector<std::string> MCParticle_Process;
    std::vector<double> MCParticle_E;
    std::vector<double> MCParticle_P;
    std::vector<double> MCParticle_Mass;

    //__________________________________________________________________________________________________________________________________________________________________________________

    // MCParticles Associated MCTruth

    std::vector<int> MCParticle_AssMCTruth_Origin;
    std::vector<int> MCParticle_AssMCTruth_NParticles;
    std::vector<int> MCParticle_AssMCTruth_Nu_PdgCode;
    std::vector<int> MCParticle_AssMCTruth_Lepton_PdgCode;
    std::vector<int> MCParticle_AssMCTruth_CCNC;
    std::vector<int> MCParticle_AssMCTruth_Mode;
    std::vector<int> MCParticle_AssMCTruth_InteractionType;
    std::vector<double> MCParticle_AssMCTruth_QSqr;
    std::vector<double> MCParticle_AssMCTruth_X;
    std::vector<double> MCParticle_AssMCTruth_Y;
    std::vector<double> MCParticle_AssMCTruth_W;

    //____________________________________________________________________________________________________________________________________________________________________________________

    // CRTHit

    int NCRTHits;
    std::vector<double> CRTHit_X;
    std::vector<double> CRTHit_Y;
    std::vector<double> CRTHit_Z;
    std::vector<double> CRTHit_PE;
    std::vector<int> CRTHit_Plane;
    std::vector<double> CRTHit_t0;
    std::vector<double> CRTHit_t1;

    //____________________________________________________________________________________________________________________________________________________________________________________

    // Showers

    //		int NumberShowers;
    //		std::vector<int> Shower_ID;
    //		std::vector<double> Shower_DirectionX;
    //		std::vector<double> Shower_DirectionY;
    //		std::vector<double> Shower_DirectionZ;
    //		std::vector<double> Shower_StartX;
    //		std::vector<double> Shower_StartY;
    //		std::vector<double> Shower_StartZ;
    //		std::vector<double> Shower_OpenAngle;
    //		std::vector<double> Shower_Length;
    //		std::vector<double> Shower_SumdEdx;
    //		std::vector<double> Shower_SumEnergy;
    //		std::vector<double> Shower_SumMIPEnergy;
    //		std::vector<std::vector<double>> Shower_dEdx;
    //		std::vector<std::vector<double>> Shower_Energy;
    //		std::vector<std::vector<double>> Shower_MIPEnergy;

    //____________________________________________________________________________________________________________________________________________________________________________________

    // MCTruth

    int NumberMCTruthEvents;
    std::vector<int> MCTruth_CCNC;
    std::vector<int> MCTruth_Mode;
    std::vector<double> MCTruth_Pt;
    std::vector<double> MCTruth_QSqr;
    std::vector<int> MCTruth_Target;
    std::vector<double> MCTruth_Theta;
    std::vector<double> MCTruth_X;
    std::vector<double> MCTruth_Y;
    std::vector<double> MCTruth_W;
    std::vector<int> MCTruth_NParticles;
    std::vector<int> MCTruth_Origin;
    std::vector<int> MCTruth_InteractionType;
    std::vector<std::vector<int>> MCTruth_Particle_Pdg;
    std::vector<std::vector<int>> MCTruth_Particle_TrackId;
    std::vector<std::vector<double>> MCTruth_Particle_EndX;
    std::vector<std::vector<double>> MCTruth_Particle_EndY;
    std::vector<std::vector<double>> MCTruth_Particle_EndZ;
    std::vector<std::vector<double>> MCTruth_Particle_Vx;
    std::vector<std::vector<double>> MCTruth_Particle_Vy;
    std::vector<std::vector<double>> MCTruth_Particle_Vz;
    std::vector<double> MCTruth_Particle_Nu_E;
    std::vector<double> MCTruth_Particle_Nu_Vx;
    std::vector<double> MCTruth_Particle_Nu_Vy;
    std::vector<double> MCTruth_Particle_Nu_Vz;

    //____________________________________________________________________________________________________________________________________________________________________________________

    // GTruth

    //		int NumberGTruthEvents;
    //		std::vector<double> GTruth_DiffXsec;
    //		std::vector<double> GTruth_Xsec;
    //		std::vector<int> GTruth_NumNeutron;
    //		std::vector<int> GTruth_NumPi0;
    //		std::vector<int> GTruth_NumPiPlus;
    //		std::vector<int> GTruth_NumPiMinus;
    //		std::vector<int> GTruth_NumProton;
    //		std::vector<double> GTruth_VertexX;
    //		std::vector<double> GTruth_VertexY;
    //		std::vector<double> GTruth_VertexZ;
};

// __________________________________________________________________________________________________________________________________________________________________________________________________

mynamespace::TTreeCreator::TTreeCreator(fhicl::ParameterSet const &p) : EDAnalyzer(p)
{

    fSoftwareTriggerModuleLabel = p.get<std::string>("SoftwareTriggerModuleLabel");
    fTrackModuleLabel = p.get<std::string>("TrackModuleLabel");
    //	fShowerModuleLabel = p.get< std::string >("ShowerModuleLabel");
    fHitModuleLabel = p.get<std::string>("HitModuleLabel");
    fBeamFlashModuleLabel = p.get<std::string>("BeamFlashModuleLabel");
    fCosmicFlashModuleLabel = p.get<std::string>("CosmicFlashModuleLabel");
    fBeamOpHitModuleLabel = p.get<std::string>("BeamOpHitModuleLabel");
    fCosmicOpHitModuleLabel = p.get<std::string>("CosmicOpHitModuleLabel");
    //	fVertexModuleLabel = p.get< std::string >("VertexModuleLabel");
    fPFParticleModuleLabel = p.get<std::string>("PFParticleModuleLabel");
    fMCParticleModuleLabel = p.get<std::string>("MCParticleModuleLabel");
    fMCTruthModuleLabel = p.get<std::string>("MCTruthModuleLabel");
    //	fGTruthModuleLabel = p.get< std::string >("GTruthModuleLabel");
    fCalorimetryModuleLabel = p.get<std::string>("CalorimetryModuleLabel");
    fPIDaModuleLabel = p.get<std::string>("PIDaModuleLabel");
    fMCSFitResultModuleLabel = p.get<std::string>("MCSFitResultModuleLabel");
    fHitsPerTrackAssModuleLabel = p.get<std::string>("HitsPerTrackAssModuleLabel");
    fMCParticleToMCTruthAssModuleLabel = p.get<std::string>("MCParticleToMCTruthAssModuleLabel");
    //	fVertexToPFParticleAssModuleLabel = p.get< std::string >("VertexToPFParticleAssModuleLabel");
    //	fPFParticleToVertexAssModuleLabel = p.get< std::string >("PFParticleToVertexAssModuleLabel");
    fPFParticleToTrackAssModuleLabel = p.get<std::string>("PFParticleToTrackAssModuleLabel");
    fTrackToPFParticleAssModuleLabel = p.get<std::string>("TrackToPFParticleAssModuleLabel");
    fSoftwareTriggerAlgo = p.get<std::string>("SoftwareTriggerAlgo");

    fPOTSummaryOverlayLabel = p.get< std::string >("POTSummaryOverlayLabel");
	fPOTSummaryDataLabel = p.get< std::string >("POTSummaryDataLabel");
	fPOTSummaryDataInstanceNameLabel = p.get< std::string >("POTSummaryDataInstanceNameLabel");
}
// _________________________________________________________________________________________________________________________________________________________________________________________________

void mynamespace::TTreeCreator::beginJob()
{

    Cosmic = -99.;

    pot_total = 0;
    pot = 0;

    myPOT = tfs->make<TTree>("myPOT", "myPOT");
    myTTree = tfs->make<TTree>("myTTree", "myTTree");
    // _________________________________________________________________________________________________________________________________________________________________________________________________

    myPOT->Branch("POT", &POT);
    myPOT->Branch("TotalPOT", &TotalPOT);

    myTTree->Branch("isData", &isData, "isData/I");
    myTTree->Branch("RunNumber", &RunNumber, "RunNumber/I");
    myTTree->Branch("SubRunNumber", &SubRunNumber, "SubRunNumber/I");
    myTTree->Branch("EventNumber", &EventNumber, "EventNumber/I");
    myTTree->Branch("EventPassedSwTrigger", &EventPassedSwTrigger, "EventPassedSwTrigger/I");
    myTTree->Branch("ientry", &ientry, "ientry/I");

    myTTree->Branch("EventWeight", &EventWeight);

    // Tracks

    myTTree->Branch("NumberTracks", &NumberTracks, "NumberTracks/I");
    myTTree->Branch("Track_IsBeamEvent", &Track_IsBeamEvent);
    myTTree->Branch("Track_StartX", &Track_StartX);
    myTTree->Branch("Track_StartY", &Track_StartY);
    myTTree->Branch("Track_StartZ", &Track_StartZ);
    myTTree->Branch("Track_EndX", &Track_EndX);
    myTTree->Branch("Track_EndY", &Track_EndY);
    myTTree->Branch("Track_EndZ", &Track_EndZ);
    myTTree->Branch("Track_ID", &Track_ID);
    myTTree->Branch("Track_ParticleId", &Track_ParticleId);
    myTTree->Branch("Track_Length", &Track_Length);
    myTTree->Branch("Track_Phi", &Track_Phi);
    myTTree->Branch("Track_Theta", &Track_Theta);
    myTTree->Branch("Track_AzimuthAngle", &Track_AzimuthAngle);
    myTTree->Branch("Track_ZenithAngle", &Track_ZenithAngle);
    //	myTTree->Branch("Track_VertexX", &Track_VertexX);
    //	myTTree->Branch("Track_VertexY", &Track_VertexY);
    //	myTTree->Branch("Track_VertexZ", &Track_VertexZ);
    myTTree->Branch("Track_Chi2", &Track_Chi2);
    myTTree->Branch("Track_Chi2PerNdof", &Track_Chi2PerNdof);
    myTTree->Branch("Track_NPoints", &Track_NPoints);
    myTTree->Branch("Track_NumberTrajectoryPoints", &Track_NumberTrajectoryPoints);
    myTTree->Branch("Track_Calorimetry_Plane0_PlaneID", &Track_Calorimetry_Plane0_PlaneID);
    myTTree->Branch("Track_Calorimetry_Plane1_PlaneID", &Track_Calorimetry_Plane1_PlaneID);
    myTTree->Branch("Track_Calorimetry_Plane2_PlaneID", &Track_Calorimetry_Plane2_PlaneID);
    myTTree->Branch("Track_Calorimetry_Plane0_KineticEnergy", &Track_Calorimetry_Plane0_KineticEnergy);
    myTTree->Branch("Track_Calorimetry_Plane1_KineticEnergy", &Track_Calorimetry_Plane1_KineticEnergy);
    myTTree->Branch("Track_Calorimetry_Plane2_KineticEnergy", &Track_Calorimetry_Plane2_KineticEnergy);
    myTTree->Branch("Track_Momentum", &Track_Momentum);
    myTTree->Branch("Track_Momentum_MCS", &Track_Momentum_MCS);
    myTTree->Branch("Track_Calorimetry_Plane0_ResidualRange", &Track_Calorimetry_Plane0_ResidualRange);
    myTTree->Branch("Track_Calorimetry_Plane1_ResidualRange", &Track_Calorimetry_Plane1_ResidualRange);
    myTTree->Branch("Track_Calorimetry_Plane2_ResidualRange", &Track_Calorimetry_Plane2_ResidualRange);
    myTTree->Branch("Track_Calorimetry_Plane0_dEdx", &Track_Calorimetry_Plane0_dEdx);
    myTTree->Branch("Track_Calorimetry_Plane1_dEdx", &Track_Calorimetry_Plane1_dEdx);
    myTTree->Branch("Track_Calorimetry_Plane2_dEdx", &Track_Calorimetry_Plane2_dEdx);
    myTTree->Branch("Track_Calorimetry_Plane0_dQdx", &Track_Calorimetry_Plane0_dQdx);
    myTTree->Branch("Track_Calorimetry_Plane1_dQdx", &Track_Calorimetry_Plane1_dQdx);
    myTTree->Branch("Track_Calorimetry_Plane2_dQdx", &Track_Calorimetry_Plane2_dQdx);
    myTTree->Branch("Track_Calorimetry_Plane0_TruncdEdx", &Track_Calorimetry_Plane0_TruncdEdx);
    myTTree->Branch("Track_Calorimetry_Plane1_TruncdEdx", &Track_Calorimetry_Plane1_TruncdEdx);
    myTTree->Branch("Track_Calorimetry_Plane2_TruncdEdx", &Track_Calorimetry_Plane2_TruncdEdx);
    myTTree->Branch("Track_Calorimetry_Plane0_TruncdQdx", &Track_Calorimetry_Plane0_TruncdQdx);
    myTTree->Branch("Track_Calorimetry_Plane1_TruncdQdx", &Track_Calorimetry_Plane1_TruncdQdx);
    myTTree->Branch("Track_Calorimetry_Plane2_TruncdQdx", &Track_Calorimetry_Plane2_TruncdQdx);
    myTTree->Branch("Track_MCParticle_P", &Track_MCParticle_P);
    myTTree->Branch("Track_MCParticle_E", &Track_MCParticle_E);
    myTTree->Branch("Track_MCParticle_Mass", &Track_MCParticle_Mass);
    myTTree->Branch("Track_MCParticle_Momentum_E", &Track_MCParticle_Momentum_E);
    myTTree->Branch("Track_MCParticle_Momentum_Px", &Track_MCParticle_Momentum_Px);
    myTTree->Branch("Track_MCParticle_Momentum_Py", &Track_MCParticle_Momentum_Py);
    myTTree->Branch("Track_MCParticle_Momentum_Pz", &Track_MCParticle_Momentum_Pz);
    myTTree->Branch("Track_MCParticle_PdgCode", &Track_MCParticle_PdgCode);
    myTTree->Branch("Track_MCParticle_StatusCode", &Track_MCParticle_StatusCode);
    myTTree->Branch("Track_MCParticle_Mother", &Track_MCParticle_Mother);
    myTTree->Branch("Track_MCParticle_TrackId", &Track_MCParticle_TrackId);
    myTTree->Branch("Track_MCParticle_MCParticleID", &Track_MCParticle_MCParticleID);
    myTTree->Branch("Track_MCParticle_Purity", &Track_MCParticle_Purity);
    myTTree->Branch("Track_MCParticle_Origin", &Track_MCParticle_Origin);
    myTTree->Branch("Track_MCParticle_Completeness", &Track_MCParticle_Completeness);
    myTTree->Branch("Track_MCParticle_EndX", &Track_MCParticle_EndX);
    myTTree->Branch("Track_MCParticle_EndY", &Track_MCParticle_EndY);
    myTTree->Branch("Track_MCParticle_EndZ", &Track_MCParticle_EndZ);
    myTTree->Branch("Track_MCParticle_Vx", &Track_MCParticle_Vx);
    myTTree->Branch("Track_MCParticle_Vy", &Track_MCParticle_Vy);
    myTTree->Branch("Track_MCParticle_Vz", &Track_MCParticle_Vz);
    myTTree->Branch("Track_MCParticle_Nu_Vx", &Track_MCParticle_Nu_Vx);
    myTTree->Branch("Track_MCParticle_Nu_Vy", &Track_MCParticle_Nu_Vy);
    myTTree->Branch("Track_MCParticle_Nu_Vz", &Track_MCParticle_Nu_Vz);
    myTTree->Branch("Track_MCParticle_Pt", &Track_MCParticle_Pt);
    myTTree->Branch("Track_MCParticle_QSqr", &Track_MCParticle_QSqr);
    myTTree->Branch("Track_MCParticle_Theta", &Track_MCParticle_Theta);
    myTTree->Branch("Track_MCParticle_X", &Track_MCParticle_X);
    myTTree->Branch("Track_MCParticle_Y", &Track_MCParticle_Y);
    myTTree->Branch("Track_MCParticle_W", &Track_MCParticle_W);
    myTTree->Branch("Track_MCParticle_CCNC", &Track_MCParticle_CCNC);
    myTTree->Branch("Track_MCParticle_Mode", &Track_MCParticle_Mode);
    myTTree->Branch("Track_MCParticle_Target", &Track_MCParticle_Target);
    myTTree->Branch("Track_MCParticle_NParticles", &Track_MCParticle_NParticles);
    myTTree->Branch("Track_MCParticle_InteractionType", &Track_MCParticle_InteractionType);
    myTTree->Branch("Track_MCParticle_Process", &Track_MCParticle_Process);
    //	myTTree->Branch("Track_Vertex_PositionX", &Track_Vertex_PositionX);
    //	myTTree->Branch("Track_Vertex_PositionY", &Track_Vertex_PositionY);
    //	myTTree->Branch("Track_Vertex_PositionZ", &Track_Vertex_PositionZ);

    //	myTTree->Branch("Track_ParticleId_Pdg", &Track_ParticleId_Pdg,"Track_ParticleId_Pdg/I");
    //	myTTree->Branch("Track_ParticleId_MinChi2", &Track_ParticleId_MinChi2);
    //	myTTree->Branch("Track_ParticleId_Chi2Proton", &Track_ParticleId_Chi2Proton);
    //	myTTree->Branch("Track_ParticleId_Chi2Kaon", &Track_ParticleId_Chi2Kaon);
    //	myTTree->Branch("Track_ParticleId_Chi2Pion", &Track_ParticleId_Chi2Pion);
    //	myTTree->Branch("Track_ParticleId_Chi2Muon", &Track_ParticleId_Chi2Muon);
    //	myTTree->Branch("Track_ParticleId_PIDA", &Track_ParticleId_PIDA);
    //	myTTree->Branch("Track_ParticleId_Ndf", &Track_ParticleId_Ndf);
    //	myTTree->Branch("Track_ParticleId_DeltaChi2", &Track_ParticleId_DeltaChi2);
    //	myTTree->Branch("Track_ParticleId_MissingE", &Track_ParticleId_MissingE);
    //	myTTree->Branch("Track_ParticleId_MissingEavg", &Track_ParticleId_MissingEavg);
    //	myTTree->Branch("Track_ParticleId_PlaneID", &Track_ParticleId_PlaneID);

    myTTree->Branch("Track_ParticleId_MIPScore_BraggPeak_YPlane", &Track_ParticleId_MIPScore_BraggPeak_YPlane);
    myTTree->Branch("Track_ParticleId_MuonScore_BraggPeak_YPlane", &Track_ParticleId_MuonScore_BraggPeak_YPlane);
    myTTree->Branch("Track_ParticleId_ProtonScore_BraggPeak_YPlane", &Track_ParticleId_ProtonScore_BraggPeak_YPlane);
    myTTree->Branch("Track_ParticleId_PionScore_BraggPeak_YPlane", &Track_ParticleId_PionScore_BraggPeak_YPlane);

    myTTree->Branch("Track_ParticleId_MIPScore_BraggPeak_UPlane", &Track_ParticleId_MIPScore_BraggPeak_UPlane);
    myTTree->Branch("Track_ParticleId_MuonScore_BraggPeak_UPlane", &Track_ParticleId_MuonScore_BraggPeak_UPlane);
    myTTree->Branch("Track_ParticleId_ProtonScore_BraggPeak_UPlane", &Track_ParticleId_ProtonScore_BraggPeak_UPlane);
    myTTree->Branch("Track_ParticleId_PionScore_BraggPeak_UPlane", &Track_ParticleId_PionScore_BraggPeak_UPlane);

    myTTree->Branch("Track_ParticleId_MIPScore_BraggPeak_VPlane", &Track_ParticleId_MIPScore_BraggPeak_VPlane);
    myTTree->Branch("Track_ParticleId_MuonScore_BraggPeak_VPlane", &Track_ParticleId_MuonScore_BraggPeak_VPlane);
    myTTree->Branch("Track_ParticleId_ProtonScore_BraggPeak_VPlane", &Track_ParticleId_ProtonScore_BraggPeak_VPlane);
    myTTree->Branch("Track_ParticleId_PionScore_BraggPeak_VPlane", &Track_ParticleId_PionScore_BraggPeak_VPlane);

    myTTree->Branch("Track_ParticleId_MIPScore_Chi2_YPlane", &Track_ParticleId_MIPScore_Chi2_YPlane);
    myTTree->Branch("Track_ParticleId_MuonScore_Chi2_YPlane", &Track_ParticleId_MuonScore_Chi2_YPlane);
    myTTree->Branch("Track_ParticleId_ProtonScore_Chi2_YPlane", &Track_ParticleId_ProtonScore_Chi2_YPlane);
    myTTree->Branch("Track_ParticleId_PionScore_Chi2_YPlane", &Track_ParticleId_PionScore_Chi2_YPlane);

    myTTree->Branch("Track_ParticleId_MIPScore_Chi2_UPlane", &Track_ParticleId_MIPScore_Chi2_UPlane);
    myTTree->Branch("Track_ParticleId_MuonScore_Chi2_UPlane", &Track_ParticleId_MuonScore_Chi2_UPlane);
    myTTree->Branch("Track_ParticleId_ProtonScore_Chi2_UPlane", &Track_ParticleId_ProtonScore_Chi2_UPlane);
    myTTree->Branch("Track_ParticleId_PionScore_Chi2_UPlane", &Track_ParticleId_PionScore_Chi2_UPlane);

    myTTree->Branch("Track_ParticleId_MIPScore_Chi2_VPlane", &Track_ParticleId_MIPScore_Chi2_VPlane);
    myTTree->Branch("Track_ParticleId_MuonScore_Chi2_VPlane", &Track_ParticleId_MuonScore_Chi2_VPlane);
    myTTree->Branch("Track_ParticleId_ProtonScore_Chi2_VPlane", &Track_ParticleId_ProtonScore_Chi2_VPlane);
    myTTree->Branch("Track_ParticleId_PionScore_Chi2_VPlane", &Track_ParticleId_PionScore_Chi2_VPlane);

    // Wires & Ticks

    //	myTTree->Branch("Track_PlaneID", &Track_PlaneID);
    //	myTTree->Branch("Track_Start_Wire", &Track_Start_Wire);
    //	myTTree->Branch("Track_Start_Time", &Track_Start_Time);
    //	myTTree->Branch("Track_End_Wire", &Track_End_Wire);
    //	myTTree->Branch("Track_End_Time", &Track_End_Time);

    // Track_AssHit

    myTTree->Branch("Track_NumberAssHits", &Track_NumberAssHits);
    myTTree->Branch("Track_AssHit_Channel", &Track_AssHit_Channel);
    myTTree->Branch("Track_AssHit_Integral", &Track_AssHit_Integral);
    myTTree->Branch("Track_AssHit_Multiplicity", &Track_AssHit_Multiplicity);
    myTTree->Branch("Track_AssHit_PeakTime", &Track_AssHit_PeakTime);
    myTTree->Branch("Track_AssHit_PeakAmplitude", &Track_AssHit_PeakAmplitude);
    myTTree->Branch("Track_AssHit_RMS", &Track_AssHit_RMS);
    myTTree->Branch("Track_AssHit_Wire", &Track_AssHit_Wire);
    myTTree->Branch("Track_AssHit_WireID_Plane", &Track_AssHit_WireID_Plane);

    //____________________________________________________________________________________________________________________________________________________________________________________________

    myTTree->Branch("NumberHits", &NumberHits, "NumberHits/I");
    /*	myTTree->Branch("Hit_IsBeamEvent", &Hit_IsBeamEvent);*/
    myTTree->Branch("Hit_Channel", &Hit_Channel);
    myTTree->Branch("Hit_Integral", &Hit_Integral);
    myTTree->Branch("Hit_Multiplicity", &Hit_Multiplicity);
    myTTree->Branch("Hit_PeakTime", &Hit_PeakTime);
    myTTree->Branch("Hit_PeakAmplitude", &Hit_PeakAmplitude);
    myTTree->Branch("Hit_RMS", &Hit_RMS);
    myTTree->Branch("Hit_Wire", &Hit_Wire);
    myTTree->Branch("Hit_SigmaPeakAmplitude", &Hit_SigmaPeakAmplitude);
    myTTree->Branch("Hit_SigmaPeakTime", &Hit_SigmaPeakTime);
    myTTree->Branch("Hit_WireID_Plane", &Hit_WireID_Plane);
    /*	myTTree->Branch("Hit_MCParticle_PdgCode", &Hit_MCParticle_PdgCode);
    myTTree->Branch("Hit_MCParticle_Purity", &Hit_MCParticle_Purity);
    myTTree->Branch("Hit_MCParticle_EndX", &Hit_MCParticle_EndX);
    myTTree->Branch("Hit_MCParticle_EndY", &Hit_MCParticle_EndY);
    myTTree->Branch("Hit_MCParticle_EndZ", &Hit_MCParticle_EndZ);
    myTTree->Branch("Hit_MCParticle_Vx", &Hit_MCParticle_Vx);
    myTTree->Branch("Hit_MCParticle_Vy", &Hit_MCParticle_Vy);
    myTTree->Branch("Hit_MCParticle_Vz", &Hit_MCParticle_Vz);*/

    //__________________________________________________________________________________________________________________________________________________________________________________________

    myTTree->Branch("NumberFlashesBeam", &NumberFlashesBeam, "NumberFlashesBeam/I");
    myTTree->Branch("FlashesBeam_Ywidth", &FlashesBeam_Ywidth);
    myTTree->Branch("FlashesBeam_Zwidth", &FlashesBeam_Zwidth);
    myTTree->Branch("FlashesBeam_Twidth", &FlashesBeam_Twidth);
    myTTree->Branch("FlashesBeam_Ycenter", &FlashesBeam_Ycenter);
    myTTree->Branch("FlashesBeam_Zcenter", &FlashesBeam_Zcenter);
    myTTree->Branch("FlashesBeam_Time", &FlashesBeam_Time);
    myTTree->Branch("FlashesBeam_TotalPE", &FlashesBeam_TotalPE);
    myTTree->Branch("FlashesBeam_PE_Per_PMT", &FlashesBeam_PE_Per_PMT);

    //__________________________________________________________________________________________________________________________________________________________________________________________

    myTTree->Branch("NumberFlashesCosmic", &NumberFlashesCosmic, "NumberFlashesCosmic/I");
    myTTree->Branch("FlashesCosmic_Ywidth", &FlashesCosmic_Ywidth);
    myTTree->Branch("FlashesCosmic_Zwidth", &FlashesCosmic_Zwidth);
    myTTree->Branch("FlashesCosmic_Twidth", &FlashesCosmic_Twidth);
    myTTree->Branch("FlashesCosmic_Ycenter", &FlashesCosmic_Ycenter);
    myTTree->Branch("FlashesCosmic_Zcenter", &FlashesCosmic_Zcenter);
    myTTree->Branch("FlashesCosmic_Time", &FlashesCosmic_Time);
    myTTree->Branch("FlashesCosmic_TotalPE", &FlashesCosmic_TotalPE);
    myTTree->Branch("FlashesCosmic_PE_Per_PMT", &FlashesCosmic_PE_Per_PMT);

    //_________________________________________________________________________________________________________________________________________________________________________________________

    myTTree->Branch("NumberOpHitsBeam", &NumberOpHitsBeam, "NumberOpHitsBeam/I");
    myTTree->Branch("OpHitsBeam_Amplitude", &OpHitsBeam_Amplitude);
    myTTree->Branch("OpHitsBeam_Area", &OpHitsBeam_Area);
    myTTree->Branch("OpHitsBeam_OpChannel", &OpHitsBeam_OpChannel);
    myTTree->Branch("OpHitsBeam_PE", &OpHitsBeam_PE);
    myTTree->Branch("OpHitsBeam_PeakTime", &OpHitsBeam_PeakTime);
    myTTree->Branch("OpHitsBeam_Width", &OpHitsBeam_Width);

    //__________________________________________________________________________________________________________________________________________________________________________________________

    myTTree->Branch("NumberOpHitsCosmic", &NumberOpHitsCosmic, "NumberOpHitsCosmic/I");
    myTTree->Branch("OpHitsCosmic_Amplitude", &OpHitsCosmic_Amplitude);
    myTTree->Branch("OpHitsCosmic_Area", &OpHitsCosmic_Area);
    myTTree->Branch("OpHitsCosmic_OpChannel", &OpHitsCosmic_OpChannel);
    myTTree->Branch("OpHitsCosmic_PE", &OpHitsCosmic_PE);
    myTTree->Branch("OpHitsCosmic_PeakTime", &OpHitsCosmic_PeakTime);
    myTTree->Branch("OpHitsCosmic_Width", &OpHitsCosmic_Width);

    //_________________________________________________________________________________________________________________________________________________________________________________________

    //	myTTree->Branch("NumberVertices", &NumberVertices, "NumberVertices/I");
    //	myTTree->Branch("Vertex_IsBeamEvent", &Vertex_IsBeamEvent);
    //	myTTree->Branch("Vertex_PositionX", &Vertex_PositionX);
    //	myTTree->Branch("Vertex_PositionY", &Vertex_PositionY);
    //	myTTree->Branch("Vertex_PositionZ", &Vertex_PositionZ);

    //___________________________________________________________________________________________________________________________________________________________________________________________

    // MCparticles

    myTTree->Branch("NumberMCParticles", &NumberMCParticles, "NumberMCParticles/I");
    myTTree->Branch("MCParticle_Mother", &MCParticle_Mother);
    myTTree->Branch("MCParticle_PdgCode", &MCParticle_PdgCode);
    myTTree->Branch("MCParticle_StatusCode", &MCParticle_StatusCode);
    myTTree->Branch("MCParticle_TrackId", &MCParticle_TrackId);
    myTTree->Branch("MCParticle_Vx", &MCParticle_Vx);
    myTTree->Branch("MCParticle_Vy", &MCParticle_Vy);
    myTTree->Branch("MCParticle_Vz", &MCParticle_Vz);
    myTTree->Branch("MCParticle_EndX", &MCParticle_EndX);
    myTTree->Branch("MCParticle_EndY", &MCParticle_EndY);
    myTTree->Branch("MCParticle_EndZ", &MCParticle_EndZ);
    myTTree->Branch("MCParticle_Process", &MCParticle_Process);
    myTTree->Branch("MCParticle_E", &MCParticle_E);
    myTTree->Branch("MCParticle_P", &MCParticle_P);
    myTTree->Branch("MCParticle_Mass", &MCParticle_Mass);

    //___________________________________________________________________________________________________________________________________________________________________________________________

    // MCParticles Associated MCTruth

    myTTree->Branch("MCParticle_AssMCTruth_Origin", &MCParticle_AssMCTruth_Origin);
    myTTree->Branch("MCParticle_AssMCTruth_NParticles", &MCParticle_AssMCTruth_NParticles);
    myTTree->Branch("MCParticle_AssMCTruth_Nu_PdgCode", &MCParticle_AssMCTruth_Nu_PdgCode);
    myTTree->Branch("MCParticle_AssMCTruth_Lepton_PdgCode", &MCParticle_AssMCTruth_Lepton_PdgCode);
    myTTree->Branch("MCParticle_AssMCTruth_CCNC", &MCParticle_AssMCTruth_CCNC);
    myTTree->Branch("MCParticle_AssMCTruth_Mode", &MCParticle_AssMCTruth_Mode);
    myTTree->Branch("MCParticle_AssMCTruth_InteractionType", &MCParticle_AssMCTruth_InteractionType);
    myTTree->Branch("MCParticle_AssMCTruth_QSqr", &MCParticle_AssMCTruth_QSqr);
    myTTree->Branch("MCParticle_AssMCTruth_X", &MCParticle_AssMCTruth_X);
    myTTree->Branch("MCParticle_AssMCTruth_Y", &MCParticle_AssMCTruth_Y);
    myTTree->Branch("MCParticle_AssMCTruth_W", &MCParticle_AssMCTruth_W);

    //_________________________________________________________________________________________________________________________________________________________________________________

    // CRTHit

    myTTree->Branch("NCRTHits", &NCRTHits);
    myTTree->Branch("CRTHit_X", &CRTHit_X);
    myTTree->Branch("CRTHit_Y", &CRTHit_Y);
    myTTree->Branch("CRTHit_Z", &CRTHit_Z);
    myTTree->Branch("CRTHit_PE", &CRTHit_PE);
    myTTree->Branch("CRTHit_Plane", &CRTHit_Plane);
    myTTree->Branch("CRTHit_t0", &CRTHit_t0);
    myTTree->Branch("CRTHit_t1", &CRTHit_t1);

    //____________________________________________________________________________________________________________________________________________________________________________________

    // Showers

    //	myTTree->Branch("NumberShowers", &NumberShowers, "NumberShowers/I");
    //	myTTree->Branch("Shower_ID", &Shower_ID);
    //	myTTree->Branch("Shower_DirectionX", &Shower_DirectionX);
    //	myTTree->Branch("Shower_DirectionY", &Shower_DirectionY);
    //	myTTree->Branch("Shower_DirectionZ", &Shower_DirectionZ);
    //	myTTree->Branch("Shower_StartX", &Shower_StartX);
    //	myTTree->Branch("Shower_StartY", &Shower_StartY);
    //	myTTree->Branch("Shower_StartZ", &Shower_StartZ);
    //	myTTree->Branch("Shower_OpenAngle", &Shower_OpenAngle);
    //	myTTree->Branch("Shower_Length", &Shower_Length);
    //	myTTree->Branch("Shower_SumdEdx", &Shower_SumdEdx);
    //	myTTree->Branch("Shower_SumEnergy", &Shower_SumEnergy);
    //	myTTree->Branch("Shower_SumMIPEnergy", &Shower_SumMIPEnergy);
    //	myTTree->Branch("Shower_dEdx", &Shower_dEdx);
    //	myTTree->Branch("Shower_Energy", &Shower_Energy);
    //	myTTree->Branch("Shower_MIPEnergy", &Shower_MIPEnergy);

    //_____________________________________________________________________________________________________________________________________________________________________________________________

    // MCTruth

    myTTree->Branch("NumberMCTruthEvents", &NumberMCTruthEvents, "NumberMCTruthEvents/I");
    myTTree->Branch("MCTruth_CCNC", &MCTruth_CCNC);
    myTTree->Branch("MCTruth_Mode", &MCTruth_Mode);
    myTTree->Branch("MCTruth_Pt", &MCTruth_Pt);
    myTTree->Branch("MCTruth_QSqr", &MCTruth_QSqr);
    myTTree->Branch("MCTruth_Target", &MCTruth_Target);
    myTTree->Branch("MCTruth_Theta", &MCTruth_Theta);
    myTTree->Branch("MCTruth_X", &MCTruth_X);
    myTTree->Branch("MCTruth_Y", &MCTruth_Y);
    myTTree->Branch("MCTruth_W", &MCTruth_W);
    myTTree->Branch("MCTruth_NParticles", &MCTruth_NParticles);
    myTTree->Branch("MCTruth_Origin", &MCTruth_Origin);
    myTTree->Branch("MCTruth_InteractionType", &MCTruth_InteractionType);
    myTTree->Branch("MCTruth_Particle_Pdg", &MCTruth_Particle_Pdg);
    myTTree->Branch("MCTruth_Particle_TrackId", &MCTruth_Particle_TrackId);
    myTTree->Branch("MCTruth_Particle_EndX", &MCTruth_Particle_EndX);
    myTTree->Branch("MCTruth_Particle_EndY", &MCTruth_Particle_EndY);
    myTTree->Branch("MCTruth_Particle_EndZ", &MCTruth_Particle_EndZ);
    myTTree->Branch("MCTruth_Particle_Vx", &MCTruth_Particle_Vx);
    myTTree->Branch("MCTruth_Particle_Vy", &MCTruth_Particle_Vy);
    myTTree->Branch("MCTruth_Particle_Vz", &MCTruth_Particle_Vz);
    myTTree->Branch("MCTruth_Particle_Nu_E", &MCTruth_Particle_Nu_E);
    myTTree->Branch("MCTruth_Particle_Nu_Vx", &MCTruth_Particle_Nu_Vx);
    myTTree->Branch("MCTruth_Particle_Nu_Vy", &MCTruth_Particle_Nu_Vy);
    myTTree->Branch("MCTruth_Particle_Nu_Vz", &MCTruth_Particle_Nu_Vz);

    //_____________________________________________________________________________________________________________________________________________________________________________________________

    //	// GTruth

    //	myTTree->Branch("NumberGTruthEvents", &NumberGTruthEvents, "NumberGTruthEvents/I");
    //	myTTree->Branch("GTruth_DiffXsec", &GTruth_DiffXsec);
    //	myTTree->Branch("GTruth_Xsec", &GTruth_Xsec);
    //	myTTree->Branch("GTruth_NumNeutron", &GTruth_NumNeutron);
    //	myTTree->Branch("GTruth_NumPi0", &GTruth_NumPi0);
    //	myTTree->Branch("GTruth_NumPiPlus", &GTruth_NumPiPlus);
    //	myTTree->Branch("GTruth_NumPiMinus", &GTruth_NumPiMinus);
    //	myTTree->Branch("GTruth_NumProton", &GTruth_NumProton);
    //	myTTree->Branch("GTruth_VertexX", &GTruth_VertexX);
    //	myTTree->Branch("GTruth_VertexY", &GTruth_VertexY);
    //	myTTree->Branch("GTruth_VertexZ", &GTruth_VertexZ);

    //____________________________________________________________________________________________________________________________________________________________________________________

    NEvents = 0;
}
// __________________________________________________________________________________________________________________________________________________________________________________________

void mynamespace::TTreeCreator::analyze(art::Event const &e)
{

    NEvents++;
    std::cout << std::endl
              << std::endl
              << "Processing "
              << "Run " << e.run() << ", "
              << "SubRun " << e.subRun() << ", "
              << "Event " << e.event() << std::endl;
    std::cout << std::endl
              << "Processing event # " << NEvents << std::endl
              << std::endl;

    //__________________________________________________________________________________________________________________________________________________________________________________________

    isData = e.isRealData();
    RunNumber = e.run();
    SubRunNumber = e.subRun();
    EventNumber = e.event();
    ientry = NEvents;

    //________________________________________________________________________________________________________________________________________________________________________________________

    // Handles

    // Tracks

    art::Handle<std::vector<recob::Track>> trk_handle;
    std::vector<art::Ptr<recob::Track>> trk_vec;
    if (e.getByLabel(fTrackModuleLabel, trk_handle))
    {
        art::fill_ptr_vector(trk_vec, trk_handle);
    }

    // Showers

    //	art::Handle<std::vector<recob::Shower>> shower_handle;
    //	std::vector<art::Ptr<recob::Shower>> shower_vec;
    //	if (e.getByLabel(fShowerModuleLabel,shower_handle)) { art::fill_ptr_vector(shower_vec,shower_handle); }

    // Hits

    art::Handle<std::vector<recob::Hit>> hit_handle;
    std::vector<art::Ptr<recob::Hit>> hit_vec;
    if (e.getByLabel(fHitModuleLabel, hit_handle))
    {
        art::fill_ptr_vector(hit_vec, hit_handle);
    }

    // Beam Flashes

    art::Handle<std::vector<recob::OpFlash>> flash_handle_beam;
    std::vector<art::Ptr<recob::OpFlash>> flash_beam_vec;
    if (e.getByLabel(fBeamFlashModuleLabel, flash_handle_beam))
    {
        art::fill_ptr_vector(flash_beam_vec, flash_handle_beam);
    }

    // Cosmic Flashes

    art::Handle<std::vector<recob::OpFlash>> flash_handle_cosmic;
    std::vector<art::Ptr<recob::OpFlash>> flash_cosmic_vec;
    if (e.getByLabel(fCosmicFlashModuleLabel, flash_handle_cosmic))
    {
        art::fill_ptr_vector(flash_cosmic_vec, flash_handle_cosmic);
    }

    // Beam OpHits

    art::Handle<std::vector<recob::OpHit>> ophit_handle_beam;
    std::vector<art::Ptr<recob::OpHit>> ophit_beam_vec;
    if (e.getByLabel(fBeamOpHitModuleLabel, ophit_handle_beam))
    {
        art::fill_ptr_vector(ophit_beam_vec, ophit_handle_beam);
    }

    // Cosmic OpHits

    art::Handle<std::vector<recob::OpHit>> ophit_handle_cosmic;
    std::vector<art::Ptr<recob::OpHit>> ophit_cosmic_vec;
    if (e.getByLabel(fCosmicOpHitModuleLabel, ophit_handle_cosmic))
    {
        art::fill_ptr_vector(ophit_cosmic_vec, ophit_handle_cosmic);
    }

    // Vertices

    //	art::Handle<std::vector<recob::Vertex>> vertex_handle;
    //	std::vector<art::Ptr<recob::Vertex> > vertex_vec;
    //	if ( e.getByLabel(fVertexModuleLabel,vertex_handle) ) { art::fill_ptr_vector(vertex_vec,vertex_handle); }

    // PFParticle

    art::Handle<std::vector<recob::PFParticle>> pfparticle_handle;
    std::vector<art::Ptr<recob::PFParticle>> pfparticle_vec;
    if (e.getByLabel(fPFParticleModuleLabel, pfparticle_handle))
    {
        art::fill_ptr_vector(pfparticle_vec, pfparticle_handle);
    }

    // MCParticle

    art::Handle<std::vector<simb::MCParticle>> mcparticle_handle;
    std::vector<art::Ptr<simb::MCParticle>> largeant_vec;
    if (e.getByLabel(fMCParticleModuleLabel, mcparticle_handle))
    {
        art::fill_ptr_vector(largeant_vec, mcparticle_handle);
    }

    // MCTruth

    art::Handle<std::vector<simb::MCTruth>> mctruthListHandle;
    std::vector<art::Ptr<simb::MCTruth>> mclist;
    if (e.getByLabel(fMCTruthModuleLabel, mctruthListHandle))
    {
        art::fill_ptr_vector(mclist, mctruthListHandle);
    }

    //	// GTruth

    //	art::Handle< std::vector<simb::GTruth> > gtruthListHandle;
    //	std::vector<art::Ptr<simb::GTruth> > gmclist;
    //	if (e.getByLabel(fGTruthModuleLabel,gtruthListHandle)) { art::fill_ptr_vector(gmclist, gtruthListHandle); }

    // CRTHit

    art::Handle<std::vector<crt::CRTHit>> crthitListHandle;
    std::vector<art::Ptr<crt::CRTHit>> crthit_vec;
    std::vector<art::Ptr<crt::CRTHit>> crthit_vec_merg;
    std::vector<art::Ptr<crt::CRTHit>> crthit_vec_mix;
    std::vector<art::Ptr<crt::CRTHit>> masked_crthit_vec;
    if (e.getByLabel("crthitsim", crthitListHandle))
    {
        art::fill_ptr_vector(crthit_vec, crthitListHandle);
    }
    if (e.getByLabel("merger", crthitListHandle))
    {
        art::fill_ptr_vector(crthit_vec_merg, crthitListHandle);
    }
    if (e.getByLabel("mixer", crthitListHandle))
    {
        art::fill_ptr_vector(crthit_vec_mix, crthitListHandle);
    }
    if (e.getByLabel("maskedcrthitsim", crthitListHandle))
    {
        art::fill_ptr_vector(masked_crthit_vec, crthitListHandle);
    }

    // Software Trigger

    art::Handle<raw::ubdaqSoftwareTriggerData> softwareTriggerHandle;

    if (e.getByLabel(fSoftwareTriggerModuleLabel, softwareTriggerHandle))
    {

        std::vector<std::string> algoNames = softwareTriggerHandle->getListOfAlgorithms();

        for (int i = 0; i < int(algoNames.size()); i++)
        {

            if (algoNames[i] == "BNB_FEMBeamTriggerAlgo")
            {
                EventPassedSwTrigger = softwareTriggerHandle->passedAlgo(algoNames[i]) ? 1 : 0;
            }
        }
    }
    else if (e.getByLabel("daq", softwareTriggerHandle))
    {

        std::vector<std::string> algoNames = softwareTriggerHandle->getListOfAlgorithms();

        for (int i = 0; i < int(algoNames.size()); i++)
        {

            if (algoNames[i] == "EXT_BNBwin_FEMBeamTriggerAlgo" || algoNames[i] == "BNB_FEMBeamTriggerAlgo")
            {
                EventPassedSwTrigger = softwareTriggerHandle->passedAlgo(algoNames[i]) ? 1 : 0;
            }
        }
    }

    // Multiple Coulomb Scattering (MCS)

    art::ValidHandle<std::vector<recob::MCSFitResult>> MCSMu = e.getValidHandle<std::vector<recob::MCSFitResult>>(fMCSFitResultModuleLabel);

    //	auto const * detprop = lar::providerFrom<detinfo::DetectorPropertiesService>();

    // Event Weight Splines

    art::Handle<std::vector<evwgh::MCEventWeight>> eventweight_handle;
    std::vector<art::Ptr<evwgh::MCEventWeight>> mceventweight;
    if (e.getByLabel("eventweightSplines", eventweight_handle))
    {
        art::fill_ptr_vector(mceventweight, eventweight_handle);
    }

    //_________________________________________________________________________________________________________________________________________________________________________________________
    //_________________________________________________________________________________________________________________________________________________________________________________________

    EventWeight.clear();

    if (eventweight_handle.isValid())
    {

        int NWeights = (int)eventweight_handle->size();

        for (int WhichWeight = 0; WhichWeight < NWeights; WhichWeight++)
        {

            const evwgh::MCEventWeight &eventweight = eventweight_handle->at(WhichWeight);
            std::map<std::string, std::vector<double>> CurrentEventWeights = eventweight.fWeight;

            for (const auto &temp_pair : CurrentEventWeights)
            {
                EventWeight.push_back(temp_pair.second.front());
            }
        }
    }
    else
    {
        EventWeight.push_back(1.);
    }

    //_________________________________________________________________________________________________________________________________________________________________________________________

    // Tracks

    NumberTracks = trk_vec.size();

    // Clear The Vectors

    Track_IsBeamEvent.clear();
    Track_StartX.clear();
    Track_StartY.clear();
    Track_StartZ.clear();
    Track_EndX.clear();
    Track_EndY.clear();
    Track_EndZ.clear();
    Track_ID.clear();
    Track_ParticleId.clear();
    Track_Length.clear();
    Track_Phi.clear();
    Track_AzimuthAngle.clear();
    Track_ZenithAngle.clear();
    Track_Theta.clear();
    //	Track_VertexX.clear();
    //	Track_VertexY.clear();
    //	Track_VertexZ.clear();
    Track_Chi2.clear();
    Track_Chi2PerNdof.clear();
    Track_NPoints.clear();
    Track_NumberTrajectoryPoints.clear();

    // Wires & Ticks

    //	Track_PlaneID.clear();
    //	Track_Start_Wire.clear();
    //	Track_Start_Time.clear();
    //	Track_End_Wire.clear();
    //	Track_End_Time.clear();

    // Clear The Vectors of Vectors

    Track_Calorimetry_Plane0_PlaneID.clear();
    Track_Calorimetry_Plane0_KineticEnergy.clear();
    Track_Calorimetry_Plane0_ResidualRange.clear();
    Track_Calorimetry_Plane0_dEdx.clear();
    Track_Calorimetry_Plane0_dQdx.clear();
    Track_Calorimetry_Plane0_TruncdEdx.clear();
    Track_Calorimetry_Plane0_TruncdQdx.clear();
    Track_Calorimetry_Plane1_PlaneID.clear();
    Track_Calorimetry_Plane1_KineticEnergy.clear();
    Track_Calorimetry_Plane1_ResidualRange.clear();
    Track_Calorimetry_Plane1_dEdx.clear();
    Track_Calorimetry_Plane1_dQdx.clear();
    Track_Calorimetry_Plane1_TruncdEdx.clear();
    Track_Calorimetry_Plane1_TruncdQdx.clear();
    Track_Calorimetry_Plane2_PlaneID.clear();
    Track_Calorimetry_Plane2_KineticEnergy.clear();
    Track_Calorimetry_Plane2_ResidualRange.clear();
    Track_Calorimetry_Plane2_dEdx.clear();
    Track_Calorimetry_Plane2_dQdx.clear();
    Track_Calorimetry_Plane2_TruncdEdx.clear();
    Track_Calorimetry_Plane2_TruncdQdx.clear();
    Track_Momentum.clear();
    Track_Momentum_MCS.clear();
    Track_MCParticle_P.clear();
    Track_MCParticle_E.clear();
    Track_MCParticle_Mass.clear();
    Track_MCParticle_Momentum_E.clear();
    Track_MCParticle_Momentum_Px.clear();
    Track_MCParticle_Momentum_Py.clear();
    Track_MCParticle_Momentum_Pz.clear();
    Track_MCParticle_PdgCode.clear();
    Track_MCParticle_TrackId.clear();
    Track_MCParticle_MCParticleID.clear();
    Track_MCParticle_Purity.clear();
    Track_MCParticle_Origin.clear();
    Track_MCParticle_StatusCode.clear();
    Track_MCParticle_Mother.clear();
    Track_MCParticle_Completeness.clear();
    Track_MCParticle_EndX.clear();
    Track_MCParticle_EndY.clear();
    Track_MCParticle_EndZ.clear();
    Track_MCParticle_Vx.clear();
    Track_MCParticle_Vy.clear();
    Track_MCParticle_Vz.clear();
    Track_MCParticle_Nu_Vx.clear();
    Track_MCParticle_Nu_Vy.clear();
    Track_MCParticle_Nu_Vz.clear();
    Track_MCParticle_Pt.clear();
    Track_MCParticle_QSqr.clear();
    Track_MCParticle_Theta.clear();
    Track_MCParticle_X.clear();
    Track_MCParticle_Y.clear();
    Track_MCParticle_W.clear();
    Track_MCParticle_CCNC.clear();
    Track_MCParticle_Mode.clear();
    Track_MCParticle_Target.clear();
    Track_MCParticle_NParticles.clear();
    Track_MCParticle_InteractionType.clear();
    Track_MCParticle_Process.clear();
    //	Track_Vertex_PositionX.clear();
    //	Track_Vertex_PositionY.clear();
    //	Track_Vertex_PositionZ.clear();

    //	Track_ParticleId_Pdg.clear();
    //	Track_ParticleId_MinChi2.clear();
    //	Track_ParticleId_Chi2Proton.clear();
    //	Track_ParticleId_Chi2Kaon.clear();
    //	Track_ParticleId_Chi2Pion.clear();
    //	Track_ParticleId_Chi2Muon.clear();
    //	Track_ParticleId_PIDA.clear();
    //	Track_ParticleId_Ndf.clear();
    //	Track_ParticleId_DeltaChi2.clear();
    //	Track_ParticleId_MissingE.clear();
    //	Track_ParticleId_MissingEavg.clear();
    //	Track_ParticleId_PlaneID.clear();

    Track_ParticleId_MIPScore_BraggPeak_YPlane.clear();
    Track_ParticleId_MuonScore_BraggPeak_YPlane.clear();
    Track_ParticleId_ProtonScore_BraggPeak_YPlane.clear();
    Track_ParticleId_PionScore_BraggPeak_YPlane.clear();

    Track_ParticleId_MIPScore_BraggPeak_UPlane.clear();
    Track_ParticleId_MuonScore_BraggPeak_UPlane.clear();
    Track_ParticleId_ProtonScore_BraggPeak_UPlane.clear();
    Track_ParticleId_PionScore_BraggPeak_UPlane.clear();

    Track_ParticleId_MIPScore_BraggPeak_VPlane.clear();
    Track_ParticleId_MuonScore_BraggPeak_VPlane.clear();
    Track_ParticleId_ProtonScore_BraggPeak_VPlane.clear();
    Track_ParticleId_PionScore_BraggPeak_VPlane.clear();

    Track_ParticleId_MIPScore_Chi2_YPlane.clear();
    Track_ParticleId_MuonScore_Chi2_YPlane.clear();
    Track_ParticleId_ProtonScore_Chi2_YPlane.clear();
    Track_ParticleId_PionScore_Chi2_YPlane.clear();

    Track_ParticleId_MIPScore_Chi2_UPlane.clear();
    Track_ParticleId_MuonScore_Chi2_UPlane.clear();
    Track_ParticleId_ProtonScore_Chi2_UPlane.clear();
    Track_ParticleId_PionScore_Chi2_UPlane.clear();

    Track_ParticleId_MIPScore_Chi2_VPlane.clear();
    Track_ParticleId_MuonScore_Chi2_VPlane.clear();
    Track_ParticleId_ProtonScore_Chi2_VPlane.clear();
    Track_ParticleId_PionScore_Chi2_VPlane.clear();

    Track_NumberAssHits.clear();
    Track_AssHit_Channel.clear();
    Track_AssHit_Integral.clear();
    Track_AssHit_Multiplicity.clear();
    Track_AssHit_PeakTime.clear();
    Track_AssHit_PeakAmplitude.clear();
    Track_AssHit_RMS.clear();
    Track_AssHit_Wire.clear();
    Track_AssHit_WireID_Plane.clear();

    std::cout << std::endl
              << "Loop over the " << NumberTracks << " tracks" << std::endl
              << std::endl;

    for (int i_t = 0; i_t < int(trk_vec.size()); ++i_t)
    {

        art::Ptr<recob::Track> CurrentTrack = trk_vec.at(i_t);

        Track_StartX.push_back(CurrentTrack->Start().X());
        Track_StartY.push_back(CurrentTrack->Start().Y());
        Track_StartZ.push_back(CurrentTrack->Start().Z());
        Track_EndX.push_back(CurrentTrack->End().X());
        Track_EndY.push_back(CurrentTrack->End().Y());
        Track_EndZ.push_back(CurrentTrack->End().Z());
        Track_ID.push_back(CurrentTrack->ID());
        Track_ParticleId.push_back(CurrentTrack->ParticleId());
        Track_Length.push_back(CurrentTrack->Length());
        Track_Phi.push_back(CurrentTrack->Phi());
        Track_AzimuthAngle.push_back(CurrentTrack->AzimuthAngle());
        Track_ZenithAngle.push_back(CurrentTrack->ZenithAngle());
        Track_Theta.push_back(CurrentTrack->Theta());
        //		Track_VertexX.push_back(CurrentTrack->Vertex().X());
        //		Track_VertexY.push_back(CurrentTrack->Vertex().Y());
        //		Track_VertexZ.push_back(CurrentTrack->Vertex().Z());
        Track_Chi2.push_back(CurrentTrack->Chi2());
        Track_Chi2PerNdof.push_back(CurrentTrack->Chi2PerNdof());
        Track_NPoints.push_back(CurrentTrack->NPoints());
        Track_NumberTrajectoryPoints.push_back(CurrentTrack->NumberTrajectoryPoints());

        //		if (pfparticle_handle.isValid()) {

        //			art::FindOneP<recob::PFParticle> TrackToPFParticle(trk_handle,e,fTrackToPFParticleAssModuleLabel);
        //					const art::Ptr<recob::PFParticle> CurrentAssPFParticle = TrackToPFParticle.at(CurrentTrack.key());
        //			art::FindOneP<recob::Vertex> PFParticleToVertex(pfparticle_handle,e,fPFParticleToVertexAssModuleLabel);
        //					const art::Ptr<recob::Vertex> CurrentAssVertex = PFParticleToVertex.at(CurrentAssPFParticle.key());

        //					if (!CurrentAssVertex.isNull() && !CurrentAssVertex.isNull()) {

        //						double AssVertexPosition[3] = {0,0,0};
        //						CurrentAssVertex->XYZ(AssVertexPosition);
        //						Track_Vertex_PositionX.push_back(AssVertexPosition[0]);
        //						Track_Vertex_PositionY.push_back(AssVertexPosition[1]);
        //						Track_Vertex_PositionZ.push_back(AssVertexPosition[2]);

        //					} else {

        //						std::cout << "The track-vertex assosiation doesn't exist" << std::endl;
        //						Track_Vertex_PositionX.push_back(-99.);
        //						Track_Vertex_PositionY.push_back(-99.);
        //						Track_Vertex_PositionZ.push_back(-99.);

        //					}

        //		}

        //____________________________________________________________________________________________________________________________________________________________________________________

        // Multiple Coulomb Scattering

        const recob::MCSFitResult &mcsMu = MCSMu->at(i_t);
        double trkMom_MuFwd = mcsMu.fwdMomentum();
        Track_Momentum_MCS.push_back(trkMom_MuFwd);

        //____________________________________________________________________________________________________________________________________________________________________________________

        //		// Ticks & Wires

        //		double StartLoc[3] = {CurrentTrack->Start().X(),CurrentTrack->Start().Y(),CurrentTrack->Start().Z()};

        //		std::vector<double> CurrentTrack_PlaneID;
        //		std::vector<double> CurrentTrack_Start_Wire;
        //		std::vector<double> CurrentTrack_Start_Time;
        //		std::vector<double> CurrentTrack_End_Wire;
        //		std::vector<double> CurrentTrack_End_Time;

        //		CurrentTrack_PlaneID.clear();
        //		CurrentTrack_Start_Wire.clear();
        //		CurrentTrack_Start_Time.clear();
        //		CurrentTrack_End_Wire.clear();
        //		CurrentTrack_End_Time.clear();

        //		// U / V / Y coordinates
        //		for (int plane = 0; plane < 3; plane++){

        //			geo::TPCID tpcID = geom->FindTPCAtPosition( StartLoc );
        //			int tpc = 0;
        //			if (tpcID.isValid) tpc = tpcID.TPC;
        //			else continue;
        //
        //			// Construct wire ID for this point projected onto the plane
        //			geo::PlaneID planeID = geo::PlaneID( 0 , tpc , plane ); // cryostat=0
        //
        //			// get start point
        //			int start_wire = (int) ( geom->WireCoordinate( CurrentTrack->Start().Y() , CurrentTrack->Start().Z() ,  planeID ) );
        //			int start_time = (int) ( detprop->ConvertXToTicks( CurrentTrack->Start().X() , planeID ) ) ;

        //			// get end point
        //			int end_wire = (int) ( geom->WireCoordinate( CurrentTrack->End().Y() , CurrentTrack->End().Z() ,  planeID ) );
        //			int end_time = (int) ( detprop->ConvertXToTicks( CurrentTrack->End().X() , planeID ) ) ;

        //			CurrentTrack_PlaneID.push_back(plane);
        //			CurrentTrack_Start_Wire.push_back(start_wire);
        //			CurrentTrack_Start_Time.push_back(start_time);
        //			CurrentTrack_End_Wire.push_back(end_wire);
        //			CurrentTrack_End_Time.push_back(end_time);

        //		}

        //		Track_PlaneID.push_back(CurrentTrack_PlaneID);
        //		Track_Start_Wire.push_back(CurrentTrack_Start_Wire);
        //		Track_Start_Time.push_back(CurrentTrack_Start_Time);
        //		Track_End_Wire.push_back(CurrentTrack_End_Wire);
        //		Track_End_Time.push_back(CurrentTrack_End_Time);

        //____________________________________________________________________________________________________________________________________________________________________________________

        // Track - T0 Associations

        //art::FindManyP<anab::T0> T0FromTracks(trk_handle,e,"pandoraCrtHitMatch");

        //if ( T0FromTracks.isValid() ) {

        //	std::vector<art::Ptr<anab::T0>> T0_vec = T0FromTracks.at(i_t);
        //	int NT0 = T0_vec.size();

        //	for (int i_t0 = 0; i_t0 < NT0; i_t0++) {

        //		art::Ptr<anab::T0> CurrentT0 = T0_vec.at(i_t0);

        //		std::cout << "CurrentT0->ID()" << CurrentT0->ID() << std::endl;
        //		std::cout << "CurrentT0->Time()" << CurrentT0->Time() << std::endl;
        //		std::cout << "CurrentT0->TriggerBits()" << CurrentT0->TriggerBits() << std::endl;
        //		std::cout << "CurrentT0->TriggerType()" << CurrentT0->TriggerType() << std::endl;
        //		std::cout << "CurrentT0->TriggerConfidence()" << CurrentT0->TriggerConfidence() << std::endl;

        //	}

        //}

        //____________________________________________________________________________________________________________________________________________________________________________________

        // Calorimetry

        art::FindManyP<anab::Calorimetry> CalorimetryFromTracks(trk_handle, e, fCalorimetryModuleLabel);

        if (CalorimetryFromTracks.isValid())
        {

            std::vector<art::Ptr<anab::Calorimetry>> Calorimetry_vec = CalorimetryFromTracks.at(i_t);

            // PID using Chi2 and BraggPeakLLH algos

            art::FindManyP<anab::ParticleID> fmcalical(trk_handle, e, fPIDaModuleLabel);

            if (fmcalical.isValid())
            {

                std::vector<art::Ptr<anab::ParticleID>> trackPID = fmcalical.at(CurrentTrack.key());

                double braggpeak_mip_YPlane = -999, braggpeak_mip_UPlane = -999, braggpeak_mip_VPlane = -999;
                double braggpeak_mu_YPlane = -999, braggpeak_mu_UPlane = -999, braggpeak_mu_VPlane = -999;
                double braggpeak_p_YPlane = -999, braggpeak_p_UPlane = -999, braggpeak_p_VPlane = -999;
                double braggpeak_pi_YPlane = -999, braggpeak_pi_UPlane = -999, braggpeak_pi_VPlane = -999;

                double chi2_mip_YPlane = -999, chi2_mip_UPlane = -999, chi2_mip_VPlane = -999;
                double chi2_mu_YPlane = -999, chi2_mu_UPlane = -999, chi2_mu_VPlane = -999;
                double chi2_p_YPlane = -999, chi2_p_UPlane = -999, chi2_p_VPlane = -999;
                double chi2_pi_YPlane = -999, chi2_pi_UPlane = -999, chi2_pi_VPlane = -999;

                if (trackPID.size() == 1)
                {

                    std::vector<anab::sParticleIDAlgScores> AlgScoresVec = trackPID.at(0)->ParticleIDAlgScores();

                    for (size_t i_algscore = 0; i_algscore < AlgScoresVec.size(); i_algscore++)
                    {

                        anab::sParticleIDAlgScores AlgScore = AlgScoresVec.at(i_algscore);

                        // BraggPeakLLH algo

                        if (AlgScore.fAlgName == "BraggPeakLLH" && AlgScore.fVariableType == anab::kLikelihood && AlgScore.fTrackDir == anab::kForward)
                        {

                            if (fabs(AlgScore.fAssumedPdg) == 0)
                            {
                                if (AlgScore.fPlaneMask[0] == 1)
                                {
                                    braggpeak_mip_YPlane = AlgScore.fValue;
                                }
                                if (AlgScore.fPlaneMask[1] == 1)
                                {
                                    braggpeak_mip_UPlane = AlgScore.fValue;
                                }
                                if (AlgScore.fPlaneMask[2] == 1)
                                {
                                    braggpeak_mip_VPlane = AlgScore.fValue;
                                }
                            }

                            if (fabs(AlgScore.fAssumedPdg) == 13)
                            {
                                if (AlgScore.fPlaneMask[0] == 1)
                                {
                                    braggpeak_mu_YPlane = AlgScore.fValue;
                                }
                                if (AlgScore.fPlaneMask[1] == 1)
                                {
                                    braggpeak_mu_UPlane = AlgScore.fValue;
                                }
                                if (AlgScore.fPlaneMask[2] == 1)
                                {
                                    braggpeak_mu_VPlane = AlgScore.fValue;
                                }
                            }

                            if (fabs(AlgScore.fAssumedPdg) == 2212)
                            {
                                if (AlgScore.fPlaneMask[0] == 1)
                                {
                                    braggpeak_p_YPlane = AlgScore.fValue;
                                }
                                if (AlgScore.fPlaneMask[1] == 1)
                                {
                                    braggpeak_p_UPlane = AlgScore.fValue;
                                }
                                if (AlgScore.fPlaneMask[2] == 1)
                                {
                                    braggpeak_p_VPlane = AlgScore.fValue;
                                }
                            }

                            if (fabs(AlgScore.fAssumedPdg) == 211)
                            {
                                if (AlgScore.fPlaneMask[0] == 1)
                                {
                                    braggpeak_pi_YPlane = AlgScore.fValue;
                                }
                                if (AlgScore.fPlaneMask[1] == 1)
                                {
                                    braggpeak_pi_UPlane = AlgScore.fValue;
                                }
                                if (AlgScore.fPlaneMask[2] == 1)
                                {
                                    braggpeak_pi_VPlane = AlgScore.fValue;
                                }
                            }

                        } // End of BraggPeakLLH algo

                        // Chi2 algo

                        if (AlgScore.fAlgName == "Chi2" && AlgScore.fVariableType == anab::kGOF && AlgScore.fTrackDir == anab::kForward)
                        {

                            //							if ( fabs(AlgScore.fAssumedPdg) == 0) {
                            //								if (AlgScore.fPlaneMask[0] == 1) { chi2_mip_YPlane = AlgScore.fValue; }
                            //								if (AlgScore.fPlaneMask[1] == 1) { chi2_mip_UPlane = AlgScore.fValue; }
                            //								if (AlgScore.fPlaneMask[2] == 1) { chi2_mip_VPlane = AlgScore.fValue; }
                            //							}

                            if (fabs(AlgScore.fAssumedPdg) == 13)
                            {
                                if (AlgScore.fPlaneMask[0] == 1)
                                {
                                    chi2_mu_YPlane = AlgScore.fValue;
                                }
                                if (AlgScore.fPlaneMask[1] == 1)
                                {
                                    chi2_mu_UPlane = AlgScore.fValue;
                                }
                                if (AlgScore.fPlaneMask[2] == 1)
                                {
                                    chi2_mu_VPlane = AlgScore.fValue;
                                }
                            }

                            if (fabs(AlgScore.fAssumedPdg) == 2212)
                            {

                                if (AlgScore.fPlaneMask[0] == 1)
                                {
                                    chi2_p_YPlane = AlgScore.fValue;
                                }
                                if (AlgScore.fPlaneMask[1] == 1)
                                {
                                    chi2_p_UPlane = AlgScore.fValue;
                                }
                                if (AlgScore.fPlaneMask[2] == 1)
                                {
                                    chi2_p_VPlane = AlgScore.fValue;
                                }
                            }

                            if (fabs(AlgScore.fAssumedPdg) == 211)
                            {
                                if (AlgScore.fPlaneMask[0] == 1)
                                {
                                    chi2_pi_YPlane = AlgScore.fValue;
                                }
                                if (AlgScore.fPlaneMask[1] == 1)
                                {
                                    chi2_pi_UPlane = AlgScore.fValue;
                                }
                                if (AlgScore.fPlaneMask[2] == 1)
                                {
                                    chi2_pi_VPlane = AlgScore.fValue;
                                }
                            }

                        } // End of Chi2 algo
                    }
                }

                Track_ParticleId_MIPScore_BraggPeak_YPlane.push_back(braggpeak_mip_YPlane);
                Track_ParticleId_MuonScore_BraggPeak_YPlane.push_back(braggpeak_mu_YPlane);
                Track_ParticleId_ProtonScore_BraggPeak_YPlane.push_back(braggpeak_p_YPlane);
                Track_ParticleId_PionScore_BraggPeak_YPlane.push_back(braggpeak_pi_YPlane);

                Track_ParticleId_MIPScore_BraggPeak_UPlane.push_back(braggpeak_mip_UPlane);
                Track_ParticleId_MuonScore_BraggPeak_UPlane.push_back(braggpeak_mu_UPlane);
                Track_ParticleId_ProtonScore_BraggPeak_UPlane.push_back(braggpeak_p_UPlane);
                Track_ParticleId_PionScore_BraggPeak_UPlane.push_back(braggpeak_pi_UPlane);

                Track_ParticleId_MIPScore_BraggPeak_VPlane.push_back(braggpeak_mip_VPlane);
                Track_ParticleId_MuonScore_BraggPeak_VPlane.push_back(braggpeak_mu_VPlane);
                Track_ParticleId_ProtonScore_BraggPeak_VPlane.push_back(braggpeak_p_VPlane);
                Track_ParticleId_PionScore_BraggPeak_VPlane.push_back(braggpeak_pi_VPlane);

                Track_ParticleId_MIPScore_Chi2_YPlane.push_back(chi2_mip_YPlane);
                Track_ParticleId_MuonScore_Chi2_YPlane.push_back(chi2_mu_YPlane);
                Track_ParticleId_ProtonScore_Chi2_YPlane.push_back(chi2_p_YPlane);
                Track_ParticleId_PionScore_Chi2_YPlane.push_back(chi2_pi_YPlane);

                Track_ParticleId_MIPScore_Chi2_UPlane.push_back(chi2_mip_UPlane);
                Track_ParticleId_MuonScore_Chi2_UPlane.push_back(chi2_mu_UPlane);
                Track_ParticleId_ProtonScore_Chi2_UPlane.push_back(chi2_p_UPlane);
                Track_ParticleId_PionScore_Chi2_UPlane.push_back(chi2_pi_UPlane);

                Track_ParticleId_MIPScore_Chi2_VPlane.push_back(chi2_mip_VPlane);
                Track_ParticleId_MuonScore_Chi2_VPlane.push_back(chi2_mu_VPlane);
                Track_ParticleId_ProtonScore_Chi2_VPlane.push_back(chi2_p_VPlane);
                Track_ParticleId_PionScore_Chi2_VPlane.push_back(chi2_pi_VPlane);
            }

            TruncMean truncmean;

            // Plane 0

            art::Ptr<anab::Calorimetry> plane0 = Calorimetry_vec.at(0);
            Track_Calorimetry_Plane0_PlaneID.push_back(plane0->PlaneID().Plane);
            Track_Calorimetry_Plane0_KineticEnergy.push_back(plane0->KineticEnergy());
            std::vector<float> dedxPlane0 = plane0->dEdx();
            std::vector<float> dqdxPlane0 = plane0->dQdx();
            std::vector<float> ResidualRangePlane0 = plane0->ResidualRange();
            int NHits_Plane0 = dedxPlane0.size();

            double Cali_Plane0_KE = 0;

            std::vector<float> vTrkPitchPlane0 = plane0->TrkPitchVec();

            for (int k = 0; k < NHits_Plane0; ++k)
            {
                Cali_Plane0_KE += dedxPlane0.at(k) * vTrkPitchPlane0.at(k);
            }

            std::vector<float> CurrentTrack_Calorimetry_Plane0_ResidualRange;
            std::vector<float> CurrentTrack_Calorimetry_Plane0_dEdx;
            std::vector<float> CurrentTrack_Calorimetry_Plane0_dQdx;
            std::vector<float> CurrentTrack_Calorimetry_Plane0_TruncdEdx;
            std::vector<float> CurrentTrack_Calorimetry_Plane0_TruncdQdx;

            CurrentTrack_Calorimetry_Plane0_ResidualRange.clear();
            CurrentTrack_Calorimetry_Plane0_dEdx.clear();
            CurrentTrack_Calorimetry_Plane0_dQdx.clear();
            CurrentTrack_Calorimetry_Plane0_TruncdEdx.clear();
            CurrentTrack_Calorimetry_Plane0_TruncdQdx.clear();

            for (int i_Hit_Plane0 = 0; i_Hit_Plane0 < NHits_Plane0; i_Hit_Plane0++)
            {

                CurrentTrack_Calorimetry_Plane0_ResidualRange.push_back(ResidualRangePlane0.at(i_Hit_Plane0));
                CurrentTrack_Calorimetry_Plane0_dEdx.push_back(dedxPlane0.at(i_Hit_Plane0));
                CurrentTrack_Calorimetry_Plane0_dQdx.push_back(dqdxPlane0.at(i_Hit_Plane0));
            }

            Track_Calorimetry_Plane0_ResidualRange.push_back(CurrentTrack_Calorimetry_Plane0_ResidualRange);
            Track_Calorimetry_Plane0_dEdx.push_back(CurrentTrack_Calorimetry_Plane0_dEdx);
            Track_Calorimetry_Plane0_dQdx.push_back(CurrentTrack_Calorimetry_Plane0_dQdx);

            truncmean.CalcTruncMean(CurrentTrack_Calorimetry_Plane0_ResidualRange, CurrentTrack_Calorimetry_Plane0_dEdx, CurrentTrack_Calorimetry_Plane0_TruncdEdx);
            truncmean.CalcTruncMean(CurrentTrack_Calorimetry_Plane0_ResidualRange, CurrentTrack_Calorimetry_Plane0_dQdx, CurrentTrack_Calorimetry_Plane0_TruncdQdx);

            Track_Calorimetry_Plane0_TruncdEdx.push_back(CurrentTrack_Calorimetry_Plane0_TruncdEdx);
            Track_Calorimetry_Plane0_TruncdQdx.push_back(CurrentTrack_Calorimetry_Plane0_TruncdQdx);

            // Plane 1

            art::Ptr<anab::Calorimetry> plane1 = Calorimetry_vec.at(1);
            Track_Calorimetry_Plane1_PlaneID.push_back(plane1->PlaneID().Plane);
            Track_Calorimetry_Plane1_KineticEnergy.push_back(plane1->KineticEnergy());
            std::vector<float> dedxPlane1 = plane1->dEdx();
            std::vector<float> dqdxPlane1 = plane1->dQdx();
            std::vector<float> ResidualRangePlane1 = plane1->ResidualRange();
            int NHits_Plane1 = dedxPlane1.size();

            double Cali_Plane1_KE = 0;

            std::vector<float> vTrkPitchPlane1 = plane1->TrkPitchVec();

            for (int k = 0; k < NHits_Plane1; ++k)
            {
                Cali_Plane1_KE += dedxPlane1.at(k) * vTrkPitchPlane1.at(k);
            }

            std::vector<float> CurrentTrack_Calorimetry_Plane1_ResidualRange;
            std::vector<float> CurrentTrack_Calorimetry_Plane1_dEdx;
            std::vector<float> CurrentTrack_Calorimetry_Plane1_dQdx;
            std::vector<float> CurrentTrack_Calorimetry_Plane1_TruncdEdx;
            std::vector<float> CurrentTrack_Calorimetry_Plane1_TruncdQdx;

            CurrentTrack_Calorimetry_Plane1_ResidualRange.clear();
            CurrentTrack_Calorimetry_Plane1_dEdx.clear();
            CurrentTrack_Calorimetry_Plane1_dQdx.clear();
            CurrentTrack_Calorimetry_Plane1_TruncdEdx.clear();
            CurrentTrack_Calorimetry_Plane1_TruncdQdx.clear();

            for (int i_Hit_Plane1 = 0; i_Hit_Plane1 < NHits_Plane1; i_Hit_Plane1++)
            {

                CurrentTrack_Calorimetry_Plane1_ResidualRange.push_back(ResidualRangePlane1.at(i_Hit_Plane1));
                CurrentTrack_Calorimetry_Plane1_dEdx.push_back(dedxPlane1.at(i_Hit_Plane1));
                CurrentTrack_Calorimetry_Plane1_dQdx.push_back(dqdxPlane1.at(i_Hit_Plane1));
            }

            Track_Calorimetry_Plane1_ResidualRange.push_back(CurrentTrack_Calorimetry_Plane1_ResidualRange);
            Track_Calorimetry_Plane1_dEdx.push_back(CurrentTrack_Calorimetry_Plane1_dEdx);
            Track_Calorimetry_Plane1_dQdx.push_back(CurrentTrack_Calorimetry_Plane1_dQdx);

            truncmean.CalcTruncMean(CurrentTrack_Calorimetry_Plane1_ResidualRange, CurrentTrack_Calorimetry_Plane1_dEdx, CurrentTrack_Calorimetry_Plane1_TruncdEdx);
            truncmean.CalcTruncMean(CurrentTrack_Calorimetry_Plane1_ResidualRange, CurrentTrack_Calorimetry_Plane1_dQdx, CurrentTrack_Calorimetry_Plane1_TruncdQdx);

            Track_Calorimetry_Plane1_TruncdEdx.push_back(CurrentTrack_Calorimetry_Plane1_TruncdEdx);
            Track_Calorimetry_Plane1_TruncdQdx.push_back(CurrentTrack_Calorimetry_Plane1_TruncdQdx);

            // Plane 2

            art::Ptr<anab::Calorimetry> plane2 = Calorimetry_vec.at(2);
            Track_Calorimetry_Plane2_PlaneID.push_back(plane2->PlaneID().Plane);
            Track_Calorimetry_Plane2_KineticEnergy.push_back(plane2->KineticEnergy());
            std::vector<float> dedxPlane2 = plane2->dEdx();
            std::vector<float> dqdxPlane2 = plane2->dQdx();
            std::vector<float> ResidualRangePlane2 = plane2->ResidualRange();
            int NHits_Plane2 = dedxPlane2.size();

            double Cali_Plane2_KE = 0;

            std::vector<float> vTrkPitchPlane2 = plane2->TrkPitchVec();

            for (int k = 0; k < NHits_Plane2; ++k)
            {
                Cali_Plane2_KE += dedxPlane2.at(k) * vTrkPitchPlane2.at(k);
            }

            std::vector<float> CurrentTrack_Calorimetry_Plane2_ResidualRange;
            std::vector<float> CurrentTrack_Calorimetry_Plane2_dEdx;
            std::vector<float> CurrentTrack_Calorimetry_Plane2_dQdx;
            std::vector<float> CurrentTrack_Calorimetry_Plane2_TruncdEdx;
            std::vector<float> CurrentTrack_Calorimetry_Plane2_TruncdQdx;

            CurrentTrack_Calorimetry_Plane2_ResidualRange.clear();
            CurrentTrack_Calorimetry_Plane2_dEdx.clear();
            CurrentTrack_Calorimetry_Plane2_dQdx.clear();
            CurrentTrack_Calorimetry_Plane2_TruncdEdx.clear();
            CurrentTrack_Calorimetry_Plane2_TruncdQdx.clear();

            for (int i_Hit_Plane2 = 0; i_Hit_Plane2 < NHits_Plane2; i_Hit_Plane2++)
            {

                CurrentTrack_Calorimetry_Plane2_ResidualRange.push_back(ResidualRangePlane2.at(i_Hit_Plane2));
                CurrentTrack_Calorimetry_Plane2_dEdx.push_back(dedxPlane2.at(i_Hit_Plane2));
                CurrentTrack_Calorimetry_Plane2_dQdx.push_back(dqdxPlane2.at(i_Hit_Plane2));
            }

            Track_Calorimetry_Plane2_ResidualRange.push_back(CurrentTrack_Calorimetry_Plane2_ResidualRange);
            Track_Calorimetry_Plane2_dEdx.push_back(CurrentTrack_Calorimetry_Plane2_dEdx);
            Track_Calorimetry_Plane2_dQdx.push_back(CurrentTrack_Calorimetry_Plane2_dQdx);

            truncmean.CalcTruncMean(CurrentTrack_Calorimetry_Plane2_ResidualRange, CurrentTrack_Calorimetry_Plane2_dEdx, CurrentTrack_Calorimetry_Plane2_TruncdEdx);
            truncmean.CalcTruncMean(CurrentTrack_Calorimetry_Plane2_ResidualRange, CurrentTrack_Calorimetry_Plane2_dQdx, CurrentTrack_Calorimetry_Plane2_TruncdQdx);

            Track_Calorimetry_Plane2_TruncdEdx.push_back(CurrentTrack_Calorimetry_Plane2_TruncdEdx);
            Track_Calorimetry_Plane2_TruncdQdx.push_back(CurrentTrack_Calorimetry_Plane2_TruncdQdx);
        }

        // End of calorimetry

        // ___________________________________________________________________________________________________________________________________________________________________

        // Track Associated Hits

        art::FindManyP<recob::Hit> hits_per_track(trk_handle, e, fHitsPerTrackAssModuleLabel);
        std::vector<art::Ptr<recob::Hit>> trk_hits_ptrs = hits_per_track.at(CurrentTrack.key());

        int NTrackAssHits = trk_hits_ptrs.size();

        Track_NumberAssHits.push_back(NTrackAssHits);

        std::vector<int> CurrentTrack_AssHit_Channel;
        std::vector<double> CurrentTrack_AssHit_Integral;
        std::vector<double> CurrentTrack_AssHit_Multiplicity;
        std::vector<double> CurrentTrack_AssHit_PeakTime;
        std::vector<double> CurrentTrack_AssHit_PeakAmplitude;
        std::vector<double> CurrentTrack_AssHit_RMS;
        std::vector<double> CurrentTrack_AssHit_Wire;
        std::vector<int> CurrentTrack_AssHit_WireID_Plane;

        CurrentTrack_AssHit_Channel.clear();
        CurrentTrack_AssHit_Integral.clear();
        CurrentTrack_AssHit_Multiplicity.clear();
        CurrentTrack_AssHit_PeakTime.clear();
        CurrentTrack_AssHit_PeakAmplitude.clear();
        CurrentTrack_AssHit_RMS.clear();
        CurrentTrack_AssHit_Wire.clear();
        CurrentTrack_AssHit_WireID_Plane.clear();

        for (int WhichTrackAssHit = 0; WhichTrackAssHit < NTrackAssHits; WhichTrackAssHit++)
        {

            art::Ptr<recob::Hit> CurrentTrackAssHit = trk_hits_ptrs.at(WhichTrackAssHit);

            CurrentTrack_AssHit_Channel.push_back(CurrentTrackAssHit->Channel());
            CurrentTrack_AssHit_Integral.push_back(CurrentTrackAssHit->Integral());
            CurrentTrack_AssHit_Multiplicity.push_back(CurrentTrackAssHit->Multiplicity());
            CurrentTrack_AssHit_PeakTime.push_back(CurrentTrackAssHit->PeakTime());
            CurrentTrack_AssHit_PeakAmplitude.push_back(CurrentTrackAssHit->PeakAmplitude());
            CurrentTrack_AssHit_RMS.push_back(CurrentTrackAssHit->RMS());
            CurrentTrack_AssHit_Wire.push_back(CurrentTrackAssHit->WireID().Wire);
            CurrentTrack_AssHit_WireID_Plane.push_back(CurrentTrackAssHit->WireID().Plane);
        }

        Track_AssHit_Channel.push_back(CurrentTrack_AssHit_Channel);
        Track_AssHit_Integral.push_back(CurrentTrack_AssHit_Integral);
        Track_AssHit_Multiplicity.push_back(CurrentTrack_AssHit_Multiplicity);
        Track_AssHit_PeakTime.push_back(CurrentTrack_AssHit_PeakTime);
        Track_AssHit_PeakAmplitude.push_back(CurrentTrack_AssHit_PeakAmplitude);
        Track_AssHit_RMS.push_back(CurrentTrack_AssHit_RMS);
        Track_AssHit_Wire.push_back(CurrentTrack_AssHit_Wire);
        Track_AssHit_WireID_Plane.push_back(CurrentTrack_AssHit_WireID_Plane);

        // ___________________________________________________________________________________________________________________________________________________________________

        // BackTracker

        if (e.getByLabel(fMCParticleModuleLabel, mcparticle_handle))
        {

            art::FindOneP<simb::MCTruth> MCParticleToMCTruth(mcparticle_handle, e, fMCParticleToMCTruthAssModuleLabel);
            BackTrackerTruthMatch backtrackertruthmatch;
            backtrackertruthmatch.MatchToMCParticle(hit_handle, e, trk_hits_ptrs);
            art::Ptr<simb::MCParticle> maxp_me = backtrackertruthmatch.ReturnMCParticle();

            if (maxp_me.isNull())
            {

                Track_IsBeamEvent.push_back(0);

                Track_Momentum.push_back(Cosmic);

                Track_MCParticle_P.push_back(Cosmic);
                Track_MCParticle_E.push_back(Cosmic);
                Track_MCParticle_Mass.push_back(Cosmic);
                Track_MCParticle_Momentum_E.push_back(Cosmic);
                Track_MCParticle_Momentum_Px.push_back(Cosmic);
                Track_MCParticle_Momentum_Py.push_back(Cosmic);
                Track_MCParticle_Momentum_Pz.push_back(Cosmic);
                Track_MCParticle_PdgCode.push_back(Cosmic);
                Track_MCParticle_TrackId.push_back(Cosmic);
                Track_MCParticle_MCParticleID.push_back(Cosmic);
                Track_MCParticle_Purity.push_back(Cosmic);
                Track_MCParticle_Origin.push_back(Cosmic);
                Track_MCParticle_StatusCode.push_back(-99);
                Track_MCParticle_Mother.push_back(-99);
                Track_MCParticle_Completeness.push_back(Cosmic);
                Track_MCParticle_EndX.push_back(Cosmic);
                Track_MCParticle_EndY.push_back(Cosmic);
                Track_MCParticle_EndZ.push_back(Cosmic);
                Track_MCParticle_Vx.push_back(Cosmic);
                Track_MCParticle_Vy.push_back(Cosmic);
                Track_MCParticle_Vz.push_back(Cosmic);
                Track_MCParticle_Nu_Vx.push_back(Cosmic);
                Track_MCParticle_Nu_Vy.push_back(Cosmic);
                Track_MCParticle_Nu_Vz.push_back(Cosmic);
                Track_MCParticle_Pt.push_back(Cosmic);
                Track_MCParticle_QSqr.push_back(Cosmic);
                Track_MCParticle_Theta.push_back(Cosmic);
                Track_MCParticle_X.push_back(Cosmic);
                Track_MCParticle_Y.push_back(Cosmic);
                Track_MCParticle_W.push_back(Cosmic);
                Track_MCParticle_CCNC.push_back(Cosmic);
                Track_MCParticle_Mode.push_back(Cosmic);
                Track_MCParticle_Target.push_back(Cosmic);
                Track_MCParticle_NParticles.push_back(Cosmic);
                Track_MCParticle_InteractionType.push_back(Cosmic);
                Track_MCParticle_Process.push_back("Cosmic Event");
            }
            else
            {

                double purity = backtrackertruthmatch.ReturnPurity();
                double completeness = backtrackertruthmatch.ReturnCompleteness();
                int mcparticleid = backtrackertruthmatch.ReturnMCParticleID();

                std::cout << "Event matched by the BackTracker" << std::endl;
                const art::Ptr<simb::MCTruth> mctruth = MCParticleToMCTruth.at(maxp_me.key());

                trkf::TrackMomentumCalculator TrackMomCalc;
                double TrackMomentum = TrackMomCalc.GetTrackMomentum(CurrentTrack->Length(), maxp_me->PdgCode());
                Track_Momentum.push_back(TrackMomentum);

                Track_MCParticle_P.push_back(maxp_me->P());
                Track_MCParticle_E.push_back(maxp_me->E());
                Track_MCParticle_Mass.push_back(maxp_me->Mass());
                Track_MCParticle_Momentum_E.push_back(maxp_me->Momentum().E());
                Track_MCParticle_Momentum_Px.push_back(maxp_me->Momentum().Px());
                Track_MCParticle_Momentum_Py.push_back(maxp_me->Momentum().Py());
                Track_MCParticle_Momentum_Pz.push_back(maxp_me->Momentum().Pz());
                Track_MCParticle_PdgCode.push_back(maxp_me->PdgCode());
                Track_MCParticle_TrackId.push_back(maxp_me->TrackId());
                Track_MCParticle_MCParticleID.push_back(mcparticleid);
                Track_MCParticle_Purity.push_back(purity);
                Track_MCParticle_Origin.push_back(mctruth->Origin());
                Track_MCParticle_StatusCode.push_back(maxp_me->StatusCode());
                Track_MCParticle_Mother.push_back(maxp_me->Mother());
                Track_MCParticle_Completeness.push_back(completeness);
                Track_MCParticle_EndX.push_back(maxp_me->EndX());
                Track_MCParticle_EndY.push_back(maxp_me->EndY());
                Track_MCParticle_EndZ.push_back(maxp_me->EndZ());
                Track_MCParticle_Vx.push_back(maxp_me->Vx());
                Track_MCParticle_Vy.push_back(maxp_me->Vy());
                Track_MCParticle_Vz.push_back(maxp_me->Vz());
                Track_MCParticle_NParticles.push_back(mctruth->NParticles());
                Track_MCParticle_Process.push_back(maxp_me->Process());

                // Beginning of the matched beam selection

                if (mctruth->Origin() == 1)
                {

                    std::cout << "Beam Event: maxp_me->PdgCode() = " << maxp_me->PdgCode() << "   purity = " << purity << "    completeness = " << completeness << std::endl;
                    Track_IsBeamEvent.push_back(1);

                    Track_MCParticle_Nu_Vx.push_back(mctruth->GetNeutrino().Nu().Vx());
                    Track_MCParticle_Nu_Vy.push_back(mctruth->GetNeutrino().Nu().Vy());
                    Track_MCParticle_Nu_Vz.push_back(mctruth->GetNeutrino().Nu().Vz());
                    Track_MCParticle_Pt.push_back(mctruth->GetNeutrino().Pt());
                    Track_MCParticle_QSqr.push_back(mctruth->GetNeutrino().QSqr());
                    Track_MCParticle_Theta.push_back(mctruth->GetNeutrino().Theta());
                    Track_MCParticle_X.push_back(mctruth->GetNeutrino().X());
                    Track_MCParticle_Y.push_back(mctruth->GetNeutrino().Y());
                    Track_MCParticle_W.push_back(mctruth->GetNeutrino().W());
                    Track_MCParticle_CCNC.push_back(mctruth->GetNeutrino().CCNC());
                    Track_MCParticle_Mode.push_back(mctruth->GetNeutrino().Mode());
                    Track_MCParticle_Target.push_back(mctruth->GetNeutrino().Target());
                    Track_MCParticle_NParticles.push_back(mctruth->NParticles());
                    Track_MCParticle_InteractionType.push_back(mctruth->GetNeutrino().InteractionType());

                } // End of the matched beam selection

                // Beginning of the unmatched / cosmic selection

                if (mctruth->Origin() == 2)
                {

                    std::cout << "Cosmic Event" << std::endl;
                    Track_IsBeamEvent.push_back(0);

                    Track_MCParticle_NParticles.push_back(Cosmic);
                    Track_MCParticle_Nu_Vx.push_back(Cosmic);
                    Track_MCParticle_Nu_Vy.push_back(Cosmic);
                    Track_MCParticle_Nu_Vz.push_back(Cosmic);
                    Track_MCParticle_Pt.push_back(Cosmic);
                    Track_MCParticle_QSqr.push_back(Cosmic);
                    Track_MCParticle_Theta.push_back(Cosmic);
                    Track_MCParticle_X.push_back(Cosmic);
                    Track_MCParticle_Y.push_back(Cosmic);
                    Track_MCParticle_W.push_back(Cosmic);
                    Track_MCParticle_CCNC.push_back(Cosmic);
                    Track_MCParticle_Mode.push_back(Cosmic);
                    Track_MCParticle_Target.push_back(Cosmic);
                    Track_MCParticle_InteractionType.push_back(Cosmic);

                } // End of the unmatched / cosmic selection

            } // End of the if-else statement for the BackTracker

        } // End of the if-statement for the MCParticle handle // end of the backtracker

    } // End of the loop over the tracks

    //__________________________________________________________________________________________________________________________________________________________________________________________

    // Hits

    NumberHits = hit_vec.size();

    std::cout << std::endl
              << "Loop over the " << NumberHits << " hits" << std::endl
              << std::endl;

    /*	Hit_IsBeamEvent.clear();*/
    Hit_Channel.clear();
    Hit_Integral.clear();
    Hit_Multiplicity.clear();
    Hit_PeakTime.clear();
    Hit_PeakAmplitude.clear();
    Hit_RMS.clear();
    Hit_Wire.clear();
    Hit_SigmaPeakAmplitude.clear();
    Hit_SigmaPeakTime.clear();
    Hit_WireID_Plane.clear();

    /*	Hit_MCParticle_PdgCode.clear();
    Hit_MCParticle_Purity.clear();
    Hit_MCParticle_EndX.clear();
    Hit_MCParticle_EndY.clear();
    Hit_MCParticle_EndZ.clear();
    Hit_MCParticle_Vx.clear();
    Hit_MCParticle_Vy.clear();
    Hit_MCParticle_Vz.clear();*/

    for (int i_h = 0; i_h < int(hit_vec.size()); ++i_h)
    {

        art::Ptr<recob::Hit> CurrentHit = hit_vec.at(i_h);

        Hit_Channel.push_back(CurrentHit->Channel());
        Hit_Integral.push_back(CurrentHit->Integral());
        Hit_Multiplicity.push_back(CurrentHit->Multiplicity());
        Hit_PeakTime.push_back(CurrentHit->PeakTime());
        Hit_PeakAmplitude.push_back(CurrentHit->PeakAmplitude());
        Hit_RMS.push_back(CurrentHit->RMS());
        Hit_Wire.push_back(CurrentHit->WireID().Wire);
        Hit_SigmaPeakAmplitude.push_back(CurrentHit->SigmaPeakAmplitude());
        Hit_SigmaPeakTime.push_back(CurrentHit->SigmaPeakTime());
        Hit_WireID_Plane.push_back(CurrentHit->WireID().Plane);

        // BackTracker

        /*		if (e.getByLabel(fMCParticleModuleLabel,mcparticle_handle)) {

            art::FindOneP<simb::MCTruth> MCParticleToMCTruth(mcparticle_handle,e,fMCParticleModuleLabel);
            BackTrackerTruthMatch backtrackertruthmatchhit;
            std::vector<art::Ptr<recob::Hit> > hit_vec_ptr;
            hit_vec_ptr.push_back(CurrentHit);
            backtrackertruthmatchhit.MatchToMCParticle(hit_handle,e,hit_vec_ptr);

            art::Ptr< simb::MCParticle > maxp_me_hit = backtrackertruthmatchhit.ReturnMCParticle();

            if ( maxp_me_hit.isNull()) { 

                //std::cout << "Event not matched by the BackTracker" << std::endl;
                Hit_IsBeamEvent.push_back(0);

                Hit_MCParticle_PdgCode.push_back(-99);
                Hit_MCParticle_EndX.push_back(-99.);
                Hit_MCParticle_EndY.push_back(-99.);
                Hit_MCParticle_EndZ.push_back(-99.);
                Hit_MCParticle_Vx.push_back(-99.);
                Hit_MCParticle_Vy.push_back(-99.);
                Hit_MCParticle_Vz.push_back(-99.);

            } else {

                //std::cout << "Event matched by the BackTracker" << std::endl;
                const art::Ptr<simb::MCTruth> mctruth_hit = MCParticleToMCTruth.at(maxp_me_hit.key());

                Hit_MCParticle_PdgCode.push_back(maxp_me_hit->PdgCode());
                Hit_MCParticle_EndX.push_back(maxp_me_hit->EndX());
                Hit_MCParticle_EndY.push_back(maxp_me_hit->EndY());
                Hit_MCParticle_EndZ.push_back(maxp_me_hit->EndZ());
                Hit_MCParticle_Vx.push_back(maxp_me_hit->Vx());
                Hit_MCParticle_Vy.push_back(maxp_me_hit->Vy());
                Hit_MCParticle_Vz.push_back(maxp_me_hit->Vz());


                // Beginning of the matched beam selection

                if (mctruth_hit->Origin() == 1) { 

                    //std::cout << "Beam Event" << std::endl;
                    Hit_IsBeamEvent.push_back(1);


                } // End of the matched beam selection


                // Beginning of the unmatched / cosmic selection

                if (mctruth_hit->Origin() == 2) { 

                    //std::cout << "Cosmic Event" << std::endl;
                    Hit_IsBeamEvent.push_back(0);

                } // End of the unmatched / cosmic selection

            } // End of the if-else statement for the BackTracker

        }*/
        // End of the if-statement for the MCParticle Handle // end of the Backtracker

    } // End of the loop over the hits

    // __________________________________________________________________________________________________________________________________________________________________________________________

    // Beam Flashes

    NumberFlashesBeam = flash_beam_vec.size();

    std::cout << std::endl
              << "Loop over the " << NumberFlashesBeam << " beam flashes" << std::endl
              << std::endl;

    FlashesBeam_Ywidth.clear();
    FlashesBeam_Zwidth.clear();
    FlashesBeam_Twidth.clear();
    FlashesBeam_Ycenter.clear();
    FlashesBeam_Zcenter.clear();
    FlashesBeam_Time.clear();
    FlashesBeam_TotalPE.clear();
    FlashesBeam_PE_Per_PMT.clear();

    for (int i_fb = 0; i_fb < int(flash_beam_vec.size()); ++i_fb)
    {

        art::Ptr<recob::OpFlash> CurrentBeamFlash = flash_beam_vec.at(i_fb);

        FlashesBeam_Ywidth.push_back(CurrentBeamFlash->YWidth());
        FlashesBeam_Zwidth.push_back(CurrentBeamFlash->ZWidth());
        FlashesBeam_Twidth.push_back(CurrentBeamFlash->TimeWidth());
        FlashesBeam_Ycenter.push_back(CurrentBeamFlash->YCenter());
        FlashesBeam_Zcenter.push_back(CurrentBeamFlash->ZCenter());
        FlashesBeam_Time.push_back(CurrentBeamFlash->Time());
        FlashesBeam_TotalPE.push_back(CurrentBeamFlash->TotalPE());

        std::vector<double> CurrentBeamFlash_PE_Per_PMT;

        CurrentBeamFlash_PE_Per_PMT.clear();

        for (int i_pmt = 0; i_pmt < 32; i_pmt++)
        {

            const double optical_channel_flashes_beam = CurrentBeamFlash->PE(i_pmt);

            CurrentBeamFlash_PE_Per_PMT.push_back(optical_channel_flashes_beam);
        }

        FlashesBeam_PE_Per_PMT.push_back(CurrentBeamFlash_PE_Per_PMT);

    } // End of the loop over the beam flashes

    // __________________________________________________________________________________________________________________________________________________________________________________________

    // Cosmic Flashes

    NumberFlashesCosmic = flash_cosmic_vec.size();

    std::cout << std::endl
              << "Loop over the " << NumberFlashesCosmic << " cosmic flashes" << std::endl
              << std::endl;

    FlashesCosmic_Ywidth.clear();
    FlashesCosmic_Zwidth.clear();
    FlashesCosmic_Twidth.clear();
    FlashesCosmic_Ycenter.clear();
    FlashesCosmic_Zcenter.clear();
    FlashesCosmic_Time.clear();
    FlashesCosmic_TotalPE.clear();

    for (int i_fc = 0; i_fc < int(flash_cosmic_vec.size()); ++i_fc)
    {

        art::Ptr<recob::OpFlash> CurrentCosmicFlash = flash_cosmic_vec.at(i_fc);

        FlashesCosmic_Ywidth.push_back(CurrentCosmicFlash->YWidth());
        FlashesCosmic_Zwidth.push_back(CurrentCosmicFlash->ZWidth());
        FlashesCosmic_Twidth.push_back(CurrentCosmicFlash->TimeWidth());
        FlashesCosmic_Ycenter.push_back(CurrentCosmicFlash->YCenter());
        FlashesCosmic_Zcenter.push_back(CurrentCosmicFlash->ZCenter());
        FlashesCosmic_Time.push_back(CurrentCosmicFlash->Time());
        FlashesCosmic_TotalPE.push_back(CurrentCosmicFlash->TotalPE());

        std::vector<double> CurrentCosmicFlash_PE_Per_PMT;

        CurrentCosmicFlash_PE_Per_PMT.clear();

        for (int i_pmt = 200; i_pmt < 232; i_pmt++)
        {

            const double optical_channel_flashes_cosmic = CurrentCosmicFlash->PE(i_pmt);

            CurrentCosmicFlash_PE_Per_PMT.push_back(optical_channel_flashes_cosmic);
        }

        FlashesCosmic_PE_Per_PMT.push_back(CurrentCosmicFlash_PE_Per_PMT);

    } // End of the loop over the cosmic flashes

    // __________________________________________________________________________________________________________________________________________________________________________________________

    // Beam OpHits

    NumberOpHitsBeam = ophit_beam_vec.size();

    std::cout << std::endl
              << "Loop over the " << NumberOpHitsBeam << " beam optical hits" << std::endl
              << std::endl;

    OpHitsBeam_Amplitude.clear();
    OpHitsBeam_Area.clear();
    OpHitsBeam_OpChannel.clear();
    OpHitsBeam_PE.clear();
    OpHitsBeam_PeakTime.clear();
    OpHitsBeam_Width.clear();

    for (int i_ohb = 0; i_ohb < int(ophit_beam_vec.size()); ++i_ohb)
    {

        art::Ptr<recob::OpHit> CurrentBeamOpHit = ophit_beam_vec.at(i_ohb);

        OpHitsBeam_Amplitude.push_back(CurrentBeamOpHit->Amplitude());
        OpHitsBeam_Area.push_back(CurrentBeamOpHit->Area());
        OpHitsBeam_OpChannel.push_back(CurrentBeamOpHit->OpChannel());
        OpHitsBeam_PE.push_back(CurrentBeamOpHit->PE());
        OpHitsBeam_PeakTime.push_back(CurrentBeamOpHit->PeakTime());
        OpHitsBeam_Width.push_back(CurrentBeamOpHit->Width());

    } // End of the loop over the beam optical hits

    // __________________________________________________________________________________________________________________________________________________________________________________________

    // Cosmic OpHits

    NumberOpHitsCosmic = ophit_cosmic_vec.size();

    std::cout << std::endl
              << "Loop over the " << NumberOpHitsCosmic << " cosmic optical hits" << std::endl
              << std::endl;

    OpHitsCosmic_Amplitude.clear();
    OpHitsCosmic_Area.clear();
    OpHitsCosmic_OpChannel.clear();
    OpHitsCosmic_PE.clear();
    OpHitsCosmic_PeakTime.clear();
    OpHitsCosmic_Width.clear();

    for (int i_ohc = 0; i_ohc < int(ophit_cosmic_vec.size()); ++i_ohc)
    {

        art::Ptr<recob::OpHit> CurrentCosmicOpHit = ophit_cosmic_vec.at(i_ohc);

        OpHitsCosmic_Amplitude.push_back(CurrentCosmicOpHit->Amplitude());
        OpHitsCosmic_Area.push_back(CurrentCosmicOpHit->Area());
        OpHitsCosmic_OpChannel.push_back(CurrentCosmicOpHit->OpChannel());
        OpHitsCosmic_PE.push_back(CurrentCosmicOpHit->PE());
        OpHitsCosmic_PeakTime.push_back(CurrentCosmicOpHit->PeakTime());
        OpHitsCosmic_Width.push_back(CurrentCosmicOpHit->Width());

    } // End of the loop over the cosmic optical hits

    // _______________________________________________________________________________________________________________________________________________________________________________________

    // Vertices

    //	NumberVertices = vertex_vec.size();

    //	std::cout << std::endl << "Loop over the " << NumberVertices << " vertices" << std::endl << std::endl;

    //	Vertex_IsBeamEvent.clear();
    //	Vertex_PositionX.clear();
    //	Vertex_PositionY.clear();
    //	Vertex_PositionZ.clear();

    //	double VertexPosition[3] = {0,0,0};

    //	for (int i_v = 0; i_v < int(vertex_vec.size()); ++i_v) {

    //		art::Ptr<recob::Vertex> CurrentVertex = vertex_vec.at(i_v);

    //		CurrentVertex->XYZ(VertexPosition);
    //		Vertex_PositionX.push_back(VertexPosition[0]);
    //		Vertex_PositionY.push_back(VertexPosition[1]);
    //		Vertex_PositionZ.push_back(VertexPosition[2]);

    //		art::FindOneP<recob::PFParticle> VertexToPFParticle(vertex_handle,e,fVertexToPFParticleAssModuleLabel);
    //		art::Ptr<recob::PFParticle> CurrentPFParticle = VertexToPFParticle.at(CurrentVertex.key());

    //		if (CurrentPFParticle.isNull()) {

    //			Vertex_IsBeamEvent.push_back(0);

    //		} else {

    //			art::FindOneP<recob::Track> PFParticleToTrack(pfparticle_handle,e,fPFParticleToTrackAssModuleLabel);
    //			art::Ptr<recob::Track> CurrentTrack = PFParticleToTrack.at(CurrentPFParticle.key());

    //			if (CurrentTrack.isNull()) {

    //				Vertex_IsBeamEvent.push_back(0);

    //			} else {

    //				if (e.getByLabel(fMCParticleModuleLabel,mcparticle_handle)) {

    //					art::FindManyP<recob::Hit> hits_per_track(trk_handle,e,fHitsPerTrackAssModuleLabel);
    //					std::vector< art::Ptr<recob::Hit> > trk_hits_ptrs_vertex = hits_per_track.at(CurrentTrack.key());
    //					BackTrackerTruthMatch backtrackertruthmatch_track_vertex;
    //					backtrackertruthmatch_track_vertex.MatchToMCParticle(hit_handle,e,trk_hits_ptrs_vertex);
    //					art::Ptr< simb::MCParticle > maxp_me_track_vertex = backtrackertruthmatch_track_vertex.ReturnMCParticle();

    //					if (maxp_me_track_vertex.isNull()) {

    //						Vertex_IsBeamEvent.push_back(0);

    //					} else {

    //						Vertex_IsBeamEvent.push_back(1);

    //					}

    //				} // End of the if-statement if the MCparticle Handle exists

    //			} // End of the track case

    //		} // End of the if-statement whether the VertexToPFParticle assosiation exists

    //	} // End of the loop over the vertices

    // ___________________________________________________________________________________________________________________________________________________________________________________________

    // Loop over the MCParticles

    NumberMCParticles = largeant_vec.size();

    MCParticle_Mother.clear();
    MCParticle_PdgCode.clear();
    MCParticle_StatusCode.clear();
    MCParticle_TrackId.clear();
    MCParticle_Vx.clear();
    MCParticle_Vy.clear();
    MCParticle_Vz.clear();
    MCParticle_EndX.clear();
    MCParticle_EndY.clear();
    MCParticle_EndZ.clear();
    MCParticle_Process.clear();
    MCParticle_E.clear();
    MCParticle_P.clear();
    MCParticle_Mass.clear();

    MCParticle_AssMCTruth_Origin.clear();
    MCParticle_AssMCTruth_NParticles.clear();
    MCParticle_AssMCTruth_Nu_PdgCode.clear();
    MCParticle_AssMCTruth_Lepton_PdgCode.clear();
    MCParticle_AssMCTruth_CCNC.clear();
    MCParticle_AssMCTruth_Mode.clear();
    MCParticle_AssMCTruth_InteractionType.clear();
    MCParticle_AssMCTruth_QSqr.clear();
    MCParticle_AssMCTruth_X.clear();
    MCParticle_AssMCTruth_Y.clear();
    MCParticle_AssMCTruth_W.clear();

    std::cout << std::endl
              << "Loop over the " << NumberMCParticles << " MCParticles." << std::endl
              << std::endl;

    for (int i_mc = 0; i_mc < int(largeant_vec.size()); ++i_mc)
    {

        art::Ptr<simb::MCParticle> CurrentMCParticle = largeant_vec.at(i_mc);

        // MCparticles

        MCParticle_Mother.push_back(CurrentMCParticle->Mother());
        MCParticle_PdgCode.push_back(CurrentMCParticle->PdgCode());
        MCParticle_StatusCode.push_back(CurrentMCParticle->StatusCode());
        MCParticle_TrackId.push_back(CurrentMCParticle->TrackId());
        MCParticle_Vx.push_back(CurrentMCParticle->Vx());
        MCParticle_Vy.push_back(CurrentMCParticle->Vy());
        MCParticle_Vz.push_back(CurrentMCParticle->Vz());
        MCParticle_EndX.push_back(CurrentMCParticle->EndX());
        MCParticle_EndY.push_back(CurrentMCParticle->EndY());
        MCParticle_EndZ.push_back(CurrentMCParticle->EndZ());
        MCParticle_Process.push_back(CurrentMCParticle->Process());
        MCParticle_E.push_back(CurrentMCParticle->E());
        MCParticle_P.push_back(CurrentMCParticle->P());
        MCParticle_Mass.push_back(CurrentMCParticle->Mass());

        art::FindOneP<simb::MCTruth> MCParticleToMCTruth(mcparticle_handle, e, fMCParticleToMCTruthAssModuleLabel);
        const art::Ptr<simb::MCTruth> mcpartruth = MCParticleToMCTruth.at(CurrentMCParticle.key());

        // Associated MCTruth

        MCParticle_AssMCTruth_Origin.push_back(mcpartruth->Origin());
        MCParticle_AssMCTruth_NParticles.push_back(mcpartruth->NParticles());
        MCParticle_AssMCTruth_Nu_PdgCode.push_back(mcpartruth->GetNeutrino().Nu().PdgCode());
        MCParticle_AssMCTruth_Lepton_PdgCode.push_back(mcpartruth->GetNeutrino().Lepton().PdgCode());
        MCParticle_AssMCTruth_CCNC.push_back(mcpartruth->GetNeutrino().CCNC());
        MCParticle_AssMCTruth_Mode.push_back(mcpartruth->GetNeutrino().Mode());
        MCParticle_AssMCTruth_InteractionType.push_back(mcpartruth->GetNeutrino().InteractionType());
        MCParticle_AssMCTruth_QSqr.push_back(mcpartruth->GetNeutrino().QSqr());
        MCParticle_AssMCTruth_X.push_back(mcpartruth->GetNeutrino().X());
        MCParticle_AssMCTruth_Y.push_back(mcpartruth->GetNeutrino().Y());
        MCParticle_AssMCTruth_W.push_back(mcpartruth->GetNeutrino().W());
    }

    // ________________________________________________________________________________________________________________________________________________________________________________________

    // Loop over the CRTHits

    // WATCH OUT!!! Two handles for the overlay files

    NCRTHits = crthit_vec.size();

    CRTHit_X.clear();
    CRTHit_Y.clear();
    CRTHit_Z.clear();
    CRTHit_PE.clear();
    CRTHit_Plane.clear();
    CRTHit_t0.clear();
    CRTHit_t1.clear();

    std::cout << std::endl
              << "Loop over the " << int(crthit_vec.size()) << " crthitsim CRTHits." << std::endl
              << std::endl;

    for (int i_crth = 0; i_crth < int(crthit_vec.size()); ++i_crth)
    {

        art::Ptr<crt::CRTHit> CurrentCRTHit = crthit_vec.at(i_crth);

        CRTHit_X.push_back(CurrentCRTHit->x_pos);
        CRTHit_Y.push_back(CurrentCRTHit->y_pos);
        CRTHit_Z.push_back(CurrentCRTHit->z_pos);
        CRTHit_PE.push_back(CurrentCRTHit->peshit);
        CRTHit_Plane.push_back(CurrentCRTHit->plane);
        CRTHit_t0.push_back(CurrentCRTHit->ts0_ns);
        CRTHit_t1.push_back(CurrentCRTHit->ts1_ns);

    } // End of the loop over the CRTHits

    std::cout << std::endl
              << "Loop over the " << int(crthit_vec_merg.size()) << " merger CRTHits." << std::endl
              << std::endl;

    for (int i_merg = 0; i_merg < int(crthit_vec_merg.size()); ++i_merg)
    {

        art::Ptr<crt::CRTHit> CurrentCRTHit = crthit_vec_merg.at(i_merg);

        CRTHit_X.push_back(CurrentCRTHit->x_pos);
        CRTHit_Y.push_back(CurrentCRTHit->y_pos);
        CRTHit_Z.push_back(CurrentCRTHit->z_pos);
        CRTHit_PE.push_back(CurrentCRTHit->peshit);
        CRTHit_Plane.push_back(CurrentCRTHit->plane);
        CRTHit_t0.push_back(CurrentCRTHit->ts0_ns);
        CRTHit_t1.push_back(CurrentCRTHit->ts1_ns);

    } // End of the loop over the merger CRTHits

    std::cout << std::endl
              << "Loop over the " << int(crthit_vec_mix.size()) << " mixer CRTHits." << std::endl
              << std::endl;

    for (int i_crth = 0; i_crth < int(crthit_vec_mix.size()); ++i_crth)
    {

        art::Ptr<crt::CRTHit> CurrentCRTHit = crthit_vec_mix.at(i_crth);

        CRTHit_X.push_back(CurrentCRTHit->x_pos);
        CRTHit_Y.push_back(CurrentCRTHit->y_pos);
        CRTHit_Z.push_back(CurrentCRTHit->z_pos);
        CRTHit_PE.push_back(CurrentCRTHit->peshit);
        CRTHit_Plane.push_back(CurrentCRTHit->plane);
        CRTHit_t0.push_back(CurrentCRTHit->ts0_ns);
        CRTHit_t1.push_back(CurrentCRTHit->ts1_ns);

    } // End of the loop over the mixer CRTHits

    std::cout << std::endl
              << "Loop over the " << int(masked_crthit_vec.size()) << " maskedcrthitsim CRTHits." << std::endl
              << std::endl;

    for (int i_crth = 0; i_crth < int(masked_crthit_vec.size()); ++i_crth)
    {

        art::Ptr<crt::CRTHit> CurrentCRTHit = masked_crthit_vec.at(i_crth);

        CRTHit_X.push_back(CurrentCRTHit->x_pos);
        CRTHit_Y.push_back(CurrentCRTHit->y_pos);
        CRTHit_Z.push_back(CurrentCRTHit->z_pos);
        CRTHit_PE.push_back(CurrentCRTHit->peshit);
        CRTHit_Plane.push_back(CurrentCRTHit->plane);
        CRTHit_t0.push_back(CurrentCRTHit->ts0_ns);
        CRTHit_t1.push_back(CurrentCRTHit->ts1_ns);

    } // End of the loop over the CRTHits

    // _______________________________________________________________________________________________________________________________________________________________________________________

    // Showers

    //	NumberShowers = shower_vec.size();

    //	std::cout << std::endl << "Loop over the " << NumberShowers << " showers" << std::endl << std::endl;

    //	Shower_ID.clear();
    //	Shower_DirectionX.clear();
    //	Shower_DirectionY.clear();
    //	Shower_DirectionZ.clear();
    //	Shower_StartX.clear();
    //	Shower_StartY.clear();
    //	Shower_StartZ.clear();
    //	Shower_OpenAngle.clear();
    //	Shower_Length.clear();
    //	Shower_SumdEdx.clear();
    //	Shower_SumEnergy.clear();
    //	Shower_SumMIPEnergy.clear();
    //	Shower_dEdx.clear();
    //	Shower_Energy.clear();
    //	Shower_MIPEnergy.clear();

    //	std::vector<double> CurrentShower_dEdx;
    //	std::vector<double> CurrentShower_Energy;
    //	std::vector<double> CurrentShower_MIPEnergy;

    //	for (int i_sh = 0; i_sh < NumberShowers; ++i_sh) {

    //		art::Ptr<recob::Shower> CurrentShower = shower_vec.at(i_sh);

    //		Shower_ID.push_back(CurrentShower->ID());
    //		Shower_DirectionX.push_back(CurrentShower->Direction().X());
    //		Shower_DirectionY.push_back(CurrentShower->Direction().Y());
    //		Shower_DirectionZ.push_back(CurrentShower->Direction().Z());
    //		Shower_StartX.push_back(CurrentShower->ShowerStart().X());
    //		Shower_StartY.push_back(CurrentShower->ShowerStart().Y());
    //		Shower_StartZ.push_back(CurrentShower->ShowerStart().Z());
    //		Shower_OpenAngle.push_back(CurrentShower->OpenAngle());
    //		Shower_Length.push_back(CurrentShower->Length());

    //		CurrentShower_dEdx.clear();
    //		CurrentShower_Energy.clear();
    //		CurrentShower_MIPEnergy.clear();

    //		double SumdEdx = 0;
    //		double SumEnergy = 0;
    //		double SumMIPEnergy = 0;

    //		int NdEdx = (CurrentShower->dEdx()).size();

    //		for (int step = 0; step < NdEdx; step ++) {
    //std::cout << "non-zero vector for shower energy deposition" << std::endl;
    //			CurrentShower_dEdx.push_back(CurrentShower->dEdx().at(step));
    //			CurrentShower_Energy.push_back(CurrentShower->Energy().at(step));
    //			CurrentShower_MIPEnergy.push_back(CurrentShower->MIPEnergy().at(step));

    //			SumdEdx += CurrentShower->dEdx().at(step);
    //			SumEnergy += CurrentShower->Energy().at(step);
    //			SumMIPEnergy += CurrentShower->MIPEnergy().at(step);

    //		}

    //		Shower_SumdEdx.push_back(SumdEdx);
    //		Shower_SumEnergy.push_back(SumEnergy);
    //		Shower_SumMIPEnergy.push_back(SumMIPEnergy);

    //		Shower_dEdx.push_back(CurrentShower_dEdx);
    //		Shower_Energy.push_back(CurrentShower_Energy);
    //		Shower_MIPEnergy.push_back(CurrentShower_MIPEnergy);

    //	} // End of the loop over the showers

    // _________________________________________________________________________________________________________________________________________________________________________________________

    // MCTruth

    NumberMCTruthEvents = mclist.size();
    std::cout << std::endl
              << "Loop over the " << NumberMCTruthEvents << " mctruth" << std::endl
              << std::endl;
    MCTruth_CCNC.clear();
    MCTruth_Mode.clear();
    MCTruth_Pt.clear();
    MCTruth_QSqr.clear();
    MCTruth_Target.clear();
    MCTruth_Theta.clear();
    MCTruth_X.clear();
    MCTruth_Y.clear();
    MCTruth_W.clear();
    MCTruth_NParticles.clear();
    MCTruth_Origin.clear();
    MCTruth_InteractionType.clear();
    MCTruth_Particle_Pdg.clear();
    MCTruth_Particle_TrackId.clear();
    MCTruth_Particle_EndX.clear();
    MCTruth_Particle_EndY.clear();
    MCTruth_Particle_EndZ.clear();
    MCTruth_Particle_Vx.clear();
    MCTruth_Particle_Vy.clear();
    MCTruth_Particle_Vz.clear();
    MCTruth_Particle_Nu_E.clear();
    MCTruth_Particle_Nu_Vx.clear();
    MCTruth_Particle_Nu_Vy.clear();
    MCTruth_Particle_Nu_Vz.clear();

    for (int i_mclist = 0; i_mclist < NumberMCTruthEvents; i_mclist++)
    {

        art::Ptr<simb::MCTruth> CurrentMCTruth = mclist.at(i_mclist);

        MCTruth_CCNC.push_back(CurrentMCTruth->GetNeutrino().CCNC());
        MCTruth_Mode.push_back(CurrentMCTruth->GetNeutrino().Mode());

        //		MCTruth_Pt.push_back(CurrentMCTruth->GetNeutrino().Pt());
        //		MCTruth_QSqr.push_back(CurrentMCTruth->GetNeutrino().QSqr());
        //		MCTruth_Target.push_back(CurrentMCTruth->GetNeutrino().Target());
        //		MCTruth_Theta.push_back(CurrentMCTruth->GetNeutrino().Theta());
        //		MCTruth_Particle_Nu_E.push_back(CurrentMCTruth->GetNeutrino().Nu().E());
        //		MCTruth_Particle_Nu_Vx.push_back(CurrentMCTruth->GetNeutrino().Nu().Vx());
        //		MCTruth_Particle_Nu_Vy.push_back(CurrentMCTruth->GetNeutrino().Nu().Vy());
        //		MCTruth_Particle_Nu_Vz.push_back(CurrentMCTruth->GetNeutrino().Nu().Vz());
        MCTruth_X.push_back(CurrentMCTruth->GetNeutrino().X());
        MCTruth_Y.push_back(CurrentMCTruth->GetNeutrino().Y());
        MCTruth_W.push_back(CurrentMCTruth->GetNeutrino().W());
        MCTruth_InteractionType.push_back(CurrentMCTruth->GetNeutrino().InteractionType());

        MCTruth_NParticles.push_back(CurrentMCTruth->NParticles());
        MCTruth_Origin.push_back(CurrentMCTruth->Origin());

        int NMCTruthParticles = CurrentMCTruth->NParticles();
        std::vector<int> CurrentMCTruth_Particle_Pdg;
        std::vector<int> CurrentMCTruth_Particle_TrackId;
        std::vector<double> CurrentMCTruth_Particle_EndX;
        std::vector<double> CurrentMCTruth_Particle_EndY;
        std::vector<double> CurrentMCTruth_Particle_EndZ;
        std::vector<double> CurrentMCTruth_Particle_Vx;
        std::vector<double> CurrentMCTruth_Particle_Vy;
        std::vector<double> CurrentMCTruth_Particle_Vz;

        CurrentMCTruth_Particle_Pdg.clear();
        CurrentMCTruth_Particle_TrackId.clear();
        CurrentMCTruth_Particle_EndX.clear();
        CurrentMCTruth_Particle_EndY.clear();
        CurrentMCTruth_Particle_EndZ.clear();
        CurrentMCTruth_Particle_Vx.clear();
        CurrentMCTruth_Particle_Vy.clear();
        CurrentMCTruth_Particle_Vz.clear();

        for (int WhichParticle = 0; WhichParticle < NMCTruthParticles; WhichParticle++)
        {

            const simb::MCParticle &mcparticle(CurrentMCTruth->GetParticle(WhichParticle));
            CurrentMCTruth_Particle_Pdg.push_back(mcparticle.PdgCode());
            CurrentMCTruth_Particle_TrackId.push_back(mcparticle.TrackId());
            CurrentMCTruth_Particle_EndX.push_back(mcparticle.EndX());
            CurrentMCTruth_Particle_EndY.push_back(mcparticle.EndY());
            CurrentMCTruth_Particle_EndZ.push_back(mcparticle.EndZ());
            CurrentMCTruth_Particle_Vx.push_back(mcparticle.Vx());
            CurrentMCTruth_Particle_Vy.push_back(mcparticle.Vy());
            CurrentMCTruth_Particle_Vz.push_back(mcparticle.Vz());

        } // End of the loop over the MCParticles

        MCTruth_Particle_Pdg.push_back(CurrentMCTruth_Particle_Pdg);
        MCTruth_Particle_TrackId.push_back(CurrentMCTruth_Particle_TrackId);
        MCTruth_Particle_EndX.push_back(CurrentMCTruth_Particle_EndX);
        MCTruth_Particle_EndY.push_back(CurrentMCTruth_Particle_EndY);
        MCTruth_Particle_EndZ.push_back(CurrentMCTruth_Particle_EndZ);
        MCTruth_Particle_Vx.push_back(CurrentMCTruth_Particle_Vx);
        MCTruth_Particle_Vy.push_back(CurrentMCTruth_Particle_Vy);
        MCTruth_Particle_Vz.push_back(CurrentMCTruth_Particle_Vz);

    } // End of the loop over the mclist

    // _________________________________________________________________________________________________________________________________________________________________________________________

    //	// GTruth

    //	NumberGTruthEvents = gmclist.size();
    //	std::cout << std::endl << "Loop over the " << NumberGTruthEvents << " gtruth" << std::endl << std::endl;
    //	GTruth_DiffXsec.clear();
    //	GTruth_Xsec.clear();
    //	GTruth_NumNeutron.clear();
    //	GTruth_NumPi0.clear();
    //	GTruth_NumPiPlus.clear();
    //	GTruth_NumPiMinus.clear();
    //	GTruth_NumProton.clear();
    //	GTruth_VertexX.clear();
    //	GTruth_VertexY.clear();
    //	GTruth_VertexZ.clear();

    //	for (int i_gmclist = 0; i_gmclist < NumberGTruthEvents; i_gmclist ++ ) {

    //		art::Ptr<simb::GTruth> CurrentGTruth = gmclist.at(i_gmclist);

    //		GTruth_DiffXsec.push_back(CurrentGTruth->fDiffXsec);
    //		GTruth_Xsec.push_back(CurrentGTruth->fXsec);
    //		GTruth_NumNeutron.push_back(CurrentGTruth->fNumNeutron);
    //		GTruth_NumPi0.push_back(CurrentGTruth->fNumPi0);
    //		GTruth_NumPiPlus.push_back(CurrentGTruth->fNumPiPlus);
    //		GTruth_NumPiMinus.push_back(CurrentGTruth->fNumPiMinus);
    //		GTruth_NumProton.push_back(CurrentGTruth->fNumProton);
    //		GTruth_VertexX.push_back(CurrentGTruth->fVertex.X());
    //		GTruth_VertexY.push_back(CurrentGTruth->fVertex.Y());
    //		GTruth_VertexZ.push_back(CurrentGTruth->fVertex.Z());

    //	}

    // __________________________________________________________________________________________________________________________________________________________________________________________

    std::cout << std::endl
              << "I just finished processing event # " << NEvents << std::endl
              << std::endl;

    myTTree->Fill();

} // End of the analysis module

// __________________________________________________________________________________________________________________________________________________________________________________________________

//void mynamespace::TTreeCreator::endSubRun(const art::SubRun& sr){

//	// POT Summary

//	art::Handle<sumdata::POTSummary> potListHandle;
//	if (sr.getByLabel("generator",potListHandle)) { pot = potListHandle->totpot; }
//	else { pot = 0.; }

//	pot_total += pot;
//	POT = pot;
//	TotalPOT = pot_total;
//	myPOT->Fill();

//	std::cout << std::endl << "pot = " << pot << " pot_total  = " << pot_total << std::endl;

//}

// __________________________________________________________________________________________________________________________________________________________________________________________________

void mynamespace::TTreeCreator::endJob()
{

    TFile &file = tfs->file();
    file.cd();

    std::cout << std::endl
              << "The TTree has been created and contains " << NEvents << " events." << std::endl
              << std::endl
              << std::endl
              << std::endl;
}
//___________________________________________________________________________________________________________________________________________________________________________________________________
void mynamespace::TTreeCreator::endSubRun ( const art::SubRun &sr) {

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
//___________________________________________________________________________________________________________________________________________________________________________________________________

DEFINE_ART_MODULE(mynamespace::TTreeCreator)
