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

void alicePlot()
{
	const char *fnames[] = {
		"/Users/joshfrandsen/test/jetty/output/alice_2_4.root",
		"/Users/joshfrandsen/test/jetty/output/alice_4_6.root",
		"/Users/joshfrandsen/test/jetty/output/alice_6_9.root",
		"/Users/joshfrandsen/test/jetty/output/alice_9_12.root",
		"/Users/joshfrandsen/test/jetty/output/alice_12_15.root",
		"/Users/joshfrandsen/test/jetty/output/alice_15_20.root",
		"/Users/joshfrandsen/test/jetty/output/alice_20_25.root",
		"/Users/joshfrandsen/test/jetty/output/alice_25_35.root",
		"/Users/joshfrandsen/test/jetty/output/alice_35_45.root",
		"/Users/joshfrandsen/test/jetty/output/alice_45_60.root",
		"/Users/joshfrandsen/test/jetty/output/alice_60_80.root",
		"/Users/joshfrandsen/test/jetty/output/alice_80_120.root",
		"/Users/joshfrandsen/test/jetty/output/alice_120_180.root",
		"/Users/joshfrandsen/test/jetty/output/alice_180_250.root",
		"/Users/joshfrandsen/test/jetty/output/alice_250_350.root",
		"/Users/joshfrandsen/test/jetty/output/alice_350_1.root",
		"/Users/joshfrandsen/test/jetty/output/HEPData-ins1241422-v1-Table1.root",
		"/Users/joshfrandsen/test/jetty/output/inel7TeV.root",
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

	files[16]->cd("Table 1");
	TGraph *realData = (TGraph*)gDirectory->Get("Graph1D_y3");

	TFile *fout = TFile::Open("7TeV_Hard_Alice.root", "RECREATE");
	fout->cd();
	
	// Setting up arrays of histograms and files
	TH1F *aliceSkeleton[16];

	aliceSkeleton[0] = (TH1F*)files[0]->Get("skeleton");

	TF1 *fun = new TF1("fun","[0]+[1]*x",0,100);
    fun->SetParameter(0, 0);
    fun->SetParameter(1,1);

	TH1F *data[16];
	double sigma[16];
	double weightSum[16];
	double dEta = 0.8;

	TH1F *inel7TeV = (TH1F*)files[17]->Get("skeleton");
	TH1F *inelData = (TH1F*)files[17]->Get("data");
	double inelSig = inelData->GetBinContent(1);
	double inelWeight = inelData->GetBinContent(2); 
	
	int numBins = (aliceSkeleton[0]->GetSize()); // 2 extra for over/under flow
	double skeletonBinWidth[numBins];

	for(int j=1; j < numBins; j++)
	{	
		skeletonBinWidth[j] = aliceSkeleton[0]->GetBinWidth(j);
		inel7TeV->SetBinContent(j, inel7TeV->GetBinContent(j) / skeletonBinWidth[j] );
		// cout << "inel bin content: " << inel7TeV->GetBinContent(j) << endl;
	}

	inel7TeV->Scale(inelSig / (2*TMath::Pi() * inelWeight * 2*dEta) );
	inel7TeV->Divide(fun);


	// setting values and normalizing
	for(int i=0; i < 16; i++)
	{
		aliceSkeleton[i] = (TH1F*)files[i]->Get("skeleton");
		data[i] = (TH1F*)files[i]->Get("data");
		sigma[i] = data[i]->GetBinContent(1);
		weightSum[i] = data[i]->GetBinContent(2);

		for(int j=1; j < numBins; j++)
		{
			aliceSkeleton[i]->SetBinContent(j, (aliceSkeleton[i]->GetBinContent(j)) / skeletonBinWidth[j]);
		}

		aliceSkeleton[i]->Scale(sigma[i] / (2*TMath::Pi() * weightSum[i] * 2*dEta) );
		aliceSkeleton[i]->Divide(fun);
	}

	// Summing the histograms
	TH1F *hsum[3] = {0, 0, 0};
	for (int j =0; j < 3; j++)
	{
		for (int i = j; i<16; i++)
		{
			if (hsum[j] == 0)
			{
				TString newName = TString::Format("%s_%d_%d_clone", aliceSkeleton[i]->GetName(), j, i);
				hsum[j] = (TH1F*)aliceSkeleton[i]->Clone(newName.Data());
				TString newTitle = TString::Format("#hat{p_{T}} bins - from %d hatbin", j);
				hsum[j]->SetTitle(newTitle.Data());
				hsum[j]->SetDirectory(0);
				cout << "[i] cloning " << aliceSkeleton[i]->GetName() << endl;
			}
			else
			{
				hsum[j]->Add(aliceSkeleton[i]);
				cout << "[i] adding " << i << " " << aliceSkeleton[i]->GetName() << " to " << hsum[j]->GetName() << endl;
			}
		}
	}

	//ratio between pythia8 and Alice data
	TH1F *copyAlice= (TH1F*)aliceSkeleton[0]->Clone();
	copyAlice->Divide(inel7TeV);

	//Ploting
	TCanvas *c1 = new TCanvas("pTDist");
	realData->SetMarkerStyle(24);
	realData->SetLineColor(kBlack);

	TLegend *leg = new TLegend();
	for (int j = 0; j < 3; j++)
	{
		hsum[j]->SetMinimum(1.e-12);
		hsum[j]->SetMaximum(5.e4);
		hsum[j]->SetLineColor(kRed);
		hsum[j]->SetLineStyle(1 + j);
		hsum[j]->SetLineWidth(2);
		if (j == 0)
		{
			hsum[j]->Draw("same hist L");
			hsum[j]->SetYTitle("1/(2#pip_{T})d^{2}#sigma/(d#etadp_{T}) [mb c^{2}/GeV^{2}]");
		}

		else
			hsum[j]->Draw("same hist L");			
			leg->AddEntry(hsum[j], hsum[j]->GetTitle());
	}
	realData->Draw(" P same");

	inel7TeV->SetLineColor(kBlue);
	inel7TeV->SetLineWidth(2);
	inel7TeV->SetLineStyle(9);
	inel7TeV->Draw("same hist L");

	leg->AddEntry(inel7TeV, "Inelastic");
	leg->AddEntry(realData, "Alice Data", "pl");
	leg->Draw("same");

	c1->SetLogy();
	c1->SetLogx();

	inel7TeV->Write("inel7TeV");
	aliceSkeleton[0]->Write("pythia8");
	copyAlice->Write("ratio");
	realData->Write("AliceData");
	fout->Close();

	inel7TeV->SetName("Inelastic");
	TH1F *hratios[4];
	for (int j =0; j < 3; j++)
	{
		hratios[j] = ratio_to_a_graph(hsum[j], realData);
	}
	hratios[3] = ratio_to_a_graph(inel7TeV, realData);
	TCanvas *c2 = new TCanvas("pTDist_ratios");

	for (int j =0; j < 4; j++)
	{
		if (j == 0)
			hratios[j]->Draw("hist l");
		else
			hratios[j]->Draw("hist l same");
	}
	gPad->BuildLegend();
}