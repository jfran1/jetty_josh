
void plot13TeV()
{

	TFile *f1= TFile::Open("/Users/joshfrandsen/test/jetty/output/minBias13TeV.root");
	if (f1 ==0) cout<< "ERROR! File 1 didn't open" << endl;
	else cout << "File 1 Opened!" << endl;

	TFile *f2 = TFile::Open("/Users/joshfrandsen/test/jetty/output/inel13TeV.root"); 
	if (f2==0) cout<< "ERROR! File 2 didn't open" << endl;
	else cout << "File2 Opened!" << endl;
	

	TFile *f3 = TFile::Open("/Users/joshfrandsen/test/jetty/output/13TeVSum.root");
	if (f3==0) cout<< "ERROR! File 3 didn't open" << endl;
	else cout << "File 3 Opened!" << endl;
	

	TFile *f4 = TFile::Open("/Users/joshfrandsen/test/jetty/output/13TeVAliceResults.root");
	if (f4==0) cout<< "ERROR! File 4 didn't open" << endl;
	else cout << "File 4 Opened!" << endl;
	f4->cd("Table 2");
	

	TH1F *aliceResults = (TH1F*)gDirectory->Get("Hist1D_y1");
	TFile *files[4] = {f1,f2,f3,f4}; 
	TH1F *data[2];
	TH1F *minBias = (TH1F*)f1->Get("hpTRaw"); // not scaled 
	TH1F *inel = (TH1F*)f2->Get("hpTRaw"); // not scaled
	TH1F *hard = (TH1F*)f3->Get("hpTRaw"); // slcaled but not by 1/pT
	TF1 *line = new TF1("line", "pol1", 0, 100); // line for 1/pT scaling
	line->SetParameter(0,0);
	line->SetParameter(1,1);

	double sigma[2];
	double binWidth[2];
	double weightSum[2];

	// 0=minbias, 1=inel 
	for (int i=0; i < 2;i++)
	{
		data[i] = (TH1F*)files[i]->Get("data");
		sigma[i]= data[i]->GetBinContent(1);
		binWidth[i] = data[i]->GetBinContent(2);
		weightSum[i] = data[i]->GetBinContent(3);
	}


	// scaling 
	minBias->Scale( sigma[0] / ( 2*TMath::Pi() * weightSum[0] * binWidth[0] * 2 * TMath::Pi()) );
	minBias->Divide(line);
	inel->Scale( sigma[1] / ( 2*TMath::Pi() * weightSum[1] * binWidth[1] * 2 * TMath::Pi()));
	inel->Divide(line);
	hard->Divide(line);

	// Drawing all three same plot
	TCanvas *c1 = new TCanvas("c1");
	c1->SetLogy();
	minBias->Draw("hist");
	inel->SetLineColor(kRed);
	inel->Draw("hist same");
	hard->SetLineColor(kBlack);
	hard->Draw("same hist");
	aliceResults->SetLineColor(kGreen);
	aliceResults->Draw("hist same");

	TLegend *leg = new TLegend();
	leg->AddEntry(minBias, "Min Bias", "L");
	leg->AddEntry(inel, "Inelastic", "L");
	leg->AddEntry(hard, "Hard", "L");
	leg->AddEntry(aliceResults, "Alice", "L");
	leg->Draw("same");




TFile *fout = TFile::Open("13TeVPlots.root" , "RECREATE");
fout->cd();

inel->Write();
minBias->Write();
hard-> Write();
aliceResults->Write();
fout->Close();	

}