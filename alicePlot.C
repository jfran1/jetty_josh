
void alicePlot()
{

	TFile *f1= TFile::Open("/Users/joshfrandsen/test/jetty/output/alice_2_4.root");
	if (f1 ==0) cout<< "ERROR! File 1 didn't open" << endl;
	else cout << "File 1 Opened!" << endl;

	TFile *f2= TFile::Open("/Users/joshfrandsen/test/jetty/output/alice_4_6.root");
	if (f2 ==0) cout<< "ERROR! File 2 didn't open" << endl;
	else cout << "File 2 Opened!" << endl;

	TFile *f3= TFile::Open("/Users/joshfrandsen/test/jetty/output/alice_6_9.root");
	if (f3 ==0) cout<< "ERROR! File 3 didn't open" << endl;
	else cout << "File 3 Opened!" << endl;	

	TFile *f4= TFile::Open("/Users/joshfrandsen/test/jetty/output/alice_9_12.root");
	if (f4 ==0) cout<< "ERROR! File 4 didn't open" << endl;
	else cout << "File 4 Opened!" << endl;	

	TFile *f5= TFile::Open("/Users/joshfrandsen/test/jetty/output/alice_12_15.root");
	if (f5 ==0) cout<< "ERROR! File 5 didn't open" << endl;
	else cout << "File 5 Opened!" << endl;	

	TFile *f6= TFile::Open("/Users/joshfrandsen/test/jetty/output/alice_15_20.root");
	if (f6 ==0) cout<< "ERROR! File 6 didn't open" << endl;
	else cout << "File 6 Opened!" << endl;

	TFile *f7= TFile::Open("/Users/joshfrandsen/test/jetty/output/alice_20_25.root");
	if (f7 ==0) cout<< "ERROR! File 7 didn't open" << endl;
	else cout << "File 7 Opened!" << endl;

	TFile *f8= TFile::Open("/Users/joshfrandsen/test/jetty/output/alice_25_35.root");
	if (f8 ==0) cout<< "ERROR! File 8 didn't open" << endl;
	else cout << "File 8 Opened!" << endl;	

	TFile *f9= TFile::Open("/Users/joshfrandsen/test/jetty/output/alice_35_45.root");
	if (f9 ==0) cout<< "ERROR! File 9 didn't open" << endl;
	else cout << "File 9 Opened!" << endl;	

	TFile *f10= TFile::Open("/Users/joshfrandsen/test/jetty/output/alice_45_60.root");
	if (f10 ==0) cout<< "ERROR! File 10 didn't open" << endl;
	else cout << "File 10 Opened!" << endl;	

	TFile *f11= TFile::Open("/Users/joshfrandsen/test/jetty/output/alice_60_80.root");
	if (f11 ==0) cout<< "ERROR! File 11 didn't open" << endl;
	else cout << "File 11 Opened!" << endl;

	TFile *f12= TFile::Open("/Users/joshfrandsen/test/jetty/output/alice_80_120.root");
	if (f12 ==0) cout<< "ERROR! File 12 didn't open" << endl;
	else cout << "File 12 Opened!" << endl;

	TFile *f13= TFile::Open("/Users/joshfrandsen/test/jetty/output/alice_120_180.root");
	if (f13 ==0) cout<< "ERROR! File 13 didn't open" << endl;
	else cout << "File 13 Opened!" << endl;	

	TFile *f14= TFile::Open("/Users/joshfrandsen/test/jetty/output/alice_180_250.root");
	if (f14 ==0) cout<< "ERROR! File 14 didn't open" << endl;
	else cout << "File 14 Opened!" << endl;	

	TFile *f15= TFile::Open("/Users/joshfrandsen/test/jetty/output/alice_250_350.root");
	if (f15 ==0) cout<< "ERROR! File 15 didn't open" << endl;
	else cout << "File 15 Opened!" << endl;	

	TFile *f16= TFile::Open("/Users/joshfrandsen/test/jetty/output/alice_350_1.root");
	if (f16 ==0) cout<< "ERROR! File 16 didn't open" << endl;
	else cout << "File 16 Opened!" << endl;	


	TFile *f17 = TFile::Open("/Users/joshfrandsen/test/jetty/output/HEPData-ins1241422-v1-Table1.root");
	if(f17 ==0) cout<< "ERROR! File 17 didn't open" << endl;
	else cout << "File 17 Opened!" << endl;	
	f17->cd("Table 1");

	// Setting up arrays of histograms and files
	TFile *files[16] = {f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16};
	gStyle->SetPalette(100);
	TH1F *hpT[16];
	TH1F *sumHpT[16];
	TH1F *raw[16];
	TH1F *HEP = (TH1F*)f17->Get("Hist1D_y3_e2");
	TH2F *eta2pT[16];

	//Drawing On Canvas 1
	TCanvas *c1 = new TCanvas("c1");
	c1->SetLogy();
	for ( int i = 0; i < 16; i++)
	{
		hpT[i] = (TH1F*)files[i]->Get("hpT");
		sumHpT[i] = (TH1F*)files[i]->Get("hpT");
		raw[i] = (TH1F*)files[i]->Get("hpTRaw");
		hpT[0]->SetMinimum(10e-10);
		if (i%2 == 0) 
		{
			hpT[i]->SetLineColor(kBlue);
			hpT[i]->Draw("same hist");
		}
		else 
		{
			hpT[i]->SetLineColor(kBlack);
			hpT[i]->Draw("same hist");
		}

	}

	// Summing the histograms, and drawing
	for (int i =1; i<16; i++)
	{
		sumHpT[0]->Add(sumHpT[i]);
		raw[0]->Add(raw[i]);
	}

	TCanvas *c2 = new TCanvas("c2");
	c2->SetLogy();
	sumHpT[0]->SetTitle("");
	sumHpT[0]->SetLineColor(kRed);
	sumHpT[0]->Draw("hist");
	raw[0]->Draw("hist same");








}