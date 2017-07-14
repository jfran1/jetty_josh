
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

	TH1F *gamma_prompt[6];
	TH1F *gamma_decay[6];
	TH1F *norm[6];

	double sigma[6];
	double weightSum[6];

	// FILLING ARRAYS WITH HISTOGRAMS FROM FILES AND GETTING NORMALIZATION VALUES
	// ###########################################################################
	for (int i =0; i < 6; i++)
	{
		gamma_prompt[i] = (TH1F*)files[i]->Get("gammaPrompt"); // filter for hard gamma
		gamma_decay[i] = (TH1F*)files[i]->Get("decayGamma"); 
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


	TH1F *prompt_decay_ratio = (TH1F*)gamma_prompt_sum->Clone("prompt_decay_ratio"); // for ratio

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

	c1->cd();
	gamma_prompt_sum->Draw("pe");
	gamma_decay_sum->Draw("same pe");

	c2->cd();
	prompt_decay_ratio->Rebin(2);
	prompt_decay_ratio->Draw("pe");

	c1->SetLogy();
	c2->SetLogy();

	// MAKE & DRAW LEGEND
	// #############################################################################	
	TLegend *leg1 = new TLegend();
	TLegend *leg2 = new TLegend();
	TLegend *leg3 = new TLegend();

	leg1->AddEntry((TObject*)0, "|#eta| < 1.37", " ");
	leg1->AddEntry((TObject*)0, " #sqrt{s} = 5.5 TeV", " ");

	leg2->AddEntry(gamma_prompt_sum, "#gamma_{prompt}");
	leg2->AddEntry(gamma_decay_sum, "#gamma_{decay}");

	leg3->AddEntry((TObject*)0, "#sqrt{s} = 5.5 TeV");

	c1->cd();
	leg1->Draw("same");
	c2->cd();
	leg3->Draw("same");
}




