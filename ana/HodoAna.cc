#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sys/time.h> 

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "TF1.h"
#include "TH2F.h"
#include "TH2S.h"
#include "TH1S.h"
#include "TEnv.h"
#include "TRandom3.h"
#include "TMath.h"

#include "CommandLineInterface.hh"
#include "HodoEvent.hh"

using namespace TMath;
using namespace std;

double get_time(){  
    struct timeval t;  
    gettimeofday(&t, NULL);  
    double d = t.tv_sec + (double) t.tv_usec/1000000;  
    return d;  
}  
Double_t fstep(Double_t *x, Double_t *par){
  Float_t sqrt2 = TMath::Sqrt(2.);
  Double_t arg;
  Double_t result =par[3];
  Double_t mean  = par[0];
  Double_t sigma = par[1];
  arg = (mean-x[0])/(sqrt2*sigma);
  result += (par[2]-par[3])/pow(1+exp(sqrt2*arg),2);
  return result;
} 
int main(int argc, char* argv[]){
  double time_start = get_time();
  
  //random variables
  ifstream devrandom("/dev/random");
  UInt_t seed;
  devrandom.read((char*)(&seed), sizeof(UInt_t));
  cout << "seed " << seed << endl;
  TRandom3 *rand = new TRandom3(seed);

  char *InputFile = NULL;
  char *RootFile = NULL;
  char* setFile = NULL;
  CommandLineInterface* interface = new CommandLineInterface();
  interface->Add("-i", "input file", &InputFile);  
  interface->Add("-o", "output file", &RootFile);    
  interface->Add("-s", "settings file", &setFile);
  interface->CheckFlags(argc, argv);


  if(InputFile == NULL){
    cout << "No input file given " << endl;
    return 1;
  }
  if(RootFile == NULL){
    cout << "No output ROOT file given " << endl;
    return 2;
  }


  TFile* infile = new TFile(InputFile);
  TTree* str = (TTree*) infile->Get("str");
  if(str == NULL){
    cout << "could not find tree tr in file " << infile->GetName() << endl;
    return 3;
  }

  //read settings
  TEnv *set = new TEnv(setFile); 
  double pwidth[2][32];
  double pthresh[3][32];			
  TF1 *threshold[32];
  //creating a list of histograms
  TEnv *setthresh = new TEnv(set->GetValue("Threshold.File","/scratch/sim/wimmer/hodo/settings/thresholds_pos2.dat")); 
  TList* hlist = new TList;
  for(int j=0;j<32;j++){
    for(int k=0;k<2;k++){
      pwidth[k][j] = set->GetValue(Form("PeakWidth.P%d",k),0.0);
      //cout << pwidth[k][j] <<"\t";
    }
    pthresh[0][j] = setthresh->GetValue(Form("Threshold.Mean.%d",j),0.0);
    pthresh[1][j] = setthresh->GetValue(Form("Threshold.Width.%d",j),0.0);
    pthresh[2][j] = setthresh->GetValue(Form("Threshold.Base.%d",j),0.0);
    //cout << endl;
    threshold[j] = new TF1(Form("Threshold_%d",j),fstep,0,5000,4);
    threshold[j]->SetParameter(0,pthresh[0][j]);
    threshold[j]->SetParameter(1,pthresh[1][j]);
    threshold[j]->SetParameter(2,1);
    threshold[j]->SetParameter(3,pthresh[2][j]);
    hlist->Add(threshold[j]);
  }
  int ncuts = set->GetValue("Number.Gates",0);
  vector<double> locut;
  vector<double> upcut;
  vector<int> ctr;
  vector<TH1F*> hcoinc;
  vector<int> hit;

  HodoRaw *hodoraw = new HodoRaw;
  str->SetBranchAddress("HodoRaw",&hodoraw);

  TFile *ofile = new TFile(RootFile,"RECREATE");
  cout<<"input file: "<<InputFile<< endl;
  cout<<"writing to output file: "<<RootFile<< endl;

  //creating histograms and adding them to the list
  TH1F* hmult = new TH1F("hmult", "hmult", 32,0,32);
  hlist->Add(hmult);
  TH1F* hesim = new TH1F("hesim", "hesim", 4000,0,4000);
  hlist->Add(hesim);
  TH2F* hpossim = new TH2F("hpossim", "hpossim", 400,-400,400, 200,-200,200);
  hlist->Add(hpossim);
  TH1F* hesingles = new TH1F("hesingles", "hesingles", 4000,0,4000);
  hlist->Add(hesingles);
  TH1F* heff = new TH1F("heff", "heff", 4000,0,4000);
  hlist->Add(heff);
  TH2F* hesim_vs_edet = new TH2F("hesim_vs_edet", "hesim_vs_edet", 2000,0,4000, 2000,0,4000);
  hlist->Add(hesim_vs_edet);
  TH1F* hetot = new TH1F("hetot", "hetot", 4000,0,4000);
  hlist->Add(hetot);
  TH1F* he[32];
  for(int i=0;i<32;i++){
    he[i] = new TH1F(Form("he_%d",i),Form("he_%d",i),4000,0,4000);
    hlist->Add(he[i]);
  }
  for(int i=0;i<ncuts;i++){
    locut.push_back(set->GetValue(Form("Low.Cut.%d",i),0));
    upcut.push_back(set->GetValue(Form("High.Cut.%d",i),0));
    cout << locut.back() <<"\t"<< upcut.back() << endl;
    ctr.push_back(0);
    hcoinc.push_back(new TH1F(Form("hcoinc_%d",i),Form("hcoinc_%d",i),1000,0,4000));
    hlist->Add(hcoinc.back());
    hit.push_back(-1);
  }
  TH2F* henen = new TH2F("henen", "henen", 1000,0,4000, 1000,0,4000);
  hlist->Add(henen);
  TH2F* henen2 = new TH2F("henen2", "henen2", 1000,0,4000, 1000,0,4000);
  hlist->Add(henen2);
  TH1F* hhitpattern = new TH1F("hhitpattern", "hhitpattern", 32,0,32);
  hlist->Add(hhitpattern);
  TH1F* hhitpattern_ph = new TH1F("hhitpattern_ph", "hhitpattern_ph", 32,0,32);
  hlist->Add(hhitpattern_ph);

  TH2F* hhitpattern_2d = new TH2F("hhitpattern_2d","hhitpattern_2d",8,0,8,4,0,4);
  hlist->Add(hhitpattern_2d);
  

  cout << "------------------------------------" << endl;
  Double_t nentries = str->GetEntries();
  Int_t nbytes = 0;
  Int_t status;
  //loop over all events
  for(int i=0; i<nentries; i++){
    //get the current event and check it
    status = str->GetEvent(i);
    if(status == -1){
      cerr<<"Error occured, couldn't read entry "<<i<<" from tree "<<str->GetName()<<" in file "<<infile->GetName()<<endl;
      return 5;
    }
    else if(status == 0){
      cerr<<"Error occured, entry "<<i<<" in tree "<<str->GetName()<<" in file "<<infile->GetName()<<" doesn't exist"<<endl;
      return 6;
    }
    nbytes += status;

    //----------fill histogram, manipulate events, etc.----------

    
    for(UShort_t j=0;j<hodoraw->GetESim().size();j++)
      hesim->Fill(hodoraw->GetESim()[j]);
    

    hpossim->Fill(hodoraw->GetPosSim().X(),hodoraw->GetPosSim().Y());
    //if the total detected energy was 0, continue...
    if(hodoraw->GetETot()==0)
      continue;

    hetot->Fill(hodoraw->GetETot());
    for(int j=0;j<ncuts;j++){
      hit[j] = -1;
    }
   
    int highest=-1;
    double highesten =0;
    vector<double> energies;
    vector<Short_t> crystals;
    bool cfd = false;
    //loop over all crystals
    for(int j=0;j<32;j++){
      // only if the energy of the current crystal was >0.
      double en = hodoraw->GetE(j);
      if(en>50){
	//cout << "raw " << en << "\t";
	//cout << pwidth[0][j]*pow(en,pwidth[1][j]) << "\t";
	double sigma = rand->Gaus(0,pwidth[0][j]*pow(en,pwidth[1][j]));
	//cout << sigma << "\t";
	en+=sigma;
	double tmp = rand->Uniform(0,1);
	if(tmp<threshold[j]->Eval(en)){
	  cfd = true;
	}

	energies.push_back(en);
	crystals.push_back(j);
	//cout << "smeared " << en << endl;
      }
    }
    if(!cfd)
      continue;
    hmult->Fill(energies.size());
    for(UShort_t c=0;c<energies.size();c++){
      //cout << energies[c]; 
      if(energies[c]>highesten){
	highesten = energies[c];
	highest = c;
      }
      //cout << "\t" << highesten << endl;
      for(int j=0;j<ncuts;j++){
	//cout << j <<"\t" << ncuts << flush << "\t" << locut[j] << "\t" << upcut[j] << flush << "\t" << energies[c] << flush;
	if(energies[c]>locut[j] && energies[c]<upcut[j]){
	  //cout  << " YES" << endl;
 	  hit[j] = c;
	  ctr[j]++;
	}
//	else
//	  cout  << " NO" << endl;
      }
      he[crystals[c]]->Fill(energies[c]);
      hesingles->Fill(energies[c]);

      if(energies[c]>0.99*hodoraw->GetESim()[0]&&energies[c]<1.01*hodoraw->GetESim()[0])
	heff->Fill(energies[c]);
      hesim_vs_edet->Fill(energies[c],hodoraw->GetESim()[0]);

      hhitpattern->Fill(crystals[c]);
      
      //plot a 2dim hitpattern:
      //    0    1    2    3   |  iy/ix
      //---------------------- |-------
      //    c0   c1   c2   c3  |  0
      //    c4   c5   c6   c7  |  1
      //    c8   c9  c10  c11  |  2
      //   c12  c13  c14  c15  |  3
      //   c16  c17  c18  c19  |  4
      //   c20  c21  c22  c23  |  5
      //   c24  c25  c26  c27  |  6
      //   c28  c29  c30  c31  |  7
      
      int ix = crystals[c]/4; //integer division
      int iy = crystals[c]%4;
      hhitpattern_2d->Fill(ix,iy);
      
   
      
    }
    for(int j=0;j<ncuts;j++){
      if(hit[j]>-1){
	for(UShort_t c=0; c<energies.size();c++){
	  if(energies[c]<50)
	    continue;
	  if(c!=hit[j]){
	    hcoinc[j]->Fill(energies[c]);
	  }
	}
      }
    }
    for(UShort_t j=0;j<energies.size();j++){
      double en = energies[j];
      for(UShort_t d=j+1; d<energies.size();d++){
	double den = energies[d];
	if(den>50 && den<en)
	  henen->Fill(en,den);
	else if(den>50)
	  henen->Fill(den,en);
      }
      
    }
    
    for(UShort_t j=0;j<energies.size();j++){
      if(highest>-1 && j!=highest){
	henen2->Fill(highesten, energies[j]);
      }
    }
    
    //output/status messages every 1000 events.
    if(i%10000 == 0){
      double time_end = get_time();
      cout << setw(5) << setiosflags(ios::fixed) << setprecision(1) << (100.*i)/nentries<<" % done\t" << 
    	(Float_t)i/(time_end - time_start) << " events/s " <<
    	(nentries-i)*(time_end - time_start)/(Float_t)i << "s to go \r" << flush;
    }
  }

  
  //finish up, write and close files
  cout << endl;
  cout << "writing histograms to file" << endl;
  //ctr->Write("",TObject::kOverwrite);
  hlist->Sort();
  hlist->Write();
  cout <<  endl;
  cout << "closing file ..." << endl;
  cout << "total counts in gates: "<< endl;
  for(int j=0;j<ncuts;j++)
    cout << "gate "<<j<<":\t" << ctr[j] << endl;

  // cout<<"Size of input tree  "<<setw(16)<<gtr->GetZipBytes()<<" bytes"<<endl
  //     <<"size of calibrated tree "<<setw(16)<<ctr->GetZipBytes()<<" bytes"<<endl
  //     <<"=> size of calibrated tree is "<<setw(5)<<setiosflags(ios::fixed)<<setprecision(1)<<(100.*ctr->GetZipBytes())/gtr->GetZipBytes()<<" % of size of input tree"<<endl;

  infile->Close();
  ofile->Close();
  double time_end = get_time();
  cout << "Program Run time " << time_end - time_start << " s." << endl;
  cout << "Calculated " << nentries/(time_end - time_start) << " events/s." << endl;
  return 0;
}
