
void ratioPlot()
{

	TFile *f1= TFile::Open("/Users/joshfrandsen/test/jetty/output/minbias.root");
	if (f1 ==0) cout<< "ERROR! File 1 didn't open" << endl;
	else cout << "File 1 Opened!" << endl;

	TFile *f2 = TFile::Open("/Users/joshfrandsen/test/jetty/output/inel.root");
	if (f2==0) cout<< "ERROR! File 2 didn't open" << endl;
	else cout << "File2 Opened!" << endl;
	

	TFile *f3 = TFile::Open("/Users/joshfrandsen/test/jetty/output/inel-nsd.root");
	if (f3==0) cout<< "ERROR! File 3 didn't open" << endl;
	else cout << "File 3 Opened!" << endl;
	

	TFile *f4 = TFile::Open("/Users/joshfrandsen/test/jetty/output/sum.root");
	if (f4==0) cout<< "ERROR! File 4 didn't open" << endl;
	else cout << "File 4 Opened!" << endl;
	


	TFile *files[4] = {f1,f2,f3,f4};
	TH1F *data[3];
	TH1F *minBias = (TH1F*)f1->Get("hpTRaw");
	TH1F *inel = (TH1F*)f2->Get("hpTRaw");
	TH1F *inelNsd = (TH1F*)f3->Get("hpTRaw");
	TH1F *hard = (TH1F*)f4->Get("hpTRaw");

	double sigma[3];
	double binWidth[3];
	double weightSum[3];

	// 0=minbias, 1=inel, 2=inelNsd 
	for (int i=0; i < 3;i++)
	{
		data[i] = (TH1F*)files[i]->Get("data");
		sigma[i]= data[i]->GetBinContent(1);
		binWidth[i] = data[i]->GetBinContent(2);
		weightSum[i] = data[i]->GetBinContent(3);
	}


	minBias->Scale( sigma[0] / ( 2*TMath::Pi() * weightSum[0] * binWidth[0] * 2 * TMath::Pi()) );
	inelNsd->Scale( sigma[1] / ( 2*TMath::Pi() * weightSum[1] * binWidth[1] * 2 * TMath::Pi()) );
	inel->Scale( sigma[2] / ( 2*TMath::Pi() * weightSum[2] * binWidth[2] * 2 * TMath::Pi()) );


	TH1F *hardCopy1 = (TH1F*)hard->Clone();
	TH1F *hardCopy2 = (TH1F*)hard->Clone();
	TH1F *hardCopy3 = (TH1F*)hard->Clone();

	// Ratio of hard/inel
	TCanvas *c1 = new TCanvas("c1");
	c1->SetLogy();
	hardCopy1->Divide(inel);
	hardCopy1->SetTitle("hard/inel");
	hardCopy1->SetYTitle("d^{2}#sigma/(d#etadp_{T})[mb]");
	hardCopy1->Draw("hist");

	// Ratio hard/minbias
	TCanvas *c2 = new TCanvas("c2");
	c2->SetLogy();
	hardCopy2->Divide(minBias);
	hardCopy2->SetYTitle("d^{2}#sigma/(d#etadp_{T})[mb]");
	hardCopy2->SetTitle("hard/minBias");
	hardCopy2->Draw("hist");

	// Ratio hard/inel-nsd
	TCanvas *c3 = new TCanvas("c3");
	c3->SetLogy();
	hardCopy3->Divide(inelNsd);
	hardCopy3->SetTitle("hard/inel-nsd");
	hardCopy3->SetYTitle("d^{2}#sigma/(d#etadp_{T})[mb]");
	hardCopy3->Draw("hist");



	

}