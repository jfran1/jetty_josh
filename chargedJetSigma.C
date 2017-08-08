TH1F *ratio_to_a_graph(TH1F *h, TGraph *gr)
{
	TString newName = TString::Format("%s_ratio_to_%s", h->GetName(), gr->GetName());
	TH1F *reth = (TH1F*)h->Clone(newName.Data());
	for (int i = 1; i <= reth->GetNbinsX(); i++)
	{
		double x = reth->GetBinCenter(i);
		double r = reth->GetBinContent(i) / gr->Eval(x);
		// reth->SetMinimum(0);
		// reth->SetMaximum(3);
		reth->SetBinContent(i, r);
		reth->SetBinError(i, 0);
	}
	return reth;
}

void chargedJetSigma()
{
	const char *fnames[] = {
		"/Users/joshfrandsen/test/jetty/output/chargedJet_10_20.root",    // 0
		"/Users/joshfrandsen/test/jetty/output/chargedJet_20_30.root",    // 1
		"/Users/joshfrandsen/test/jetty/output/chargedJet_30_50.root",    // 2
		"/Users/joshfrandsen/test/jetty/output/chargedJet_50_80.root",    // 3 
		"/Users/joshfrandsen/test/jetty/output/chargedJet_80_-1.root",    // 4
		"/Users/joshfrandsen/test/jetty/output/aliceChargedJetData.root", // 5
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

	files[5]->cd("Table 6");
	TGraph *real_data = (TGraph*)gDirectory->Get("Graph1D_y1");

	float bin_array[12] = {20., 24., 28., 32., 38., 44., 50., 58., 67., 76., 87., 100. };
	int num_bins = sizeof(bin_array) - 1;
	TH1F *jet_pt = new TH1F("jet_pt", "Charged Jet", 11, bin_array);

	// Setting up arrays of histograms and files
	TChain jet("jet");
	TH1F *norm[5];
	double weightSum[5];
	double dEta = 0.9;

	for(int i =0; i < 5; i++)
	{
		norm[i]   = (TH1F*)files[i]->Get("norm");
		weightSum[i] = norm[i]->GetBinContent(2);
		jet.Add(fnames[i]);
	}

   	TCanvas *c1 = new TCanvas("charged_jet_pT");
	TCanvas *c2 = new TCanvas("pTDist_ratio");
	TCanvas *temp = new TCanvas();
	
	// write data to hists
	jet.Draw("pt>>jet_pt");
	temp->Close();
	
	// scale hists
	jet_pt->Scale( 1.e6 / ( weightSum[1] * 2*dEta));
	for( int i =1; i < jet_pt->GetSize() - 1; i++)
	{
		jet_pt->SetBinContent( i, jet_pt->GetBinContent(i) / jet_pt->GetBinWidth(i) );
	}

	// Get Ratio
	TH1F *ratio = ratio_to_a_graph(jet_pt, real_data);

	TLegend *leg1 = new TLegend();
	TLegend *leg2 = new TLegend();


	leg1->AddEntry(jet_pt,"PYTHIA8", "pl");
	leg1->AddEntry(real_data,"ALICE", "pl");

	leg2->AddEntry((TObject*)0, "#sqrt{s} = 7 TeV", " ");
	leg2->AddEntry((TObject*)0, "Anti-kt R=0.4" , " ");
	leg2->AddEntry((TObject*)0, "|#eta|<0.9   |eta_{jet}| < 0.5 " , " ");

   	jet_pt->SetLineColor(kRed);
   	jet_pt->SetMarkerColor(kRed);
   	jet_pt->SetMarkerStyle(26);
   	jet_pt->SetLineWidth(2);
   	jet_pt->SetYTitle("d^{2}#sigma/(d#etadp_{T}) [mb GeV^{-1}]");
   	jet_pt->SetXTitle("p_{T} [Gev]");

   	ratio->SetLineColor(kRed);
   	ratio->SetMarkerStyle(24);
   	ratio->SetMarkerColor(kRed);
   	ratio->SetYTitle("Ratio");
   	ratio->SetTitle("PYTHIA8/ALICE");

   	real_data->SetLineColor(kBlack);
   	real_data->SetMarkerStyle(20);
   	real_data->SetMarkerColor(kBlack);

	c1->cd();
	jet_pt->Draw("pe ");
	real_data->Draw("same pe");
	leg1->Draw("same");
	leg2->Draw("same");

	c2->cd();
	ratio->Draw("p");
  	
	c1->SetLogy();


}