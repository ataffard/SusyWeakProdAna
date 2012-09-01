{

  TFile *_file0 = TFile::Open("../../histoAna/SusyAna/histo_ttbar.105200.root");
  
  TCanvas c1("SRjveto","SRjveto");
  c1.Divide(3,2);
  c1.cd(1);
  DG2L_SRjveto_EE_DG2L_orgl1->Draw("text");gPad->SetLogy();
  c1.cd(2);
  DG2L_SRjveto_MM_DG2L_orgl1->Draw("text");gPad->SetLogy();
  c1.cd(3);
  DG2L_SRjveto_EM_DG2L_orgl1->Draw("text");gPad->SetLogy();
  c1.cd(4);
  DG2L_SRjveto_EE_DG2L_orgl2->Draw("text");gPad->SetLogy();
  c1.cd(5);
  DG2L_SRjveto_MM_DG2L_orgl2->Draw("text");gPad->SetLogy();
  c1.cd(6);
  DG2L_SRjveto_EM_DG2L_orgl2->Draw("text");gPad->SetLogy();
  c1->Print("../../histoAna/SusyAna/dummyPlots/loose_wMet_SRjveto.png");
  
  TCanvas c2("SRSSjveto","SRSSjveto");
  c2.Divide(3,2);
  c2.cd(1);
  DG2L_SRSSjveto_EE_DG2L_orgl1->Draw("text");gPad->SetLogy();
  c2.cd(2);
  DG2L_SRSSjveto_MM_DG2L_orgl1->Draw("text");gPad->SetLogy();
  c2.cd(3);
  DG2L_SRSSjveto_EM_DG2L_orgl1->Draw("text");gPad->SetLogy();
  c2.cd(4);
  DG2L_SRSSjveto_EE_DG2L_orgl2->Draw("text");gPad->SetLogy();
  c2.cd(5);
  DG2L_SRSSjveto_MM_DG2L_orgl2->Draw("text");gPad->SetLogy();
  c2.cd(6);
  DG2L_SRSSjveto_EM_DG2L_orgl2->Draw("text");gPad->SetLogy();
  c2->Print("../../histoAna/SusyAna/dummyPlots/loose_wMet_SRSSjveto.png");

  TCanvas c3("SR2jet","SR2jet");
  c3.Divide(3,2);
  c3.cd(1);
  DG2L_SR2jet_EE_DG2L_orgl1->Draw("text");gPad->SetLogy();
  c3.cd(2);
  DG2L_SR2jet_MM_DG2L_orgl1->Draw("text");gPad->SetLogy();
  c3.cd(3);
  DG2L_SR2jet_EM_DG2L_orgl1->Draw("text");gPad->SetLogy();
  c3.cd(4);
  DG2L_SR2jet_EE_DG2L_orgl2->Draw("text");gPad->SetLogy();
  c3.cd(5);
  DG2L_SR2jet_MM_DG2L_orgl2->Draw("text");gPad->SetLogy();
  c3.cd(6);
  DG2L_SR2jet_EM_DG2L_orgl2->Draw("text");gPad->SetLogy();
  c3->Print("../../histoAna/SusyAna/dummyPlots/loose_wMet_SR2jet.png");


  TCanvas c4("SRmT2","SRmT2");
  c4.Divide(3,2);
  c4.cd(1);
  DG2L_SRmT2_EE_DG2L_orgl1->Draw("text");gPad->SetLogy();
  c4.cd(2);
  DG2L_SRmT2_MM_DG2L_orgl1->Draw("text");gPad->SetLogy();
  c4.cd(3);
  DG2L_SRmT2_EM_DG2L_orgl1->Draw("text");gPad->SetLogy();
  c4.cd(4);
  DG2L_SRmT2_EE_DG2L_orgl2->Draw("text");gPad->SetLogy();
  c4.cd(5);
  DG2L_SRmT2_MM_DG2L_orgl2->Draw("text");gPad->SetLogy();
  c4.cd(6);
  DG2L_SRmT2_EM_DG2L_orgl2->Draw("text");gPad->SetLogy();
  c4->Print("../../histoAna/SusyAna/dummyPlots/loose_wMet_SRmT2.png");



}
