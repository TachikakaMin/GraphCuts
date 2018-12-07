#include <iostream>
#include <cstdio>
#include <cstring>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
#define ll int
const int MAXN=4000005;
const int INF=0x7fffffff;
struct edge{
    int to,cap,next;
};
edge e[30*MAXN];
int n,cnt=1;
int s,t;
int ans=0;
int g[MAXN],cur[MAXN],d[MAXN];
bool vis[MAXN];
queue<int> q;

int gcd(int a,int b)
{
    return b==0?a:gcd(b,a%b);
}
bool jud(ll x,ll y)
{
    ll t=x*x+y*y,sq=sqrt(t);
    if(sq*sq!=t)return 1;
    if(gcd(x,y)>1)return 1;
    return 0;
}

inline void addedge(int from,int to,int cap)
{
    cnt++;
    e[cnt].cap=cap;
    e[cnt].to=to;
    e[cnt].next=g[from];
    g[from]=cnt;
    cnt++;
    e[cnt].cap=0;
    e[cnt].to=from;
    e[cnt].next=g[to];
    g[to]=cnt;
    return ;
}
bool bfs()
{
    memset(vis,0,sizeof(vis));
    vis[s]=1;
    d[s]=0;
    q.push(s);
    while (!q.empty())
    {
        int x=q.front();q.pop();
        for (int i=g[x];i;i=e[i].next)
        {
            if (!vis[e[i].to]&&e[i].cap>0)
            {
                vis[e[i].to]=1;
                d[e[i].to]=d[x]+1;
                q.push(e[i].to);
            }
        }
    }
    return vis[t];
}
  
int dfs(int x,int a)
{
    if (x==t||a==0) return a;
    int f,flow=0;
    for (int &i=cur[x];i;i=e[i].next)
    {
        if (d[x]+1==d[e[i].to]&&(f=dfs(e[i].to,min(a,e[i].cap)))>0)
        {
            flow+=f;
            a-=f;
            e[i].cap-=f;
            e[i^1].cap+=f;
            if (a==0) break;
        }
    }
    return flow;
}
 
void max_flow(){

	while (bfs())
    {
        for (int i=s;i<=t;i++) cur[i]=g[i];
        ans+=dfs(s,INF);
    }
}


int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }
    Mat image;
    image = imread( argv[1]);
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }


 //    Scalar_<double> sum = Scalar_<double>(0,0,0) ;
 //    for(int i = 0; i < image.rows; i++)
	// {
	//     for(int j = 0; j < image.cols; j++)
	//         cout<<image[i][j]<<endl;
	// }
	n = image.size().height*image.size().width;
	cout<<n<<endl;
	s = 0;t = n+1;
	for (int i = 0; i<image.rows; i++)
	{
		for (int j = 0; j<image.cols; j++)
		{
			int x = image.at<Vec3b>(i, j)[0]; 
			int y = image.at<Vec3b>(i, j)[1];
			int z = image.at<Vec3b>(i, j)[2];
			int ai = sqrt(x*x+y*y+z*z);
			int bi = sqrt((x+y+z)*(x+y+z)/3);
			addedge(s,i*image.cols+j+1,ai);
		 	addedge(i*image.cols+j+1,t,bi);
			for (int k=0;k<4;k++)
			{
				int i_new = i+d[i];
				int j_new = j+d[j];
				if (image.rows<=i_new || image.cols<=j_new) 
					if (image.rows<0 || image.cols<0) continue;
				int x_new = image.at<Vec3b>(i_new, j_new)[0]; 
				int y_new = image.at<Vec3b>(i_new, j_new)[1];
				int z_new = image.at<Vec3b>(i_new, j_new)[2];
				int diff = sqrt((x-x_new)*(x-x_new)+(y-y_new)*(y-y_new)+(z-z_new)*(z-z_new));
				diff = INF - diff;
				addedge(i*image.cols+j+1,i_new*image.cols+j_new+1,diff);
				addedge(i_new*image.cols+j_new+1,i*image.cols+j+1,diff);
			}
		}
	}
	max_flow();
	printf("%d\n",ans);
	bfs();
	for (int i=1;i<=n;i++)
	{
		int x = i/image.cols;
		int y = i%image.cols;
		if (!vis[i]) 
			for (int k=0;k<3;k++)
				image.at<Vec3b>(x, y)[k] = 0;
		else
			for (int k=0;k<3;k++)
				image.at<Vec3b>(x, y)[k] = 255;
	}
    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image);
    waitKey(0);
    return 0;
}