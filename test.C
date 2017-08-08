

// This function will take a tuple and a branch name then give you a histogram
// cut: Allows you to pass a bool statement for any type of cuts associated with your graph from tuple data
// bin: number of bins
// low: lower limit bin
// up : upper limit bin
//####################################################################################################
TH1F* tuple2Hist(TNtuple *tuple,  TString branch, TString cut="", int bin=75, int low=0, int up = 150)
{
	TString draw_param = TString::Format("%s>>hist", branch.Data());
	TCanvas *temp = new TCanvas("temp");
	temp->cd();
	TH1F *hist = new TH1F("hist", branch, bin, low, up);
	tuple->Draw(draw_param, cut);
	temp->Close();
	
	return hist;
} // Having an issue with this function when looping due to naming of the histograms 
//###################################################################################



void test()
{

	//Opening Files
	//######################################################################
	const char *filenames[] =
	{	"~/test/jetty/output/jetGamma_10_20.root", // 0
		"~/test/jetty/output/jetGamma_20_30.root", // 2
		"~/test/jetty/output/jetGamma_30_40.root", // 3
		"~/test/jetty/output/jetGamma_40_50.root", // 4
		"~/test/jetty/output/jetGamma_50_-1.root", // 5
		0
	};

	int _temp = 0;						
	while(filenames[_temp] != 0) _temp++;

	const int nfiles = _temp;
	TFile *files[nfiles];
	for (int i =0; i < nfiles; i++)
	{
		files[i] = TFile::Open(filenames[i]);
		if (files[i] ==0) 
		{
			cout<< "ERROR! File " << i << " didn't open" << endl;
			return;
		}	
	}

	//Declare tuples/hist 
	//######################################################################
	TNtuple *tuples[nfiles];
	TH1F *h_gammaPt[nfiles];
	TH1F *h_dpt_1[nfiles];
	TH1F *h_dpt_2[nfiles];
	TH1F *h_dpt_3[nfiles];
	TH1F *projections[nfiles];
	TH1F *norm[nfiles];
	TH1F *test[nfiles];

	double sigma[nfiles];
	double weight[nfiles];

	//Get TNtuple from files 
	//######################################################################
	TCanvas *c1 = new TCanvas("dpt_20_40");
	TCanvas *c2 = new TCanvas("dpt_40_80");
	TCanvas *c3 = new TCanvas("dpt_80_120");
	TCanvas *c4 = new TCanvas("TEMP");

	c4->cd();
	for(int i=0; i < nfiles; i++)
	{
		norm[i] = (TH1F*)files[i]->Get("norm");
		sigma[i] = norm[i]->GetBinContent(1);
		weight[i] = norm[i]->GetBinContent(2);
		tuples[i] = (TNtuple*)files[i]->Get("jet_space_pT");
		
		TString plot_string_1 = TString::Format("delta_pt>>h_dpt_1_%i", i);
		TString plot_string_2 = TString::Format("delta_pt>>h_dpt_2_%i", i);
		TString plot_string_3 = TString::Format("delta_pt>>h_dpt_3_%i", i);
		TString hist_name_1 = TString::Format("h_dpt_1_%i", i);
		TString hist_name_2 = TString::Format("h_dpt_2_%i", i);
		TString hist_name_3 = TString::Format("h_dpt_3_%i", i);
		h_dpt_1[i] = new TH1F(hist_name_1, "Title; #Deltap_{T}; counts", 100, -100, 100);
		h_dpt_2[i] = new TH1F(hist_name_2, "Title; #Deltap_{T}; counts", 100, -100, 100);
		h_dpt_3[i] = new TH1F(hist_name_3, "Title; #Deltap_{T}; counts", 100, -100, 100);

		tuples[i]->Draw(plot_string_1, "gamma_pt > 20 && gamma_pt < 40", "");
		tuples[i]->Draw(plot_string_2, "gamma_pt > 40 && gamma_pt < 80", "");
		tuples[i]->Draw(plot_string_3, "gamma_pt > 80 && gamma_pt < 120", "");

		h_dpt_1[i]->Scale( sigma[i] / (weight[i] * h_dpt_1[i]->GetBinWidth(1)) ); 
		h_dpt_2[i]->Scale( sigma[i] / (weight[i] * h_dpt_2[i]->GetBinWidth(1)) ); 
		h_dpt_3[i]->Scale( sigma[i] / (weight[i] * h_dpt_3[i]->GetBinWidth(1)) ); 

		c4->Close();
	}	


	TH1F *sum_20_40 = (TH1F*)h_dpt_1[0]->Clone("sum_20_40");
	TH1F *sum_40_80 = (TH1F*)h_dpt_2[0]->Clone("sum_40_80");
	TH1F *sum_80_120 = (TH1F*)h_dpt_3[0]->Clone("sum_80_120");

	for(int i =1; i < nfiles; i++)
	{
		sum_20_40->Add(h_dpt_1[i]);
		sum_40_80->Add(h_dpt_2[i]);
		sum_80_120->Add(h_dpt_3[i]);
	}

	sum_20_40->SetMarkerStyle(24);
	sum_20_40->SetMarkerColor(kRed);
	sum_20_40->SetLineColor(kRed);
	sum_20_40->SetLineWidth(2);
	sum_20_40->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb / GeV]");

	sum_40_80->SetMarkerStyle(24);
	sum_40_80->SetMarkerColor(kRed);
	sum_40_80->SetLineColor(kRed);
	sum_40_80->SetLineWidth(2);
	sum_40_80->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb / GeV]");

	sum_80_120->SetMarkerStyle(24);
	sum_80_120->SetMarkerColor(kRed);
	sum_80_120->SetLineColor(kRed);
	sum_80_120->SetLineWidth(2);
	sum_80_120->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb / GeV]");	
	
	c1->cd();	
	sum_20_40->Draw("pe");
	c2->cd();
	sum_40_80->Draw(" pe");
	c3->cd();
	sum_80_120->Draw(" pe");


}
