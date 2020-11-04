#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
//规定：学生id只能是正整数，成绩只能是[0,100]的任意浮点数，名字里面只能有字母和空格，且不能以空格开头，可以以空格结尾；
typedef struct information
{
    long int id;
    char name[20];
    float scores[8];/*各科分数*/
    int rank;
}INFO;
typedef struct ISCORE/*结构体内设置结构体是为了如果后续学生信息需要写入文档的时候方便将信息结构体整个写入*/
{
    struct ISCORE *up;
    INFO info;
    struct ISCORE *next;
}ISC;
//函数功能：添加学生信息，从尾部添加到数组以及链表中，返回链表头指针
ISC *Add_record(long int ids[],float scores[][8],char names[][20],int number,int count,int sub_num,char sub_name[][20],ISC *table);
//函数功能：/*排列方式，id-name-sub_score(长度20)-[rank](长度5)-average(长度10)-sum(长度5)-type为1显示rank;否则不显示
void show_title(char sub_names[][20],int sub_num,int type);
//函数功能：显示目标结构体的数据，用于按照id或者姓名查找
void show_target(ISC *temp1,char sub_names[][20],int sub_num,int type);
//函数功能：显示所有成员，传递type为1时，会显示排名栏，不为1时，不会显示排名栏
void show_all(ISC *table,char sub_names[][20],int sub_num,int type);
//函数功能：计算平均分，并打印出表格，但是不会保存
void cal_ave(int sub_num,char sub_names[][20],float scores[][8],int count,float sum_list[],float aver_list[]);
//函数功能：按照给定的函数，确定判断条件，然后逆序排列链表
ISC *sort_table(ISC *table,int (*func)(ISC *pr,ISC *p));
//函数功能：按当前链表顺序展示链表，如果type给了1，就显示排名，否则不显示
void show_table(ISC *table,char sub_names[][20],int sub_num,int type);
//按指定条件判断，条件相同的时候，rank不变，堵塞增加，条件不同时，按照出现的先后顺序依次排序
ISC *rank_table(ISC *table, int(*func)(ISC *pr,ISC*p));
//函数功能:创建一个和当前链表完全反序的新链表
ISC *get_reverse_table(ISC *table,ISC *reverse_table);
//函数功能:释放掉某一指定链表所有链节申请的动态内存
void Free_table(ISC *table);
//函数功能:函数功能,按照id查找指定学生，输入id、成绩和排名
void find_id(ISC *table,char sub_names[][20],int sub_num);
//函数功能：分析数据，打印出结果
void analysis(float scores[][8],int count,char sub_names[][20],int sub_num);
//函数功能:按照姓名查找学生
void find_name(ISC *table,char sub_names[][20],int sub_sum);

int judge_by_id(ISC *pr,ISC *p);
int judge_by_score(ISC *pr,ISC *p);/*定义按照成绩判断pr和p的关系，如果pr成绩比p小，返回1，和p一样，返回2；比p大，返回3*/
int judge_by_name(ISC *pr,ISC *p);/*定义按照名字判断pr和p的关系,如果pr名字比p小，返回1，如果和p一样，返回2，比p大，返回3*/
int input_over(void);/*每次输入后，如果在缓冲区还能检测到除了回车符以外的字符，则说明有非法输入，此时需要重新输入,正确输入返回1，错误输入返回0*/
float right_input_float(void);/*规范输入浮点数，并返回浮点数*/
int right_input_int(void);/*规范输入整型，并返回整型*/
char *right_input_char(void);/*规范输入字符串，并将字符串返回,用完之后记得释放！！！*/
long int right_input_long(void);/*规范输入长整型，并返回*/

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
        number=right_input_int();/*规范输入整型，包括了判断和清空缓冲区*/
        if(number >0 && number <=30)/*规范输入后，数据是规范的整型，但是数据是否在0-30内还需判断*/
        {
            check = 1;
        }
        else
        {
            printf("Wrong input,input again!\n");
        }
    }
/*输入科目数量*/
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

    char sub_names[sub_num][20];/*定义科目名称数组*/
    long int ids[number];
    float scores[number][8];/*定义长度为8的数组，六个给科目用，剩下平均分和总分*/
    char names[number][20];
    float sum_list[6],aver_list[6];
/*输入科目名称，方便打印用*/
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
        printf("1．Input record\n");
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
                ISC *TEST=Add_record(ids,scores,names,number,count,sub_num,sub_names,table);/*每次添加学生，自动生成一张链表(没有顺序的)*/
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
        case 4:/*总分从高到低，需要排名*/
            {
                table=sort_table(table,judge_by_score);
                table=rank_table(table,judge_by_score);
                show_table(table,sub_names,sub_num,1);
                Sleep(1000);
                break;
            }
        case 5:/*总分从低到高，需要排名*/
            {
                table=sort_table(table,judge_by_score);
                reverse_table=get_reverse_table(table,reverse_table);
                reverse_table=rank_table(reverse_table,judge_by_score);
                show_table(reverse_table,sub_names,sub_num,1);
                Sleep(1000);
                break;
            }
        case 6:/*按照id排序，不需要排名*/
            {
                table=sort_table(table,judge_by_id);
                reverse_table=get_reverse_table(table,reverse_table);
                show_table(reverse_table,sub_names,sub_num,0);
                Sleep(1000);
                /*show_all(ids,scores,count);*/
                break;
            }
        case 7:/*按照姓名排序，不需要排名*/
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

/*输入id*/
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
    }/*此处id保存在了变量id里面*/
    ids[count]=id;

/*输入名字*/
    printf("Please enter the students' name(only alphabet and spaces are permitted):");
    char *temp_name=right_input_char();
    strcpy(name,temp_name);
    free(temp_name);/*使用完之后立刻释放掉right_input_char申请的动态内存*/
    /*此时name保存到了变量name里面*/
    strcpy(names[count],name);

/*写入结构体中*/
    ISC *p=(ISC *)malloc(sizeof(ISC));
    ISC *pr=table;
    if(pr != NULL)
    {
        while(pr->next != NULL)
        {
            pr=pr->next;
        }
    }/*返回的pr，要么是链表为空时的头指针table，要么是next为NULL的最后一个结构体*/
    p->info.id=id;
    strcpy(p->info.name,name);
    p->next=NULL;
    p->up=NULL;
    p->info.rank=0;

    float sum=0,aver=0;/*开始输入成绩*/
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

    if(table==NULL)/*此时链表没有结构体，pr代表table*/
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
void show_title(char sub_names[][20],int sub_num,int type)/*排列方式，id    name  sub_score(长度20)-average（长度10）-sum(长度7)-[rank](长度5) type为1，显示rank，否则不显示*/
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
void show_all(ISC *table,char sub_names[][20],int sub_num,int type)/*传递type为1时，会显示排名栏，不为1时，不会显示排名栏*/
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
    for(i=0;i<sub_num;i++)/*这里是按照科目轮换*/
    {
        sum=0,aver=0;
        for(k=0;k<count;k++)/*这里是按照人数轮换*/
        {
            sum+=scores[k][i];/*将每个人的这一科成绩加起来*/
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
ISC *sort_table(ISC *table,int (*func)(ISC *pr,ISC *p))/*链表排序*/
{
    if(table==NULL)
    {
        printf("No student here\n");
        return table;
    }
    ISC *fake_table=NULL,*pr_up=table,*pr_now=table,*p=NULL;/*创建排序后的列表的头指针*/
    ISC *head=(ISC *)malloc(sizeof(ISC));/*定义新表头节点*/

    fake_table=head;
    head->up=fake_table;
    head->next=NULL;
    head->info=pr_now->info;

    pr_up=pr_now;
    pr_now=pr_now->next;
    free(pr_up);/*pr跳到下一个，释放掉原pr指向的内存*/

    while(pr_now != NULL)
    {
        p=(ISC *)malloc(sizeof(ISC));/*定义新的结构体*/
        if(p==NULL)
        {
            printf("Something wrong!\n");
            exit(1);/*说明内存已满或者出现问题，终止程序*/
        }

        /*定义p的数据，up和next指针在这一步不会进行制定*/
        p->up=NULL;
        p->info=pr_now->info;
        p->next=NULL;

        pr_up=pr_now;/*已经获取了pr的数据，则立刻释放pr，pr指向下一个内容*/
        pr_now=pr_now->next;
        free(pr_up);
        /*定义p插入新链表时的操作*/
        int judge_head = func(p,head); /*定义按照指定条件判断p和head的关系，如果pr比p小，返回-1，和p一样，返回0；比p大，返回1*/
        switch(judge_head)/*因此所有判断条件都将是逆序排列，如果需要顺序排列，则调用get_reverse即可，默认条件不为id时，重复数据按照id逆序排序*/
        {
            case 1:/*如果p的判断条件比head的大*/
            {
                p->next=head;
                head->up=p;
                p->up=fake_table;
                head=p;
                fake_table=p;
                break;
            }
            case 0:/*如果p的判断条件和head的一样*/
            {
                if(p->info.id > head->info.id)/*如果id比头节点大*/
                {
                    p->next=head;
                    head->up=p;
                    head=p;
                    p->up=fake_table;
                    fake_table=p;
                }
                else/*如果id比头节点小*/
                {
                    int temp_judge=0;
                    ISC *temp_pr=NULL,*pr=fake_table;
                    if(pr->next==NULL)/*如果只有头节点一个结构体，且id还比p要大，此时p应该是在链表末尾*/
                    {
                        pr->next=p;
                        p->up=pr;
                    }
                    else
                    {
                        temp_judge=func(pr,p);/*比较p和pr的条件，pr比p小，返回-1，pr和p一样，返回0，pr比p大，返回1*/
                        while(pr!=NULL && temp_judge == 0 && pr->info.id > p->info.id )/*如果pr的条件和p一样*/
                        {
                            temp_pr=pr;
                            pr=pr->next;
                            if(pr != NULL)
                            {
                                temp_judge=func(pr,p);
                            }
                        }/*如果pr的条件和p相同，且id比p大，则跳转至下一个*/
                        /*返回的pr，如果为空，则说明全部节点条件相同，且p的id是其中最小的；若pr不为空，如果条件和p不一样，则说明p是该条件下p的id最小的，若果条件一样，则p的id比pr的大，p都应该放在pr前面*/
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
            case -1:/*如果p的成绩比head的成绩小*/
            {
                ISC *pr=fake_table;
                ISC *temp_pr=NULL;
                int temp_judge=func(pr,p);/*比较p和pr的条件，pr的条件小，返回-1,；p和pr一样，0；pr条件大，1；*/
                while(pr!=NULL && temp_judge==1)/*当pr不为空且pr条件比p大的时候，将pr指向下一个结构体*/
                {
                    temp_pr=pr;
                    pr=pr->next;
                    if(pr!=NULL)
                    {
                        temp_judge=func(pr,p);/*循环结束的标志：找到第一个结构体，成绩不比p大*/
                    }
                }
                if(pr==NULL)/*如果pr为空，表示所有结构体成绩都比P大*/
                {
                    pr=temp_pr;
                    pr->next=p;
                    p->up=pr;
                }
                else if(temp_judge == 0)/*如果如果p的成绩和pr的成绩一样*/
                {
                    if(p->info.id > pr->info.id)  /*此时开始比较id，如果P的id比pr的大*/
                    {
                        /*以下为得到pr，将P插入pr前面的*/
                        ISC *temp_ago = pr->up;
                        temp_ago->next=p;
                        p->up=temp_ago;
                        p->next=pr;
                        pr->up=p;
                    }
                    else
                    {
                        ISC *temp_pr=NULL;
                        while(pr != NULL && temp_judge == 0 && pr->info.id > p->info.id)/*如果pr的成绩和p的相同,pr的id比p的id大，pr不为空*/
                        {
                            temp_pr=pr;
                            pr=pr->next;/*返回pr的条件，pr为空，p放在链表末尾；pr的成绩不等于p了，说明p是该成绩下id最小的；或者分数相同，但是pr的id比p的id小,此时只需要交换pr和p即可*/
                            if(pr!=NULL)
                            {
                                temp_judge=func(pr,p);
                            }
                        }
                        if(pr==NULL)/*pr为空，表面p是该成绩下id最小的，且是结构体的最后一个*/
                        {
                            temp_pr->next=p;
                            p->up=temp_pr;
                        }
                        else/*pr不为空，则在pr前面插入p*/
                        {
                            temp_pr->next=p;
                            p->up=temp_pr;
                            p->next=pr;
                            pr->up=p;
                        }
                    }
                }
                else/*如果p的成绩比pr的成绩大*/
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
void show_table(ISC *table,char sub_names[][20],int sub_num,int type)/*按当前链表顺序展示链表，如果type给了1，就显示排名，否则不显示*/
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
ISC *rank_table(ISC *table, int(*func)(ISC *pr,ISC*p))/*按指定条件判断，条件相同的时候，rank不变，堵塞增加，条件不同时，按照出现的先后顺序依次排序*/
{
    ISC *temp=table;
    table->info.rank=1;/*指定头节点的rank为1*/
    if(table->next==NULL)/*只有一个结构体的时候，第一个置为1，直接返回*/
    {
        return table;
    }
    temp=temp->next;
    while(temp!=NULL)
    {
        int temp_judge=func(temp,temp->up);
        if(temp_judge == 0)/*如果temp与之前的条件相同，认为排名相同，此时排名同上，阻塞加一*/
        {
            temp->info.rank=temp->up->info.rank;
            block+=1;
        }
        else/*条件不同的时候，排名应该是上一个的排名加上阻塞，同时将阻塞归零*/
        {
            temp->info.rank=temp->up->info.rank + block;
            block=1;
        }
        temp=temp->next;
    }
    block =1;
    return table;
}
ISC *get_reverse_table(ISC *table,ISC *reverse_table)/*创建一个和当前链表完全反序的新链表*/
{
    /*释放掉上一次reverse_table申请的内存*/
    Free_table(reverse_table);

    ISC *temp=table,*reverse_head=NULL,*symbol=NULL;
    while(temp->next != NULL)
    {
        temp=temp->next;/*temp可能有的特殊情况，temp就是头节点、temp是第二个节点*/
    }
    /*此时返回的temp是原链表里面的最后一个结构体*/
    reverse_head=(ISC *)malloc(sizeof(ISC));/*定义逆列表的头节点*/
    reverse_head->info=temp->info;
    reverse_head->next=NULL;
    reverse_table=reverse_head;
    reverse_head->up=reverse_table;
    if(temp->up==table && temp==table)/*此时意味着temp就是头节点,原链表只有一个节点*/
    {
        return reverse_table;
    }
    symbol=reverse_head;
    temp=temp->up;/*此时temp跳到导数第二节*/
    while(temp != table)
    {
        ISC *rp=NULL,*rpr=NULL;
        /*定义新链接rp*/
        rp=(ISC *)malloc(sizeof(ISC));
        rp->info=temp->info;
        rp->up=NULL;
        rp->next=NULL;
        /*插入新链接到reverse_table*/
        rpr=reverse_table;
        while(rpr->next != NULL)
        {
            rpr=rpr->next;
        }/*此时返回的rpr就是逆链节的最后一个结构体*/
        rpr->next=rp;
        rp->up=rpr;
        temp=temp->up;
        symbol=rp;
    }
    /*此时temp就是第一个链节,且此时的symbol就是最后一个结构体*/
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
        temp1=temp1->next;/*此时返回的temp1只有两种情况，一种是找到了指定的id，另一种是temp1搜索到table表尾还没找到，即为空*/
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
    char *temp_name=right_input_char();/*将temp指向返回的规范的字符串*/
    strcpy(name,temp_name);
    free(temp_name);

    ISC *temp=table;
    if(temp == NULL)/*若链表为空，即没有学生信息时*/
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
    int analy[sub_num][5];/*一行代表一个科目，一列代表该科目对应阶段的人数*/
    for(o=0;o<sub_num;o++)/*给analy初始化，全部赋值0*/
    {
        for(p=0;p<5;p++)
        {
            analy[o][p]=0;
        }
    }

    for(o=0;o<sub_num;o++)/*按科目分类*/
    {
        for(p=0;p<count;p++)/*按人数循环*/
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
	if( p->info.scores[7] > pr->info.scores[7] && fabs(p->info.scores[7] - pr->info.scores[7])>eof)/*如果pr的成绩比p的成绩小*/
	{
		result =-1;
	}
	else if(fabs(pr->info.scores[7] - p->info.scores[7])<=eof)/*如果pr的成绩和p的一样*/
	{
		result =0;
	}
	else if(p->info.scores[7] < pr->info.scores[7] && fabs(p->info.scores[7] - pr->info.scores[7])>eof)/*如果pr的成绩比p的成绩大*/
	{
		result =1;
	}
	return result;
}
int judge_by_id(ISC *pr,ISC *p)/*pr的id小，返回-1，一样，返回0，pr的id大，返回1*/
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
int judge_by_name(ISC *pr,ISC *p)/*定义按照名字判断pr和p的关系,如果pr名字比p小，返回-1，如果和p一样，返回0，比p大，返回1*/
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
    if(left == '\n')/*判断正常输入后，还是可以将缓冲区的回车符读回，保证缓冲区没有其他数据*/
    {
        check=1;
    }
    else
    {
        check=0;
        while(getchar()!='\n');/*错误输入时，默认清除掉缓冲区所有内容*/
        printf("Wrong input,input again!\n");
    }
    return check;
}
float right_input_float(void)/*规范输入浮点数，并返回浮点数,不包含提示语句*/
{
    float result;
    int ret=0;
    while(ret != 1)
    {
        ret = scanf("%f",&result);
        if(ret != 1)/*如果输入有误，比如第一个就是非法字符，则需要立刻清除缓冲区，否则无限循环*/
        {
            printf("Wrong input,input again!\n");
            while(getchar()!='\n');
        }
        if(ret ==1)/*如果输入成功，判断缓冲区有无其他异常数据*/
        {
            ret = input_over();
        }
    }
    return result;
}
int right_input_int(void)/*规范输入整型，并返回整型*/
{
    int result;
    int ret=0;
    while(ret != 1)
    {
        ret = scanf("%d",&result);
        if(ret != 1)/*如果输入有误，比如第一个就是非法字符，则需要立刻清除缓冲区，否则无限循环*/
        {
            printf("Wrong input,input again!\n");
            while(getchar()!='\n');
        }
        else if(ret ==1)/*如果输入成功，判断缓冲区有无其他异常数据*/
        {
            ret = input_over();
        }

    }
    return result;
}
char *right_input_char(void)/*规范输入字符串，并将字符串(动态内存申请的一个地址指针)返回*/
{
    char *result=(char *)malloc(20);
    int length,i=0,flag=0;
    while(flag != 1)
    {
        fgets(result,20,stdin);/*从缓冲区读取至多20个字符*/
        if(result[0]=='\n')/*表示fgets读取到的第一个字符就是回车，必然异常*/
        {
            flag=0;
        }
        else
        {
            /*以下仅从字符串长度来判断字符串是否合格*/
            while(result[i]!='\n' && i<20)
            /*如果接收进来的字符串的下标为i的元素不是换行符，则跳到下一个元素*/
            {
                i+=1;
            }
            /*此时返回的i，如果i=20，说明整个字符串里面没有换行符,此时必须判断后面是否还有其他数据.如果缓冲区只剩下\n，则正常，否则异常，清空缓冲区，重新输入*/
            if(i==20)
            {
                flag=input_over();/*正常-flag=1,异常-flag=0*/
            }
            else/*若i不等于20，则说明字符串输入到此结束了,把换行符改成字符串结束符*/
            {
                result[i]='\0';
                flag=1;
            }
            /*以下为判断符合长度要求的字符串的内容是否符合要求*/
            if(flag==1)
            {
                length=strlen(result);
                for(i=0;i<length;i++)
                {
                    if(result[0]==32)/*如果名字以空格开头，视为错误输入*/
                    {
                        flag=0;
                        break;
                    }
                    if(!((result[i]>=65 && result[i]<=90) || (result[i] <=122 && result[i]>=97) || result[i] ==32 ))
                    {
                        flag=0;
                        break;/*只要有一个不是字母空格，就说明输入错误，直接结束*/
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
long int right_input_long(void)/*规范输入长整型，并返回*/
{
    long int result;
    int ret=0;
    while(ret !=1)
    {
        ret=scanf("%ld",&result);
        if(ret != 1)/*如果输入有误，比如第一个就是非法字符，则需要立刻清除缓冲区，否则无限循环*/
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

