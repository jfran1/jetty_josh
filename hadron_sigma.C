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

	TFile *f1 = TFile::Open("~/test/jetty/output/charged_hadron.root");
	TFile *f2 = TFile::Open("~/test/jetty/output/cms_charged_hadron_data.root");

	f2->cd("Table 4");

	TH1F *cms_data = (TH1F*)gDirectory->Get("Hist1D_y1");
	TH1F *charged_hadron = (TH1F*)f1->Get("charged_hadron");
	TH1F *norm = (TH1F*)f1->Get("norm");

	double sigma = norm->GetBinContent(1);
	double weightSum = norm->GetBinContent(2);
	double binWidth = charged_hadron->GetBinWidth(1);
	double dEta = 2.4;
	double pi = TMath::Pi();

	// NORMALIZTION
	// ############################################################################

	for(int i =1; i < charged_hadron->GetSize()-1 ; i++)
	{
		charged_hadron->SetBinContent(i, ( charged_hadron->GetBinContent(i) / charged_hadron->GetBinWidth(i) ) );
	}

	charged_hadron->Scale( 1 / (2*dEta * 2*pi * weightSum ) );

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
	charged_hadron->SetYTitle("#frac{1}{2#pip_{T}} #frac{d^{2}N_{ch}}{d#etadp_{T}} [(GeV/c)^{-2}]");
	charged_hadron->SetTitle("Inelastic Charged Hadron");
	charged_hadron->SetXTitle("p_{T} [GeV]");

	cms_data->SetLineWidth(0);
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

	l_specs->AddEntry((TObject*)0, "pp #sqrt{s} = 7 TeV", " ");
	l_specs->AddEntry((TObject*)0, "|#eta| < 2.4", " ");


	c1->cd();
	charged_hadron->Draw("pe");
	cms_data->Draw("same p");
	l_sigma->Draw("same");
	l_specs->Draw("same");

	c2->cd();
	ratio->Draw("p");

	c1->SetLogy();

}




