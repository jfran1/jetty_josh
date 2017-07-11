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
		reth->SetBinError(i, 0);
	}
	return reth;
}



// Start main plot
void cmsPlot()
{
	//################ OPENING FILES #################
	const char *fnames[] = {
		"/Users/joshfrandsen/test/jetty/output/gamma_10_20.root",   	// 0
		"/Users/joshfrandsen/test/jetty/output/gamma_20_30.root",   	// 1
		"/Users/joshfrandsen/test/jetty/output/gamma_30_40.root",   	// 2
		"/Users/joshfrandsen/test/jetty/output/gamma_40_50.root",   	// 3
		"/Users/joshfrandsen/test/jetty/output/gamma_50_-1.root",   	// 4
		"/Users/joshfrandsen/test/jetty/output/cms_data.root"   ,
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

	
	files[5]->cd("Table 3");
	TGraph *cms_data = (TGraph*)gDirectory->Get("Graph1D_y1");

	TH1F *gamma_prompt[5];
	TH1F *norm[5];

	double sigma[5];
	double weightSum[5];

	// FILLING ARRAYS WITH HISTOGRAMS FROM FILES AND GETTING NORMALIZATION VALUES
	// ###########################################################################
	for (int i =0; i < 5; i++)
	{
		gamma_prompt[i] = (TH1F*)files[i]->Get("gammaPrompt"); // filter for hard gamma
		norm[i] = (TH1F*)files[i]->Get("norm");

		sigma[i] = norm[i]->GetBinContent(1);
		weightSum[i] = norm[i]->GetBinContent(2);
	}
	// NORMALIZTION
	// ############################################################################
	for (int i = 0; i < 5; i++)
	{
		gamma_prompt[i]->Scale( (1.e9 * sigma[i]) / (weightSum[i] * gamma_prompt[i]->GetBinWidth(1)) );
	}

	// GETT SUM OF EACH PT HAT BIN
	// #############################################################################
	TH1F *gamma_prompt_sum = (TH1F*)gamma_prompt[0]->Clone("gamma_prompt_sum");

	for(int i=1; i < 5; i++)
	{
		gamma_prompt_sum->Add(gamma_prompt[i]);
	}

	// DRAWING HISTOGRAMS
	// #############################################################################
	TCanvas *c1 = new TCanvas("gamma_prompt_pTHatBins");
	TCanvas *c2 = new TCanvas("gamma_cms_pythia_ratio");
	TCanvas *c3 = new TCanvas("gamma_cms_pythia");

	for(int i=0;i<5; i++)
	{
		gamma_prompt[i]->SetLineColor(kAzure+i);
		gamma_prompt[i]->SetMarkerColor(kAzure+i);
		gamma_prompt[i]->SetMarkerStyle(24+i);
		gamma_prompt[i]->SetLineWidth(2);
		gamma_prompt[i]->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb GeV]");
		gamma_prompt[i]->SetTitle("#prompt p_{T}^{#gamma}  #sqrt{s} = 2.76 TeV #hat{p_{T}} > 10 GeV");

		if(i==0)
		{
			c1->cd();
			// gamma_prompt[i]->SetMinimum(1.e-6);
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
	gamma_prompt_sum->SetTitle("CMS vs. PYTHIA8");
	gamma_prompt_sum->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb GeV]");

	TH1F *gamma_ratio = ratio_to_a_graph(gamma_prompt_sum, cms_data);//Get Ratio
	c2->cd();
	gamma_ratio->Draw();
	gamma_ratio->SetYTitle("Ratio");
	gamma_ratio->SetXTitle("p_{T} [GeV]");
	gamma_ratio->SetTitle("PYTHIA8/CMS ");

	// cms_data->SetLineColor(kBlack);
	// cms_data->SetMarkerStyle(kBlack);
	// cms_data->SetMarkerStyle(20);
	// cms_data->SetLineWidth(2);


	c3->cd();
	gamma_prompt_sum->Draw("pe");
	cms_data->Draw("same");
	
	c1->SetLogy();
	c2->SetLogy();
	c3->SetLogy();

	// MAKE & DRAW LEGEND
	// #############################################################################	
	TLegend *leg1 = new TLegend();
	TLegend *leg2 = new TLegend();
	TLegend *leg3 = new TLegend();
	TLegend *leg4 = new TLegend();
	TLegend *leg5 = new TLegend(); 
	TLegend *leg6 = new TLegend(); 
	TLegend *leg7 = new TLegend();

	
	leg1->AddEntry(gamma_prompt[0], "10 < #hat{p_{T}} < 20 GeV");
	leg1->AddEntry(gamma_prompt[1], "20 < #hat{p_{T}} < 30 GeV");
	leg1->AddEntry(gamma_prompt[2], "30 < #hat{p_{T}} < 40 GeV");
	leg1->AddEntry(gamma_prompt[3], "40 < #hat{p_{T}} < 50 GeV");	
	leg1->AddEntry(gamma_prompt[4], "50 < #hat{p_{T}} < inf GeV");

	leg2->AddEntry((TObject*)0, "|#eta^{#gamma}| < 1.44", " ");
	leg2->AddEntry((TObject*)0, " #sqrt{s} = 2.76 TeV", " ");

	leg3->AddEntry(gamma_prompt_sum, "PYTHIA8" );
	leg3->AddEntry(cms_data, "CMS","pl" );

	c1->cd();
	leg1->Draw("same");

	c3->cd();
	leg3->Draw("same");
	leg2->Draw("same");
}




