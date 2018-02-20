#define _GNU_SOURCE //For using function strcasestr

#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
#include<string.h>

//Concept Structure used for storing concepts and its synonms
struct Concept 
{
	char concept[50];
	char synonms[10][50];
	int count;
};

//PlaceHolder Structure used for storing placeholders and its contents
struct PlaceHolder
{
	char placeholder[50];
	char contents[30][50];
	int count,type;
};

//Grammar Structure for storing Command names and following placeholders and concepts in every possible definition

struct Grammar 
{
	char command[50];
	char placeholder[20][300];
	char concept[20][300];
	char superset[100];
	int cncount,phcount;
	int sset;
	int ophrase;
	char syntax[20][300];
};

//Input Structure used for Storing input string , the command the input belongs to , Placeholders and Concepts 
struct Input
{
	int grmatch;
	char input[500];
	char command[50];
	char placeholder[300];
	char concept[300];
	char content[300];
	int synmatch;
};

struct Concept cnarr[30];
struct Grammar grarr[30];
struct PlaceHolder pharr[30];
struct Input inarr[30];
int cncount=0,grcount=0,opt,phcount=0,incount=0;

void NoteNameOpenPhrase(struct Input *input)
{

}

void NoteContentOpenPhrase(struct Input *input)
{
}

void AlarmNameOpenPhrase(struct Input *input)
{
}

void SrcEventNameOpenPhrase(struct Input *input)
{
}

void TgtEventNameOpenPhrase(struct Input *input)
{
}

int main()
{
	char *ConceptDir="Concept";
	char *PlaceHolderFl="PlaceHolder/PlaceHolderDetail.txt";
	char *GrammarDir="Grammar";	
	char file[200],buffer[1000],*temp1,*temp2,input[100],output[100],*temp3,*temp4,tempbuffer[500];
	struct dirent *dent;
	DIR *dir;
	FILE *fp;
	strcpy(input,"Testing/input.txt");
	strcpy(output,"Testing/output.txt");

	//Opening ../Concept Dir to read it files and learn concepts
	dir=opendir(ConceptDir);
	if(dir!=NULL)
	{
		while((dent=readdir(dir))!=NULL)
		{
			cnarr[cncount].count=0;
			strcpy(cnarr[cncount].concept,dent->d_name);
			temp1=strstr(cnarr[cncount].concept,".txt");
			if(temp1==NULL)
			{
				continue;
			}
			sprintf(file,"%s/%s",ConceptDir,cnarr[cncount].concept);
			fp=fopen(file,"r");
			temp1=strstr(cnarr[cncount].concept,"_");
			*temp1='\0';
			if(fp!=NULL)
			{
				while(fgets(buffer,500,fp)!=NULL)
				{
					temp1=strstr(buffer,"\n");
					if(temp1!=NULL)
						*temp1='\0';
					strcpy(cnarr[cncount].synonms[cnarr[cncount].count],buffer);
					cnarr[cncount].count++;
				}
				fclose(fp);
			}
			else
			{
				printf("Error Opening file %s\n",file);
				exit(0);
			}
			cncount++;
		}
	}
	else
	{
		printf("Error Opening Directory %s\n",ConceptDir);
		exit(0);
	}
	closedir(dir);


	//Opening ../PlaceHolder/PlaceHolderDetail.txt to learn placeholders and look its corresponding file if it needs FileLookup
	fp=fopen(PlaceHolderFl,"r");
	if(fp!=NULL)
	{
		while(fgets(buffer,500,fp)!=NULL)
		{
			temp1=strstr(buffer,"\n");
			if(temp1!=NULL)
				*temp1='\0';
			temp1=buffer;
			temp1=strstr(temp1,"<")+1;
			temp2=strstr(temp1,">");
			*temp2='\0';
			strcpy(pharr[phcount].placeholder,temp1);
			temp1=temp2+1;
			temp1=strstr(temp1,"\t")+1;
			temp2=strstr(temp1,"\t");
			*temp2='\0';
			if(strcmp(temp1,"FileLookup")==0)
				pharr[phcount].type=0;
			else if(strcmp(temp1,"Codded")==0)
			{
				pharr[phcount].type=1;
				phcount++;
				continue;
			}
			else if(strcmp(temp1,"openPhrase")==0)
			{
				pharr[phcount].type=2;
				phcount++;
				continue;
			}
			temp1=temp2+1;
			sprintf(file,"%s/%s","PlaceHolder",temp1);
			FILE *f=fopen(file,"r");
			if(f!=NULL)
			{
				while(fgets(buffer,500,f)!=NULL)
				{
					temp1=strstr(buffer,"\n");
					if(temp1!=NULL)
						*temp1='\0';
					strcpy(pharr[phcount].contents[pharr[phcount].count],buffer);
					pharr[phcount].count++;
				}
				fclose(f);
			}
			else
			{
				printf("Error Opening file %s\n",file);
				exit(0);
			}
			phcount++;
		}
		fclose(fp);
	}
	else
	{
		printf("Error Opening file %s\n",PlaceHolderFl);
		exit(0);
	}

	//Opening ../Grammar Dir to learn command and its definitons(placeholders and concepts)
	dir=opendir(GrammarDir);
	if(dir!=NULL)
	{
		while((dent=readdir(dir))!=NULL)
		{
			grarr[grcount].cncount=0;
			grarr[grcount].phcount=0;
			grarr[grcount].sset=0;
			bzero(&grarr[grcount].superset,sizeof(grarr[grcount].superset));
			grarr[grcount].ophrase=0;
			strcpy(grarr[grcount].command,dent->d_name);
			temp1=strstr(grarr[grcount].command,".txt");
			if(temp1==NULL)
				continue;
			*temp1='\0';
			sprintf(file,"%s/%s.txt",GrammarDir,grarr[grcount].command);
			fp=fopen(file,"r");
			if(fp!=NULL)
			{
				while(fgets(buffer,500,fp)!=NULL)
				{
					temp1=buffer;
					bzero(&grarr[grcount].concept[grarr[grcount].cncount],300);
					bzero(&grarr[grcount].placeholder[grarr[grcount].phcount],300);
					strcpy(grarr[grcount].syntax[grarr[grcount].phcount],buffer);
					while((strstr(temp1,"{")!=NULL)||(strstr(temp1,"<")!=NULL))
					{
						if(strstr(temp1,"{")==NULL)
							opt=0;
						else if(strstr(temp1,"<")==NULL)
							opt=1;
						else if(strstr(temp1,"{")<strstr(temp1,"<"))
							opt=1;
						else 
							opt=0;

						if(opt)
						{
							temp1=strstr(temp1,"{")+1;
							temp2=strstr(temp1,"_"); 
							*temp2='\0';
							//Ignoring please concept because client is not going to be respectable all the time
							if(strstr(temp1,"please")==NULL)
							{
								sprintf(grarr[grcount].concept[grarr[grcount].cncount]+strlen(grarr[grcount].concept[grarr[grcount].cncount]),"%s ",temp1);
							}
							temp1=temp2+1;
						}
						else
						{
							temp1=strstr(temp1,"<")+1;
							temp2=strstr(temp1,">");
							*temp2='\0';
							sprintf(grarr[grcount].placeholder[grarr[grcount].phcount]+strlen(grarr[grcount].placeholder[grarr[grcount].phcount]),"%s ",temp1);
							for(int i=0;i<phcount;i++)
							{
								if(pharr[i].type!=2)
									continue;
								if(strcmp(pharr[i].placeholder,temp1)==0)
								{
									grarr[grcount].ophrase=1;
									break;
								}
							}
							temp1=temp2+1;
						}
					}
					grarr[grcount].cncount++;
					grarr[grcount].phcount++;
				}
			}
			else
			{
				printf("Error Opening file %s\n",file);
				exit(0);
			}
			grcount++;
		}
	}
	else
	{
		printf("Error Opening Directory %s\n",GrammarDir);
		exit(0);
	}
	closedir(dir);

	for(int i=0;i<grcount;i++)
	{
		for(int j=0;j<grcount;j++)
		{
			if(i==j)
				continue;
			else
			{
				if(strcasestr(grarr[j].command,grarr[i].command)!=NULL) 
				{
					grarr[i].sset=1;
					strcpy(grarr[i].superset,grarr[j].command);
				}
			}
		}
	}


	//Processing Input Files to identify PlaceHolders and Concepts it have
	fp=fopen(input,"r"); 
	if(fp!=NULL)
	{
		while(fgets(buffer,500,fp)!=NULL)
		{
			bzero(&inarr[incount].concept,300);
			bzero(&inarr[incount].placeholder,300);
			bzero(&inarr[incount].content,300);
			temp1=strstr(buffer,"\n");
			if(temp1!=NULL)
				*temp1='\0';
			if(strcasestr(buffer,"Case#")!=NULL) //Skiping out the commented lines
				continue;
			strcpy(inarr[incount].input,buffer);
			for(int i=0;i<cncount;i++)
			{
				for(int j=0;j<cnarr[i].count;j++)
				{
					if(strcasestr(buffer,cnarr[i].synonms[j])!=NULL)
					{
						sprintf(inarr[incount].concept+strlen(inarr[incount].concept),"%s ",cnarr[i].concept);
						break;
					}
				}
			}
			for(int i=0;i<phcount;i++)
			{
				for(int j=0;j<pharr[i].count;j++)
				{
					if(pharr[i].type!=0)
						continue;
					if(strcasestr(buffer,pharr[i].contents[j])!=NULL)
					{
						sprintf(inarr[incount].placeholder+strlen(inarr[incount].placeholder),"%s ",pharr[i].placeholder);
						sprintf(inarr[incount].content+strlen(inarr[incount].content),"\"%s\" ",pharr[i].contents[j]);
					}
					else if(strstr(pharr[i].contents[j]," ")!=NULL)
					{
						strcpy(tempbuffer,pharr[i].contents[j]);
						temp1=tempbuffer;
						while(1)
						{
							temp2=strstr(temp1," ");
							if(temp2!=NULL)
								*temp2='\0';
							if(strcasestr(buffer,temp1)!=NULL)
							{
								sprintf(inarr[incount].placeholder+strlen(inarr[incount].placeholder),"%s ",pharr[i].placeholder);
								sprintf(inarr[incount].content+strlen(inarr[incount].content),"\"%s\" ",temp1);
							}
							if(temp2==NULL)
								break;
							else
								temp1=temp2+1;
						}
					}

				}

			}

			//HardCoding  to find dateTime Time Day Date 
			opt=0;
			if(strstr(buffer,"a.m")!=NULL)
				opt=1;
			else if(strstr(buffer,"p.m")!=NULL)
				opt=1;
			if(opt==0)
			{
				if((strcasestr(buffer,"o'")!=NULL))
					opt=1;
			}
			if(opt==1)
			{
				char *p[4];
				if(strcasestr(buffer,"today")!=NULL)
					p[0]=strcasestr(buffer,"today");
				else if(strcasestr(buffer,"tomorrow")!=NULL)
					p[0]=strcasestr(buffer,"tomorrow");
				else 
					p[0]=NULL;
				if(strcasestr(buffer,"january")!=NULL)
					p[1]=strcasestr(buffer,"january");
				else if(strcasestr(buffer,"February")!=NULL)
					p[1]=strcasestr(buffer,"February");
				else if(strcasestr(buffer,"march")!=NULL)
					p[1]=strcasestr(buffer,"march");
				else if(strcasestr(buffer,"april")!=NULL)
					p[1]=strcasestr(buffer,"april");
				else if(strcasestr(buffer,"may")!=NULL)
					p[1]=strcasestr(buffer,"may");
				else if(strcasestr(buffer,"june")!=NULL)
					p[1]=strcasestr(buffer,"june");
				else if(strcasestr(buffer,"july")!=NULL)
					p[1]=strcasestr(buffer,"july");
				else if(strcasestr(buffer,"august")!=NULL)
					p[1]=strcasestr(buffer,"august");
				else if(strcasestr(buffer,"september")!=NULL)
					p[1]=strcasestr(buffer,"september");
				else if(strcasestr(buffer,"october")!=NULL)
					p[1]=strcasestr(buffer,"october");
				else if(strcasestr(buffer,"november")!=NULL)
					p[1]=strcasestr(buffer,"november");
				else if(strcasestr(buffer,"december")!=NULL)
					p[1]=strcasestr(buffer,"decmeber");
				else
					p[1]=NULL;
				if(strcasestr(buffer,"monday")!=NULL)
					p[2]=strcasestr(buffer,"monday");
				else if(strcasestr(buffer,"tuesday")!=NULL)
					p[2]=strcasestr(buffer,"tuesday");
				else if(strcasestr(buffer,"wednesday")!=NULL)
					p[2]=strcasestr(buffer,"wednesday"); 
				else if(strcasestr(buffer,"thursday")!=NULL)
					p[2]=strcasestr(buffer,"thursday");
				else if(strcasestr(buffer,"friday")!=NULL)
					p[2]=strcasestr(buffer,"friday");
				else if(strcasestr(buffer,"saturday")!=NULL)
					p[2]=strcasestr(buffer,"saturday");
				else if(strcasestr(buffer,"sunday")!=NULL)
					p[2]=strcasestr(buffer,"sunday");
				else 
					p[2]=NULL;
				if(strcasestr(buffer,"a.m")!=NULL)
					p[3]=strcasestr(buffer,"a.m");
				else if(strcasestr(buffer,"p.m")!=NULL)
					p[3]=strcasestr(buffer,"p.m");
				else if(strcasestr(buffer,"o'")!=NULL)
					p[3]=strcasestr(buffer,"o'");
				else 
					p[3]=NULL;
				char *pmax=p[0];
				for(int i=0;i<4;i++)
				{
					if(pmax<p[i])
						pmax=p[i];
				}
				char *pmin=p[0];
				for(int i=0;i<4;i++)
				{
					if(pmin==NULL)
						pmin=p[i];
					else if(p[i]!=NULL&&pmin>p[i])
						pmin=p[i];
				}
				if(pmin==p[3])
				{
					temp1=p[3]-1;
					char c=' '; 
					while(1)
					{
						if(*temp1==' ')
						{
							if(c!=' ')
								break;
						}
						else
							c=*temp1;
						temp1--;
					}
					pmin=temp1+1;
				}
				if(pmin==pmax)
				{
					temp1=strstr(pmin," ");
					if(temp1!=NULL)
						*temp1='\0';
					sprintf(inarr[incount].content+strlen(inarr[incount].content),"\"%s\" ",pmin);
				}
				else
				{
					temp1=strstr(pmax," ");
					if(temp1!=NULL)
						*temp1='\0';
					sprintf(inarr[incount].content+strlen(inarr[incount].content),"\"%s\" ",pmin);
				}
				sprintf(inarr[incount].placeholder+strlen(inarr[incount].placeholder),"dateTime ");
			}
			else
			{
				if((strcasestr(buffer,"january")!=NULL)||(strcasestr(buffer,"february")!=NULL)||(strcasestr(buffer,"march")!=NULL)||(strcasestr(buffer,"april")!=NULL)||(strcasestr(buffer,"may")!=NULL)||(strcasestr(buffer,"june")!=NULL)||(strcasestr(buffer,"july")!=NULL)||(strcasestr(buffer,"august")!=NULL)||(strcasestr(buffer,"september")!=NULL)||(strcasestr(buffer,"october")!=NULL)||(strcasestr(buffer,"november")!=NULL)||(strcasestr(buffer,"december")!=NULL))
					sprintf(inarr[incount].placeholder+strlen(inarr[incount].placeholder),"date ");
				else if((strcasestr(buffer,"monday")!=NULL)||(strcasestr(buffer,"tuesday")!=NULL)||(strcasestr(buffer,"wednesday")!=NULL)||(strcasestr(buffer,"thursday")!=NULL)||(strcasestr(buffer,"friday")!=NULL)||(strcasestr(buffer,"saturday")!=NULL)||(strcasestr(buffer,"sunday")!=NULL)||(strcasestr(buffer,"today")!=NULL)||(strcasestr(buffer,"tomorrow")!=NULL))
					sprintf(inarr[incount].placeholder+strlen(inarr[incount].placeholder),"day ");
			}
			incount++;
		}
	}
	else
	{
		printf("Error Opening Input File %s\n",input);
		exit(0);
	}
	
	//Comapring Proccesd Input File with Grammar Rules to predict the category it belongs to
	int record,max;
	char cnbuffer[300],phbuffer[300],command[100],tempcommand[100],arpr[500];
	float chance[grcount],cmweight=1,cnweight=1,phweight=1,cmcont,cncont,phcont,total,pass,weight,tempweight,inweight;

	//Artciles and Preposition file to eliminate prepositons from command name to increase accuarcy of predictor
	fp=fopen("./ArPr.txt","r");
	if(fp==NULL)
	{
		printf("Unable to open ArtcilesandPreposition File\n");
		exit(0);
	}
	fgets(arpr,500,fp);
	for(int i=0;i<incount;i++)
	{
		inweight=0;
		bzero(&chance,sizeof(chance));
		for(int j=0;j<grcount;j++)
		{
			bzero(&command,sizeof(command));
			record=0;
			for(int l=0;l<strlen(grarr[j].command);l++)
			{
				if(grarr[j].command[l]>=65 && grarr[j].command[l]<=90)
				{
					if(*command=='\0')
					{
						command[record]=grarr[j].command[l];
						record++;
					}
					else
					{
						command[record]=' ';
						record++;
						command[record]=grarr[j].command[l];
						record++;
					}
				}
				else
				{
					command[record]=grarr[j].command[l];
					record++;
				}
			}
			command[record]=' ';
			weight=0;
			for(int k=0;k<grarr[j].cncount;k++)
			{
				tempweight=0;
				total=0;
				pass=0;
				strcpy(cnbuffer,grarr[j].concept[k]);
				temp1=cnbuffer;
				while(*temp1!='\0')
				{
					total++;
					temp2=strstr(temp1," "); 
					*temp2='\0';
					if(strcasestr(inarr[i].concept,temp1)!=NULL)
						pass++;
					temp1=temp2+1;
				}
				if(total!=0)
					cncont=pass/total;
				else 
					cncont=0;
				total=0;
				pass=0;
				strcpy(phbuffer,grarr[j].placeholder[k]);
				temp1=phbuffer;
				while(*temp1!='\0')
				{
					total++;
					temp2=strstr(temp1," ");
					*temp2='\0';
					if(strcasestr(inarr[i].placeholder,temp1)!=NULL)
						pass++;
					temp1=temp2+1;
				}
				if(total!=0)
					phcont=pass/total;
				else
					phcont=0;
				total=0;
				pass=0;
				strcpy(tempcommand,command);
				temp1=tempcommand;
				while(*temp1!='\0')
				{
					temp2=strstr(temp1," ");
					*temp2='\0';
					if(strcasestr(arpr,temp1)==NULL)
					{
						total++;
						if(strcasestr(inarr[i].input,temp1)!=NULL)
							pass++;
					}
					temp1=temp2+1;
				}
				if(total!=0)
					cmcont=pass/total;
				else
					cmcont=0;
				tempweight=(cncont*cnweight)+(cmcont*cmweight)+(phcont*phweight);
				if(tempweight>weight)
				{
					weight=tempweight;
					if(tempweight>inweight)
					{
						inweight=tempweight;
						inarr[i].synmatch=k;
						inarr[i].grmatch=j;
					}
				}
			}
			chance[j]=weight;
		}
		max=0;
		for(int j=0;j<grcount;j++)
		{
			if(chance[j]>chance[max])
				max=j;
		}
		if(grarr[max].sset==1)
		{
			bzero(&command,sizeof(command));
			record=0;
			temp1=grarr[max].superset+strlen(grarr[max].command);
			for(int l=0;l<strlen(temp1);l++)
			{
				if(temp1[l]>=65 && temp1[l]<=90)
				{
					if(*command=='\0')
					{
						command[record]=temp1[l];
						record++;
					}
					else
					{
						command[record]=' ';
						record++;
						command[record]=temp1[l];
						record++;
					}
				}
				else
				{
					command[record]=temp1[l];
					record++;
				}
			}
			command[record]=' ';
			total=0;
			pass=0;
			temp1=command;
			while(*temp1!='\0')
			{
				temp2=strstr(temp1," ");
				*temp2='\0';
				if(strcasestr(arpr,temp1)==NULL)
				{
					total++;
					if((strcasestr(inarr[i].placeholder,temp1)!=NULL)||(strcasestr(inarr[i].concept,temp1)!=NULL))
						pass++;
				}
				temp1=temp2+1;
			}
			if(total!=0)
				cmcont=pass/total;
			else
				cmcont=0;
			if(cmcont>=0.5)
			{
				strcpy(inarr[i].command,grarr[max].superset);
				continue;
			}
		}
		strcpy(inarr[i].command,grarr[max].command);
		if(grarr[max].ophrase==1)
		{
			if(strcmp(inarr[i].command,"RenameEvent")==0)
			{
				SrcEventNameOpenPhrase(&inarr[i]);
				TgtEventNameOpenPhrase(&inarr[i]);

			}
			else if(strcmp(inarr[i].command,"CreateNoteWithNameAndContent")==0)
			{
				NoteNameOpenPhrase(&inarr[i]);
				NoteContentOpenPhrase(&inarr[i]);
			}
			else if(strcmp(inarr[i].command,"CreateAlarmByName")==0)
			{
				AlarmNameOpenPhrase(&inarr[i]);
			}
		}
	}

	//Writing The results output file
	fp=fopen(output,"w");
	if(fp!=NULL)
	{
		for(int i=0;i<incount;i++)
		{
			fputs(inarr[i].command,fp);
			fputc('\n',fp);
			temp1=inarr[i].content;
			temp3=inarr[i].placeholder;
			while(*temp1!='\0')
			{
				temp2=strstr(temp1+1,"\"");
				*(temp2+1)='\0';
				temp4=strstr(temp3," ");
				*temp4='\0';
				fputc('<',fp);
				fputs(temp3,fp);
				fputs(">: ",fp);
				fputs(temp1,fp);
				fputc('\n',fp);
				temp1=temp2+2;
				temp3=temp4+1;
			}


		}
	}
	else
	{ 
		printf("Error Opening Output File %s , Displaying result on stdout\n",output);
		for(int i=0;i<incount;i++)
		{
			printf("%s\n",inarr[i].command);
		}
	}
	fclose(fp);
	return 0;
}
