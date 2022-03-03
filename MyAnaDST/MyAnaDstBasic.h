#ifndef __MyAnaDstBasic_H__
#define __MyAnaDstBasic_H__

#include <fun4all/SubsysReco.h>
#include <string>

//Forward declerations
class PHCompositeNode;
class SvtxVertexMap;
class SvtxTrackMap;
class PHG4TruthInfoContainer;
class SvtxEvalStack;
class PHHepMCGenEventMap;
class TFile;
class TTree;


//Brief: basic ntuple and histogram creation for sim evaluation
class MyAnaDstBasic: public SubsysReco
{
 public: 
  //Default constructor
  MyAnaDstBasic(const std::string &name="MyAnaDstBasic");

  //Initialization, called for initialization
  int Init(PHCompositeNode *);

  //Process Event, called for each event
  int process_event(PHCompositeNode *);

  //End, write and close files
  int End(PHCompositeNode *);

  //Change output filename
  void set_filename(const char* file)
  { if(file) _outfile = file; }

  //User modules
  void fill_tree(PHCompositeNode*);
  void reset_variables();

 private:
  //output filename
  std::string _outfile;
   
  //Event counter
  int _event;

  //Get all the nodes
  void GetNodes(PHCompositeNode *);
  
  //flags
  unsigned int _flags;

	TFile *_file;

  //TTrees
  TTree* _events;
  int event;

	//SvtxTrack
	float track_pt[1000];
	float track_eta[1000];
	float track_dca3d_xy[1000];
	float track_dca3d_z[1000];

	float truth_pt[1000];
	float truth_eta[1000];

	float vtx_reco[100][3];
	float vtx_gen[100][3];

  //Node pointers
	SvtxVertexMap *_vtxmap;
	SvtxTrackMap *_trkmap;
	PHG4TruthInfoContainer *_truth_container;
	SvtxEvalStack *_svtxevalstack;
	PHHepMCGenEventMap *_hepmc_event_map;

};

#endif //* __MyAnaDstBasic_H__ *//
