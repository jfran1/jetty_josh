
void alice2()
{

	TFile *f1= TFile::Open("/Users/joshfrandsen/test/jetty/output/minbias.root");
	if (f1 ==0) cout<< "ERROR! File 1 didn't open" << endl;
	else cout << "File 1 Opened!" << endl;

	TFile *f2 = TFile::Open("/Users/joshfrandsen/test/jetty/output/HEPData-ins1241422-v1-Table1.root");
	if (f2==0) cout<< "ERROR! File 2 didn't open" << endl;
	else cout << "File2 Opened!" << endl;
	f2->cd("Table 1");


	// Get plots
	TH1F *HEP = (TH1F*)gDirectory->Get("Hist1D_y3");
	TH1F *h1 = (TH1F*)f1->Get("hpT");
	TH1F *h2 = (TH1F*)f1->Get("data");
	TH1F *h3 = (TH1F*)f1->Get("hpTRaw");
	
	// make linear pT function 
	TF1 *line = new TF1("line", "pol1", 0,100);
	line->SetParameter(0,0);
	line->SetParameter(1,1);

	// Get data from histogram
	double sigma = h2->GetBinContent(1);
	double weightSum = h2->GetBinContent(2);
	double binWidth = h2->GetBinContent(3);

	cout << "Cross Section: " << sigma << endl;
	cout << "weightSum: " << weightSum << endl;
	cout << "Bin Width: " << binWidth << endl;

	//Draw plots with and without scaling
	TCanvas *c1 = new TCanvas("c1");
	c1->SetLogy();
	h3->SetLineColor(kGreen);
	h3->Scale( sigma / ( 2*TMath::Pi() * weightSum * binWidth * 2 * TMath::Pi() * 1000) );
	h3->Divide(line);
	h3->Draw("hist same");
	HEP->SetLineColor(kBlack);
	HEP->Draw("hist same");

	TLegend *leg = new TLegend();
	leg->AddEntry(h3, "Pythia8 |#eta| < 0.8", "L");
	leg->AddEntry(HEP, "Alice |#eta| < 0.8", "L");
	leg->Draw("same");
	

}