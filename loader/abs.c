#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main()
{
        FILE *fp;
        fp = fopen("objectcode.txt","r");
        if (!fp) 
            {
                        printf("Error: objectcode.txt not found.\n");
                        exit(0);
            }
        char recordtype[2],progname[50],startaddr[20],length[20],line[100];
        fscanf(fp,"%s",recordtype);
        if(recordtype[0] =='H')
        {
                fscanf(fp,"%s %s %s",progname,startaddr,length);
                printf("Program Name is %s\n",progname);
                printf("Starting Address is %s\n",startaddr);
                printf("Total Length %s\n",length);

        }
        while(fscanf(fp,"%s",recordtype)!=EOF)
        {

                if(recordtype[0]=='T')
                {
                        fscanf(fp,"%s %s",startaddr,length);
                        printf("Text record Starting at %s.Its Length is%s\n",startaddr,length);
                        fgets(line,sizeof(line),fp);
                        int addr=(int)strtol(startaddr,NULL,16);
                        char *tok=strtok(line," \n");
                        while(tok!=NULL)
                        {
                                printf("%04X\t%s\n",addr,tok);
                                addr=addr+(strlen(tok)/2);
                                tok=strtok(NULL," \n");
                        }
                }
                else if(recordtype[0]=='E')
                {
                        fscanf(fp,"%s",startaddr);
                        printf("Execution starts at %s\n",startaddr);
                }

        }
        fclose(fp);
}


