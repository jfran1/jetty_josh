

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
	TH1F *h_dpt[nfiles];
	TH1F *projections[nfiles];
	TH1F *norm[nfiles];
	TH1F *test[nfiles];

	double sigma[nfiles];
	double weight[nfiles];

	//Get TNtuple from files 
	//######################################################################
	TCanvas *c1 = new TCanvas("Test1");
	TCanvas *c2 = new TCanvas("Test2");
	TCanvas *c3 = new TCanvas("TEST");

	for(int i =0; i<nfiles; i++)
	{
		test[i] = (TH1F*)files[i]->Get("test_hist");
		
		if(i !=0 )
		{
			test[0]->Add(test[i]);
		}
	}


	c1->cd();
	for(int i=0; i < nfiles; i++)
	{
		norm[i] = (TH1F*)files[i]->Get("norm");
		sigma[i] = norm[i]->GetBinContent(1);
		weight[i] = norm[i]->GetBinContent(2);
		tuples[i] = (TNtuple*)files[i]->Get("jet_space_pT");
		// h_dpt[i] = tuple2Hist(tuples[i], "delta_pt");
		
		TString temp_string = TString::Format("delta_pt>>h_dpt%i", i);
		TString hist_name = TString::Format("h_dpt%i", i);
		h_dpt[i] = new TH1F(hist_name, "Title; #Deltap_{T}; counts", 500, -100, 100);
		tuples[i]->Draw(temp_string, "gamma_pt > 20 && gamma_pt < 40", "");
		h_dpt[i]->Rebin(5);
		// h_dpt[i]->Scale( sigma[i] / (weight[i] * h_dpt[i]->GetBinWidth(1)) ); // normalize might be off because I'm looking at a cut of the data, so norm values might be off

		c1->Close();
	}	

	c3->cd();
	test[0]->SetLineColor(kRed);
	test[0]->SetMarkerStyle(27);
	test[0]->SetMarkerColor(kRed);
	test[0]->Rebin(5);
	test[0]->Draw("hist pe");


	TH1F *clone_sum = (TH1F*)h_dpt[0]->Clone("sum");
	for(int i =1; i < nfiles; i++)
	{
		clone_sum->Add(h_dpt[i]);
	}

	clone_sum->SetMarkerStyle(24);
	clone_sum->SetMarkerColor(kRed);
	clone_sum->SetLineColor(kRed);
	clone_sum->SetLineWidth(2);
	clone_sum->SetYTitle("#frac{1}{N_{events}} #frac{d#sigma}{dp_{T}} [mb / GeV]");

	c2->cd();
	clone_sum->Draw("pe");

	c1->SetLogy();

}