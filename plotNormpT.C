
void plotNormpT()
{

	TFile *f1= TFile::Open("/Users/joshfrandsen/test/jetty/output/normpT_20_70.root");
	if (f1 ==0) cout<< "ERROR! File 1 didn't open" << endl;
	else cout << "File 1 Opened!" << endl;

	TFile *f2= TFile::Open("/Users/joshfrandsen/test/jetty/output/normpT_70_120.root");
	if (f2 ==0) cout<< "ERROR! File 2 didn't open" << endl;
	else cout << "File 2 Opened!" << endl;

	TFile *f3= TFile::Open("/Users/joshfrandsen/test/jetty/output/normpT_120_170.root");
	if (f3 ==0) cout<< "ERROR! File 3 didn't open" << endl;
	else cout << "File 3 Opened!" << endl;	

	TFile *f4= TFile::Open("/Users/joshfrandsen/test/jetty/output/normpT_170_220.root");
	if (f4 ==0) cout<< "ERROR! File 4 didn't open" << endl;
	else cout << "File 4 Opened!" << endl;	


	// Get plots
	TH1F *h1 = (TH1F*)f1->Get("norm1");
	TH1F *h2 = (TH1F*)f2->Get("norm1");
	TH1F *h3 = (TH1F*)f3->Get("norm1");
	TH1F *h4 = (TH1F*)f4->Get("norm1");

	gStyle->SetPalette(100);
	TCanvas *c1 = new TCanvas("pT Distributions");
	c1->SetLogy();

	TLegend *leg = new TLegend();
	leg->AddEntry(h1, "20 < p_{T} Hat < 70", "L" );
	leg->AddEntry(h2, "70 < p_{T} Hat < 120", "L" );
	leg->AddEntry(h3, "120 < p_{T} Hat < 170", "L" );
	leg->AddEntry(h4, "170 < p_{T} Hat < 220", "L" );


	h1->SetMinimum(10e-10);
	h1->Draw();
	h2->SetLineColor(kRed);
	h2->Draw("same");
	h3->SetLineColor(kGreen);
	h3->Draw("same");
	h4->SetLineColor(kBlack);
	h4->Draw("same");
	leg->Draw("same");




}