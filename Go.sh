# This is an example for 3-bit burst error code
#file_name: Go.sh
Length_D=16
Length_R=7
Length_Code=23
Pattern_Cor_Num=4
Pattern_Dec_Num=0
Stoping_time=1000
Mode=0
Injection_Num=1

#ECC_Generate_Program
g++ ECC_Search_Gen.C -o ECC_Search_Gen
./ECC_Search_Gen  $Pattern_Cor_Num $Pattern_Dec_Num $Stoping_time $Mode
g++ ECC_Search.C -o ECC_Search
./ECC_Search

#Check_Program
g++ Check_Gen.C -o Check_Gen
./Check_Gen $Length_D $Length_R $Pattern_Cor_Num $Pattern_Dec_Num $Stoping_time $Mode
g++ Check.C -o Check
./Check

#File Preparation
head -n $(($Pattern_Cor_Num+1)) ./Set.txt > EP_in.txt
tail -n 2 ./1.txt > Matrix.txt

#Error_Pattern_Generation
g++ Error_Pattern_Gen.C -o Error_Pattern_Gen
./Error_Pattern_Gen $Length_Code $Pattern_Cor_Num

#Fault_Injection_Generation
g++ Fault_Injection_Gen.C -o Fault_Injection_Gen
./Fault_Injection_Gen $Length_D $Length_R $Pattern_Cor_Num $Pattern_Dec_Num $Stoping_time
g++ Fault_Inject.C -o Fault_Inject
./Fault_Inject $Injection_Num
