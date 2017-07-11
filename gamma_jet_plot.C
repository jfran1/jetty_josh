
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
	TH1F *norm[5];

	double sigma[5];
	double weightSum[5];

	// FILLING ARRAYS WITH HISTOGRAMS FROM FILES AND GETTING NORMALIZATION VALUES
	// ###########################################################################
	for (int i =0; i < 5; i++)
	{
		gamma_prompt[i] = (TH1F*)files[i]->Get("gammaPrompt"); // filter for hard gamma
		jet[i] = (TH1F*)files[i]->Get("jet");
		norm[i] = (TH1F*)files[i]->Get("norm");

		sigma[i] = norm[i]->GetBinContent(1);
		weightSum[i] = norm[i]->GetBinContent(2);
	}
	// NORMALIZTION
	// ############################################################################
	for (int i = 0; i < 5; i++)
	{
		gamma_prompt[i]->Scale( (1.e9 * sigma[i]) / (weightSum[i] * gamma_prompt[i]->GetBinWidth(1)) );
		jet[i] -> Scale( (1.e9 * sigma[i]) / (weightSum[i] * jet[i]->GetBinWidth(1)) );
	}

	// GETT SUM OF EACH PT HAT BIN
	// #############################################################################
	TH1F *gamma_prompt_sum = (TH1F*)gamma_prompt[0]->Clone("gamma_prompt_sum");
	TH1F *jet_sum = (TH1F*)jet[0]->Clone("jet_sum");

	for(int i=1; i < 5; i++)
	{
		gamma_prompt_sum->Add(gamma_prompt[i]);
		jet_sum->Add(jet[i]);
	}

	// DRAWING HISTOGRAMS
	// #############################################################################
	TCanvas *c1 = new TCanvas("jet_pTHatBins");
	TCanvas *c2 = new TCanvas("Jet");
	TCanvas *c3 = new TCanvas("prompt_photon");

	for(int i=0;i<5; i++)
	{
		jet[i]->SetLineColor(kAzure+i);
		jet[i]->SetMarkerColor(kAzure+i);
		jet[i]->SetMarkerStyle(24+i);
		jet[i]->SetLineWidth(2);
		jet[i]->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [pb GeV]");
		jet[i]->SetTitle("prompt p_{T}^{#gamma}  #sqrt{s} = 7 TeV #hat{p_{T}} > 50 GeV");

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
	gamma_prompt_sum->SetMinimum(1.e-4);
	gamma_prompt_sum->SetLineWidth(2);
	gamma_prompt_sum->SetMarkerStyle(24);
	gamma_prompt_sum->SetXTitle("p_{T}^{#gamma} GeV");
	gamma_prompt_sum->SetTitle("#gamma prompt #sqrt{s} = 13 TeV");
	gamma_prompt_sum->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [pb GeV]");

	jet_sum->SetLineColor(kRed);
	jet_sum->SetLineWidth(2);
	jet_sum->SetMarkerStyle(24);
	jet_sum->SetMarkerColor(kRed);
	jet_sum->SetTitle("#gamma+jet #sqrt{s} = 12 TeV");
	jet_sum->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} ");


	c2->cd();
	jet_sum->Draw("pe");
	
	c3->cd();
	gamma_prompt_sum->Draw("pe");

	c1->SetLogy();
	c2->SetLogy();
	c3->SetLogy();

	// MAKE & DRAW LEGEND
	// #############################################################################	
	TLegend *leg1 = new TLegend();
	
	leg1->AddEntry(jet[0], "10 < #hat{p_{T}} < 20 GeV");
	leg1->AddEntry(jet[1], "20 < #hat{p_{T}} < 30 GeV");
	leg1->AddEntry(jet[2], "30 < #hat{p_{T}} < 40 GeV");
	leg1->AddEntry(jet[3], "40 < #hat{p_{T}} < 50 GeV");	
	leg1->AddEntry(jet[4], "50 < #hat{p_{T}} < inf GeV");


	c1->cd();
	leg1->Draw("same");

}




