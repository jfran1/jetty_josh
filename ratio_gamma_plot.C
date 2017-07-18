
// Start main plot
void ratio_gamma_plot()
{
	//################ OPENING FILES #################
	const char *fnames[] = {
		"/Users/joshfrandsen/test/jetty/output/ratio_gamma_10_20.root",   	// 0
		"/Users/joshfrandsen/test/jetty/output/ratio_gamma_20_30.root",   	// 1
		"/Users/joshfrandsen/test/jetty/output/ratio_gamma_30_40.root",   	// 2
		"/Users/joshfrandsen/test/jetty/output/ratio_gamma_40_50.root",   	// 3
		"/Users/joshfrandsen/test/jetty/output/ratio_gamma_50_70.root",   	// 4
		"/Users/joshfrandsen/test/jetty/output/ratio_gamma_70_-1.root",   	// 5		
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

	TFile *file1 = TFile::Open("/Users/joshfrandsen/test/jetty/output/ratio_gamma_inel.root");
	TH1F *prompt = (TH1F*)file1->Get("gammaPrompt");
	TH1F *pi0 = (TH1F*)file1->Get("pi0");
	TH1F *norm1 = (TH1F*)file1->Get("norm");

	double sig = norm1->GetBinContent(1);
	double weight = norm1->GetBinContent(2);

	prompt->Scale( sig/ (weight * prompt->GetBinWidth(1)));
	pi0->Scale( sig/ (weight * pi0->GetBinWidth(1)));

	TH1F *gamma_prompt[6];
	TH1F *gamma_decay[6];
	TH1F *norm[6];

	double sigma[6];
	double weightSum[6];
	double pub_dataX[12] = {3.3, 3.7, 4.1, 4.7, 5.0, 5.7, 6.0, 6.5, 7.5, 10.0, 10.5, 13.0};
	double pub_dataY[12] = {1.2e-2, 1.8e-2, 2.0e-2, 2.3e-2, 3.0e-2, 3.5e-2, 4.0e-2, 4.5e-2, 5.5e-2, 7.0e-2, 8.1e-2, 0.1};

	TGraph *pub_data = new TGraph(12,pub_dataX, pub_dataY);

	// FILLING ARRAYS WITH HISTOGRAMS FROM FILES AND GETTING NORMALIZATION VALUES
	// ###########################################################################
	for (int i =0; i < 6; i++)
	{
		gamma_prompt[i] = (TH1F*)files[i]->Get("gammaPrompt"); // filter for hard gamma
		gamma_decay[i] = (TH1F*)files[i]->Get("pi0"); 
		norm[i] = (TH1F*)files[i]->Get("norm");

		sigma[i] = norm[i]->GetBinContent(1);
		weightSum[i] = norm[i]->GetBinContent(2);
	}
	// NORMALIZTION
	// ############################################################################
	for (int i = 0; i < 6; i++)
	{
		gamma_prompt[i]->Scale( sigma[i] / (weightSum[i] * gamma_prompt[i]->GetBinWidth(1)) );
		gamma_decay[i]->Scale( sigma[i] / (weightSum[i] * gamma_decay[i]->GetBinWidth(1)) ) ;
	}

	// GETT SUM OF EACH PT HAT BIN
	// #############################################################################
	TH1F *gamma_prompt_sum = (TH1F*)gamma_prompt[0]->Clone("gamma_prompt_sum");
	TH1F *gamma_decay_sum = (TH1F*)gamma_decay[0]->Clone("gamma_decay_sum");

	for(int i=1; i < 6; i++)
	{
		gamma_prompt_sum->Add(gamma_prompt[i]);
		gamma_decay_sum->Add(gamma_decay[i]);
	}

	prompt->Add(gamma_prompt_sum);
	pi0->Add(gamma_decay_sum);

	TH1F *prompt_decay_ratio = (TH1F*)gamma_prompt_sum->Clone("prompt_decay_ratio"); // for ratio
	TH1F *prompt_ratio = (TH1F*)prompt->Clone("prompt_ratio"); // for ratio

	// DRAWING HISTOGRAMS
	// #############################################################################
	TCanvas *c1 = new TCanvas("gamma_prompt_decay");
	TCanvas *c2 = new TCanvas("gamma_prompt_decay_ratio");

	gamma_prompt_sum->SetLineColor(kRed);
	gamma_prompt_sum->SetMarkerColor(kRed);
	gamma_prompt_sum->SetLineWidth(2);
	gamma_prompt_sum->SetMarkerStyle(24);
	gamma_prompt_sum->SetXTitle("p_{T}^{#gamma} GeV");
	gamma_prompt_sum->SetTitle("Prompt and Decay #gamma");
	gamma_prompt_sum->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [pb / GeV]");

	gamma_decay_sum->SetLineColor(kRed+2);
	gamma_decay_sum->SetLineWidth(2);
	gamma_decay_sum->SetMarkerStyle(24+1);
	gamma_decay_sum->SetMarkerColor(kRed+2);
	gamma_decay_sum->SetTitle("Decay #gamma #sqrt{s} = 7 TeV");
	gamma_decay_sum->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [pb / GeV] ");

	prompt_decay_ratio->Divide(gamma_decay_sum);
	prompt_decay_ratio->SetMarkerStyle(24);
	prompt_decay_ratio->SetMarkerColor(kRed);
	prompt_decay_ratio->SetYTitle("Ratio");
	prompt_decay_ratio->SetXTitle("p_{T} [GeV]");
	prompt_decay_ratio->SetTitle("prompt #gamma/ decay #gamma ");

	prompt_ratio->Divide(pi0);
	prompt_ratio->SetMarkerStyle(24);
	prompt_ratio->SetMarkerColor(kRed);
	prompt_ratio->SetLineColor(kRed);	
	prompt_ratio->SetYTitle("Ratio");
	prompt_ratio->SetXTitle("p_{T} [GeV]");
	prompt_ratio->SetTitle("prompt #gamma/ #pi^{0} ");
	
	prompt->SetMarkerStyle(24);
	prompt->SetMarkerColor(kRed);
	prompt->SetLineColor(kRed);
	prompt->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb / GeV] ");
	prompt->SetXTitle("p_{T} [GeV]");

	pi0->SetMarkerStyle(24);
	pi0->SetLineColor(kBlack);
	pi0->SetMarkerColor(kBlack);
	pi0->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb / GeV] ");
	pi0->SetXTitle("p_{T} [GeV]");

	pub_data->SetMarkerStyle(20);
	pub_data->SetMarkerColor(kBlack);

	c1->cd();
	prompt->Draw("pe");
	pi0->Draw("same pe");

	c2->cd();
	// prompt_ratio->Rebin(2);
	// prompt_ratio->Scale(3);
	prompt_ratio->Draw("pe");
	pub_data->Draw("same p");

	c1->SetLogy();
	c2->SetLogy();
	c2->SetLogx();

	// MAKE & DRAW LEGEND
	// #############################################################################	
	TLegend *leg1 = new TLegend();
	TLegend *leg2 = new TLegend();
	TLegend *leg3 = new TLegend();
	TLegend *leg4 = new TLegend();

	leg1->AddEntry((TObject*)0, "|#eta| < 1.37", " ");
	leg1->AddEntry((TObject*)0, " #sqrt{s} = 5.5 TeV", " ");

	leg2->AddEntry(prompt, "#gamma_{prompt}");
	leg2->AddEntry(pi0, "#pi^{0}");

	leg3->AddEntry((TObject*)0, "#sqrt{s} = 5.5 TeV");
	leg4->AddEntry(prompt_ratio, "PYTHIA8","pl");
	leg4->AddEntry(pub_data, "NLO Prediciton (from paper) ", "pl");

	c1->cd();
	leg1->Draw("same");
	c2->cd();
	leg3->Draw("same");
	leg4->Draw("same");
}




