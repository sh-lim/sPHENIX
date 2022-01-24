#ifndef __MyAnaDstBasic_H__
#define __MyAnaDstBasic_H__

#include <fun4all/SubsysReco.h>
#include <string>

//Forward declerations
class PHCompositeNode;
class SvtxVertexMap;
class SvtxTrackMap;
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

	float vtx_reco[10][3];

  //Node pointers
	SvtxVertexMap *_vtxmap;
	SvtxTrackMap *_trkmap;

};

#endif //* __MyAnaDstBasic_H__ *//
