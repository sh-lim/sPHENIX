#include "Style.h"

void DrawAnaVertexV03(int grp=0){

	const bool bSAVE = false;

	gStyle->SetOptStat(0);

	//TFile *infile = new TFile(Form("outfile_AnaVertexV03_grp%d.root",grp),"read");
	TFile *infile = new TFile("outfile_AnaVertexV03_grp20_2.root","read");

	string tag = "PYTHIA8 HardQCD:all";
	if ( grp==41 ){
		tag = "PYTHIA8 HardQCD:ccbar";
	}else if ( grp==42 ){
		tag = "PYTHIA8 HardQCD:bbbar";
	}else if ( grp==43 ){
		tag = "PYTHIA8 HardQCD:all";
	}else if ( grp==44 ){
		tag = "PYTHIA8 HardQCD:bbbar";
	}else if ( grp==0 || grp==1 || grp==2 || grp==3 ){
		tag = "PYTHIA8 pp D0-filtered";
	}

	const int nMarker[4] = {24, 24, 24, 24};
	const int nColor[4] = {1, 2, 4, 6};

	TH2D *h2d_eta_pt_p_mvtx[4];
	TH2D *h2d_eta_pt_s_mvtx[4];

	TH1D *h1d_pt_p_mvtx[4];
	TH1D *h1d_pt_s_mvtx[4];

	TH1D *h1d_pt_p_mvtx_f[4];
	TH1D *h1d_pt_s_mvtx_f[4];

	TH1D *h1d_eta_p_mvtx[4];
	TH1D *h1d_eta_s_mvtx[4];

	TH1D *h1d_DL_mvtx[4];
	TH1D *h1d_DL_mvtx_f[4];

	TH2D *hvtx_res_acts[3];
	TH2D *hvtx_res_rave[3];
	TH2D *hvtx_res_rave_refit[3];

	for (int ii=0; ii<4; ii++){
		h2d_eta_pt_p_mvtx[ii] = (TH2D*)infile->Get(Form("h2d_eta_pt_p_mvtx%d",ii));
		h2d_eta_pt_s_mvtx[ii] = (TH2D*)infile->Get(Form("h2d_eta_pt_s_mvtx%d",ii));

		h2d_eta_pt_p_mvtx[ii]->Add(h2d_eta_pt_s_mvtx[ii]);

		h1d_DL_mvtx[ii] = (TH1D*)infile->Get(Form("h1d_DecayLength_mvtx%d",ii));
		h1d_DL_mvtx[ii]->Rebin(2);
		h1d_DL_mvtx_f[ii] = (TH1D*)h1d_DL_mvtx[ii]->Clone(Form("h1d_DecayLength_mvtx_f_%d",ii));

		TH1D *hprojx = (TH1D*)h2d_eta_pt_p_mvtx[ii]->ProjectionX();
		TH1D *hprojy = (TH1D*)h2d_eta_pt_p_mvtx[ii]->ProjectionY();

		int etabin_min = hprojx->FindBin(-1.0+0.01);
		int etabin_max = hprojx->FindBin(+1.0-0.01);

		h1d_pt_p_mvtx[ii] = (TH1D*)h2d_eta_pt_p_mvtx[ii]->ProjectionY(Form("h1d_pt_p_mvtx_%d",ii),etabin_min,etabin_max);
		h1d_pt_p_mvtx[ii]->Rebin(4);

		h1d_pt_s_mvtx[ii] = (TH1D*)h2d_eta_pt_s_mvtx[ii]->ProjectionY(Form("h1d_pt_s_mvtx_%d",ii),etabin_min,etabin_max);
		h1d_pt_s_mvtx[ii]->Rebin(4);

		h1d_pt_p_mvtx_f[ii] = (TH1D*)h2d_eta_pt_p_mvtx[ii]->ProjectionY(Form("h1d_pt_p_mvtx_f_%d",ii),etabin_min,etabin_max);
		h1d_pt_p_mvtx_f[ii]->Rebin(4);

		h1d_pt_s_mvtx_f[ii] = (TH1D*)h2d_eta_pt_s_mvtx[ii]->ProjectionY(Form("h1d_pt_s_mvtx_f_%d",ii),etabin_min,etabin_max);
		h1d_pt_s_mvtx_f[ii]->Rebin(4);

		int ptbin_min = hprojy->FindBin(1.0+0.01);
		int ptbin_max = hprojy->FindBin(10.0-0.01);

		h1d_eta_p_mvtx[ii] = (TH1D*)h2d_eta_pt_p_mvtx[ii]->ProjectionX(Form("h1d_eta_p_mvtx_%d",ii),ptbin_min,ptbin_max);
		h1d_eta_s_mvtx[ii] = (TH1D*)h2d_eta_pt_s_mvtx[ii]->ProjectionX(Form("h1d_eta_s_mvtx_%d",ii),ptbin_min,ptbin_max);
		/*
		*/
	}

	for (int jj=0; jj<3; jj++){
		hvtx_res_acts[jj] = (TH2D*)infile->Get(Form("hvtx_res_acts_%d",jj));
		hvtx_res_rave[jj] = (TH2D*)infile->Get(Form("hvtx_res_rave_%d",jj));
		hvtx_res_rave_refit[jj] = (TH2D*)infile->Get(Form("hvtx_res_rave_refit_%d",jj));
	}

	TH1D *h1d_pt_p_combined = (TH1D*)h1d_pt_p_mvtx[0]->Clone("h1d_pt_p_combined");
	h1d_pt_p_combined->Add(h1d_pt_p_mvtx[1]);
	h1d_pt_p_combined->Add(h1d_pt_p_mvtx[2]);
	h1d_pt_p_combined->Add(h1d_pt_p_mvtx[3]);

	TH1D *h1d_pt_s_combined = (TH1D*)h1d_pt_s_mvtx[0]->Clone("h1d_pt_s_combined");
	h1d_pt_s_combined->Add(h1d_pt_s_mvtx[1]);
	h1d_pt_s_combined->Add(h1d_pt_s_mvtx[2]);
	h1d_pt_s_combined->Add(h1d_pt_s_mvtx[3]);

	TH1D *h1d_eta_p_combined = (TH1D*)h1d_eta_p_mvtx[0]->Clone("h1d_eta_p_combined");
	h1d_eta_p_combined->Add(h1d_eta_p_mvtx[1]);
	h1d_eta_p_combined->Add(h1d_eta_p_mvtx[2]);
	h1d_eta_p_combined->Add(h1d_eta_p_mvtx[3]);

	TH1D *h1d_eta_s_combined = (TH1D*)h1d_eta_s_mvtx[0]->Clone("h1d_eta_s_combined");
	h1d_eta_s_combined->Add(h1d_eta_s_mvtx[1]);
	h1d_eta_s_combined->Add(h1d_eta_s_mvtx[2]);
	h1d_eta_s_combined->Add(h1d_eta_s_mvtx[3]);

	TH1D *h1d_DL_combined = (TH1D*)h1d_DL_mvtx[0]->Clone("h1d_DL_combined");
	h1d_DL_combined->Add(h1d_DL_mvtx[1]);
	h1d_DL_combined->Add(h1d_DL_mvtx[2]);
	h1d_DL_combined->Add(h1d_DL_mvtx[3]);

	for (int ii=0; ii<4; ii++){
		h1d_pt_p_mvtx_f[ii]->Divide(h1d_pt_p_combined);
		h1d_pt_p_mvtx_f[ii]->SetMarkerStyle(nMarker[ii]);
		h1d_pt_p_mvtx_f[ii]->SetMarkerColor(nColor[ii]);
		h1d_pt_p_mvtx_f[ii]->SetLineColor(nColor[ii]);

		h1d_pt_p_mvtx[ii]->SetMarkerStyle(nMarker[ii]);
		h1d_pt_p_mvtx[ii]->SetMarkerColor(nColor[ii]);
		h1d_pt_p_mvtx[ii]->SetLineColor(nColor[ii]);

		h1d_pt_s_mvtx_f[ii]->Divide(h1d_pt_s_combined);
		h1d_pt_s_mvtx_f[ii]->SetMarkerStyle(nMarker[ii]);
		h1d_pt_s_mvtx_f[ii]->SetMarkerColor(nColor[ii]);
		h1d_pt_s_mvtx_f[ii]->SetLineColor(nColor[ii]);

		h1d_pt_s_mvtx[ii]->SetMarkerStyle(nMarker[ii]);
		h1d_pt_s_mvtx[ii]->SetMarkerColor(nColor[ii]);
		h1d_pt_s_mvtx[ii]->SetLineColor(nColor[ii]);

		h1d_eta_p_mvtx[ii]->Divide(h1d_eta_p_combined);
		h1d_eta_p_mvtx[ii]->SetMarkerStyle(nMarker[ii]);
		h1d_eta_p_mvtx[ii]->SetMarkerColor(nColor[ii]);
		h1d_eta_p_mvtx[ii]->SetLineColor(nColor[ii]);

		h1d_eta_s_mvtx[ii]->Divide(h1d_eta_s_combined);
		h1d_eta_s_mvtx[ii]->SetMarkerStyle(nMarker[ii]);
		h1d_eta_s_mvtx[ii]->SetMarkerColor(nColor[ii]);
		h1d_eta_s_mvtx[ii]->SetLineColor(nColor[ii]);

		h1d_DL_mvtx[ii]->SetMarkerStyle(nMarker[ii]);
		h1d_DL_mvtx[ii]->SetMarkerColor(nColor[ii]);
		h1d_DL_mvtx[ii]->SetLineColor(nColor[ii]);

		h1d_DL_mvtx_f[ii]->Divide(h1d_DL_combined);
		h1d_DL_mvtx_f[ii]->SetMarkerStyle(nMarker[ii]);
		h1d_DL_mvtx_f[ii]->SetMarkerColor(nColor[ii]);
		h1d_DL_mvtx_f[ii]->SetLineColor(nColor[ii]);

		for (int ibin=0; ibin<h1d_DL_mvtx_f[ii]->GetNbinsX(); ibin++){
			h1d_DL_mvtx_f[ii]->SetBinError(ibin+1, 0);
		}
	}

	TH2D *h2d_ntrk_evt_vtx_acts = (TH2D*)infile->Get("h2d_ntrk_evt_vtx_acts");
	TH2D *h2d_ntrk_evt_vtx_rave = (TH2D*)infile->Get("h2d_ntrk_evt_vtx_rave");
	TH2D *h2d_ntrk_evt_vtx_rave_refit = (TH2D*)infile->Get("h2d_ntrk_evt_vtx_rave_refit");

	TCanvas *c0 = new TCanvas("c0","c0",1.1*3*400,400);
	c0->Divide(3,1);

	/*
	{
		c0->cd(1);
		SetPadStyle();
		gPad->SetLogy();

		htmp = (TH1D*)gPad->DrawFrame(0,1,10,5.0*h1d_pt_p_combined->GetMaximum());
		SetHistoStyle("p_{T} [GeV/c]","N_{track} / 0.2 GeV/c");

		h1d_pt_p_mvtx[0]->Draw("p same");
		h1d_pt_p_mvtx[1]->Draw("p same");
		h1d_pt_p_mvtx[2]->Draw("p same");
		h1d_pt_p_mvtx[3]->Draw("p same");

		TLegend *leg = new TLegend(0.5,0.63,0.9,0.93);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.045);
		leg->AddEntry("",tag.c_str(),"h");
		leg->AddEntry("","|#eta|<1","h");
		leg->AddEntry(h1d_pt_p_mvtx[0],"0 MVTX cluster","P");
		leg->AddEntry(h1d_pt_p_mvtx[1],"1 MVTX cluster","P");
		leg->AddEntry(h1d_pt_p_mvtx[2],"2 MVTX cluster","P");
		leg->AddEntry(h1d_pt_p_mvtx[3],"3 MVTX cluster","P");
		leg->Draw();
	}
	*/

	{
		c0->cd(1);
		SetPadStyle();

		htmp = (TH1D*)gPad->DrawFrame(0,0,10,1.6);
		SetHistoStyle("p_{T} [GeV/c]","Fraction");

		h1d_pt_p_mvtx_f[0]->Draw("p same");
		h1d_pt_p_mvtx_f[1]->Draw("p same");
		h1d_pt_p_mvtx_f[2]->Draw("p same");
		h1d_pt_p_mvtx_f[3]->Draw("p same");

		TLegend *leg = new TLegend(0.5,0.63,0.9,0.93);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.045);
		leg->AddEntry("",tag.c_str(),"h");
		leg->AddEntry("","|#eta|<1","h");
		leg->AddEntry(h1d_pt_p_mvtx[0],"0 MVTX cluster","P");
		leg->AddEntry(h1d_pt_p_mvtx[1],"1 MVTX cluster","P");
		leg->AddEntry(h1d_pt_p_mvtx[2],"2 MVTX cluster","P");
		leg->AddEntry(h1d_pt_p_mvtx[3],"3 MVTX cluster","P");
		leg->Draw();
	}

	{
		c0->cd(2);
		SetPadStyle();

		htmp = (TH1D*)gPad->DrawFrame(-1,0,1,1.6);
		SetHistoStyle("#eta","Fraction");

		h1d_eta_p_mvtx[0]->Draw("p same");
		h1d_eta_p_mvtx[1]->Draw("p same");
		h1d_eta_p_mvtx[2]->Draw("p same");
		h1d_eta_p_mvtx[3]->Draw("p same");

		TLegend *leg = new TLegend(0.5,0.63,0.9,0.93);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.045);
		leg->AddEntry("",tag.c_str(),"h");
		leg->AddEntry("","p_{T}>1 GeV/c","h");
		leg->AddEntry(h1d_pt_p_mvtx[0],"0 MVTX cluster","P");
		leg->AddEntry(h1d_pt_p_mvtx[1],"1 MVTX cluster","P");
		leg->AddEntry(h1d_pt_p_mvtx[2],"2 MVTX cluster","P");
		leg->AddEntry(h1d_pt_p_mvtx[3],"3 MVTX cluster","P");
		leg->Draw();
	}

	{
		c0->cd(3);
		SetPadStyle();

		htmp = (TH1D*)gPad->DrawFrame(0,0,20,1.6);
		SetHistoStyle("Distance from truth event vertex [mm]","Fraction");

		h1d_DL_mvtx_f[0]->Draw("same");
		h1d_DL_mvtx_f[1]->Draw("same");
		h1d_DL_mvtx_f[2]->Draw("same");
		h1d_DL_mvtx_f[3]->Draw("same");

		TLegend *leg = new TLegend(0.5,0.63,0.9,0.93);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.045);
		leg->AddEntry("",tag.c_str(),"h");
		leg->AddEntry("","p_{T}>1 GeV/c, |#eta|<1","h");
		leg->AddEntry(h1d_DL_mvtx[0],"0 MVTX cluster","L");
		leg->AddEntry(h1d_DL_mvtx[1],"1 MVTX cluster","L");
		leg->AddEntry(h1d_DL_mvtx[2],"2 MVTX cluster","L");
		leg->AddEntry(h1d_DL_mvtx[3],"3 MVTX cluster","L");
		leg->Draw();
	}

	/*
	TCanvas *c1 = new TCanvas("c1","c1",1.1*3*400,400);
	c1->Divide(3,1);

	{
		c1->cd(1);
		SetPadStyle();
		gPad->SetLogy();

		htmp = (TH1D*)gPad->DrawFrame(0,1,10,2.0*h1d_pt_p_combined->GetMaximum());
		SetHistoStyle("p_{T} [GeV/c]","N_{track} / 0.2 GeV/c");

		h1d_pt_s_mvtx[0]->Draw("p same");
		h1d_pt_s_mvtx[1]->Draw("p same");
		h1d_pt_s_mvtx[2]->Draw("p same");
		h1d_pt_s_mvtx[3]->Draw("p same");

		TLegend *leg = new TLegend(0.55,0.7,0.9,0.93);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.045);
		leg->AddEntry("","|#eta|<1","h");
		leg->AddEntry(h1d_pt_s_mvtx[0],"0 MVTX cluster","P");
		leg->AddEntry(h1d_pt_s_mvtx[1],"1 MVTX cluster","P");
		leg->AddEntry(h1d_pt_s_mvtx[2],"2 MVTX cluster","P");
		leg->AddEntry(h1d_pt_s_mvtx[3],"3 MVTX cluster","P");
		leg->Draw();
	}

	{
		c1->cd(2);
		SetPadStyle();

		htmp = (TH1D*)gPad->DrawFrame(0,0,10,1.4);
		SetHistoStyle("p_{T} [GeV/c]","Fraction");

		h1d_pt_s_mvtx_f[0]->Draw("p same");
		h1d_pt_s_mvtx_f[1]->Draw("p same");
		h1d_pt_s_mvtx_f[2]->Draw("p same");
		h1d_pt_s_mvtx_f[3]->Draw("p same");

		TLegend *leg = new TLegend(0.55,0.7,0.9,0.93);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.045);
		leg->AddEntry("","|#eta|<1","h");
		leg->AddEntry(h1d_pt_s_mvtx[0],"0 MVTX cluster","P");
		leg->AddEntry(h1d_pt_s_mvtx[1],"1 MVTX cluster","P");
		leg->AddEntry(h1d_pt_s_mvtx[2],"2 MVTX cluster","P");
		leg->AddEntry(h1d_pt_s_mvtx[3],"3 MVTX cluster","P");
		leg->Draw();
	}

	{
		c1->cd(3);
		SetPadStyle();

		htmp = (TH1D*)gPad->DrawFrame(-1,0,1,1.4);
		SetHistoStyle("#eta","Fraction");

		h1d_eta_s_mvtx[0]->Draw("p same");
		h1d_eta_s_mvtx[1]->Draw("p same");
		h1d_eta_s_mvtx[2]->Draw("p same");
		h1d_eta_s_mvtx[3]->Draw("p same");

		TLegend *leg = new TLegend(0.55,0.7,0.9,0.93);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.045);
		leg->AddEntry("","p_{T}>1 GeV/c","h");
		leg->AddEntry(h1d_pt_p_mvtx[0],"0 MVTX cluster","P");
		leg->AddEntry(h1d_pt_p_mvtx[1],"1 MVTX cluster","P");
		leg->AddEntry(h1d_pt_p_mvtx[2],"2 MVTX cluster","P");
		leg->AddEntry(h1d_pt_p_mvtx[3],"3 MVTX cluster","P");
		leg->Draw();
	}
	*/


	/*
	TCanvas *c2 = new TCanvas("c2","c2",1.1*2*400,400);
	c2->Divide(2,1);

	{
		c2->cd(1);
		SetPadStyle();
		gPad->SetLogy();

		htmp = (TH1D*)gPad->DrawFrame(0,1,20,2.0*h1d_DL_combined->GetMaximum());
		SetHistoStyle("Distance from truth event vertex [mm]","N_{track} / 0.2 mm");

		h1d_DL_mvtx[0]->Draw("same");
		h1d_DL_mvtx[1]->Draw("same");
		h1d_DL_mvtx[2]->Draw("same");
		h1d_DL_mvtx[3]->Draw("same");

		TLegend *leg = new TLegend(0.5,0.63,0.9,0.93);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.045);
		leg->AddEntry("",tag.c_str(),"h");
		leg->AddEntry("","p_{T}>1 GeV/c, |#eta|<1","h");
		leg->AddEntry(h1d_DL_mvtx[0],"0 MVTX cluster","L");
		leg->AddEntry(h1d_DL_mvtx[1],"1 MVTX cluster","L");
		leg->AddEntry(h1d_DL_mvtx[2],"2 MVTX cluster","L");
		leg->AddEntry(h1d_DL_mvtx[3],"3 MVTX cluster","L");
		leg->Draw();
	}

	{
		c2->cd(2);
		SetPadStyle();

		htmp = (TH1D*)gPad->DrawFrame(0,0,20,1.4);
		SetHistoStyle("Distance from truth event vertex [mm]","Fraction");

		h1d_DL_mvtx_f[0]->Draw("same");
		h1d_DL_mvtx_f[1]->Draw("same");
		h1d_DL_mvtx_f[2]->Draw("same");
		h1d_DL_mvtx_f[3]->Draw("same");

		TLegend *leg = new TLegend(0.5,0.63,0.9,0.93);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.045);
		leg->AddEntry("",tag.c_str(),"h");
		leg->AddEntry("","p_{T}>1 GeV/c, |#eta|<1","h");
		leg->AddEntry(h1d_DL_mvtx[0],"0 MVTX cluster","L");
		leg->AddEntry(h1d_DL_mvtx[1],"1 MVTX cluster","L");
		leg->AddEntry(h1d_DL_mvtx[2],"2 MVTX cluster","L");
		leg->AddEntry(h1d_DL_mvtx[3],"3 MVTX cluster","L");
		leg->Draw();
	}
	*/

	TCanvas *c3 = new TCanvas("c3","c3",1.1*3*500,2*500);
	c3->Divide(3,2);
	{
		c3->cd(1);
		SetPadStyle(1);
		gPad->SetLogz();

		htmp = (TH1D*)hvtx_res_acts[2];
		SetHistoStyle("Number of reconstrcuted tracks","z_{Reco} - z_{Truth} [cm]");

		hvtx_res_acts[2]->SetAxisRange(0,50,"X");
		hvtx_res_acts[2]->Draw("colz");

		TLegend *leg = new TLegend(0.45,0.83,0.9,0.93);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.045);
		leg->AddEntry("",tag.c_str(),"h");
		leg->AddEntry("","ACTS","h");
		leg->Draw();

		c3->cd(4);
		SetPadStyle(1);
		gPad->SetLogz();

		h2d_ntrk_evt_vtx_acts->SetAxisRange(0,50,"X");
		h2d_ntrk_evt_vtx_acts->SetAxisRange(0,50,"Y");

		htmp = (TH1D*)h2d_ntrk_evt_vtx_acts;
		SetHistoStyle("Number of reconstrcuted tracks","Number of associated tracks to vertex");
		h2d_ntrk_evt_vtx_acts->Draw("colz");

		leg->Draw();

		c3->cd(2);
		SetPadStyle(1);
		gPad->SetLogz();

		htmp = (TH1D*)hvtx_res_rave[2];
		SetHistoStyle("Number of reconstructed tracks","z_{Reco} - z_{Truth} [cm]");

		hvtx_res_rave[2]->SetAxisRange(0,50,"X");
		hvtx_res_rave[2]->Draw("colz");

		leg = new TLegend(0.45,0.83,0.9,0.93);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.045);
		leg->AddEntry("",tag.c_str(),"h");
		leg->AddEntry("","RAVE","h");
		leg->Draw();

		c3->cd(5);
		SetPadStyle(1);
		gPad->SetLogz();

		h2d_ntrk_evt_vtx_rave->SetAxisRange(0,50,"X");
		h2d_ntrk_evt_vtx_rave->SetAxisRange(0,50,"Y");

		htmp = (TH1D*)h2d_ntrk_evt_vtx_rave;
		SetHistoStyle("Number of reconstrcuted tracks","Number of associated tracks to vertex");
		h2d_ntrk_evt_vtx_rave->Draw("colz");

		leg->Draw();

		c3->cd(3);
		SetPadStyle(1);
		gPad->SetLogz();

		htmp = (TH1D*)hvtx_res_rave_refit[2];
		SetHistoStyle("Number of reconstructed tracks","z_{Reco} - z_{Truth} [cm]");

		hvtx_res_rave_refit[2]->SetAxisRange(0,50,"X");
		hvtx_res_rave_refit[2]->Draw("colz");

		leg = new TLegend(0.45,0.83,0.9,0.93);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.045);
		leg->AddEntry("",tag.c_str(),"h");
		leg->AddEntry("","RAVE w/ MVTX","h");
		leg->Draw();

		c3->cd(6);
		SetPadStyle(1);
		gPad->SetLogz();

		h2d_ntrk_evt_vtx_rave_refit->SetAxisRange(0,50,"X");
		h2d_ntrk_evt_vtx_rave_refit->SetAxisRange(0,50,"Y");

		htmp = (TH1D*)h2d_ntrk_evt_vtx_rave_refit;
		SetHistoStyle("Number of reconstrcuted tracks","Number of associated tracks to vertex");
		h2d_ntrk_evt_vtx_rave_refit->Draw("colz");

		leg->Draw();
	}

	TCanvas *c4 = new TCanvas("c4","c4",1.1*1*500,1*500);
	{
		SetPadStyle();
		gPad->SetLogy();

		TH1D *hnvertex_reco = (TH1D*)infile->Get("hnvertex_reco");
		htmp = (TH1D*)hnvertex_reco;
		SetHistoStyle("Number of vertices per event","");

		hnvertex_reco->SetAxisRange(0,10);
		hnvertex_reco->Draw();

		TLegend *leg = new TLegend(0.5,0.78,0.9,0.93);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.045);
		leg->AddEntry("",tag.c_str(),"h");
		leg->AddEntry("","ACTS","h");
		leg->Draw();
	}

	TH1D *h1vtx_res_acts_z = hvtx_res_acts[2]->ProjectionY("h1vtx_res_acts_z",9,13);
	TH1D *h1vtx_res_rave_z = hvtx_res_rave[2]->ProjectionY("h1vtx_res_rave_z",9,13);
	TH1D *h1vtx_res_rave_refit_z = hvtx_res_rave_refit[2]->ProjectionY("h1vtx_res_rave_refit_z",9,13);

	h1vtx_res_acts_z->Rebin(4);
	h1vtx_res_rave_z->Rebin(4);
	h1vtx_res_rave_refit_z->Rebin(4);

	TCanvas *c5 = new TCanvas("c5","c5",1.1*1*500,1*500);
	{
		SetPadStyle();
		gPad->SetLogy();

		htmp = (TH1D*)gPad->DrawFrame(-0.3,1,0.3,5*h1vtx_res_rave_refit_z->GetMaximum());
		SetHistoStyle("z_{Reco} - z_{Truth} [mm]","");

		h1vtx_res_acts_z->SetLineColor(1);
		h1vtx_res_acts_z->Draw("same");

		h1vtx_res_rave_z->SetLineColor(2);
		h1vtx_res_rave_z->Draw("same");

		h1vtx_res_rave_refit_z->SetLineColor(4);
		h1vtx_res_rave_refit_z->Draw("same");


		TLegend *leg = new TLegend(0.2,0.73,0.5,0.93);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.035);
		leg->AddEntry("",tag.c_str(),"h");
		leg->AddEntry("","8#leqN_{reco track}#leq12","h");
		leg->AddEntry(h1vtx_res_acts_z,"ACTS","L");
		leg->AddEntry(h1vtx_res_rave_z,"RAVE","L");
		leg->AddEntry(h1vtx_res_rave_refit_z,"RAVE w/ MVTX","L");
		leg->Draw();
	}

	/*
	if ( bSAVE ){
		c0->cd();
		c0->SaveAs(Form("AnaVertexV02_grp%d_c0.eps",grp));

		c2->cd();
		c2->SaveAs(Form("AnaVertexV02_grp%d_c2.eps",grp));

		c3->cd();
		c3->SaveAs(Form("AnaVertexV02_grp%d_c3.eps",grp));
	}
	*/

}
