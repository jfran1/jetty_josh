
void plotPartNormpT()
{

	TFile *f1= TFile::Open("/Users/joshfrandsen/test/jetty/output/partNorm_2_4.root");
	if (f1 ==0) cout<< "ERROR! File 1 didn't open" << endl;
	else cout << "File 1 Opened!" << endl;

	TFile *f2= TFile::Open("/Users/joshfrandsen/test/jetty/output/partNorm_4_6.root");
	if (f2 ==0) cout<< "ERROR! File 2 didn't open" << endl;
	else cout << "File 2 Opened!" << endl;

	TFile *f3= TFile::Open("/Users/joshfrandsen/test/jetty/output/partNorm_6_8.root");
	if (f3 ==0) cout<< "ERROR! File 3 didn't open" << endl;
	else cout << "File 3 Opened!" << endl;	

	TFile *f4= TFile::Open("/Users/joshfrandsen/test/jetty/output/partNorm_8_11.root");
	if (f4 ==0) cout<< "ERROR! File 4 didn't open" << endl;
	else cout << "File 4 Opened!" << endl;	

	TFile *f5= TFile::Open("/Users/joshfrandsen/test/jetty/output/partNorm_11_14.root");
	if (f5 ==0) cout<< "ERROR! File 5 didn't open" << endl;
	else cout << "File 5 Opened!" << endl;	

	TFile *f6= TFile::Open("/Users/joshfrandsen/test/jetty/output/partNorm_14_20.root");
	if (f6 ==0) cout<< "ERROR! File 6 didn't open" << endl;
	else cout << "File 6 Opened!" << endl;

	TFile *f7= TFile::Open("/Users/joshfrandsen/test/jetty/output/partNorm_20_25.root");
	if (f7 ==0) cout<< "ERROR! File 7 didn't open" << endl;
	else cout << "File 7 Opened!" << endl;

	TFile *f8= TFile::Open("/Users/joshfrandsen/test/jetty/output/partNorm_25_30.root");
	if (f8 ==0) cout<< "ERROR! File 8 didn't open" << endl;
	else cout << "File 8 Opened!" << endl;	

	TFile *f9= TFile::Open("/Users/joshfrandsen/test/jetty/output/partNorm_30_40.root");
	if (f9 ==0) cout<< "ERROR! File 9 didn't open" << endl;
	else cout << "File 9 Opened!" << endl;	

	TFile *f10= TFile::Open("/Users/joshfrandsen/test/jetty/output/partNorm_40_50.root");
	if (f10 ==0) cout<< "ERROR! File 10 didn't open" << endl;
	else cout << "File 10 Opened!" << endl;	

	TFile *f11= TFile::Open("/Users/joshfrandsen/test/jetty/output/partNorm_50_70.root");
	if (f11 ==0) cout<< "ERROR! File 11 didn't open" << endl;
	else cout << "File 11 Opened!" << endl;

	TFile *f12= TFile::Open("/Users/joshfrandsen/test/jetty/output/partNorm_70_100.root");
	if (f12 ==0) cout<< "ERROR! File 12 didn't open" << endl;
	else cout << "File 12 Opened!" << endl;

	TFile *f13= TFile::Open("/Users/joshfrandsen/test/jetty/output/partNorm_100_150.root");
	if (f13 ==0) cout<< "ERROR! File 13 didn't open" << endl;
	else cout << "File 13 Opened!" << endl;	

	TFile *f14= TFile::Open("/Users/joshfrandsen/test/jetty/output/partNorm_150_200.root");
	if (f14 ==0) cout<< "ERROR! File 14 didn't open" << endl;
	else cout << "File 14 Opened!" << endl;	

	TFile *f15= TFile::Open("/Users/joshfrandsen/test/jetty/output/partNorm_200_270.root");
	if (f15 ==0) cout<< "ERROR! File 15 didn't open" << endl;
	else cout << "File 15 Opened!" << endl;	

	TFile *f16= TFile::Open("/Users/joshfrandsen/test/jetty/output/partNorm_270_350.root");
	if (f16 ==0) cout<< "ERROR! File 16 didn't open" << endl;
	else cout << "File 16 Opened!" << endl;	

	TFile *f17= TFile::Open("/Users/joshfrandsen/test/jetty/output/partNorm_350_1.root");
	if (f17 ==0) cout<< "ERROR! File 17 didn't open" << endl;
	else cout << "File 17 Opened!" << endl;	


	// Setting up arrays of histograms and files
	TFile *files[17] = {f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17};
	gStyle->SetPalette(100);
	TH1F *final[17];
	TH1F *partons[17];
	TH1F *sumPart[17];
	TH1F *sumFinal[17];


	//Drawing On Canvas 1
	TCanvas *c1 = new TCanvas("c1");
	c1->SetLogy();
	for ( int i = 0; i < 17; i++)
	{
		partons[i] = (TH1F*)files[i]->Get("partNorm");
		sumPart[i] = (TH1F*)files[i]->Get("partNorm");
		partons[0]->SetMinimum(10e-10);
		if (i%2 == 0) 
		{
			partons[i]->SetLineColor(kBlue);
			partons[i]->Draw("same hist");
		}
		else 
		{
			partons[i]->SetLineColor(kBlack);
			partons[i]->Draw("same hist");
		}

	}

	// Drawing on Canvas 2
	TCanvas *c2 = new TCanvas("c2");
	c2->SetLogy();

	for ( int i = 0; i < 17; i++)
	{
		final[i] = (TH1F*)files[i]->Get("final");
		sumFinal[i] = (TH1F*)files[i]->Get("final");
		final[0]->SetMinimum(10e-10);		
		if (i%2 == 0)
		{
			final[i]->SetLineColor(kBlue);
			final[i]->Draw("same hist");
		}
		else
		{
			final[i]->SetLineColor(kBlack);
			final[i]->Draw("same hist");
		}
	}

	// Drawing on canvas 3
	for (int i =1; i<17; i++)
	{
		sumPart[0]->Add(sumPart[i]);
		sumFinal[0]->Add(sumFinal[i]);
	}

	TCanvas *c3 = new TCanvas("c3");
	c3->SetLogy();
	sumPart[0]->SetTitle("");
	sumPart[0]->SetLineColor(kBlue);
	sumPart[0]->Draw("hist");
	sumFinal[0]->SetLineColor(kBlack);
	sumFinal[0]->Draw("hist same");

	TLegend *sumLeg = new TLegend();
	sumLeg->AddEntry(sumPart[0], "Outgoing Partons" , "L");
	sumLeg->AddEntry(sumFinal[0], "Final State Particles", "L");
	sumLeg->Draw();


/*
	

	TLegend *leg = new TLegend();
	leg->AddEntry(h1, "2 < p_{T} Hat < 4", "L" );
	leg->AddEntry(h2, "4 < p_{T} Hat < 6", "L" );
	leg->AddEntry(h3, "6 < p_{T} Hat < 8", "L" );
	leg->AddEntry(h4, "8 < p_{T} Hat < 11", "L" );
	leg->AddEntry(h5, "11 < p_{T} Hat < 14", "L" );
	leg->AddEntry(h6, "14 < p_{T} Hat < 20", "L" );
	leg->AddEntry(h7, "20 < p_{T} Hat < 25", "L" );
	leg->AddEntry(h8, "25 < p_{T} Hat < 30", "L" );
	leg->AddEntry(h9, "30 < p_{T} Hat < 40", "L" );
	leg->AddEntry(h10, "40 < p_{T} Hat < 50", "L" );
	leg->AddEntry(h11, "50 < p_{T} Hat < 70", "L" );
	leg->AddEntry(h12, "70 < p_{T} Hat < 100", "L" );
	leg->AddEntry(h13, "100 < p_{T} Hat < 150", "L" );
	leg->AddEntry(h14, "150 < p_{T} Hat < 200", "L" );
	leg->AddEntry(h15, "200 < p_{T} Hat < 270", "L" );
	leg->AddEntry(h16, "270 < p_{T} Hat < 350", "L" );
	leg->AddEntry(h17, "350 < p_{T} Hat < -1", "L" );

*/


}