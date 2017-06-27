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

void chargedJetSigma()
{
	const char *fnames[] = {
		"/Users/joshfrandsen/test/jetty/output/chargedJet_1.root",
		"/Users/joshfrandsen/test/jetty/output/chargedJet_2.root",
		"/Users/joshfrandsen/test/jetty/output/chargedJet_3.root",
		"/Users/joshfrandsen/test/jetty/output/chargedJet_4.root",
		"/Users/joshfrandsen/test/jetty/output/chargedJet_5.root",
		"/Users/joshfrandsen/test/jetty/output/chargedJet_6.root",
		"/Users/joshfrandsen/test/jetty/output/chargedJet_7.root",
		"/Users/joshfrandsen/test/jetty/output/chargedJet_8.root",
		"/Users/joshfrandsen/test/jetty/output/chargedJet_9.root",
		"/Users/joshfrandsen/test/jetty/output/chargedJet_10.root",
		"/Users/joshfrandsen/test/jetty/output/chargedJet_11.root",
		"/Users/joshfrandsen/test/jetty/output/chargedJet_12.root",
		"/Users/joshfrandsen/test/jetty/output/chargedJet_13.root",
		"/Users/joshfrandsen/test/jetty/output/chargedJet_14.root",
		"/Users/joshfrandsen/test/jetty/output/chargedJet_15.root",
		"/Users/joshfrandsen/test/jetty/output/chargedJet_16.root",
		"/Users/joshfrandsen/test/jetty/output/aliceChargedJetData.root",
		"/Users/joshfrandsen/test/jetty/output/inelChargedJet.root",
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
	TGraph *realData = (TGraph*)gDirectory->Get("Graph1D_y1");
	
	// Setting up arrays of histograms and files
	TH1F *jetpT[16];
	jetpT[0] = (TH1F*)files[0]->Get("hpT");
	TH1F *data[16];
	TH2F *hat2inel[16];
	double sigma[16];
	double weightSum[16];
	double dEta = 0.5;
	double binWidth=jetpT[0]->GetBinWidth(1);

	TH1F *inel = (TH1F*)files[17]->Get("hpT");
	TH1F *dataInel = (TH1F*)files[17]->Get("data");
	double inelSig = dataInel->GetBinContent(1);
	double inelWeight = dataInel->GetBinWidth(2);
	double inelWidth = inel->GetBinWidth(1);
	double scalingRatio = 1.58177;

	inel->Scale( (inelSig) / (inelWeight * inelWidth * 2*dEta) ); 
	double totSig = 0;
	TCanvas *c3 = new TCanvas();
	// setting values and normalizing
	for(int i=0; i < 16; i++)
	{
		jetpT[i] = (TH1F*)files[i]->Get("hpT");
		hat2inel[i] = (TH2F*)files[i]->Get("hat2inel");
		data[i] = (TH1F*)files[i]->Get("data");
		sigma[i] = data[i]->GetBinContent(1);
		weightSum[i] = data[i]->GetBinContent(2);
		totSig += sigma[i];

		jetpT[i]->Scale( (sigma[i] * 1.e6 ) / ( weightSum[i] * 2*dEta * binWidth) );
		jetpT[i]->Sumw2();
		if (i == 0)
		{
			jetpT[i]->Draw("hist l");
			jetpT[i]->SetMinimum(10e-10);
			jetpT[i]->SetLineColor(kBlue);
			jetpT[i]->SetTitle("Chrged Jets");
			jetpT[i]->SetYTitle("d^{2}#sigma/(d#etadp_{T}) [mb c^{2}/GeV^{2}]");
			realData->Draw("same pl");
		}
		else
		{
			jetpT[i]->Draw("same hist l ");
			jetpT[i]->SetLineColor(kAzure+(i-1) );
		}
	}


	TString newName = TString::Format("%s_sum", jetpT[0]->GetName());
	TString newName2 = TString::Format("%s_sum", hat2inel[0]->GetName());
	TH1F *hsum = (TH1F*)jetpT[0]->Clone(newName.Data());
	TH2F *h2Sum = (TH2F*)hat2inel[0]->Clone(newName2.Data());
	for(int i=0;i<16; i++) 
	{
		hsum->Add(jetpT[i]);
		h2Sum->Add(hat2inel[i]);

	}
	cout << "Total pT hat sigma: " << totSig << endl;
	cout << "Inelastic sigma   : " << inelSig << endl;
	cout << " InelSig/TotalSig : " << inelSig/totSig << endl;

	//Ploting
	TCanvas *c1 = new TCanvas("pTDist");
	TLegend *leg = new TLegend();
	
	hsum->Draw("pe");
	hsum->SetMarkerStyle(24);
	hsum->SetMarkerColor(kRed);
	realData->SetMarkerStyle(24);
	realData->SetLineColor(kBlack);
	hsum->SetMaximum(1.0e5);
	hsum->SetLineColor(kRed);
	hsum->SetLineStyle(1);
	hsum->SetLineWidth(2);
/*
	inel->SetLineWidth(2);
	inel->SetLineColor(kBlue);
	inel->SetLineStyle(2);
	inel->Draw("same pe");
*/
		
	realData->Draw("same pl");
	hsum->SetYTitle("d^{2}#sigma/(d#etadp_{T}) [mb c^{2}/GeV^{2}]");			
	leg->AddEntry(hsum, hsum->GetTitle());
	leg->AddEntry(realData, "Alice Data", "pl");
	leg->Draw("same");

	c1->SetLogy();
	c3->SetLogy();


	//Getting Ratio
	TH1F *hratio;
	hratio = ratio_to_a_graph(hsum, realData);
	TCanvas *c2 = new TCanvas("pTDist_ratio");
	hratio->Draw("hist l");
	hratio->SetTitle("Pythia8/Alice");
	hratio->SetYTitle("Ratio");
	//gPad->BuildLegend();

   
	TFile *fout = TFile::Open("chrgedJetSigma.root", "RECREATE");
	fout->cd();

	jetpT[0]->Write("pythia8");
	realData->Write("AliceData");
	hsum->Write();
	fout->Close();

	TCanvas *c4 = new TCanvas();
	c4->SetLogz();
	h2Sum->Draw("contz");


}