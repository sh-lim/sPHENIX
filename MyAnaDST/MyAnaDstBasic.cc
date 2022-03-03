////////////////////////////////////////////////////////////////////////////////
//
// This module is desgined to grab svtx tracks and put truth and cluster
// information into a TTree for GenFit testing
//
////////////////////////////////////////////////////////////////////////////////
//
// Darren McGlinchey
// 1 Apr 2016
//
////////////////////////////////////////////////////////////////////////////////


#include "MyAnaDstBasic.h"

#include <g4main/PHG4TruthInfoContainer.h>
#include <g4main/PHG4Particle.h>
#include <g4main/PHG4VtxPoint.h>

#include <phool/phool.h>
#include <phool/getClass.h>
#include <fun4all/PHTFileServer.h>
#include <fun4all/Fun4AllServer.h>

#include <phhepmc/PHHepMCGenEventMap.h>
#include <phhepmc/PHHepMCGenEvent.h>
#include <HepMC/GenEvent.h>
#include <HepMC/GenVertex.h>
#include <HepMC/GenParticle.h>

#include <trackbase_historic/SvtxVertexMap.h>
#include <trackbase_historic/SvtxVertex.h>
#include <trackbase_historic/SvtxTrackMap.h>
#include <trackbase_historic/SvtxTrack.h>

#include <g4eval/SvtxEvalStack.h>
#include <g4eval/SvtxTrackEval.h>
#include <g4eval/SvtxTruthEval.h>

#include <TTree.h>
#include <TFile.h>
#include <TVector3.h>

#include <iostream>

using namespace std;

//----------------------------------------------------------------------------//
//-- Constructor:
//--  simple initialization
//----------------------------------------------------------------------------//
MyAnaDstBasic::MyAnaDstBasic(const string &name):
  SubsysReco( name ),
  _events( NULL )
{
  //initialize
  _event = 0;
  _outfile = "MyAnaDstBasic.root";

	_vtxmap = NULL;
	_trkmap = NULL;
	_svtxevalstack = NULL;

	_truth_container = NULL;
	_hepmc_event_map = NULL;
	
}

//----------------------------------------------------------------------------//
//-- Init():
//--   Intialize all histograms, trees, and ntuples
//----------------------------------------------------------------------------//
int MyAnaDstBasic::Init(PHCompositeNode *topNode)
{
  cout << PHWHERE << " Openning file " << _outfile << endl;
	_file = new TFile(_outfile.c_str(), "RECREATE");

  // create TTree
  _events = new TTree("T", "T");

  _events->Branch("event",0,"event/I");

	_events->Branch("nvertex_reco",0,"nvertex_reco/I");
	_events->Branch("vtx_reco",0,"vtx_reco[nvertex_reco][3]/F");

	_events->Branch("nvertex_gen",0,"nvertex_gen/I");
	_events->Branch("vtx_gen",0,"vtx_gen[nvertex_gen][3]/F");

	_events->Branch("ntrack",0,"ntrack/I");
	_events->Branch("track_pt",0,"track_pt[ntrack]/F");
	_events->Branch("track_eta",0,"track_eta[ntrack]/F");
	_events->Branch("track_dca3d_xy",0,"track_dca3d_xy[ntrack]/F");
	_events->Branch("track_dca3d_z",0,"track_dca3d_z[ntrack]/F");

	_events->Branch("ntruth",0,"ntruth/I");
	_events->Branch("truth_pt",0,"truth_pt[ntruth]/F");
	_events->Branch("truth_eta",0,"truth_eta[ntruth]/F");

  return 0;
}

//----------------------------------------------------------------------------//
//-- process_event():
//--   Call user instructions for every event.
//--   This function contains the analysis structure.
//----------------------------------------------------------------------------//
int MyAnaDstBasic::process_event(PHCompositeNode *topNode)
{
  _event++;
  if (_event % 1000 == 0)
    cout << PHWHERE << "Events processed: " << _event << endl;

  GetNodes(topNode);

  if (!_svtxevalstack) {
    _svtxevalstack = new SvtxEvalStack(topNode);
    _svtxevalstack->set_strict(false);
		_svtxevalstack->next_event(topNode);
  } else {
    _svtxevalstack->next_event(topNode);
  }

  fill_tree(topNode);

  return 0;
}

//----------------------------------------------------------------------------//
//-- End():
//--   End method, wrap everything up
//----------------------------------------------------------------------------//
int MyAnaDstBasic::End(PHCompositeNode *topNode)
{

	cout << "-----MyAnaDstBasic::End------" << endl;

	_file->cd();
  _events->Write();
	_file->Close();

  return 0;
}


//----------------------------------------------------------------------------//
//-- fill_tree():
//--   Fill the trees with truth, track fit, and cluster information
//----------------------------------------------------------------------------//
void MyAnaDstBasic::fill_tree(PHCompositeNode *topNode)
{

  // Make sure to reset all the TTree variables before trying to set them.
  reset_variables();

	SvtxTruthEval *trutheval = _svtxevalstack->get_truth_eval();
	SvtxTrackEval *trackeval = _svtxevalstack->get_track_eval();

	int ntrack = 0;
	int ntruth = 0;
	int nvertex_reco = 0;
	int nvertex_gen = 0;

	// Reconstructed vertex
	if ( _vtxmap ){

		for (SvtxVertexMap::ConstIter iter=_vtxmap->begin(); iter!=_vtxmap->end(); ++iter){
			SvtxVertex *vtx = iter->second;

			vtx_reco[nvertex_reco][0] = vtx->get_x();
			vtx_reco[nvertex_reco][1] = vtx->get_y();
			vtx_reco[nvertex_reco][2] = vtx->get_z();

			nvertex_reco++;

			if ( nvertex_reco>=100 ) break;
		}

	}//vtxmap

	// Reconstructed track
	if ( _trkmap ){
		for (SvtxTrackMap::ConstIter iter=_trkmap->begin(); iter!=_trkmap->end(); ++iter){
			SvtxTrack *svtx_trk = iter->second;

			//truth matching
			PHG4Particle* g4particle = trackeval->max_truth_particle_by_nclusters(svtx_trk);
			if ( !g4particle ) continue;

			if ( trutheval->is_primary(g4particle)==0 ) continue;

			short pid = abs(g4particle->get_pid());
			if ( !(pid==211 || pid==321 || pid==2212) ) continue;
			//

			track_pt[ntrack] = svtx_trk->get_pt();
			track_eta[ntrack] = svtx_trk->get_eta();
			track_dca3d_xy[ntrack] = svtx_trk->get_dca3d_xy();
			track_dca3d_z[ntrack] = svtx_trk->get_dca3d_z();

			ntrack++;

			if ( ntrack>=1000 ) break;
		}
	}//trkmap

	if ( _hepmc_event_map ){
	//if ( 0 ){

		for (PHHepMCGenEventMap::ConstIter iter=_hepmc_event_map->begin(); iter!=_hepmc_event_map->end(); ++iter){
			const PHHepMCGenEvent *hepmc_event = iter->second;

			float xx = hepmc_event->get_collision_vertex().x();
			float yy = hepmc_event->get_collision_vertex().y();
			float zz = hepmc_event->get_collision_vertex().z();

			vtx_gen[nvertex_gen][0] = xx;
			vtx_gen[nvertex_gen][1] = yy;
			vtx_gen[nvertex_gen][2] = zz;

			nvertex_gen++;

			if ( nvertex_gen>=100 ) break;
		}
	}//hepmc_event_map

	// Truth track
	if ( _truth_container ){

		PHG4TruthInfoContainer::ConstRange range =  _truth_container->GetParticleRange();

		for (PHG4TruthInfoContainer::ConstIterator iter=range.first; iter!=range.second; ++iter)
		{

			PHG4Particle *g4particle = iter->second;

			if ( !g4particle ) continue;
			if ( trutheval->is_primary(g4particle)==0 ) continue;

			short pid = abs(g4particle->get_pid());
			if ( !(pid==211 || pid==321 || pid==2212) ) continue;

			PHG4VtxPoint* vtx = trutheval->get_vertex(g4particle);
			if ( !vtx ) continue;

			float gvx = vtx->get_x();
			float gvy = vtx->get_y();
			float gvz = vtx->get_z();

			float dist = sqrt(pow(gvx-vtx_gen[0][0],2) + pow(gvy-vtx_gen[0][1],2) + pow(gvz-vtx_gen[0][2],2));
			if ( dist>2 ) continue;

			float px = g4particle->get_px();
			float py = g4particle->get_py();
			float pz = g4particle->get_pz();

			TVector3 vec(px, py, pz);

			truth_pt[ntruth] = vec.Pt();
			truth_eta[ntruth] = vec.Eta();

			ntruth++;

			if ( ntruth>=1000 ) break;
		}

	}//truth_container

	int count = 0;

	((TBranch*) _events->GetListOfBranches()->At(count++))->SetAddress(&_event);

	((TBranch*) _events->GetListOfBranches()->At(count++))->SetAddress(&nvertex_reco);
	((TBranch*) _events->GetListOfBranches()->At(count++))->SetAddress(vtx_reco);

	((TBranch*) _events->GetListOfBranches()->At(count++))->SetAddress(&nvertex_gen);
	((TBranch*) _events->GetListOfBranches()->At(count++))->SetAddress(vtx_gen);

	((TBranch*) _events->GetListOfBranches()->At(count++))->SetAddress(&ntrack);
	((TBranch*) _events->GetListOfBranches()->At(count++))->SetAddress(track_pt);
	((TBranch*) _events->GetListOfBranches()->At(count++))->SetAddress(track_eta);
	((TBranch*) _events->GetListOfBranches()->At(count++))->SetAddress(track_dca3d_xy);
	((TBranch*) _events->GetListOfBranches()->At(count++))->SetAddress(track_dca3d_z);

	((TBranch*) _events->GetListOfBranches()->At(count++))->SetAddress(&ntruth);
	((TBranch*) _events->GetListOfBranches()->At(count++))->SetAddress(truth_pt);
	((TBranch*) _events->GetListOfBranches()->At(count++))->SetAddress(truth_eta);

	_events->Fill();

  return;

}

//----------------------------------------------------------------------------//
//-- reset_variables():
//--   Reset all the tree variables to their default values.
//--   Needs to be called at the start of every event
//----------------------------------------------------------------------------//
void MyAnaDstBasic::reset_variables()
{

	for (int ii=0; ii<100; ii++){
		for (int iv=0; iv<3; iv++){
			vtx_reco[ii][iv] =  vtx_gen[ii][iv] = -9999;
		}
	}

	for (int it=0; it<1000; it++){
		track_eta[it] = track_pt[it] = -9999;
		truth_eta[it] = truth_pt[it] = -9999;
	}

}

//----------------------------------------------------------------------------//
//-- GetNodes():
//--   Get all the all the required nodes off the node tree
//----------------------------------------------------------------------------//
void MyAnaDstBasic::GetNodes(PHCompositeNode * topNode)
{
  //DST objects

  //SvtxVertexMap
  _vtxmap = findNode::getClass<SvtxVertexMap>(topNode,"SvtxVertexMap");
  if (!_vtxmap && _event<2)
  {
    cout << PHWHERE << " SvtxVertexMap node not found on node tree" << endl;
  }

  //SvtxTrackMap
  _trkmap = findNode::getClass<SvtxTrackMap>(topNode,"SvtxTrackMap");
  if (!_trkmap && _event<2)
  {
    cout << PHWHERE << " SvtxTrackMap node not found on node tree" << endl;
  }

	//HepMCGenEventMap
	_hepmc_event_map = findNode::getClass<PHHepMCGenEventMap>(topNode,"PHHepMCGenEventMap");
	if (!_hepmc_event_map && _event<2)
	{
		cout << PHWHERE << " PHHepMCGenEventMap node not found on node tree" << endl;
	}

  //PHG4TruthInfoContainer
  _truth_container = findNode::getClass<PHG4TruthInfoContainer>(topNode,"G4TruthInfo");
  if (!_truth_container && _event<2)
  {
    cout << PHWHERE << " PHG4TruthInfoContainer node not found on node tree" << endl;
  }

}



