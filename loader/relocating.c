#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char bitmask[15];
char bit[12]={0};


void convert(char bitmask[15])
{
        int len=strlen(bitmask);
        strcpy(bit,"");
        for(int i=0;i<len;i++)
        {
                switch(bitmask[i])
                {
                        case '0':
                        {
                                strcat(bit,"0");
                                break;
                        }
                        case '1':
                        {
                                strcat(bit,"1");
                                break;
                        }
                        case '2':
                        {
                                strcat(bit,"10");
                                break;
                        }
                        case '3':
                        {
                                strcat(bit,"11");
                                break;
                        }
                        case '4':
                        {
                                strcat(bit,"100");
                                break;
                        }
                        case '5':
                        {
                                strcat(bit,"101");
                                break;
                        }
                        case '6':
                        {
                                strcat(bit,"110");
                                break;
                        }
                        case '7':
                        {
                                strcat(bit,"111");
                                break;
                        }
                        case '8':
                        {
                                strcat(bit,"1000");
                                break;
                        }
                        case '9':
                        {
                                strcat(bit,"1001");
                                break;
                        }
                        case 'A':
                        {
                                strcat(bit,"1010");
                                break;
                        }
                        case 'B':
                        {
                                strcat(bit,"1011");
                                break;
                        }
                        case 'C':
                        {
                                strcat(bit,"1100");
                                break;
                        }
                        case 'D':
                        {
                                strcat(bit,"1101");
                                break;
                        }
                        case 'E':
                        {
                                strcat(bit,"1110");
                                break;
                        }
                        case 'F':
                        {
                                strcat(bit,"1111");
                                break;
                        }
                        default:
                        break;

                }
        }
}


void main()
{
        FILE *fp,*fp2;
        char record[20],progname[20],addy[20],length[20],tlen[20];
        int start,addr,actualaddr,symadd,opcode;
        fp=fopen("objectcode.txt","r");
        fp2=fopen("reloc.txt","w");

        printf("Enter Starting Address\n");
        scanf("%x",&start);

        fscanf(fp,"%s",record);
        while(strcmp(record,"E")!=0)
        {
                if(strcmp(record,"H")==0)
                {
                        fscanf(fp,"%s",progname);
                        fscanf(fp,"%s",addy);
                        fscanf(fp,"%s",length);
                        fscanf(fp,"%s",record);
                }
                if(strcmp(record,"T")==0)
                {
                        fscanf(fp,"%x",&addr);
                        fscanf(fp,"%s",tlen);
                        fscanf(fp,"%s",bitmask);

                        addr=addr+start;
                        convert(bitmask);
                        int leny=strlen(bit);
                        if(leny>=11)
                        {
                                leny=10;
                        }
                        for(int i=0;i<leny;i++)
                        {
                                fscanf(fp,"%x",&opcode);
                                fscanf(fp,"%x",&symadd);
                                char bi=bit[i];
                                if(bi==0)
                                {
                                        actualaddr=symadd;
                                }
                                else
                                {
                                        actualaddr=symadd+start;
                                }
                                fprintf(fp2,"%x\t%x\t%x\n",addr,opcode,actualaddr);
                                addr=addr+3;
                        }
                }
                fscanf(fp,"%s",record);

        }
        fclose(fp);
        fclose(fp2);
}
