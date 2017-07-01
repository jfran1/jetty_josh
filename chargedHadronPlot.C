
void chargedHadronPlot()
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

	// ############ PULLING HISTOGRAMS FROM FILES AND STORING IN ARAAYS ###############
	TH1F *hadronPT[5];
	TH1F *jetNoFilter[5];
	TH1F *jetFilter[5];
	TH1F *jetCuts[5];
	TH1F *inelHadron;
	TH1F *inelNorm;
	TH1F *norm[5];
	hadronPT[0] = (TH1F*)files[0]->Get("hpT");
	
	double sigma[5];
	double weightSum[5];	
	double inelBinWidth;
	double inelSig;
	double inelWeight;

	// looping through each file
	for (int i=0; i < 6 ; i++)
	{
		if (i == 5)
		{
			inelHadron = (TH1F*)files[i]->Get("hpT");
			inelNorm = (TH1F*)files[i]->Get("norm");
			inelBinWidth = inelHadron->GetBinWidth(1);
			inelSig = inelNorm->GetBinContent(1);
			inelWeight = inelNorm->GetBinContent(2);
		}
		else
		{	
			jetNoFilter[i] = (TH1F*)files[i]->Get("jetNoFilter");
			jetFilter[i]  = (TH1F*)files[i]->Get("jetFilter");
			jetCuts[i]     = (TH1F*)files[i]->Get("jetCuts");
			hadronPT[i] = (TH1F*)files[i]->Get("hpT");
			norm[i] = (TH1F*)files[i]->Get("norm");
			sigma[i] = norm[i]->GetBinContent(1);
			weightSum[i] = norm[i]->GetBinContent(2);
		}
	}


	// ############## SCALE HISTOGRAMS ###############
	for(int i=0; i < 6; i++)
	{
		if (i == 5)
		{	
			inelHadron->Scale( inelSig / (inelWeight * inelBinWidth) );
		}
		else
		{
			
			hadronPT[i]->Scale( sigma[i] / (weightSum[i] * hadronPT[i]->GetBinWidth(1) ) ); // normalization
			jetNoFilter[i]->Scale( 1 / (weightSum[i] * jetNoFilter[i]->GetBinWidth(1) ) );
			jetFilter[i]->Scale( 1 / (weightSum[i] * jetFilter[i]->GetBinWidth(1) ) );
			jetCuts[i]->Scale( 1 / (weightSum[i] * jetCuts[i]->GetBinWidth(1) ) );
		  		
		}
	}

	// copy histogram after scaling 
	TString sumName = TString::Format("%s_sum", hadronPT[0]->GetName());
	TH1F *hardSum = (TH1F*)hadronPT[0]->Clone(sumName.Data());
	for (int i=0; i < 5; i++) hardSum-> Add(hadronPT[i]); // summing all pT hat bins together	


    // ########### DRAWING HISTOGRAMS ############
	TCanvas *c1 = new TCanvas("HadronpT");
	TCanvas *c2 = new TCanvas("allpTHatBins");
	TCanvas *c3 = new TCanvas("hadronCounts");
	TCanvas *c4 = new TCanvas("jetHadronpT");
	TCanvas *c5 = new TCanvas("sumJet");


	for (int i =0; i < 6; i++)
	{
		if(i==5) // do inelastic stuff
		{
			inelHadron->SetLineColor(kBlue);
			inelHadron->SetMarkerColor(kBlue);
			inelHadron->SetMarkerStyle(26);
			inelHadron->SetMinimum(1.e-5);
			c1->cd();
			inelHadron->Draw("pe");
		}
		else
		{
			hadronPT[i]->SetMarkerStyle(24);
			hadronPT[i]->SetMinimum(1.e-7);
			hadronPT[i]->SetMarkerColor(kViolet+i);
			hadronPT[i]->SetLineColor(kViolet+i);
			hadronPT[i]->SetYTitle("1/N_{evets} d#sigma/dp_{T} [mb c/GeV]");
			hadronPT[i]->SetTitle("Charged Hadron p_{T} #sqrt{s} = 5 TeV");

			jetFilter[i]->SetLineColor(kViolet+i);
			jetFilter[i]->SetMarkerColor(kViolet+i);
			jetFilter[i]->SetMarkerStyle(24);
 			jetFilter[i]->SetLineWidth(2);
			jetFilter[i]->SetYTitle("1/N_{evets} dN/dp_{T} [c/GeV]");
			jetFilter[i]->SetTitle("Charged Hadron Jet");
			jetFilter[i]->SetXTitle("Jet p_{T} [Gev/c]");

			jetCuts[i]->SetLineColor(kViolet+i);
			jetCuts[i]->SetMarkerColor(kViolet+i);
			jetCuts[i]->SetLineWidth(2);
			jetCuts[i]->SetMarkerStyle(24);
			jetCuts[i]->SetYTitle("1/N_{evets} dN/dp_{T} [c/GeV]");
			jetCuts[i]->SetTitle("Charged Hadron Jet |#eta_{jet}| < 1.5");
			jetCuts[i]->SetXTitle("Jet p_{T} [Gev/c]");

			jetNoFilter[i]->SetLineColor(kViolet+i);
			jetNoFilter[i]->SetMarkerColor(kViolet+i);
			jetNoFilter[i]->SetLineWidth(2);
			jetNoFilter[i]->SetMarkerStyle(24);
			jetNoFilter[i]->SetYTitle("1/N_{evets} dN/dp_{T} [c/GeV]");
			jetNoFilter[i]->SetTitle("All Particle Jet");
			jetNoFilter[i]->SetXTitle("Jet p_{T} [Gev/c]");

			c2->cd();
			if (i==0) hadronPT[i]->Draw("pe");
			else hadronPT[i]->Draw("same pe");

			c3->cd();
			if (i==0) jetFilter[i]->Draw("pe");
			else jetFilter[i]->Draw("same pe");

			c4->cd();
			if (i==0) jetCuts[i]->Draw("pe");
			else jetCuts[i]->Draw("same pe");

			c5->cd();
			if (i==0) jetNoFilter[i]->Draw("pe");
			else jetNoFilter[i]->Draw("same pe");
		}
	}

	// Don't need a loop for this one 
	hardSum->SetLineColor(kRed);
	hardSum->SetMarkerColor(kRed);
	hardSum->SetMarkerStyle(24);
	hardSum->SetMinimum(1.e-7);
	hardSum->SetYTitle("1/N_{evets} d#sigma/dp_{T} [mb c GeV^{-1}]");
	hardSum->SetTitle("Charged Hadron p_{T} #sqrt{s} = 5 TeV ");
	c1->cd();
	hardSum->Draw("same pe");

	TLegend *lg = new TLegend();
	lg ->AddEntry(hardSum, "#hat{p_{T}} > 10 GeV","lp");
	lg ->AddEntry(inelHadron, "Inelastic");
	c1->cd();
	lg ->Draw("same");
	
	// SETTING UP LEGENDS
	TLegend *lg2 = new TLegend();
	lg2->AddEntry(hadronPT[0], "#hat{p_{T}} bins 10-20");
	lg2->AddEntry(hadronPT[1], "#hat{p_{T}} bins 20-30");
	lg2->AddEntry(hadronPT[2], "#hat{p_{T}} bins 30-40");
	lg2->AddEntry(hadronPT[3], "#hat{p_{T}} bins 40-50");
	lg2->AddEntry(hadronPT[4], "#hat{p_{T}} bins 50-> ");
	c2->cd();
	lg2->Draw("same");

	TLegend *lg3 = new TLegend();
	lg3->AddEntry(jetFilter[0], "#hat{p_{T}} bins 10-20");
	lg3->AddEntry(jetFilter[1], "#hat{p_{T}} bins 20-30");
	lg3->AddEntry(jetFilter[2], "#hat{p_{T}} bins 30-40");
	lg3->AddEntry(jetFilter[3], "#hat{p_{T}} bins 40-50");
	lg3->AddEntry(jetFilter[4], "#hat{p_{T}} bins 50-> ");
	c3->cd();
	lg3->Draw("same");

	TLegend *lg4 = new TLegend();
	lg4->AddEntry(jetCuts[0], "#hat{p_{T}} bins 10-20");
	lg4->AddEntry(jetCuts[1], "#hat{p_{T}} bins 20-30");
	lg4->AddEntry(jetCuts[2], "#hat{p_{T}} bins 30-40");
	lg4->AddEntry(jetCuts[3], "#hat{p_{T}} bins 40-50");
	lg4->AddEntry(jetCuts[4], "#hat{p_{T}} bins 50-> ");
	c4->cd();

	TLegend *lg6 = new TLegend();
	lg6->SetTextSize(0.05);
	lg6->AddEntry((TObject*)0, "|#eta_{Particle}| < 2", "");
	lg6->AddEntry((TObject*)0, "|#eta_{Jet}| < 1.5", "");
	lg4->Draw("same");
	lg6->Draw("same");

	TLegend *lg5 = new TLegend();
	lg5->AddEntry(jetNoFilter[0], "#hat{p_{T}} bins 10-20");
	lg5->AddEntry(jetNoFilter[1], "#hat{p_{T}} bins 20-30");
	lg5->AddEntry(jetNoFilter[2], "#hat{p_{T}} bins 30-40");
	lg5->AddEntry(jetNoFilter[3], "#hat{p_{T}} bins 40-50");
	lg5->AddEntry(jetNoFilter[4], "#hat{p_{T}} bins 50-> ");
	c5->cd();
	lg5->Draw("same");



	// SETTING AXIS VIEW
	c1->SetLogy();
	c2->SetLogy();
	c3->SetLogy();
	c4->SetLogy();
	c5->SetLogy();

	// ############# write to file ##############
/*
	TFile *fout = TFile::Open("chargedHadrons.root", "RECREATE");
	fout->cd();

	inelHadron->Write("inelasitc");
	hardSum -> Write("pTHat_Bins");
	fout->Close();
*/

}