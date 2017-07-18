void dumby()
{
	TH2F *hist = new TH2F("hist", "Radom values; value1;value2", 100, 0, 1, 100, 0, 1);
	TNtuple *tuple = new TNtuple("tuple","random", "ran1:ran2");

	TRandom *test1 = new TRandom();
	TRandom *test2 = new TRandom();
	for(int i =0; i < 100000; i++)	
	{
		double value1 = test1->Rndm();
		double value2 = test2->Rndm();

		hist->Fill(value1, value2);
		tuple->Fill(value1, value2);
	}

	TCanvas *c1 = new TCanvas();
	TCanvas *c2 = new TCanvas();
	
	TH1D *hist2 = hist->ProjectionY("ProjectionY", 1, 4);
	TH1F *tuple_hist = new TH1F("tuple_hist", "tuple_hist; value2; proj",100,0,1);

	c1->cd();
	tuple->Draw("ran2>>tuple_hist", "ran2 < 0.04", "");

	c2->cd();
	hist2->Draw("hist");
	// tuple_hist->SetLineColor(kRed);
	// tuple_hist->Draw("same hist");



}