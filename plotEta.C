
void plotEta()
{

	TFile *f1= TFile::Open("/Users/joshfrandsen/test/jetty/output/eta_pTbin.root");
	if (f1 ==0) cout<< "ERROR! File didn't open" << endl;
	else cout << "File1 Opened!" << endl;

	// Get plots
	TH1F *h1 = (TH1F*)f1->Get("etaLowpT");
	TH1F *h2 = (TH1F*)f1->Get("etaMedpT");
	TH1F *h3 = (TH1F*)f1->Get("etaHighpT");

	gStyle->SetOptStat(kFALSE);
	TCanvas *c1 = new TCanvas("Eta Plots");
	c1->Divide(2,1);
	c1->cd(1);
	h1->SetTitle("pT < 5 [GeV]");
	h1-> Draw();
	c1->cd(2);
	h3->SetLineColor(kRed);
	h2->SetTitle("20 < pT < 25 [GeV] (red); 10 < pT < 15 [GeV] (blue) ");
	h2-> Draw();
	h3-> Draw("same");





}