
void gammaPlot()
{
	//################ OPENING FILES #################
	const char *fnames[] = {
		"/Users/joshfrandsen/test/jetty/output/gamma_10_20.root",   	// 0
		"/Users/joshfrandsen/test/jetty/output/gamma_20_30.root",   	// 1
		"/Users/joshfrandsen/test/jetty/output/gamma_30_40.root",   	// 2
		"/Users/joshfrandsen/test/jetty/output/gamma_40_50.root",   	// 3
		"/Users/joshfrandsen/test/jetty/output/gamma_50_-1.root",   	// 4
		"/Users/joshfrandsen/test/jetty/output/soft_gamma_10_20.root",  // 5
		"/Users/joshfrandsen/test/jetty/output/soft_gamma_20_30.root",  // 6
		"/Users/joshfrandsen/test/jetty/output/soft_gamma_30_40.root",  // 7
		"/Users/joshfrandsen/test/jetty/output/soft_gamma_40_50.root",  // 8
		"/Users/joshfrandsen/test/jetty/output/soft_gamma_50_-1.root",	// 9
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
	TH1F *gamma_prompt_test[5];
	TH1F *gamma_prompt_sigma[5];
	TH1F *gamma_soft[5];
	TH1F *norm[5];
	TH1F *norm_soft[5];

	double sigma[5];
	double weightSum[5];
	double sigma_soft[5];
	double weightSum_soft[5];

	// FILLING ARRAYS WITH HISTOGRAMS FROM FILES AND GETTING NORMALIZATION VALUES
	// ###########################################################################
	for (int i =0; i < 5; i++)
	{
		gamma_prompt[i] = (TH1F*)files[i]->Get("gammaPrompt"); // filter for hard gamma
		gamma_prompt_sigma[i] = (TH1F*)gamma_prompt[i]->Clone("gamma_prompt_sigma");
		gamma_prompt_test[i] = (TH1F*)files[i]->Get("gammaPrompt_test");
		norm[i] = (TH1F*)files[i]->Get("norm");

		gamma_soft[i] = (TH1F*)files[i+5]->Get("gammaSoft"); 
		norm_soft[i] = (TH1F*)files[i+5]->Get("norm");

		sigma_soft[i] = norm_soft[i]->GetBinContent(1);
		weightSum_soft[i] = norm_soft[i]->GetBinContent(2);

		sigma[i] = norm[i]->GetBinContent(1);
		weightSum[i] = norm[i]->GetBinContent(2);
	}

	// NORMALIZTION
	// ############################################################################
	for (int i = 0; i < 5; i++)
	{
		gamma_prompt[i]->Scale( sigma[i] / (weightSum[i] * gamma_prompt[i]->GetBinWidth(1)) );
		gamma_prompt_test[i]->Scale( sigma[i] / (weightSum[i] * gamma_prompt[i]->GetBinWidth(1)) );
		gamma_prompt_sigma[i]->Scale( sigma[i] / (weightSum[i] * gamma_prompt_sigma[i]->GetBinWidth(1)) ); // scaled by sigma		
		gamma_soft[i]->Scale( sigma_soft[i] / (weightSum_soft[i] * gamma_soft[i]->GetBinWidth(1)) );	
	}

	// GETT SUM OF EACH PT HAT BIN
	// #############################################################################
	TH1F *gamma_prompt_sum = (TH1F*)gamma_prompt[0]->Clone("gamma_prompt_sum");
	TH1F *gamma_prompt_test_sum = (TH1F*)gamma_prompt_test[0]->Clone("gamma_prompt_test_sum");
	TH1F *gamma_soft_sum =(TH1F*)gamma_soft[0]->Clone("gamma_soft_sum");

	for(int i=1; i < 5; i++)
	{
		gamma_prompt_sum->Add(gamma_prompt[i]);
		gamma_prompt_test_sum->Add(gamma_prompt_sigma[i]);
		gamma_soft_sum->Add(gamma_soft[i]);
	}

	TH1F *gammaRatio = (TH1F*)gamma_prompt_sum->Clone("gammaRatio"); // need plot for gamma ratio


	// DRAWING HISTOGRAMS
	// #############################################################################
	TCanvas *c1 = new TCanvas("gamma_prompt_pTHat");
	TCanvas *c2 = new TCanvas("gamma_soft_pTHat");
	TCanvas *c3 = new TCanvas("gamma_soft_prompt");
	TCanvas *c4 = new TCanvas("prompt_soft_ratio");
	TCanvas *c5 = new TCanvas("gamma_prompt_test");

	for(int i=0;i<5; i++)
	{
		gamma_prompt[i]->SetLineColor(kAzure+i);
		gamma_prompt[i]->SetMarkerColor(kAzure+i);
		gamma_prompt[i]->SetMarkerStyle(24+i);
		gamma_prompt[i]->SetLineWidth(2);
		gamma_prompt[i]->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb c/GeV]");
		gamma_prompt[i]->SetTitle("#gamma prompt p_{T}  #sqrt{s} = 5 TeV #hat{p_{T}} > 10 GeV");

		gamma_soft[i]->SetLineColor(kAzure+i);
		gamma_soft[i]->SetMarkerColor(kAzure+i);
		gamma_soft[i]->SetMarkerStyle(24+i);
		gamma_soft[i]->SetLineWidth(2);
		gamma_soft[i]->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb c/GeV]");
		gamma_soft[i]->SetTitle("#gamma non-prompt  p_{T}  #sqrt{s} = 5 TeV #hat{p_{T}} > 10 GeV");

		if(i==0)
		{
			c1->cd();
			// gamma_prompt[i]->SetMinimum(1.e-6);
			gamma_prompt[i]->Draw("pe");
			c2->cd();
			// gamma_soft[i]->SetMinimum(1.e-5);
			gamma_soft[i]->Draw("pe");
			
			
		}
		else
		{
			c1->cd();
			gamma_prompt[i]->Draw("same pe");
			c2->cd();
			gamma_soft[i]->Draw("same pe");
			
		}
	}
	
	gamma_prompt_sum->SetLineColor(kRed);
	gamma_prompt_sum->SetMarkerColor(kRed);
	gamma_prompt_sum->SetLineWidth(2);
	gamma_prompt_sum->SetMarkerStyle(24);
	gamma_prompt_sum->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb c/GeV]");

	gamma_soft_sum->SetLineColor(kBlue);
	gamma_soft_sum->SetMarkerColor(kBlue);
	gamma_soft_sum->SetLineWidth(2);
	gamma_soft_sum->SetMarkerStyle(24);
	gamma_soft_sum->SetMinimum(1.e-5);
	gamma_soft_sum->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb c/GeV]");

	gammaRatio->Divide(gamma_soft_sum); // get ratio
	gammaRatio->SetLineColor(kRed);
	gammaRatio->SetMarkerColor(kRed);
	gammaRatio->SetMarkerStyle(24);
	gammaRatio->SetLineWidth(2);
	gammaRatio->SetYTitle("Ratio");
	gammaRatio->SetXTitle("p_{T} [GeV/c]");
	gammaRatio->SetTitle("#gamma_{prompt}/#gamma_{non-prompt}");

	gamma_prompt_test_sum->SetLineColor(kBlack);
	gamma_prompt_test_sum->SetLineWidth(2);
	gamma_prompt_test_sum->SetYTitle("#frac{1}{N_{events}}#frac{d#sigma}{dp_{T}} [mb c/GeV]");		
	gamma_prompt_test_sum->SetTitle("#sqrt{s} = 5 TeV  |#eta| < 2");
	gamma_prompt_test_sum->SetMarkerStyle(24);
	gamma_prompt_test_sum->SetMarkerColor(kBlack);	

	c3->cd();
	gamma_soft_sum->SetMinimum(1.e-10);
	gamma_soft_sum->Draw(" pe");
	gamma_prompt_sum->Draw("same pe");

	c4->cd();
	gammaRatio->Draw("pe");

	c5->cd();
	gamma_prompt_test_sum->Draw("pe");
	gamma_prompt_sum ->Draw("same pe");

	c1->SetLogy();
	c2->SetLogy();
	c3->SetLogy();
	c4->SetLogy();
	c5->SetLogy();
	
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
	
	leg3->AddEntry(gamma_prompt_sum, "prompt #gamma" );
	leg3->AddEntry(gamma_soft_sum, "Decay #gamma" );

	leg4->AddEntry(gamma_prompt_sum, "gamma prompt 'daughters'");
	leg4->AddEntry(gamma_prompt_test_sum, "gamma intial Mother prompt");

	c1->cd();
	leg1->Draw("same");

	c2->cd();
	leg2->Draw("same");

	c3->cd();
	leg3->Draw("same");

	c5->cd();
	leg4->Draw("same");
}




