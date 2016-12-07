// -*- C++ -*-
//
// Package:    bTaggingEffAnalyzer
// Class:      bTaggingEffAnalyzer
// 
/**\class bTaggingEffAnalyzer bTaggingEffAnalyzer.cc Analysis/EDSHyFT/plugins/bTaggingEffAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Dinko Ferencek
//         Created:  Thu Oct  4 20:25:54 CDT 2012
// $Id: bTaggingEffAnalyzer.cc,v 1.1.2.1 2012/10/05 06:11:47 ferencek Exp $
//
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH2D.h"

#include <string>
//
// class declaration
//

using namespace std;
using namespace edm;

class bTaggingEffAnalyzer : public edm::EDAnalyzer {
public:
  explicit bTaggingEffAnalyzer(const edm::ParameterSet&);
  ~bTaggingEffAnalyzer();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

  // ----------member data ---------------------------

  typedef std::vector<pat::Jet> PatJetCollection;
  
  edm::EDGetTokenT< PatJetCollection > t_jetsTag_;
  
  const edm::InputTag jetsTag;
  const std::string   discriminatorTag;
  const double  discriminatorValue;
  const int     ptNBins;
  const double  ptMin;
  const double  ptMax;
  const int     etaNBins;
  const double  etaMin;
  const double  etaMax;
  
  edm::Service<TFileService>  fs;
  TH2D  *h2_bTaggingEff_Denom_b;
  TH2D  *h2_bTaggingEff_Denom_c;
  TH2D  *h2_bTaggingEff_Denom_udsg;
  TH2D  *h2_bTaggingEff_Num_b;
  TH2D  *h2_bTaggingEff_Num_c;
  TH2D  *h2_bTaggingEff_Num_udsg;

};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
bTaggingEffAnalyzer::bTaggingEffAnalyzer(const edm::ParameterSet& iConfig):

  jetsTag(iConfig.getParameter<edm::InputTag>("JetsTag")),
  discriminatorTag(iConfig.getParameter<std::string>("DiscriminatorTag")),
  discriminatorValue(iConfig.getParameter<double>("DiscriminatorValue")),
  ptNBins(iConfig.getParameter<int>("PtNBins")),
  ptMin(iConfig.getParameter<double>("PtMin")),
  ptMax(iConfig.getParameter<double>("PtMax")),
  etaNBins(iConfig.getParameter<int>("EtaNBins")),
  etaMin(iConfig.getParameter<double>("EtaMin")),
  etaMax(iConfig.getParameter<double>("EtaMax"))
                                                                 
{
  t_jetsTag_ = consumes< PatJetCollection >( jetsTag );
  
  //now do what ever initialization is needed
  h2_bTaggingEff_Denom_b    = fs->make<TH2D>("h2_bTaggingEff_Denom_b", ";p_{T} [GeV];#eta", ptNBins, ptMin, ptMax, etaNBins, etaMin, etaMax);
  h2_bTaggingEff_Denom_c    = fs->make<TH2D>("h2_bTaggingEff_Denom_c", ";p_{T} [GeV];#eta", ptNBins, ptMin, ptMax, etaNBins, etaMin, etaMax);
  h2_bTaggingEff_Denom_udsg = fs->make<TH2D>("h2_bTaggingEff_Denom_udsg", ";p_{T} [GeV];#eta", ptNBins, ptMin, ptMax, etaNBins, etaMin, etaMax);
  h2_bTaggingEff_Num_b    = fs->make<TH2D>("h2_bTaggingEff_Num_b", ";p_{T} [GeV];#eta", ptNBins, ptMin, ptMax, etaNBins, etaMin, etaMax);
  h2_bTaggingEff_Num_c    = fs->make<TH2D>("h2_bTaggingEff_Num_c", ";p_{T} [GeV];#eta", ptNBins, ptMin, ptMax, etaNBins, etaMin, etaMax);
  h2_bTaggingEff_Num_udsg = fs->make<TH2D>("h2_bTaggingEff_Num_udsg", ";p_{T} [GeV];#eta", ptNBins, ptMin, ptMax, etaNBins, etaMin, etaMax);
}


bTaggingEffAnalyzer::~bTaggingEffAnalyzer()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void bTaggingEffAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle<PatJetCollection> jets;
  iEvent.getByToken(t_jetsTag_,jets ); 
  
  // loop over jets
  for(PatJetCollection::const_iterator it = jets->begin(); it != jets->end(); ++it)
    {
      int partonFlavor = it->partonFlavour();
      if( abs(partonFlavor)==5 )
	{
	  h2_bTaggingEff_Denom_b->Fill(it->pt(), it->eta());
	  if( it->bDiscriminator(discriminatorTag.c_str()) >= discriminatorValue ) h2_bTaggingEff_Num_b->Fill(it->pt(), it->eta());
	}
      else if( abs(partonFlavor)==4 )
	{
	  h2_bTaggingEff_Denom_c->Fill(it->pt(), it->eta());
	  if( it->bDiscriminator(discriminatorTag.c_str()) >= discriminatorValue ) h2_bTaggingEff_Num_c->Fill(it->pt(), it->eta());
	}
      else
	{
	  h2_bTaggingEff_Denom_udsg->Fill(it->pt(), it->eta());
	  if( it->bDiscriminator(discriminatorTag.c_str()) >= discriminatorValue ) h2_bTaggingEff_Num_udsg->Fill(it->pt(), it->eta());
	}
    }
}


// ------------ method called once each job just before starting event loop  ------------
void 
bTaggingEffAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
bTaggingEffAnalyzer::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
bTaggingEffAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
bTaggingEffAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
bTaggingEffAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
bTaggingEffAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
bTaggingEffAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(bTaggingEffAnalyzer);
