#include "Style.h"

void DrawAnaVertex(){

	gStyle->SetOptStat(0);

	//TFile *infile = new TFile("outfile_AnaVertex.root","read");
	TFile *infile = new TFile("outfile_AnaVertex_jetty.root","read");

	const int nMarker[4] = {24, 24, 24, 24};
	const int nColor[4] = {1, 2, 4, 6};

	TH2D *h2d_eta_pt_mvtx[4];
	TH1D *h1d_eta_mvtx[4];
	TH1D *h1d_pt_mvtx[4];
	TH1D *h1d_pt_mvtx_f[4];

	for (int ii=0; ii<4; ii++){
		h2d_eta_pt_mvtx[ii] = (TH2D*)infile->Get(Form("h2d_eta_pt_mvtx%d",ii));

		TH1D *hprojx = (TH1D*)h2d_eta_pt_mvtx[ii]->ProjectionX();
		TH1D *hprojy = (TH1D*)h2d_eta_pt_mvtx[ii]->ProjectionY();

		int etabin_min = hprojx->FindBin(-1.0+0.01);
		int etabin_max = hprojx->FindBin(+1.0-0.01);

		h1d_pt_mvtx[ii] = (TH1D*)h2d_eta_pt_mvtx[ii]->ProjectionY(Form("h1d_pt_mvtx_%d",ii),etabin_min,etabin_max);
		h1d_pt_mvtx[ii]->Rebin();

		h1d_pt_mvtx_f[ii] = (TH1D*)h2d_eta_pt_mvtx[ii]->ProjectionY(Form("h1d_pt_mvtx_f_%d",ii),etabin_min,etabin_max);
		h1d_pt_mvtx_f[ii]->Rebin();

		int ptbin_min = hprojy->FindBin(1.0+0.01);
		int ptbin_max = hprojy->FindBin(10.0-0.01);

		h1d_eta_mvtx[ii] = (TH1D*)h2d_eta_pt_mvtx[ii]->ProjectionX(Form("h1d_eta_mvtx_%d",ii),ptbin_min,ptbin_max);
	}

	TH1D *h1d_pt_combined = (TH1D*)h1d_pt_mvtx[0]->Clone("h1d_pt_combined");
	h1d_pt_combined->Add(h1d_pt_mvtx[1]);
	h1d_pt_combined->Add(h1d_pt_mvtx[2]);
	h1d_pt_combined->Add(h1d_pt_mvtx[3]);

	TH1D *h1d_eta_combined = (TH1D*)h1d_eta_mvtx[0]->Clone("h1d_eta_combined");
	h1d_eta_combined->Add(h1d_eta_mvtx[1]);
	h1d_eta_combined->Add(h1d_eta_mvtx[2]);
	h1d_eta_combined->Add(h1d_eta_mvtx[3]);

	for (int ii=0; ii<4; ii++){
		h1d_pt_mvtx_f[ii]->Divide(h1d_pt_combined);
		h1d_pt_mvtx_f[ii]->SetMarkerStyle(nMarker[ii]);
		h1d_pt_mvtx_f[ii]->SetMarkerColor(nColor[ii]);
		h1d_pt_mvtx_f[ii]->SetLineColor(nColor[ii]);

		h1d_pt_mvtx[ii]->SetMarkerStyle(nMarker[ii]);
		h1d_pt_mvtx[ii]->SetMarkerColor(nColor[ii]);
		h1d_pt_mvtx[ii]->SetLineColor(nColor[ii]);

		h1d_eta_mvtx[ii]->Divide(h1d_eta_combined);
		h1d_eta_mvtx[ii]->SetMarkerStyle(nMarker[ii]);
		h1d_eta_mvtx[ii]->SetMarkerColor(nColor[ii]);
		h1d_eta_mvtx[ii]->SetLineColor(nColor[ii]);
	}

	TCanvas *c0 = new TCanvas("c0","c0",1.1*3*400,400);
	c0->Divide(3,1);

	{
		c0->cd(1);
		SetPadStyle();
		gPad->SetLogy();

		htmp = (TH1D*)gPad->DrawFrame(0,1,10,2.0*h1d_pt_combined->GetMaximum());
		SetHistoStyle("p_{T} [GeV/c]","N_{track} / 0.2 GeV/c");

		h1d_pt_mvtx[0]->Draw("p same");
		h1d_pt_mvtx[1]->Draw("p same");
		h1d_pt_mvtx[2]->Draw("p same");
		h1d_pt_mvtx[3]->Draw("p same");

		TLegend *leg = new TLegend(0.55,0.7,0.9,0.93);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.045);
		leg->AddEntry("","|#eta|<1","h");
		leg->AddEntry(h1d_pt_mvtx[0],"0 MVTX cluster","P");
		leg->AddEntry(h1d_pt_mvtx[1],"1 MVTX cluster","P");
		leg->AddEntry(h1d_pt_mvtx[2],"2 MVTX cluster","P");
		leg->AddEntry(h1d_pt_mvtx[3],"3 MVTX cluster","P");
		leg->Draw();
	}

	{
		c0->cd(2);
		SetPadStyle();

		htmp = (TH1D*)gPad->DrawFrame(0,0,10,1.3);
		SetHistoStyle("p_{T} [GeV/c]","Fraction");

		h1d_pt_mvtx_f[0]->Draw("p same");
		h1d_pt_mvtx_f[1]->Draw("p same");
		h1d_pt_mvtx_f[2]->Draw("p same");
		h1d_pt_mvtx_f[3]->Draw("p same");

		TLegend *leg = new TLegend(0.55,0.7,0.9,0.93);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.045);
		leg->AddEntry("","|#eta|<1","h");
		leg->AddEntry(h1d_pt_mvtx[0],"0 MVTX cluster","P");
		leg->AddEntry(h1d_pt_mvtx[1],"1 MVTX cluster","P");
		leg->AddEntry(h1d_pt_mvtx[2],"2 MVTX cluster","P");
		leg->AddEntry(h1d_pt_mvtx[3],"3 MVTX cluster","P");
		leg->Draw();
	}

	{
		c0->cd(3);
		SetPadStyle();

		htmp = (TH1D*)gPad->DrawFrame(-1,0,1,1.3);
		SetHistoStyle("#eta","Fraction");

		h1d_eta_mvtx[0]->Draw("p same");
		h1d_eta_mvtx[1]->Draw("p same");
		h1d_eta_mvtx[2]->Draw("p same");
		h1d_eta_mvtx[3]->Draw("p same");

		TLegend *leg = new TLegend(0.55,0.7,0.9,0.93);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.045);
		leg->AddEntry("","p_{T}>1 GeV/c","h");
		leg->AddEntry(h1d_pt_mvtx[0],"0 MVTX cluster","P");
		leg->AddEntry(h1d_pt_mvtx[1],"1 MVTX cluster","P");
		leg->AddEntry(h1d_pt_mvtx[2],"2 MVTX cluster","P");
		leg->AddEntry(h1d_pt_mvtx[3],"3 MVTX cluster","P");
		leg->Draw();
	}

	//return;

	TH1D *hnvertex_reco = (TH1D*)infile->Get("hnvertex_reco");
	TH1D *hnvertex_rave = (TH1D*)infile->Get("hnvertex_rave");
	TH1D *hnvertex_acts = (TH1D*)infile->Get("hnvertex_acts");

	TH1D *hnvertex_rave_refit = (TH1D*)infile->Get("hnvertex_rave_refit");
	TH1D *hnvertex_acts_refit = (TH1D*)infile->Get("hnvertex_acts_refit");

	//TH2D *hntrk_pv_rave_acts = (TH2D*)infile->Get("hntrk_pv_rave_acts");

	TH2D *hvtx_res_rave[3];
	TH2D *hvtx_res_acts[3];
	TH2D *hvtx_res_rave_refit[3];
	TH2D *hvtx_res_acts_refit[3];

	TH1D *h1d_rave[3][50];
	TH1D *h1d_acts[3][50];
	TH1D *h1d_rave_refit[3][50];
	TH1D *h1d_acts_refit[3][50];

	TGraphErrors *gres_rave[3];
	TGraphErrors *gres_acts[3];

	TGraphErrors *gres_rave_refit[3];
	TGraphErrors *gres_acts_refit[3];

	for (int jj=0; jj<3; jj++){
		hvtx_res_rave[jj] = (TH2D*)infile->Get(Form("hvtx_res_rave_%d",jj));
		hvtx_res_acts[jj] = (TH2D*)infile->Get(Form("hvtx_res_acts_%d",jj));

		hvtx_res_rave_refit[jj] = (TH2D*)infile->Get(Form("hvtx_res_rave_refit_%d",jj));
		hvtx_res_acts_refit[jj] = (TH2D*)infile->Get(Form("hvtx_res_acts_refit_%d",jj));

		gres_rave[jj] = new TGraphErrors;
		gres_acts[jj] = new TGraphErrors;

		gres_rave_refit[jj] = new TGraphErrors;
		gres_acts_refit[jj] = new TGraphErrors;

		for (int kk=0; kk<20; kk++){
			h1d_rave[jj][kk] = (TH1D*)hvtx_res_rave[jj]->ProjectionY(Form("h1d_rave_%d_%d",jj,kk),5*kk+1,5*kk+5);
			h1d_rave_refit[jj][kk] = (TH1D*)hvtx_res_rave_refit[jj]->ProjectionY(Form("h1d_rave_refit_%d_%d",jj,kk),5*kk+1,5*kk+5);

			h1d_acts[jj][kk] = (TH1D*)hvtx_res_acts[jj]->ProjectionY(Form("h1d_acts_%d_%d",jj,kk),5*kk+1,5*kk+5);
			h1d_acts_refit[jj][kk] = (TH1D*)hvtx_res_acts_refit[jj]->ProjectionY(Form("h1d_acts_refit_%d_%d",jj,kk),5*kk+1,5*kk+5);
		}//kk

		/*
		*/
	}//jj

	TH1D *hntrk_rave = (TH1D*)hvtx_res_rave[0]->ProjectionX("hntrk_rave");
	TH1D *hntrk_acts = (TH1D*)hvtx_res_acts[0]->ProjectionX("hntrk_acts");
	TH1D *hntrk_rave_refit = (TH1D*)hvtx_res_rave_refit[0]->ProjectionX("hntrk_rave_refit");
	TH1D *hntrk_acts_refit = (TH1D*)hvtx_res_acts_refit[0]->ProjectionX("hntrk_acts_refit");

	hntrk_rave->Scale(1./hntrk_rave->Integral());
	hntrk_acts->Scale(1./hntrk_acts->Integral());
	hntrk_rave_refit->Scale(1./hntrk_rave_refit->Integral());
	hntrk_acts_refit->Scale(1./hntrk_acts_refit->Integral());

	//return;

	TCanvas *c1 = new TCanvas("c1","c1",1.1*2*400,1*400);
	c1->Divide(2,1);

	{
		c1->cd(1);
		SetPadStyle();

		htmp = (TH1D*)gPad->DrawFrame(0,0,60,1.15*hnvertex_reco->GetMaximum());
		SetHistoStyle("Number of reco vtx","Number of events");
		hnvertex_reco->SetLineColor(1);
		hnvertex_reco->SetLineWidth(2);
		hnvertex_reco->Draw("same");

		TLegend *leg = new TLegend(0.5,0.8,0.9,0.9);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.045);
		leg->AddEntry(hnvertex_reco,"SvtxVertexMap","L");
		leg->Draw();
	}

	{
		c1->cd(2);
		SetPadStyle();

		htmp = (TH1D*)gPad->DrawFrame(0,0,5,1.3*hnvertex_rave_refit->GetMaximum());
		SetHistoStyle("Number of reco vtx","Number of events");
		hnvertex_rave->SetLineColor(1);
		hnvertex_acts->SetLineColor(2);

		hnvertex_rave_refit->SetLineColor(1);
		hnvertex_rave_refit->SetLineStyle(7);
		hnvertex_acts_refit->SetLineColor(2);
		hnvertex_acts_refit->SetLineStyle(7);

		hnvertex_rave->Draw("same");
		hnvertex_acts->Draw("same");
		hnvertex_rave_refit->Draw("same");
		hnvertex_acts_refit->Draw("same");

		TLegend *leg = new TLegend(0.5,0.8,0.9,0.9);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.045);
		leg->AddEntry(hnvertex_rave,"SvtxVertexMapRave","L");
		leg->Draw();
	}

	TCanvas *c1_1 = new TCanvas("c1_1","c1_1",1.1*400,1*400);
	{
		SetPadStyle();

		htmp = (TH1D*)gPad->DrawFrame(0,0,30,1.2*hntrk_rave_refit->GetMaximum());
		SetHistoStyle("Number of reco vtx","");

		hntrk_rave->SetMarkerStyle(20);
		hntrk_rave->SetMarkerColor(1);
		hntrk_rave->SetLineColor(1);
		hntrk_acts->SetMarkerStyle(20);
		hntrk_acts->SetMarkerColor(2);
		hntrk_acts->SetLineColor(2);

		hntrk_rave_refit->SetMarkerStyle(24);
		hntrk_rave_refit->SetMarkerColor(1);
		hntrk_rave_refit->SetLineColor(1);
		hntrk_acts_refit->SetMarkerStyle(24);
		hntrk_acts_refit->SetMarkerColor(2);
		hntrk_acts_refit->SetLineColor(2);

		hntrk_rave->Draw("p same");
		hntrk_acts->Draw("p same");
		hntrk_rave_refit->Draw("same");
		hntrk_acts_refit->Draw("same");

		TLegend *leg = new TLegend(0.5,0.65,0.9,0.9);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.045);
		leg->AddEntry(hntrk_rave,"RAVE","P");
		leg->AddEntry(hntrk_acts,"ACTS","P");
		leg->AddEntry(hntrk_rave_refit,"RAVE w/ MVTX","P");
		leg->AddEntry(hntrk_acts_refit,"ACTS w/ MVTX","P");
		leg->Draw();
	}


	TCanvas *c3 = new TCanvas("c3","c3",1.1*4*300,3*300);
	c3->Divide(4,3);

	for (int jj=0; jj<3; jj++){
		c3->cd(4*jj+1);
		SetPadStyle(1);
		gPad->SetLogz();

		htmp = (TH1D*)gPad->DrawFrame(0, -0.1, 50, 0.1);
		if ( jj==0 ){
			SetHistoStyle("Number of associated tracks","x_{Reco} - x_{Gen} (cm)");
		}else if ( jj==1 ){
			SetHistoStyle("Number of associated tracks","y_{Reco} - y_{Gen} (cm)");
		}else{
			SetHistoStyle("Number of associated tracks","z_{Reco} - z_{Gen} (cm)");
		}
		hvtx_res_rave[jj]->Draw("colz same");

		TLegend *leg = new TLegend(0.2,0.8,0.4,0.9);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.05);
		leg->AddEntry("","RAVE","");
		leg->Draw();
	}

	for (int jj=0; jj<3; jj++){
		c3->cd(4*jj+2);
		SetPadStyle(1);
		gPad->SetLogz();

		htmp = (TH1D*)gPad->DrawFrame(0, -0.1, 50, 0.1);
		if ( jj==0 ){
			SetHistoStyle("Number of associated tracks","x_{Reco} - x_{Gen} (cm)");
		}else if ( jj==1 ){
			SetHistoStyle("Number of associated tracks","y_{Reco} - y_{Gen} (cm)");
		}else{
			SetHistoStyle("Number of associated tracks","z_{Reco} - z_{Gen} (cm)");
		}
		hvtx_res_acts[jj]->Draw("colz same");

		TLegend *leg = new TLegend(0.2,0.8,0.4,0.9);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.05);
		leg->AddEntry("","ACTS","");
		leg->Draw();
	}

	for (int jj=0; jj<3; jj++){
		c3->cd(4*jj+3);
		SetPadStyle(1);
		gPad->SetLogz();

		htmp = (TH1D*)gPad->DrawFrame(0, -0.1, 50, 0.1);
		if ( jj==0 ){
			SetHistoStyle("Number of associated tracks","x_{Reco} - x_{Gen} (cm)");
		}else if ( jj==1 ){
			SetHistoStyle("Number of associated tracks","y_{Reco} - y_{Gen} (cm)");
		}else{
			SetHistoStyle("Number of associated tracks","z_{Reco} - z_{Gen} (cm)");
		}
		hvtx_res_rave_refit[jj]->Draw("colz same");

		TLegend *leg = new TLegend(0.5,0.8,0.8,0.9);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.05);
		leg->AddEntry("","RAVE w/ MVTX","");
		leg->Draw();
	}

	for (int jj=0; jj<3; jj++){
		c3->cd(4*jj+4);
		SetPadStyle(1);
		gPad->SetLogz();

		htmp = (TH1D*)gPad->DrawFrame(0, -0.1, 50, 0.1);
		if ( jj==0 ){
			SetHistoStyle("Number of associated tracks","x_{Reco} - x_{Gen} (cm)");
		}else if ( jj==1 ){
			SetHistoStyle("Number of associated tracks","y_{Reco} - y_{Gen} (cm)");
		}else{
			SetHistoStyle("Number of associated tracks","z_{Reco} - z_{Gen} (cm)");
		}
		hvtx_res_acts_refit[jj]->Draw("colz same");

		TLegend *leg = new TLegend(0.5,0.8,0.8,0.9);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.05);
		leg->AddEntry("","ACTS w/ MVTX","");
		leg->Draw();
	}

	TCanvas *c5 = new TCanvas("c5","c5",1.1*12*120,4*120);
	c5->Divide(12,4);

	for (int kk=1; kk<12; kk++){
		c5->cd(kk+1);
		SetPadStyle();
		gPad->SetMargin(0.1,0.01,0.1,0.01);

		htmp = (TH1D*)gPad->DrawFrame(-0.02,0,0.02,1.1*h1d_rave[2][kk]->GetMaximum());
		SetHistoStyle();
		h1d_rave[2][kk]->Draw("same");

		TF1 *f1 = new TF1("f1","gaus",-0.005,0.005);
		h1d_rave[2][kk]->Fit(f1, "R0Q");

		float mean = f1->GetParameter(1);
		float sigma = f1->GetParameter(2);

		f1->SetRange(mean-2*sigma, mean+2*sigma);
		h1d_rave[2][kk]->Fit(f1,"R0Q");
		f1->Draw("same");

		float xx = 2.5 + 5*kk;
		float yy = f1->GetParameter(2);
		float yy_err = f1->GetParError(2);

		gres_rave[2]->SetPoint(kk, xx, yy);
		gres_rave[2]->SetPointError(kk, 0, yy_err);
	}

	for (int kk=1; kk<12; kk++){
		c5->cd(kk+12+1);
		SetPadStyle();
		gPad->SetMargin(0.1,0.01,0.1,0.01);

		htmp = (TH1D*)gPad->DrawFrame(-0.2,0,0.2,1.1*h1d_acts[2][kk]->GetMaximum());
		SetHistoStyle();
		h1d_acts[2][kk]->Draw("same");

		TF1 *f1 = new TF1("f1","gaus",-0.1,0.1);
		h1d_acts[2][kk]->Fit(f1, "R0Q");

		float mean = f1->GetParameter(1);
		float sigma = f1->GetParameter(2);

		f1->SetRange(mean-2*sigma, mean+2*sigma);
		h1d_acts[2][kk]->Fit(f1,"R0Q");
		f1->Draw("same");
	}

	for (int kk=1; kk<12; kk++){
		c5->cd(kk+24+1);
		SetPadStyle();
		gPad->SetMargin(0.1,0.01,0.1,0.01);

		htmp = (TH1D*)gPad->DrawFrame(-0.02,0,0.02,1.1*h1d_rave_refit[2][kk]->GetMaximum());
		SetHistoStyle();
		h1d_rave_refit[2][kk]->Draw("same");

		TF1 *f1 = new TF1("f1","gaus",-0.01,0.01);
		h1d_rave_refit[2][kk]->Fit(f1, "R0Q");

		float mean = f1->GetParameter(1);
		float sigma = f1->GetParameter(2);

		f1->SetRange(mean-2*sigma, mean+2*sigma);
		h1d_rave_refit[2][kk]->Fit(f1,"R0Q");
		f1->Draw("same");

		float xx = 2.5 + 5*kk;
		float yy = f1->GetParameter(2);
		float yy_err = f1->GetParError(2);

		gres_rave_refit[2]->SetPoint(kk, xx, yy);
		gres_rave_refit[2]->SetPointError(kk, 0, yy_err);
	}

	for (int kk=1; kk<12; kk++){
		c5->cd(kk+36+1);
		SetPadStyle();
		gPad->SetMargin(0.1,0.01,0.1,0.01);

		htmp = (TH1D*)gPad->DrawFrame(-0.02,0,0.02,1.1*h1d_acts_refit[2][kk]->GetMaximum());
		SetHistoStyle();
		h1d_acts_refit[2][kk]->Draw("same");

		TF1 *f1 = new TF1("f1","gaus",-0.01,0.01);
		h1d_acts_refit[2][kk]->Fit(f1, "R0Q");

		float mean = f1->GetParameter(1);
		float sigma = f1->GetParameter(2);

		f1->SetRange(mean-2*sigma, mean+2*sigma);
		h1d_acts_refit[2][kk]->Fit(f1,"R0Q");
		f1->Draw("same");

		float xx = 2.5 + 5*kk;
		float yy = f1->GetParameter(2);
		float yy_err = f1->GetParError(2);

		gres_acts_refit[2]->SetPoint(kk, xx, yy);
		gres_acts_refit[2]->SetPointError(kk, 0, yy_err);
	}

	TCanvas *c6 = new TCanvas("c6","c6",1.1*500,500);
	SetPadStyle();

	{
		htmp = (TH1D*)gPad->DrawFrame(0, 0, 60, 0.005);
		SetHistoStyle("Number of associated track", "z-vtx resolution (cm)");
		htmp->GetYaxis()->SetTitleOffset(1.3);

		gres_rave[2]->SetLineColor(1);
		gres_rave[2]->SetMarkerColor(1);
		gres_rave[2]->SetMarkerStyle(24);
		gres_rave[2]->Draw("p");

		gres_rave_refit[2]->SetLineColor(2);
		gres_rave_refit[2]->SetMarkerColor(2);
		gres_rave_refit[2]->SetMarkerStyle(24);
		gres_rave_refit[2]->Draw("p");

		gres_acts_refit[2]->SetLineColor(4);
		gres_acts_refit[2]->SetMarkerColor(4);
		gres_acts_refit[2]->SetMarkerStyle(24);
		gres_acts_refit[2]->Draw("p");

		TLegend *leg = new TLegend(0.5,0.7,0.9,0.9);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.045);
		leg->AddEntry(gres_rave[2],"RAVE","P");
		leg->AddEntry(gres_rave_refit[2],"RAVE w/ MVTX","P");
		leg->AddEntry(gres_acts_refit[2],"ACTS w/ MVTX","P");
		leg->Draw();
	}

	TH2D *hvtxz_gen_rave = (TH2D*)infile->Get("hvtxz_gen_rave");
	TH2D *hvtxz_gen_acts = (TH2D*)infile->Get("hvtxz_gen_acts");
	TH2D *hvtxz_gen_rave_refit = (TH2D*)infile->Get("hvtxz_gen_rave_refit");
	TH2D *hvtxz_gen_acts_refit = (TH2D*)infile->Get("hvtxz_gen_acts_refit");

	TH1D *hvtxz_gen = (TH1D*)hvtxz_gen_rave->ProjectionX("hvtxz_gen");

	TCanvas *c7 = new TCanvas("c7","c7",1.1*3*400,2*400);
	c7->Divide(3,2);

	{
		c7->cd(1);
		SetPadStyle(1);
		gPad->SetLogz();
		htmp = (TH1D*)hvtxz_gen;
		SetHistoStyle("z_{Gen} [cm]","");

		hvtxz_gen->Draw();
	}

	{
		c7->cd(2);
		SetPadStyle(1);
		gPad->SetLogz();
		htmp = (TH1D*)hvtxz_gen_rave;
		SetHistoStyle("z_{Gen} [cm]","z_{Reco} [cm]");
		hvtxz_gen_rave->RebinX(10);
		hvtxz_gen_rave->RebinY(10);
		hvtxz_gen_rave->Draw("colz");

		htmp->SetTitleSize(0.05);
		htmp->SetTitleFont(62);
		htmp->SetTitle("RAVE");
	}

	{
		c7->cd(3);
		SetPadStyle(1);
		gPad->SetLogz();
		htmp = (TH1D*)hvtxz_gen_acts;
		SetHistoStyle("z_{Gen} [cm]","z_{Reco} [cm]");
		hvtxz_gen_acts->RebinX(10);
		hvtxz_gen_acts->RebinY(10);
		hvtxz_gen_acts->Draw("colz");

		htmp->SetTitleSize(0.05);
		htmp->SetTitleFont(62);
		htmp->SetTitle("ACTS");
	}

	{
		c7->cd(5);
		SetPadStyle(1);
		gPad->SetLogz();
		htmp = (TH1D*)hvtxz_gen_rave_refit;
		SetHistoStyle("z_{Gen} [cm]","z_{Reco} [cm]");
		hvtxz_gen_rave_refit->RebinX(10);
		hvtxz_gen_rave_refit->RebinY(10);
		hvtxz_gen_rave_refit->Draw("colz");

		htmp->SetTitleSize(0.05);
		htmp->SetTitleFont(62);
		htmp->SetTitle("RAVE w/ MVTX");
	}

	{
		c7->cd(6);
		SetPadStyle(1);
		gPad->SetLogz();
		htmp = (TH1D*)hvtxz_gen_acts_refit;
		SetHistoStyle("z_{Gen} [cm]","z_{Reco} [cm]");
		hvtxz_gen_acts_refit->RebinX(10);
		hvtxz_gen_acts_refit->RebinY(10);
		hvtxz_gen_acts_refit->Draw("colz");

		htmp->SetTitleSize(0.05);
		htmp->SetTitleFont(62);
		htmp->SetTitle("ACTS w/ MVTX");
	}

	/*
	TCanvas *c4 = new TCanvas("c4","c4",1.1*3*300,1*300);
	c4->Divide(3,1);

	for (int jj=0; jj<3; jj++){
		c4->cd(jj+1);
		SetPadStyle(1);
		gPad->SetLogz();

		htmp = (TH1D*)gPad->DrawFrame(0, -0.1, 30, 0.1);
		if ( jj==0 ){
			SetHistoStyle("Number of associated tracks","RAVE vtx-x (cm)");
		}else if ( jj==1 ){
			SetHistoStyle("Number of associated tracks","RAVE vtx-y (cm)");
		}else{
			SetHistoStyle("Number of associated tracks","RAVE vtx-z (cm)");
		}
		hvtx_res_refit[jj]->Draw("colz same");
	}

	TGraph *gres[3];

	*/




}
