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

	// set up plots
	cout << endl << "############# Fit for p_{T}_Hat [20,50] ################" << endl;
	TH2F *h2 = (TH2F*)f2->Get("pT2pTHat");
	TProfile *ph2 = (TProfile*)h2->ProfileX("20-50");
	ph2->Fit(line2, "RMN+ w=1");


	cout << endl << "############# Fit for p_{T}_Hat [80,120] ################" << endl;
	TH2F * h1 = (TH2F*)f1->Get("pT2pTHat"); 
	TProfile *ph1 = (TProfile*)h1->ProfileX("80-120");
	ph1->Fit(line1, "RMN+ w=1");

	cout << endl << "############# Fit for p_{T}_Hat [140,200] ################" << endl;
	TH2F *h3 = (TH2F*)f3->Get("pT2pTHat");
	TProfile *ph3 = (TProfile*)h3->ProfileX("140-200");
	ph3->Fit(line3, "RMN+ w=1");


	// set up canvas and draw 
	TCanvas *c1=  new TCanvas();
	c1->SetLogz();

	h1->Draw("contz");
	h2->Draw("cont, sames");
	h3->Draw("cont, sames");
	line1->Draw("same");
	line2->Draw("same");
	line3->Draw("same");

}