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
void hadron_sigma()
{
	//################ OPENING FILES #################

	const char *file_names[] =
	{
		"~/test/jetty/output/charged_hadron_10_20.root",  //1
		"~/test/jetty/output/charged_hadron_20_40.root",  //2
		"~/test/jetty/output/charged_hadron_40_60.root",  //3
		"~/test/jetty/output/charged_hadron_60_80.root", //4
		"~/test/jetty/output/charged_hadron_80_-1.root", //5
		0
	};

	int _temp = 0;
	while(file_names[_temp] != 0) _temp++;

	const int nFiles = _temp;
	TFile *files[nFiles];
	for (int i = 0; i < nFiles; i++)
	{
		files[i] = TFile::Open(file_names[i]);
		if (files[i] == 0) std::cout << "[i] ERROR FILE DID NOT OPEN" << std::endl;
	}

	TFile *f1 = TFile::Open("~/test/jetty/output/charged_hadron.root");
	TFile *f2 = TFile::Open("~/test/jetty/output/cms_charged_hadron_data.root");

	f2->cd("Table 5");

	// GET HISTOGRAMS
	// ############################################################################
	TH1F *cms_data = (TH1F*)gDirectory->Get("Hist1D_y1");
	TH1F *cms_error = (TH1F*)gDirectory->Get("Hist1D_y1_e2");
	TH1F *charged_hadron = (TH1F*)f1->Get("charged_hadron");
	TH1F *norm = (TH1F*)f1->Get("norm");
	TH1F *p_norm[nFiles];
	TH1F *p_charged_hadron[nFiles];

	double sigma = norm->GetBinContent(1);
	double weightSum = norm->GetBinContent(2);
	double p_sigma[nFiles];
	double p_weightSum[nFiles];
	double dEta = 1.0;
	double pi = TMath::Pi();

	int num_bins = cms_error->GetNbinsX();
	double a_cms_error[num_bins];
	for(int i =1; i <= num_bins; i++)
	{
		a_cms_error[i-1] = cms_error->GetBinContent(i); 
		cout << a_cms_error[i-1] << endl;
	}
	cms_data->SetError(a_cms_error);

	for(int i =0; i < nFiles; i++)
	{
		p_charged_hadron[i] = (TH1F*)files[i]->Get("charged_hadron");
		p_norm[i] = (TH1F*)files[i]->Get("norm");
		p_sigma[i] = p_norm[i]->GetBinContent(1);
		p_weightSum[i] = p_norm[i]->GetBinContent(2);
	}

	// NORMALIZTION
	// ############################################################################

	for(int i =1; i < charged_hadron->GetSize()-1 ; i++)
	{
		charged_hadron->SetBinContent(i, ( charged_hadron->GetBinContent(i) / charged_hadron->GetBinWidth(i) ) );	
	}

	for (int i=0; i < nFiles; i++ )
	{
		for (int j =1 ; j < p_charged_hadron[i]->GetSize()-1 ;j++)
		{
			p_charged_hadron[i]->SetBinContent(j, (p_charged_hadron[i]->GetBinContent(j) / p_charged_hadron[i]->GetBinWidth(j) ) );
			
		}
		
		p_charged_hadron[i]->Scale(p_sigma[i] / (2*dEta * 2*pi * p_weightSum[i]) );
	}	
	
	charged_hadron->Scale( sigma / (2*dEta * 2*pi * weightSum ) );

	// SUM UP PT HAT BINS
	TH1F *p_charged_sum = (TH1F*)p_charged_hadron[0]->Clone("chaged_hadron_pTHat");
	for(int i=1; i < nFiles; i++) p_charged_sum->Add(p_charged_hadron[i]);

	std::cout << "[i] Bin For 8 Gev: " << p_charged_sum->FindBin(8) << std::endl;
	
	// Combine low pT from inel wiht high pT from pT hat bins
	for(int i =1; i < p_charged_sum->GetSize()-1; i++)
	{
		if(i < 11) p_charged_sum->SetBinContent(i, 0);

		else charged_hadron->SetBinContent(i, 0);
	}

	charged_hadron->Add(p_charged_sum);
	//Get Ratio
	TH1F *ratio = (TH1F*)charged_hadron->Clone("charged_hadron_ratio");
	ratio->Divide(cms_data); // PYTHIA/CMS

	// DRAWING HISTOGRAMS
	// #############################################################################
	TCanvas *c1 = new TCanvas("charged_hadron_sigma");
	TCanvas *c2 = new TCanvas("charged_hadron_sigma_ratio");

	charged_hadron->SetLineColor(kRed);
	charged_hadron->SetMarkerColor(kRed);
	charged_hadron->SetMarkerStyle(27);
	charged_hadron->SetLineWidth(2);
	charged_hadron->SetYTitle("1/(2#pip_{T}) d^{2}#sigma/(d#etadp_{T}) [mb (GeV)^{-2}]");
	charged_hadron->SetTitle("Inelastic Charged Hadron");
	charged_hadron->SetXTitle("p_{T} [GeV]");

	p_charged_sum->SetLineColor(kBlue);
	p_charged_sum->SetMarkerColor(kBlue);
	p_charged_sum->SetMarkerStyle(27);
	p_charged_sum->SetLineWidth(0);

	cms_data->SetLineWidth(2);
	cms_data->SetLineColor(kBlack);
	cms_data->SetMarkerStyle(24);
	cms_data->SetMarkerColor(kBlack);

	ratio->SetLineColor(kRed);
	ratio->SetLineWidth(0);
	ratio->SetMarkerColor(kRed);
	ratio->SetMarkerStyle(24);
	ratio->SetYTitle("RATIO");
	ratio->SetXTitle("p_{T} [GeV]");
	ratio->SetTitle("PYTHIA8/CMS");

	TLegend *l_sigma = new TLegend();
	TLegend *l_specs = new TLegend();
	
	l_sigma->AddEntry(charged_hadron, "PYTHIA8", "pl");
	l_sigma->AddEntry(cms_data, "CMS" , "pl");
	l_sigma->AddEntry(p_charged_sum, "#hat{p_{T}} > 10 GeV ", "pl");

	l_specs->AddEntry((TObject*)0, "pp #sqrt{s} = 2.76 TeV", " ");
	l_specs->AddEntry((TObject*)0, "|#eta| < 1.0", " ");


	c1->cd();
	charged_hadron->Draw(" pe");
	cms_data->Draw("same pe");
	l_sigma->Draw("same");
	l_specs->Draw("same");

	c2->cd();
	ratio->Draw("p");

	c1->SetLogy();

}




