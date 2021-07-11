#include "Style.h"

void DrawAnaVertex(){

	TFile *infile = new TFile("outfile_AnaVertex.root","read");

	TH1D *hnvertex_reco = (TH1D*)infile->Get("hnvertex_reco");
	TH1D *hnvertex_rave = (TH1D*)infile->Get("hnvertex_rave");
	TH1D *hnvertex_acts = (TH1D*)infile->Get("hnvertex_acts");

	TH2D *hntrk_pv_rave_acts = (TH2D*)infile->Get("hntrk_pv_rave_acts");

	TH2D *hvtx_res_rave[3];
	TH2D *hvtx_res_acts[3];
	TH2D *hvtx_res_refit[3];

	TH1D *h1d_rave[3];
	TH1D *h1d_acts[3];
	TH1D *h1d_refit[3][10];

	for (int jj=0; jj<3; jj++){
		hvtx_res_rave[jj] = (TH2D*)infile->Get(Form("hvtx_res_rave_%d",jj));
		hvtx_res_acts[jj] = (TH2D*)infile->Get(Form("hvtx_res_acts_%d",jj));
		hvtx_res_refit[jj] = (TH2D*)infile->Get(Form("hvtx_res_refit_%d",jj));

		h1d_rave[jj] = (TH1D*)hvtx_res_rave[jj]->ProjectionY(Form("h1d_rave_%d",jj));
		h1d_acts[jj] = (TH1D*)hvtx_res_acts[jj]->ProjectionY(Form("h1d_acts_%d",jj));

		for (int kk=0; kk<10; kk++){
			h1d_refit[jj][kk] = (TH1D*)hvtx_res_refit[jj]->ProjectionY(Form("h1d_refit_%d_%d",jj,kk),5*kk+1,5*kk+5);
		}//kk
	}//jj

	TCanvas *c1 = new TCanvas("c1","c1",1.1*3*400,400);
	c1->Divide(3,1);

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

		htmp = (TH1D*)gPad->DrawFrame(0,0,5,1.15*hnvertex_rave->GetMaximum());
		SetHistoStyle("Number of reco vtx","Number of events");
		hnvertex_rave->SetLineColor(2);
		hnvertex_rave->SetLineWidth(2);
		hnvertex_rave->Draw("same");

		TLegend *leg = new TLegend(0.5,0.8,0.9,0.9);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.045);
		leg->AddEntry(hnvertex_rave,"SvtxVertexMapRave","L");
		leg->Draw();
	}

	{
		c1->cd(3);
		SetPadStyle();

		htmp = (TH1D*)gPad->DrawFrame(0,0,5,1.15*hnvertex_acts->GetMaximum());
		SetHistoStyle("Number of reco vtx","Number of events");
		hnvertex_acts->SetLineColor(4);
		hnvertex_acts->SetLineWidth(2);
		hnvertex_acts->Draw("same");

		TLegend *leg = new TLegend(0.5,0.8,0.9,0.9);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.045);
		leg->AddEntry(hnvertex_acts,"SvtxVertexMapActs","L");
		leg->Draw();
	}

	TCanvas *c2 = new TCanvas("c2","c2",1.1*500,500);
	{

		SetPadStyle(1);

		htmp = (TH1D*)gPad->DrawFrame(0,0,50,50);
		SetHistoStyle("Number of associated tracks to RAVE vertex","Number of associated tracks to ACTS vertex","",0.045,0.04);

		hntrk_pv_rave_acts->Draw("colz same");

	}

	TCanvas *c3 = new TCanvas("c3","c3",1.1*3*300,3*300);
	c3->Divide(3,3);

	for (int jj=0; jj<3; jj++){
		c3->cd(jj+1);
		SetPadStyle(1);
		gPad->SetLogz();

		htmp = (TH1D*)gPad->DrawFrame(0, -0.1, 50, 0.1);
		if ( jj==0 ){
			SetHistoStyle("Number of associated tracks","RAVE vtx-x (cm)");
		}else if ( jj==1 ){
			SetHistoStyle("Number of associated tracks","RAVE vtx-y (cm)");
		}else{
			SetHistoStyle("Number of associated tracks","RAVE vtx-z (cm)");
		}
		hvtx_res_rave[jj]->Draw("colz same");
	}

	for (int jj=0; jj<3; jj++){
		c3->cd(jj+1+3);
		SetPadStyle(1);
		gPad->SetLogz();

		htmp = (TH1D*)gPad->DrawFrame(0, -0.1, 50, 0.1);
		if ( jj==0 ){
			SetHistoStyle("Number of associated tracks","ACTS vtx-x (cm)");
		}else if ( jj==1 ){
			SetHistoStyle("Number of associated tracks","ACTS vtx-y (cm)");
		}else{
			SetHistoStyle("Number of associated tracks","ACTS vtx-z (cm)");
		}
		hvtx_res_acts[jj]->Draw("colz same");
	}

	for (int jj=0; jj<3; jj++){
		c3->cd(jj+1+6);
		SetPadStyle();

		float max = TMath::Max(h1d_rave[jj]->GetMaximum(), h1d_acts[jj]->GetMaximum());
		htmp = (TH1D*)gPad->DrawFrame(-0.02, 0, 0.02, 1.15*max);

		if ( jj==0 ){
			SetHistoStyle("vtx-x (cm)");
		}else if ( jj==1 ){
			SetHistoStyle("vtx-y (cm)");
		}else{
			SetHistoStyle("vtx-z (cm)");
		}

		h1d_rave[jj]->SetLineColor(2);
		h1d_rave[jj]->Draw("same");

		h1d_acts[jj]->SetLineColor(4);
		h1d_acts[jj]->Draw("same");

		TLegend *leg = new TLegend(0.20,0.75,0.4,0.9);
		leg->SetFillStyle(0);
		leg->SetBorderSize(0);
		leg->SetTextSize(0.045);
		leg->AddEntry(h1d_rave[jj],"RAVE","L");
		leg->AddEntry(h1d_acts[jj],"ACTS","L");
		leg->Draw();

	}

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

	TCanvas *c5 = new TCanvas("c5","c5",1.1*5*200,3*200);
	c5->Divide(5,3);

	for (int jj=0; jj<3; jj++){

		gres[jj] = new TGraph;
		gres[jj]->SetMarkerStyle(24);

		for (int kk=0; kk<5; kk++){
			c5->cd(5*jj+kk+1);
			SetPadStyle();

			htmp = (TH1D*)gPad->DrawFrame(-0.01, 0, 0.01, 1.1*h1d_refit[jj][kk]->GetMaximum());
			SetHistoStyle();
			h1d_refit[jj][kk]->Draw("same");

			TF1 *f1 = new TF1("f1","gaus",-0.005,0.005);
			h1d_refit[jj][kk]->Fit(f1,"R0Q");

			float mean = f1->GetParameter(1);
			float sigma = f1->GetParameter(2);

			f1->SetRange(mean-2*sigma, mean+2*sigma);
			h1d_refit[jj][kk]->Fit(f1,"R0Q");

			f1->Draw("same");

			float xx = 2.5 + 5*kk;

			gres[jj]->SetPoint(kk, xx, f1->GetParameter(2));
		}
	}

	TCanvas *c6 = new TCanvas("c6","c6",1.1*400,400);
	SetPadStyle();

	{

		htmp = (TH1D*)gPad->DrawFrame(0, 0, 30, 0.004);
		SetHistoStyle();

		gres[0]->SetMarkerColor(1);
		gres[1]->SetMarkerColor(2);
		gres[2]->SetMarkerColor(4);

		gres[0]->Draw("p");
		gres[1]->Draw("p");
		gres[2]->Draw("p");


	}




}
