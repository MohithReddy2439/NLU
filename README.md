{NLU-Pattren Matching Algorithm} Note:This is not problem description for problem description check "Problem.docx"

__________________________________
PreRequsites,Algorithm Description
__________________________________

_____________
PreRequisites
_____________

1)We Expect all files to be in Unix Format.

2)strstr(char*,char*),strcasestr(char*,char*) are been extensively used so non-printable characters like '\n' ,'\r' ,' ' matters , particularly in case of concepts,placeholders file we directly check for \n to remove that and copy remaining buffer stream to character array if ' ' or '\r' presents before '\n' then those are also copied to file stream which breaks the code 

Testing-: cat -e should output (content)$ but not (content) $ or (content)'\r''\n'$

3)No Unicode character is supported that might break the code

4)When Another language which has same script as english is used application may not recognise dateTime , time , day , date because the english months and weeks are hardcoded

______________________
Alogorthim Description
______________________

Whole code can be broke out in 6 parts
______________________
1)Declaring Structures
______________________ 

Four Structures PlaceHolder,Grammar,Concept,Input are defined 

PlaceHolder is used to store placeholder name and contents of it and it has count variable to track contents of char contents

Grammar is used to store command name ,concepts , placeholders in definition of command

Concept is used to store concept name and it synonms 
________________________________________________
2)Processing Concept Directory to learn concepts
________________________________________________

dirent.h is used to open Concept directory and view files and in return the file names are used to open files and store contents of it in Concept Structure

__________________________________________________________________
3)Processing Grammar Directory to leran Grammar Rules and syntaxes
__________________________________________________________________

Grammar Directory is opend to view files and placeholders,concepts of each command and each possible definiton of command stored in systamatic order

________________________________________________________
4)Processing PlaceHolder Directory to learn placeholders
________________________________________________________

PlaceHolder/PlaceHolderDetail.txt is opened to know possible placeholder and if it demands for FileLookup corresponding file is opened to copy its contents

______________________________
5)Processing Testing/input.txt
______________________________

input.txt file is opened to know queries and if "Case#" is there that line is skiped assuming it as comment and remaining lines are copied for future use

Every line is checked across every placeholder concept to recognise possible placeholders and concepts in it and stored in Input Structure

We limited ourself to identify placeholder which are of type codded , we hardcoded all english months and weeks to check the existense of dateTime,time,date,day placeholders

___________________________________________________________________________________
6)Comparing Input Structres with Grammar Syntaxes and Writing Results to output.txt
___________________________________________________________________________________

6 important variables to know are cncont(concept contribution),cmcont(command contribution), phcont(placeholder contribution) ,cmweight(command weight),cnweight(concept contribution),phweight(placeholder weight)

It is clearly understandable from their names that every input line is compared with every grammar rule and  contribution are calculated , multiplied with weights  and command with highest weight is declared as result

Comparing with concept and placeholders of grammar makes sense why to even compare command name??

Take CreateAlarmWithTime as example while storing its grammar syntax Alarm keyword which is really important is not stored since it is not placeholder or concept , So comparring Words command name with input buffer really increases accuracy

But Articles Prepositions in command name are going to effect accuracy so to eliminate that ArPr.txt file is used which has list of commonly used artciles and prepositions

Finally after finding result it is written to "Testing/output.txt" file




















