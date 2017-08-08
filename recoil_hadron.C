void recoil_hadron()
{
	
	const char *f_names[] = 
	{
		"~/test/jetty/output/recoil_hadron_5_10.root",  // 0
		"~/test/jetty/output/recoil_hadron_10_20.root", // 1
		"~/test/jetty/output/recoil_hadron_20_30.root", // 2
		"~/test/jetty/output/recoil_hadron_30_40.root", // 3
		"~/test/jetty/output/recoil_hadron_40_70.root", // 4
		// "~/test/jetty/output/recoil_hadron_50_70.root", // 5
		// "~/test/jetty/output/recoil_hadron_70_-1.root", // 6
		0
	};

	int temp = 0;
	while (f_names[temp] != 0) temp += 1;

	int nFiles = temp;
	TFile *files[nFiles];
	for (int i=0; i < nFiles; i++)
	{
		files[i] = TFile::Open(f_names[i]);
		if (files[i] == 0) std::cout << "ERROR! FILE " << i << " DIDN'T OPEN!" << std::endl;
	}

	
	// TFile *f2 = TFile::Open("~/test/jetty/output/recoil_hadron_20_70.root");
	TFile *f1 = TFile::Open("~/test/jetty/output/alice_hadron_recoil_data.root");
	f1->cd("Table 28");
	TGraph *alice_data = (TGraph*)gDirectory->Get("Graph1D_y1");

	TChain hadron_recoil_20_50("hadron_jet_20_50");
	for(int i =0;i < nFiles; i++) hadron_recoil_20_50.Add(f_names[i]);
	TChain hadrons_20_50("hadrons_20_50");
	for(int i =0;i < nFiles; i++) hadrons_20_50.Add(f_names[i]);

	TChain hadron_recoil_8_9("hadron_jet_8_9");
	for(int i =0;i < nFiles; i++) hadron_recoil_8_9.Add(f_names[i]);
	TChain hadrons_8_9("hadrons_20_50");
	for(int i =0;i < nFiles; i++) hadrons_8_9.Add(f_names[i]);

	
	// TNtuple *hadrons = (TNtuple*)f2->Get("hadrons");
	// TNtuple *hadron_recoil = (TNtuple*)f2->Get("hadron_jet");

	TH1F *jet_pt_20_50 = (TH1F*)gDirectory->Get("Hist1D_y1");
	jet_pt_20_50->SetName("jet_pt_20_50");
	jet_pt_20_50->Reset();

	TH1F *jet_pt_8_9 = (TH1F*)gDirectory->Get("Hist1D_y1");
	jet_pt_8_9->SetName("jet_pt_8_9");
	jet_pt_8_9->Reset();

	TH1F *h_alice_data = (TH1F*)gDirectory->Get("Hist1D_y1");

	TH1F *norm[nFiles];
	double sigma[nFiles];
	double weight[nFiles];
	double R = 0.5;
	double dEta_jet = 0.4;
	
	for(int i =0; i<nFiles; i++)
	{
		norm[i] = (TH1F*)files[i]->Get("norm");
		sigma[i] = norm[i]->GetBinContent(1);
		weight[i] = norm[i]->GetBinContent(2);
	}

	TCanvas *tmp = new TCanvas();
	hadron_recoil_20_50.Draw("pt>>jet_pt_20_50");
	hadron_recoil_8_9.Draw("pt>>jet_pt_8_9");

	TH1F *events_20_50 = new TH1F("events_20_50", "", 50, 0, 80);
	TH1F *events_8_9 = new TH1F("events_8_9", "", 50, 0, 80);
	hadrons_20_50.Draw("pt>>events_20_50");
	hadrons_8_9.Draw("pt>>events_8_9");
	int n_trig_20_50 = events_20_50->GetEntries();
	int n_trig_8_9 = events_8_9->GetEntries();
	std::cout << "[i] size: " << n_trig_20_50 << std::endl;
	tmp->Close();


	// Scale 
	// The normalization for the jet is 1/N_trig * d2N/(dpt*deata_jet) (per trigger yield)
	jet_pt_20_50->Scale( 1 / (2*dEta_jet * jet_pt_20_50->GetBinWidth(1) * n_trig_20_50  ) );
	jet_pt_8_9->Scale( 1 / (2*dEta_jet * jet_pt_8_9->GetBinWidth(1) * n_trig_8_9  ) );

	TH1F *delta_recoil = (TH1F*)jet_pt_20_50->Clone("delta_recoil");
	delta_recoil->Add(jet_pt_8_9, -1);
	TH1F *ratio = (TH1F*)delta_recoil->Clone("ratio");
	ratio->Divide(h_alice_data);
	for(int i=1; i <= ratio->GetNbinsX(); i++) ratio->SetBinError(i, 0);

	TCanvas *c1 = new TCanvas("yield");
	TCanvas *c2 = new TCanvas("yeild_ratio");

	delta_recoil->SetLineColor(kRed);
	delta_recoil->SetMarkerColor(kRed);
	delta_recoil->SetMarkerStyle(24);
	delta_recoil->SetLineWidth(2);
	delta_recoil->SetYTitle("1/N_{trig} d^{2}N_{jet}/ (d#eta_{jet}dp_{T}) [GeV^{-1}]");
	delta_recoil->SetXTitle("p_{T} [GeV]");
	delta_recoil->SetTitle("#Delta_{recoil} = T{20,50} - T{8,9}");

	ratio->SetLineColor(kRed);
	ratio->SetMarkerColor(kRed);
	ratio->SetMarkerStyle(24);
	ratio->SetLineWidth(2);
	ratio->SetTitle("PYTHIA / ALICE");
	ratio->SetYTitle("Ratio");

	TLegend *yield_info = new TLegend();
	yield_info->AddEntry((TObject*)0, "| #Delta#phi | > #pi - 0.6" , " ");
	yield_info->AddEntry((TObject*)0, "Anti-kt   R = 0.5 " , " ");
	yield_info->AddEntry((TObject*)0, "| eta_{jet} | < 0.4" , " ");
	yield_info->AddEntry((TObject*)0, " pp  #sqrt{s} = 7 TeV" , " ");

	TLegend *dif_plot = new TLegend();
	dif_plot->AddEntry(delta_recoil, "PYTHIA", "pl");
	dif_plot->AddEntry(alice_data, "Alice", "pl");

	c1->cd();
	delta_recoil->Draw("pe  ");
	alice_data->Draw("same");
	yield_info->Draw("same");
	dif_plot->Draw("same");

	c2->cd();
	ratio->Draw("p");

	c1->SetLogy();
}