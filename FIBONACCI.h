#ifndef _Fibonacci_
#define _Fibonacci_
#define inf 1000000
typedef struct city
{
    int city_name;
    int can_get[2][1000];
    int can_get_num;
    int pre_city;
    struct city * parent,*children,*left,*right;
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
int Find_Shortest_Path();
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

#endif