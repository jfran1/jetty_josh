
void gammaPlot()
{
	//################ OPENING FILES #################
	const char *fnames[] = {
		"/Users/joshfrandsen/test/jetty/output/gamma_10_20.root",   // 0
		"/Users/joshfrandsen/test/jetty/output/gamma_20_30.root",   // 1
		"/Users/joshfrandsen/test/jetty/output/gamma_30_40.root",   // 2
		"/Users/joshfrandsen/test/jetty/output/gamma_40_50.root",   // 3
		"/Users/joshfrandsen/test/jetty/output/gamma_50_-1.root",   // 4
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

	
	TH1F *gamma_prompt[5];
	TH1F *gamma_soft[5];
	TH1F *gammaJet_prompt[5];
	TH1F *gammaJet_soft[5];
	TH1F *norm[5];

	double sigma[5];
	double weightSum[5];

	// FILLING ARRAYS WITH HISTOGRAMS FROM FILES AND GETTING NORMALIZATION VALUES
	// ###########################################################################
	for (int i =0; i < 5; i++)
	{
		gamma_prompt[i] = (TH1F*)files[i]->Get("gammaPrompt"); // filter for hard gamma
		gamma_soft[i] = (TH1F*)files[i]->Get("gammaSoft"); // no filter
		gammaJet_prompt[i] = (TH1F*)files[i]->Get("gammaJet_Prompt");
		gammaJet_soft[i] = (TH1F*)files[i]->Get("gammaJet_Soft");		
		norm[i] = (TH1F*)files[i]->Get("norm");

		sigma[i] = norm[i]->GetBinContent(1);
		weightSum[i] = norm[i]->GetBinContent(2);
	}

	// NORMALIZTION
	// ############################################################################
	for (int i = 0; i < 5; i++)
	{
		gamma_prompt[i]->Scale( 1 / (weightSum[i] * gamma_prompt[i]->GetBinWidth(1)) );
		gamma_soft[i]->Scale( 1 / (weightSum[i] * gamma_soft[i]->GetBinWidth(1)) );
		gammaJet_prompt[i]->Scale( 1 / (weightSum[i] * gammaJet_prompt[i]->GetBinWidth(1)) );
		gammaJet_soft[i]->Scale( 1 / (weightSum[i] * gammaJet_soft[i]->GetBinWidth(1)) );
		
	}

	// GETT SUM OF EACH PT HAT BIN
	// #############################################################################
	TH1F *gamma_prompt_sum = (TH1F*)gamma_prompt[0]->Clone("gamma_prompt_sum");
	TH1F *gammaJet_prompt_sum = (TH1F*)gammaJet_prompt[0]->Clone("gammaJet_prompt_sum");
	TH1F *gamma_soft_sum =(TH1F*)gamma_soft[0]->Clone("gamma_soft_sum");
	TH1F *gammaJet_soft_sum = (TH1F*)gammaJet_soft[0]->Clone("gammaJet_soft_sum");

	for(int i=1; i < 5; i++)
	{
		gamma_prompt_sum->Add(gamma_prompt[i]);
		gamma_soft_sum->Add(gamma_soft[i]);
		gammaJet_prompt_sum->Add(gammaJet_prompt[i]);
		gammaJet_soft_sum->Add(gammaJet_soft[i]);				
	}

	TH1F *gammaRatio = (TH1F*)gamma_soft_sum->Clone("gammaRatio"); // need plot for gamma ratio

	// DRAWING HISTOGRAMS
	// #############################################################################
	TCanvas *c1 = new TCanvas("gamma_prompt_pTHat");
	TCanvas *c2 = new TCanvas("gamma_soft_pTHat");
	TCanvas *c3 = new TCanvas("gammaJet_prompt_pTHat");
	TCanvas *c4 = new TCanvas("gammaJet_soft_pTHat");
	TCanvas *c5 = new TCanvas("prompt_soft_ratio");
	TCanvas *c6 = new TCanvas("gamma_soft_prompt");

	for(int i=0;i<5; i++)
	{
		gamma_prompt[i]->SetLineColor(kAzure+i);
		gamma_prompt[i]->SetMarkerColor(kAzure+i);
		gamma_prompt[i]->SetMarkerStyle(24+i);
		gamma_prompt[i]->SetLineWidth(2);
		gamma_prompt[i]->SetYTitle("#frac{1}{N_{events}} #frac{dN}{dp_{T}} [mb c/GeV]");
		gamma_prompt[i]->SetTitle("#gamma prompt p_{T}  #sqrt{s} = 5 TeV #hat{p_{T}} > 10 GeV");

		gamma_soft[i]->SetLineColor(kAzure+i);
		gamma_soft[i]->SetMarkerColor(kAzure+i);
		gamma_soft[i]->SetMarkerStyle(24+i);
		gamma_soft[i]->SetLineWidth(2);
		gamma_soft[i]->SetYTitle("#frac{1}{N_{events}} #frac{dN}{dp_{T}} [mb c/GeV]");
		gamma_soft[i]->SetTitle("#gamma soft  p_{T}  #sqrt{s} = 5 TeV #hat{p_{T}} > 10 GeV");

		gammaJet_prompt[i]->SetLineColor(kAzure+i);
		gammaJet_prompt[i]->SetMarkerColor(kAzure+i);
		gammaJet_prompt[i]->SetMarkerStyle(24+i);
		gammaJet_prompt[i]->SetLineWidth(2);
		gammaJet_prompt[i]->SetYTitle("#frac{1}{N_{events}} #frac{dN}{dp_{T}} [mb c/GeV]");
		gammaJet_prompt[i]->SetTitle("#gamma prompt_jet p_{T}  #sqrt{s} = 5 TeV #hat{p_{T}} > 10 GeV");

		gammaJet_soft[i]->SetLineColor(kAzure+i);
		gammaJet_soft[i]->SetMarkerColor(kAzure+i);
		gammaJet_soft[i]->SetMarkerStyle(24+i);
		gammaJet_soft[i]->SetLineWidth(2);
		gammaJet_soft[i]->SetYTitle("#frac{1}{N_{events}} #frac{dN}{dp_{T}} [mb c/GeV]");
		gammaJet_soft[i]->SetTitle("#gamma soft_jet p_{T}  #sqrt{s} = 5 TeV #hat{p_{T}} > 10 GeV");

		if(i==0)
		{
			c1->cd();
			// gamma_prompt[i]->SetMinimum(1.e-6);
			gamma_prompt[i]->Draw("pe");
			c2->cd();
			// gamma_soft[i]->SetMinimum(1.e-5);
			gamma_soft[i]->Draw("pe");
			
			c3->cd();
			// gammaJet_prompt[i]->SetMinimum(1.e-5);
			gammaJet_prompt[i]->Draw("pe");
			
			c4->cd();
			// gammaJet_prompt[i]->SetMinimum(1.e-5);
			gammaJet_soft[i]->Draw("pe");
		}
		else
		{
			c1->cd();
			gamma_prompt[i]->Draw("same pe");
			c2->cd();
			gamma_soft[i]->Draw("same pe");
			c3->cd();
			gammaJet_prompt[i]->Draw("same pe");
			c4->cd();
			gammaJet_soft[i]->Draw("same pe");
		}
	}
	
	gamma_prompt_sum->SetLineColor(kBlack);
	gamma_prompt_sum->SetMarkerColor(kBlack);
	gamma_prompt_sum->SetLineWidth(2);
	gamma_prompt_sum->SetMarkerStyle(24);
	gamma_prompt_sum->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb c/GeV]");

	gamma_soft_sum->SetLineColor(kBlue);
	gamma_soft_sum->SetMarkerColor(kBlue);
	gamma_soft_sum->SetLineWidth(2);
	gamma_soft_sum->SetMarkerStyle(24);
	gamma_soft_sum->SetMinimum(1.e-5);
	gamma_soft_sum->SetYTitle("#frac{1}{N_{events}} #frac{N}{dp_{T}} [mb c/GeV]");

	gammaJet_prompt_sum->SetLineColor(kRed);
	gammaJet_prompt_sum->SetMarkerColor(kRed);
	gammaJet_prompt_sum->SetLineWidth(2);	
	gammaJet_prompt_sum->SetMarkerStyle(26);
	gammaJet_prompt_sum->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb c/GeV]");

	gammaJet_soft_sum->SetLineColor(kGreen);
	gammaJet_soft_sum->SetMarkerColor(kGreen);
	gammaJet_soft_sum->SetLineWidth(2);	
	gammaJet_soft_sum->SetMarkerStyle(26);
	gammaJet_soft_sum->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb c/GeV]");

	gammaRatio->Divide(gamma_prompt_sum); // get ratio
	gammaRatio->SetLineColor(kRed);
	gammaRatio->SetMarkerColor(kRed);
	gammaRatio->SetMarkerStyle(24);
	gammaRatio->SetLineWidth(2);
	gammaRatio->SetYTitle("Ratio");
	gammaRatio->SetXTitle("p_{T} [GeV/c]");
	gammaRatio->SetTitle("#gamma_{soft}/#gamma_{prompt}");

	c5->cd();
	gammaRatio->Draw("pe");

	c6->cd();
	gamma_soft_sum->Draw(" pe");
	gamma_prompt_sum->Draw("same pe");

	c1->SetLogy();
	c2->SetLogy();
	c3->SetLogy();
	c4->SetLogy();
	c6->SetLogy();

	// MAKE/DRAW LEGEND
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

	leg2->AddEntry(gamma_soft[0], "10 < #hat{p_{T}} < 20 GeV");
	leg2->AddEntry(gamma_soft[1], "20 < #hat{p_{T}} < 30 GeV");
	leg2->AddEntry(gamma_soft[2], "30 < #hat{p_{T}} < 40 GeV");
	leg2->AddEntry(gamma_soft[3], "40 < #hat{p_{T}} < 50 GeV");	
	leg2->AddEntry(gamma_soft[4], "50 < #hat{p_{T}} < inf GeV");

	leg3->AddEntry(gammaJet_prompt[0], "10 < #hat{p_{T}} < 20 GeV");
	leg3->AddEntry(gammaJet_prompt[1], "20 < #hat{p_{T}} < 30 GeV");
	leg3->AddEntry(gammaJet_prompt[2], "30 < #hat{p_{T}} < 40 GeV");
	leg3->AddEntry(gammaJet_prompt[3], "40 < #hat{p_{T}} < 50 GeV");	
	leg3->AddEntry(gammaJet_prompt[4], "50 < #hat{p_{T}} < inf GeV");

	leg4->AddEntry(gammaJet_soft[0], "10 < #hat{p_{T}} < 20 GeV");
	leg4->AddEntry(gammaJet_soft[1], "20 < #hat{p_{T}} < 30 GeV");
	leg4->AddEntry(gammaJet_soft[2], "30 < #hat{p_{T}} < 40 GeV");
	leg4->AddEntry(gammaJet_soft[3], "40 < #hat{p_{T}} < 50 GeV");	
	leg4->AddEntry(gammaJet_soft[4], "50 < #hat{p_{T}} < inf GeV");
	
	leg6->AddEntry((TObject*)0, " Anti-kt R = 0.4" , " ");

	leg7->AddEntry(gamma_prompt_sum, "prompt #gamma" );
	leg7->AddEntry(gamma_soft_sum, "soft #gamma" );

	c1->cd();
	leg1->Draw("same");

	c2->cd();
	leg2->Draw("same");

	c3->cd();
	leg3->Draw("same");
	leg6->Draw("same");

	c4->cd();
	leg4->Draw("same");
	leg6->Draw("same");

	c6->cd();
	leg7->Draw("same");
}




