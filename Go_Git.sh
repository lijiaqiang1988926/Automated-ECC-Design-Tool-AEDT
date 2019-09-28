#file_name: Go.sh
Length_D=12
Length_R=6
Length_Code=18
Pattern_Cor_Num=4
Pattern_Dec_Num=0
Stoping_time=1000
Mode=0
Injection_Num=1

#ECC_Generate_Program
./ECC_Search_Gen $Length_D $Length_R $Pattern_Cor_Num $Pattern_Dec_Num $Stoping_time $Mode
g++ ECC_Search.C -o ECC_Search
./ECC_Search

#Check_Program
./Check_Gen $Length_D $Length_R $Pattern_Cor_Num $Pattern_Dec_Num $Stoping_time $Mode
g++ Check.C -o Check
./Check

#File Preparation
head -n $(($Pattern_Cor_Num+1)) ./Set.txt > EP_in.txt
tail -n 2 ./1.txt > Matrix.txt

#Error_Pattern_Generation
./Error_Pattern_Gen $Length_Code $Pattern_Cor_Num

#Fault_Injection_Generation
./Fault_Injection_Gen $Length_D $Length_R $Pattern_Cor_Num $Pattern_Dec_Num $Stoping_time
g++ Fault_Inject.C -o Fault_Inject
./Fault_Inject $Injection_Num
