
void plot13TeV()
{

	TFile *f1= TFile::Open("/Users/joshfrandsen/test/jetty/output/13TeV.root");
	if (f1 ==0) cout<< "ERROR! File 1 didn't open" << endl;
	else cout << "File 1 Opened!" << endl;

	TFile *f2 = TFile::Open("/Users/joshfrandsen/test/jetty/output/13TeVAliceResults.root");
	if (f2==0) cout<< "ERROR! File 4 didn't open" << endl;
	else cout << "File 4 Opened!" << endl;
	f2->cd("Table 2");
	

	TH1F *aliceResults = (TH1F*)gDirectory->Get("Hist1D_y1");
	TH1F *TeV13 = (TH1F*)f1->Get("hpTRaw"); // not scaled 
	TH1F *data = (TH1F*)f1->Get("data");


	double sigma = data->GetBinContent(1);
	double weightSum = data->GetBinContent(2);
	double binWidth = data->GetBinContent(3);
	double dEta = 0.8;

	cout << "cross section: "<< sigma << endl;
	cout << "weightSum: " << weightSum << endl;
	cout << "Bin Width: " << binWidth << endl;



	// scaling 
	TeV13->Scale( 1 / ( 2*TMath::Pi() * weightSum * binWidth * 2 * TMath::Pi() *2*dEta) );
	TH1F *copyTeV13 = (TH1F*)TeV13->Clone();
	copyTeV13->Scale(5);

	// Drawing all three same plot
	TCanvas *c1 = new TCanvas("c1");
	c1->SetLogy();
	TeV13->SetYTitle("1/(2#pip_{T})(dN^{2}/(d#etadp_{T}) [mb GeV^{-2}]");
	TeV13->Draw("hist");
	aliceResults->SetLineColor(kRed+4);
	aliceResults->Draw("hist same");
	copyTeV13->SetLineColor(kMagenta);
	copyTeV13->Draw("hist same");

	TLegend *leg = new TLegend();
	leg->AddEntry(TeV13, "Pythia8 13 [TeV]", "L");
	leg->AddEntry(aliceResults, "Alice 13 [TeV]", "L");
	leg->AddEntry(copyTeV13, "Pythia8 Plot Scaled by 5", "L");
	leg->Draw("same");


	TeV13->SetTitle("Pythia8 vs. Alice 13 TeV p_{T} Distribution");



// Write some histograms to root file
TFile *fout = TFile::Open("13TeVPlots.root" , "RECREATE");
fout->cd();

TeV13->Write("TeV13");
aliceResults->Write("aliceResults");

fout->Close();	

}