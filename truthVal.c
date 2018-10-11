//TEST
//(P<=>Q)<=>((P^Q)V((7P)^(7Q))) 
//((PVQ)^(P->R)^(Q->R))->R 
//(P->Q)^((7R)V(7S))^(PVR)^((7Q)->S)
//((P->Q)^((7R)V(7S))^(PVR))->((7Q)->S)
#include <stdio.h>
#include <stdlib.h>
typedef struct stack
{
	int w[100],top;
}stk;
void Push(stk *stack,int data)
{
	stack->w[++(stack->top)]=data;
}
int Pop(stk *stack)
{
		return stack->w[(stack->top)--];
}

int Convert(char *s,char *exp,char *var)
{
	char c;
	int i,j,k=0,l;
	exp[0]='(';
	for(i=1,j=0;s[j]!='\0';i++,j++)
	{
		c=s[j];
		switch(c)
		{
			case '7': exp[i]='2'; break;
			case '^': exp[i]='3'; break;
			case 'V':
			case 'v': exp[i]='4'; break;
			case '-':
				if(s[++j]=='>')
					exp[i]='5';
				else return -1;
				break;
			case '<':
				if(s[++j]=='='&&s[++j]=='>')
					exp[i]='6';
				else return -1;
				break;
			case '(':
			case ')':
			case 'T':
			case 'F':
				exp[i]=c;
				break;
			default:
				if(c>='A'&&c<='Z')
				{
					for(l=0;l<k && var[l]!=c;l++);
					if(l==k)
						var[k++]=c;

					exp[i]=c;
				}
				else return -1;
		}
	}
	exp[i++]=')';
	exp[i]='\0';
	//printf("exp %s\n", exp);
	return k;
}

int Pow2(int n)
{
	int i,pow=1;
	for(i=0;i<n;i++,pow*=2);
	return pow;
}
int** CreateTable(int vc)
{
	int row=Pow2(vc),n,i,j;
	int** table=(int**)malloc(vc*sizeof(int*));
	for(i=0;i<vc;i++)
		table[i]=(int*)malloc(row*sizeof(int));
	for(i=0;i<row;i++)
	{
		n=i;
		for(j=vc-1;j>=0;j--)
		{
			if(n>0)
			{
				table[j][i]=n%2;
				n/=2;
			}
			else
				table[j][i]=0;
		}
	}
	return table;
}

int Val(int i,char c,int vc,int **table,char *var)
{
	int j;
	if(c==0||c==1)
		return c;
	for(j=0;j<vc;j++)
	{
		if(var[j]==c)
			return table[j][i];
	}
}

int CheckResult(int *result,int row)
{
	int t=1,f=0,i;
	for(i=0;i<row;i++)
	{
		t=t&result[i];
		f=f|result[i];
	}
	return (t)?1:((f)?2:0);
}

int Evaluate(char *exp,char *var,int vc)
{
	int **table=CreateTable(vc);
	int i,j,row=Pow2(vc),a,b;
	int *result=(int*)malloc(row*sizeof(int));
	stk oprnd,oprtr;
	oprtr.top=oprnd.top=-1;
	char c,popc;

	for(i=0;i<row;i++)
	{
		for(j=0;j<vc;j++)
			printf(" %c |",(table[j][i])?'T':'F');
		for(j=0;exp[j]!='\0';j++)
		{
			c=exp[j];
			switch(c)
			{
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '(':
					Push(&oprtr,c);
					break;
				case ')':
					while((popc=Pop(&oprtr))!='(')
					{
						if(popc=='2')
							Push(&oprnd,!Val(i,Pop(&oprnd),vc,table,var));
						else
						{
							b=Val(i,Pop(&oprnd),vc,table,var);
							a=Val(i,Pop(&oprnd),vc,table,var);
							//printf("a=%d  b=%d\n",a,b );
							if(popc=='3') Push(&oprnd,a&b);
							else if(popc=='4') Push(&oprnd,a|b);
							else if(popc=='5') Push(&oprnd,(!a)|b);
							else if(popc=='6') Push(&oprnd,((!a)|b)&((!b)|a));
						}
					}
					break;
				case 'T': Push(&oprnd,1); break;
				case 'F': Push(&oprnd,0); break;
				default:
					if(c>='A'&&c<='Z')
						//Push(&oprnd,Val(i,c,vc,table,var));
						Push(&oprnd,c);
			}
		}
		result[i]=Pop(&oprnd);
		printf(" %c \n",(result[i])?'T':'F');
	}
	return CheckResult(result,row);
}

int main()
{
	char c,exp[100],s[100],var[20];
	int i,varCount;
	int re;
	printf("\nEnter\n:");
	scanf("%s",s);
	varCount=Convert(s,exp,var);
	re=Evaluate(exp,var,varCount);
	if(re==0)
		printf("\nContradiction.\n");
	else if(re==1)
		printf("\nTautology.\n");
	else
		printf("\nSatisfiable.\n");
	return 0;
}
