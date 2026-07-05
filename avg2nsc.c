#include <stdio.h>
#include <stdlib.h>

int namechg(char *s,char *d) {
	int i,j,k;
	i=0;
	j=0;
	k=0;
	while(s[i]!='\0') {
		if(i==0 && s[i]==0x81 && s[i+1]==0x79) {
			k=1;
			i++;
			i++;
		}
		else if(k==1 && s[i]==0x81 && s[i+1]==0x7a && s[i+2]=='\0') {
			k=0;
			i++;
			i++;
		}
		else if(s[i]==0x81 && s[i+1]==0x96 && s[i+2]==0x82 && s[i+3]==0x60 && s[i+4]==0x82 && s[i+5]==0x61) {
			d[j++]=0x92;
			d[j++]=0xBC;
			d[j++]=0x96;
			d[j++]=0xE7;
			i++;
			i++;
			i++;
			i++;
			i++;
			i++;
		}
		else if(s[i]==0x81 && s[i+1]==0x96 && s[i+2]==0x82 && s[i+3]==0x61 && s[i+4]==0x82 && s[i+5]==0x61) {
			d[j++]=0x90;
			d[j++]=0x5E;
			d[j++]=0x8F;
			d[j++]=0x83;
			i++;
			i++;
			i++;
			i++;
			i++;
			i++;
		}
		else if(s[i]==0x81 && s[i+1]==0x96 && s[i+2]==0x82 && s[i+3]==0x79 && s[i+4]==0x82 && s[i+5]==0x61) {
			d[j++]=0x8F;
			d[j++]=0x7E;
			d[j++]=0x8E;
			d[j++]=0x71;
			i++;
			i++;
			i++;
			i++;
			i++;
			i++;
		}
		else {
			d[j++]=s[i++];
		}
	}
	d[j]=s[i];
}

main(int argc, char *argv[]) {
	FILE *fp;
	char b[4];
	int i;
	long j;
	char x[16384];
	char y[16384];

	if(argc<3) {
		printf("usage: avg2nsc <file> <address>\n");
		return 0;
	}
	fp=fopen(argv[1],"rb");
	printf("*define\n");
	printf("effect 2,10,500\n");
	printf("game\n");
	printf("\n");
	printf("*start\n");
	printf("setwindow 39,364,23,4,24,24,0,0,20,1,1,#999999,11,360,626,463\n");
	fseek(fp,strtol(argv[2],NULL,16),SEEK_SET);
	while(1) {
		if(fread(b,1,1,fp)==0) return 0;
		if(b[0]==0x01) {
			printf("\\\n");
		}
		else if(b[0]==0x02) {
			if(fread(b,1,1,fp)==0) return 0;
			if(b[0]==0x03) {
				printf("\n");
			}
			else {
				printf("; error 02 %02X\n",b[0]);
			}
		}
		else if(b[0]==0x0b) {
			if(fread(b,1,1,fp)==0) return 0;
			if(b[0]==0x05) {
				i=0;
				while(1) {
					if(fread(b,1,1,fp)==0) return 0;
					x[i]=b[0];
					if(b[0]==0x00) break;
					i++;
				}
				if(fread(b,1,1,fp)==0) return 0;
				if(b[0]==0x10) {
					printf("bg \"BMP\\%s.BMP\",2\n",x);
				}
				else if(b[0]==0x1b) {
					printf("bg \"BMP\\%s.BMP\",2\n",x);
				}
				else {
					printf("; error 0B 05 \"%s\" %02X\n",x,b[0]);
				}
			}
			else if(b[0]==0x22) {
				if(fread(b,1,1,fp)==0) return 0;
				if(b[0]==0x01) {
					i=0;
					while(1) {
						if(fread(b,1,1,fp)==0) return 0;
						x[i]=b[0];
						if(b[0]==0x00) break;
						i++;
					}
					if(fread(b,1,2,fp)<2) return 0;
					if(b[0]==0x10 && b[1]==0x01) {
						i=0;
						while(1) {
							if(fread(b,1,1,fp)==0) return 0;
							y[i]=b[0];
							if(b[0]==0x00) break;
							i++;
						}
						printf("bg \"BMP\\%s.BMP\",0\n",x);
						printf("ld c,\":l;BMP\\%s.BMP\",2\n",y);
					}
					else {
						printf("; error 0B 22 01 \"%s\" %02X %02X\n",x,b[0],b[1]);
					}
				}
				else {
					printf("; error 0B 22 %02X\n",b[0]);
				}
			}
			else {
				printf("; error 0B %02X\n",b[0]);
			}
		}
		else if(b[0]==0x0e) {
			if(fread(b,1,1,fp)==0) return 0;
			if(b[0]==0x01) {
				i=0;
				while(1) {
					if(fread(b,1,1,fp)==0) return 0;
					x[i]=b[0];
					if(b[0]==0x00) break;
					i++;
				}
				printf("bgm \"WAV\\%s.WAV\"\n",x);
			}
			else if(b[0]==0x10) {
				printf("bgmfadeout 500\n");
			}
			else if(b[0]==0x11) {
				printf("stop\n");
			}
			else if(b[0]==0x21) {
				if(fread(b,1,4,fp)<4) return 0;
				j=((b[3]*256+b[2])*256+b[1])*16+(b[0] & 0x0f);
				printf("dwave 0,\"WAV\\Z%08.ld.WAV\"\n",j);
			}
			else if(b[0]==0x30) {
				i=0;
				while(1) {
					if(fread(b,1,1,fp)==0) return 0;
					x[i]=b[0];
					if(b[0]==0x00) break;
					i++;
				}
				printf("dwave 1,\"WAV\\%s.WAV\"\n",x);
			}
			else {
				printf("; error 0E %02X\n",b[0]);
			}
		}
		else if(b[0]==0x13) {
			if(fread(b,1,2,fp)<2) return 0;
			if(b[0]==0x01 && b[1]==0x11) {
				printf("bg black,2\n");
			}
			else if(b[0]==0x10 && b[1]==0x10) {
				printf("bg white,1\n");
			}
			else {
				printf("; error 13 %02X %02X\n",b[0],b[1]);
			}
		}
		else if(b[0]==0x17) {
			if(fread(b,1,2,fp)<2) return 0;
			if(b[0]==0x01 && b[1]==0x17) {
				printf("quake 16,500\n");
			}
			else {
				printf("; error 17 %02X %02X\n",b[0],b[1]);
			}
		}
		else if(b[0]==0x19) {
			if(fread(b,1,3,fp)<3) return 0;
			if(b[0]==0x01 && b[1]==0x20 && b[2]==0x7d) {
				printf("wait 500\n");
			}
			else {
				printf("; error 19 %02X %02X %02X\n",b[0],b[1],b[2]);
			}
		}
/*
		else if(b[0]==0x58) {
			if(fread(b,1,1,fp)==0) return 0;
			printf("; error 58 %02X\n",b[0]);
		}
*/
		else if(b[0]==0xff) {
			if(fread(b,1,4,fp)<4) return 0;
			if(b[3]==0x00) {
				i=0;
				j=0;
			}
			else {
				x[0]=b[0];
				x[1]=b[1];
				x[2]=b[2];
				x[3]=b[3];
				i=4;
				j=1;
			}
			while(1) {
				if(fread(b,1,1,fp)==0) return 0;
				x[i]=b[0];
				if(b[0]==0x00) break;
				i++;
			}
			namechg(x,y);
			if(j==0) {
				printf("%s",y);
			}
			else {
				printf("%s\\\n",y);
			}
		}
		else {
			printf("; error %02X\n",b[0]);
		}
	}
}
