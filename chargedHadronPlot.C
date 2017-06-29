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

void chargedHadronPlot()
{
	const char *fnames[] = {
		"/Users/joshfrandsen/test/jetty/output/hadron_10_20.root",   // 0
		"/Users/joshfrandsen/test/jetty/output/hadron_20_30.root",   // 1
		"/Users/joshfrandsen/test/jetty/output/hadron_30_40.root",   // 2
		"/Users/joshfrandsen/test/jetty/output/hadron_40_50.root",   // 3
		"/Users/joshfrandsen/test/jetty/output/hadron_50_-1.root",   // 4
		"/Users/joshfrandsen/test/jetty/output/inelHadron.root",     // 5
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

	// Setting up arrays of histograms and files
	TH1F *hadronPT[5];
	TH1F *inelHadron;
	TH1F *inelNorm;
	hadronPT[0] = (TH1F*)files[0]->Get("hpT");

	TH1F *norm[5];
	double sigma[5];
	double weightSum[5];	
	double binWidth = hadronPT[0]->GetBinWidth(1);
	double inelBinWidth;
	double inelSig;
	double inelWeight;


	TCanvas *c2 = new TCanvas("allpTHatBins");

	// setting values and normalizing while ploting each pT hat on same canvas
	for(int i=0; i < 6; i++)
	{
		if (i == 5)
		{
			inelHadron = (TH1F*)files[i]->Get("hpT");
			inelNorm = (TH1F*)files[i]->Get("norm");
			inelBinWidth = inelHadron->GetBinWidth(1);
			inelSig = inelNorm->GetBinContent(1);
			inelWeight = inelNorm->GetBinContent(2);
			inelHadron->Scale( inelSig / (inelWeight * inelBinWidth) );
		}
		else
		{
			hadronPT[i] = (TH1F*)files[i]->Get("hpT");
			norm[i] = (TH1F*)files[i]->Get("norm");
			sigma[i] = norm[i]->GetBinContent(1);
			weightSum[i] = norm[i]->GetBinContent(2);
			hadronPT[i]->Scale( sigma[i] / (weightSum[i] * binWidth) ); // normalization
			hadronPT[i]->SetMarkerStyle(30);
			hadronPT[i]->SetMinimum(1.e-7);
			hadronPT[i]->SetMarkerColor(kViolet+i);
			hadronPT[i]->SetLineColor(kViolet+i);
			hadronPT[i]->SetYTitle("1/N_{evets} d#sigma/dp_{T} [mb c/GeV]");
			hadronPT[i]->SetTitle("Charged Hadron p_{T} #sqrt{s} = 5 TeV");

			if (i == 0) hadronPT[i]->Draw("hist L");
			else hadronPT[i]->Draw("same hist L");
		}
	}
	// making legend to draw on canvas 2
	TLegend *lg2 = new TLegend();
	lg2->AddEntry(hadronPT[0], "#hat{p_{T}} bins 10-20");
	lg2->AddEntry(hadronPT[1], "#hat{p_{T}} bins 20-30");
	lg2->AddEntry(hadronPT[2], "#hat{p_{T}} bins 30-40");
	lg2->AddEntry(hadronPT[3], "#hat{p_{T}} bins 40-50");
	lg2->AddEntry(hadronPT[4], "#hat{p_{T}} bins 50-> ");

	lg2->Draw("same");

	// summing all pT hat bins together
	TString sumName = TString::Format("%s_sum", hadronPT[0]->GetName());
	TH1F *hardSum = (TH1F*)hadronPT[0]->Clone(sumName.Data());
	for (int i=0; i < 5; i++)
	{
		hardSum	-> Add(hadronPT[i]);
	}

	// Drawing sum of pT hat bins with inelastic plot
	TCanvas *c1 = new TCanvas("HadronpT");
	hardSum->SetLineColor(kRed);
	hardSum->SetMarkerColor(kRed);
	hardSum->SetMarkerStyle(24);
	hardSum->SetMinimum(1.e-7);
	hardSum->SetYTitle("1/N_{evets} d#sigma/dp_{T} [mb c GeV^{-1}]");
	hardSum->SetTitle("Charged Hadron p_{T} #sqrt{s} = 5 TeV ");
	hardSum->Draw("pe");

	inelHadron->SetLineColor(kBlue);
	inelHadron->SetMarkerColor(kBlue);
	inelHadron->SetMarkerStyle(26);
	inelHadron->Draw("same pe");

	TLegend *lg = new TLegend();
	lg ->AddEntry(hardSum, "using pT Hat Bins [#hat{p_{T}} starting at 10 GeV] ","lp");
	lg ->AddEntry(inelHadron, "using inelastic");
	lg ->Draw("same");

	// Making plot that is not normalized by cross section
	TH1F *hadronCopy[5];
	TCanvas *c3 = new TCanvas("hadronCounts");
	for(int i =0; i < 5; i++)
	{
		hadronCopy[i] = (TH1F*)hadronPT[i]->Clone();
		hadronCopy[i]->SetLineWidth(2);
		hadronCopy[i]->Scale( 1 / sigma[i] ); //getting rid of the sigma normalization from above

		if (i == 0) 
		{
			hadronCopy[i]->SetYTitle("(1/N_{ev}) dN/dp_{T} [#it{c}/GeV]");
			hadronCopy[i]->SetMarkerStyle(24);
			hadronCopy[i]->SetMarkerColor(kViolet);
			hadronCopy[i]->Draw("hist pe");
		}
		else
		{
			hadronCopy[i]->SetMarkerStyle(24+i);
			hadronCopy[i]->SetMarkerColor(kViolet+i);
			hadronCopy[i]->Draw("same pe");	
		}
	}	

	TLegend *lg3 = new TLegend();
	lg3->AddEntry(hadronCopy[0], "#hat{p_{T}} bins 10-20");
	lg3->AddEntry(hadronCopy[1], "#hat{p_{T}} bins 20-30");
	lg3->AddEntry(hadronCopy[2], "#hat{p_{T}} bins 30-40");
	lg3->AddEntry(hadronCopy[3], "#hat{p_{T}} bins 40-50");
	lg3->AddEntry(hadronCopy[4], "#hat{p_{T}} bins 50-> ");
	

	lg3->Draw("same");


	c1->SetLogy();
	c2->SetLogy();
	c3->SetLogy();

	// write to root file
	TFile *fout = TFile::Open("chargedHadrons.root", "RECREATE");
	fout->cd();

	inelHadron->Write("inelasitc");
	hardSum -> Write("pTHat_Bins");
	hadronCopy[0]->Write("notSigmaNormalized");
	fout->Close();

}