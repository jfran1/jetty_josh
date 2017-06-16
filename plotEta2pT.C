

void plotEta2pT()
{
	// Open files
	TFile *f1 = TFile::Open("/Users/joshfrandsen/test/jetty/output/eta2pT_20_70.root");
	if (f1 == 0) cout << "ERROR: FILE 1 DID NOT OPEN!" << endl;
	else cout << "FILE 1 OPENED!" << endl;

	TFile *f2 = TFile::Open("/Users/joshfrandsen/test/jetty/output/eta2pT_70_120.root");
	if (f2 == 0) cout << "ERROR: FILE 2 DID NOT OPEN!" << endl;
	else cout << "FILE 2 OPENED!" << endl;

	TFile *f3 = TFile::Open("/Users/joshfrandsen/test/jetty/output/eta2pT_120_170.root");
	if (f3 == 0) cout << "ERROR: FILE 3 DID NOT OPEN!" << endl;
	else cout << "FILE 3 OPENED!" << endl;

	TFile *f4 = TFile::Open("/Users/joshfrandsen/test/jetty/output/eta2pT_170_220.root");
	if (f4 == 0) cout << "ERROR: FILE 4 DID NOT OPEN!" << endl;
	else cout << "FILE 4 OPENED!" << endl;


	//Get Histograms 
	TH2F *h1 = (TH2F*)f1->Get("eta2pT");
	TH2F *h2 = (TH2F*)f2->Get("eta2pT");
	TH2F *h3 = (TH2F*)f3->Get("eta2pT");
	TH2F *h4 = (TH2F*)f4->Get("eta2pT");

	h1->SetTitle("20 < p_{T} Hat < 70 ");
	h2->SetTitle("70 < p_{T} Hat < 120");
	h3->SetTitle("120 < p_{T} Hat < 170");	
	h4->SetTitle("170 < p_{T} Hat < 220");	

	TCanvas *c1 = new TCanvas("eta vs p_{T}");
	c1->Divide(2,2);
	c1->cd(1)->SetLogz();
	c1->cd(2)->SetLogz();
	c1->cd(3)->SetLogz();
	c1->cd(4)->SetLogz();

	c1->cd(1);
	h1->Draw("contz");
	
	c1->cd(2);
	h2->Draw("contz");
	
	c1->cd(3);
	h3->Draw("contz");
	
	c1->cd(4);
	h4->Draw("contz");



}
