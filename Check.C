#include <iostream>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
        
#define Length_Code 18          //Code length
#define Length_R 6              //Redundant bit
#define Length_D 12             //Data bit
#define Length_Range 64    //Size of array of probability of adding array
        
        
int main(){
        
ifstream fin;
ofstream fout;
fin.open("1.txt");
        
int col[Length_Code]={0};
unsigned char tmp[30];
int cnt = 100; 
int counter = 0;
        
while(cnt){
// Read "******************" Row
  fin>>tmp;
// Indentify the end of the document
  if (!fin.good()) break;
//Identify to read a new line
  if(tmp[0]=='*')
  cout<<"****** Read Matrix Successfully ******"<<endl;
  else
  {cout<<"****** Read Failure ******"<<endl; break;}
//Process Number
counter++;
cout<<"Process No. "<<counter<<" is doing !"<<endl;
  
//Begining Reading the Matrix
  for (int i=0;i<Length_Code;i++){
    fin>>col[i];
  }
        
int e_p1[1] = {1};
int e_p2[2] = {1,1};
int e_p3[3] = {1,0,1};
int e_p4[3] = {1,1,1};
        
int Syn_1[18] = {0};
int Syn_2[18] = {0};
int Syn_3[18] = {0};
int Syn_4[18] = {0};
        
int p1_en[18] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
int p2_en[18] = {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
int p3_en[18] = {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
int p4_en[18] = {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
        
int Syndrome_P1 (int A[Length_Code], int column_i, int row_number, int e_p1[1]);
int Syndrome_P2 (int A[Length_Code], int column_i, int row_number, int e_p2[2]);
int Syndrome_P3 (int A[Length_Code], int column_i, int row_number, int e_p3[3]);
int Syndrome_P4 (int A[Length_Code], int column_i, int row_number, int e_p4[3]);
        
// Error_Pattern_to_Syndrome 
for (int i=0;i<Length_Code;i++){
if (p1_en[i]==1) Syn_1[i] = Syndrome_P1(col,i,Length_R,e_p1);
else Syn_1[i]=74;
if (p2_en[i]==1) Syn_2[i] = Syndrome_P2(col,i,Length_R,e_p2);
else Syn_2[i]=74;
if (p3_en[i]==1) Syn_3[i] = Syndrome_P3(col,i,Length_R,e_p3);
else Syn_3[i]=74;
if (p4_en[i]==1) Syn_4[i] = Syndrome_P4(col,i,Length_R,e_p4);
else Syn_4[i]=74;
}
        
// !=0 
for (int i=0;i<Length_Code;i++){
  if (Syn_1[i] == 0) {
    cout<<"Check_Failed"<<endl;
    return 0;
  }
  if (Syn_2[i] == 0) {
    cout<<"Check_Failed"<<endl;
    return 0;
  }
  if (Syn_3[i] == 0) {
    cout<<"Check_Failed"<<endl;
    return 0;
  }
  if (Syn_4[i] == 0) {
    cout<<"Check_Failed"<<endl;
    return 0;
  }
}
        
// Comparison in the Same Group 
  for (int i=0;i<Length_Code-1;i++){
    if (Syn_1[i] == 74) continue;
    for (int j=i+1;j<Length_Code;j++){
      if (Syn_1[i] == Syn_1[j]){
        cout<<"Check_Failed"<<endl;
        return 0;
      }
    }
  }
  for (int i=0;i<Length_Code-1;i++){
    if (Syn_2[i] == 74) continue;
    for (int j=i+1;j<Length_Code;j++){
      if (Syn_2[i] == Syn_2[j]){
        cout<<"Check_Failed"<<endl;
        return 0;
      }
    }
  }
  for (int i=0;i<Length_Code-1;i++){
    if (Syn_3[i] == 74) continue;
    for (int j=i+1;j<Length_Code;j++){
      if (Syn_3[i] == Syn_3[j]){
        cout<<"Check_Failed"<<endl;
        return 0;
      }
    }
  }
  for (int i=0;i<Length_Code-1;i++){
    if (Syn_4[i] == 74) continue;
    for (int j=i+1;j<Length_Code;j++){
      if (Syn_4[i] == Syn_4[j]){
        cout<<"Check_Failed"<<endl;
        return 0;
      }
    }
  }
        
// Comparison in the Different Group 
   for (int i=0;i<Length_Code;i++){
     if (Syn_1[i] == 74) continue;
     for (int j=0;j<Length_Code;j++){
       if (Syn_1[i] == Syn_2[j]){
         cout<<"Check_Failed"<<endl;
         return 0;
       }
     }
   }
   for (int i=0;i<Length_Code;i++){
     if (Syn_1[i] == 74) continue;
     for (int j=0;j<Length_Code;j++){
       if (Syn_1[i] == Syn_3[j]){
         cout<<"Check_Failed"<<endl;
         return 0;
       }
     }
   }
   for (int i=0;i<Length_Code;i++){
     if (Syn_1[i] == 74) continue;
     for (int j=0;j<Length_Code;j++){
       if (Syn_1[i] == Syn_4[j]){
         cout<<"Check_Failed"<<endl;
         return 0;
       }
     }
   }
   for (int i=0;i<Length_Code;i++){
     if (Syn_2[i] == 74) continue;
     for (int j=0;j<Length_Code;j++){
       if (Syn_2[i] == Syn_3[j]){
         cout<<"Check_Failed"<<endl;
         return 0;
       }
     }
   }
   for (int i=0;i<Length_Code;i++){
     if (Syn_2[i] == 74) continue;
     for (int j=0;j<Length_Code;j++){
       if (Syn_2[i] == Syn_4[j]){
         cout<<"Check_Failed"<<endl;
         return 0;
       }
     }
   }
   for (int i=0;i<Length_Code;i++){
     if (Syn_3[i] == 74) continue;
     for (int j=0;j<Length_Code;j++){
       if (Syn_3[i] == Syn_4[j]){
         cout<<"Check_Failed"<<endl;
         return 0;
       }
     }
   }
        
cout<<" Check_OK "<<endl;
        
} //while_end()
fout.close();
}
        
        
int Syndrome_P1 (int A[Length_Code], int column_i, int row_number, int e_p1[1]){
         
int a_1 = A[column_i-0];
     
int A_1[row_number];
         
int A_S[row_number];
int syn = 0;
         
for(int i=0;i<row_number;i++){
  if ((a_1&(1<<i))) A_1[i] = 1;
  else A_1[i] = 0;
}
         
for(int i=0;i<row_number;i++){
  A_S[i] = (e_p1[0] && A_1[i]);
}
for(int i=0;i<row_number;i++){
  syn += pow(2,i)*A_S[i];
}
return syn;
}
        
int Syndrome_P2 (int A[Length_Code], int column_i, int row_number, int e_p2[2]){
         
int a_1 = A[column_i-1];
int a_2 = A[column_i-0];
     
int A_1[row_number];
int A_2[row_number];
         
int A_S[row_number];
int syn = 0;
         
for(int i=0;i<row_number;i++){
  if ((a_1&(1<<i))) A_1[i] = 1;
  else A_1[i] = 0;
}
for(int i=0;i<row_number;i++){
  if ((a_2&(1<<i))) A_2[i] = 1;
  else A_2[i] = 0;
}
         
for(int i=0;i<row_number;i++){
  A_S[i] = (e_p2[0]&&A_1[i])^(e_p2[1]&&A_2[i]);
}
for(int i=0;i<row_number;i++){
  syn += pow(2,i)*A_S[i];
}
return syn;
}
        
int Syndrome_P3 (int A[Length_Code], int column_i, int row_number, int e_p3[3]){
         
int a_1 = A[column_i-2];
int a_2 = A[column_i-1];
int a_3 = A[column_i-0];
     
int A_1[row_number];
int A_2[row_number];
int A_3[row_number];
         
int A_S[row_number];
int syn = 0;
         
for(int i=0;i<row_number;i++){
  if ((a_1&(1<<i))) A_1[i] = 1;
  else A_1[i] = 0;
}
for(int i=0;i<row_number;i++){
  if ((a_2&(1<<i))) A_2[i] = 1;
  else A_2[i] = 0;
}
for(int i=0;i<row_number;i++){
  if ((a_3&(1<<i))) A_3[i] = 1;
  else A_3[i] = 0;
}
         
for(int i=0;i<row_number;i++){
  A_S[i] = (e_p3[0]&&A_1[i])^(e_p3[1]&&A_2[i])^(e_p3[2]&&A_3[i]);
}
for(int i=0;i<row_number;i++){
  syn += pow(2,i)*A_S[i];
}
return syn;
}
        
int Syndrome_P4 (int A[Length_Code], int column_i, int row_number, int e_p4[3]){
         
int a_1 = A[column_i-2];
int a_2 = A[column_i-1];
int a_3 = A[column_i-0];
     
int A_1[row_number];
int A_2[row_number];
int A_3[row_number];
         
int A_S[row_number];
int syn = 0;
         
for(int i=0;i<row_number;i++){
  if ((a_1&(1<<i))) A_1[i] = 1;
  else A_1[i] = 0;
}
for(int i=0;i<row_number;i++){
  if ((a_2&(1<<i))) A_2[i] = 1;
  else A_2[i] = 0;
}
for(int i=0;i<row_number;i++){
  if ((a_3&(1<<i))) A_3[i] = 1;
  else A_3[i] = 0;
}
         
for(int i=0;i<row_number;i++){
  A_S[i] = (e_p4[0]&&A_1[i])^(e_p4[1]&&A_2[i])^(e_p4[2]&&A_3[i]);
}
for(int i=0;i<row_number;i++){
  syn += pow(2,i)*A_S[i];
}
return syn;
}
