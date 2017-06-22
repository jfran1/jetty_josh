
void allEnergyAlice()
{

	TFile *f1= TFile::Open("/Users/joshfrandsen/test/jetty/output/7TeV.root");
	if (f1 ==0) cout<< "ERROR! File 1 didn't open" << endl;
	else cout << "File 1 Opened!" << endl;

	TFile *f2 = TFile::Open("/Users/joshfrandsen/test/jetty/output/2.76TeV.root"); 
	if (f2==0) cout<< "ERROR! File 2 didn't open" << endl;
	else cout << "File2 Opened!" << endl;
	

	TFile *f3 = TFile::Open("/Users/joshfrandsen/test/jetty/output/0.9TeV.root");
	if (f3==0) cout<< "ERROR! File 3 didn't open" << endl;
	else cout << "File 3 Opened!" << endl;
	

	TFile *f4 = TFile::Open("/Users/joshfrandsen/test/jetty/output/HEPData-ins1241422-v1-Table1.root");
	if (f4==0) cout<< "ERROR! File 4 didn't open" << endl;
	else cout << "File 4 Opened!" << endl;
	f4->cd("Table 1");
	

	TH1F *alice7TeV = (TH1F*)gDirectory->Get("Hist1D_y3");
	TH1F *alice2TeV = (TH1F*)gDirectory->Get("Hist1D_y2");
	TH1F *alice1TeV = (TH1F*)gDirectory->Get("Hist1D_y1") ; 
	TFile *files[4] = {f1,f2,f3,f4}; 
	TH1F *data[3];
	TH1F *TeV7 = (TH1F*)f1->Get("hpTRaw"); // not scaled 
	TH1F *TeV2 = (TH1F*)f2->Get("hpTRaw"); // not scaled
	TH1F *TeV1 = (TH1F*)f3->Get("hpTRaw"); // not scaled

	double sigma[3];
	double binWidth[3];
	double weightSum[3];
	double dEta = 0.8;

	// 0=7 TeV, 1=2.76 TeV, 2 = 0.9 TeV
	for (int i=0; i < 3;i++)
	{
		data[i] = (TH1F*)files[i]->Get("data");
		sigma[i]= data[i]->GetBinContent(1);
		binWidth[i] = data[i]->GetBinContent(3);
		weightSum[i] = data[i]->GetBinContent(2);
		cout << "cross section " << i << ": " << sigma[i] << endl;
		cout << "weightSum " << i << ": " << weightSum[i] << endl;
	}



	// scaling 
	TeV7->Scale( sigma[0] / ( 2*TMath::Pi() * weightSum[0] * binWidth[0] * 2*dEta) );
	TeV2->Scale( sigma[1] / ( 2*TMath::Pi() * weightSum[1] * binWidth[1] * 2*dEta ));
	TeV1->Scale(sigma[2]/ (2*TMath::Pi() * weightSum[1] * binWidth[1] * 2*dEta));

	// Drawing all three same plot
	TCanvas *c1 = new TCanvas("c1");
	c1->SetLogy();
	TeV7->SetYTitle("1/(2#pip_{T})(d#sigma^{2}/(d#etadp_{T}) [mb GeV^{-2}]");
	TeV7->SetLineColor(kRed);	
	TeV7->Draw("hist");
	TeV2->SetYTitle("1/(2#pip_{T})(d#sigma^{2}/(d#etadp_{T}) [mb GeV^{-2}]");
	TeV2->SetLineColor(kRed+2);
	TeV2->Draw("hist same");
	TeV1->SetYTitle("1/(2#pip_{T})(d#sigma^{2}/(d#etadp_{T}) [mb GeV^{-2}]");
	TeV1->SetLineColor(kRed+4);
	TeV1->Draw("same hist");
	alice7TeV->SetLineColor(kBlue);
	alice7TeV->Draw("hist same");
	alice2TeV->SetLineColor(kBlue+2);
	alice2TeV->Draw("hist same");
	alice1TeV->SetLineColor(kBlue+4);
	alice1TeV->Draw("hist same");


	TLegend *leg = new TLegend();
	leg->AddEntry(TeV7, "Pythia8 7 TeV", "L");
	leg->AddEntry(TeV2, "Pythia8 2.76 TeV", "L");
	leg->AddEntry(TeV1, "Pythia8 0.9 Tev", "L");
	leg->AddEntry(alice7TeV, "Alice 7 TeV", "L");
	leg->AddEntry(alice2TeV, "Alice 2.76 TeV", "L");
	leg->AddEntry(alice1TeV, "Alice 0.9 TeV", "L");
	leg->Draw("same");


	TeV7->SetTitle("Pythia8 vs. Alice p_{T} Distribution");



// Write some histograms to root file
TFile *fout = TFile::Open("3EnergyPlots.root" , "RECREATE");
fout->cd();

TeV2->Write("inel");
TeV7->Write("7TeV");
TeV1-> Write("1TeV");
alice7TeV->Write("alice7TeV");
alice2TeV->Write("alice2TeV");
alice1TeV->Write("alice1TeV");

fout->Close();	

}