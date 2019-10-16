#include <iostream>
#include <stdio.h>
#include <fstream>
#include <math.h>

#define P_in_length 10      // the size of the error pattern vector
#define Pran_num 5          // the number of range for each error pattern
//#define Stop_time 1*5*60    // hour * miniute * second

using namespace std;

int main(int argc, char *argv[]){

  int Length_D = atoi(argv[1]); // number of data bits
  int Length_R = atoi(argv[2]); // number of parity bits
  int p_num = atoi(argv[3]);    // number of the error pattern correction
  int pd_num = atoi(argv[4]);    // number of the error pattern detection
  int Stop_time = atoi(argv[5]);    // stopping time
  int Mode = atoi(argv[6]);    // stopping time 0 is correction ; 1 is correction+detection

  void swap(int *a, int *b);
  int Counter(int a, int row_number);

  ofstream fout;
  ifstream fin;

  fout.open("Check.C");
  fin.open("PSet.txt");

  unsigned char tmp[30];
  int cnt=100;

  if(Mode == 0){
    int Length_Code = Length_D + Length_R;

    int *p_size = new int[p_num];

    int **p = new int*[p_num];  // error_pattern
    for(int i=0;i<p_num;i++) {p[i] = new int[P_in_length];}

    int **p_range = new int*[p_num];   // error pattern range
    for(int i=0;i<p_num;i++) p_range[i] = new int[2*Pran_num];

    int **en_p = new int*[p_num];    // error_pattern enable
    for(int i=0;i<p_num;i++) en_p[i] = new int[Length_D+Length_R];

    for(int i=0;i<p_num;i++){
      for(int j=0;j<Length_D+Length_R;j++){
        *(*(en_p+i)+j) = 0;
      }
    }

    int *parity = new int[Length_R];  // parity_bit value
    for(int i=0;i<Length_R;i++){
      *(parity+i) = 0;
    }

    int *pb_position = new int[Length_R];  //parity_bit position
    for(int i=0;i<Length_R;i++){
      *(pb_position+i) = 0;
    }

    cout<<"********************************************"<<endl;
    cout<<"               Check_Program                "<<endl;
    cout<<"********************************************"<<endl;

    while(cnt){
      fin>>tmp;  // read ****************** line
      if (!fin.good()) break;
    // read data
      if(tmp[0]=='*')
      cout<<"             Read_Data_Successful         "<<endl;
      else
      {cout<<"            Read_Data_Failure            "<<endl; break;}
      cout<<"********************************************"<<endl;
    // start to read Error_Pattern
      for (int i=0;i<p_num;i++){
        for(int j=0;j<P_in_length;j++){
          fin>> *(*(p+i)+j);
        }
      }
      fin>>tmp;  // read ****************** line
    // start to read Error_Pattern_Range
      for (int i=0;i<p_num;i++){
        for(int j=0;j<2*Pran_num;j++){
          fin>> *(*(p_range+i)+j);
        }
      }
      fin>>tmp;  // read ****************** line
    // start to read Parity_Bit_Position
      for (int i=0;i<Length_R;i++){
        fin>> *(pb_position+i);
      }
    }// while{}end

    // --------------------------------
    // Obtain the Error_Pattern size by using the  error_pattern
    for (int i=0;i<p_num;i++){
      for(int j=(P_in_length-1);j>=0;j--){
        if (*(*(p+i)+j) == 1){
          *(p_size+i) = j+1;
          break;
        }
      }
    }

    // --------------------------------
    // 5 groups of the range to the error pattern enable
    for(int i=0;i<p_num;i++){
      for(int j=0;j<Pran_num;j++){
        if(*(*(p_range+i)+2*j) != 0){
          for(int k=(*(*(p_range+i)+2*j)-1 + *(p_size+i)-1); k<*(*(p_range+i)+2*j+1); k++)
            *(*(en_p+i)+k) = 1;
          }
        if(*(*(p_range+i)+2*j) == 0) continue;
       }
      }

    // --------------------------------
    // set the range of the circle ( the element ordered by the weight from low to high)
    int array_size = pow(2,Length_R)-1;
    int *array = new int[array_size];
    for(int i=0;i<array_size;i++){
      array[i]=i+1;
    }
    int key;
    for (int i = 0; i < array_size; i++){
        key=0;
        for (int j = 0; j+1<array_size-i; j++){
            if (Counter(*(array+j),Length_R) > Counter(*(array+j+1),Length_R)){
                key=1;
                swap((array+j), (array+j+1));
            }
        }
        if (key==0) {
            break;
        }
    }

  // --------------------------------
    cout<<">>> Error Pattern Range : "<<endl;
    for(int i=0;i<p_num;i++){
      for(int j=0;j<2*Pran_num;j++){
        cout<< *(*(p_range+i)+j) <<" ";
      }
      cout<<"\n";
    }
    cout<<"********************************************"<<endl;
    cout<<">>> Error Pattern Enable : "<<endl;
    for(int i=0;i<p_num;i++){
      for(int j=0;j<Length_D+Length_R;j++){
        cout<< *(*(en_p+i)+j) <<" ";
      }
      cout<<"\n";
    }

    // definition
    fout<<"#include <iostream>"<<endl;
    fout<<"#include <stdio.h>"<<endl;
    fout<<"#include <fstream>"<<endl;
    fout<<"#include <math.h>"<<endl;
    fout<<"#include <stdlib.h>"<<endl;
    fout<<"#include <time.h>"<<endl;
    fout<<"using namespace std;"<<endl;
    fout<<"        "<<endl;
    fout<<"#define Length_Code "<<Length_Code<<"          //Code length"<<endl;
    fout<<"#define Length_R "<<Length_R<<"              //Redundant bit"<<endl;
    fout<<"#define Length_D "<<Length_D<<"             //Data bit"<<endl;
    fout<<"#define Length_Range "<<pow(2,Length_R)<<"    //Size of array of probability of adding array"<<endl;

    fout<<"        "<<endl;
    fout<<"        "<<endl;
    // main body of the function
    fout<<"int main(){"<<endl;
    fout<<"        "<<endl;
    fout<<"ifstream fin;"<<endl;
    fout<<"ofstream fout;"<<endl;
    fout<<"fin.open(\"1.txt\");"<<endl;
    fout<<"        "<<endl;
    fout<<"int col[Length_Code]={0};"<<endl;
    fout<<"unsigned char tmp["<<30<<"];"<<endl;
    fout<<"int cnt = 100; "<<endl;
    fout<<"int counter = 0;"<<endl;
    fout<<"        "<<endl;

    fout<<"while(cnt){"<<endl;
    fout<<"// Read \"******************\" Row"<<endl;
    fout<<"  fin>>tmp;"<<endl;
    fout<<"// Indentify the end of the document"<<endl;
    fout<<"  if (!fin.good()) break;"<<endl;
    fout<<"//Identify to read a new line"<<endl;
    fout<<"  if(tmp[0]=='*')"<<endl;
    fout<<"  cout<<\"****** Read Matrix Successfully ******\"<<endl;"<<endl;
    fout<<"  else"<<endl;
    fout<<"  {cout<<\"****** Read Failure ******\"<<endl; break;}"<<endl;
    fout<<"//Process Number"<<endl;
    fout<<"counter++;"<<endl;
    fout<<"cout<<\"Process No. \"<<counter<<\" is doing !\"<<endl;"<<endl;
    fout<<"  "<<endl;
    fout<<"//Begining Reading the Matrix"<<endl;
    fout<<"  for (int i=0;i<Length_Code;i++){"<<endl;
    fout<<"    fin>>col[i];"<<endl;
    fout<<"  }"<<endl;
    fout<<"        "<<endl;


    for(int i=0;i<p_num;i++){
      fout<<"int e_p"<<(i+1)<<"["<<*(p_size+i)<<"] = {";
      for(int j=0;j<*(p_size+i)-1;j++){
        fout<< *(*(p+i)+j) <<",";
      }
      fout<<*(*(p+i) + *(p_size+i)-1)<<"};"<<endl;
    }
    fout<<"        "<<endl;
    for(int i=0;i<p_num;i++){
      fout<<"int Syn_"<<(i+1)<<"["<<Length_Code<<"] = {0};"<<endl;
    }
    fout<<"        "<<endl;
    for(int i=0;i<p_num;i++){
      fout<<"int p"<<(i+1)<<"_en["<<Length_Code<<"] = {";
      for(int j=0;j<Length_D+Length_R-1;j++){
        fout<<*(*(en_p+i)+j)<<",";
      }
      fout<<*(*(en_p+i)+Length_D+Length_R-2)<<"};";
      fout<<"\n";
    }
    fout<<"        "<<endl;
    for(int l=0;l<p_num;l++){
      fout<<"int Syndrome_P"<<(l+1)<<" (int A[Length_Code], int column_i, int row_number, int e_p"<<(l+1)<<"["<<*(p_size+l)<<"]);"<<endl;
    }
    fout<<"        "<<endl;
    fout<<"// Error_Pattern_to_Syndrome "<<endl;
    fout<<"for (int i=0;i<Length_Code;i++){"<<endl;
    for(int j=0;j<p_num;j++){
      fout<<"if (p"<<(j+1)<<"_en[i]==1) Syn_"<<(j+1)<<"[i] = Syndrome_P"<<(j+1)<<"(col,i,Length_R,e_p"<<(j+1)<<");"<<endl;
      fout<<"else Syn_"<<(j+1)<<"[i]="<<pow(2,Length_R)+10<<";"<<endl;
    }
    fout<<"}"<<endl;

    fout<<"        "<<endl;
    fout<<"// !=0 "<<endl;
    fout<<"for (int i=0;i<Length_Code;i++){"<<endl;
    for(int j=0;j<p_num;j++){
    fout<<"  if (Syn_"<<(j+1)<<"[i] == 0) {"<<endl;
    fout<<"    cout<<\"Check_Failed\"<<endl;"<<endl;
    fout<<"    return 0;"<<endl;
    fout<<"  }"<<endl;
    }
    fout<<"}"<<endl;


    fout<<"        "<<endl;
    fout<<"// Comparison in the Same Group "<<endl;
    for(int j=0;j<p_num;j++){
    fout<<"  for (int i=0;i<Length_Code-1;i++){"<<endl;
    fout<<"    if (Syn_"<<(j+1)<<"[i] == "<<pow(2,Length_R)+10<<") continue;"<<endl;
    fout<<"    for (int j=i+1;j<Length_Code;j++){"<<endl;
    fout<<"      if (Syn_"<<(j+1)<<"[i] == Syn_"<<(j+1)<<"[j]){"<<endl;
    fout<<"        cout<<\"Check_Failed\"<<endl;"<<endl;
    fout<<"        return 0;"<<endl;
    fout<<"      }"<<endl;
    fout<<"    }"<<endl;
    fout<<"  }"<<endl;
    }

    fout<<"        "<<endl;
    fout<<"// Comparison in the Different Group "<<endl;
    for(int i=0;i<p_num;i++){
      for(int j=i+1;j<p_num;j++){
    fout<<"   for (int i=0;i<Length_Code;i++){"<<endl;
    fout<<"     if (Syn_"<<(i+1)<<"[i] == "<<pow(2,Length_R)+10<<") continue;"<<endl;
    fout<<"     for (int j=0;j<Length_Code;j++){"<<endl;
    fout<<"       if (Syn_"<<(i+1)<<"[i] == Syn_"<<j+1<<"[j]){"<<endl;
    fout<<"         cout<<\"Check_Failed\"<<endl;"<<endl;
    fout<<"         return 0;"<<endl;
    fout<<"       }"<<endl;
    fout<<"     }"<<endl;
    fout<<"   }"<<endl;
      }
    }

    fout<<"        "<<endl;
    fout<<"cout<<\" Check_OK \"<<endl;"<<endl;
    fout<<"        "<<endl;
    fout<<"} //while_end()"<<endl;
    fout<<"fout.close();"<<endl;
    fout<<"}"<<endl; //main {}
    fout<<"        "<<endl;

    //************************
    // Syndrome_Calculation Function
    for(int l=0;l<p_num;l++){
      fout<<"        "<<endl;
      fout<<"int Syndrome_P"<<(l+1)<<" (int A[Length_Code], int column_i, int row_number, int e_p"<<(l+1)<<"["<<*(p_size+l)<<"]){"<<endl;
      fout<<"         "<<endl;
      for(int i=0;i<*(p_size+l);i++){
        fout<<"int a_"<<(i+1)<<" = A[column_i-"<<(*(p_size+l)-1-i)<<"];"<<endl;
      }
      fout<<"     "<<endl;
      for(int i=0;i<*(p_size+l);i++){
        fout<<"int A_"<<(i+1)<<"[row_number];"<<endl;
      }
      fout<<"         "<<endl;
      fout<<"int A_S[row_number];"<<endl;
      fout<<"int syn = 0;"<<endl;
      fout<<"         "<<endl;
      for(int i=0;i<*(p_size+l);i++){
        fout<<"for(int i=0;i<row_number;i++){"<<endl;
        fout<<"  if ((a_"<<(i+1)<<"&(1<<i))) A_"<<(i+1)<<"[i] = 1;"<<endl;
        fout<<"  else A_"<<(i+1)<<"[i] = 0;"<<endl;
        fout<<"}"<<endl;
      }
      fout<<"         "<<endl;
      fout<<"for(int i=0;i<row_number;i++){"<<endl;
      fout<<"  A_S[i] = ";
      if(*(p_size+l) == 1) fout<<"(e_p"<<(l+1)<<"[0] && A_"<<*(p_size+l)<<"[i]);"<<endl;
      if(*(p_size+l) > 1){
        fout<<"(e_p"<<(l+1)<<"[0]&&A_"<<1<<"[i])";
        for(int i=1;i<*(p_size+l);i++){
            fout<<"^(e_p"<<(l+1)<<"["<<i<<"]&&A_"<<(i+1)<<"[i])";
        }
      fout<<";"<<endl;
      }
      fout<<"}"<<endl;

      fout<<"for(int i=0;i<row_number;i++){"<<endl;
      fout<<"  syn += pow(2,i)*A_S[i];"<<endl;
      fout<<"}"<<endl;
      fout<<"return syn;"<<endl;
      fout<<"}"<<endl;
    }


    // delete the dynamic array
    delete[] p_size;
    delete[] parity;
    delete[] pb_position;

    for(int i=0;i<p_num;i++){delete[] p[i];}
    delete[] p;

    for(int i=0;i<p_num;i++){delete[] p_range[i];}
    delete[] p_range;

    for(int i=0;i<p_num;i++){delete[] en_p[i];}
    delete[] en_p;

    cout<<"--------------------------------------------"<<endl;
    cout<<">>> Auto_ECC Check_Program Successful ***"<<endl;
    cout<<"--------------------------------------------"<<endl;
    cout<<"##############################################"<<endl;
    cout<<"##############################################"<<endl;
    cout<<"                                            "<<endl;
  }



if(Mode == 1){
  int Length_Code = Length_D + Length_R;

  int *p_size = new int[p_num]; // error_pattern correction size
  for(int i=0;i<p_num;i++){
    *(p_size+i) = 0;
  }

  int *pd_size = new int[pd_num];  // error_pattern detection size
  for(int i=0;i<pd_num;i++){
    *(pd_size+i) = 0;
  }

  int **p = new int*[p_num];  // error_pattern correction
  for(int i=0;i<p_num;i++) {p[i] = new int[P_in_length];}

  int **pd = new int*[pd_num];  // error_pattern detection
  for(int i=0;i<pd_num;i++) {pd[i] = new int[P_in_length];}

  int **p_range = new int*[p_num];   // error pattern range correction
  for(int i=0;i<p_num;i++) p_range[i] = new int[2*Pran_num];

  int **pd_range = new int*[pd_num];   // error pattern range detection
  for(int i=0;i<pd_num;i++) pd_range[i] = new int[2*Pran_num];

  int **en_p = new int*[p_num];    // error_pattern correction enable
  for(int i=0;i<p_num;i++) en_p[i] = new int[Length_D+Length_R];

  int **en_pd = new int*[pd_num];    // error_pattern detection enable
  for(int i=0;i<pd_num;i++) en_pd[i] = new int[Length_D+Length_R];

  for(int i=0;i<p_num;i++){    // error_pattern correction enable initialization
    for(int j=0;j<Length_D+Length_R;j++){
      *(*(en_p+i)+j) = 0;
    }
  }

  for(int i=0;i<pd_num;i++){   // error_pattern detection enable initialization
    for(int j=0;j<Length_D+Length_R;j++){
      *(*(en_pd+i)+j) = 0;
    }
  }

  int *parity = new int[Length_R];  // parity_bit value
  for(int i=0;i<Length_R;i++){
    *(parity+i) = 0;
  }

  int *pb_position = new int[Length_R];  //parity_bit position
  for(int i=0;i<Length_R;i++){
    *(pb_position+i) = 0;
  }

  cout<<"********************************************"<<endl;
  cout<<"               Check_Program                "<<endl;
  cout<<"********************************************"<<endl;

  while(cnt){
    fin>>tmp;  // read ****************** line
    if (!fin.good()) break;
  // read data
    if(tmp[0]=='*')
    cout<<"             Read_Data_Successful         "<<endl;
    else
    {cout<<"            Read_Data_Failure            "<<endl; break;}
    cout<<"********************************************"<<endl;
  // start to read Error_Pattern
    for (int i=0;i<p_num;i++){
      for(int j=0;j<P_in_length;j++){
        fin>> *(*(p+i)+j);
      }
    }
    fin>>tmp;  // read ****************** line
  // start to read Error_Pattern_Range
    for (int i=0;i<p_num;i++){
      for(int j=0;j<2*Pran_num;j++){
        fin>> *(*(p_range+i)+j);
      }
    }
    fin>>tmp;  // read ****************** line
  // start to read Error_Pattern Detection
    for (int i=0;i<pd_num;i++){
      for(int j=0;j<P_in_length;j++){
        fin>> *(*(pd+i)+j);
      }
    }
    fin>>tmp;  // read ****************** line
  // start to read Error_Pattern_Range Detection
    for (int i=0;i<pd_num;i++){
      for(int j=0;j<2*Pran_num;j++){
        fin>> *(*(pd_range+i)+j);
      }
    }
    fin>>tmp;  // read ****************** line
  // start to read Parity_Bit_Position
    for (int i=0;i<Length_R;i++){
      fin>> *(pb_position+i);
    }
  }// while{}end

  // --------------------------------
  // Obtain the Error_Pattern size by using the  error_pattern
  for (int i=0;i<p_num;i++){
    for(int j=(P_in_length-1);j>=0;j--){
      if (*(*(p+i)+j) == 1){
        *(p_size+i) = j+1;
        break;
      }
    }
  }
  // Obtain the Detection Error_Pattern size by using the error_pattern
  for (int i=0;i<pd_num;i++){
    for(int j=(P_in_length-1);j>=0;j--){
      if (*(*(pd+i)+j) == 1){
        *(pd_size+i) = j+1;
        break;
      }
    }
  }
  // --------------------------------
  // 5 groups of the range to the error pattern enable
  for(int i=0;i<p_num;i++){
    for(int j=0;j<Pran_num;j++){
      if(*(*(p_range+i)+2*j) != 0){
        for(int k=(*(*(p_range+i)+2*j)-1 + *(p_size+i)-1); k<*(*(p_range+i)+2*j+1); k++)
          *(*(en_p+i)+k) = 1;
        }
      if(*(*(p_range+i)+2*j) == 0) continue;
     }
    }
  // 5 groups of the range to the detection error pattern enable
  for(int i=0;i<pd_num;i++){
    for(int j=0;j<Pran_num;j++){
      if(*(*(pd_range+i)+2*j) != 0){
        for(int k=(*(*(pd_range+i)+2*j)-1 + *(pd_size+i)-1); k<*(*(pd_range+i)+2*j+1); k++)
          *(*(en_pd+i)+k) = 1;
        }
      if(*(*(pd_range+i)+2*j) == 0) continue;
     }
    }
  // --------------------------------
  // set the range of the circle ( the element ordered by the weight from low to high)
  int array_size = pow(2,Length_R)-1;
  int *array = new int[array_size];
  for(int i=0;i<array_size;i++){
    array[i]=i+1;
  }
  int key;
  for (int i = 0; i < array_size; i++){
      key=0;
      for (int j = 0; j+1<array_size-i; j++){
          if (Counter(*(array+j),Length_R) > Counter(*(array+j+1),Length_R)){
              key=1;
              swap((array+j), (array+j+1));
          }
      }
      if (key==0) {
          break;
      }
  }

// --------------------------------
  cout<<">>> Correction Error Pattern Range : "<<endl;
  for(int i=0;i<p_num;i++){
    for(int j=0;j<2*Pran_num;j++){
      cout<< *(*(p_range+i)+j) <<" ";
    }
    cout<<"\n";
  }
  cout<<"********************************************"<<endl;
  cout<<">>> Detection Error Pattern Range : "<<endl;
  for(int i=0;i<pd_num;i++){
    for(int j=0;j<2*Pran_num;j++){
      cout<< *(*(pd_range+i)+j) <<" ";
    }
    cout<<"\n";
  }
  cout<<"********************************************"<<endl;
  cout<<">>> Correction Error Pattern Enable : "<<endl;
  for(int i=0;i<p_num;i++){
    for(int j=0;j<Length_D+Length_R;j++){
      cout<< *(*(en_p+i)+j) <<" ";
    }
    cout<<"\n";
  }
  cout<<"********************************************"<<endl;
  cout<<">>> Detection Error Pattern Enable : "<<endl;
  for(int i=0;i<pd_num;i++){
    for(int j=0;j<Length_D+Length_R;j++){
      cout<< *(*(en_pd+i)+j) <<" ";
    }
    cout<<"\n";
  }

  // definition
  fout<<"#include <iostream>"<<endl;
  fout<<"#include <stdio.h>"<<endl;
  fout<<"#include <fstream>"<<endl;
  fout<<"#include <math.h>"<<endl;
  fout<<"#include <stdlib.h>"<<endl;
  fout<<"#include <time.h>"<<endl;
  fout<<"using namespace std;"<<endl;
  fout<<"        "<<endl;
  fout<<"#define Length_Code "<<Length_Code<<"          //Code length"<<endl;
  fout<<"#define Length_R "<<Length_R<<"              //Redundant bit"<<endl;
  fout<<"#define Length_D "<<Length_D<<"             //Data bit"<<endl;
  fout<<"#define Length_Range "<<pow(2,Length_R)<<"    //Size of array of probability of adding array"<<endl;

  fout<<"        "<<endl;
  fout<<"        "<<endl;
  // main body of the function
  fout<<"int main(){"<<endl;
  fout<<"        "<<endl;
  fout<<"ifstream fin;"<<endl;
  fout<<"ofstream fout;"<<endl;
  fout<<"fin.open(\"1.txt\");"<<endl;
  fout<<"        "<<endl;
  fout<<"int col[Length_Code]={0};"<<endl;
  fout<<"unsigned char tmp["<<30<<"];"<<endl;
  fout<<"int cnt = 100; "<<endl;
  fout<<"int counter = 0;"<<endl;
  fout<<"        "<<endl;

  fout<<"while(cnt){"<<endl;
  fout<<"// Read \"******************\" Row"<<endl;
  fout<<"  fin>>tmp;"<<endl;
  fout<<"// Indentify the end of the document"<<endl;
  fout<<"  if (!fin.good()) break;"<<endl;
  fout<<"//Identify to read a new line"<<endl;
  fout<<"  if(tmp[0]=='*')"<<endl;
  fout<<"  cout<<\"****** Read Matrix Successfully ******\"<<endl;"<<endl;
  fout<<"  else"<<endl;
  fout<<"  {cout<<\"****** Read Failure ******\"<<endl; break;}"<<endl;
  fout<<"//Process Number"<<endl;
  fout<<"counter++;"<<endl;
  fout<<"cout<<\"Process No. \"<<counter<<\" is doing !\"<<endl;"<<endl;
  fout<<"  "<<endl;
  fout<<"//Begining Reading the Matrix"<<endl;
  fout<<"  for (int i=0;i<Length_Code;i++){"<<endl;
  fout<<"    fin>>col[i];"<<endl;
  fout<<"  }"<<endl;
  fout<<"        "<<endl;


  for(int i=0;i<p_num;i++){
    fout<<"int e_p"<<(i+1)<<"["<<*(p_size+i)<<"] = {";
    for(int j=0;j<*(p_size+i)-1;j++){
      fout<< *(*(p+i)+j) <<",";
    }
    fout<<*(*(p+i) + *(p_size+i)-1)<<"};"<<endl;
  }
  fout<<"        "<<endl;

  for(int i=0;i<pd_num;i++){
    fout<<"int e_pd"<<(i+1)<<"["<<*(pd_size+i)<<"] = {";
    for(int j=0;j<*(pd_size+i)-1;j++){
      fout<< *(*(pd+i)+j) <<",";
    }
    fout<<*(*(pd+i) + *(pd_size+i)-1)<<"};"<<endl;
  }
  fout<<"        "<<endl;

  for(int i=0;i<p_num;i++){
    fout<<"int Syn_"<<(i+1)<<"["<<Length_Code<<"] = {0};"<<endl;
  }
  fout<<"        "<<endl;
  for(int i=0;i<pd_num;i++){
    fout<<"int Syn_pd"<<(i+1)<<"["<<Length_Code<<"] = {0};"<<endl;
  }
  fout<<"        "<<endl;

  for(int i=0;i<p_num;i++){
    fout<<"int p"<<(i+1)<<"_en["<<Length_Code<<"] = {";
    for(int j=0;j<Length_D+Length_R-1;j++){
      fout<<*(*(en_p+i)+j)<<",";
    }
    fout<<*(*(en_p+i)+Length_D+Length_R-2)<<"};";
    fout<<"\n";
  }
  fout<<"        "<<endl;

  for(int i=0;i<pd_num;i++){
    fout<<"int pd"<<(i+1)<<"_en["<<Length_Code<<"] = {";
    for(int j=0;j<Length_D+Length_R-1;j++){
      fout<< *(*(en_pd+i)+j) <<",";
    }
    fout<<*(*(en_pd+i)+Length_D+Length_R-2)<<"};";
    fout<<"\n";
  }
  fout<<"        "<<endl;

  for(int l=0;l<p_num;l++){
    fout<<"int Syndrome_P"<<(l+1)<<" (int A[Length_Code], int column_i, int row_number, int e_p"<<(l+1)<<"["<<*(p_size+l)<<"]);"<<endl;
  }
  fout<<"        "<<endl;
  for(int l=0;l<pd_num;l++){
    fout<<"int Syndrome_PD"<<(l+1)<<" (int A[Length_Code], int column_i, int row_number, int e_pd"<<(l+1)<<"["<<*(pd_size+l)<<"]);"<<endl;
  }
  fout<<"        "<<endl;

  fout<<"// Error_Pattern_to_Syndrome "<<endl;
  fout<<"for (int i=0;i<Length_Code;i++){"<<endl;
  for(int j=0;j<p_num;j++){
  fout<<" if (p"<<(j+1)<<"_en[i]==1) Syn_"<<(j+1)<<"[i] = Syndrome_P"<<(j+1)<<"(col,i,Length_R,e_p"<<(j+1)<<");"<<endl;
  fout<<" else Syn_"<<(j+1)<<"[i]="<<pow(2,Length_R)+10<<";"<<endl;
  }
  fout<<"}"<<endl;
  fout<<"        "<<endl;

  fout<<"for (int i=0;i<Length_Code;i++){"<<endl;
  for(int j=0;j<pd_num;j++){
  fout<<" if (pd"<<(j+1)<<"_en[i]==1) Syn_pd"<<(j+1)<<"[i] = Syndrome_PD"<<(j+1)<<"(col,i,Length_R,e_pd"<<(j+1)<<");"<<endl;
  fout<<" else Syn_pd"<<(j+1)<<"[i]="<<pow(2,Length_R)+10<<";"<<endl;
  }
  fout<<"}"<<endl;
  fout<<"        "<<endl;

  fout<<"// !=0 "<<endl;
  fout<<"for (int i=0;i<Length_Code;i++){"<<endl;
  for(int j=0;j<p_num;j++){
  fout<<"  if (Syn_"<<(j+1)<<"[i] == 0) {"<<endl;
  fout<<"    cout<<\"Check_Failed\"<<endl;"<<endl;
  fout<<"    return 0;"<<endl;
  fout<<"  }"<<endl;
  }
  fout<<"}"<<endl;
  fout<<"        "<<endl;

  fout<<"for (int i=0;i<Length_Code;i++){"<<endl;
  for(int j=0;j<pd_num;j++){
  fout<<"  if (Syn_pd"<<(j+1)<<"[i] == 0) {"<<endl;
  fout<<"    cout<<\"Check_Failed\"<<endl;"<<endl;
  fout<<"    return 0;"<<endl;
  fout<<"  }"<<endl;
  }
  fout<<"}"<<endl;

  fout<<"        "<<endl;
  fout<<"// Comparison in the Same Group "<<endl;
  for(int j=0;j<p_num;j++){
  fout<<"  for (int i=0;i<Length_Code-1;i++){"<<endl;
  fout<<"    if (Syn_"<<(j+1)<<"[i] == "<<pow(2,Length_R)+10<<") continue;"<<endl;
  fout<<"    for (int j=i+1;j<Length_Code;j++){"<<endl;
  fout<<"      if (Syn_"<<(j+1)<<"[i] == Syn_"<<(j+1)<<"[j]){"<<endl;
  fout<<"        cout<<\"Check_Failed\"<<endl;"<<endl;
  fout<<"        return 0;"<<endl;
  fout<<"      }"<<endl;
  fout<<"    }"<<endl;
  fout<<"  }"<<endl;
  }

  fout<<"        "<<endl;
  fout<<"// Comparison in the Different Correction Group "<<endl;
  for(int i=0;i<p_num;i++){
    for(int j=i+1;j<p_num;j++){
  fout<<"   for (int i=0;i<Length_Code;i++){"<<endl;
  fout<<"     if (Syn_"<<(i+1)<<"[i] == "<<pow(2,Length_R)+10<<") continue;"<<endl;
  fout<<"     for (int j=0;j<Length_Code;j++){"<<endl;
  fout<<"       if (Syn_"<<(i+1)<<"[i] == Syn_"<<j+1<<"[j]){"<<endl;
  fout<<"         cout<<\"Check_Failed\"<<endl;"<<endl;
  fout<<"         return 0;"<<endl;
  fout<<"       }"<<endl;
  fout<<"     }"<<endl;
  fout<<"   }"<<endl;
    }
  }

  fout<<"// Comparison in the Different Detection Group "<<endl;
  for(int i=0;i<p_num;i++){
    for(int j=0;j<pd_num;j++){
  fout<<"   for (int i=0;i<Length_Code;i++){"<<endl;
  fout<<"     if (Syn_"<<(i+1)<<"[i] == "<<pow(2,Length_R)+10<<") continue;"<<endl;
  fout<<"     for (int j=0;j<Length_Code;j++){"<<endl;
  fout<<"       if (Syn_"<<(i+1)<<"[i] == Syn_pd"<<j+1<<"[j]){"<<endl;
  fout<<"         cout<<\"Check_Failed\"<<endl;"<<endl;
  fout<<"         return 0;"<<endl;
  fout<<"       }"<<endl;
  fout<<"     }"<<endl;
  fout<<"   }"<<endl;
    }
  }

  fout<<"        "<<endl;
  fout<<"cout<<\" Check_OK \"<<endl;"<<endl;
  fout<<"        "<<endl;
  fout<<"} //while_end()"<<endl;
  fout<<"fout.close();"<<endl;
  fout<<"}"<<endl; //main {}
  fout<<"        "<<endl;

  //************************
  // Syndrome_Correction Calculation Function
  for(int l=0;l<p_num;l++){
    fout<<"        "<<endl;
    fout<<"int Syndrome_P"<<(l+1)<<" (int A[Length_Code], int column_i, int row_number, int e_p"<<(l+1)<<"["<<*(p_size+l)<<"]){"<<endl;
    fout<<"         "<<endl;
    for(int i=0;i<*(p_size+l);i++){
      fout<<"int a_"<<(i+1)<<" = A[column_i-"<<(*(p_size+l)-1-i)<<"];"<<endl;
    }
    fout<<"     "<<endl;
    for(int i=0;i<*(p_size+l);i++){
      fout<<"int A_"<<(i+1)<<"[row_number];"<<endl;
    }
    fout<<"         "<<endl;
    fout<<"int A_S[row_number];"<<endl;
    fout<<"int syn = 0;"<<endl;
    fout<<"         "<<endl;
    for(int i=0;i<*(p_size+l);i++){
      fout<<"for(int i=0;i<row_number;i++){"<<endl;
      fout<<"  if ((a_"<<(i+1)<<"&(1<<i))) A_"<<(i+1)<<"[i] = 1;"<<endl;
      fout<<"  else A_"<<(i+1)<<"[i] = 0;"<<endl;
      fout<<"}"<<endl;
    }
    fout<<"         "<<endl;
    fout<<"for(int i=0;i<row_number;i++){"<<endl;
    fout<<"  A_S[i] = ";
    if(*(p_size+l) == 1) fout<<"(e_p"<<(l+1)<<"[0] && A_"<<*(p_size+l)<<"[i]);"<<endl;
    if(*(p_size+l) > 1){
      fout<<"(e_p"<<(l+1)<<"[0]&&A_"<<1<<"[i])";
      for(int i=1;i<*(p_size+l);i++){
          fout<<"^(e_p"<<(l+1)<<"["<<i<<"]&&A_"<<(i+1)<<"[i])";
      }
    fout<<";"<<endl;
    }
    fout<<"}"<<endl;

    fout<<"for(int i=0;i<row_number;i++){"<<endl;
    fout<<"  syn += pow(2,i)*A_S[i];"<<endl;
    fout<<"}"<<endl;
    fout<<"return syn;"<<endl;
    fout<<"}"<<endl;
  }
  // Syndrome_Detection Calculation Function
  for(int l=0;l<pd_num;l++){
    fout<<"        "<<endl;
    fout<<"int Syndrome_PD"<<(l+1)<<" (int A[Length_Code], int column_i, int row_number, int e_pd"<<(l+1)<<"["<<*(pd_size+l)<<"]){"<<endl;
    fout<<"         "<<endl;
    for(int i=0;i<*(pd_size+l);i++){
      fout<<"int a_"<<(i+1)<<" = A[column_i-"<<(*(pd_size+l)-1-i)<<"];"<<endl;
    }
    fout<<"     "<<endl;
    for(int i=0;i<*(pd_size+l);i++){
      fout<<"int A_"<<(i+1)<<"[row_number];"<<endl;
    }
    fout<<"         "<<endl;
    fout<<"int A_S[row_number];"<<endl;
    fout<<"int syn = 0;"<<endl;
    fout<<"         "<<endl;
    for(int i=0;i<*(pd_size+l);i++){
      fout<<"for(int i=0;i<row_number;i++){"<<endl;
      fout<<"  if ((a_"<<(i+1)<<"&(1<<i))) A_"<<(i+1)<<"[i] = 1;"<<endl;
      fout<<"  else A_"<<(i+1)<<"[i] = 0;"<<endl;
      fout<<"}"<<endl;
    }
    fout<<"         "<<endl;
    fout<<"for(int i=0;i<row_number;i++){"<<endl;
    fout<<"  A_S[i] = ";
    if(*(pd_size+l) == 1) fout<<"(e_pd"<<(l+1)<<"[0] && A_"<<*(p_size+l)<<"[i]);"<<endl;
    if(*(pd_size+l) > 1){
      fout<<"(e_pd"<<(l+1)<<"[0]&&A_"<<1<<"[i])";
      for(int i=1;i<*(pd_size+l);i++){
          fout<<"^(e_pd"<<(l+1)<<"["<<i<<"]&&A_"<<(i+1)<<"[i])";
      }
    fout<<";"<<endl;
    }
    fout<<"}"<<endl;

    fout<<"for(int i=0;i<row_number;i++){"<<endl;
    fout<<"  syn += pow(2,i)*A_S[i];"<<endl;
    fout<<"}"<<endl;
    fout<<"return syn;"<<endl;
    fout<<"}"<<endl;
  }


  // delete the dynamic array
  delete[] p_size;
  delete[] parity;
  delete[] pb_position;

  for(int i=0;i<p_num;i++){delete[] p[i];}
  delete[] p;

  for(int i=0;i<p_num;i++){delete[] p_range[i];}
  delete[] p_range;

  for(int i=0;i<p_num;i++){delete[] en_p[i];}
  delete[] en_p;

  cout<<"                                            "<<endl;
  cout<<"********************************************"<<endl;
  cout<<"***--------------------------------------***"<<endl;
  cout<<"*** Auto_ECC Check_Program Successful ***"<<endl;
  cout<<"***--------------------------------------***"<<endl;
  cout<<"********************************************"<<endl;
  cout<<"                                            "<<endl;
}

fout.close();
}
// other function
//---------------------
int Counter(int a, int row_number){
  int A[row_number];
  for(int i=0;i<row_number;i++){
    if ((a&(1<<i))) A[i] = 1;
    else A[i] = 0;
  }
  int counter = 0;
  for(int i=0;i<row_number;i++){
      if(A[i]==1) counter++;
      else continue;
  }
  return counter;
}

void swap(int *a, int *b){
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
