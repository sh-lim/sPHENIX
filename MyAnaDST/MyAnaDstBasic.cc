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

#include <phool/phool.h>
#include <phool/getClass.h>
#include <fun4all/PHTFileServer.h>
#include <fun4all/Fun4AllServer.h>

#include <trackbase_historic/SvtxVertexMap.h>
#include <trackbase_historic/SvtxVertex.h>
#include <trackbase_historic/SvtxTrackMap.h>
#include <trackbase_historic/SvtxTrack.h>

#include <TTree.h>
#include <TFile.h>

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

	_events->Branch("nvertex",0,"nvertex/I");
	_events->Branch("vtx_reco",0,"vtx_reco[nvertex][3]/F");

	_events->Branch("ntrack",0,"ntrack/I");
	_events->Branch("track_pt",0,"track_pt[ntrack]/F");
	_events->Branch("track_eta",0,"track_eta[ntrack]/F");

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

	int ntrack = 0;
	int nvertex = 0;

	if ( _vtxmap ){

		for (SvtxVertexMap::ConstIter iter=_vtxmap->begin(); iter!=_vtxmap->end(); ++iter){
			SvtxVertex *vtx = iter->second;

			vtx_reco[nvertex][0] = vtx->get_x();
			vtx_reco[nvertex][1] = vtx->get_y();
			vtx_reco[nvertex][2] = vtx->get_z();

			nvertex++;

			if ( nvertex>=10 ) break;
		}

	}//vtxmap

	if ( _trkmap ){
		for (SvtxTrackMap::ConstIter iter=_trkmap->begin(); iter!=_trkmap->end(); ++iter){
			SvtxTrack *svtx_trk = iter->second;

			track_pt[ntrack] = svtx_trk->get_pt();
			track_eta[ntrack] = svtx_trk->get_eta();

			ntrack++;

			if ( ntrack>=1000 ) break;
		}
	}

	int count = 0;

	((TBranch*) _events->GetListOfBranches()->At(count++))->SetAddress(&_event);

	((TBranch*) _events->GetListOfBranches()->At(count++))->SetAddress(&nvertex);
	((TBranch*) _events->GetListOfBranches()->At(count++))->SetAddress(vtx_reco);

	((TBranch*) _events->GetListOfBranches()->At(count++))->SetAddress(&ntrack);
	((TBranch*) _events->GetListOfBranches()->At(count++))->SetAddress(track_pt);
	((TBranch*) _events->GetListOfBranches()->At(count++))->SetAddress(track_eta);

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

	for (int ii=0; ii<10; ii++){
		for (int iv=0; iv<3; iv++){
			vtx_reco[ii][iv] = -9999;
		}
	}

	for (int it=0; it<1000; it++){
		track_eta[it] = track_pt[it] = -9999;
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

}



