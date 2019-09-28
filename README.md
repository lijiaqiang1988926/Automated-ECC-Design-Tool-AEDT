# Pbulic-Automated-ECC-Design-Tool-AEDT
# Motivation
This tool provides a solution to the design of custom Error Correction Codes (ECCs) for memory protection. Normally, ECC design can be done using algebraic equations or computer search algorithms. When the former method is used, the algorithms and programs used are develop ad-hoc and do not support the design of general ECCs. In AEDT, a computer searching algorithm method is selected to be the kernel of the tool construction. This code design technique is based on the binary linear block codes theory can be seen as the Boolean satisfiability problem and solved by the use of the recursive backtracing algorithm. However, the traditional searching program is just applicable to the specific ECC requirement limited by the systematic structure, correction ability, parity bits and data bits length. If these parameters change, the searching program needs to be specially designed and updated. This poses a barrier for the ECC designer and limits its ability to design codes for specific cases. To overcome those issues, we developed the AEDT to help the ECC designers obtain what they want with the least effort and provide various ECC functions.
# Compilation
This tool is developed by using C++. The Visual Studio in Windows or gcc/g++ compiler in Linux is needed.
# Command line arguments
Command line arguments for AEDT are:
Length_D: Length of the data bit
Length_R: Length of the parity bit
Length_Code: Length of the codeword
Pattern_Cor_Num: Number of the error pattern to be corrected
Pattern_Dec_Num: Number of the error pattern to be detected
Stoping_time: Duration time of the running program
Mode: [0] Correction; [1] Correction and Detection
Injection_Num: The number of fault injection.
# Authors
The Tool is developed by Jiaqiang Li. Pedro Reviriego gives important suggestions to improve this tool. Lulu Liao and Zhaoqian zhang develop the GUI of the Tool.
