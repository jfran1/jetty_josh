
void gammaPlot()
{
	//################ OPENING FILES #################
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

	
	TH1F *gammapT[5];
	TH1F *gammaJet[5];
	TH1F *gammaJetEta[5];
	TH1F *norm[5];

	double sigma[5];
	double weightSum[5];

	// FILLING ARRAYS WITH HISTOGRAMS FROM FILES AND GETTING NORMALIZATION VALUES
	for (int i =0; i < 5; i++)
	{
		gammapT[i] = (TH1F*)files[i]->Get("gammapT");
		gammaJet[i] = (TH1F*)files[i]->Get("gammaJet");
		gammaJetEta[i] = (TH1F*)files[i]->Get("gammaEtaJet");		
		norm[i] = (TH1F*)files[i]->Get("norm");

		sigma[i] = norm[i]->GetBinContent(1);
		weightSum[i] = norm[i]->GetBinContent(2);
	}

	// NORMALIZTION
	for (int i = 0; i < 5; i++)
	{
		gammapT[i]->Scale( sigma[i] / (weightSum[i] * gammapT[i]->GetBinWidth(i)) );
		gammaJet[i]->Scale( sigma[i] / (weightSum[i] * gammaJet[i]->GetBinWidth(i)) );
		gammaJetEta[i]->Scale( sigma[i] / (weightSum[i] * gammaJetEta[i]->GetBinWidth(i)) );
	}

	// GETT SUM OF EACH PT HAT BIN
	TH1F *gammapTSum = (TH1F*)gammapT[0]->Clone("gammapT_sum");
	TH1F *gammaJetSum = (TH1F*)gammaJet[0]->Clone("gammaJet_sum");
	TH1F *gammaJetEtaSum = (TH1F*)gammaJetEta[0]->Clone("gammaJetEta_sum");

	for(int i=1; i < 5; i++)
	{
		gammapTSum->Add(gammapT[i]);
		gammaJetSum->Add(gammaJet[i]);
		gammaJetEtaSum->Add(gammaJetEta[i]);				
	}


	// DRAWING HISTOGRAMS
	TCanvas *c1 = new TCanvas("gammpT");
	TCanvas *c2 = new TCanvas("allpTHat_gammpT");
	TCanvas *c3 = new TCanvas("allpTHat_gammJet");
	TCanvas *c4 = new TCanvas("allpTHat_gammJetEta");

	for(int i=0;i<5; i++)
	{
		gammapT[i]->SetLineColor(kAzure+i);
		gammapT[i]->SetMarkerColor(kAzure+i);
		gammapT[i]->SetMarkerStyle(24+i);
		gammapT[i]->SetLineWidth(2);
		gammapT[i]->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb c/GeV]");
		gammapT[i]->SetTitle("Final State #gamma p_{T}  #sqrt{s} = 5 TeV #hat{p_{T}} > 10 GeV");

		gammaJet[i]->SetLineColor(kAzure+i);
		gammaJet[i]->SetMarkerColor(kAzure+i);
		gammaJet[i]->SetMarkerStyle(24+i);
		gammaJet[i]->SetLineWidth(2);
		gammaJet[i]->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb c/GeV]");
		gammaJet[i]->SetTitle("#gamma Jets p_{T}  #sqrt{s} = 5 TeV #hat{p_{T}} > 10 GeV");

		gammaJetEta[i]->SetLineColor(kAzure+i);
		gammaJetEta[i]->SetMarkerColor(kAzure+i);
		gammaJetEta[i]->SetMarkerStyle(24+i);
		gammaJetEta[i]->SetLineWidth(2);
		gammaJetEta[i]->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb c/GeV]");
		gammaJetEta[i]->SetTitle("#gamma Jets  #sqrt{s} = 5 TeV #hat{p_{T}} > 10 GeV");

		if(i==0)
		{
			c2->cd();
			gammapT[i]->SetMinimum(1.e-6);
			gammapT[i]->Draw("pe");
			c3->cd();
			gammaJet[i]->SetMinimum(1.e-5);
			gammaJet[i]->Draw("pe");
			c4->cd();
			gammaJetEta[i]->SetMinimum(1.e-5);
			gammaJetEta[i]->Draw("pe");
		}
		else
		{
			c2->cd();
			gammapT[i]->Draw("same pe");
			c3->cd();
			gammaJet[i]->Draw("same pe");
			c4->cd();
			gammaJetEta[i]->Draw("same pe");

		}
	}




	
	gammapTSum->SetLineColor(kBlack);
	gammapTSum->SetMarkerColor(kBlack);
	gammapTSum->SetLineWidth(2);
	gammapTSum->SetMarkerStyle(24);
	gammapTSum->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb c/GeV]");

	gammaJetSum->SetLineColor(kBlue);
	gammaJetSum->SetMarkerColor(kBlue);
	gammaJetSum->SetLineWidth(2);
	gammaJetSum->SetMarkerStyle(24);
	gammaJetSum->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb c/GeV]");
	gammaJetSum->SetTitle("Gamma & Gamma Jet p_{T}   #hat{p_{T}} > 10 GeV  #sqrt{s} = 5 TeV ");

	gammaJetEtaSum->SetLineColor(kRed);
	gammaJetEtaSum->SetMarkerColor(kRed);
	gammaJetEtaSum->SetLineWidth(2);	
	gammaJetEtaSum->SetMarkerStyle(26);
	gammaJetEtaSum->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb c/GeV]");

	// gammapTSum->Draw("pe");
	c1->cd();
	gammaJetSum->Draw(" pe");
	gammaJetEtaSum->Draw("same pe");

	c1->SetLogy();
	c2->SetLogy();
	c3->SetLogy();
	c4->SetLogy();

	// MAKE/DRAW LEGEND
	TLegend *leg1 = new TLegend();
	TLegend *leg2 = new TLegend();
	TLegend *leg3 = new TLegend();
	TLegend *leg4 = new TLegend();

	leg1->AddEntry(gammaJetSum, "Gamma Jet p_{T} ");
	leg1->AddEntry(gammaJetEtaSum, "Gamma Jet p_{T} |#eta_{jet}| < 1.5 ");		
	
	leg2->AddEntry(gammapT[0], "10 < #hat{p_{T}} < 20 GeV");
	leg2->AddEntry(gammapT[1], "20 < #hat{p_{T}} < 30 GeV");
	leg2->AddEntry(gammapT[2], "30 < #hat{p_{T}} < 40 GeV");
	leg2->AddEntry(gammapT[3], "40 < #hat{p_{T}} < 50 GeV");	
	leg2->AddEntry(gammapT[4], "50 < #hat{p_{T}} < inf GeV");

	leg3->AddEntry(gammaJet[0], "10 < #hat{p_{T}} < 20 GeV");
	leg3->AddEntry(gammaJet[1], "20 < #hat{p_{T}} < 30 GeV");
	leg3->AddEntry(gammaJet[2], "30 < #hat{p_{T}} < 40 GeV");
	leg3->AddEntry(gammaJet[3], "40 < #hat{p_{T}} < 50 GeV");	
	leg3->AddEntry(gammaJet[4], "50 < #hat{p_{T}} < inf GeV");

	leg4->AddEntry(gammaJetEta[0], "10 < #hat{p_{T}} < 20 GeV");
	leg4->AddEntry(gammaJetEta[1], "20 < #hat{p_{T}} < 30 GeV");
	leg4->AddEntry(gammaJetEta[2], "30 < #hat{p_{T}} < 40 GeV");
	leg4->AddEntry(gammaJetEta[3], "40 < #hat{p_{T}} < 50 GeV");	
	leg4->AddEntry(gammaJetEta[4], "50 < #hat{p_{T}} < inf GeV");

	c1->cd();
	leg1->Draw("same");
	c2->cd();
	leg2->Draw("same");
	c3->cd();
	leg3->Draw("same");
	c4->cd();
	leg4->Draw("same");
}




