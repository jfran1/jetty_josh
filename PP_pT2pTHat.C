void PP_pT2pTHat()
{

	// open file and check if it opened
	TFile *f1= TFile::Open("/Users/joshfrandsen/test/jetty/output/pTHat_80_120.root");
	if (f1 ==0) cout<< "ERROR! File didn't open" << endl;
	else cout << "File1 Opened!" << endl;

	//open next set of data
	TFile *f2= TFile::Open("/Users/joshfrandsen/test/jetty/output/pTHat_20_50.root");
	if (f2 ==0) cout<< "ERROR! File didn't open" << endl;
	else cout << "File2 Opened!" << endl;

	TFile *f3= TFile::Open("/Users/joshfrandsen/test/jetty/output/pTHat_140_200.root");
	if (f3 ==0) cout<< "ERROR! File didn't open" << endl;
	else cout << "File3 Opened!" << endl;

	//fitting functions
	//TF1 *line = new TF1("line", "[0] + [1] * x[0]", 80, 120);
	TF1 *line1 = new TF1("line1", "pol1", 80, 120);
	line1 -> SetParameter(0, 0.0);
	line1 -> SetParameter(1, 1.0);

	TF1 *line2 = new TF1("line2", "pol1", 19, 50);
	line2 -> SetParameter(0, 0.0);
	line2 -> SetParameter(1, 1.0);

	TF1 *line3 = new TF1("line3", "pol1", 140, 200);
	line2 -> SetParameter(0, 0.0);
	line2 -> SetParameter(1, 1.0);	

	TF1 *line4 = new TF1("line4", "pol1", 40, 100);
	line4 -> SetParameter(0, 0.0);
	line4 -> SetParameter(1, .5);	

	TF1 *line5 = new TF1("line5", "pol1", 160, 240);
	line5 -> SetParameter(0, 0.0);
	line5 -> SetParameter(1, .5);	

	TF1 *line6 = new TF1("line6", "pol1", 280, 300);
	line6 -> SetParameter(0, 0.0);
	line6 -> SetParameter(1, .5);


	// ############ Get plots from file ############
	TH2F *h2 = (TH2F*)f2->Get("pT2pTHat");
	TH2F * h1 = (TH2F*)f1->Get("pT2pTHat"); 
	TH2F *h3 = (TH2F*)f3->Get("pT2pTHat");
	TH2F *sum1 = (TH2F*)f2->Get("sumpT");
	TH2F *sum2 = (TH2F*)f1->Get("sumpT");
	TH2F *sum3 = (TH2F*)f3->Get("sumpT");

	// ############ Make profile of plots ##########
	TProfile *ph2 = (TProfile*)h2->ProfileX("20-50");
	TProfile *ph1 = (TProfile*)h1->ProfileX("80-120");
	TProfile *ph3 = (TProfile*)h3->ProfileX("140-200");
	TProfile *psum1 = (TProfile*)sum1->ProfileX("sum1");
	TProfile *psum2 = (TProfile*)sum2->ProfileX("sum2");
	TProfile *psum3 = (TProfile*)sum3->ProfileX("sum3");

	// ########### Fit profiles ##############
	cout << endl << "############# Fit for p_{T}_Hat [20,50] ################" << endl;
	ph2->Fit(line2, "RMNL+ w=1");
	cout << endl << "############# Fit for p_{T}_Hat [80,120] ################" << endl;
	ph1->Fit(line1, "RMNL+ w=1");
	cout << endl << "############# Fit for p_{T}_Hat [140,200] ################" << endl;
	ph3->Fit(line3, "RMNL+ w=1");
	cout << endl << "################### Fit for pT sum1  ######################" << endl;
	psum1->Fit(line4, "RMNL+ w=1");
	cout << endl << "################### Fit for pT sum2  ######################" << endl;
	psum2->Fit(line5, "RMNL+ w=1");
	cout << endl << "################### Fit for pT sum3  ######################" << endl;
	psum3->Fit(line6, "RMNL+ w=1");


	//######## Graph ##################

	// Canvas 1
	TCanvas *c1=  new TCanvas();
	c1->SetLogz();

	h1->Draw("contz");
	h2->Draw("cont, sames");
	h3->Draw("cont, sames");
	line1->Draw("same");
	line2->Draw("same");
	line3->Draw("same");
	line4->Draw("same");

	// Canvas 2
	TCanvas *c2=new TCanvas("Sum of pT from outgoing partons");
	c2->SetLogz();

	sum1->Draw("contz");
	sum2->Draw("cont, sames");
	sum3->Draw("cont, sames");
	line4->Draw("same");
	line5->Draw("same");
	line6->Draw("same");

}