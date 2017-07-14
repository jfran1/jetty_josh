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

	double sigma[nfiles];
	double weight[nfiles];

	//Get TNtuple from files 
	//######################################################################
	TCanvas *c1 = new TCanvas("Test1");
	TCanvas *c2 = new TCanvas("Test2");
	
	c1->cd();
	for(int i=0; i < nfiles; i++)
	{
		norm[i] = (TH1F*)files[i]->Get("norm");
		sigma[i] = norm[i]->GetBinContent(1);
		weight[i] = norm[i]->GetBinContent(2);
		TString temp_string = TString::Format("delta_pt>>h_dpt%i", i);
		TString hist_name = TString::Format("h_dpt%i", i);
		tuples[i] = (TNtuple*)files[i]->Get("jet_space_pT");
		h_dpt[i] = new TH1F(hist_name, "Title; #Deltap_{T}; counts", 75, -50, 50);
		tuples[i]->Draw(temp_string, "gamma_pt < 80 && gamma_pt > 40", "");
		h_dpt[i]->Scale( sigma[i] / (weight[i] * h_dpt[i]->GetBinWidth(1)) );
	}	
	TH1F *clone_sum = (TH1F*)h_dpt[0]->Clone("sum");
	c2->cd();
	std::cout << "[i] # of files: " << nfiles << std::endl;
	for(int i =1; i < nfiles; i++)
	{
		clone_sum->Add(h_dpt[i]);
	}

	clone_sum->SetMarkerStyle(24);
	clone_sum->SetMarkerColor(kRed);
	clone_sum->SetLineColor(kRed);
	clone_sum->SetLineWidth(2);
	clone_sum->Draw("pe");

	c1->SetLogy();
	c2->SetLogy();

}