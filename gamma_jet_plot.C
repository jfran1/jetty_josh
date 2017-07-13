
// Start main plot
void gamma_jet_plot()
{
	//################ OPENING FILES #################
	const char *fnames[] = {
		"/Users/joshfrandsen/test/jetty/output/jetGamma_10_20.root",   	// 0
		"/Users/joshfrandsen/test/jetty/output/jetGamma_20_30.root",   	// 1
		"/Users/joshfrandsen/test/jetty/output/jetGamma_30_40.root",   	// 2
		"/Users/joshfrandsen/test/jetty/output/jetGamma_40_50.root",   	// 3
		"/Users/joshfrandsen/test/jetty/output/jetGamma_50_-1.root",   	// 4
		0
	};

	int _tmp = 0;
	while (fnames[_tmp] != 0)
		_tmp++;

	const int nfiles = _tmp;
	TFile *files[nfiles];
	for (int i = 0; i < nfiles; i++)
	{
		files[i] = TFile::Open(fnames[i]);
		if (files[i] ==0) 
		{
			cout<< "ERROR! File " << i << " didn't open" << endl;
			return;
		}
	}


	TH1F *gamma_prompt[5];
	TH1F *jet[5];
	TH2F *dPhi_pT[5];
	TH2F *dJet_pT[5];
	TH2F *dJet_pT_max[5];
	TH1F *norm[5];
	TH1D*dJet_projection_20_40[5];	
	TH1D*dJet_projection_40_80[5];
	TH1D*dJet_projection_80_120[5];
	TH1D*dJet_projection_max_20_40[5];	
	TH1D*dJet_projection_max_40_80[5];
	TH1D*dJet_projection_max_80_120[5];
	int bin20[5];
	int bin40[5];
	int bin80[5];
	int bin120[5];	

	double sigma[5];
	double weightSum[5];

	// FILLING ARRAYS WITH HISTOGRAMS FROM FILES AND GETTING NORMALIZATION VALUES
	// ###########################################################################
	for (int i =0; i < 5; i++)
	{
		gamma_prompt[i] = (TH1F*)files[i]->Get("gammaPrompt"); // filter for hard gamma
		jet[i] = (TH1F*)files[i]->Get("jet");
		dPhi_pT[i] = (TH2F*)files[i]->Get("dphi_pT");
		dJet_pT[i] = (TH2F*)files[i]->Get("dJet_pT");
		dJet_pT_max[i] = (TH2F*)files[i]->Get("dJet_pT_max");

		bin20[i] = dJet_pT[i]->GetXaxis()->FindBin(20);
		bin40[i] = dJet_pT[i]->GetXaxis()->FindBin(40);
		bin80[i] = dJet_pT[i]->GetXaxis()->FindBin(80);
		bin120[i] = dJet_pT[i]->GetXaxis()->FindBin(120);

		dJet_projection_20_40[i] =dJet_pT[i]->ProjectionY("dJet_projection_20_40", bin20[i],bin40[i]);
		dJet_projection_40_80[i] =dJet_pT[i]->ProjectionY("dJet_projection_40_80", bin40[i],bin80[i]);
		dJet_projection_80_120[i] =dJet_pT[i]->ProjectionY("dJet_projection_80_120", bin80[i],bin120[i]);
		dJet_projection_20_40[i]->Rebin(2);
		dJet_projection_40_80[i]->Rebin(2);
		dJet_projection_80_120[i]->Rebin(2);	
		
		dJet_projection_max_20_40[i] =dJet_pT_max[i]->ProjectionY("dJet_projection_max_20_40", bin20[i],bin40[i]);
		dJet_projection_max_40_80[i] =dJet_pT_max[i]->ProjectionY("dJet_projection_max_40_80", bin40[i],bin80[i]);
		dJet_projection_max_80_120[i] =dJet_pT_max[i]->ProjectionY("dJet_projection_max_80_120", bin80[i],bin120[i]);	
		dJet_projection_max_20_40[i]->Rebin(2);
		dJet_projection_max_40_80[i]->Rebin(2);
		dJet_projection_max_80_120[i]->Rebin(2);

		norm[i] = (TH1F*)files[i]->Get("norm");

		sigma[i] = norm[i]->GetBinContent(1);
		weightSum[i] = norm[i]->GetBinContent(2);
	}


	// NORMALIZTION
	// ############################################################################
	for (int i = 0; i < 5; i++)
	{
		gamma_prompt[i]->Scale( (sigma[i]) / (weightSum[i] * gamma_prompt[i]->GetBinWidth(1)) );
		jet[i] -> Scale( (sigma[i]) / (weightSum[i] * jet[i]->GetBinWidth(1)) );
		dJet_projection_20_40[i] -> Scale( (sigma[i]) / (weightSum[i] * dJet_projection_20_40[i]->GetBinWidth(1)) );	
		dJet_projection_40_80[i] -> Scale( (sigma[i]) / (weightSum[i] * dJet_projection_40_80[i]->GetBinWidth(1)) );	
		dJet_projection_80_120[i] -> Scale( (sigma[i]) / (weightSum[i] * dJet_projection_80_120[i]->GetBinWidth(1)) );	

		dJet_projection_max_20_40[i] -> Scale( (sigma[i]) / (weightSum[i] * dJet_projection_max_20_40[i]->GetBinWidth(1)) );	
		dJet_projection_max_40_80[i] -> Scale( (sigma[i]) / (weightSum[i] * dJet_projection_max_40_80[i]->GetBinWidth(1)) );	
		dJet_projection_max_80_120[i] -> Scale( (sigma[i]) / (weightSum[i] * dJet_projection_max_80_120[i]->GetBinWidth(1)) );			
	}

	// GETT SUM OF EACH PT HAT BIN
	// #############################################################################
	TH1F *gamma_prompt_sum = (TH1F*)gamma_prompt[0]->Clone("gamma_prompt_sum");
	TH1F *jet_sum = (TH1F*)jet[0]->Clone("jet_sum");
	TH2F *dJet_sum = (TH2F*)dJet_pT[0]->Clone("dJet_sum");
	TH2F *dPhi_pT_sum = (TH2F*)dPhi_pT[0]->Clone("dPhi_pT_sum");
	TH1D *dJet_projection_20_40_sum = (TH1D*)dJet_projection_20_40[0]->Clone("dJet_projection_20_40_sum");
	TH1D *dJet_projection_40_80_sum = (TH1D*)dJet_projection_40_80[0]->Clone("dJet_projection_40_80_sum");
	TH1D *dJet_projection_80_120_sum = (TH1D*)dJet_projection_80_120[0]->Clone("dJet_projection_80_120_sum");

	TH1D *dJet_projection_max_20_40_sum = (TH1D*)dJet_projection_max_20_40[0]->Clone("dJet_projection_max_20_40_sum");
	TH1D *dJet_projection_max_40_80_sum = (TH1D*)dJet_projection_max_40_80[0]->Clone("dJet_projection_max_40_80_sum");
	TH1D *dJet_projection_max_80_120_sum = (TH1D*)dJet_projection_max_80_120[0]->Clone("dJet_projection_max_80_120_sum");

	for(int i=1; i < 5; i++)
	{
		gamma_prompt_sum->Add(gamma_prompt[i]);
		jet_sum->Add(jet[i]);
		dJet_sum->Add(dJet_pT[i]);
		dPhi_pT_sum->Add(dPhi_pT[i]);
		dJet_projection_20_40_sum->Add(dJet_projection_20_40[i]);
		dJet_projection_40_80_sum->Add(dJet_projection_40_80[i]);
		dJet_projection_80_120_sum->Add(dJet_projection_80_120[i]);	

		dJet_projection_max_20_40_sum->Add(dJet_projection_max_20_40[i]);
		dJet_projection_max_40_80_sum->Add(dJet_projection_max_40_80[i]);
		dJet_projection_max_80_120_sum->Add(dJet_projection_max_80_120[i]);	
	}	

	// DRAWING HISTOGRAMS
	// #############################################################################
	TCanvas *c1 = new TCanvas("jet_pTHatBins");
	TCanvas *c2 = new TCanvas("Jet");
	TCanvas *c3 = new TCanvas("prompt_photon");
	TCanvas *c4 = new TCanvas("dJet");
	TCanvas *c5 = new TCanvas("dPhi_pT");
	TCanvas *c6 = new TCanvas("dJet_projection_20_40");
	TCanvas *c7 = new TCanvas("dJet_projection_40_80");
	TCanvas *c8 = new TCanvas("dJet_projection_80_120");
	// TCanvas *c9 = new TCanvas("dJet_projection_max_20_40");
	// TCanvas *c10 = new TCanvas("dJet_projection_max_40_80");
	// TCanvas *c11 = new TCanvas("dJet_projection_max_80_120");

	for(int i=0;i<5; i++)
	{
		jet[i]->SetLineColor(kAzure+i);
		jet[i]->SetMarkerColor(kAzure+i);
		jet[i]->SetMarkerStyle(24+i);
		jet[i]->SetLineWidth(2);
		jet[i]->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb / GeV]");
		jet[i]->SetTitle("prompt p_{T}^{#gamma}  #sqrt{s} = 5 TeV #hat{p_{T}} > 10 GeV");

		if(i==0)
		{
			c1->cd();
			jet[i]->Draw("pe");
		}
		else
		{
			c1->cd();
			jet[i]->Draw("same pe");
		}
	}
	
	gamma_prompt_sum->SetLineColor(kRed);
	gamma_prompt_sum->SetMarkerColor(kRed);
	gamma_prompt_sum->SetLineWidth(2);
	gamma_prompt_sum->SetMarkerStyle(24);
	gamma_prompt_sum->SetXTitle("p_{T}^{#gamma} GeV");
	gamma_prompt_sum->SetTitle("#gamma prompt #sqrt{s} = 5 TeV #hat{p_{T}} > 10 GeV");
	gamma_prompt_sum->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb / GeV]");

	jet_sum->SetLineColor(kRed);
	jet_sum->SetLineWidth(2);
	jet_sum->SetMarkerStyle(24);
	jet_sum->SetMarkerColor(kRed);
	jet_sum->SetTitle("#gamma+jet #sqrt{s} = 5 TeV");
	jet_sum->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb / GeV] ");

	dJet_sum->SetLineColor(kRed);
	dJet_sum->SetLineWidth(2);
	dJet_sum->SetMarkerStyle(24);
	dJet_sum->SetMarkerColor(kRed);

	dJet_projection_20_40_sum->SetLineColor(kRed);
	dJet_projection_20_40_sum->SetLineWidth(2);
	dJet_projection_20_40_sum->SetMarkerStyle(24);
	dJet_projection_20_40_sum->SetMarkerColor(kRed);
	dJet_projection_20_40_sum->SetYTitle(" (Projection)  #frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb / GeV]");
	dJet_projection_20_40_sum->SetTitle(" 20-40 GeV projection");

	dJet_projection_40_80_sum->SetLineColor(kRed);
	dJet_projection_40_80_sum->SetLineWidth(2);
	dJet_projection_40_80_sum->SetMarkerStyle(24);
	dJet_projection_40_80_sum->SetMarkerColor(kRed);
	dJet_projection_40_80_sum->SetYTitle(" (Projection)  #frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb / GeV]");
	dJet_projection_40_80_sum->SetTitle(" 40-80 GeV projection");

	dJet_projection_80_120_sum->SetLineColor(kRed);
	dJet_projection_80_120_sum->SetLineWidth(2);
	dJet_projection_80_120_sum->SetMarkerStyle(24);
	dJet_projection_80_120_sum->SetMarkerColor(kRed);
	dJet_projection_80_120_sum->SetYTitle(" (Projection)  #frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb / GeV]");
	dJet_projection_80_120_sum->SetTitle(" 80-120 GeV projection");

	dJet_projection_max_20_40_sum->SetLineColor(kBlack);
	dJet_projection_max_20_40_sum->SetLineWidth(2);
	dJet_projection_max_20_40_sum->SetMarkerStyle(20);
	dJet_projection_max_20_40_sum->SetMarkerColor(kBlack);
	dJet_projection_max_20_40_sum->SetYTitle(" (Projection)  #frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb / GeV]");
	dJet_projection_max_20_40_sum->SetTitle(" leading jet 20-40 GeV projection");

	dJet_projection_max_40_80_sum->SetLineColor(kBlack);
	dJet_projection_max_40_80_sum->SetLineWidth(2);
	dJet_projection_max_40_80_sum->SetMarkerStyle(20);
	dJet_projection_max_40_80_sum->SetMarkerColor(kBlack);
	dJet_projection_max_40_80_sum->SetYTitle(" (Projection)  #frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb / GeV]");
	dJet_projection_max_40_80_sum->SetTitle(" leading jet 40-80 GeV projection");

	dJet_projection_max_80_120_sum->SetLineColor(kBlack);
	dJet_projection_max_80_120_sum->SetLineWidth(2);
	dJet_projection_max_80_120_sum->SetMarkerStyle(20);
	dJet_projection_max_80_120_sum->SetMarkerColor(kBlack);
	dJet_projection_max_80_120_sum->SetYTitle(" (Projection)  #frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb / GeV]");
	dJet_projection_max_80_120_sum->SetTitle(" leading jet 80-120 GeV projection");

	c2->cd();
	jet_sum->Draw("pe");
	
	c3->cd();
	gamma_prompt_sum->Draw("pe");

	c4->cd();
	dJet_sum->Draw("colz");

	c5->cd();
	dPhi_pT_sum->Draw("lego2");

	c6->cd();
	dJet_projection_max_20_40_sum->Draw(" pe");
	dJet_projection_20_40_sum->Draw("pe same");

	c7->cd();
	dJet_projection_max_40_80_sum->Draw(" pe");
	dJet_projection_40_80_sum->Draw("pe same");
	
	c8->cd();
	dJet_projection_max_80_120_sum->Draw(" pe");
	dJet_projection_80_120_sum->Draw("pe same");

	c1->SetLogy();
	c2->SetLogy();
	c3->SetLogy();
	c4->SetLogz();


	// MAKE & DRAW LEGEND
	// #############################################################################	
	TLegend *leg1 = new TLegend();
	TLegend *leg2 = new TLegend();
	
	leg1->AddEntry(jet[0], "10 < #hat{p_{T}} < 20 GeV");
	leg1->AddEntry(jet[1], "20 < #hat{p_{T}} < 30 GeV");
	leg1->AddEntry(jet[2], "30 < #hat{p_{T}} < 40 GeV");
	leg1->AddEntry(jet[3], "40 < #hat{p_{T}} < 50 GeV");	
	leg1->AddEntry(jet[4], "50 < #hat{p_{T}} < inf GeV");

	leg2->AddEntry(dJet_projection_20_40_sum, "Jets #Delta #phi > #frac{2#pi}{3}");
	leg2->AddEntry(dJet_projection_max_20_40_sum, "Leading Jet");



	c1->cd();
	leg1->Draw("same");

	c6->cd();
	leg2->Draw("same");
	c7->cd();
	leg2->Draw("same");
	c8->cd();
	leg2->Draw("same");
}




