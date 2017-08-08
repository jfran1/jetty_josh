void gamma_pete_jet()
{
	TFile * f1 = TFile::Open("/Users/joshfrandsen/test/jetty/output/gamma_jet_20_30.root");

	// double bins[] = {0, 10, 20, 30, 50, 75, 100, 125, 150, 175, 200, 250, 300, 350, 400, 500};

	TNtuple *jet = (TNtuple*)f1->Get("jet");
	TNtuple *photon = (TNtuple*)f1->Get("photon");
	TH1F *jet_pt = new TH1F("jet_pt", " Recoil gamma+jet; p_{T}; counts", 50, 0, 50);
	TH1F *norm = (TH1F*)f1->Get("norm");

	double sigma = norm->GetBinContent(1);
	double weight = norm->GetBinContent(2);
	double R = .3;
	double dEta_jet = 1-R;
	double d_dphi = 3*TMath::Pi()/4;
	double dEta_photon = 1.;

	TCanvas *tmp = new TCanvas();
	jet->Draw("pt>>jet_pt");

	TH1F *events = new TH1F("events", "", 50, 0, 50);
	photon->Draw("pt>>events");
	int n_trig = events->GetEntries();
	std::cout << "[i] size: " << n_trig << std::endl;
	// jet->Draw("dphi");
	tmp->Close();

	TH1F *photon_pt = (TH1F*)events->Clone("photon_pt");
	photon_pt->Scale( 1 / (2*dEta_photon * photon_pt->GetBinWidth(1) * weight));

	// Scale 
	// The normalization for the jet is 1/N_trig * d2N/(dpt*deata_jet) (per trigger yield)
	jet_pt->Scale( 1 / ( 2*dEta_jet * jet_pt->GetBinWidth(1) * n_trig * 2*d_dphi ) );
	TH1F *cross = (TH1F*)jet_pt->Clone("cross_section");
	TH1F *ratio =(TH1F*)photon_pt->Clone("ratio");
	cross->Scale(sigma);

	TCanvas *c1 = new TCanvas("yield");
	TCanvas *c2 = new TCanvas("cross_section");

	jet_pt->SetLineColor(kRed);
	jet_pt->SetMarkerColor(kRed);
	jet_pt->SetMarkerStyle(24);
	jet_pt->SetLineWidth(2);
	jet_pt->SetYTitle("1/N_{trig} d^{2}N_{jet}/ (d#eta_{jet}dp_{T}d#Delta#phi) [GeV^{-1}]");
	jet_pt->SetXTitle("p_{T} [GeV]");

	cross->SetLineColor(kRed);
	cross->SetMarkerColor(kRed);
	cross->SetMarkerStyle(24);
	cross->SetLineWidth(2);

	photon_pt->SetLineColor(kBlack);
	photon_pt->SetMarkerColor(kBlack);
	photon_pt->SetMarkerStyle(24);
	photon_pt->SetLineWidth(2);



	TLegend *yield_info = new TLegend();
	yield_info->AddEntry((TObject*)0, "| #Delta#phi | > 3#pi/4" , " ");
	yield_info->AddEntry((TObject*)0, "Anti-kt   R = 0.3 " , " ");
	yield_info->AddEntry((TObject*)0, "| #eta | < 1   | eta_{jet} | < 1-R" , " ");
	yield_info->AddEntry((TObject*)0, " pp  #sqrt{s} = 5 TeV" , " ");
	yield_info->AddEntry((TObject*)0, "20 GeV < p_{T}^{#gamma} < 30 GeV" , " ");

	TLegend *dif_plot = new TLegend();
	dif_plot->AddEntry(jet_pt, "Jet Yeild", "pe");
	dif_plot->AddEntry(photon_pt, "Photon yeild", "pe");

	c1->cd();
	jet_pt->Draw("pe ");
	photon_pt->Draw("same");
	yield_info->Draw("same");
	dif_plot->Draw("same");

	c2->cd();
	ratio->Divide(jet_pt);

	c1->SetLogy();
	c2->SetLogy();
}