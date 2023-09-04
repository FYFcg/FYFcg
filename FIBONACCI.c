#include <stdio.h>
#include <stdlib.h>
#include "FIBONACCI.h"

typedef struct city
{
    int city_name;
    int can_get[2][1000];
    int can_get_num;
    int pre_city;
    struct city *parent,*children,*left,*right;
    int child_num;
    int distance;
    int mark;
}City;
typedef City *CITY;
typedef struct hheap{
    int root_num;
    CITY min;
    CITY first_root,end_root;
    CITY graph[1000];
}Heap;
typedef Heap *HEAP;
// int Find_Shortest_Path();
int is_not_empty(HEAP H);
void Dijkstra(HEAP H);
void Relax(HEAP H,CITY temp,int i);
void Breakup(HEAP H,CITY change);
CITY DeleteMin(HEAP H);
void consolidate(HEAP H);
void Link(HEAP H,CITY v1,CITY v2);
void DeleteRoot(HEAP H,CITY min);
void read_line(HEAP H,CITY temp);
void Insert(HEAP H,CITY item);
void initialize(HEAP H,int city_cnt);
int Find_Shortest_Path()
{
    int city_cnt,path_cnt;
    HEAP H=(HEAP)malloc(sizeof(Heap));
    CITY temp=(CITY)malloc(sizeof(City));
    int v1,v2,dis;
    int k;
    scanf("%d%d\n",&city_cnt,&path_cnt);
    // printf("%d %d",city_cnt,path_cnt);
    initialize(H,city_cnt);
    for(k=0;k<path_cnt;k++){
        scanf("%d%d%d",&temp->city_name,&temp->can_get[0][0],&temp->can_get[1][0]);
        read_line(H,temp);
    }
    int begin,end;
    scanf("%d %d",&begin,&end);
    H->graph[begin]->distance=0;
    H->graph[begin]->pre_city=-1;
    H->min=H->graph[begin];
    Dijkstra(H);
    int cnt=1;int path[1000];
    path[0]=end;
    while(path[cnt-1]!=-1){
        path[cnt]=H->graph[path[cnt-1]]->pre_city;
        cnt++;
    }
    if(path[cnt-2]==begin){
    for(int i=cnt-2;i>0;i--){
        printf("%d->",path[i]);
    }
    printf("%d",path[0]);
    }else{
        printf("No path!");
    }
    free(H);
}
int is_not_empty(HEAP H){
    if(H->min==NULL) return 0;
    return 1;
}
void Dijkstra(HEAP H){
    // printf("1");
        CITY temp;
        while(is_not_empty(H)){
            temp=DeleteMin(H);
            for(int i=0;i<temp->can_get_num;i++){
                Relax(H,temp,i);
            }
        }
}
void Relax(HEAP H,CITY temp,int i){
    int zhongdian=temp->can_get[0][i];
    if(H->graph[zhongdian]->distance>temp->distance+temp->can_get[1][i]){
        H->graph[zhongdian]->pre_city=temp->city_name;
        H->graph[zhongdian]->distance=temp->distance+temp->can_get[1][i];
        CITY change=H->graph[zhongdian];
        if(change->parent!=NULL&&change->distance<change->parent->distance){
            Breakup(H,change);
        }
        if(H->min&&change->distance<H->min->distance) H->min=change;
    }
}
void Breakup(HEAP H,CITY change){
    CITY pa=change->parent;
    if(pa==NULL) return ;
    if(change==change->right){
        pa->children=NULL;
    }else{
        if(pa->children==change) pa->children=change->right;
        change->left->right=change->right;
        change->right->left=change->left;
    }
        change->parent=NULL;
        pa->child_num--;
        Insert(H,change);
        if(pa->mark==0) pa->mark=1;
        else{
            pa->mark=0;
            Breakup(H,pa);
        }
}
CITY DeleteMin(HEAP H){
    CITY min=H->min;
    if(min->children!=NULL){ //如果最小节点是有孩子的，还要对孩子进行操作
        CITY temp[min->child_num];int cnt=0;
        CITY i;
        for(i=min->children;i->right!=i;i=i->right){
            temp[cnt]=i;cnt++;
        }
        min->children=NULL;
        min->child_num=0;
        temp[cnt]=i;cnt++;
        for(int j=0;j<cnt;j++){
            temp[j]->parent=NULL;
            Insert(H,temp[j]);
            H->root_num++;
        }
    }
    DeleteRoot(H,min); //删除最小的节点。
    if(min->right==min) H->min=NULL;//如果堆中没有元素了，就可以结束了。
    else{
        consolidate(H);
    }
    return min;
}
void consolidate(HEAP H){
    CITY Hash[10000];//建立一个类似哈希表，A[i]指向一个有i个孩子的根节点
    for(int i=0;i<10000;i++) Hash[i]=NULL;
    CITY root[H->root_num];//建立一个指向所有根节点的数组
    int cnt=0;
    for(CITY i=H->first_root;i!=H->end_root;i=i->right){
        root[cnt]=i;cnt++;
    }
    root[cnt]=H->end_root;cnt++;
    //数组指向根链表的每一个元素
    for(int i=0;i<cnt;i++){
        int num=root[i]->child_num;
        while(Hash[num]!=NULL){
            CITY temp=Hash[num];
            if(root[i]->distance<temp->distance){
                // printf("1");
                Link(H,root[i],temp);
            }else{
                // printf("2");
                Link(H,temp,root[i]);
                root[i]=temp;
            }
            Hash[num]=NULL;
            num++;
        }
        Hash[num]=root[i];
    }
    H->min=NULL;
    for(int i=0;i<H->root_num;i++){
        if(Hash[i]!=NULL){
            Insert(H,Hash[i]);
        }
    }
}
void Link(HEAP H,CITY v1,CITY v2){
                DeleteRoot(H,v2);
                if(v1->children!=NULL&&v1->child_num!=0){
                CITY iter=v1->children;
                // for(iter=v1->children;iter->right!=iter;iter=iter->right){}
                for(int i=0;i<v1->child_num-1;i++){
                    iter=iter->right;
                }
                iter->right=v2;
                v2->right=v2;v2->left=iter;
                }else{
                    v1->children=v2;
                    v2->left=v2;v2->right=v2;
                }
                v2->parent=v1;v1->child_num++;
}
void DeleteRoot(HEAP H,CITY min){
    min->left->right=min->right;
    min->right->left=min->left;
    if(min==H->first_root) H->first_root=min->right;
    if(min==H->end_root) H->end_root=min->left;
    H->root_num--;
}
void read_line(HEAP H,CITY temp){
    int a=H->graph[temp->city_name]->can_get_num;
    H->graph[temp->city_name]->can_get[0][a]=temp->can_get[0][0];
    H->graph[temp->city_name]->can_get[1][a]=temp->can_get[1][0];
    H->graph[temp->city_name]->can_get_num++;
}
void Insert(HEAP H,CITY item){
    item->parent=NULL;
    if(!H->min){
        H->first_root=H->end_root=item;
        item->left=item->right=item;
        H->min=item;
    }else{
        item->left=H->end_root;
        item->right=H->first_root;
        H->first_root->left=item;
        H->end_root->right=item;
        H->end_root=item;
        if(H->min->distance>item->distance) H->min=item;
    }
    H->root_num++;
}
void initialize(HEAP H,int city_cnt){
    H->root_num=0;H->min=NULL;H->first_root=NULL;
    H->end_root=NULL;
    for(int i=0;i<city_cnt;i++){
        H->graph[i]=(CITY)malloc(sizeof(City));
        H->graph[i]->city_name=i;
        H->graph[i]->can_get_num=0;
        H->graph[i]->child_num=0;
        H->graph[i]->parent=NULL;
        H->graph[i]->children=NULL;
        H->graph[i]->left=H->graph[i]->right=NULL;
        H->graph[i]->pre_city=-1;
        H->graph[i]->distance=inf;
        H->graph[i]->mark=0;
    }
    for(int i=0;i<city_cnt;i++)
       Insert(H,H->graph[i]);
}
