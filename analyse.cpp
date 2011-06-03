#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>

#include "SkipList.hpp"
#include "BloomFilter.hpp"
#include "RBST.hpp"

#include <sys/time.h>
#include <stdlib.h>
int gettimeofday(struct timeval * tv, struct timezone *tz);

using namespace std;

const unsigned int MAX_WORDS = 98569;
const unsigned int NUM_BINS = 100;
const unsigned int BUFFER_SIZE = 256;

const string psColor = "set terminal postscript eps color; ";
const string setxlabel = " set xlabel \"Set Size\";";
const string setylabelCount =  "set ylabel \"Counter Value\";";
const string setyLabelMean = "set ylabel \"Mean Execution Time (in milliseconds)\"; ";
const string countL = " \"counter.dat\" using 1:";
const string timeL  = " \"timer.dat\" using 1:";
const string lSkip = " with line title \"Skip List\"";
const string lBloom = " with line title \"Bloom Filter\"";
const string lRBST = " with line title \"RBST\"";
const string echoAddcount ="echo 'set output \"addcounter.ps\";";
const string echoAddterm ="echo 'set output \"addtimer.ps\";";
const string delcount = "echo 'set output \"delcounter.ps\"; ";
const string deltime = "echo 'set output \"deltimer.ps\"; ";
const string echoFind = "echo 'set output \"findcounter.ps\"; ";
const string echoFindTime = "echo 'set output \"findtimer.ps\"; ";
     
unsigned long long microseconds() {
  struct timeval tv;
  struct timezone tz;
  gettimeofday(&tv, &tz);
  return tv.tv_sec*1000000 + tv.tv_usec;
}

template <class type>
std::string stringFrom (type i) {
  std::stringstream tmp;
  tmp<<i;
  return tmp.str();
}

double outlierDeletedMean(vector<unsigned long long> samples) {


  long double sampleMean = 0;
  for (unsigned int i=0; i< samples.size(); i++) {
    sampleMean += samples[i];
  }
  sampleMean /= samples.size();

  long double sampleBound = 0;
  for (unsigned int i=0; i< samples.size(); i++) {
    sampleBound += ((long double)(samples[i]) - sampleMean)*
                   ((long double)(samples[i]) - sampleMean);
  }
  sampleBound = 3*sampleBound/samples.size();

  long double  sample = 0;
  unsigned int sampleCount=0;

  for (unsigned int i=0; i <samples.size() ; i++) {
    if (((long double)(samples[i]) - sampleMean)*((long double)(samples[i]) -
            sampleMean) <  sampleBound) {
      sampleCount++;
      sample += samples[i];
    }
  }
  return sample/sampleCount;
}

int loadDictionary(char *name, vector<string> &words) {
  char buffer[BUFFER_SIZE];
  unsigned int i;
  ifstream wordFile(name);
  if (!wordFile.is_open()) {
    cout<<"Can't find the file "<<name<<endl;
    exit(1);
  }
  cout<<"Loading dictionary";
  for (i=0; !wordFile.eof() && i <MAX_WORDS ; i++) {
    if (i%(MAX_WORDS/100) == 0 ) {
      cout<<".";
    }
    cout.flush();
    wordFile.getline(buffer,BUFFER_SIZE);
    words[i] = buffer;
  }
  cout<<endl<<"Done."<<endl;
  return i;
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    cout<<"Program requires a dictionary file ";
    cout<<" and a string specify the container to test"<<endl;
    cout<<"Syntax: analyse dictionary [b][s][r]"<<endl;
    cout<<"flags: "<<endl<<"b: bloom filter."<<endl<<"s: skip list."<<endl
    <<"r randomized binary search tree."<<endl;
    return 0;
  }
  
  string gnuPSPlotCount = psColor +"set title \"Sample Counter\";"
                          +setxlabel+setylabelCount+"plot";
  string gnuPSPlotTime = psColor + "set title \"Sample Timer\";"
                         +setyLabelMean+ setxlabel+" plot";

  string gnuScreenPlotCount = "echo 'set title \"Counter\"; "+
                               setylabelCount+ setxlabel+ "plot";
  string gnuScreenPlotTime = "echo 'set title \"Timer\";"+
                              setyLabelMean+setxlabel+"plot";


  bool plotBF=false, plotRBST=false, plotList=false;
  for (unsigned int i=0; i<3 && argv[2][i] != '\0'; i++) {
    switch (argv[2][i]) {
    case 'b':
      plotBF = true;
      break;
    case 's':
      plotList = true;
      break;
    case 'r':
      plotRBST = true;
      break;
    default:
      cout<<"Error in the flags."<<argv[2][i]<<endl;
      return 0;
    }
  }

  

  int columnCount=2;
  if (plotList) {
    if (columnCount >2) {
      gnuPSPlotCount+=',';
    }
    gnuPSPlotCount +=  countL +stringFrom<int>(columnCount)+ lSkip;

    if (columnCount >2) {
      gnuPSPlotTime+=',';
    }
    gnuPSPlotTime += timeL + stringFrom<int>(columnCount)+lSkip;
                     
    if (columnCount >2) {
      gnuScreenPlotCount+=',';
    }
    gnuScreenPlotCount += countL +stringFrom<int>(columnCount)+lSkip;
    if (columnCount >2) {
      gnuScreenPlotTime+=',';
      gnuScreenPlotTime += timeL + stringFrom<int>(columnCount)+lSkip;
    }
    columnCount++;
  }
  if (plotBF) {
    if (columnCount >2) {
      gnuPSPlotCount+=',';
    }
    gnuPSPlotCount += countL + stringFrom<int>(columnCount)+ lBloom;
    if (columnCount >2) {
      gnuPSPlotTime+=',';
    }
    gnuPSPlotTime += timeL + stringFrom<int>(columnCount)+ lBloom;
    if (columnCount >2) {
      gnuScreenPlotCount+=',';
    }
    gnuScreenPlotCount += countL + stringFrom<int>(columnCount)+ lBloom;
    if (columnCount >2) {
      gnuScreenPlotTime+=',';
    }
    gnuScreenPlotTime += timeL + stringFrom<int>(columnCount)+lBloom;
    columnCount++;
  }
  if (plotRBST) {
    if (columnCount >2) {
      gnuPSPlotCount+=',';
    }
    gnuPSPlotCount += countL + stringFrom<int>(columnCount)+lRBST;
    if (columnCount >2) {
      gnuPSPlotTime+=',';
    }
    gnuPSPlotTime += timeL + stringFrom<int>(columnCount)+lRBST;
    if (columnCount >2) {
      gnuScreenPlotCount+=',';
    }
    gnuScreenPlotCount += countL + stringFrom<int>(columnCount)+lRBST;
    if (columnCount >2) {
      gnuScreenPlotTime+=',';
    }
    gnuScreenPlotTime += timeL + stringFrom<int>(columnCount)+lRBST;
    columnCount++;
  }

  gnuPSPlotCount += "' | gnuplot";
  gnuPSPlotTime += "' | gnuplot";
  gnuScreenPlotCount += "' | gnuplot -persist";
  gnuScreenPlotTime += "' | gnuplot -persist";


  vector<string> word(MAX_WORDS);

  const unsigned int TOTAL_WORDS = loadDictionary(argv[1],word);
  srand(0);

  unsigned int NUM_TRIALS;
  cout<<"How many repeats for each test? ";
  cin >>NUM_TRIALS;

  cout<<"Initializing test container" << endl;

  vector<SkipList*> testList(NUM_TRIALS);
  vector<BloomFilter*>  testBF(NUM_TRIALS);
  vector<RBST*>  testRBST(NUM_TRIALS);

  unsigned int testNum =0;

  string command = "";
  while (command != "q" && command != "quit") {
    cout<<"What would you like to do? (add/delete/find/quit) ";
    cin>>command;

    bool error = false;
    ofstream counteroutput("counter.dat");
    ofstream timeroutput("timer.dat");

    if (command == "a" || command == "add") {
      cout << "How many words to add? ";
      cin >> testNum;
      cout <<endl;

      cout<<"Refreshing memory";
      for (unsigned int t=0; t <NUM_TRIALS ; t++) {
        if (NUM_TRIALS < 100 || t%(NUM_TRIALS/100) == 0 ) {
          cout<<".";
          cout.flush();
        }
        if (testList[t]) {
          delete testList[t];
        }
        if (testBF[t]) {
          delete testBF[t];
        }
        if (testRBST[t]) {
          delete testRBST[t];
        }

        testList[t] = new SkipList(int(log(MAX_WORDS)));
        testBF[t] = new BloomFilter(int(MAX_WORDS));
        testRBST[t] = new RBST();
      }
      cout<<endl;


      cout<<"Start Testing";
      int binSize = 1;
      vector<unsigned long long>  testListTimes(NUM_TRIALS,0);
      vector<unsigned long long>  testBFTimes(NUM_TRIALS,0);
      vector<unsigned long long>  testRBSTTimes(NUM_TRIALS,0);
      for (unsigned int i =0; i <testNum ; i++) {
        if (testNum <= 100 || i%(testNum/100) == 0 ) {
          cout<<".";
          cout.flush();
        }

        double testListCount = 0;
        double testBFCount = 0;
        double testRBSTCount = 0;

        for (unsigned int t=0; t <NUM_TRIALS ; t++) {
          int index = rand()%TOTAL_WORDS;
          testListTimes[t] -= microseconds();
          testList[t]->add(word[index]);
          testListTimes[t] += microseconds();
          testBFTimes[t] -= microseconds();
          testBF[t]->add(word[index]);
          testBFTimes[t] += microseconds();
          testRBSTTimes[t] -= microseconds();
          testRBST[t]->add(word[index]);
          testRBSTTimes[t] += microseconds();

          testListCount += testList[t]->getCountAdd()+testList[t]->getCountFind();
          testBFCount += testBF[t]->getCountAdd() +testBF[t]->getCountFind();
          testRBSTCount += testRBST[t]->getCountAdd()+testRBST[t]->getCountFind();
        }

        if (testNum <= NUM_BINS || i%(testNum/NUM_BINS) == 0 ) {
          if (plotList) {
            counteroutput << i <<" "<< testListCount/NUM_TRIALS/binSize << endl;
            timeroutput<< i <<" "<< outlierDeletedMean(testListTimes)/binSize << endl ;
          }
          if (plotBF) {
            counteroutput << i <<" "<< testBFCount/NUM_TRIALS/binSize << endl;
            timeroutput << i << " "<< outlierDeletedMean(testBFTimes)/binSize << endl;
          }
          if (plotRBST) {
            counteroutput << i << " "<< testRBSTCount/NUM_TRIALS/binSize << endl;
           timeroutput<< i << " "<< outlierDeletedMean(testRBSTTimes)/binSize << endl;
           }
  
  
          binSize = 1;

          for (unsigned int t=0; t <NUM_TRIALS ; t++) {
            testList[t]->resetCountAdd();
            testList[t]->resetCountFind();
            testBF[t]->resetCountAdd();
            testBF[t]->resetCountFind();
            testRBST[t]->resetCountAdd();
            testRBST[t]->resetCountFind();
          }
          testListTimes.assign(NUM_TRIALS,0);
          testBFTimes.assign(NUM_TRIALS,0);
          testRBSTTimes.assign(NUM_TRIALS,0);

        } else {
          binSize++;
        }
      }
      cout<<endl;
      system ((echoAddcount + gnuPSPlotCount).c_str());
      system ((echoAddterm + gnuPSPlotTime).c_str());
    }
    else if (command == "d" || command == "del"|| command == "delete") {
      cout << "How many entries to start with?";
      if (testNum > TOTAL_WORDS) {
        testNum = TOTAL_WORDS;
      }
      cin >> testNum;
      cout <<endl;

      cout<<"Refreshing memory";
      for (unsigned int t=0; t <NUM_TRIALS ; t++) {
        if (testNum <= 100 || t%(testNum/100) == 0 ) {
          cout<<".";
          cout.flush();
        }
        if (testList[t]) {
          delete testList[t];
        }
        if (testBF[t]) {
          delete testBF[t];
        }
        if (testRBST[t]) {
          delete testRBST[t];
        }

        testList[t] = new SkipList(int(log(MAX_WORDS)));
        testBF[t] = new BloomFilter(int(MAX_WORDS));
        testRBST[t] = new RBST();
      }
      cout<<endl;

      cout<<"Adding random words";
      vector<string> addedWords(testNum);
      for (unsigned int i=0; i <testNum ; i++) {
        if (testNum <= 100 || i%(testNum/100) == 0 ) {
          cout<<".";
          cout.flush();
        }

        int index = rand()%TOTAL_WORDS;

        int ret = 0;
        while (!ret) {
          index = rand()%TOTAL_WORDS;
          for (unsigned int t=0; t <NUM_TRIALS ; t++) {
            ret = testList[t]->add(word[index]);
            if (ret == 0) {
              break;
            }
            testBF[t]->add(word[index]);
            testRBST[t]->add(word[index]);
          }
        }
        addedWords[i] = word[index];

      }
      cout<<endl;

      cout<<"Start Deleting from the set"<<endl;
      int binSize = 1;
      vector<unsigned long long>  testListTimes(NUM_TRIALS,0);
      vector<unsigned long long>  testBFTimes(NUM_TRIALS,0);
      vector<unsigned long long>  testRBSTTimes(NUM_TRIALS,0);

      for (unsigned int i=0; i <testNum ; i++) {
        if (testNum <= 100 || i%(testNum/100) == 0 ) {
          cout<<".";
          cout.flush();
        }
        double testListCount = 0;
        double testBFCount = 0;
        double testRBSTCount = 0;

        int index = rand()% addedWords.size();
        for (unsigned int t=0; t <NUM_TRIALS ; t++) {
          testListTimes[t] -= microseconds();
          testList[t]->del(addedWords[index]);
          testListTimes[t] += microseconds();
          testBFTimes[t] -= microseconds();
          testBF[t]->del(addedWords[index]);
          testBFTimes[t] += microseconds();
          testRBSTTimes[t] -= microseconds();
          testRBST[t]->del(addedWords[index]);
          testRBSTTimes[t] += microseconds();

          testListCount += testList[t]->getCountDelete();
          testBFCount += testBF[t]->getCountDelete();
          testRBSTCount += testRBST[t]->getCountDelete();
        }
        addedWords.erase(addedWords.begin()+ index);

        if (testNum <= NUM_BINS || i%(testNum/NUM_BINS) == 0 ) {
           int counter =testNum - i-1;
           if (plotList) {
            counteroutput << counter <<" "<< testListCount/NUM_TRIALS/binSize << endl;
            timeroutput << counter <<" "<< outlierDeletedMean(testListTimes)/binSize <<endl;
           }
          if (plotBF) {
            counteroutput<< counter << " "<< testBFCount/NUM_TRIALS/binSize <<endl;
            timeroutput  << counter << " "<< outlierDeletedMean(testBFTimes)/binSize <<endl;
          }
          if (plotRBST) {
            counteroutput << counter << " "<< testRBSTCount/NUM_TRIALS/binSize <<endl;
            timeroutput <<  counter << " "<< outlierDeletedMean(testRBSTTimes)/binSize <<endl;
          }
 
          binSize = 1;

          for (unsigned int t=0; t <NUM_TRIALS ; t++) {
            testList[t]->resetCountDelete();
            testBF[t]->resetCountDelete();
            testRBST[t]->resetCountDelete();
          }

          testListTimes.assign(NUM_TRIALS,0);
          testBFTimes.assign(NUM_TRIALS,0);
          testRBSTTimes.assign(NUM_TRIALS,0);

        } else {
          binSize++;
        }
      }
      cout<<endl;
      system ((delcount + gnuPSPlotCount).c_str());
      system ((deltime + gnuPSPlotTime).c_str());

    } else if (command == "f" || command == "find") {
      cout << "Up to how many words in the dictionary? ";
      cin >> testNum;
      if (testNum > TOTAL_WORDS) {
        testNum = TOTAL_WORDS;
      }
      cout <<endl;

      cout<<"Refreshing memory";
      for (unsigned int t=0; t <NUM_TRIALS ; t++) {
        if (NUM_TRIALS < 100 || t%(NUM_TRIALS/100) == 0 ) {
          cout<<".";
          cout.flush();
        }
        if (testList[t]) {
          delete testList[t];
        }
        if (testBF[t]) {
          delete testBF[t];
        }
        if (testRBST[t]) {
          delete testRBST[t];
        }

        testList[t] = new SkipList(int(log(MAX_WORDS)));
        testBF[t] = new BloomFilter(int(MAX_WORDS));
        testRBST[t] = new RBST();
      }
      cout<<endl;

      int binSize = 1;

      vector<unsigned long long>  testListTimes(NUM_TRIALS,0);
      vector<unsigned long long>  testBFTimes(NUM_TRIALS,0);
      vector<unsigned long long>  testRBSTTimes(NUM_TRIALS,0);

      for(unsigned int i=0; i <testNum ; i++) {
        if (testNum <= 100 || i%(testNum/100) == 0 ) {
          cout<<".";
          cout.flush();
        }

        double testListCount = 0;
        double testBFCount = 0;
        double testRBSTCount = 0;

        for (unsigned int t=0; t <NUM_TRIALS ; t++) {
          int index = rand()%TOTAL_WORDS;
          int ret = 0;
          while (!ret) {
            index = rand()%TOTAL_WORDS;
            ret = testList[t]->add(word[index]);
            testBF[t]->add(word[index]);
            testRBST[t]->add(word[index]);
          }

          index = rand()%TOTAL_WORDS;
          testListTimes[t] -= microseconds();
          testList[t]->find(word[index]);
          testListTimes[t] += microseconds();
          testBFTimes[t] -= microseconds();
          testBF[t]->testExist(word[index]);
          testBFTimes[t] += microseconds();
          testRBSTTimes[t] -= microseconds();
          testRBST[t]->find(word[index]);
          testRBSTTimes[t] += microseconds();

          testListCount += testList[t]->getCountFind();
          testBFCount += testBF[t]->getCountFind();
          testRBSTCount += testRBST[t]->getCountFind();

        }

        if (testNum <= NUM_BINS || i%(testNum/NUM_BINS) == 0 ) {
          if (plotList) {
            counteroutput<< i <<" "<< testListCount/NUM_TRIALS/binSize << endl;
            timeroutput << i<<" "<< outlierDeletedMean(testListTimes)/binSize << endl;
         }
          if (plotBF) {
            counteroutput<< i << " "<< testBFCount/NUM_TRIALS/binSize << endl;
            timeroutput<< i << " "<< outlierDeletedMean(testBFTimes)/binSize << endl;
         }
          if (plotRBST) {
            counteroutput<< i<< " "<< testRBSTCount/NUM_TRIALS/binSize << endl;
            timeroutput<< i<< " "<< outlierDeletedMean(testRBSTTimes)/binSize << endl;
          }
   
  
          binSize = 1;

          for (unsigned int t=0; t <NUM_TRIALS ; t++) {
            testList[t]->resetCountFind();
            testBF[t]->resetCountFind();
            testRBST[t]->resetCountFind();
          }

          testListTimes.assign(NUM_TRIALS,0);
          testBFTimes.assign(NUM_TRIALS,0);
          testRBSTTimes.assign(NUM_TRIALS,0);

        } else {
          binSize++;
        }
      }
      cout<<endl;
        
      system ((echoFind + gnuPSPlotCount).c_str());
      system ((echoFindTime + gnuPSPlotTime).c_str());

    } else {
      if (command == "q" || command == "quit") {
        break;
      } else {
        cout<<"Wrong command!"<<endl;
        error = true;
      }
    }
    if (!error) {
      system (gnuScreenPlotCount.c_str());
      system (gnuScreenPlotTime.c_str());
    }
    timeroutput.close();
    counteroutput.close();
  }
  cout<<"Freeing memory";
  for (unsigned t=0; t <NUM_TRIALS ; t++) {
    if (NUM_TRIALS < 100 || t%(NUM_TRIALS/100) == 0 ) {
      cout<<".";
      cout.flush();
    }
    delete testList[t];
    delete testBF[t];
    delete testRBST[t];
  }
  cout<<endl;
  cout<<"done."<<endl;
  return 1;
}

