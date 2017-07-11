// Takes ratio of TH1 with respect to TGraph
// ###########################################################################
TH1F *ratio_to_a_graph(TH1F *h, TGraph *gr)
{
	TString newName = TString::Format("%s_ratio_to_%s", h->GetName(), gr->GetName());
	TH1F *reth = (TH1F*)h->Clone(newName.Data());
	for (int i = 1; i < reth->GetNbinsX(); i++)
	{
		double x = reth->GetBinCenter(i);
		double r = reth->GetBinContent(i) / gr->Eval(x);
		reth->SetMinimum(0);
		reth->SetMaximum(3);
		reth->SetBinContent(i, r);
		// reth->SetBinError(i, 0);
	}
	return reth;
}



// Start main plot
void atlas_plot()
{
	//################ OPENING FILES #################
	const char *fnames[] = {
		"/Users/joshfrandsen/test/jetty/output/atlas_prompt_50_100.root",   	// 0
		"/Users/joshfrandsen/test/jetty/output/atlas_prompt_100_200.root",   	// 1
		"/Users/joshfrandsen/test/jetty/output/atlas_prompt_200_300.root",   	// 2
		"/Users/joshfrandsen/test/jetty/output/atlas_prompt_300_500.root",   	// 3
		"/Users/joshfrandsen/test/jetty/output/atlas_prompt_500_700.root",   	// 4
		"/Users/joshfrandsen/test/jetty/output/atlas_prompt_700_-1.root",   	// 5		
		"/Users/joshfrandsen/test/jetty/output/atlas_data.root"         ,		// 6
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

	
	files[6]->cd("Table 1");
	TGraph *atlas_data = (TGraph*)gDirectory->Get("Graph1D_y1");

	TH1F *gamma_prompt[6];
	TH1F *gamma_decay[6];
	TH1F *jet[6];
	TH1F *norm[6];

	double sigma[6];
	double weightSum[6];

	// FILLING ARRAYS WITH HISTOGRAMS FROM FILES AND GETTING NORMALIZATION VALUES
	// ###########################################################################
	for (int i =0; i < 6; i++)
	{
		gamma_prompt[i] = (TH1F*)files[i]->Get("gammaPrompt"); // filter for hard gamma
		gamma_decay[i] = (TH1F*)files[i]->Get("decayGamma"); 
		jet[i] = (TH1F*)files[i]->Get("jet");
		norm[i] = (TH1F*)files[i]->Get("norm");

		sigma[i] = norm[i]->GetBinContent(1);
		weightSum[i] = norm[i]->GetBinContent(2);
	}
	// NORMALIZTION
	// ############################################################################
	for (int i = 0; i < 6; i++)
	{
		gamma_prompt[i]->Scale( (1.e9 * sigma[i]) / (weightSum[i] * gamma_prompt[i]->GetBinWidth(1)) );
		gamma_decay[i]->Scale( (1.e9 * sigma[i]) / (weightSum[i] * gamma_decay[i]->GetBinWidth(1)) ) ;
		jet[i] -> Scale( (1.e9 * sigma[i]) / (weightSum[i] * jet[i]->GetBinWidth(1)) );
	}

	// GETT SUM OF EACH PT HAT BIN
	// #############################################################################
	TH1F *gamma_prompt_sum = (TH1F*)gamma_prompt[0]->Clone("gamma_prompt_sum");
	TH1F *gamma_decay_sum = (TH1F*)gamma_decay[0]->Clone("gamma_decay_sum");
	TH1F *jet_sum = (TH1F*)jet[0]->Clone("jet_sum");

	for(int i=1; i < 6; i++)
	{
		gamma_prompt_sum->Add(gamma_prompt[i]);
		gamma_decay_sum->Add(gamma_decay[i]);
		jet_sum->Add(jet[i]);
	}

	TH1F *prompt_decay_ratio = (TH1F*)gamma_prompt_sum->Clone("prompt_decay_ratio"); // for ratio
	TH1F *gamma_ratio = ratio_to_a_graph(gamma_prompt_sum, atlas_data); //for Ratio

	// DRAWING HISTOGRAMS
	// #############################################################################
	TCanvas *c1 = new TCanvas("gamma_prompt_pTHatBins");
	TCanvas *c2 = new TCanvas("gamma_atlas_pythia_ratio");
	TCanvas *c3 = new TCanvas("gamma_atlas_pythia");
	TCanvas *c4 = new TCanvas("decay_cross_section");
	TCanvas *c5 = new TCanvas("prompt_decay_ratio");
	TCanvas *c6 = new TCanvas("Jet");

	for(int i=0;i<6; i++)
	{
		gamma_prompt[i]->SetLineColor(kAzure+i);
		gamma_prompt[i]->SetMarkerColor(kAzure+i);
		gamma_prompt[i]->SetMarkerStyle(24+i);
		gamma_prompt[i]->SetLineWidth(2);
		gamma_prompt[i]->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [pb GeV]");
		gamma_prompt[i]->SetTitle("prompt p_{T}^{#gamma}  #sqrt{s} = 7 TeV #hat{p_{T}} > 50 GeV");
		gamma_prompt[i]->SetMinimum(1.e-6);

		if(i==0)
		{
			c1->cd();
			gamma_prompt[i]->Draw("pe");
		}
		else
		{
			c1->cd();
			gamma_prompt[i]->Draw("same pe");
		}
	}
	
	gamma_prompt_sum->SetLineColor(kRed);
	gamma_prompt_sum->SetMarkerColor(kRed);
	gamma_prompt_sum->SetLineWidth(2);
	gamma_prompt_sum->SetMarkerStyle(24);
	gamma_prompt_sum->SetXTitle("p_{T}^{#gamma} GeV");
	gamma_prompt_sum->SetTitle("ATLAS vs. PYTHIA8");
	gamma_prompt_sum->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [pb GeV]");

	gamma_ratio->SetMarkerStyle(24);
	gamma_ratio->SetMarkerColor(kRed);
	gamma_ratio->SetLineColor(kRed);
	gamma_ratio->SetYTitle("Ratio");
	gamma_ratio->SetXTitle("p_{T} [GeV]");
	gamma_ratio->SetTitle("PYTHIA8 (All but decay photons)/ATLAS ");

	gamma_decay_sum->SetLineColor(kRed);
	gamma_decay_sum->SetLineWidth(2);
	gamma_decay_sum->SetMarkerStyle(24);
	gamma_decay_sum->SetMarkerColor(kRed);
	gamma_decay_sum->SetTitle("Decay #gamma #sqrt{s} = 12 TeV");
	gamma_decay_sum->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} ");

	jet_sum->SetLineColor(kRed);
	jet_sum->SetLineWidth(2);
	jet_sum->SetMarkerStyle(24);
	jet_sum->SetMarkerColor(kRed);
	jet_sum->SetTitle("#gamma+jet #sqrt{s} = 12 TeV");
	jet_sum->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} ");

	prompt_decay_ratio->Divide(gamma_decay_sum);
	prompt_decay_ratio->SetMarkerStyle(24);
	prompt_decay_ratio->SetMarkerColor(kRed);
	prompt_decay_ratio->SetYTitle("Ratio");
	prompt_decay_ratio->SetXTitle("p_{T} [GeV]");
	prompt_decay_ratio->SetTitle("prompt #gamma/ decay #gamma ");


	c2->cd();
	gamma_ratio->Draw("pe");

	c3->cd();
	gamma_prompt_sum->Draw("pe");
	atlas_data->Draw("same");

	c4->cd();
	gamma_decay_sum->Draw("pe");
		
	c5->cd();
	prompt_decay_ratio->Draw("pe");

	c6->cd();
	jet_sum->Draw("pe");


	c1->SetLogy();
	c3->SetLogy();
	c4->SetLogy();
	c6->SetLogy();

	// MAKE & DRAW LEGEND
	// #############################################################################	
	TLegend *leg1 = new TLegend();
	TLegend *leg2 = new TLegend();
	TLegend *leg3 = new TLegend();
	TLegend *leg4 = new TLegend();
	TLegend *leg5 = new TLegend(); 
	TLegend *leg6 = new TLegend(); 
	TLegend *leg7 = new TLegend();

	
	leg1->AddEntry(gamma_prompt[0], "50 < #hat{p_{T}} < 100 GeV");
	leg1->AddEntry(gamma_prompt[1], "100 < #hat{p_{T}} < 200 GeV");
	leg1->AddEntry(gamma_prompt[2], "200 < #hat{p_{T}} < 300 GeV");
	leg1->AddEntry(gamma_prompt[3], "300 < #hat{p_{T}} < 500 GeV");	
	leg1->AddEntry(gamma_prompt[4], "500 < #hat{p_{T}} < 700 GeV");
	leg1->AddEntry(gamma_prompt[5], "700 < #hat{p_{T}} < inf GeV");

	leg2->AddEntry((TObject*)0, "|#eta^{#gamma}| < 1.37", " ");
	leg2->AddEntry((TObject*)0, " #sqrt{s} = 7 TeV", " ");

	leg3->AddEntry(gamma_prompt_sum, "PYTHIA8" );
	leg3->AddEntry(atlas_data, "ATLAS","pl" );

	c1->cd();
	leg1->Draw("same");

	c3->cd();
	leg3->Draw("same");
	leg2->Draw("same");
}




