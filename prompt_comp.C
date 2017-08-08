#include <fstream>
#include <string> 
#include <vector>








void prompt_comp()
{
	

	std::vector<double> pt_min;
	std::vector<double> pt_centers;
	std::vector<double> pt_max;
	std::vector<double> cross_section;
	std::vector<double> cross_error;

	std::ifstream in_file;
	in_file.open("/Users/joshfrandsen/test/jetty/output/fullnlo.txt");
	if(in_file.fail() )
	{
		std::cerr << "Failure to open input text file" << std::endl;
		exit(1);
	}

	in_file.ignore(1000, '\n');

	while (!in_file.eof())
	{	
		double _ptMin, _ptMax, _ptCenter, _crossSection, _crossError;
		in_file >> _ptMin >> _ptCenter >> _ptMax >> _crossSection >> _crossError;

		pt_min.push_back(_ptMin);
		pt_max.push_back(_ptMax);
		pt_centers.push_back(_ptCenter);
		cross_section.push_back(_crossSection);
		cross_error.push_back(_crossError);
	}


	// GET THE FILES 
	const char *file_names[] = 
	{
		"~/test/jetty/output/prompt_comp_10_20.root",	// 0
		"~/test/jetty/output/prompt_comp_20_30.root",	// 1
		"~/test/jetty/output/prompt_comp_30_50.root",	// 2
		"~/test/jetty/output/prompt_comp_50_70.root",	// 3
		"~/test/jetty/output/prompt_comp_70_100.root",	// 4
		"~/test/jetty/output/prompt_comp_100_-1.root",	// 5
		0
	};

	int _tmp = 0;
	while( file_names[_tmp] != 0 ) _tmp++;

	const int nfiles = _tmp;
	TFile *files[nfiles];
	for(int i = 0; i < nfiles; i++)
	{
		files[i] = TFile::Open(file_names[i]);
		if (files[i] == 0) std::cout << "Error opening file " << i  << std::endl;
	}

	//You are about to enter things by hand because you couldn't read from file
	double pt_center[] = {8.25, 8.75, 9.25, 9.75, 10.25, 10.75, 11.25, 11.75, 12.25, 
						  12.75, 13.25, 13.75, 14.25, 14.75, 15.25, 15.75, 16.25,
						  16.75, 17.25, 17.75, 18.25, 18.75, 19.25, 19.75 };
	double dsig2dpt[] = {
							50216.2571402 , 41239.3818138 , 31589.3146944,
							25650.0568973 , 21614.4803268 , 18034.5696895,
							15080.9014927 , 12384.0436133 , 10610.9883367,
							8876.8597562 ,  7842.8357013  , 6706.27456587,
							5802.55232051,  5003.70282613 , 4428.2858607, 
							3792.41702673,  3410.21435521 , 2983.9808265,
							2589.74056393,  2297.65236591 , 2088.08804661,
							1856.67733505,  1694.30832426 , 1548.3024392
						};

	double full_center[9];
	double full_cross[9];
	double full_error[9];
	for (int i =0; i < 9 ; i++)
	{
		full_center[i] = pt_centers[i];
		full_cross[i] = cross_section[i];
		full_error[i] = cross_error[i];
		std::cout << "cross: " << cross_section[i] << " center: " << full_center[i] << std::endl;
	}

	// DECLARE OBJECTS
	TChain ch_photon("weighted_photon");

	float bins[] = {10., 12., 14., 16., 18., 20., 25., 30., 40., 60.};
	TH1F *photon_pt = new TH1F("photon_pt" , "prompt photon; p_{T} GeV; #sigma", 15 , 0, 60);
	TGraphErrors *jet_phox = new TGraphErrors( 9, full_center, full_cross,0,full_error);
	
	double weightSum = ((TH1F*)files[0]->Get("norm"))->GetBinContent(2);
	double dEta = 0.27;

	for(int i = 0; i < nfiles; i++)
	{
		ch_photon.Add(file_names[i]);
	}

	// PUT NTUPLES INTO HISTOGRAM
	TCanvas *c_temp2 = new TCanvas();
	c_temp2->cd();
	ch_photon.Draw("pt>>photon_pt");
	c_temp2->Close();

	// Scale
	photon_pt->Scale( 1.e9 / (weightSum * photon_pt->GetBinWidth(1) ) ) ;

	double luminosity = 5.09 * 1.e3; //pb^-1
	double pp2pPb = 208;
	TH1F *scaleByLuminosity = (TH1F*)photon_pt->Clone("scaleByLuminosity");
	scaleByLuminosity->Scale(pp2pPb*luminosity);

	// Draw
	TCanvas *c1 = new TCanvas("promt_pt");
	TCanvas *c2 = new TCanvas("ExpectedNumberPhotons");
	
	photon_pt->SetLineColor(kBlack);
	photon_pt->SetMarkerColor(kBlack);
	photon_pt->SetMarkerStyle(24);
	photon_pt->SetLineWidth(2);
	photon_pt->SetYTitle(" d#sigma/dp_{T} [pb GeV^{-2}]");
	photon_pt->SetTitle("Prompt Photon Cross Section  #sqrt{s} = 5 TeV");


	jet_phox->SetLineColor(kRed);
	jet_phox->SetMarkerColor(kRed);
	jet_phox->SetMarkerStyle(20);
	jet_phox->SetLineWidth(2);

	scaleByLuminosity->SetLineColor(kBlack);
	scaleByLuminosity->SetMarkerColor(kBlack);
	scaleByLuminosity->SetMarkerStyle(24);
	scaleByLuminosity->SetLineWidth(2);
	scaleByLuminosity->SetYTitle("N_{pp->pPb} L d#sigma/dp_{T} [Number Of Photons]");

	TLegend *leg1 = new TLegend();
	leg1->AddEntry(photon_pt, "PYTHIA8 pp", "pl");
	leg1->AddEntry(jet_phox, "JETPHOX pp", "pl");

	TLegend *leg2 = new TLegend();
	leg2->AddEntry((TObject*)0, " | #eta | < 0.47 " , " " );

	TLegend *leg3 = new TLegend();
	leg3->AddEntry((TObject*)0, "L = 5090 pb^{-1} (luminosity) ", " ");
	leg3->AddEntry((TObject*)0, "N_{pp->pPb} = 208", " ");
	leg3->AddEntry((TObject*)0, " | #eta | < 0.47 " , " " );

	c1->cd();
	photon_pt->Draw("pe ");
	jet_phox->Draw("same pe");
	leg1->Draw("same");
	leg2->Draw("same");

	c2->cd();
	scaleByLuminosity->Draw("pe");
	leg3->Draw("same");


	c1->SetLogy();
	c2->SetLogy();
}