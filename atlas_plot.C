void atlas_plot()
{
	//################ OPENING FILES #################
	const char *fnames[] = {
		"/Users/joshfrandsen/test/jetty/output/prompt_atlas_100_200.root",   	// 0
		"/Users/joshfrandsen/test/jetty/output/prompt_atlas_200_300.root",   	// 1
		"/Users/joshfrandsen/test/jetty/output/prompt_atlas_300_500.root",   	// 2
		"/Users/joshfrandsen/test/jetty/output/prompt_atlas_500_700.root",   	// 3
		"/Users/joshfrandsen/test/jetty/output/prompt_atlas_700_1000.root",   	// 4
		"/Users/joshfrandsen/test/jetty/output/atlas_data.root"          ,		// 5
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


	
	files[5]->cd("Table 1");
	TH1F *atlas_data = (TH1F*)gDirectory->Get("Hist1D_y1");
	TH1F *atlas_error = (TH1F*)gDirectory->Get("Hist1D_y1_e1");

	int num_bins = atlas_error->GetSize()-2;
	double a_atlas_error[num_bins];
	for(int i=0; i < num_bins ; i++)
	{
		a_atlas_error[i] = atlas_error->GetBinContent(i+1);
	} 
	atlas_data->SetError(a_atlas_error);

	TH1F *gamma_prompt[5];
	TH1F *norm[5];

	double sigma[5];
	double weightSum[5];

	// FILLING ARRAYS WITH HISTOGRAMS FROM FILES AND GETTING NORMALIZATION VALUES
	// ###########################################################################
	for (int i =0; i < 5; i++)
	{
		gamma_prompt[i] = (TH1F*)files[i]->Get("gamma_prompt"); 
		norm[i] = (TH1F*)files[i]->Get("norm");

		sigma[i] = norm[i]->GetBinContent(1);
		weightSum[i] = norm[i]->GetBinContent(2);
	}

	for(int i = 0; i < 5; i++)
	{
		for (int j=1; j < gamma_prompt[0]->GetSize()-1; j++)
		{
			std::cout << "[i] file " << i << " bin " << j << ": "  << gamma_prompt[i]->GetBinContent(j) << std::endl;
		}
	}


	// NORMALIZTION
	// ############################################################################
	for (int i = 0; i < 5; i++)
	{
		for(int j=1; j < gamma_prompt[i]->GetSize()-1;j++)
		{
			gamma_prompt[i]->SetBinContent(j, gamma_prompt[i]->GetBinContent(j) / gamma_prompt[i]->GetBinWidth(j) );
		}
		
		gamma_prompt[i]->Scale( (1.e9 * sigma[i]) / weightSum[i] );
	}

	// GETT SUM OF EACH PT HAT BIN
	// #############################################################################
	TH1F *gamma_prompt_sum = (TH1F*)gamma_prompt[0]->Clone("gamma_prompt_sum");

	for(int i=1; i < 5; i++)
	{
		gamma_prompt_sum->Add(gamma_prompt[i]);
	}


	TH1F *gamma_ratio = (TH1F*)gamma_prompt_sum->Clone("gamma_ratio");
	gamma_ratio->Divide(atlas_data);

	// DRAWING HISTOGRAMS
	// #############################################################################
	TCanvas *c1 = new TCanvas("gamma_prompt");
	TCanvas *c2 = new TCanvas("gamma_atlas_pythia_ratio");
	
	gamma_prompt_sum->SetLineColor(kRed);
	gamma_prompt_sum->SetMarkerColor(kRed);
	gamma_prompt_sum->SetLineWidth(2);
	gamma_prompt_sum->SetMarkerStyle(27);
	gamma_prompt_sum->SetXTitle("p_{T}^{#gamma} GeV");
	gamma_prompt_sum->SetTitle("Prompt Photons ATLAS vs. PYTHIA8");
	gamma_prompt_sum->SetYTitle("#frac{d#sigma}{dp_{T}} [pb / GeV]");

	atlas_data->SetLineColor(kBlack);
	atlas_data->SetMarkerColor(kBlack);
	atlas_data->SetLineWidth(2);
	atlas_data->SetMarkerStyle(20);

	gamma_ratio->SetMarkerStyle(24);
	gamma_ratio->SetMarkerColor(kRed);
	gamma_ratio->SetLineColor(kRed);
	gamma_ratio->SetLineWidth(0);			
	gamma_ratio->SetYTitle("Ratio");
	gamma_ratio->SetXTitle("p_{T} [GeV]");
	gamma_ratio->SetTitle("PYTHIA8/ATLAS ");

	c1->SetLogy();

	// MAKE & DRAW LEGEND
	// #############################################################################	
	TLegend *leg1 = new TLegend();
	TLegend *leg2 = new TLegend();

	
	leg1->AddEntry(gamma_prompt_sum, "PYTHIA8" ,"pl");
	leg1->AddEntry(atlas_data, "ATLAS","pl" );

	leg2->AddEntry((TObject*)0, "|#eta^{#gamma}| < 1.37", " ");
	leg2->AddEntry((TObject*)0, " #sqrt{s} = 7 TeV", " ");


	c1->cd();
	std::cout << "[i] Size: " << gamma_prompt_sum->GetSize() << std::endl;
	for (int i=1; i < gamma_prompt_sum->GetSize() -1 ; i++)
	{
		std:: cout << "[i] bin " << i << " counts: " << gamma_prompt_sum->GetBinContent(i) << std::endl; 
		std::cout << "[i] # Error bin " << i << ": " << gamma_prompt_sum->GetBinError(i) << std::endl;
	}

	gamma_prompt_sum->Draw("pe");
	atlas_data->Draw("same pe");
	leg1->Draw("same");
	leg2->Draw("same");

	c2->cd();
	gamma_ratio->SetMinimum(.5);
	gamma_ratio->SetMaximum(1.5);
	gamma_ratio->Draw("p");

}




