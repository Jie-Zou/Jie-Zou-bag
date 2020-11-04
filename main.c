#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
//�涨��ѧ��idֻ�������������ɼ�ֻ����[0,100]�����⸡��������������ֻ������ĸ�Ϳո��Ҳ����Կո�ͷ�������Կո��β��
typedef struct information
{
    long int id;
    char name[20];
    float scores[8];/*���Ʒ���*/
    int rank;
}INFO;
typedef struct ISCORE/*�ṹ�������ýṹ����Ϊ���������ѧ����Ϣ��Ҫд���ĵ���ʱ�򷽱㽫��Ϣ�ṹ������д��*/
{
    struct ISCORE *up;
    INFO info;
    struct ISCORE *next;
}ISC;
//�������ܣ����ѧ����Ϣ����β����ӵ������Լ������У���������ͷָ��
ISC *Add_record(long int ids[],float scores[][8],char names[][20],int number,int count,int sub_num,char sub_name[][20],ISC *table);
//�������ܣ�/*���з�ʽ��id-name-sub_score(����20)-[rank](����5)-average(����10)-sum(����5)-typeΪ1��ʾrank;������ʾ
void show_title(char sub_names[][20],int sub_num,int type);
//�������ܣ���ʾĿ��ṹ������ݣ����ڰ���id������������
void show_target(ISC *temp1,char sub_names[][20],int sub_num,int type);
//�������ܣ���ʾ���г�Ա������typeΪ1ʱ������ʾ����������Ϊ1ʱ��������ʾ������
void show_all(ISC *table,char sub_names[][20],int sub_num,int type);
//�������ܣ�����ƽ���֣�����ӡ����񣬵��ǲ��ᱣ��
void cal_ave(int sub_num,char sub_names[][20],float scores[][8],int count,float sum_list[],float aver_list[]);
//�������ܣ����ո����ĺ�����ȷ���ж�������Ȼ��������������
ISC *sort_table(ISC *table,int (*func)(ISC *pr,ISC *p));
//�������ܣ�����ǰ����˳��չʾ�������type����1������ʾ������������ʾ
void show_table(ISC *table,char sub_names[][20],int sub_num,int type);
//��ָ�������жϣ�������ͬ��ʱ��rank���䣬�������ӣ�������ͬʱ�����ճ��ֵ��Ⱥ�˳����������
ISC *rank_table(ISC *table, int(*func)(ISC *pr,ISC*p));
//��������:����һ���͵�ǰ������ȫ�����������
ISC *get_reverse_table(ISC *table,ISC *reverse_table);
//��������:�ͷŵ�ĳһָ������������������Ķ�̬�ڴ�
void Free_table(ISC *table);
//��������:��������,����id����ָ��ѧ��������id���ɼ�������
void find_id(ISC *table,char sub_names[][20],int sub_num);
//�������ܣ��������ݣ���ӡ�����
void analysis(float scores[][8],int count,char sub_names[][20],int sub_num);
//��������:������������ѧ��
void find_name(ISC *table,char sub_names[][20],int sub_sum);

int judge_by_id(ISC *pr,ISC *p);
int judge_by_score(ISC *pr,ISC *p);/*���尴�ճɼ��ж�pr��p�Ĺ�ϵ�����pr�ɼ���pС������1����pһ��������2����p�󣬷���3*/
int judge_by_name(ISC *pr,ISC *p);/*���尴�������ж�pr��p�Ĺ�ϵ,���pr���ֱ�pС������1�������pһ��������2����p�󣬷���3*/
int input_over(void);/*ÿ�����������ڻ��������ܼ�⵽���˻س���������ַ�����˵���зǷ����룬��ʱ��Ҫ��������,��ȷ���뷵��1���������뷵��0*/
float right_input_float(void);/*�淶���븡�����������ظ�����*/
int right_input_int(void);/*�淶�������ͣ�����������*/
char *right_input_char(void);/*�淶�����ַ����������ַ�������,����֮��ǵ��ͷţ�����*/
long int right_input_long(void);/*�淶���볤���ͣ�������*/

int count=0,block=1;
const float eof=1e-6;

int main()
{
    ISC *table=NULL,*reverse_table=NULL;
    int flag=1,number,i,check=0,sub_num=0;
    printf("number: 200320219\n");
    printf("Subject No.7 program No.7\n");

    printf("Please enter the number of students,no more than 30:");
    while(check != 1)
    {
        number=right_input_int();/*�淶�������ͣ��������жϺ���ջ�����*/
        if(number >0 && number <=30)/*�淶����������ǹ淶�����ͣ����������Ƿ���0-30�ڻ����ж�*/
        {
            check = 1;
        }
        else
        {
            printf("Wrong input,input again!\n");
        }
    }
/*�����Ŀ����*/
    printf("Please input the number of the subject(decimal only):");
    check=0;
    while(check !=1)
    {
        sub_num=right_input_int();
        if(sub_num >0 && sub_num <=6)
        {
            check=1;
        }
        else
        {
            printf("Wrong input,input again");
        }
    }

    char sub_names[sub_num][20];/*�����Ŀ��������*/
    long int ids[number];
    float scores[number][8];/*���峤��Ϊ8�����飬��������Ŀ�ã�ʣ��ƽ���ֺ��ܷ�*/
    char names[number][20];
    float sum_list[6],aver_list[6];
/*�����Ŀ���ƣ������ӡ��*/
    for(i=0;i<sub_num;i++)
    {
        printf("The No.%d subject is(alphbet and spaces supported only):",i+1);
        char *temp_sub=right_input_char();
        strcpy(sub_names[i],temp_sub);
        free(temp_sub);
        printf("The No. %d subject is %s\n",i+1,sub_names[i]);
    }

    for(i=0;i<number;i++)
    {
        ids[i]=0;
    }

    while(flag==1)
    {
        int opera;
        printf("1��Input record\n");
        printf("2. Calculate total and average score of every course\n");
        printf("3. Calculate total and average score of every student\n");
        printf("4. Sort in descending order by total score of every student\n");
        printf("5. Sort in ascending order by total score of every student\n");
        printf("6. Sort in ascending order by number\n");
        printf("7. Sort in dictionary order by name\n");
        printf("8. Search by number\n");
        printf("9. Search by name\n");
        printf("10.Statistic analysis for every course\n");
        printf("11.List record\n");
        printf("0.Exit\n");
        printf("please enter your choice:");
        opera=right_input_int();

        switch(opera)
        {
            case 1:
            {
                ISC *TEST=Add_record(ids,scores,names,number,count,sub_num,sub_names,table);/*ÿ�����ѧ�����Զ�����һ������(û��˳���)*/
                if(TEST != NULL)
                {
                    table = TEST;
                    count +=1;
                }
                Sleep(1000);
                break;
            }
        case 2:
            {
                cal_ave(sub_num,sub_names,scores,count,sum_list,aver_list);
                Sleep(1000);
                break;
            }
        case 3:
            {
                show_table(table,sub_names,sub_num,0);
                break;
            }
        case 4:/*�ִܷӸߵ��ͣ���Ҫ����*/
            {
                table=sort_table(table,judge_by_score);
                table=rank_table(table,judge_by_score);
                show_table(table,sub_names,sub_num,1);
                Sleep(1000);
                break;
            }
        case 5:/*�ִܷӵ͵��ߣ���Ҫ����*/
            {
                table=sort_table(table,judge_by_score);
                reverse_table=get_reverse_table(table,reverse_table);
                reverse_table=rank_table(reverse_table,judge_by_score);
                show_table(reverse_table,sub_names,sub_num,1);
                Sleep(1000);
                break;
            }
        case 6:/*����id���򣬲���Ҫ����*/
            {
                table=sort_table(table,judge_by_id);
                reverse_table=get_reverse_table(table,reverse_table);
                show_table(reverse_table,sub_names,sub_num,0);
                Sleep(1000);
                /*show_all(ids,scores,count);*/
                break;
            }
        case 7:/*�����������򣬲���Ҫ����*/
            {
                table=sort_table(table,judge_by_name);
                reverse_table=get_reverse_table(table,reverse_table);
                show_table(reverse_table,sub_names,sub_num,0);
                Sleep(1000);
                break;
            }
        case 8:
            {
                table=sort_table(table,judge_by_score);
                table=rank_table(table,judge_by_score);
                find_id(table,sub_names,sub_num);
                break;
            }
        case 9:
            {
                table=sort_table(table,judge_by_score);
                table=rank_table(table,judge_by_score);
                find_name(table,sub_names,sub_num);
                Sleep(1000);
                break;
            }
        case 10:
            {
                analysis(scores,count,sub_names,sub_num);
                break;
            }
        case 11:
            {
                show_all(table,sub_names,sub_num,0);
                cal_ave(sub_num,sub_names,scores,count,sum_list,aver_list);
                break;
            }
        case 0:
            {
                flag=0;
                Free_table(table);
                Free_table(reverse_table);
                printf("exit now\n");
                break;
            }
        default:
            {
                printf("Wrong input,input again!\n");
                break;
            }
        }
    }
    return 0;
}
ISC *Add_record(long int ids[],float scores[][8],char names[][20],int number,int count,int sub_num,char sub_name[][20],ISC *table)
{
    if(count == number)
    {
        printf("can't add student now\n");
        return NULL;
    }

    long int id;
    int check=0,i;
    float score;
    char name[20];

/*����id*/
    printf("Please input the student's id(decimal only and no more than 9999999999):");
    while(check != 1)
    {
        id = right_input_long();
        if(id >0)
        {
            check=1;
        }
        else
        {
            printf("Wrong input,input again!\n");
        }
        if(check==1)
        {
            for(i=0;i<number;i++)
            {
                if(id==ids[i])
                {
                    check=0;
                    printf("The id has been used!Input again!\n");
                    break;
                }
            }
        }
    }/*�˴�id�������˱���id����*/
    ids[count]=id;

/*��������*/
    printf("Please enter the students' name(only alphabet and spaces are permitted):");
    char *temp_name=right_input_char();
    strcpy(name,temp_name);
    free(temp_name);/*ʹ����֮�������ͷŵ�right_input_char����Ķ�̬�ڴ�*/
    /*��ʱname���浽�˱���name����*/
    strcpy(names[count],name);

/*д��ṹ����*/
    ISC *p=(ISC *)malloc(sizeof(ISC));
    ISC *pr=table;
    if(pr != NULL)
    {
        while(pr->next != NULL)
        {
            pr=pr->next;
        }
    }/*���ص�pr��Ҫô������Ϊ��ʱ��ͷָ��table��Ҫô��nextΪNULL�����һ���ṹ��*/
    p->info.id=id;
    strcpy(p->info.name,name);
    p->next=NULL;
    p->up=NULL;
    p->info.rank=0;

    float sum=0,aver=0;/*��ʼ����ɼ�*/
    for(i=0;i<sub_num;i++)
    {
        printf("Please enter the student's %s score(more than 0 and no more than 100):",sub_name[i]);
        check =0;
        while(check != 1)
        {
            score=right_input_float();
            if((score<0 && fabs(score -0)>eof) || (score>100 && fabs(score - 100)>eof))
            {
                printf("Wrong input,input again!\n");
            }
            else
            {
                sum+=score;
                scores[count][i]=score;
                p->info.scores[i]=score;
                check =1;
            }
        }
    }
    aver=sum/sub_num;
    p->info.scores[6]=scores[count][6]=aver;
    p->info.scores[7]=scores[count][7]=sum;

    if(table==NULL)/*��ʱ����û�нṹ�壬pr����table*/
    {
        table=p;
        p->up=table;
    }
    else
    {
        pr->next=p;
        p->up=p;
    }
    return table;
}
void show_title(char sub_names[][20],int sub_num,int type)/*���з�ʽ��id    name  sub_score(����20)-average������10��-sum(����7)-[rank](����5) typeΪ1����ʾrank��������ʾ*/
{
    int i=0;
    char str1[10]="id",str2[20]="name",str4[5]="rank",str5[10]="average",str6[7]="sum";
    printf("%-10s%-20s",str1,str2);
    for(i=0;i<sub_num;i++)
    {
        printf("%-20s",sub_names[i]);
    }
    printf("%-10s%-7s",str5,str6);
    if(type==1)
    {
        printf("%-5s\n",str4);
    }
    else
    {
        printf("\n");
    }
}
void show_target(ISC *temp1,char sub_names[][20],int sub_num,int type)
{

    if(temp1 == NULL)
    {
        printf("No such student!\n");
        return;
    }
    else
    {
        int i;
        printf("%-10ld%-20s",temp1->info.id,temp1->info.name);
        for(i=0;i<sub_num;i++)
        {
            printf("%-20.2f",temp1->info.scores[i]);
        }
        printf("%-10.2f%-7.2f",temp1->info.scores[6],temp1->info.scores[7]);
        printf("%-5d\n",temp1->info.rank);
    }
    return;
}
void show_all(ISC *table,char sub_names[][20],int sub_num,int type)/*����typeΪ1ʱ������ʾ����������Ϊ1ʱ��������ʾ������*/
{
    ISC *temp=table;
    int i=0;
    if(temp ==NULL)
    {
        printf("No student here\n");
        return;
    }
    else
    {
        show_title(sub_names,sub_num,type);
        while(temp != NULL)
        {
            printf("%-10ld%-20s",temp->info.id,temp->info.name);
            for(i=0;i<sub_num;i++)
            {
                printf("%-20.2f",temp->info.scores[i]);
            }
            printf("%-10.2f%-7.2f",temp->info.scores[6],temp->info.scores[7]);
            if(type == 1)
            {
                printf("%-5d\n",temp->info.rank);
            }
            else
            {
                printf("\n");
            }
            temp=temp->next;
        }
    }
}
void cal_ave(int sub_num,char sub_names[][20],float scores[][8],int count,float sum_list[],float aver_list[])
{
    int i=0,k=0;
    float sum,aver;
    for(i=0;i<sub_num;i++)/*�����ǰ��տ�Ŀ�ֻ�*/
    {
        sum=0,aver=0;
        for(k=0;k<count;k++)/*�����ǰ��������ֻ�*/
        {
            sum+=scores[k][i];/*��ÿ���˵���һ�Ƴɼ�������*/
        }
        aver=sum/count;
        sum_list[i]=sum;
        aver_list[i]=aver;
    }
    char str_sum[5]="sum",str_aver[10]="average";
    printf("%-20s%-10s%-5s\n"," ",str_aver,str_sum);
    for(i=0;i<sub_num;i++)
    {
        printf("%-20s%-10.2f%-5.2f\n",sub_names[i],aver_list[i],sum_list[i]);
    }
    return;
}
ISC *sort_table(ISC *table,int (*func)(ISC *pr,ISC *p))/*��������*/
{
    if(table==NULL)
    {
        printf("No student here\n");
        return table;
    }
    ISC *fake_table=NULL,*pr_up=table,*pr_now=table,*p=NULL;/*�����������б��ͷָ��*/
    ISC *head=(ISC *)malloc(sizeof(ISC));/*�����±�ͷ�ڵ�*/

    fake_table=head;
    head->up=fake_table;
    head->next=NULL;
    head->info=pr_now->info;

    pr_up=pr_now;
    pr_now=pr_now->next;
    free(pr_up);/*pr������һ�����ͷŵ�ԭprָ����ڴ�*/

    while(pr_now != NULL)
    {
        p=(ISC *)malloc(sizeof(ISC));/*�����µĽṹ��*/
        if(p==NULL)
        {
            printf("Something wrong!\n");
            exit(1);/*˵���ڴ��������߳������⣬��ֹ����*/
        }

        /*����p�����ݣ�up��nextָ������һ����������ƶ�*/
        p->up=NULL;
        p->info=pr_now->info;
        p->next=NULL;

        pr_up=pr_now;/*�Ѿ���ȡ��pr�����ݣ��������ͷ�pr��prָ����һ������*/
        pr_now=pr_now->next;
        free(pr_up);
        /*����p����������ʱ�Ĳ���*/
        int judge_head = func(p,head); /*���尴��ָ�������ж�p��head�Ĺ�ϵ�����pr��pС������-1����pһ��������0����p�󣬷���1*/
        switch(judge_head)/*��������ж������������������У������Ҫ˳�����У������get_reverse���ɣ�Ĭ��������Ϊidʱ���ظ����ݰ���id��������*/
        {
            case 1:/*���p���ж�������head�Ĵ�*/
            {
                p->next=head;
                head->up=p;
                p->up=fake_table;
                head=p;
                fake_table=p;
                break;
            }
            case 0:/*���p���ж�������head��һ��*/
            {
                if(p->info.id > head->info.id)/*���id��ͷ�ڵ��*/
                {
                    p->next=head;
                    head->up=p;
                    head=p;
                    p->up=fake_table;
                    fake_table=p;
                }
                else/*���id��ͷ�ڵ�С*/
                {
                    int temp_judge=0;
                    ISC *temp_pr=NULL,*pr=fake_table;
                    if(pr->next==NULL)/*���ֻ��ͷ�ڵ�һ���ṹ�壬��id����pҪ�󣬴�ʱpӦ����������ĩβ*/
                    {
                        pr->next=p;
                        p->up=pr;
                    }
                    else
                    {
                        temp_judge=func(pr,p);/*�Ƚ�p��pr��������pr��pС������-1��pr��pһ��������0��pr��p�󣬷���1*/
                        while(pr!=NULL && temp_judge == 0 && pr->info.id > p->info.id )/*���pr��������pһ��*/
                        {
                            temp_pr=pr;
                            pr=pr->next;
                            if(pr != NULL)
                            {
                                temp_judge=func(pr,p);
                            }
                        }/*���pr��������p��ͬ����id��p������ת����һ��*/
                        /*���ص�pr�����Ϊ�գ���˵��ȫ���ڵ�������ͬ����p��id��������С�ģ���pr��Ϊ�գ����������p��һ������˵��p�Ǹ�������p��id��С�ģ���������һ������p��id��pr�Ĵ�p��Ӧ�÷���prǰ��*/
                        if(pr==NULL)
                        {
                            temp_pr->next=p;
                            p->up=temp_pr;
                        }
                        else
                        {
                            p->up=pr->up;
                            temp_pr->next=p;
                            p->next=pr;
                            pr->up=p;
                        }
                    }
                }
                break;
            }
            case -1:/*���p�ĳɼ���head�ĳɼ�С*/
            {
                ISC *pr=fake_table;
                ISC *temp_pr=NULL;
                int temp_judge=func(pr,p);/*�Ƚ�p��pr��������pr������С������-1,��p��prһ����0��pr������1��*/
                while(pr!=NULL && temp_judge==1)/*��pr��Ϊ����pr������p���ʱ�򣬽�prָ����һ���ṹ��*/
                {
                    temp_pr=pr;
                    pr=pr->next;
                    if(pr!=NULL)
                    {
                        temp_judge=func(pr,p);/*ѭ�������ı�־���ҵ���һ���ṹ�壬�ɼ�����p��*/
                    }
                }
                if(pr==NULL)/*���prΪ�գ���ʾ���нṹ��ɼ�����P��*/
                {
                    pr=temp_pr;
                    pr->next=p;
                    p->up=pr;
                }
                else if(temp_judge == 0)/*������p�ĳɼ���pr�ĳɼ�һ��*/
                {
                    if(p->info.id > pr->info.id)  /*��ʱ��ʼ�Ƚ�id�����P��id��pr�Ĵ�*/
                    {
                        /*����Ϊ�õ�pr����P����prǰ���*/
                        ISC *temp_ago = pr->up;
                        temp_ago->next=p;
                        p->up=temp_ago;
                        p->next=pr;
                        pr->up=p;
                    }
                    else
                    {
                        ISC *temp_pr=NULL;
                        while(pr != NULL && temp_judge == 0 && pr->info.id > p->info.id)/*���pr�ĳɼ���p����ͬ,pr��id��p��id��pr��Ϊ��*/
                        {
                            temp_pr=pr;
                            pr=pr->next;/*����pr��������prΪ�գ�p��������ĩβ��pr�ĳɼ�������p�ˣ�˵��p�Ǹóɼ���id��С�ģ����߷�����ͬ������pr��id��p��idС,��ʱֻ��Ҫ����pr��p����*/
                            if(pr!=NULL)
                            {
                                temp_judge=func(pr,p);
                            }
                        }
                        if(pr==NULL)/*prΪ�գ�����p�Ǹóɼ���id��С�ģ����ǽṹ������һ��*/
                        {
                            temp_pr->next=p;
                            p->up=temp_pr;
                        }
                        else/*pr��Ϊ�գ�����prǰ�����p*/
                        {
                            temp_pr->next=p;
                            p->up=temp_pr;
                            p->next=pr;
                            pr->up=p;
                        }
                    }
                }
                else/*���p�ĳɼ���pr�ĳɼ���*/
                {
                    ISC *temp_ago=pr->up;
                    temp_ago->next=p;
                    p->up=temp_ago;
                    p->next=pr;
                    pr->up=p;
                }
                break;
            }
        }
    }
    return fake_table;
}
void show_table(ISC *table,char sub_names[][20],int sub_num,int type)/*����ǰ����˳��չʾ�������type����1������ʾ������������ʾ*/
{
    ISC *temp = table;
    int i;
    if(table==NULL)
    {
        printf("No student here\n");
    }
    else
    {
        printf("Show table now\n");
        show_title(sub_names,sub_num,type);
        while(temp != NULL)
        {
            printf("%-10ld%-20s",temp->info.id,temp->info.name);
            for(i=0;i<sub_num;i++)
            {
                printf("%-20.2f",temp->info.scores[i]);
            }
            printf("%-10.2f%-7.2f",temp->info.scores[6],temp->info.scores[7]);
            if(type == 1)
            {
                printf("%-5d\n",temp->info.rank);
            }
            else
            {
                printf("\n");
            }
            temp=temp->next;
        }
    return;
    }
}
ISC *rank_table(ISC *table, int(*func)(ISC *pr,ISC*p))/*��ָ�������жϣ�������ͬ��ʱ��rank���䣬�������ӣ�������ͬʱ�����ճ��ֵ��Ⱥ�˳����������*/
{
    ISC *temp=table;
    table->info.rank=1;/*ָ��ͷ�ڵ��rankΪ1*/
    if(table->next==NULL)/*ֻ��һ���ṹ���ʱ�򣬵�һ����Ϊ1��ֱ�ӷ���*/
    {
        return table;
    }
    temp=temp->next;
    while(temp!=NULL)
    {
        int temp_judge=func(temp,temp->up);
        if(temp_judge == 0)/*���temp��֮ǰ��������ͬ����Ϊ������ͬ����ʱ����ͬ�ϣ�������һ*/
        {
            temp->info.rank=temp->up->info.rank;
            block+=1;
        }
        else/*������ͬ��ʱ������Ӧ������һ������������������ͬʱ����������*/
        {
            temp->info.rank=temp->up->info.rank + block;
            block=1;
        }
        temp=temp->next;
    }
    block =1;
    return table;
}
ISC *get_reverse_table(ISC *table,ISC *reverse_table)/*����һ���͵�ǰ������ȫ�����������*/
{
    /*�ͷŵ���һ��reverse_table������ڴ�*/
    Free_table(reverse_table);

    ISC *temp=table,*reverse_head=NULL,*symbol=NULL;
    while(temp->next != NULL)
    {
        temp=temp->next;/*temp�����е����������temp����ͷ�ڵ㡢temp�ǵڶ����ڵ�*/
    }
    /*��ʱ���ص�temp��ԭ������������һ���ṹ��*/
    reverse_head=(ISC *)malloc(sizeof(ISC));/*�������б��ͷ�ڵ�*/
    reverse_head->info=temp->info;
    reverse_head->next=NULL;
    reverse_table=reverse_head;
    reverse_head->up=reverse_table;
    if(temp->up==table && temp==table)/*��ʱ��ζ��temp����ͷ�ڵ�,ԭ����ֻ��һ���ڵ�*/
    {
        return reverse_table;
    }
    symbol=reverse_head;
    temp=temp->up;/*��ʱtemp���������ڶ���*/
    while(temp != table)
    {
        ISC *rp=NULL,*rpr=NULL;
        /*����������rp*/
        rp=(ISC *)malloc(sizeof(ISC));
        rp->info=temp->info;
        rp->up=NULL;
        rp->next=NULL;
        /*���������ӵ�reverse_table*/
        rpr=reverse_table;
        while(rpr->next != NULL)
        {
            rpr=rpr->next;
        }/*��ʱ���ص�rpr���������ڵ����һ���ṹ��*/
        rpr->next=rp;
        rp->up=rpr;
        temp=temp->up;
        symbol=rp;
    }
    /*��ʱtemp���ǵ�һ������,�Ҵ�ʱ��symbol�������һ���ṹ��*/
    ISC *tail=NULL;
    tail=(ISC *)malloc(sizeof(ISC));
    tail->info=temp->info;
    tail->up=symbol;
    symbol->next=tail;
    tail->next=NULL;

    return reverse_table;
}
void Free_table(ISC *table)
{
    if(table == NULL)
    {
        return;
    }
    else
    {
        ISC *temp=table;
        while(table != NULL)
        {
            temp=table;
            table=table->next;
            free(temp);
        }
        return;
    }
}
void find_id(ISC *table,char sub_names[][20],int sub_num)
{
    long int sid;
    int ret=0;
    printf("Please input the id:");
    while(ret != 1)
    {
        sid=right_input_long();
        ret =1;
        if(sid <= 0)
        {
            ret=0;
        }
        if(ret ==0)
        {
            printf("Wrong input,input again!\n");
        }
    }

    ISC *temp1=table;
    if(temp1==NULL)
    {
        printf("no student here!\n");
        return;
    }
    while(temp1 != NULL && temp1->info.id != sid )
    {
        temp1=temp1->next;/*��ʱ���ص�temp1ֻ�����������һ�����ҵ���ָ����id����һ����temp1������table��β��û�ҵ�����Ϊ��*/
    }
    if(temp1 != NULL)
    {
        show_title(sub_names,sub_num,1);
    }
    show_target(temp1,sub_names,sub_num,1);
    return;
}
void find_name(ISC *table,char sub_names[][20],int sub_num)
{
    char name[20];
    int flag=0;
    printf("Please input the name:\n");
    char *temp_name=right_input_char();/*��tempָ�򷵻صĹ淶���ַ���*/
    strcpy(name,temp_name);
    free(temp_name);

    ISC *temp=table;
    if(temp == NULL)/*������Ϊ�գ���û��ѧ����Ϣʱ*/
    {
        printf("No student here!\n");
        return;
    }

    while(temp != NULL)
    {
        if(strcmp(temp->info.name,name) == 0)
        {
            if(flag==0)
            {
                show_title(sub_names,sub_num,1);
            }
            flag=1;
            show_target(temp,sub_names,sub_num,1);
        }
        temp=temp->next;
    }

    if(flag==0)
    {
        printf("no such student!\n");
    }
    return;
}
void analysis(float scores[][8],int count,char sub_names[][20],int sub_num)
{
    int o,p,temp;
    int analy[sub_num][5];/*һ�д���һ����Ŀ��һ�д���ÿ�Ŀ��Ӧ�׶ε�����*/
    for(o=0;o<sub_num;o++)/*��analy��ʼ����ȫ����ֵ0*/
    {
        for(p=0;p<5;p++)
        {
            analy[o][p]=0;
        }
    }

    for(o=0;o<sub_num;o++)/*����Ŀ����*/
    {
        for(p=0;p<count;p++)/*������ѭ��*/
        {
            temp=(int)scores[p][o]/10;
            if(temp == 9 || temp == 10)
            {
                analy[o][0] += 1;
            }
            else if(temp == 8)
            {
                analy[o][1] += 1;
            }
            else if(temp ==7)
            {
                analy[o][2] += 1;
            }
            else if(temp == 6)
            {
                analy[o][3] += 1;
            }
            else
            {
                analy[o][4] += 1;
            }
        }
    }
    printf("%-20s%-4s%-8s %-4s%-8s %-4s%-8s %-4s%-8s %-4s%-8s\n"," ","A","A-rate","B","B-rate","C","C-rate","D","D-rate","E","E-rate");
    for(o=0;o<sub_num;o++)
    {
        printf("%-20s",sub_names[o]);
        for(p=0;p<5;p++)
        {
            printf("%-4d%6.2f%%  ",analy[o][p],((float)analy[o][p]/count)*100);
        }
        printf("\n");
    }

}
int judge_by_score(ISC *pr,ISC *p)
{
    int result=0;
	if( p->info.scores[7] > pr->info.scores[7] && fabs(p->info.scores[7] - pr->info.scores[7])>eof)/*���pr�ĳɼ���p�ĳɼ�С*/
	{
		result =-1;
	}
	else if(fabs(pr->info.scores[7] - p->info.scores[7])<=eof)/*���pr�ĳɼ���p��һ��*/
	{
		result =0;
	}
	else if(p->info.scores[7] < pr->info.scores[7] && fabs(p->info.scores[7] - pr->info.scores[7])>eof)/*���pr�ĳɼ���p�ĳɼ���*/
	{
		result =1;
	}
	return result;
}
int judge_by_id(ISC *pr,ISC *p)/*pr��idС������-1��һ��������0��pr��id�󣬷���1*/
{
    int result=0;
    if( p->info.id > pr->info.id)
	{
		result =-1;
	}
	else if(p->info.id == pr->info.id)
	{
		result =0;
	}
	else if(p->info.id < pr->info.id)
	{
		result =1;
	}
	return result;
}
int judge_by_name(ISC *pr,ISC *p)/*���尴�������ж�pr��p�Ĺ�ϵ,���pr���ֱ�pС������-1�������pһ��������0����p�󣬷���1*/
{
    int result = 0;
    if(strcmp(pr->info.name,p->info.name)<0)
    {
        result =-1;
    }
    else if(strcmp(pr->info.name,p->info.name)==0)
    {
        result =0;
    }
    else if(strcmp(pr->info.name,p->info.name)>0)
    {
        result =1;
    }
    return result;
}
int input_over(void)
{
    int check;
    char left=getchar();
    if(left == '\n')/*�ж���������󣬻��ǿ��Խ��������Ļس������أ���֤������û����������*/
    {
        check=1;
    }
    else
    {
        check=0;
        while(getchar()!='\n');/*��������ʱ��Ĭ���������������������*/
        printf("Wrong input,input again!\n");
    }
    return check;
}
float right_input_float(void)/*�淶���븡�����������ظ�����,��������ʾ���*/
{
    float result;
    int ret=0;
    while(ret != 1)
    {
        ret = scanf("%f",&result);
        if(ret != 1)/*����������󣬱����һ�����ǷǷ��ַ�������Ҫ�����������������������ѭ��*/
        {
            printf("Wrong input,input again!\n");
            while(getchar()!='\n');
        }
        if(ret ==1)/*�������ɹ����жϻ��������������쳣����*/
        {
            ret = input_over();
        }
    }
    return result;
}
int right_input_int(void)/*�淶�������ͣ�����������*/
{
    int result;
    int ret=0;
    while(ret != 1)
    {
        ret = scanf("%d",&result);
        if(ret != 1)/*����������󣬱����һ�����ǷǷ��ַ�������Ҫ�����������������������ѭ��*/
        {
            printf("Wrong input,input again!\n");
            while(getchar()!='\n');
        }
        else if(ret ==1)/*�������ɹ����жϻ��������������쳣����*/
        {
            ret = input_over();
        }

    }
    return result;
}
char *right_input_char(void)/*�淶�����ַ����������ַ���(��̬�ڴ������һ����ַָ��)����*/
{
    char *result=(char *)malloc(20);
    int length,i=0,flag=0;
    while(flag != 1)
    {
        fgets(result,20,stdin);/*�ӻ�������ȡ����20���ַ�*/
        if(result[0]=='\n')/*��ʾfgets��ȡ���ĵ�һ���ַ����ǻس�����Ȼ�쳣*/
        {
            flag=0;
        }
        else
        {
            /*���½����ַ����������ж��ַ����Ƿ�ϸ�*/
            while(result[i]!='\n' && i<20)
            /*������ս������ַ������±�Ϊi��Ԫ�ز��ǻ��з�����������һ��Ԫ��*/
            {
                i+=1;
            }
            /*��ʱ���ص�i�����i=20��˵�������ַ�������û�л��з�,��ʱ�����жϺ����Ƿ�����������.���������ֻʣ��\n���������������쳣����ջ���������������*/
            if(i==20)
            {
                flag=input_over();/*����-flag=1,�쳣-flag=0*/
            }
            else/*��i������20����˵���ַ������뵽�˽�����,�ѻ��з��ĳ��ַ���������*/
            {
                result[i]='\0';
                flag=1;
            }
            /*����Ϊ�жϷ��ϳ���Ҫ����ַ����������Ƿ����Ҫ��*/
            if(flag==1)
            {
                length=strlen(result);
                for(i=0;i<length;i++)
                {
                    if(result[0]==32)/*��������Կո�ͷ����Ϊ��������*/
                    {
                        flag=0;
                        break;
                    }
                    if(!((result[i]>=65 && result[i]<=90) || (result[i] <=122 && result[i]>=97) || result[i] ==32 ))
                    {
                        flag=0;
                        break;/*ֻҪ��һ��������ĸ�ո񣬾�˵���������ֱ�ӽ���*/
                    }
                }
            }
        }
        if(flag==0)
        {
            printf("Wrong input,input again!\n");
        }
    }
return result;
}
long int right_input_long(void)/*�淶���볤���ͣ�������*/
{
    long int result;
    int ret=0;
    while(ret !=1)
    {
        ret=scanf("%ld",&result);
        if(ret != 1)/*����������󣬱����һ�����ǷǷ��ַ�������Ҫ�����������������������ѭ��*/
        {
            printf("Wrong input,input again!\n");
            while(getchar()!='\n');
        }
        else if(ret==1)
        {
            ret = input_over();
        }
    }
    return result;
}

